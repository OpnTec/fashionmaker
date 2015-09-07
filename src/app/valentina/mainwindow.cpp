/************************************************************************
 **
 **  @file   mainwindow.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../vgeometry/vspline.h"
#include "exception/vexceptionobjecterror.h"
#include "exception/vexceptionconversionerror.h"
#include "exception/vexceptionemptyparameter.h"
#include "exception/vexceptionwrongid.h"
#include "exception/vexceptionundo.h"
#include "version.h"
#include "xml/vstandardmeasurements.h"
#include "xml/vindividualmeasurements.h"
#include "core/vapplication.h"
#include "../vmisc/undoevent.h"
#include "../vmisc/vsettings.h"
#include "undocommands/renamepp.h"
#include "core/vtooloptionspropertybrowser.h"
#include "options.h"
#include "../ifc/xml/vpatternconverter.h"
#include "../vmisc/logging.h"
#include "../vformat/vmeasurements.h"
#include "../ifc/xml/vvstconverter.h"
#include "../ifc/xml/vvitconverter.h"

#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>
#include <QShowEvent>
#include <QScrollBar>
#include <QFileDialog>
#include <QSourceLocation>
#include <QUndoStack>
#include <QAction>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <QtGlobal>
#include <QDesktopWidget>
#include <QDesktopServices>
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
#   include <QLockFile>
#endif
#include <chrono>
#include <thread>

Q_LOGGING_CATEGORY(vMainWindow, "v.mainwindow")

#define OUT_FILE_ERROR vStdErr() << tr("File error: ") << e.ErrorMessage() << e.DetailedInformation() << "\n"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MainWindow constructor.
 * @param parent parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    :MainWindowsNoGUI(parent), ui(new Ui::MainWindow), watcher(new QFileSystemWatcher(this)), currentTool(Tool::Arrow),
      lastUsedTool(Tool::Arrow), sceneDraw(nullptr), sceneDetails(nullptr),
      mouseCoordinate(nullptr), helpLabel(nullptr), isInitialized(false), mChanges(false), dialogTable(nullptr),
      dialogTool(nullptr),
      dialogHistory(nullptr), comboBoxDraws(nullptr), mode(Draw::Calculation), currentDrawIndex(0),
      currentToolBoxIndex(0), drawMode(true), recentFileActs(),
      separatorAct(nullptr),
      leftGoToStage(nullptr), rightGoToStage(nullptr), autoSaveTimer(nullptr), guiEnabled(true),
      gradationHeights(nullptr), gradationSizes(nullptr),
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
      toolOptions(nullptr), lock(nullptr)
#else
      toolOptions(nullptr)
#endif
{
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = nullptr;
    }

    CreateActions();
    CreateMenus();
    ToolBarDraws();
    ToolBarStages();
    InitToolButtons();
    InitScenes();

    helpLabel = new QLabel(QObject::tr("Create new pattern piece to start working."));
    ui->statusBar->addWidget(helpLabel);

    ToolBarTools();

    doc = new VPattern(pattern, &mode, sceneDraw, sceneDetails);
    connect(doc, &VPattern::ClearMainWindow, this, &MainWindow::Clear);
    connect(doc, &VPattern::patternChanged, this, &MainWindow::PatternWasModified);
    connect(doc, &VPattern::UndoCommand, this, &MainWindow::FullParseFile);
    connect(doc, &VPattern::SetEnabledGUI, this, &MainWindow::SetEnabledGUI);
    connect(doc, &VPattern::CheckLayout, this, &MainWindow::Layout);
    connect(doc, &VPattern::SetCurrentPP, this, &MainWindow::GlobalChangePP);
    qApp->setCurrentDocument(doc);

    connect(qApp->getUndoStack(), &QUndoStack::cleanChanged, this, &MainWindow::PatternWasModified);

    InitAutoSave();

    ui->toolBox->setCurrentIndex(0);

    ReadSettings();
    PropertyBrowser();

    setCurrentFile("");
    WindowsLocale();

#if defined(Q_OS_MAC)
    // On Mac deafault icon size is 32x32.
    ui->toolBarArrows->setIconSize(QSize(24, 24));
    ui->toolBarDraws->setIconSize(QSize(24, 24));
    ui->toolBarOption->setIconSize(QSize(24, 24));
    ui->toolBarStages->setIconSize(QSize(24, 24));
    ui->toolBarTools->setIconSize(QSize(24, 24));
#endif

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &MainWindow::ShowPaper);
    ui->dockWidgetLayoutPages->setVisible(false);

    connect(watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::MeasurementsChanged);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionNewPP add to scene new pattern piece.
 */
void MainWindow::NewPP()
{
    qCDebug(vMainWindow, "New PP.");
    QString patternPieceName = QString(tr("Pattern piece %1")).arg(comboBoxDraws->count()+1);
    qCDebug(vMainWindow, "Generated PP name: %s", patternPieceName.toUtf8().constData());

    qCDebug(vMainWindow, "PP count %d", comboBoxDraws->count());
    patternPieceName = PatternPieceName(patternPieceName);
    qCDebug(vMainWindow, "PP name: %s", patternPieceName.toUtf8().constData());
    if (patternPieceName.isEmpty())
    {
        qCDebug(vMainWindow, "Name empty.");
        return;
    }

    AddPP(patternPieceName);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::AddPP(const QString &PPName)
{
    if (doc->appendPP(PPName) == false)
    {
        qCDebug(vMainWindow, "Error creating pattern piece with the name %s.", PPName.toUtf8().constData());
        return;
    }
    comboBoxDraws->blockSignals(true);
    comboBoxDraws->addItem(PPName);

    pattern->ClearGObjects();
    //Create single point
    ui->view->itemClicked(nullptr);//hide options previous tool
    const QString label = doc->GenerateLabel(LabelType::NewPatternPiece);
    const quint32 id = pattern->AddGObject(new VPointF(30+comboBoxDraws->count()*5, 40, label, 5, 10));
    VToolBasePoint *spoint = new VToolBasePoint(doc, pattern, id, Source::FromGui, PPName);
    sceneDraw->addItem(spoint);
    ui->view->itemClicked(spoint);

    connect(spoint, &VToolSinglePoint::ChoosedTool, sceneDraw, &VMainGraphicsScene::ChoosedItem);
    connect(sceneDraw, &VMainGraphicsScene::DisableItem, spoint, &VToolBasePoint::Disable);
    connect(sceneDraw, &VMainGraphicsScene::NewFactor, spoint, &VToolBasePoint::SetFactor);
    connect(sceneDraw, &VMainGraphicsScene::EnableToolMove, spoint, &VToolBasePoint::EnableToolMove);

    QHash<quint32, VDataTool*>* tools = doc->getTools();
    SCASSERT(tools != nullptr);
    tools->insert(id, spoint);
    VDrawTool::AddRecord(id, Tool::BasePoint, doc);
    SetEnableTool(true);
    SetEnableWidgets(true);

    const qint32 index = comboBoxDraws->findText(PPName);
    if ( index != -1 )
    { // -1 for not found
        comboBoxDraws->setCurrentIndex(index);
    }
    else
    {
        comboBoxDraws->setCurrentIndex(0);
    }
    comboBoxDraws->blockSignals(false);

    ui->actionNewDraw->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitScenes()
{
    sceneDraw = new VMainGraphicsScene();
    currentScene = sceneDraw;
    qApp->setCurrentScene(currentScene);
    connect(this, &MainWindow::EnableItemMove, sceneDraw, &VMainGraphicsScene::EnableItemMove);
    connect(sceneDraw, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    sceneDetails = new VMainGraphicsScene();
    connect(sceneDetails, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);

    ui->view->setScene(currentScene);

    sceneDraw->setTransform(ui->view->transform());
    sceneDetails->setTransform(ui->view->transform());

    connect(ui->view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(12);
    ui->view->setSizePolicy(policy);
    qApp->setSceneView(ui->view);
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::LoadMeasurements(const QString &path)
{
    if (path.isEmpty())
    {
        return false;
    }

    VMeasurements *m = nullptr;
    try
    {
        m = new VMeasurements(pattern);
        m->setXMLContent(path);

        if (m->Type() == MeasurementsType::Unknown)
        {
            VException e("Measurement file has unknown format.");
            throw e;
        }

        if (m->Type() == MeasurementsType::Standard)
        {
            VVSTConverter converter(path);
            converter.Convert();

            VDomDocument::ValidateXML(VVSTConverter::CurrentSchema, path);
        }
        else
        {
            VVITConverter converter(path);
            converter.Convert();

            VDomDocument::ValidateXML(VVITConverter::CurrentSchema, path);
        }

        m->setXMLContent(path);// Read again after conversion

        const QStringList mList = m->ListAll();
        const QStringList pList = doc->ListMeasurements();

        const QSet<QString> match = pList.toSet().subtract(mList.toSet());
        if (not match.isEmpty())
        {
            VException e("Measurement file doesn't include all required measurements.");
            e.AddMoreInformation(QString("Please, additionaly provide: %1")
                                 .arg(QStringList(match.toList()).join(", ")));
            throw e;
        }

        if (m->Type() == MeasurementsType::Standard)
        {
            if (m->MUnit() == Unit::Inch)
            {
                QMessageBox::critical(this, tr("Wrong units."),
                                      tr("Application doesn't support standard table with inches."));
                qCDebug(vMainWindow, "Application doesn't support standard table with inches.");
                return false;
            }
            m->SetDataSize();
            m->SetDataHeight();
        }

        qApp->setPatternType(m->Type());
        ToolBarOption();
        pattern->ClearVariables(VarType::Measurement);
        m->ReadMeasurements();
        delete m;
    }
    catch (VException &e)
    {
        if (qApp->CheckGUI())
        {
            e.CriticalMessageBox(tr("File error."), this);
        }
        else
        {
            OUT_FILE_ERROR;
        }
        delete m;
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToggleMSync(bool toggle)
{
    if (mChanges)
    {
        ui->actionSyncMeasurements->setEnabled(toggle);
    }
    else
    {
        ui->actionSyncMeasurements->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::RelativeMPath(const QString &patternPath, const QString &absoluteMPath) const
{
    if (patternPath.isEmpty())
    {
        return absoluteMPath;
    }

    if (absoluteMPath.isEmpty())
    {
        return absoluteMPath;
    }

    QDir dir(QFileInfo(patternPath).absoluteDir());
    return dir.relativeFilePath(absoluteMPath);
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::AbsoluteMPath(const QString &patternPath, const QString &relativeMPath) const
{
    if (patternPath.isEmpty())
    {
        return relativeMPath;
    }
    else
    {
        return QFileInfo(QFileInfo(patternPath).absoluteDir(), relativeMPath).absoluteFilePath();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief OptionDraw help change name of pattern piece.
 */
void MainWindow::DrawOption()
{
    const QString activDraw = doc->GetNameActivPP();
    const QString nameDraw = PatternPieceName(activDraw);
    if (nameDraw.isEmpty())
    {
        return;
    }
    RenamePP *renamePP = new RenamePP(doc, nameDraw, comboBoxDraws);
    qApp->getUndoStack()->push(renamePP);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetToolButton set tool and show dialog.
 * @param checked true if tool button checked.
 * @param t tool type.
 * @param cursor path tool cursor icon.
 * @param toolTip first tooltipe.
 * @param closeDialogSlot function what handle after close dialog.
 */
template <typename Dialog, typename Func>
void MainWindow::SetToolButton(bool checked, Tool t, const QString &cursor, const QString &toolTip,
                               Func closeDialogSlot)
{
    if (checked)
    {
        CancelTool();
        emit EnableItemMove(false);
        currentTool = lastUsedTool = t;
        QPixmap pixmap(cursor);
        QCursor cur(pixmap, 2, 3);
        ui->view->setCursor(cur);
        helpLabel->setText(toolTip);
        ui->view->setShowToolOptions(false);
        dialogTool = new Dialog(pattern, 0, this);

        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr);

        connect(scene, &VMainGraphicsScene::ChoosedObject, dialogTool, &DialogTool::ChosenObject);
        connect(dialogTool, &DialogTool::DialogClosed, this, closeDialogSlot);
        connect(dialogTool, &DialogTool::ToolTip, this, &MainWindow::ShowToolTip);
        ui->view->itemClicked(nullptr);
    }
    else
    {
        if (QToolButton *tButton = qobject_cast< QToolButton * >(this->sender()))
        {
            tButton->setChecked(true);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Dialog, typename Func, typename Func2>
/**
 * @brief SetToolButtonWithApply set tool and show dialog.
 * @param checked true if tool button checked.
 * @param t tool type.
 * @param cursor path tool cursor icon.
 * @param toolTip first tooltipe.
 * @param closeDialogSlot function to handle close of dialog.
 * @param applyDialogSlot function to handle apply in dialog.
 */
void MainWindow::SetToolButtonWithApply(bool checked, Tool t, const QString &cursor, const QString &toolTip,
                               Func closeDialogSlot, Func2 applyDialogSlot)
{
    if (checked)
    {
        CancelTool();
        emit EnableItemMove(false);
        currentTool = lastUsedTool = t;
        QPixmap pixmap(cursor);
        QCursor cur(pixmap, 2, 3);
        ui->view->setCursor(cur);
        ui->view->setShowToolOptions(false);
        helpLabel->setText(toolTip);
        dialogTool = new Dialog(pattern, NULL_ID, this);

        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr);

        connect(scene, &VMainGraphicsScene::ChoosedObject, dialogTool, &DialogTool::ChosenObject);
        connect(dialogTool, &DialogTool::DialogClosed, this, closeDialogSlot);
        connect(dialogTool, &DialogTool::DialogApplied, this, applyDialogSlot);
        connect(dialogTool, &DialogTool::ToolTip, this, &MainWindow::ShowToolTip);
        connect(ui->view, &VMainGraphicsView::MouseRelease, this, &MainWindow::ClickEndVisualization);
        ui->view->itemClicked(nullptr);
    }
    else
    {
        if (QToolButton *tButton = qobject_cast< QToolButton * >(this->sender()))
        {
            tButton->setChecked(true);
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialog handle close dialog
 * @param result result working dialog.
 */
template <typename DrawTool>
void MainWindow::ClosedDialog(int result)
{
    SCASSERT(dialogTool != nullptr);
    if (result == QDialog::Accepted)
    {
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr);

        QGraphicsItem *tool = dynamic_cast<QGraphicsItem *>(DrawTool::Create(dialogTool, scene, doc, pattern));
        ui->view->itemClicked(tool);
    }
    ArrowTool();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogWithApply handle close dialog that has apply button
 * @param result result working dialog.
 */
template <typename DrawTool>
void MainWindow::ClosedDialogWithApply(int result)
{
    SCASSERT(dialogTool != nullptr);
    if (result == QDialog::Accepted)
    {
        // Only create tool if not already created with apply
        if (dialogTool->GetAssociatedTool() == nullptr)
        {
            VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
            SCASSERT(scene != nullptr);

            dialogTool->SetAssociatedTool(
                    dynamic_cast<VAbstractTool * > (DrawTool::Create(dialogTool, scene, doc, pattern)));
        }
        else
        { // Or update associated tool with data
            VDrawTool * vtool= static_cast<VDrawTool *>(dialogTool->GetAssociatedTool());
            vtool->FullUpdateFromGuiApply();
        }
    }
    QGraphicsItem *tool = dynamic_cast<QGraphicsItem *>(dialogTool->GetAssociatedTool());
    ui->view->itemClicked(tool);
    if (dialogTool->GetAssociatedTool() != nullptr)
    {
        VDrawTool *vtool= static_cast<VDrawTool *>(dialogTool->GetAssociatedTool());
        vtool->DialogLinkDestroy();
    }
    ArrowTool();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ApplyDialog handle apply in dialog
 */
template <typename DrawTool>
void MainWindow::ApplyDialog()
{
    SCASSERT(dialogTool != nullptr);

    // Only create tool if not already created with apply
    if (dialogTool->GetAssociatedTool() == nullptr)
    {
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr);

        dialogTool->SetAssociatedTool(
                static_cast<VAbstractTool * > (DrawTool::Create(dialogTool, scene, doc, pattern)));
    }
    else
    { // Or update associated tool with data
        VDrawTool * vtool= static_cast<VDrawTool *>(dialogTool->GetAssociatedTool());
        SCASSERT(vtool != nullptr);
        vtool->FullUpdateFromGuiApply();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolEndLine handler tool endLine.
 * @param checked true - button checked.
 */
void MainWindow::ToolEndLine(bool checked)
{
    SetToolButtonWithApply<DialogEndLine>(checked, Tool::EndLine, ":/cursor/endline_cursor.png", tr("Select point"),
                                          &MainWindow::ClosedDialogWithApply<VToolEndLine>,
                                          &MainWindow::ApplyDialog<VToolEndLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolLine handler tool line.
 * @param checked true - button checked.
 */
void MainWindow::ToolLine(bool checked)
{
    SetToolButtonWithApply<DialogLine>(checked, Tool::Line, ":/cursor/line_cursor.png", tr("Select first point"),
                                       &MainWindow::ClosedDialogWithApply<VToolLine>,
                                       &MainWindow::ApplyDialog<VToolLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolAlongLine handler tool alongLine.
 * @param checked true - button checked.
 */
void MainWindow::ToolAlongLine(bool checked)
{
    SetToolButtonWithApply<DialogAlongLine>(checked, Tool::AlongLine, ":/cursor/alongline_cursor.png",
                                            tr("Select point"), &MainWindow::ClosedDialogWithApply<VToolAlongLine>,
                                            &MainWindow::ApplyDialog<VToolAlongLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolShoulderPoint handler tool shoulderPoint.
 * @param checked true - button checked.
 */
void MainWindow::ToolShoulderPoint(bool checked)
{
    SetToolButtonWithApply<DialogShoulderPoint>(checked, Tool::ShoulderPoint, ":/cursor/shoulder_cursor.png",
                                                tr("Select point"),
                                                &MainWindow::ClosedDialogWithApply<VToolShoulderPoint>,
                                                &MainWindow::ApplyDialog<VToolShoulderPoint>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolNormal handler tool normal.
 * @param checked true - button checked.
 */
void MainWindow::ToolNormal(bool checked)
{
    SetToolButtonWithApply<DialogNormal>(checked, Tool::Normal, ":/cursor/normal_cursor.png",
                                         tr("Select first point of line"),
                                         &MainWindow::ClosedDialogWithApply<VToolNormal>,
                                         &MainWindow::ApplyDialog<VToolNormal>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBisector handler tool bisector.
 * @param checked true - button checked.
 */
void MainWindow::ToolBisector(bool checked)
{
    SetToolButtonWithApply<DialogBisector>(checked, Tool::Bisector, ":/cursor/bisector_cursor.png",
                                           tr("Select first point of angle"),
                                           &MainWindow::ClosedDialogWithApply<VToolBisector>,
                                           &MainWindow::ApplyDialog<VToolBisector>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolLineIntersect handler tool lineIntersect.
 * @param checked true - button checked.
 */
void MainWindow::ToolLineIntersect(bool checked)
{
    SetToolButtonWithApply<DialogLineIntersect>(checked, Tool::LineIntersect, ":/cursor/intersect_cursor.png",
                                                tr("Select first point of first line"),
                                                &MainWindow::ClosedDialogWithApply<VToolLineIntersect>,
                                                &MainWindow::ApplyDialog<VToolLineIntersect>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolSpline handler tool spline.
 * @param checked true - button checked.
 */
void MainWindow::ToolSpline(bool checked)
{
    SetToolButtonWithApply<DialogSpline>(checked, Tool::Spline, ":/cursor/spline_cursor.png",
                                         tr("Select first point curve"),
                                         &MainWindow::ClosedDialogWithApply<VToolSpline>,
                                         &MainWindow::ApplyDialog<VToolSpline>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutSpline handler tool CutSpline.
 * @param checked true - button is checked
 */
void MainWindow::ToolCutSpline(bool checked)
{
    SetToolButtonWithApply<DialogCutSpline>(checked, Tool::CutSpline, ":/cursor/spline_cut_point_cursor.png",
                                            tr("Select simple curve"),
                                            &MainWindow::ClosedDialogWithApply<VToolCutSpline>,
                                            &MainWindow::ApplyDialog<VToolCutSpline>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolArc handler tool arc.
 * @param checked true - button checked.
 */
void MainWindow::ToolArc(bool checked)
{
    SetToolButtonWithApply<DialogArc>(checked, Tool::Arc, ":/cursor/arc_cursor.png",
                                      tr("Select point of center of arc"), &MainWindow::ClosedDialogWithApply<VToolArc>,
                                      &MainWindow::ApplyDialog<VToolArc>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolSplinePath handler tool splinePath.
 * @param checked true - button checked.
 */
void MainWindow::ToolSplinePath(bool checked)
{
    SetToolButtonWithApply<DialogSplinePath>(checked, Tool::SplinePath, ":/cursor/splinepath_cursor.png",
                                             tr("Select point of curve path"),
                                             &MainWindow::ClosedDialogWithApply<VToolSplinePath>,
                                             &MainWindow::ApplyDialog<VToolSplinePath>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutSplinePath handler tool CutSplinePath.
 * @param checked true - button is checked
 */
void MainWindow::ToolCutSplinePath(bool checked)
{
    SetToolButtonWithApply<DialogCutSplinePath>(checked, Tool::CutSplinePath,
                                                ":/cursor/splinepath_cut_point_cursor.png", tr("Select curve path"),
                                                &MainWindow::ClosedDialogWithApply<VToolCutSplinePath>,
                                                &MainWindow::ApplyDialog<VToolCutSplinePath>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolPointOfContact handler tool pointOfContact.
 * @param checked true - button checked.
 */
void MainWindow::ToolPointOfContact(bool checked)
{
    SetToolButtonWithApply<DialogPointOfContact>(checked, Tool::PointOfContact, ":/cursor/pointcontact_cursor.png",
                                                 tr("Select first point of line"),
                                                 &MainWindow::ClosedDialogWithApply<VToolPointOfContact>,
                                                 &MainWindow::ApplyDialog<VToolPointOfContact>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolDetail handler tool detail.
 * @param checked true - button checked.
 */
void MainWindow::ToolDetail(bool checked)
{
    SetToolButton<DialogDetail>(checked, Tool::Detail, "://cursor/new_detail_cursor.png",
                                tr("Select points, arcs, curves clockwise."), &MainWindow::ClosedDialogDetail);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogDetail actions after closing DialogDetail.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogDetail(int result)
{
    if (result == QDialog::Accepted)
    {
        VToolDetail::Create(dialogTool, sceneDetails, doc, pattern);
    }
    ArrowTool();
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolHeight handler tool height.
 * @param checked true - button checked.
 */
void MainWindow::ToolHeight(bool checked)
{
    SetToolButtonWithApply<DialogHeight>(checked, Tool::Height, ":/cursor/height_cursor.png", tr("Select base point"),
                                         &MainWindow::ClosedDialogWithApply<VToolHeight>,
                                         &MainWindow::ApplyDialog<VToolHeight>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolTriangle handler tool triangle.
 * @param checked true - button checked.
 */
void MainWindow::ToolTriangle(bool checked)
{
    SetToolButtonWithApply<DialogTriangle>(checked, Tool::Triangle, ":/cursor/triangle_cursor.png",
                                           tr("Select first point of axis"),
                                           &MainWindow::ClosedDialogWithApply<VToolTriangle>,
                                           &MainWindow::ApplyDialog<VToolTriangle>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolPointOfIntersection handler tool pointOfIntersection.
 * @param checked true - button checked.
 */
void MainWindow::ToolPointOfIntersection(bool checked)
{
    SetToolButtonWithApply<DialogPointOfIntersection>(checked, Tool::PointOfIntersection,
                                                      ":/cursor/pointofintersect_cursor.png",
                                                      tr("Select point for X value (vertical)"),
                                                      &MainWindow::ClosedDialogWithApply<VToolPointOfIntersection>,
                                                      &MainWindow::ApplyDialog<VToolPointOfIntersection>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolUnionDetails handler tool unionDetails.
 * @param checked true - button checked.
 */
void MainWindow::ToolUnionDetails(bool checked)
{
    SetToolButton<DialogUnionDetails>(checked, Tool::UnionDetails, ":/cursor/union_cursor.png",
                                      tr("Select detail"), &MainWindow::ClosedDialogUnionDetails);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogUnionDetails actions after closing DialogUnionDetails.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogUnionDetails(int result)
{
    ClosedDialog<VToolUnionDetails>(result);
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutArc handler tool cutArc.
 * @param checked true - button checked.
 */
void MainWindow::ToolCutArc(bool checked)
{
    SetToolButtonWithApply<DialogCutArc>(checked, Tool::CutArc, ":/cursor/arc_cut_cursor.png",
                                         tr("Select arc"), &MainWindow::ClosedDialogWithApply<VToolCutArc>,
                                         &MainWindow::ApplyDialog<VToolCutArc>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolLineIntersectAxis(bool checked)
{
    SetToolButtonWithApply<DialogLineIntersectAxis>(checked, Tool::LineIntersectAxis,
                                                    ":/cursor/line_intersect_axis_cursor.png",
                                                    tr("Select first point of line"),
                                                    &MainWindow::ClosedDialogWithApply<VToolLineIntersectAxis>,
                                                    &MainWindow::ApplyDialog<VToolLineIntersectAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolCurveIntersectAxis(bool checked)
{
    SetToolButtonWithApply<DialogCurveIntersectAxis>(checked, Tool::CurveIntersectAxis,
                                                     ":/cursor/curve_intersect_axis_cursor.png",
                                                     tr("Select curve"),
                                                     &MainWindow::ClosedDialogWithApply<VToolCurveIntersectAxis>,
                                                     &MainWindow::ApplyDialog<VToolCurveIntersectAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointOfIntersectionArcs(bool checked)
{
    SetToolButtonWithApply<DialogPointOfIntersectionArcs>(checked, Tool::PointOfIntersectionArcs,
                                                          "://cursor/point_of_intersection_arcs.png",
                                                          tr("Select first an arc"),
                                                       &MainWindow::ClosedDialogWithApply<VToolPointOfIntersectionArcs>,
                                                          &MainWindow::ApplyDialog<VToolPointOfIntersectionArcs>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointOfIntersectionCircles(bool checked)
{
    SetToolButtonWithApply<DialogPointOfIntersectionCircles>(checked, Tool::PointOfIntersectionCircles,
                                                             "://cursor/point_of_intersection_circles.png",
                                                             tr("Select first circle center "),
                                                    &MainWindow::ClosedDialogWithApply<VToolPointOfIntersectionCircles>,
                                                             &MainWindow::ApplyDialog<VToolPointOfIntersectionCircles>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointFromCircleAndTangent(bool checked)
{
    SetToolButtonWithApply<DialogPointFromCircleAndTangent>(checked, Tool::PointFromCircleAndTangent,
                                                            "://cursor/point_from_circle_and_tangent_cursor.png",
                                                            tr("Select point on tangent "),
                                                    &MainWindow::ClosedDialogWithApply<VToolPointFromCircleAndTangent>,
                                                            &MainWindow::ApplyDialog<VToolPointFromCircleAndTangent>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointFromArcAndTangent(bool checked)
{
    SetToolButtonWithApply<DialogPointFromArcAndTangent>(checked, Tool::PointFromArcAndTangent,
                                                         "://cursor/point_from_arc_and_tangent_cursor.png",
                                                         tr("Select point on tangent "),
                                                        &MainWindow::ClosedDialogWithApply<VToolPointFromArcAndTangent>,
                                                         &MainWindow::ApplyDialog<VToolPointFromArcAndTangent>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolArcWithLength(bool checked)
{
    SetToolButtonWithApply<DialogArcWithLength>(checked, Tool::ArcWithLength,
                                                "://cursor/arc_with_length_cursor.png",
                                                tr("Select point of the center of the arc"),
                                                &MainWindow::ClosedDialogWithApply<VToolArcWithLength>,
                                                &MainWindow::ApplyDialog<VToolArcWithLength>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolTrueDarts(bool checked)
{
    SetToolButtonWithApply<DialogTrueDarts>(checked, Tool::TrueDarts,
                                                "://cursor/true_darts_cursor.png",
                                                tr("Select the first base line point"),
                                                &MainWindow::ClosedDialogWithApply<VToolTrueDarts>,
                                            &MainWindow::ApplyDialog<VToolTrueDarts>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief About show widows about.
 */
void MainWindow::About()
{
    DialogAboutApp * about_dialog = new DialogAboutApp(this);
    about_dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    about_dialog->show();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AboutQt show widows aboutQt.
 */
void MainWindow::AboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool  highlight tool.Tip show tools tooltip.
 * @param toolTip tooltip text.
 */
void MainWindow::ShowToolTip(const QString &toolTip)
{
    helpLabel->setText(toolTip);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::OpenRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        const QString filePath = action->data().toString();
        if (filePath.isEmpty() == false)
        {
            LoadPattern(filePath);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::PatternProperties()
{
    DialogPatternProperties proper(doc, this);
    connect(&proper, &DialogPatternProperties::UpdateGradation, this, &MainWindow::UpdateGradation);
    proper.exec();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::EditPatternCode()
{
    DialogPatternXmlEdit *Pattern = new DialogPatternXmlEdit (this, doc);
    Pattern->setAttribute(Qt::WA_DeleteOnClose, true);
    Pattern->show();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showEvent handle after show window.
 * @param event show event.
 */
void MainWindow::showEvent( QShowEvent *event )
{
    QMainWindow::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }

    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    MinimumScrollBar();

    isInitialized = true;//first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeEvent handle after close window.
 * @param event close event.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    qCDebug(vMainWindow, "Closing main window");
    if (MaybeSave())
    {
        FileClosedCorrect();

        event->accept();
        qApp->closeAllWindows();
    }
    else
    {
        qCDebug(vMainWindow, "Closing canceled.");
        event->ignore();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::customEvent(QEvent *event)
{
    if (event->type() == UNDO_EVENT)
    {
        qApp->getUndoStack()->undo();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::CleanLayout()
{
    qDeleteAll (scenes);
    scenes.clear();
    shadows.clear();
    papers.clear();
    ui->listWidget->clear();
    SetLayoutModeActions(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::PrepareSceneList()
{
    for (int i=1; i<=scenes.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(ScenePreview(i-1), QString::number(i));
        ui->listWidget->addItem(item);
    }

    if (not scenes.isEmpty())
    {
        ui->listWidget->setCurrentRow(0);
        SetLayoutModeActions(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::LoadIndividual()
{
    const QString filter = tr("Individual measurements (*.vit);;Standard measurements (*.vst)");
    //Use standard path to standard measurements
    const QString path = qApp->ValentinaSettings()->GetPathStandardMeasurements();
    const QString mPath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);

    if (not mPath.isEmpty())
    {
        if (LoadMeasurements(mPath))
        {
            if (not doc->MPath().isEmpty())
            {
                watcher->removePath(AbsoluteMPath(curFile, doc->MPath()));
            }
            doc->SetPath(RelativeMPath(curFile, mPath));
            watcher->addPath(mPath);
            PatternWasModified(false);
            ui->actionShowM->setEnabled(true);
            helpLabel->setText(tr("Measurements loaded"));
            doc->LiteParseTree(Document::LiteParse);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::LoadStandard()
{
    const QString filter = tr("Individual measurements (*.vit);;Standard measurements (*.vst)");
    //Use standard path to individual measurements
    const QString path = qApp->ValentinaSettings()->GetPathIndividualMeasurements();
    const QString mPath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);

    if (not mPath.isEmpty())
    {
        if(LoadMeasurements(mPath))
        {
            if (not doc->MPath().isEmpty())
            {
                watcher->removePath(AbsoluteMPath(curFile, doc->MPath()));
            }
            doc->SetPath(RelativeMPath(curFile, mPath));
            watcher->addPath(mPath);
            PatternWasModified(false);
            ui->actionShowM->setEnabled(true);
            helpLabel->setText(tr("Measurements loaded"));
            doc->LiteParseTree(Document::LiteParse);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::CreateMeasurements()
{
    const QString tape = qApp->TapeFilePath();
    const QString workingDirectory = QFileInfo(tape).absoluteDir().absolutePath();
    QProcess::startDetached(tape, QStringList(), workingDirectory);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowMeasurements()
{
    if (not doc->MPath().isEmpty())
    {
        const QString absoluteMPath = AbsoluteMPath(curFile, doc->MPath());

        QString run;
        if (qApp->patternType() == MeasurementsType::Standard)
        {
            run = QString("\"%1\" \"%2\" -u %3 -h %4 -s %5").arg(qApp->TapeFilePath()).arg(absoluteMPath)
                    .arg(VDomDocument::UnitsToStr(qApp->patternUnit()))
                    .arg(static_cast<int>(pattern->height()))
                    .arg(static_cast<int>(pattern->size()));
        }
        else
        {
            run = QString("\"%1\" \"%2\" -u %3").arg(qApp->TapeFilePath()).arg(absoluteMPath)
                    .arg(VDomDocument::UnitsToStr(qApp->patternUnit()));
        }
        QProcess::startDetached(run);
    }
    else
    {
        ui->actionShowM->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::MeasurementsChanged(const QString &path)
{
    mChanges = false;
    QFileInfo checkFile(path);
    if (checkFile.exists())
    {
        mChanges = true;
    }
    else
    {
        for(int i=0; i<=1000; i=i+10)
        {
            if (checkFile.exists())
            {
                mChanges = true;
                break;
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    ToggleMSync(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SyncMeasurements()
{
    if (mChanges)
    {
        if(LoadMeasurements(doc->MPath()))
        {
            if (not watcher->files().contains(doc->MPath()))
            {
                watcher->addPath(doc->MPath());
            }
            helpLabel->setText(tr("Measurements updated"));
            doc->LiteParseTree(Document::LiteParse);
            mChanges = false;
        }
    }

    ToggleMSync(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBarOption enable option toolbar.
 */
void MainWindow::ToolBarOption()
{
    ui->toolBarOption->clear();
    if (qApp->patternType() == MeasurementsType::Standard)
    {
        const QStringList listHeights = VMeasurement::ListHeights(doc->GetGradationHeights(), qApp->patternUnit());
        const QStringList listSizes = VMeasurement::ListSizes(doc->GetGradationSizes(), qApp->patternUnit());

        gradationHeights = SetGradationList(tr("Height: "), listHeights);
        SetDefaultHeight(static_cast<int>(pattern->height()));
        connect(gradationHeights, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::ChangedHeight);

        gradationSizes = SetGradationList(tr("Size: "), listSizes);
        SetDefaultSize(static_cast<int>(pattern->size()));
        connect(gradationSizes, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::ChangedSize);

        ui->toolBarOption->addSeparator();
    }

    mouseCoordinate = new QLabel(QString("0, 0 (%1)").arg(doc->UnitsToStr(qApp->patternUnit())));
    ui->toolBarOption->addWidget(mouseCoordinate);
}

//---------------------------------------------------------------------------------------------------------------------
QComboBox *MainWindow::SetGradationList(const QString &label, const QStringList &list)
{
    ui->toolBarOption->addWidget(new QLabel(label, this));

    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItems(list);
    ui->toolBarOption->addWidget(comboBox);

    return comboBox;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetDefaultHeight set base height in combobox.
 * @param value [in] height value in pattern units.
 */
void MainWindow::SetDefaultHeight(int value)
{
    const qint32 index = gradationHeights->findText(QString("%1").arg(value));
    if (index != -1)
    {
        gradationHeights->setCurrentIndex(index);
    }
    else
    {
        pattern->SetHeight(gradationHeights->currentText().toInt());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetDefaultSize set base size in combobox.
 * @param value [in] size value in pattern units.
 */
void MainWindow::SetDefaultSize(int value)
{
    const qint32 index = gradationSizes->findText(QString("%1").arg(value));
    if (index != -1)
    {
        gradationSizes->setCurrentIndex(index);
    }
    else
    {
        pattern->SetSize(gradationSizes->currentText().toInt());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolBarStages()
{
    leftGoToStage = new QLabel(this);
    leftGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_left_to_right_arrow.png"));
    ui->toolBarStages->insertWidget(ui->actionDetails, leftGoToStage);

    rightGoToStage = new QLabel(this);
    rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));
    ui->toolBarStages->insertWidget(ui->actionLayout, rightGoToStage);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBarDraws enable draw toolbar.
 */
void MainWindow::ToolBarDraws()
{
    ui->toolBarDraws->addWidget(new QLabel(tr("Pattern Piece: ")));

    // By using Qt UI Designer we can't add QComboBox to toolbar
    comboBoxDraws = new QComboBox;
    ui->toolBarDraws->addWidget(comboBoxDraws);
    comboBoxDraws->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    comboBoxDraws->setEnabled(false);
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::currentPPChanged);

    ui->toolBarDraws->addAction(ui->actionOptionDraw);
    connect(ui->actionOptionDraw, &QAction::triggered, this, &MainWindow::DrawOption);
}

void MainWindow::ToolBarTools()
{
    /*First we will try use Standard Shortcuts from Qt, but because keypad "-" and "+" not the same keys like in main
    keypad, shortcut Ctrl+"-" or "+" from keypad will not working with standard shortcut (QKeySequence::ZoomIn or
    QKeySequence::ZoomOut). For examle "+" is Qt::Key_Plus + Qt::KeypadModifier for keypad.
    Also for me don't work Qt:CTRL and work Qt::ControlModifier.*/

    QList<QKeySequence> zoomInShortcuts;
    zoomInShortcuts.append(QKeySequence(QKeySequence::ZoomIn));
    zoomInShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Plus + Qt::KeypadModifier));
    ui->actionZoomIn->setShortcuts(zoomInShortcuts);
    connect(ui->actionZoomIn, &QAction::triggered, ui->view, &VMainGraphicsView::ZoomIn);

    QList<QKeySequence> zoomOutShortcuts;
    zoomOutShortcuts.append(QKeySequence(QKeySequence::ZoomOut));
    zoomOutShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Minus + Qt::KeypadModifier));
    ui->actionZoomOut->setShortcuts(zoomOutShortcuts);
    connect(ui->actionZoomOut, &QAction::triggered, ui->view, &VMainGraphicsView::ZoomOut);

    QList<QKeySequence> zoomOriginalShortcuts;
    zoomOriginalShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_0));
    zoomOriginalShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_0 + Qt::KeypadModifier));
    ui->actionZoomOriginal->setShortcuts(zoomOriginalShortcuts);
    connect(ui->actionZoomOriginal, &QAction::triggered, ui->view, &VMainGraphicsView::ZoomOriginal);

    QList<QKeySequence> zoomFitBestShortcuts;
    zoomFitBestShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Equal));
    ui->actionZoomFitBest->setShortcuts(zoomFitBestShortcuts);
    connect(ui->actionZoomFitBest, &QAction::triggered, ui->view, &VMainGraphicsView::ZoomFitBest);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitToolButtons()
{
    connect(ui->toolButtonEndLine, &QToolButton::clicked, this, &MainWindow::ToolEndLine);
    connect(ui->toolButtonLine, &QToolButton::clicked, this, &MainWindow::ToolLine);
    connect(ui->toolButtonAlongLine, &QToolButton::clicked, this, &MainWindow::ToolAlongLine);
    connect(ui->toolButtonShoulderPoint, &QToolButton::clicked, this, &MainWindow::ToolShoulderPoint);
    connect(ui->toolButtonNormal, &QToolButton::clicked, this, &MainWindow::ToolNormal);
    connect(ui->toolButtonBisector, &QToolButton::clicked, this, &MainWindow::ToolBisector);
    connect(ui->toolButtonLineIntersect, &QToolButton::clicked, this, &MainWindow::ToolLineIntersect);
    connect(ui->toolButtonSpline, &QToolButton::clicked, this, &MainWindow::ToolSpline);
    connect(ui->toolButtonArc, &QToolButton::clicked, this, &MainWindow::ToolArc);
    connect(ui->toolButtonSplinePath, &QToolButton::clicked, this, &MainWindow::ToolSplinePath);
    connect(ui->toolButtonPointOfContact, &QToolButton::clicked, this, &MainWindow::ToolPointOfContact);
    connect(ui->toolButtonNewDetail, &QToolButton::clicked, this, &MainWindow::ToolDetail);
    connect(ui->toolButtonHeight, &QToolButton::clicked, this, &MainWindow::ToolHeight);
    connect(ui->toolButtonTriangle, &QToolButton::clicked, this, &MainWindow::ToolTriangle);
    connect(ui->toolButtonPointOfIntersection, &QToolButton::clicked, this, &MainWindow::ToolPointOfIntersection);
    connect(ui->toolButtonSplineCutPoint, &QToolButton::clicked, this, &MainWindow::ToolCutSpline);
    connect(ui->toolButtonSplinePathCutPoint, &QToolButton::clicked, this, &MainWindow::ToolCutSplinePath);
    connect(ui->toolButtonUnionDetails, &QToolButton::clicked, this, &MainWindow::ToolUnionDetails);
    connect(ui->toolButtonArcCutPoint, &QToolButton::clicked, this, &MainWindow::ToolCutArc);
    connect(ui->toolButtonLineIntersectAxis, &QToolButton::clicked, this, &MainWindow::ToolLineIntersectAxis);
    connect(ui->toolButtonCurveIntersectAxis, &QToolButton::clicked, this, &MainWindow::ToolCurveIntersectAxis);
    connect(ui->toolButtonArcIntersectAxis, &QToolButton::clicked, this, &MainWindow::ToolCurveIntersectAxis);
    connect(ui->toolButtonLayoutSettings, &QToolButton::clicked, this, &MainWindow::ToolLayoutSettings);
    connect(ui->toolButtonPointOfIntersectionArcs, &QToolButton::clicked, this,
            &MainWindow::ToolPointOfIntersectionArcs);
    connect(ui->toolButtonPointOfIntersectionCircles, &QToolButton::clicked, this,
            &MainWindow::ToolPointOfIntersectionCircles);
    connect(ui->toolButtonPointFromCircleAndTangent, &QToolButton::clicked, this,
            &MainWindow::ToolPointFromCircleAndTangent);
    connect(ui->toolButtonPointFromArcAndTangent, &QToolButton::clicked, this, &MainWindow::ToolPointFromArcAndTangent);
    connect(ui->toolButtonArcWithLength, &QToolButton::clicked, this, &MainWindow::ToolArcWithLength);
    connect(ui->toolButtonTrueDarts, &QToolButton::clicked, this, &MainWindow::ToolTrueDarts);

}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief currentPPChanged change active pattern piece.
 * @param index index in combobox.
 */
void MainWindow::currentPPChanged(int index)
{
    ChangePP(index);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseMove save mouse position and show user.
 * @param scenePos position mouse.
 */
void MainWindow::mouseMove(const QPointF &scenePos)
{
    QString string = QString("%1, %2 (%3)").arg(static_cast<qint32>(qApp->fromPixel(scenePos.x())))
                                      .arg(static_cast<qint32>(qApp->fromPixel(scenePos.y())))
                                      .arg(doc->UnitsToStr(qApp->patternUnit()));
    if (mouseCoordinate != nullptr)
    {
        mouseCoordinate->setText(string);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CancelTool cancel tool.
 */
void MainWindow::CancelTool()
{
    qCDebug(vMainWindow, "Canceling tool.");
    delete dialogTool;
    dialogTool = nullptr;
    qCDebug(vMainWindow, "Dialog closed.");
    switch ( currentTool )
    {
        case Tool::Arrow:
            ui->actionArrowTool->setChecked(false);
            helpLabel->setText("");
            ui->actionStopTool->setEnabled(true);
            return;
        case Tool::BasePoint:
            Q_UNREACHABLE();
            //Nothing to do here because we can't create this tool from main window.
            break;
        case Tool::EndLine:
            ui->toolButtonEndLine->setChecked(false);
            break;
        case Tool::Line:
            ui->toolButtonLine->setChecked(false);
            break;
        case Tool::AlongLine:
            ui->toolButtonAlongLine->setChecked(false);
            break;
        case Tool::ShoulderPoint:
            ui->toolButtonShoulderPoint->setChecked(false);
            break;
        case Tool::Normal:
            ui->toolButtonNormal->setChecked(false);
            break;
        case Tool::Bisector:
            ui->toolButtonBisector->setChecked(false);
            break;
        case Tool::LineIntersect:
            ui->toolButtonLineIntersect->setChecked(false);
            break;
        case Tool::Spline:
            ui->toolButtonSpline->setChecked(false);
            break;
        case Tool::Arc:
            ui->toolButtonArc->setChecked(false);
            break;
        case Tool::ArcWithLength:
            ui->toolButtonArcWithLength->setChecked(false);
            break;
        case Tool::SplinePath:
            ui->toolButtonSplinePath->setChecked(false);
            break;
        case Tool::PointOfContact:
            ui->toolButtonPointOfContact->setChecked(false);
            break;
        case Tool::Detail:
            ui->toolButtonNewDetail->setChecked(false);
            break;
        case Tool::Height:
            ui->toolButtonHeight->setChecked(false);
            break;
        case Tool::Triangle:
            ui->toolButtonTriangle->setChecked(false);
            break;
        case Tool::PointOfIntersection:
            ui->toolButtonPointOfIntersection->setChecked(false);
            break;
        case Tool::CutSpline:
            ui->toolButtonSplineCutPoint->setChecked(false);
            break;
        case Tool::CutSplinePath:
            ui->toolButtonSplinePathCutPoint->setChecked(false);
            break;
        case Tool::UnionDetails:
            ui->toolButtonUnionDetails->setChecked(false);
            break;
        case Tool::CutArc:
            ui->toolButtonArcCutPoint->setChecked(false);
            break;
        case Tool::LineIntersectAxis:
            ui->toolButtonLineIntersectAxis->setChecked(false);
            break;
        case Tool::CurveIntersectAxis:
            ui->toolButtonCurveIntersectAxis->setChecked(false);
            ui->toolButtonArcIntersectAxis->setChecked(false);
            break;
        case Tool::PointOfIntersectionArcs:
            ui->toolButtonPointOfIntersectionArcs->setChecked(false);
            break;
        case Tool::PointOfIntersectionCircles:
            ui->toolButtonPointOfIntersectionCircles->setChecked(false);
            break;
        case Tool::PointFromCircleAndTangent:
            ui->toolButtonPointFromCircleAndTangent->setChecked(false);
            break;
        case Tool::PointFromArcAndTangent:
            ui->toolButtonPointFromArcAndTangent->setChecked(false);
            break;
        case Tool::TrueDarts:
            ui->toolButtonTrueDarts->setChecked(false);
            break;
        case Tool::NodePoint:
        case Tool::NodeArc:
        case Tool::NodeSpline:
        case Tool::NodeSplinePath:
        default:
            qDebug()<<"Got wrong tool type. Ignored.";
            break;
    }
    currentScene->setFocus(Qt::OtherFocusReason);
    currentScene->clearSelection();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowTool enable arrow tool.
 */
void  MainWindow::ArrowTool()
{
    qCDebug(vMainWindow, "Arrow tool.");
    CancelTool();
    ui->actionArrowTool->setChecked(true);
    ui->actionStopTool->setEnabled(false);
    currentTool = Tool::Arrow;
    emit EnableItemMove(true);
    QCursor cur(Qt::ArrowCursor);
    ui->view->setCursor(cur);
    helpLabel->setText("");
    ui->view->setShowToolOptions(true);
    qCDebug(vMainWindow, "Enabled arrow tool.");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyPressEvent handle key press events.
 * @param event key event.
 */
void MainWindow::keyPressEvent ( QKeyEvent * event )
{
    switch (event->key())
    {
        case Qt::Key_Escape:
            ArrowTool();
            break;
        case Qt::Key_Return:
            EndVisualization();
            break;
        case Qt::Key_Enter:
            EndVisualization();
            break;
        default:
            break;
    }
    QMainWindow::keyPressEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveCurrentScene save scene options before set another.
 */
void MainWindow::SaveCurrentScene()
{
    if (mode == Draw::Calculation || mode == Draw::Modeling)
    {
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr);

        /*Save transform*/
        scene->setTransform(ui->view->transform());
        /*Save scroll bars value for previous scene.*/
        QScrollBar *horScrollBar = ui->view->horizontalScrollBar();
        scene->setHorScrollBar(horScrollBar->value());
        QScrollBar *verScrollBar = ui->view->verticalScrollBar();
        scene->setVerScrollBar(verScrollBar->value());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RestoreCurrentScene restore scene options after change.
 */
void MainWindow::RestoreCurrentScene()
{
    VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
    SCASSERT(scene != nullptr);

    /*Set transform for current scene*/
    ui->view->setTransform(scene->transform());
    /*Set value for current scene scroll bar.*/
    QScrollBar *horScrollBar = ui->view->horizontalScrollBar();
    horScrollBar->setValue(scene->getHorScrollBar());
    QScrollBar *verScrollBar = ui->view->verticalScrollBar();
    verScrollBar->setValue(scene->getVerScrollBar());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionDraw show draw scene.
 * @param checked true - button checked.
 */
void MainWindow::ActionDraw(bool checked)
{
    if (checked)
    {
        qCDebug(vMainWindow, "Show draw scene");

        leftGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_left_to_right_arrow.png"));
        rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));

        ui->actionDetails->setChecked(false);
        ui->actionLayout->setChecked(false);
        SaveCurrentScene();

        currentScene = sceneDraw;
        ui->view->setScene(currentScene);
        connect(ui->view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor,
                Qt::UniqueConnection);
        RestoreCurrentScene();

        mode = Draw::Calculation;
        comboBoxDraws->setEnabled(true);
        comboBoxDraws->setCurrentIndex(currentDrawIndex);//restore current pattern peace
        drawMode = true;

        SetEnableTool(true);
        doc->setCurrentData();
        ui->toolBox->setCurrentIndex(currentToolBoxIndex);

        ui->actionHistory->setEnabled(true);
        ui->actionOptionDraw->setEnabled(true);
        ui->actionNewDraw->setEnabled(true);
        ui->actionTable->setEnabled(true);
        ui->actionArrowTool->setEnabled(true);
        ui->actionShowCurveDetails->setEnabled(true);
        actionDockWidgetToolOptions->setEnabled(true);
        if (qApp->getUndoStack()->canUndo())
        {
            undoAction->setEnabled(true);
        }
        if (qApp->getUndoStack()->canRedo())
        {
            redoAction->setEnabled(true);
        }

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            ui->toolBarOption->setVisible(true);
        }

        ui->dockWidgetLayoutPages->setVisible(false);
        ui->dockWidgetToolOptions->setVisible(true);
    }
    else
    {
        ui->actionDraw->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionDetails show details scene.
 * @param checked true - button checked.
 */
void MainWindow::ActionDetails(bool checked)
{
    if (checked)
    {
        const QHash<quint32, VDetail> *details = pattern->DataDetails();
        if(not qApp->getOpeningPattern())
        {
            if (details->count() == 0)
            {
                QMessageBox::information(this, tr("Detail mode"), tr("You can't use now the Detail mode. "
                                                                     "Please, create at least one workpiece."),
                                         QMessageBox::Ok, QMessageBox::Ok);
                Layout();
                return;
            }
        }

        qCDebug(vMainWindow, "Show details scene");

        leftGoToStage->setPixmap(QPixmap("://icon/24x24/right_to_left_arrow.png"));
        rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));

        ui->actionDraw->setChecked(false);
        ui->actionLayout->setChecked(false);
        SaveCurrentScene();

        currentScene = sceneDetails;
        ui->view->itemClicked(nullptr);
        ui->view->setScene(currentScene);
        disconnect(ui->view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
        RestoreCurrentScene();

        drawMode = false;
        currentDrawIndex = comboBoxDraws->currentIndex();//save current pattern peace
        comboBoxDraws->setCurrentIndex(comboBoxDraws->count()-1);
        comboBoxDraws->setEnabled(false);

        if (mode == Draw::Calculation)
        {
            currentToolBoxIndex = ui->toolBox->currentIndex();
        }
        mode = Draw::Modeling;
        SetEnableTool(true);
        ui->toolBox->setCurrentIndex(4);

        ui->actionHistory->setEnabled(false);
        ui->actionOptionDraw->setEnabled(false);
        ui->actionNewDraw->setEnabled(false);
        ui->actionTable->setEnabled(false);
        ui->actionArrowTool->setEnabled(true);
        ui->actionShowCurveDetails->setEnabled(false);
        actionDockWidgetToolOptions->setEnabled(true);
        if (qApp->getUndoStack()->canUndo())
        {
            undoAction->setEnabled(true);
        }
        if (qApp->getUndoStack()->canRedo())
        {
            redoAction->setEnabled(true);
        }

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            ui->toolBarOption->setVisible(true);
        }

        ui->dockWidgetLayoutPages->setVisible(false);
        ui->dockWidgetToolOptions->setVisible(true);

        helpLabel->setText("");
    }
    else
    {
        ui->actionDetails->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionLayout begin creation layout.
 * @param checked true - button checked.
 */
void MainWindow::ActionLayout(bool checked)
{
    if (checked)
    {
        const QHash<quint32, VDetail> *details = pattern->DataDetails();
        if(not qApp->getOpeningPattern())
        {
            if (details->count() == 0)
            {
                QMessageBox::information(this, tr("Layout mode"), tr("You can't use now the Layout mode. "
                                                                     "Please, create at least one workpiece."),
                                         QMessageBox::Ok, QMessageBox::Ok);
                Layout();
                return;
            }
        }

        qCDebug(vMainWindow, "Show layout scene");

        leftGoToStage->setPixmap(QPixmap("://icon/24x24/right_to_left_arrow.png"));
        rightGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_right_to_left_arrow.png"));

        ui->actionDraw->setChecked(false);
        ui->actionDetails->setChecked(false);
        SaveCurrentScene();

        PrepareDetailsForLayout(details);

        currentScene = tempSceneLayout;
        ui->view->itemClicked(nullptr);
        ui->view->setScene(currentScene);
        disconnect(ui->view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);

        drawMode = false;
        currentDrawIndex = comboBoxDraws->currentIndex();//save current pattern peace
        comboBoxDraws->setCurrentIndex(-1);
        comboBoxDraws->setEnabled(false);

        if (mode == Draw::Calculation)
        {
            currentToolBoxIndex = ui->toolBox->currentIndex();
        }
        mode = Draw::Layout;
        SetEnableTool(true);
        ui->toolBox->setCurrentIndex(5);

        ui->actionHistory->setEnabled(false);
        ui->actionOptionDraw->setEnabled(false);
        ui->actionNewDraw->setEnabled(false);
        ui->actionArrowTool->setEnabled(false);
        ui->actionTable->setEnabled(false);
        ui->actionShowCurveDetails->setEnabled(false);
        actionDockWidgetToolOptions->setEnabled(false);
        undoAction->setEnabled(false);
        redoAction->setEnabled(false);
        mouseCoordinate->setText("");

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            ui->toolBarOption->setVisible(false);
        }

        ui->dockWidgetLayoutPages->setVisible(true);
        ui->dockWidgetToolOptions->setVisible(false);

        ShowPaper(ui->listWidget->currentRow());

        if (scenes.isEmpty())
        {
            ui->toolButtonLayoutSettings->click();
        }

        helpLabel->setText("");
    }
    else
    {
        ui->actionLayout->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveAs save as pattern file.
 * @return true for successes saving.
 */
bool MainWindow::SaveAs()
{
    QString filters(tr("Pattern files (*.val)"));
    QString dir;
    if (curFile.isEmpty())
    {
        dir = qApp->ValentinaSettings()->GetPathPattern() + "/" + tr("pattern") + ".val";
    }
    else
    {
        dir = QFileInfo(curFile).absolutePath() + "/" + tr("pattern") + ".val";
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"), dir, filters);

    if (fileName.isEmpty())
    {
        return false;
    }
    QFileInfo f( fileName );
    if (f.suffix().isEmpty() && f.suffix() != "val")
    {
        fileName += ".val";
    }
    const QString oldFileName = curFile;
    QString error;
    bool result = SavePattern(fileName, error);
    if (result == false)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save file"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }
    if (oldFileName != curFile)
    {// Now we have new file name after save as.
     // But still have previous name in restore list. We should delete them.
        QStringList restoreFiles = qApp->ValentinaSettings()->GetRestoreFileList();
        restoreFiles.removeAll(oldFileName);
        qApp->ValentinaSettings()->SetRestoreFileList(restoreFiles);
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Save save pattern file.
 * @return true for successes saving.
 */
bool MainWindow::Save()
{
    if (curFile.isEmpty())
    {
        return SaveAs();
    }
    else
    {
        QString error;
        bool result = SavePattern(curFile, error);
        if (result)
        {
            QString autofile = curFile +".autosave";
            QFile file(autofile);
            file.remove();
        }
        else
        {
            QMessageBox messageBox;
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.setInformativeText(tr("Could not save file"));
            messageBox.setDefaultButton(QMessageBox::Ok);
            messageBox.setDetailedText(error);
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.exec();
        }
        return result;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Open ask user select pattern file.
 */
void MainWindow::Open()
{
    qCDebug(vMainWindow, "Openning new file.");
    const QString filter(tr("Pattern files (*.val)"));
    //Get list last open files
    const QStringList files = qApp->ValentinaSettings()->GetRecentFileList();
    QString dir;
    if (files.isEmpty())
    {
        dir = QDir::homePath();
    }
    else
    {
        //Absolute path to last open file
        dir = QFileInfo(files.first()).absolutePath();
    }
    qCDebug(vMainWindow, "Run QFileDialog::getOpenFileName: dir = %s.", dir.toUtf8().constData());
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), dir, filter);
    if (filePath.isEmpty())
    {
        return;
    }
    LoadPattern(filePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Preferences config dialog.
 */
void MainWindow::Preferences()
{
    ConfigDialog dlg(this);
    connect(&dlg, &ConfigDialog::UpdateProperties, this, &MainWindow::WindowsLocale); // Must be first
    connect(&dlg, &ConfigDialog::UpdateProperties, toolOptions, &VToolOptionsPropertyBrowser::RefreshOptions);
    connect(&dlg, &ConfigDialog::UpdateProperties, this, &MainWindow::ToolBarStyles);
    if (dlg.exec() == QDialog::Accepted)
    {
        InitAutoSave();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::RepotBug()
{
    qCDebug(vMainWindow, "Reporting bug");
    QDesktopServices::openUrl(QUrl("https://bitbucket.org/dismine/valentina/issues/new"));
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::OnlineHelp()
{
    qCDebug(vMainWindow, "Showing online help");
    QDesktopServices::openUrl(QUrl("https://bitbucket.org/dismine/valentina/wiki/manual/Content"));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear reset to default window.
 */
void MainWindow::Clear()
{
    qCDebug(vMainWindow, "Reseting main window.");

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    delete lock; // Unlock pattern file
    lock = nullptr;
    qCDebug(vMainWindow, "Unlocked pattern file.");
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)

    ui->actionDetails->setChecked(true);
    ui->actionDraw->setChecked(true);
    ui->actionLayout->setEnabled(true);
    qCDebug(vMainWindow, "Returned to Draw mode.");
    pattern->Clear();
    qCDebug(vMainWindow, "Clearing pattern.");
    if (not curFile.isEmpty())
    {
        watcher->removePath(AbsoluteMPath(curFile, doc->MPath()));
    }
    doc->clear();
    setCurrentFile(QString());
    qCDebug(vMainWindow, "Clearing scenes.");
    sceneDraw->clear();
    sceneDetails->clear();
    ArrowTool();
    comboBoxDraws->clear();
    ui->actionOptionDraw->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionSaveAs->setEnabled(false);
    ui->actionPattern_properties->setEnabled(false);
    ui->actionZoomIn->setEnabled(false);
    ui->actionZoomOut->setEnabled(false);
    ui->actionZoomFitBest->setEnabled(false);
    ui->actionZoomOriginal->setEnabled(false);
    ui->actionHistory->setEnabled(false);
    ui->actionTable->setEnabled(false);
    ui->actionEdit_pattern_code->setEnabled(false);
    ui->actionLast_tool->setEnabled(false);
    ui->actionShowCurveDetails->setEnabled(false);
    ui->actionLoadIndividual->setEnabled(false);
    ui->actionLoadStandard->setEnabled(false);
    ui->actionShowM->setEnabled(false);
    SetEnableTool(false);
    qApp->setPatternUnit(Unit::Cm);
    qApp->setPatternType(MeasurementsType::Unknown);
    ui->toolBarOption->clear();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    Layout();

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup--; // turn it off again
#endif /*Q_OS_WIN32*/
    qApp->getUndoStack()->clear();
    toolOptions->itemClicked(nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::FileClosedCorrect()
{
    WriteSettings();

    //File was closed correct.
    QStringList restoreFiles = qApp->ValentinaSettings()->GetRestoreFileList();
    restoreFiles.removeAll(curFile);
    qApp->ValentinaSettings()->SetRestoreFileList(restoreFiles);

    // Remove autosave file
    QFile autofile(curFile +".autosave");
    if (autofile.exists())
    {
        autofile.remove();
    }
    qCDebug(vMainWindow, "File %s closed correct.", curFile.toUtf8().constData());
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ResetWindow()
{
    if (MaybeSave())
    {
        FileClosedCorrect();
    }
    else
    {
        return;
    }
    Clear();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::FullParseFile()
{
    qCDebug(vMainWindow, "Full parsing file");

    toolOptions->ClearPropertyBrowser();
    try
    {
        SetEnabledGUI(true);
        doc->Parse(Document::FullParse);
    }
    catch (const VExceptionUndo &e)
    {
        Q_UNUSED(e);
        /* If user want undo last operation before undo we need finish broken redo operation. For those we post event
         * myself. Later in method customEvent call undo.*/
        QApplication::postEvent(this, new UndoEvent());
        return;
    }
    catch (const VExceptionObjectError &e)
    {
        e.CriticalMessageBox(tr("Error parsing file."), this);
        SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionConversionError &e)
    {
        e.CriticalMessageBox(tr("Error can't convert value."), this);
        SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        e.CriticalMessageBox(tr("Error empty parameter."), this);
        SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error wrong id."), this);
        SetEnabledGUI(false);
        return;
    }
    catch (VException &e)
    {
        e.CriticalMessageBox(tr("Error parsing file."), this);
        SetEnabledGUI(false);
        return;
    }
    catch (const std::bad_alloc &)
    {
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        QMessageBox::critical(this, tr("Critical error!"), tr("Error parsing file (std::bad_alloc)."), QMessageBox::Ok,
                              QMessageBox::Ok);
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        SetEnabledGUI(false);
        return;
    }

    QString patternPiece = QString();
    if (comboBoxDraws->currentIndex() != -1)
    {
        patternPiece = comboBoxDraws->itemText(comboBoxDraws->currentIndex());
    }
    comboBoxDraws->blockSignals(true);
    comboBoxDraws->clear();
    comboBoxDraws->addItems(doc->getPatternPieces());
    comboBoxDraws->blockSignals(false);
    ui->actionPattern_properties->setEnabled(true);

    GlobalChangePP(patternPiece);

    SetEnableTool(comboBoxDraws->count() > 0);
    SetEnableWidgets(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::GlobalChangePP(const QString &patternPiece)
{
    const qint32 index = comboBoxDraws->findText(patternPiece);
    try
    {
        if ( index != -1 )
        { // -1 for not found
            ChangePP(index, false);
        }
        else
        {
            ChangePP(0, false);
        }
    }
    catch (VExceptionBadId &e)
    {
        e.CriticalMessageBox(tr("Bad id."), this);
        SetEnabledGUI(false);
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        e.CriticalMessageBox(tr("Error empty parameter."), this);
        SetEnabledGUI(false);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetEnabledGUI(bool enabled)
{
    if (guiEnabled != enabled)
    {
        if (enabled == false)
        {
            ArrowTool();
            qApp->getUndoStack()->clear();
        }
        SetEnableWidgets(enabled);

        guiEnabled = enabled;

        SetEnableTool(enabled);
        ui->toolBarOption->setEnabled(enabled);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    #endif
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetEnableWidgets enable action button.
 * @param enable enable value.
 */
void MainWindow::SetEnableWidgets(bool enable)
{
    comboBoxDraws->setEnabled(enable);
    ui->actionOptionDraw->setEnabled(enable);
    if (enable && not curFile.isEmpty())
    {
        ui->actionSave->setEnabled(enable);
    }
    ui->actionSaveAs->setEnabled(enable);
    ui->actionPattern_properties->setEnabled(enable);
    ui->actionEdit_pattern_code->setEnabled(enable);
    ui->actionZoomIn->setEnabled(enable);
    ui->actionZoomOut->setEnabled(enable);
    ui->actionArrowTool->setEnabled(enable);
    ui->actionHistory->setEnabled(enable);
    ui->actionNewDraw->setEnabled(enable);
    ui->actionDraw->setEnabled(enable);
    ui->actionDetails->setEnabled(enable);
    ui->actionLayout->setEnabled(enable);
    ui->actionTable->setEnabled(enable);
    ui->actionZoomFitBest->setEnabled(enable);
    ui->actionZoomOriginal->setEnabled(enable);
    ui->actionShowCurveDetails->setEnabled(enable);
    ui->actionLoadIndividual->setEnabled(enable);
    ui->actionLoadStandard->setEnabled(enable);

    //Now we don't want allow user call context menu
    sceneDraw->SetDisableTools(!enable, doc->GetNameActivPP());
    ui->view->setEnabled(enable);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClickEndVisualization()
{
    EndVisualization(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::Layout()
{
    if (pattern->DataDetails()->size() > 0)
    {
        SetLayoutModeActions(true);
    }
    else
    {
        listDetails.clear();
        ui->actionDetails->setChecked(false);
        ui->actionLayout->setChecked(false);
        ui->actionDraw->setChecked(true);
        SetLayoutModeActions(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateGradation()
{
    UpdateHeightsList(VMeasurement::ListHeights(doc->GetGradationHeights(), qApp->patternUnit()));
    UpdateSizesList(VMeasurement::ListSizes(doc->GetGradationSizes(), qApp->patternUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateHeightsList(const QStringList &list)
{
    QString val;
    if (gradationHeights->currentIndex() != -1)
    {
        val = gradationHeights->currentText();
    }

    gradationHeights->blockSignals(true);
    gradationHeights->clear();
    gradationHeights->addItems(list);
    gradationHeights->blockSignals(false);

    int index = gradationHeights->findText(val);
    if (index != -1)
    {
        gradationHeights->setCurrentIndex(index);
    }
    else
    {
        ChangedHeight(list.at(0));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateSizesList(const QStringList &list)
{
    QString val;
    if (gradationSizes->currentIndex() != -1)
    {
        val = gradationSizes->currentText();
    }

    gradationSizes->blockSignals(true);
    gradationSizes->clear();
    gradationSizes->addItems(list);
    gradationSizes->blockSignals(false);

    int index = gradationSizes->findText(val);
    if (index != -1)
    {
        gradationSizes->setCurrentIndex(index);
    }
    else
    {
        ChangedSize(list.at(0));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewPattern create new empty pattern.
 */
void MainWindow::New()
{
    if (comboBoxDraws->count() == 0)
    {
        qCDebug(vMainWindow, "New PP.");
        QString patternPieceName = QString(tr("Pattern piece %1")).arg(comboBoxDraws->count()+1);
        qCDebug(vMainWindow, "Generated PP name: %s", patternPieceName.toUtf8().constData());

        qCDebug(vMainWindow, "First PP");
        DialogNewPattern newPattern(pattern, patternPieceName, this);
        if (newPattern.exec() == QDialog::Accepted)
        {
            patternPieceName = newPattern.name();
            qApp->setPatternUnit(newPattern.PatternUnit());
            qCDebug(vMainWindow, "PP name: %s", patternPieceName.toUtf8().constData());
        }
        else
        {
            qCDebug(vMainWindow, "Creation a new pattern was canceled.");
            return;
        }

        //Set scene size to size scene view
        VMainGraphicsView::NewSceneRect(sceneDraw, ui->view);
        VMainGraphicsView::NewSceneRect(sceneDetails, ui->view);

        AddPP(patternPieceName);
    }
    else
    {
        OpenNewValentina();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief haveChange enable action save if we have unsaved change.
 */
void MainWindow::PatternWasModified(bool saved)
{
    if (guiEnabled)
    {
        setWindowModified(!saved);
        ui->actionSave->setEnabled(!saved);
        isLayoutStale = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedSize change new size value.
 * @param text value size.
 */
void MainWindow::ChangedSize(const QString & text)
{
    pattern->SetSize(text.toInt());
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedGrowth change new height value.
 * @param text value height.
 */
void MainWindow::ChangedHeight(const QString &text)
{
    pattern->SetHeight(text.toInt());
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionTable show table with variables.
 * @param checked true - button checked.
 */
void MainWindow::ActionTable(bool checked)
{
    if (checked)
    {
        dialogTable = new DialogIncrements(pattern, doc, this);
        connect(dialogTable, &DialogIncrements::DialogClosed, this, &MainWindow::ClosedActionTable);
        dialogTable->show();
    }
    else
    {
        ui->actionTable->setChecked(true);
        dialogTable->activateWindow();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedActionTable actions after closing table with variables.
 */
void MainWindow::ClosedActionTable()
{
    ui->actionTable->setChecked(false);
    delete dialogTable;
    dialogTable = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionHistory show tool history.
 * @param checked true - button checked.
 */
void MainWindow::ActionHistory(bool checked)
{
    if (checked)
    {
        dialogHistory = new DialogHistory(pattern, doc, this);
        dialogHistory->setWindowFlags(Qt::Window);
        connect(this, &MainWindow::RefreshHistory, dialogHistory, &DialogHistory::UpdateHistory);
        connect(dialogHistory, &DialogHistory::DialogClosed, this, &MainWindow::ClosedActionHistory);
        dialogHistory->show();
    }
    else
    {
        ui->actionHistory->setChecked(true);
        dialogHistory->activateWindow();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ActionCurveDetailsMode(bool checked)
{
    ui->view->itemClicked(nullptr);
    sceneDraw->EnableDetailsMode(checked);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedActionHistory actions after closing history window with variables.
 */
void MainWindow::ClosedActionHistory()
{
    ui->actionHistory->setChecked(false);
    delete dialogHistory;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetEnableTool enable button.
 * @param enable enable value.
 */
void MainWindow::SetEnableTool(bool enable)
{
    bool drawTools = false;
    bool modelingTools = false;
    bool layoutTools = false;

    switch (mode)
    {
        case Draw::Calculation:
            drawTools = enable;
            break;
        case Draw::Modeling:
            modelingTools = enable;
            break;
        case Draw::Layout:
            layoutTools = enable;
            break;
        default:
            break;
    }

    //Drawing Tools
    ui->toolButtonEndLine->setEnabled(drawTools);
    ui->toolButtonLine->setEnabled(drawTools);
    ui->toolButtonAlongLine->setEnabled(drawTools);
    ui->toolButtonShoulderPoint->setEnabled(drawTools);
    ui->toolButtonNormal->setEnabled(drawTools);
    ui->toolButtonBisector->setEnabled(drawTools);
    ui->toolButtonLineIntersect->setEnabled(drawTools);
    ui->toolButtonSpline->setEnabled(drawTools);
    ui->toolButtonArc->setEnabled(drawTools);
    ui->toolButtonSplinePath->setEnabled(drawTools);
    ui->toolButtonPointOfContact->setEnabled(drawTools);
    ui->toolButtonNewDetail->setEnabled(drawTools);
    ui->toolButtonHeight->setEnabled(drawTools);
    ui->toolButtonTriangle->setEnabled(drawTools);
    ui->toolButtonPointOfIntersection->setEnabled(drawTools);
    ui->toolButtonSplineCutPoint->setEnabled(drawTools);
    ui->toolButtonSplinePathCutPoint->setEnabled(drawTools);
    ui->toolButtonArcCutPoint->setEnabled(drawTools);
    ui->toolButtonLineIntersectAxis->setEnabled(drawTools);
    ui->toolButtonCurveIntersectAxis->setEnabled(drawTools);
    ui->toolButtonArcIntersectAxis->setEnabled(drawTools);
    ui->toolButtonPointOfIntersectionArcs->setEnabled(drawTools);
    ui->toolButtonPointOfIntersectionCircles->setEnabled(drawTools);
    ui->toolButtonPointFromCircleAndTangent->setEnabled(drawTools);
    ui->toolButtonPointFromArcAndTangent->setEnabled(drawTools);
    ui->toolButtonArcWithLength->setEnabled(drawTools);
    ui->toolButtonTrueDarts->setEnabled(drawTools);

    ui->actionLast_tool->setEnabled(drawTools);

    //Modeling Tools
    ui->toolButtonUnionDetails->setEnabled(modelingTools);

    //Layout tools
    ui->toolButtonLayoutSettings->setEnabled(layoutTools);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetLayoutModeActions(bool enable)
{
    bool value = enable;
    if (scenes.isEmpty())
    {
        value = false;
    }
    ui->actionExportAs->setEnabled(value);
    ui->actionPrintPreview->setEnabled(value);
    ui->actionPrintPreviewTailed->setEnabled(value);
    ui->actionSaveAsPDF->setEnabled(value);
    ui->actionSaveAsTiledPDF->setEnabled(value);
    ui->actionPrint->setEnabled(value);
    ui->actionPrintTiled->setEnabled(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MinimumScrollBar set scroll bar to minimum.
 */
void MainWindow::MinimumScrollBar()
{
    QScrollBar *horScrollBar = ui->view->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar *verScrollBar = ui->view->verticalScrollBar();
    verScrollBar->setValue(verScrollBar->minimum());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SavePattern save pattern file.
 * @param fileName pattern file name.
 * @return true if all is good.
 */
bool MainWindow::SavePattern(const QString &fileName, QString &error)
{
    qCDebug(vMainWindow, "Saving pattern file %s.", fileName.toUtf8().constData());
    QFileInfo tempInfo(fileName);

    const QString mPath = doc->MPath();
    if (not mPath.isEmpty() && curFile != fileName)
    {
        doc->SetPath(RelativeMPath(fileName, mPath));
    }

    const bool result = doc->SaveDocument(fileName, error);
    if (result)
    {
        if (tempInfo.suffix() != "autosave")
        {
            setCurrentFile(fileName);
            helpLabel->setText(tr("File saved"));
            qCDebug(vMainWindow, "File %s saved.", fileName.toUtf8().constData());
            PatternWasModified(result);
        }
    }
    else
    {
        doc->SetPath(mPath);
        qCDebug(vMainWindow, "Could not save file %s. %s.", fileName.toUtf8().constData(), error.toUtf8().constData());
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AutoSavePattern start safe saving.
 */
void MainWindow::AutoSavePattern()
{
    qCDebug(vMainWindow, "Autosaving pattern.");

    if (curFile.isEmpty() == false && this->isWindowModified() == true)
    {
        QString autofile = curFile +".autosave";
        QString error;
        SavePattern(autofile, error);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentFile the function is called to reset the state of a few variables when a file
 * is loaded or saved, or when the user starts editing a new file (in which case fileName is empty).
 * @param fileName file name.
 */
void MainWindow::setCurrentFile(const QString &fileName)
{
    qCDebug(vMainWindow, "Set current name to \"%s\"", fileName.toUtf8().constData());
    curFile = fileName;
    qApp->getUndoStack()->setClean();

    QString shownName = strippedName(curFile);
    if (curFile.isEmpty())
    {
        shownName = tr("untitled.val");
    }
    else
    {
        qCDebug(vMainWindow, "Updating recent file list.");
        QStringList files = qApp->ValentinaSettings()->GetRecentFileList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while (files.size() > MaxRecentFiles)
        {
            files.removeLast();
        }

        qApp->ValentinaSettings()->SetRecentFileList(files);
        UpdateRecentFileActions();

        qCDebug(vMainWindow, "Updating restore file list.");
        QStringList restoreFiles = qApp->ValentinaSettings()->GetRestoreFileList();
        restoreFiles.removeAll(fileName);
        restoreFiles.prepend(fileName);
        qApp->ValentinaSettings()->SetRestoreFileList(restoreFiles);
    }
    shownName+="[*]";
    setWindowTitle(shownName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief strippedName the function call around curFile to exclude the path to the file.
 * @param fullFileName full path to the file.
 * @return file name.
 */
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ReadSettings read setting for app.
 */
void MainWindow::ReadSettings()
{
    qCDebug(vMainWindow, "Reading settings.");
    restoreGeometry(qApp->ValentinaSettings()->GetGeometry());
    restoreState(qApp->ValentinaSettings()->GetWindowState());
    restoreState(qApp->ValentinaSettings()->GetToolbarsState(), APP_VERSION);

    // Scene antialiasing
    const bool graphOutputValue = qApp->ValentinaSettings()->GetGraphicalOutput();
    ui->view->setRenderHint(QPainter::Antialiasing, graphOutputValue);
    ui->view->setRenderHint(QPainter::SmoothPixmapTransform, graphOutputValue);

    // Stack limit
    qApp->getUndoStack()->setUndoLimit(qApp->ValentinaSettings()->GetUndoCount());

    // Text under tool buton icon
    ToolBarStyles();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief WriteSettings save setting for app.
 */
void MainWindow::WriteSettings()
{
    ActionDraw(true);

    qApp->ValentinaSettings()->SetGeometry(saveGeometry());
    qApp->ValentinaSettings()->SetWindowState(saveState());
    qApp->ValentinaSettings()->SetToolbarsState(saveState(APP_VERSION));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MaybeSave The function is called to save pending changes.
 * @return returns true in all cases, except when the user clicks Cancel.
 */
bool MainWindow::MaybeSave()
{
    if (this->isWindowModified() && guiEnabled)
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Unsaved changes"), tr("The pattern has been modified.\n"
                                                             "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        {
            return Save();
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateRecentFileActions()
{
    qCDebug(vMainWindow, "Updating recent file actions.");
    const QStringList files = qApp->ValentinaSettings()->GetRecentFileList();
    const int numRecentFiles = qMin(files.size(), static_cast<int>(MaxRecentFiles));

    for (int i = 0; i < numRecentFiles; ++i)
    {
       QString text = QString("&%1. %2").arg(i + 1).arg(strippedName(files.at(i)));
       recentFileActs[i]->setText(text);
       recentFileActs[i]->setData(files.at(i));
       recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
    {
       recentFileActs[j]->setVisible(false);
    }

    separatorAct->setVisible(numRecentFiles > 0);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::CreateMenus()
{
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        ui->menuFile->insertAction(ui->actionPreferences, recentFileActs[i]);
    }
    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    ui->menuFile->insertAction(ui->actionPreferences, separatorAct);
    UpdateRecentFileActions();

    //Add Undo/Redo actions.
    undoAction = qApp->getUndoStack()->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setIcon(QIcon::fromTheme("edit-undo"));
    ui->menuPatternPiece->insertAction(ui->actionLast_tool, undoAction);
    ui->toolBarTools->addAction(undoAction);

    redoAction = qApp->getUndoStack()->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setIcon(QIcon::fromTheme("edit-redo"));
    ui->menuPatternPiece->insertAction(ui->actionLast_tool, redoAction);
    ui->toolBarTools->addAction(redoAction);

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, separatorAct);

    //Add dock
    actionDockWidgetToolOptions = ui->dockWidgetToolOptions->toggleViewAction();
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, actionDockWidgetToolOptions);

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, separatorAct);
    AddDocks();
}

void MainWindow::LastUsedTool()
{
    if (currentTool == lastUsedTool)
    {
        return;
    }

    switch ( lastUsedTool )
    {
        case Tool::Arrow:
            ui->actionArrowTool->setChecked(true);
            ArrowTool();
            break;
        case Tool::BasePoint:
            Q_UNREACHABLE();
            //Nothing to do here because we can't create this tool from main window.
            break;
        case Tool::EndLine:
            ui->toolButtonEndLine->setChecked(true);
            ToolEndLine(true);
            break;
        case Tool::Line:
            ui->toolButtonLine->setChecked(true);
            ToolLine(true);
            break;
        case Tool::AlongLine:
            ui->toolButtonAlongLine->setChecked(true);
            ToolAlongLine(true);
            break;
        case Tool::ShoulderPoint:
            ui->toolButtonShoulderPoint->setChecked(true);
            ToolShoulderPoint(true);
            break;
        case Tool::Normal:
            ui->toolButtonNormal->setChecked(true);
            ToolNormal(true);
            break;
        case Tool::Bisector:
            ui->toolButtonBisector->setChecked(true);
            ToolBisector(true);
            break;
        case Tool::LineIntersect:
            ui->toolButtonLineIntersect->setChecked(true);
            ToolLineIntersect(true);
            break;
        case Tool::Spline:
            ui->toolButtonSpline->setChecked(true);
            ToolSpline(true);
            break;
        case Tool::Arc:
            ui->toolButtonArc->setChecked(true);
            ToolArc(true);
            break;
        case Tool::SplinePath:
            ui->toolButtonSplinePath->setChecked(true);
            ToolSplinePath(true);
            break;
        case Tool::PointOfContact:
            ui->toolButtonPointOfContact->setChecked(true);
            ToolPointOfContact(true);
            break;
        case Tool::Detail:
            ui->toolButtonNewDetail->setChecked(true);
            ToolDetail(true);
            break;
        case Tool::Height:
            ui->toolButtonHeight->setChecked(true);
            ToolHeight(true);
            break;
        case Tool::Triangle:
            ui->toolButtonTriangle->setChecked(true);
            ToolTriangle(true);
            break;
        case Tool::PointOfIntersection:
            ui->toolButtonPointOfIntersection->setChecked(true);
            ToolPointOfIntersection(true);
            break;
        case Tool::PointOfIntersectionArcs:
            ui->toolButtonPointOfIntersectionArcs->setChecked(true);
            ToolPointOfIntersectionArcs(true);
            break;
        case Tool::CutSpline:
            ui->toolButtonSplineCutPoint->setChecked(true);
            ToolCutSpline(true);
            break;
        case Tool::CutSplinePath:
            ui->toolButtonSplinePathCutPoint->setChecked(true);
            ToolCutSplinePath(true);
            break;
        case Tool::UnionDetails:
            ui->toolButtonUnionDetails->setChecked(true);
            ToolUnionDetails(true);
            break;
        case Tool::CutArc:
            ui->toolButtonArcCutPoint->setChecked(true);
            ToolCutArc(true);
            break;
        case Tool::LineIntersectAxis:
            ui->toolButtonLineIntersectAxis->setChecked(true);
            ToolLineIntersectAxis(true);
            break;
        case Tool::CurveIntersectAxis:
            ui->toolButtonCurveIntersectAxis->setChecked(true);
            ToolCurveIntersectAxis(true);
            break;
        case Tool::PointOfIntersectionCircles:
            ui->toolButtonPointOfIntersectionCircles->setChecked(true);
            ToolPointOfIntersectionCircles(true);
            break;
        case Tool::PointFromCircleAndTangent:
            ui->toolButtonPointFromCircleAndTangent->setChecked(true);
            ToolPointFromCircleAndTangent(true);
            break;
        case Tool::PointFromArcAndTangent:
            ui->toolButtonPointFromArcAndTangent->setChecked(true);
            ToolPointFromArcAndTangent(true);
            break;
        case Tool::ArcWithLength:
            ui->toolButtonArcWithLength->setChecked(true);
            ToolArcWithLength(true);
            break;
        case Tool::TrueDarts:
            ui->toolButtonTrueDarts->setChecked(true);
            ToolTrueDarts(true);
            break;
        case Tool::NodePoint:
        case Tool::NodeArc:
        case Tool::NodeSpline:
        case Tool::NodeSplinePath:
        default:
            qDebug()<<"Got wrong tool type. Ignored.";
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::AddDocks()
{
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, ui->dockWidgetToolOptions->toggleViewAction());

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, separatorAct);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::PropertyBrowser()
{
    qCDebug(vMainWindow, "Initialization property browser.");
    toolOptions = new VToolOptionsPropertyBrowser(ui->dockWidgetToolOptions);

    connect(ui->view, &VMainGraphicsView::itemClicked, toolOptions, &VToolOptionsPropertyBrowser::itemClicked);
    connect(doc, &VPattern::FullUpdateFromFile, toolOptions, &VToolOptionsPropertyBrowser::UpdateOptions);
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenNewValentina(const QString &fileName) const
{
    if (this->isWindowModified() || curFile.isEmpty() == false)
    {
        VApplication::NewValentina(fileName);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::CreateActions()
{
    ui->setupUi(this);

    connect(ui->actionStopTool, &QAction::triggered, this, &MainWindow::ArrowTool);
    connect(ui->actionArrowTool, &QAction::triggered, this, &MainWindow::ArrowTool);
    connect(ui->actionDraw, &QAction::triggered, this, &MainWindow::ActionDraw);
    connect(ui->actionDetails, &QAction::triggered, this, &MainWindow::ActionDetails);
    connect(ui->actionLayout, &QAction::triggered, this, &MainWindow::ActionLayout);
    connect(ui->actionHistory, &QAction::triggered, this, &MainWindow::ActionHistory);
    connect(ui->actionNewDraw, &QAction::triggered, this, &MainWindow::NewPP);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::SaveAs);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::Save);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::Open);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::New);
    connect(ui->actionTable, &QAction::triggered, this, &MainWindow::ActionTable);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::AboutQt);
    connect(ui->actionAbout_Valentina, &QAction::triggered, this, &MainWindow::About);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::Preferences);
    connect(ui->actionRepotBug, &QAction::triggered, this, &MainWindow::RepotBug);
    connect(ui->actionOnlineHelp, &QAction::triggered, this, &MainWindow::OnlineHelp);
    connect(ui->actionLast_tool, &QAction::triggered, this, &MainWindow::LastUsedTool);
    connect(ui->actionPattern_properties, &QAction::triggered, this, &MainWindow::PatternProperties);
    ui->actionPattern_properties->setEnabled(false);
    connect(ui->actionEdit_pattern_code, &QAction::triggered, this, &MainWindow::EditPatternCode);
    connect(ui->actionCloseWindow, &QAction::triggered, this, &MainWindow::ResetWindow);
    connect(ui->actionShowCurveDetails, &QAction::triggered, this, &MainWindow::ActionCurveDetailsMode);
    connect(ui->actionLoadIndividual, &QAction::triggered, this, &MainWindow::LoadIndividual);
    connect(ui->actionLoadStandard, &QAction::triggered, this, &MainWindow::LoadStandard);
    connect(ui->actionCreateNew, &QAction::triggered, this, &MainWindow::CreateMeasurements);
    connect(ui->actionShowM, &QAction::triggered, this, &MainWindow::ShowMeasurements);
    connect(ui->actionExportAs, &QAction::triggered, this, &MainWindow::ExportLayoutAs);
    connect(ui->actionPrintPreview, &QAction::triggered, this, &MainWindow::PrintPreviewOrigin);
    connect(ui->actionPrintPreviewTailed, &QAction::triggered, this, &MainWindow::PrintPreviewTiled);
    connect(ui->actionSaveAsPDF, &QAction::triggered, this, &MainWindow::SaveAsPDF);
    connect(ui->actionSaveAsTiledPDF, &QAction::triggered, this, &MainWindow::SaveAsTiledPDF);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::PrintOrigin);
    connect(ui->actionPrintTiled, &QAction::triggered, this, &MainWindow::PrintTiled);
    ui->actionEdit_pattern_code->setEnabled(false);

    //Actions for recent files loaded by a main window application.
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], &QAction::triggered, this, &MainWindow::OpenRecentFile);
    }

    connect(ui->actionSyncMeasurements, &QAction::triggered, this, &MainWindow::SyncMeasurements);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitAutoSave()
{
    //Autosaving file each 1 minutes
    delete autoSaveTimer;
    autoSaveTimer = nullptr;

    autoSaveTimer = new QTimer(this);
    autoSaveTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::AutoSavePattern);
    autoSaveTimer->stop();

    if (qApp->ValentinaSettings()->GetAutosaveState())
    {
        const qint32 autoTime = qApp->ValentinaSettings()->GetAutosaveTime();
        autoSaveTimer->start(autoTime*60000);
        qCDebug(vMainWindow, "Autosaving each %d minutes.", autoTime);
    }
    qApp->setAutoSaveTimer(autoSaveTimer);
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::PatternPieceName(const QString &text)
{
    QInputDialog *dlg = new QInputDialog(this);
    dlg->setInputMode( QInputDialog::TextInput );
    dlg->setLabelText(tr("Pattern piece:"));
    dlg->setTextEchoMode(QLineEdit::Normal);
    dlg->setWindowTitle(tr("Enter a new label for the pattern piece."));
    dlg->resize(300, 100);
    dlg->setTextValue(text);
    QString nameDraw;
    while (1)
    {
        const bool bOk = dlg->exec();
        nameDraw = dlg->textValue();
        if (bOk == false || nameDraw.isEmpty())
        {
            delete dlg;
            return QString();
        }
        if (comboBoxDraws->findText(nameDraw) == -1)
        {
            break;//repeate show dialog
        }
    }
    delete dlg;
    return nameDraw;
}

//---------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    CancelTool();
    CleanLayout();

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    delete lock; // Unlock pattern file
#endif
    delete doc;
    delete sceneDetails;
    delete sceneDraw;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LoadPattern open pattern file.
 * @param fileName name of file.
 */
bool MainWindow::LoadPattern(const QString &fileName, const QString& customMeasureFile)
{
    qCDebug(vMainWindow, "Loading new file %s.", fileName.toUtf8().constData());

    //We have unsaved changes or load more then one file per time
    if (OpenNewValentina(fileName))
    {
        return false;
    }

    if (fileName.isEmpty())
    {
        qCDebug(vMainWindow, "Got empty file.");
        Clear();
        return false;
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    qCDebug(vMainWindow, "Loking file");
    lock = new QLockFile(fileName+".lock");
    lock->setStaleLockTime(0);
    if (VApplication::TryLock(lock))
    {
        qCDebug(vMainWindow, "Pattern file %s was locked.", fileName.toUtf8().constData());
    }
    else
    {
        qCDebug(vMainWindow, "Failed to lock %s", fileName.toUtf8().constData());
        qCDebug(vMainWindow, "Error type: %d", lock->error());
        if (lock->error() == QLockFile::LockFailedError)
        {
            qCCritical(vMainWindow, "%s", tr("This file already opened in another window.").toUtf8().constData());
            Clear();
            return false;
        }
    }
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)

    // On this stage scene empty. Fit scene size to view size
    VMainGraphicsView::NewSceneRect(sceneDraw, ui->view);
    VMainGraphicsView::NewSceneRect(sceneDetails, ui->view);

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup++; // turn checking on
#endif /*Q_OS_WIN32*/

    qApp->setOpeningPattern();//Begin opening file
    try
    {
        VPatternConverter converter(fileName);
        converter.Convert();

        VDomDocument::ValidateXML(VPatternConverter::CurrentSchema, fileName);
        doc->setXMLContent(fileName);
        if (!customMeasureFile.isEmpty())
        {
            doc->SetPath(customMeasureFile);
        }
        qApp->setPatternUnit(doc->MUnit());
        QString path = doc->MPath();

        if (not path.isEmpty())
        {
            // Check if exist
            path = CheckPathToMeasurements(fileName, AbsoluteMPath(fileName, path));
            if (path.isEmpty())
            {
                Clear();
                return false;
            }

            if (not LoadMeasurements(path))
            {
                Clear();
                return false;
            }
            else
            {
                watcher->addPath(path);
                ui->actionShowM->setEnabled(true);
            }
        }

        ToolBarOption();
    }
    catch (VException &e)
    {
        if (qApp->CheckGUI())
        {
            e.CriticalMessageBox(tr("File error."), this);
        }
        else
        {
            OUT_FILE_ERROR;
        }
        Clear();
        return false;
    }

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup--; // turn it off again
#endif /*Q_OS_WIN32*/

    FullParseFile();

    if (guiEnabled)
    { // No errors occurred
        bool patternModified = this->isWindowModified();
        setCurrentFile(fileName);
        if (patternModified)
        {
            //For situation where was fixed wrong formula need return for document status was modified.
            PatternWasModified(!patternModified);
        }
        helpLabel->setText(tr("File loaded"));
        qCDebug(vMainWindow, "File loaded.");

        //Fit scene size to best size for first show
        ZoomFirstShow();

        qApp->setOpeningPattern();// End opening file

        ui->actionDraw->setChecked(true);
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList MainWindow::GetUnlokedRestoreFileList() const
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    QStringList restoreFiles;
    //Take all files that need to be restored
    QStringList files = qApp->ValentinaSettings()->GetRestoreFileList();
    if (files.size() > 0)
    {
        for (int i = 0; i < files.size(); ++i)
        {
            // Seeking file that realy need reopen
            QLockFile *lock = new QLockFile(files.at(i)+".lock");
            lock->setStaleLockTime(0);
            if (VApplication::TryLock(lock))
            {
                restoreFiles.append(files.at(i));
            }
            delete lock;
            lock = nullptr;
        }

        // Clearing list after filtering
        for (int i = 0; i < restoreFiles.size(); ++i)
        {
            files.removeAll(restoreFiles.at(i));
        }

        qApp->ValentinaSettings()->SetRestoreFileList(files);

    }
    return restoreFiles;
#else
    return qApp->getSettings()->GetRestoreFileList();
#endif //QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::WindowsLocale()
{
    qApp->ValentinaSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolBarStyles()
{
    ToolBarStyle(ui->toolBarArrows);
    ToolBarStyle(ui->toolBarDraws);
    ToolBarStyle(ui->toolBarOption);
    ToolBarStyle(ui->toolBarStages);
    ToolBarStyle(ui->toolBarTools);
    ToolBarStyle(ui->mainToolBar);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowPaper(int index)
{
    if (index < 0 || index >= scenes.size())
    {
        ui->view->setScene(tempSceneLayout);
        SetLayoutModeActions(false);
    }
    else
    {
        ui->view->setScene(scenes.at(index));
        SetLayoutModeActions(true);
    }

    ui->view->fitInView(ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolBarStyle(QToolBar *bar)
{
    if (qApp->ValentinaSettings()->GetToolBarStyle())
    {
        bar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }
    else
    {
        bar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ReopenFilesAfterCrash(QStringList &args)
{
    const QStringList files = GetUnlokedRestoreFileList();
    if (files.size() > 0)
    {
        qCDebug(vMainWindow, "Reopen files after crash.");

        QStringList restoreFiles;
        for (int i = 0; i < files.size(); ++i)
        {
            QFile file(files.at(i) +".autosave");
            if (file.exists())
            {
                restoreFiles.append(files.at(i));
            }
        }

        if (restoreFiles.size() > 0)
        {
            QMessageBox::StandardButton reply;
            QString mes=QString(tr("Valentina didn't shut down correctly. "
                                   "Do you want reopen files (%1) you had open?")).arg(restoreFiles.size());
            reply = QMessageBox::question(this, tr("Reopen files."), mes, QMessageBox::Yes|QMessageBox::No,
                                          QMessageBox::Yes);
            if (reply == QMessageBox::Yes)
            {
                qCDebug(vMainWindow, "User said Yes.");

                for (int i = 0; i < restoreFiles.size(); ++i)
                {
                    QString error;
                    if (VDomDocument::SafeCopy(restoreFiles.at(i) +".autosave", restoreFiles.at(i), error))
                    {
                        QFile autoFile(restoreFiles.at(i) +".autosave");
                        autoFile.remove();
                        LoadPattern(restoreFiles.at(i));
                        args.removeAll(restoreFiles.at(i));// Do not open file twice after we restore him.
                    }
                    else
                    {
                        qCDebug(vMainWindow, "Could not copy %s.autosave to %s %s",
                                restoreFiles.at(i).toUtf8().constData(), restoreFiles.at(i).toUtf8().constData(),
                                error.toUtf8().constData());
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::CheckPathToMeasurements(const QString &patternPath, const QString &path)
{
    if (path.isEmpty())
    {
        return path;
    }

    QFileInfo table(path);
    if (table.exists() == false)
    {
        if (!qApp->CheckGUI())
        {
            vStdErr() << tr("The measurements file \"") << path << tr("\" could not be found.\n");
            return QString();
        }

        QString text = tr("The measurements file <br/><br/> <b>%1</b> <br/><br/> %3").arg(path)
                .arg(tr("could not be found. Do you want to update the file location"));
        QMessageBox::StandardButton res = QMessageBox::question(this, "Loading measurements file", text,
                                                                QMessageBox::Yes | QMessageBox::No,
                                                                QMessageBox::Yes);
        if (res == QMessageBox::No)
        {
            return QString();
        }
        else
        {
            MeasurementsType patternType;
            if (table.suffix() == QLatin1String("vst"))
            {
                patternType = MeasurementsType::Standard;
            }
            else if (table.suffix() == QLatin1String("vit"))
            {
                patternType = MeasurementsType::Individual;
            }
            else
            {
                patternType = MeasurementsType::Unknown;
            }

            QString mPath;
            if (patternType == MeasurementsType::Standard)
            {
                const QString filter = tr("Standard measurements (*.vst)");
                //Use standard path to standard measurements
                const QString path = qApp->ValentinaSettings()->GetPathStandardMeasurements();
                mPath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);
            }
            else if (patternType == MeasurementsType::Individual)
            {
                const QString filter = tr("Individual measurements (*.vit)");
                //Use standard path to individual measurements
                const QString path = qApp->ValentinaSettings()->GetPathIndividualMeasurements();
                mPath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);
            }
            else
            {
                const QString filter = tr("Individual measurements (*.vit);;Standard measurements (*.vst)");
                //Use standard path to individual measurements
                const QString path = qApp->ValentinaSettings()->GetPathIndividualMeasurements();
                mPath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);
            }

            if (mPath.isEmpty())
            {
                return mPath;
            }
            else
            {
                VMeasurements *m = new VMeasurements(pattern);
                m->setXMLContent(mPath);

                patternType = m->Type();

                if (patternType == MeasurementsType::Unknown)
                {
                    VException e("Measurement file has unknown format.");
                    throw e;
                }

                if (patternType == MeasurementsType::Standard)
                {
                    VVSTConverter converter(mPath);
                    converter.Convert();

                    VDomDocument::ValidateXML(VVSTConverter::CurrentSchema, mPath);
                }
                else
                {
                    VVITConverter converter(mPath);
                    converter.Convert();

                    VDomDocument::ValidateXML(VVITConverter::CurrentSchema, mPath);
                }

                m->setXMLContent(mPath);// Read again after conversion

                const QStringList mList = m->ListAll();
                const QStringList pList = doc->ListMeasurements();

                delete m;

                const QSet<QString> match = pList.toSet().subtract(mList.toSet());
                if (not match.isEmpty())
                {
                    VException e("Measurement file doesn't include all required measurements.");
                    e.AddMoreInformation(QString("Please, additionaly provide: %1")
                                         .arg(QStringList(match.toList()).join(", ")));
                    throw e;
                }

                doc->SetPath(RelativeMPath(patternPath, mPath));
                PatternWasModified(false);
                qApp->setPatternType(patternType);
                return mPath;
            }
        }
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ChangePP(int index, bool zoomBestFit)
{
    if (index != -1)
    {
        doc->ChangeActivPP(comboBoxDraws->itemText(index));
        doc->LiteParseTree(Document::LiteParse);
        emit RefreshHistory();
        if (drawMode)
        {
            ArrowTool();
            if (zoomBestFit)
            {
                ui->view->fitInView(doc->ActiveDrawBoundingRect(), Qt::KeepAspectRatio);
                ui->view->NewFactor(ui->view->transform().m11());
            }
        }
        toolOptions->itemClicked(nullptr);//hide options for tool in previous pattern piece
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::EndVisualization(bool click)
{
    if (dialogTool != nullptr)
    {
        dialogTool->ShowDialog(click);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ZoomFirstShow Fit scene size to best size for first show
 */
void MainWindow::ZoomFirstShow()
{
    /* If don't call ZoomFitBest() twice, after first scaling or moving pattern piece, scene change coordinate and whole
     * pattern will be moved. Looks very ugly. It is best solution that i have now.
     */
    ActionDetails(true);
    ui->view->ZoomFitBest();

    ActionDraw(true);
    ui->view->ZoomFitBest();

    VMainGraphicsView::NewSceneRect(sceneDraw, ui->view);
    VMainGraphicsView::NewSceneRect(sceneDetails, ui->view);

    ActionDetails(true);
    ui->view->ZoomFitBest();

    ActionDraw(true);
    ui->view->ZoomFitBest();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::DoExport(const VCommandLinePtr &expParams)
{
    auto settings = expParams->DefaultGenerator();

    const QHash<quint32, VDetail> *details = pattern->DataDetails();
    if(not qApp->getOpeningPattern())
    {
        if (details->count() == 0)
        {
            AppAbort(tr("You can't export empty scene."));
            return;
        }
    }
    PrepareDetailsForLayout(details);
    LayoutSettings(*settings.get());
    DialogSaveLayout dialog(scenes.size(), expParams->OptExportPath(), this);
    dialog.SetFullPath(expParams->OptExportPath());
    dialog.SelectFormate(expParams->OptExportType());
    ExportLayout(dialog);
}
