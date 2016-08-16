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
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../ifc/exception/vexceptionconversionerror.h"
#include "../ifc/exception/vexceptionemptyparameter.h"
#include "../ifc/exception/vexceptionwrongid.h"
#include "../ifc/exception/vexceptionundo.h"
#include "version.h"
#include "core/vapplication.h"
#include "../vmisc/undoevent.h"
#include "../vmisc/vsettings.h"
#include "../vmisc/def.h"
#include "undocommands/renamepp.h"
#include "core/vtooloptionspropertybrowser.h"
#include "options.h"
#include "../ifc/xml/vpatternconverter.h"
#include "../vmisc/logging.h"
#include "../vformat/vmeasurements.h"
#include "../ifc/xml/vvstconverter.h"
#include "../ifc/xml/vvitconverter.h"
#include "../vwidgets/vwidgetpopup.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "tools/drawTools/drawtools.h"
#include "../vtools/dialogs/tooldialogs.h"
#include "tools/vtooldetail.h"
#include "tools/vtooluniondetails.h"
#include "dialogs/dialogs.h"
#include "dialogs/vwidgetgroups.h"
#include "../vtools/undocommands/addgroup.h"
#include "dialogs/vwidgetdetails.h"

#include <QInputDialog>
#include <QtDebug>
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
#include <chrono>
#include <thread>
#include <QFileSystemWatcher>
#include <QComboBox>

#if defined(Q_OS_MAC)
#include <QMimeData>
#include <QDrag>
#endif //defined(Q_OS_MAC)

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wmissing-prototypes")
QT_WARNING_DISABLE_INTEL(1418)

Q_LOGGING_CATEGORY(vMainWindow, "v.mainwindow")

QT_WARNING_POP

const QString autosavePrefix = QStringLiteral(".autosave");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MainWindow constructor.
 * @param parent parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    :MainWindowsNoGUI(parent), ui(new Ui::MainWindow), watcher(new QFileSystemWatcher(this)), currentTool(Tool::Arrow),
      lastUsedTool(Tool::Arrow), sceneDraw(nullptr), sceneDetails(nullptr),
      mouseCoordinate(nullptr), helpLabel(nullptr), isInitialized(false), mChanges(false), mChangesAsked(true),
      patternReadOnly(false),
      dialogTable(nullptr),
      dialogTool(nullptr),
      dialogHistory(nullptr), comboBoxDraws(nullptr), patternPieceLabel(nullptr), mode(Draw::Calculation),
      currentDrawIndex(0), currentToolBoxIndex(0),
      isDockToolOptionsVisible(true),
      isDockGroupsVisible(true),
      drawMode(true), recentFileActs(),
      separatorAct(nullptr),
      leftGoToStage(nullptr), rightGoToStage(nullptr), autoSaveTimer(nullptr), guiEnabled(true),
      gradationHeights(nullptr), gradationSizes(nullptr), gradationHeightsLabel(nullptr), gradationSizesLabel(nullptr),
      toolOptions(nullptr),
      groupsWidget(nullptr),
      detailsWidget(nullptr),
      lock(nullptr),
      toolButtonPointerList()
{
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = nullptr;
    }

    CreateActions();
    InitScenes();

    doc = new VPattern(pattern, &mode, sceneDraw, sceneDetails);
    connect(doc, &VPattern::ClearMainWindow, this, &MainWindow::Clear);
    connect(doc, &VPattern::patternChanged, this, &MainWindow::PatternChangesWereSaved);
    connect(doc, &VPattern::UndoCommand, this, &MainWindow::FullParseFile);
    connect(doc, &VPattern::SetEnabledGUI, this, &MainWindow::SetEnabledGUI);
    connect(doc, &VPattern::CheckLayout, [this]()
    {
        if (pattern->DataDetails()->count() == 0)
        {
            if(not ui->actionDraw->isChecked())
            {
                ActionDraw(true);
            }
        }
    });
    connect(doc, &VPattern::SetCurrentPP, this, &MainWindow::GlobalChangePP);
    qApp->setCurrentDocument(doc);

    InitDocksContain();
    CreateMenus();
    ToolBarDraws();
    ToolBarStages();
    InitToolButtons();

    helpLabel = new QLabel(QObject::tr("Create new pattern piece to start working."));
    ui->statusBar->addWidget(helpLabel);

    ToolBarTools();

    connect(qApp->getUndoStack(), &QUndoStack::cleanChanged, this, &MainWindow::PatternChangesWereSaved);

    InitAutoSave();

    ui->toolBox->setCurrentIndex(0);

    ReadSettings();

    setCurrentFile("");
    WindowsLocale();

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &MainWindow::ShowPaper);
    ui->dockWidgetLayoutPages->setVisible(false);

    connect(watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::MeasurementsChanged);
    connect(qApp, &QApplication::focusChanged, [this](QWidget *old, QWidget *now)
    {
        if (old == nullptr && isAncestorOf(now) == true)
        {// focus IN
            if (mChanges && not mChangesAsked)
            {
                mChangesAsked = true;
                const auto answer = QMessageBox::question(this, tr("Measurements"),
                                                  tr("Measurements was changed. Do you want to sync measurements now?"),
                                                          QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
                if (answer == QMessageBox::Yes)
                {
                    SyncMeasurements();
                }
            }
        }

        // In case we will need it
        // else if (isAncestorOf(old) == true && now == nullptr)
        // focus OUT
    });

#if defined(Q_OS_MAC)
    // On Mac deafault icon size is 32x32.
    ui->toolBarDraws->setIconSize(QSize(24, 24));
    ui->toolBarOption->setIconSize(QSize(24, 24));
    ui->toolBarStages->setIconSize(QSize(24, 24));
    ui->toolBarTools->setIconSize(QSize(24, 24));

    setUnifiedTitleAndToolBarOnMac(true);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 2)
    // Mac OS Dock Menu
    QMenu *menu = new QMenu(this);

    QAction *actionNewPattern = menu->addAction(tr("New pattern"));
    actionNewPattern->setMenuRole(QAction::NoRole);
    connect(actionNewPattern, &QAction::triggered, this, &MainWindow::New);

    QAction *actionOpenPattern = menu->addAction(tr("Open pattern"));
    actionOpenPattern->setMenuRole(QAction::NoRole);
    connect(actionOpenPattern, &QAction::triggered, this, &MainWindow::Open);

    QAction *actionOpenTape = menu->addAction(tr("Create/Edit measurements"));
    actionOpenTape->setMenuRole(QAction::NoRole);
    connect(actionOpenTape, &QAction::triggered, this, &MainWindow::CreateMeasurements);

    QAction *actionPreferences = menu->addAction(tr("Preferences"));
    actionPreferences->setMenuRole(QAction::NoRole);
    connect(actionPreferences, &QAction::triggered, this, &MainWindow::Preferences);

    extern void qt_mac_set_dock_menu(QMenu *);
    qt_mac_set_dock_menu(menu);
#endif
#endif //defined(Q_OS_MAC)
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::AddPP(const QString &PPName)
{
    if (doc->appendPP(PPName) == false)
    {
        qCDebug(vMainWindow, "Error creating pattern piece with the name %s.", qUtf8Printable(PPName));
        return;
    }

    if (comboBoxDraws->count() == 0)
    {
        sceneDraw->InitOrigins();
        sceneDetails->InitOrigins();
    }

    comboBoxDraws->blockSignals(true);
    comboBoxDraws->addItem(PPName);

    pattern->ClearGObjects();
    //Create single point
    ui->view->itemClicked(nullptr);//hide options previous tool
    const QString label = doc->GenerateLabel(LabelType::NewPatternPiece);
    const QPointF startPosition = StartPositionNewPP();
    VPointF *point = new VPointF(startPosition.x(), startPosition.y(), label, 5, 10);
    auto spoint = VToolBasePoint::Create(0, PPName, point, sceneDraw, doc, pattern, Document::FullParse,
                                         Source::FromGui);
    ui->view->itemClicked(spoint);

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

    // Show best for new PP
    ui->view->fitInView(doc->ActiveDrawBoundingRect(), Qt::KeepAspectRatio);
    VMainGraphicsView::NewSceneRect(ui->view->scene(), ui->view);
    ui->view->NewFactor(ui->view->transform().m11());

    ui->actionNewDraw->setEnabled(true);
    helpLabel->setText("");
}

//---------------------------------------------------------------------------------------------------------------------
QPointF MainWindow::StartPositionNewPP() const
{
    const qreal originX = 30.0;
    const qreal originY = 40.0;
    const qreal margin = 40.0;
    if (comboBoxDraws->count() > 1)
    {
        const QRectF rect = sceneDraw->VisibleItemsBoundingRect();
        if (rect.width() <= rect.height())
        {
            return QPointF(rect.width()+margin, originY);
        }
        else
        {
            return QPointF(originX, rect.height()+margin);
        }
    }
    else
    {
        return QPointF(originX, originY);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitScenes()
{
    sceneDraw = new VMainGraphicsScene();
    currentScene = sceneDraw;
    qApp->setCurrentScene(currentScene);
    connect(this, &MainWindow::EnableItemMove, sceneDraw, &VMainGraphicsScene::EnableItemMove);
    connect(this, &MainWindow::ItemsSelection, sceneDraw, &VMainGraphicsScene::ItemsSelection);

    connect(this, &MainWindow::EnableLabelSelection, sceneDraw, &VMainGraphicsScene::ToggleLabelSelection);
    connect(this, &MainWindow::EnablePointSelection, sceneDraw, &VMainGraphicsScene::TogglePointSelection);
    connect(this, &MainWindow::EnableLineSelection, sceneDraw, &VMainGraphicsScene::ToggleLineSelection);
    connect(this, &MainWindow::EnableArcSelection, sceneDraw, &VMainGraphicsScene::ToggleArcSelection);
    connect(this, &MainWindow::EnableSplineSelection, sceneDraw, &VMainGraphicsScene::ToggleSplineSelection);
    connect(this, &MainWindow::EnableSplinePathSelection, sceneDraw, &VMainGraphicsScene::ToggleSplinePathSelection);

    connect(this, &MainWindow::EnableLabelHover, sceneDraw, &VMainGraphicsScene::ToggleLabelHover);
    connect(this, &MainWindow::EnablePointHover, sceneDraw, &VMainGraphicsScene::TogglePointHover);
    connect(this, &MainWindow::EnableLineHover, sceneDraw, &VMainGraphicsScene::ToggleLineHover);
    connect(this, &MainWindow::EnableArcHover, sceneDraw, &VMainGraphicsScene::ToggleArcHover);
    connect(this, &MainWindow::EnableSplineHover, sceneDraw, &VMainGraphicsScene::ToggleSplineHover);
    connect(this, &MainWindow::EnableSplinePathHover, sceneDraw, &VMainGraphicsScene::ToggleSplinePathHover);

    connect(sceneDraw, &VMainGraphicsScene::mouseMove, this, &MainWindow::MouseMove);

    sceneDetails = new VMainGraphicsScene();
    connect(this, &MainWindow::EnableItemMove, sceneDetails, &VMainGraphicsScene::EnableItemMove);

    connect(this, &MainWindow::EnableNodeLabelSelection, sceneDetails, &VMainGraphicsScene::ToggleNodeLabelSelection);
    connect(this, &MainWindow::EnableNodePointSelection, sceneDetails, &VMainGraphicsScene::ToggleNodePointSelection);
    connect(this, &MainWindow::EnableDetailSelection, sceneDetails, &VMainGraphicsScene::ToggleDetailSelection);

    connect(this, &MainWindow::EnableNodeLabelHover, sceneDetails, &VMainGraphicsScene::ToggleNodeLabelHover);
    connect(this, &MainWindow::EnableNodePointHover, sceneDetails, &VMainGraphicsScene::ToggleNodePointHover);
    connect(this, &MainWindow::EnableDetailHover, sceneDetails, &VMainGraphicsScene::ToggleDetailHover);

    connect(sceneDetails, &VMainGraphicsScene::mouseMove, this, &MainWindow::MouseMove);

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
QSharedPointer<VMeasurements> MainWindow::OpenMeasurementFile(const QString &path)
{
    QSharedPointer<VMeasurements> m;
    if (path.isEmpty())
    {
        return m;
    }

    try
    {
        m = QSharedPointer<VMeasurements>(new VMeasurements(pattern));
        m->setXMLContent(path);

        if (m->Type() == MeasurementsType::Unknown)
        {
            VException e(tr("Measurement file has unknown format."));
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

        if (not m->IsDefinedKnownNamesValid())
        {
            VException e(tr("Measurement file contains invalid known measurement(s)."));
            throw e;
        }

        const QStringList mList = m->ListAll();
        const QStringList pList = doc->ListMeasurements();

        const QSet<QString> match = pList.toSet().subtract(mList.toSet());
        if (not match.isEmpty())
        {
            VException e(tr("Measurement file doesn't include all required measurements."));
            e.AddMoreInformation(tr("Please, additionaly provide: %1").arg(QStringList(match.toList()).join(", ")));
            throw e;
        }

        if (m->Type() == MeasurementsType::Standard)
        {
            if (m->MUnit() == Unit::Inch)
            {
                qCCritical(vMainWindow, "%s\n\n%s", qUtf8Printable(tr("Wrong units.")),
                          qUtf8Printable(tr("Application doesn't support standard table with inches.")));
                m->clear();
                if (not VApplication::IsGUIMode())
                {
                    qApp->exit(V_EX_DATAERR);
                }
                return m;
            }
        }
    }
    catch (VException &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        m->clear();
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return m;
    }
    return m;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::LoadMeasurements(const QString &path)
{
    QSharedPointer<VMeasurements> m = OpenMeasurementFile(path);

    if (m->isNull())
    {
        return false;
    }

    if (m->Type() == MeasurementsType::Standard)
    {
        m->SetDataSize();
        m->SetDataHeight();
    }

    try
    {
        qApp->setPatternType(m->Type());
        ToolBarOption();
        pattern->ClearVariables(VarType::Measurement);
        m->ReadMeasurements();
    }
    catch (VExceptionEmptyParameter &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::UpdateMeasurements(const QString &path, int size, int height)
{
    QSharedPointer<VMeasurements> m = OpenMeasurementFile(path);

    if (m->isNull())
    {
        return false;
    }

    if (qApp->patternType() != m->Type())
    {
        qCCritical(vMainWindow, "%s", qUtf8Printable(tr("Measurement files types have not match.")));
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_DATAERR);
        }
        return false;
    }

    if (m->Type() == MeasurementsType::Standard)
    {
        pattern->SetSize(size);
        pattern->SetHeight(height);
    }

    try
    {
        pattern->ClearVariables(VarType::Measurement);
        m->ReadMeasurements();
    }
    catch (VExceptionEmptyParameter &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }
    return true;
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

        connect(scene, &VMainGraphicsScene::ChoosedObject, dialogTool.data(), &DialogTool::ChosenObject);
        connect(scene, &VMainGraphicsScene::SelectedObject, dialogTool.data(), &DialogTool::SelectedObject);
        connect(dialogTool.data(), &DialogTool::DialogClosed, this, closeDialogSlot);
        connect(dialogTool.data(), &DialogTool::ToolTip, this, &MainWindow::ShowToolTip);
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
        auto cursorResource = cursor;
        if (qApp->devicePixelRatio() >= 2)
        {
            // Try to load HiDPI versions of the cursors if availible
            auto cursorHidpiResource = QString(cursor).replace(".png", "@2x.png");
            if (QFileInfo(cursorResource).exists())
            {
                cursorResource = cursorHidpiResource;
            }
        }
        QPixmap pixmap(cursorResource);
        QCursor cur(pixmap, 2, 3);
        ui->view->setCursor(cur);
        ui->view->setShowToolOptions(false);
        helpLabel->setText(toolTip);
        dialogTool = new Dialog(pattern, NULL_ID, this);

        if (t == Tool::Midpoint)
        {
            dialogTool->Build(t);
        }

        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(currentScene);
        SCASSERT(scene != nullptr);

        connect(scene, &VMainGraphicsScene::ChoosedObject, dialogTool.data(), &DialogTool::ChosenObject);
        connect(scene, &VMainGraphicsScene::SelectedObject, dialogTool.data(), &DialogTool::SelectedObject);
        connect(dialogTool.data(), &DialogTool::DialogClosed, this, closeDialogSlot);
        connect(dialogTool.data(), &DialogTool::DialogApplied, this, applyDialogSlot);
        connect(dialogTool.data(), &DialogTool::ToolTip, this, &MainWindow::ShowToolTip);
        connect(ui->view, &VMainGraphicsView::MouseRelease, [this](){EndVisualization(true);});
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
    SCASSERT(not dialogTool.isNull());
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
    SCASSERT(not dialogTool.isNull());
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
    SCASSERT(not dialogTool.isNull());
    QGraphicsItem *tool = dynamic_cast<QGraphicsItem *>(dialogTool->GetAssociatedTool());
    ui->view->itemClicked(tool);
    if (dialogTool->GetAssociatedTool() != nullptr)
    {
        VDrawTool *vtool= static_cast<VDrawTool *>(dialogTool->GetAssociatedTool());
        vtool->DialogLinkDestroy();
        connect(vtool, &VDrawTool::ToolTip, this, &MainWindow::ShowToolTip);
    }
    ArrowTool();
    // If insert not to the end of file call lite parse
    if (doc->getCursor() > 0)
    {
        doc->LiteParseTree(Document::LiteParse);
        if (dialogHistory)
        {
            dialogHistory->UpdateHistory();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ApplyDialog handle apply in dialog
 */
template <typename DrawTool>
void MainWindow::ApplyDialog()
{
    SCASSERT(not dialogTool.isNull());

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
    ToolSelectPointByRelease();
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
    ToolSelectPointByRelease();
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
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogAlongLine>(checked, Tool::AlongLine, ":/cursor/alongline_cursor.png",
                                            tr("Select point"), &MainWindow::ClosedDialogWithApply<VToolAlongLine>,
                                            &MainWindow::ApplyDialog<VToolAlongLine>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolMidpoint(bool checked)
{
    ToolSelectPointByRelease();
    // Reuse DialogAlongLine and VToolAlongLine but with different cursor
    SetToolButtonWithApply<DialogAlongLine>(checked, Tool::Midpoint, ":/cursor/midpoint_cursor.png",
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
    ToolSelectPointByRelease();
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
    ToolSelectPointByRelease();
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
    ToolSelectPointByRelease();
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
    ToolSelectPointByRelease();
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
    ToolSelectPointByPress();
    SetToolButtonWithApply<DialogSpline>(checked, Tool::Spline, ":/cursor/spline_cursor.png",
                                         tr("Select first point curve"),
                                         &MainWindow::ClosedDialogWithApply<VToolSpline>,
                                         &MainWindow::ApplyDialog<VToolSpline>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolCubicBezier(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogCubicBezier>(checked, Tool::CubicBezier, ":/cursor/cubic_bezier_cursor.png",
                                              tr("Select first curve point"),
                                              &MainWindow::ClosedDialogWithApply<VToolCubicBezier>,
                                              &MainWindow::ApplyDialog<VToolCubicBezier>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutSpline handler tool CutSpline.
 * @param checked true - button is checked
 */
void MainWindow::ToolCutSpline(bool checked)
{
    ToolSelectSpline();
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
    ToolSelectPointByRelease();
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
    ToolSelectPointByPress();
    SetToolButtonWithApply<DialogSplinePath>(checked, Tool::SplinePath, ":/cursor/splinepath_cursor.png",
                                             tr("Select point of curve path"),
                                             &MainWindow::ClosedDialogWithApply<VToolSplinePath>,
                                             &MainWindow::ApplyDialog<VToolSplinePath>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolCubicBezierPath(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogCubicBezierPath>(checked, Tool::CubicBezierPath,
                                                  ":/cursor/cubic_bezier_path_cursor.png",
                                                  tr("Select point of cubic bezier path"),
                                                  &MainWindow::ClosedDialogWithApply<VToolCubicBezierPath>,
                                                  &MainWindow::ApplyDialog<VToolCubicBezierPath>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutSplinePath handler tool CutSplinePath.
 * @param checked true - button is checked
 */
void MainWindow::ToolCutSplinePath(bool checked)
{
    ToolSelectSplinePath();
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
    ToolSelectPointByRelease();
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
    ToolSelectAllDrawObjects();
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
    ToolSelectPointByRelease();
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
    ToolSelectPointByRelease();
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
    ToolSelectPointByRelease();
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
    ToolSelectDetail();
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
void MainWindow::ToolGroup(bool checked)
{
    ToolSelectGroupObjects();
    SetToolButton<DialogGroup>(checked, Tool::Group, ":/cursor/group_plus_cursor.png",
                               tr("Select one or more objects, <b>Enter</b> - finish creation"),
                               &MainWindow::ClosedDialogGroup);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolRotation(bool checked)
{
    ToolSelectOperationObjects();
    SetToolButtonWithApply<DialogRotation>(checked, Tool::Rotation,
                                           ":/cursor/rotation_cursor.png",
                                           tr("Select one or more objects, <b>Enter</b> - confirm selection"),
                                           &MainWindow::ClosedDialogWithApply<VToolRotation>,
                                           &MainWindow::ApplyDialog<VToolRotation>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedDialogGroup(int result)
{
    SCASSERT(dialogTool != nullptr);
    if (result == QDialog::Accepted)
    {
        DialogGroup *dialog = qobject_cast<DialogGroup*>(dialogTool);
        SCASSERT(dialog != nullptr);
        const QDomElement group = doc->CreateGroup(pattern->getNextId(), dialog->GetName(), dialog->GetGroup());
        if (not group.isNull())
        {
            AddGroup *addGroup = new AddGroup(group, doc);
            connect(addGroup, &AddGroup::UpdateGroups, groupsWidget, &VWidgetGroups::UpdateGroups);
            qApp->getUndoStack()->push(addGroup);
        }
    }
    ArrowTool();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutArc handler tool cutArc.
 * @param checked true - button checked.
 */
void MainWindow::ToolCutArc(bool checked)
{
    ToolSelectArc();
    SetToolButtonWithApply<DialogCutArc>(checked, Tool::CutArc, ":/cursor/arc_cut_cursor.png",
                                         tr("Select arc"), &MainWindow::ClosedDialogWithApply<VToolCutArc>,
                                         &MainWindow::ApplyDialog<VToolCutArc>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolLineIntersectAxis(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogLineIntersectAxis>(checked, Tool::LineIntersectAxis,
                                                    ":/cursor/line_intersect_axis_cursor.png",
                                                    tr("Select first point of line"),
                                                    &MainWindow::ClosedDialogWithApply<VToolLineIntersectAxis>,
                                                    &MainWindow::ApplyDialog<VToolLineIntersectAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolCurveIntersectAxis(bool checked)
{
    ToolSelectAllDrawObjects();
    SetToolButtonWithApply<DialogCurveIntersectAxis>(checked, Tool::CurveIntersectAxis,
                                                     ":/cursor/curve_intersect_axis_cursor.png",
                                                     tr("Select curve"),
                                                     &MainWindow::ClosedDialogWithApply<VToolCurveIntersectAxis>,
                                                     &MainWindow::ApplyDialog<VToolCurveIntersectAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolArcIntersectAxis(bool checked)
{
    ToolSelectAllDrawObjects();
    // Reuse ToolCurveIntersectAxis but with different cursor and tool tip
    SetToolButtonWithApply<DialogCurveIntersectAxis>(checked, Tool::ArcIntersectAxis,
                                                     ":/cursor/arc_intersect_axis_cursor.png",
                                                     tr("Select arc"),
                                                     &MainWindow::ClosedDialogWithApply<VToolCurveIntersectAxis>,
                                                     &MainWindow::ApplyDialog<VToolCurveIntersectAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointOfIntersectionArcs(bool checked)
{
    ToolSelectArc();
    SetToolButtonWithApply<DialogPointOfIntersectionArcs>(checked, Tool::PointOfIntersectionArcs,
                                                          "://cursor/point_of_intersection_arcs.png",
                                                          tr("Select first an arc"),
                                                       &MainWindow::ClosedDialogWithApply<VToolPointOfIntersectionArcs>,
                                                          &MainWindow::ApplyDialog<VToolPointOfIntersectionArcs>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointOfIntersectionCircles(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogPointOfIntersectionCircles>(checked, Tool::PointOfIntersectionCircles,
                                                             "://cursor/point_of_intersection_circles.png",
                                                             tr("Select first circle center"),
                                                    &MainWindow::ClosedDialogWithApply<VToolPointOfIntersectionCircles>,
                                                             &MainWindow::ApplyDialog<VToolPointOfIntersectionCircles>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointOfIntersectionCurves(bool checked)
{
    ToolSelectCurve();
    SetToolButtonWithApply<DialogPointOfIntersectionCurves>(checked, Tool::PointOfIntersectionCurves,
                                                             "://cursor/intersection_curves_cursor.png",
                                                             tr("Select first curve"),
                                                    &MainWindow::ClosedDialogWithApply<VToolPointOfIntersectionCurves>,
                                                             &MainWindow::ApplyDialog<VToolPointOfIntersectionCurves>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointFromCircleAndTangent(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogPointFromCircleAndTangent>(checked, Tool::PointFromCircleAndTangent,
                                                            "://cursor/point_from_circle_and_tangent_cursor.png",
                                                            tr("Select point on tangent"),
                                                    &MainWindow::ClosedDialogWithApply<VToolPointFromCircleAndTangent>,
                                                            &MainWindow::ApplyDialog<VToolPointFromCircleAndTangent>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolPointFromArcAndTangent(bool checked)
{
    ToolSelectPointArc();
    SetToolButtonWithApply<DialogPointFromArcAndTangent>(checked, Tool::PointFromArcAndTangent,
                                                         "://cursor/point_from_arc_and_tangent_cursor.png",
                                                         tr("Select point on tangent"),
                                                        &MainWindow::ClosedDialogWithApply<VToolPointFromArcAndTangent>,
                                                         &MainWindow::ApplyDialog<VToolPointFromArcAndTangent>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolArcWithLength(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogArcWithLength>(checked, Tool::ArcWithLength,
                                                "://cursor/arc_with_length_cursor.png",
                                                tr("Select point of the center of the arc"),
                                                &MainWindow::ClosedDialogWithApply<VToolArcWithLength>,
                                                &MainWindow::ApplyDialog<VToolArcWithLength>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolTrueDarts(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogTrueDarts>(checked, Tool::TrueDarts,
                                                "://cursor/true_darts_cursor.png",
                                                tr("Select the first base line point"),
                                                &MainWindow::ClosedDialogWithApply<VToolTrueDarts>,
                                            &MainWindow::ApplyDialog<VToolTrueDarts>);
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
void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
        undoAction->setText(tr("&Undo"));
        redoAction->setText(tr("&Redo"));
        helpLabel->setText(QObject::tr("Changes applied."));
        patternPieceLabel->setText(tr("Pattern Piece:"));
        UpdateWindowTitle();
    }
    // remember to call base class implementation
    QMainWindow::changeEvent(event);
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
    SetLayoutModeActions();
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
        SetLayoutModeActions();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::LoadIndividual()
{
    const QString filter = tr("Individual measurements (*.vit);;Standard measurements (*.vst)");
    //Use standard path to individual measurements
    const QString path = qApp->ValentinaSettings()->GetPathIndividualMeasurements();
    const QString mPath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);

    if (not mPath.isEmpty())
    {
        if (LoadMeasurements(mPath))
        {
            if (not doc->MPath().isEmpty())
            {
                watcher->removePath(AbsoluteMPath(curFile, doc->MPath()));
            }
            ui->actionUnloadMeasurements->setEnabled(true);
            doc->SetPath(RelativeMPath(curFile, mPath));
            watcher->addPath(mPath);
            PatternChangesWereSaved(false);
            ui->actionShowM->setEnabled(true);
            helpLabel->setText(tr("Measurements loaded"));
            doc->LiteParseTree(Document::LiteParse);

            UpdateWindowTitle();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::LoadStandard()
{
    const QString filter = tr("Standard measurements (*.vst);;Individual measurements (*.vit)");
    //Use standard path to standard measurements
    const QString path = qApp->ValentinaSettings()->GetPathStandardMeasurements();
    const QString mPath = QFileDialog::getOpenFileName(this, tr("Open file"), path, filter);

    if (not mPath.isEmpty())
    {
        QString hText;
        if (not gradationHeights.isNull())
        {
            hText = gradationHeights->currentText();
        }
        QString sText;
        if (not gradationSizes.isNull())
        {
            sText = gradationSizes->currentText();
        }

        if(LoadMeasurements(mPath))
        {
            if (not doc->MPath().isEmpty())
            {
                watcher->removePath(AbsoluteMPath(curFile, doc->MPath()));
            }
            ui->actionUnloadMeasurements->setEnabled(true);
            doc->SetPath(RelativeMPath(curFile, mPath));
            watcher->addPath(mPath);
            PatternChangesWereSaved(false);
            ui->actionShowM->setEnabled(true);
            helpLabel->setText(tr("Measurements loaded"));
            doc->LiteParseTree(Document::LiteParse);

            UpdateWindowTitle();

            if (qApp->patternType() == MeasurementsType::Standard)
            {
                if (not hText.isEmpty() && not gradationSizes.isNull())
                {
                    gradationHeights->setCurrentText(hText);
                }

                if (not sText.isEmpty() && not gradationSizes.isNull())
                {
                    gradationSizes->setCurrentText(sText);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UnloadMeasurements()
{
    if (doc->MPath().isEmpty())
    {
        ui->actionUnloadMeasurements->setDisabled(true);
        return;
    }

    if (doc->ListMeasurements().isEmpty())
    {
        watcher->removePath(AbsoluteMPath(curFile, doc->MPath()));
        doc->SetPath(QString());
        PatternChangesWereSaved(false);
        ui->actionShowM->setEnabled(false);
        ui->actionUnloadMeasurements->setDisabled(true);
        helpLabel->setText(tr("Measurements unloaded"));

        UpdateWindowTitle();
    }
    else
    {
        qCWarning(vMainWindow, "%s",
                  qUtf8Printable(tr("Couldn't unload measurements. Some of them are used in the pattern.")));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowMeasurements()
{
    if (not doc->MPath().isEmpty())
    {
        const QString absoluteMPath = AbsoluteMPath(curFile, doc->MPath());

        QStringList arguments;
        if (qApp->patternType() == MeasurementsType::Standard)
        {
            arguments = QStringList()
                    << absoluteMPath
                    << "-u"
                    << VDomDocument::UnitsToStr(qApp->patternUnit())
                    << "-e"
                    << QString().setNum(static_cast<int>(UnitConvertor(pattern->height(), doc->MUnit(), Unit::Cm)))
                    << "-s"
                    << QString().setNum(static_cast<int>(UnitConvertor(pattern->size(), doc->MUnit(), Unit::Cm)));
        }
        else
        {
            arguments = QStringList() << absoluteMPath
                                      << "-u"
                                      << VDomDocument::UnitsToStr(qApp->patternUnit());
        }
        const QString tape = qApp->TapeFilePath();
        const QString workingDirectory = QFileInfo(tape).absoluteDir().absolutePath();
        QProcess::startDetached(tape, arguments, workingDirectory);
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
        mChangesAsked = false;
    }
    else
    {
        for(int i=0; i<=1000; i=i+10)
        {
            if (checkFile.exists())
            {
                mChanges = true;
                mChangesAsked = false;
                break;
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    UpdateWindowTitle();
    ui->actionSyncMeasurements->setEnabled(mChanges);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SyncMeasurements()
{
    if (mChanges)
    {
        const QString path = AbsoluteMPath(curFile, doc->MPath());
        if(UpdateMeasurements(path, static_cast<int>(pattern->size()), static_cast<int>(pattern->height())))
        {
            if (not watcher->files().contains(path))
            {
                watcher->addPath(path);
            }
            const QString msg = tr("Measurements was synced");
            qCDebug(vMainWindow, "%s", qUtf8Printable(msg));
            helpLabel->setText(msg);
            VWidgetPopup::PopupMessage(this, msg);
            doc->LiteParseTree(Document::LiteParse);
            mChanges = false;
            mChangesAsked = true;
            UpdateWindowTitle();
            ui->actionSyncMeasurements->setEnabled(mChanges);
        }
        else
        {
            qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Couldn't sync measurements.")));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
#if defined(Q_OS_MAC)
void MainWindow::OpenAt(QAction *where)
{
    const QString path = curFile.left(curFile.indexOf(where->text())) + where->text();
    if (path == curFile)
    {
        return;
    }
    QProcess process;
    process.start(QStringLiteral("/usr/bin/open"), QStringList() << path, QIODevice::ReadOnly);
    process.waitForFinished();
}
#endif //defined(Q_OS_MAC)

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBarOption enable option toolbar.
 */
void MainWindow::ToolBarOption()
{
    ui->toolBarOption->clear();
    if (not mouseCoordinate.isNull())
    {
        delete mouseCoordinate;
    }
    if (not gradationHeights.isNull())
    {
        delete gradationHeights;
    }
    if (not gradationSizes.isNull())
    {
        delete gradationSizes;
    }
    if (not gradationHeightsLabel.isNull())
    {
        delete gradationHeightsLabel;
    }
    if (not gradationSizesLabel.isNull())
    {
        delete gradationSizesLabel;
    }

    if (qApp->patternType() == MeasurementsType::Standard)
    {
        const QStringList listHeights = VMeasurement::ListHeights(doc->GetGradationHeights(), qApp->patternUnit());
        const QStringList listSizes = VMeasurement::ListSizes(doc->GetGradationSizes(), qApp->patternUnit());

        gradationHeightsLabel = new QLabel(tr("Height:"), this);
        gradationHeights = SetGradationList(gradationHeightsLabel, listHeights);

        // set default height
        SetDefaultHeight();

        connect(gradationHeights.data(),
                static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::ChangedHeight);

        gradationSizesLabel = new QLabel(tr("Size:"), this);
        gradationSizes = SetGradationList(gradationSizesLabel, listSizes);

        // set default size
        SetDefaultSize();

        connect(gradationSizes.data(),
                static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::ChangedSize);

        ui->toolBarOption->addSeparator();
    }

    mouseCoordinate = new QLabel(QString("0, 0 (%1)").arg(doc->UnitsToStr(qApp->patternUnit(), true)));
    ui->toolBarOption->addWidget(mouseCoordinate);
}

//---------------------------------------------------------------------------------------------------------------------
QComboBox *MainWindow::SetGradationList(QLabel *label, const QStringList &list)
{
    ui->toolBarOption->addWidget(label);

    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItems(list);
    ui->toolBarOption->addWidget(comboBox);

    return comboBox;
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
    patternPieceLabel = new QLabel(tr("Pattern Piece:"));
    ui->toolBarDraws->addWidget(patternPieceLabel);

    // By using Qt UI Designer we can't add QComboBox to toolbar
    comboBoxDraws = new QComboBox;
    ui->toolBarDraws->addWidget(comboBoxDraws);
    comboBoxDraws->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    comboBoxDraws->setEnabled(false);
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this](int index){ChangePP(index);});

    connect(ui->actionOptionDraw, &QAction::triggered, [this]()
    {
        const QString activDraw = doc->GetNameActivPP();
        const QString nameDraw = PatternPieceName(activDraw);
        if (nameDraw.isEmpty())
        {
            return;
        }
        RenamePP *renamePP = new RenamePP(doc, nameDraw, comboBoxDraws);
        qApp->getUndoStack()->push(renamePP);
    });
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
    toolButtonPointerList.append(ui->toolButtonPointerPoint);
    toolButtonPointerList.append(ui->toolButtonPointerLine);
    toolButtonPointerList.append(ui->toolButtonPointerCurve);
    toolButtonPointerList.append(ui->toolButtonPointerArc);
    toolButtonPointerList.append(ui->toolButtonPointerDetail);
    toolButtonPointerList.append(ui->toolButtonPointerOperations);

    for (auto pointer : toolButtonPointerList)
    {
        connect(pointer, &QToolButton::clicked, this, &MainWindow::ArrowTool);
    }

    connect(ui->toolButtonEndLine, &QToolButton::clicked, this, &MainWindow::ToolEndLine);
    connect(ui->toolButtonLine, &QToolButton::clicked, this, &MainWindow::ToolLine);
    connect(ui->toolButtonAlongLine, &QToolButton::clicked, this, &MainWindow::ToolAlongLine);
    connect(ui->toolButtonShoulderPoint, &QToolButton::clicked, this, &MainWindow::ToolShoulderPoint);
    connect(ui->toolButtonNormal, &QToolButton::clicked, this, &MainWindow::ToolNormal);
    connect(ui->toolButtonBisector, &QToolButton::clicked, this, &MainWindow::ToolBisector);
    connect(ui->toolButtonLineIntersect, &QToolButton::clicked, this, &MainWindow::ToolLineIntersect);
    connect(ui->toolButtonSpline, &QToolButton::clicked, this, &MainWindow::ToolSpline);
    connect(ui->toolButtonCubicBezier, &QToolButton::clicked, this, &MainWindow::ToolCubicBezier);
    connect(ui->toolButtonArc, &QToolButton::clicked, this, &MainWindow::ToolArc);
    connect(ui->toolButtonSplinePath, &QToolButton::clicked, this, &MainWindow::ToolSplinePath);
    connect(ui->toolButtonCubicBezierPath, &QToolButton::clicked, this, &MainWindow::ToolCubicBezierPath);
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
    connect(ui->toolButtonArcIntersectAxis, &QToolButton::clicked, this, &MainWindow::ToolArcIntersectAxis);
    connect(ui->toolButtonLayoutSettings, &QToolButton::clicked, this, &MainWindow::ToolLayoutSettings);
    connect(ui->toolButtonPointOfIntersectionArcs, &QToolButton::clicked, this,
            &MainWindow::ToolPointOfIntersectionArcs);
    connect(ui->toolButtonPointOfIntersectionCircles, &QToolButton::clicked, this,
            &MainWindow::ToolPointOfIntersectionCircles);
    connect(ui->toolButtonIntersectionCurves, &QToolButton::clicked, this, &MainWindow::ToolPointOfIntersectionCurves);
    connect(ui->toolButtonPointFromCircleAndTangent, &QToolButton::clicked, this,
            &MainWindow::ToolPointFromCircleAndTangent);
    connect(ui->toolButtonPointFromArcAndTangent, &QToolButton::clicked, this, &MainWindow::ToolPointFromArcAndTangent);
    connect(ui->toolButtonArcWithLength, &QToolButton::clicked, this, &MainWindow::ToolArcWithLength);
    connect(ui->toolButtonTrueDarts, &QToolButton::clicked, this, &MainWindow::ToolTrueDarts);
    connect(ui->toolButtonGroup, &QToolButton::clicked, this, &MainWindow::ToolGroup);
    connect(ui->toolButtonRotation, &QToolButton::clicked, this, &MainWindow::ToolRotation);
    connect(ui->toolButtonMidpoint, &QToolButton::clicked, this, &MainWindow::ToolMidpoint);
    connect(ui->toolButtonLayoutExportAs, &QToolButton::clicked, this, &MainWindow::ExportLayoutAs);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseMove save mouse position and show user.
 * @param scenePos position mouse.
 */
void MainWindow::MouseMove(const QPointF &scenePos)
{
    if (mouseCoordinate)
    {
        //: Coords in status line: "X, Y (units)"
        mouseCoordinate->setText(QString("%1, %2 (%3)").arg(static_cast<qint32>(qApp->fromPixel(scenePos.x())))
                                                       .arg(static_cast<qint32>(qApp->fromPixel(scenePos.y())))
                                                       .arg(doc->UnitsToStr(qApp->patternUnit(), true)));
    }
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
/**
 * @brief CancelTool cancel tool.
 */
void MainWindow::CancelTool()
{
    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 45, "Not all tools was handled.");

    qCDebug(vMainWindow, "Canceling tool.");
    delete dialogTool;
    qCDebug(vMainWindow, "Dialog closed.");

    currentScene->setFocus(Qt::OtherFocusReason);
    currentScene->clearSelection();

    switch ( currentTool )
    {
        case Tool::Arrow:
            for (auto pointer : toolButtonPointerList)
            {
                pointer->setChecked(false);
            }
            helpLabel->setText("");

            // Crash: using CRTL+Z while using line tool.
            // related bug report:
            // https://bitbucket.org/dismine/valentina/issues/454/crash-using-crtl-z-while-using-line-tool
            undoAction->setEnabled(false);
            redoAction->setEnabled(false);
            return;
        case Tool::BasePoint:
        case Tool::SinglePoint:
        case Tool::DoublePoint:
        case Tool::LinePoint:
        case Tool::AbstractSpline:
        case Tool::Cut:
        case Tool::LAST_ONE_DO_NOT_USE:
        case Tool::NodePoint:
        case Tool::NodeArc:
        case Tool::NodeSpline:
        case Tool::NodeSplinePath:
            Q_UNREACHABLE(); //-V501
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
        case Tool::Midpoint:
            ui->toolButtonMidpoint->setChecked(false);
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
        case Tool::CubicBezier:
            ui->toolButtonCubicBezier->setChecked(false);
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
        case Tool::CubicBezierPath:
            ui->toolButtonCubicBezierPath->setChecked(false);
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
            break;
        case Tool::ArcIntersectAxis:
            ui->toolButtonArcIntersectAxis->setChecked(false);
            break;
        case Tool::PointOfIntersectionArcs:
            ui->toolButtonPointOfIntersectionArcs->setChecked(false);
            break;
        case Tool::PointOfIntersectionCircles:
            ui->toolButtonPointOfIntersectionCircles->setChecked(false);
            break;
        case Tool::PointOfIntersectionCurves:
            ui->toolButtonIntersectionCurves->setChecked(false);
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
        case Tool::Group:
            ui->toolButtonGroup->setChecked(false);
            break;
        case Tool::Rotation:
            ui->toolButtonRotation->setChecked(false);
            break;
    }

    // Crash: using CRTL+Z while using line tool.
    // related bug report:
    // https://bitbucket.org/dismine/valentina/issues/454/crash-using-crtl-z-while-using-line-tool
    undoAction->setEnabled(qApp->getUndoStack()->canUndo());
    redoAction->setEnabled(qApp->getUndoStack()->canRedo());
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowTool enable arrow tool.
 */
void  MainWindow::ArrowTool()
{
    qCDebug(vMainWindow, "Arrow tool.");
    CancelTool();
    for (auto pointer : toolButtonPointerList)
    {
        pointer->setChecked(true);
    }
    currentTool = Tool::Arrow;
    emit EnableItemMove(true);
    emit ItemsSelection(SelectionType::ByMouseRelease);

    // Only true for rubber band selection
    emit EnableLabelSelection(true);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);
    emit EnableNodeLabelSelection(true);
    emit EnableNodePointSelection(true);
    emit EnableDetailSelection(true);// Disable when done visualization details

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(true);
    emit EnableArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);
    emit EnableNodeLabelHover(true);
    emit EnableNodePointHover(true);
    emit EnableDetailHover(true);

    ui->view->AllowRubberBand(true);

    RestoreOverrideCursor(cursorArrowCloseHand);
    RestoreOverrideCursor(cursorArrowOpenHand);
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
        ArrowTool();

        leftGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_left_to_right_arrow.png"));
        rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));

        ui->actionDraw->setChecked(true);
        ui->actionDetails->setChecked(false);
        ui->actionLayout->setChecked(false);
        SaveCurrentScene();

        currentScene = sceneDraw;
        ui->view->setScene(currentScene);
        connect(ui->view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor,
                Qt::UniqueConnection);
        RestoreCurrentScene();

        mode = Draw::Calculation;
        comboBoxDraws->setCurrentIndex(currentDrawIndex);//restore current pattern peace
        drawMode = true;

        SetEnableTool(true);
        SetEnableWidgets(true);
        ui->toolBox->setCurrentIndex(currentToolBoxIndex);

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            ui->toolBarOption->setVisible(true);
        }

        ui->dockWidgetLayoutPages->setVisible(false);
        ui->dockWidgetToolOptions->setVisible(isDockToolOptionsVisible);

        ui->dockWidgetGroups->setWidget(groupsWidget);
        ui->dockWidgetGroups->setVisible(isDockGroupsVisible);
        ui->dockWidgetGroups->setToolTip(tr("Contains all visibility groups"));
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
        ArrowTool();

        if(drawMode)
        {
            currentDrawIndex = comboBoxDraws->currentIndex();//save current pattern piece
            drawMode = false;
        }
        comboBoxDraws->setCurrentIndex(comboBoxDraws->count()-1);// Need to get data about all details

        leftGoToStage->setPixmap(QPixmap("://icon/24x24/right_to_left_arrow.png"));
        rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));

        ui->actionDraw->setChecked(false);
        ui->actionDetails->setChecked(true);
        ui->actionLayout->setChecked(false);

        if(not qApp->getOpeningPattern())
        {
            if (pattern->DataDetails()->count() == 0)
            {
                QMessageBox::information(this, tr("Detail mode"), tr("You can't use now the Detail mode. "
                                                                     "Please, create at least one workpiece."),
                                         QMessageBox::Ok, QMessageBox::Ok);
                ActionDraw(true);
                return;
            }
        }

        detailsWidget->UpdateList();

        qCDebug(vMainWindow, "Show details scene");
        SaveCurrentScene();

        currentScene = sceneDetails;
        ui->view->itemClicked(nullptr);
        ui->view->setScene(currentScene);
        disconnect(ui->view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
        RestoreCurrentScene();

        if (mode == Draw::Calculation)
        {
            currentToolBoxIndex = ui->toolBox->currentIndex();
        }
        mode = Draw::Modeling;
        SetEnableTool(true);
        SetEnableWidgets(true);
        ui->toolBox->setCurrentIndex(5);

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            ui->toolBarOption->setVisible(true);
        }

        ui->dockWidgetLayoutPages->setVisible(false);
        ui->dockWidgetToolOptions->setVisible(isDockToolOptionsVisible);

        ui->dockWidgetGroups->setWidget(detailsWidget);
        ui->dockWidgetGroups->setVisible(isDockGroupsVisible);
        ui->dockWidgetGroups->setToolTip(tr("Show which details will go in layout"));

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
        ArrowTool();

        if(drawMode)
        {
            currentDrawIndex = comboBoxDraws->currentIndex();//save current pattern piece
            drawMode = false;
        }
        comboBoxDraws->setCurrentIndex(comboBoxDraws->count()-1);// Need to get data about all details

        leftGoToStage->setPixmap(QPixmap("://icon/24x24/right_to_left_arrow.png"));
        rightGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_right_to_left_arrow.png"));

        ui->actionDraw->setChecked(false);
        ui->actionDetails->setChecked(false);
        ui->actionLayout->setChecked(true);

        QHash<quint32, VDetail> details;
        if(not qApp->getOpeningPattern())
        {
            const QHash<quint32, VDetail> *allDetails = pattern->DataDetails();
            if (allDetails->count() == 0)
            {
                QMessageBox::information(this, tr("Layout mode"), tr("You can't use now the Layout mode. "
                                                                     "Please, create at least one workpiece."),
                                         QMessageBox::Ok, QMessageBox::Ok);
                ActionDraw(true);
                return;
            }
            else
            {
                QHash<quint32, VDetail>::const_iterator i = allDetails->constBegin();
                while (i != allDetails->constEnd())
                {
                    if (i.value().IsInLayout())
                    {
                        details.insert(i.key(), i.value());
                    }
                    ++i;
                }

                if (details.count() == 0)
                {
                    QMessageBox::information(this, tr("Layout mode"),  tr("You can't use now the Layout mode. Please, "
                                                                          "include at least one detail in layout."),
                                             QMessageBox::Ok, QMessageBox::Ok);
                    mode == Draw::Calculation ? ActionDraw(true) : ActionDetails(true);
                    return;
                }
            }
        }

        comboBoxDraws->setCurrentIndex(-1);// Hide pattern pieces

        qCDebug(vMainWindow, "Show layout scene");

        SaveCurrentScene();

        PrepareDetailsForLayout(&details);

        currentScene = tempSceneLayout;
        ui->view->itemClicked(nullptr);
        ui->view->setScene(currentScene);
        disconnect(ui->view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);

        if (mode == Draw::Calculation)
        {
            currentToolBoxIndex = ui->toolBox->currentIndex();
        }
        mode = Draw::Layout;
        SetEnableTool(true);
        SetEnableWidgets(true);
        ui->toolBox->setCurrentIndex(6);

        mouseCoordinate->setText("");

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            ui->toolBarOption->setVisible(false);
        }

        ui->dockWidgetLayoutPages->setVisible(true);

        ui->dockWidgetToolOptions->blockSignals(true);
        ui->dockWidgetToolOptions->setVisible(false);
        ui->dockWidgetToolOptions->blockSignals(false);

        ui->dockWidgetGroups->blockSignals(true);
        ui->dockWidgetGroups->setVisible(false);
        ui->dockWidgetGroups->blockSignals(false);

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

    if (QFileInfo(fileName).exists())
    {
        // Temporary try to lock the file before saving
        // Also help to rewite current read-only pattern
        VLockGuard<char> tmp(fileName);
        if (not tmp.IsLocked())
        {
            qCCritical(vMainWindow, "%s",
                       qUtf8Printable(tr("Failed to lock. This file already opened in another window.")));
            return false;
        }
    }

    // Need for restoring previous state in case of failure
    const bool wasModified = doc->IsModified(); // Need because SetReadOnly() will change internal state
    const bool readOnly = doc->IsReadOnly();

    doc->SetReadOnly(false);// Save as... disable read only state
    QString error;
    const bool result = SavePattern(fileName, error);
    if (result == false)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save file"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();

        // Restoring previous state
        doc->SetReadOnly(readOnly);
        doc->SetModified(wasModified);

        return result;
    }

    patternReadOnly = false;

    qCDebug(vMainWindow, "Locking file");
    VlpCreateLock(lock, fileName);

    if (lock->IsLocked())
    {
        qCDebug(vMainWindow, "Pattern file %s was locked.", qUtf8Printable(fileName));
    }
    else
    {
        qCDebug(vMainWindow, "Failed to lock %s", qUtf8Printable(fileName));
        qCDebug(vMainWindow, "Error type: %d", lock->GetLockError());
        qCCritical(vMainWindow, "%s",
                   qUtf8Printable(tr("Failed to lock. This file already opened in another window. Expect "
                                     "collissions when run 2 copies of the program.")));
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
            QString autofile = curFile + autosavePrefix;
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
    qCDebug(vMainWindow, "Run QFileDialog::getOpenFileName: dir = %s.", qUtf8Printable(dir));
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), dir, filter);
    if (filePath.isEmpty())
    {
        return;
    }
    LoadPattern(filePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear reset to default window.
 */
void MainWindow::Clear()
{
    qCDebug(vMainWindow, "Reseting main window.");
    lock.reset();
    qCDebug(vMainWindow, "Unlocked pattern file.");
    ActionDraw(true);
    qCDebug(vMainWindow, "Returned to Draw mode.");
    pattern->Clear();
    qCDebug(vMainWindow, "Clearing pattern.");
    if (not curFile.isEmpty() && not doc->MPath().isEmpty())
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
    ui->actionDraw->setEnabled(false);
    ui->actionDetails->setEnabled(false);
    ui->actionLayout->setEnabled(false);
    ui->actionNewDraw->setEnabled(false);
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
    ui->actionUnloadMeasurements->setEnabled(false);
    ui->actionShowM->setEnabled(false);
    SetEnableTool(false);
    qApp->setPatternUnit(Unit::Cm);
    qApp->setPatternType(MeasurementsType::Unknown);
    ui->toolBarOption->clear();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    CleanLayout();
    listDetails.clear(); // don't move to CleanLayout()

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup--; // turn it off again
#endif /*Q_OS_WIN32*/
    qApp->getUndoStack()->clear();
    toolOptions->ClearPropertyBrowser();
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
    QFile autofile(curFile + autosavePrefix);
    if (autofile.exists())
    {
        autofile.remove();
    }
    qCDebug(vMainWindow, "File %s closed correct.", qUtf8Printable(curFile));
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
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file.")), //-V807
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const VExceptionConversionError &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error can't convert value.")),
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error empty parameter.")),
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const VExceptionWrongId &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error wrong id.")),
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (VException &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error parsing file.")),
                               qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const std::bad_alloc &)
    {
        qCCritical(vMainWindow, "%s", qUtf8Printable(tr("Error parsing file (std::bad_alloc).")));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }

    QString patternPiece;
    if (comboBoxDraws->currentIndex() != -1)
    {
        patternPiece = comboBoxDraws->itemText(comboBoxDraws->currentIndex());
    }
    comboBoxDraws->blockSignals(true);
    comboBoxDraws->clear();
    comboBoxDraws->addItems(doc->getPatternPieces());
    if (not drawMode)
    {
        comboBoxDraws->setCurrentIndex(comboBoxDraws->count()-1);
    }
    else
    {
        const qint32 index = comboBoxDraws->findText(patternPiece);
        if ( index != -1 )
        {
            comboBoxDraws->setCurrentIndex(index);
        }
    }
    comboBoxDraws->blockSignals(false);
    ui->actionPattern_properties->setEnabled(true);

    GlobalChangePP(patternPiece);

    SetEnableTool(comboBoxDraws->count() > 0);
    patternReadOnly = doc->IsReadOnly();
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
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Bad id.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("Error empty parameter.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        SetEnabledGUI(false);
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
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
    const bool drawStage = (mode == Draw::Calculation);
    const bool designStage = (drawStage || mode == Draw::Modeling);

    comboBoxDraws->setEnabled(enable && drawStage);
    ui->actionOptionDraw->setEnabled(enable && drawStage);
    ui->actionSave->setEnabled(enable && not patternReadOnly);
    ui->actionSaveAs->setEnabled(enable);
    ui->actionPattern_properties->setEnabled(enable && designStage);
    ui->actionEdit_pattern_code->setEnabled(enable && designStage);
    ui->actionZoomIn->setEnabled(enable);
    ui->actionZoomOut->setEnabled(enable);
    ui->actionArrowTool->setEnabled(enable && designStage);
    ui->actionHistory->setEnabled(enable && drawStage);
    ui->actionNewDraw->setEnabled(enable && drawStage);
    ui->actionDraw->setEnabled(enable);
    ui->actionDetails->setEnabled(enable);
    ui->actionLayout->setEnabled(enable);
    ui->actionTable->setEnabled(enable && drawStage);
    ui->actionZoomFitBest->setEnabled(enable);
    ui->actionZoomOriginal->setEnabled(enable);
    ui->actionShowCurveDetails->setEnabled(enable && drawStage);
    ui->actionLoadIndividual->setEnabled(enable && designStage);
    ui->actionLoadStandard->setEnabled(enable && designStage);
    ui->actionUnloadMeasurements->setEnabled(enable && designStage);

    actionDockWidgetToolOptions->setEnabled(enable && designStage);
    actionDockWidgetGroups->setEnabled(enable && designStage);

    undoAction->setEnabled(enable && designStage && qApp->getUndoStack()->canUndo());
    redoAction->setEnabled(enable && designStage && qApp->getUndoStack()->canRedo());

    //Now we don't want allow user call context menu
    sceneDraw->SetDisableTools(!enable, doc->GetNameActivPP());
    ui->view->setEnabled(enable);
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
        QString patternPieceName = tr("Pattern piece %1").arg(comboBoxDraws->count()+1);
        qCDebug(vMainWindow, "Generated PP name: %s", qUtf8Printable(patternPieceName));

        qCDebug(vMainWindow, "First PP");
        DialogNewPattern newPattern(pattern, patternPieceName, this);
        if (newPattern.exec() == QDialog::Accepted)
        {
            patternPieceName = newPattern.name();
            qApp->setPatternUnit(newPattern.PatternUnit());
            qCDebug(vMainWindow, "PP name: %s", qUtf8Printable(patternPieceName));
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

        mouseCoordinate = new QLabel(QString("0, 0 (%1)").arg(doc->UnitsToStr(qApp->patternUnit(), true)));
        ui->toolBarOption->addWidget(mouseCoordinate);
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
void MainWindow::PatternChangesWereSaved(bool saved)
{
    if (guiEnabled)
    {
        const bool state = doc->IsModified() || !saved;
        setWindowModified(state);
        not patternReadOnly ? ui->actionSave->setEnabled(state): ui->actionSave->setEnabled(false);
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
    const int size = static_cast<int>(pattern->size());
    if (UpdateMeasurements(AbsoluteMPath(curFile, doc->MPath()), text.toInt(), static_cast<int>(pattern->height())))
    {
        doc->LiteParseTree(Document::LiteParse);
    }
    else
    {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Couldn't update measurements.")));

        const qint32 index = gradationSizes->findText(QString().setNum(size));
        if (index != -1)
        {
            gradationSizes->setCurrentIndex(index);
        }
        else
        {
            qCDebug(vMainWindow, "Couldn't restore size value.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedGrowth change new height value.
 * @param text value height.
 */
void MainWindow::ChangedHeight(const QString &text)
{
    const int height = static_cast<int>(pattern->height());
    if (UpdateMeasurements(AbsoluteMPath(curFile, doc->MPath()), static_cast<int>(pattern->size()), text.toInt()))
    {
        doc->LiteParseTree(Document::LiteParse);
    }
    else
    {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Couldn't update measurements.")));

        const qint32 index = gradationHeights->findText(QString().setNum(height));
        if (index != -1)
        {
            gradationHeights->setCurrentIndex(index);
        }
        else
        {
            qCDebug(vMainWindow, "Couldn't restore height value.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetDefaultHeight()
{
    const QString defHeight = QString().setNum(doc->GetDefCustomHeight());
    int index = gradationHeights->findText(defHeight);
    if (index != -1)
    {
        gradationHeights->setCurrentIndex(index);
    }
    else
    {
        index = gradationHeights->findText(QString().setNum(pattern->height()));
        if (index != -1)
        {
            gradationHeights->setCurrentIndex(index);
        }
    }
    pattern->SetHeight(gradationHeights->currentText().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetDefaultSize()
{
    const QString defSize = QString().setNum(doc->GetDefCustomSize());
    int index = gradationSizes->findText(defSize);
    if (index != -1)
    {
        gradationSizes->setCurrentIndex(index);
    }
    else
    {
        index = gradationSizes->findText(QString().setNum(pattern->size()));
        if (index != -1)
        {
            gradationSizes->setCurrentIndex(index);
        }
    }
    pattern->SetSize(gradationSizes->currentText().toInt());
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
    ui->toolButtonCubicBezier->setEnabled(drawTools);
    ui->toolButtonArc->setEnabled(drawTools);
    ui->toolButtonSplinePath->setEnabled(drawTools);
    ui->toolButtonCubicBezierPath->setEnabled(drawTools);
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
    ui->toolButtonIntersectionCurves->setEnabled(drawTools);
    ui->toolButtonPointOfIntersectionCircles->setEnabled(drawTools);
    ui->toolButtonPointFromCircleAndTangent->setEnabled(drawTools);
    ui->toolButtonPointFromArcAndTangent->setEnabled(drawTools);
    ui->toolButtonArcWithLength->setEnabled(drawTools);
    ui->toolButtonTrueDarts->setEnabled(drawTools);
    ui->toolButtonGroup->setEnabled(drawTools);
    ui->toolButtonRotation->setEnabled(drawTools);
    ui->toolButtonMidpoint->setEnabled(drawTools);

    ui->actionLast_tool->setEnabled(drawTools);

    for (auto pointer : toolButtonPointerList)
    {
        pointer->setEnabled(drawTools || modelingTools);
        pointer->setChecked(drawTools || modelingTools);
    }

    //Modeling Tools
    ui->toolButtonUnionDetails->setEnabled(modelingTools);

    //Layout tools
    ui->toolButtonLayoutSettings->setEnabled(layoutTools);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetLayoutModeActions()
{
    const bool enabled = not scenes.isEmpty();

    ui->toolButtonLayoutExportAs->setEnabled(enabled);
    ui->actionExportAs->setEnabled(enabled);
    ui->actionPrintPreview->setEnabled(enabled);
    ui->actionPrintPreviewTiled->setEnabled(enabled);
    ui->actionSaveAsTiledPDF->setEnabled(enabled);
    ui->actionPrint->setEnabled(enabled);
    ui->actionPrintTiled->setEnabled(enabled);
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
    qCDebug(vMainWindow, "Saving pattern file %s.", qUtf8Printable(fileName));
    QFileInfo tempInfo(fileName);

    const QString mPath = AbsoluteMPath(curFile, doc->MPath());
    if (not mPath.isEmpty() && curFile != fileName)
    {
        doc->SetPath(RelativeMPath(fileName, mPath));
    }

    const bool result = doc->SaveDocument(fileName, error);
    if (result)
    {
        if (tempInfo.suffix() != QLatin1String("autosave"))
        {
            setCurrentFile(fileName);
            helpLabel->setText(tr("File saved"));
            qCDebug(vMainWindow, "File %s saved.", qUtf8Printable(fileName));
            PatternChangesWereSaved(result);
        }
    }
    else
    {
        doc->SetPath(mPath);
        qCDebug(vMainWindow, "Could not save file %s. %s.", qUtf8Printable(fileName), qUtf8Printable(error));
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
        QString autofile = curFile + autosavePrefix;
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
    qCDebug(vMainWindow, "Set current name to \"%s\"", qUtf8Printable(fileName));
    curFile = fileName;
    qApp->getUndoStack()->setClean();

    if (not curFile.isEmpty() && VApplication::IsGUIMode())
    {
        qCDebug(vMainWindow, "Updating recent file list.");
        VSettings *settings = qApp->ValentinaSettings();
        QStringList files = settings->GetRecentFileList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while (files.size() > MaxRecentFiles)
        {
            files.removeLast();
        }

        settings->SetRecentFileList(files);
        UpdateRecentFileActions();

        qCDebug(vMainWindow, "Updating restore file list.");
        QStringList restoreFiles = settings->GetRestoreFileList();
        restoreFiles.removeAll(fileName);
        restoreFiles.prepend(fileName);
        settings->SetRestoreFileList(restoreFiles);
    }

    UpdateWindowTitle();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ReadSettings read setting for app.
 */
void MainWindow::ReadSettings()
{
    qCDebug(vMainWindow, "Reading settings.");
    const VSettings *settings = qApp->ValentinaSettings();
    restoreGeometry(settings->GetGeometry());
    restoreState(settings->GetWindowState());
    restoreState(settings->GetToolbarsState(), APP_VERSION);

    // Scene antialiasing
    const bool graphOutputValue = settings->GetGraphicalOutput();
    ui->view->setRenderHint(QPainter::Antialiasing, graphOutputValue);
    ui->view->setRenderHint(QPainter::SmoothPixmapTransform, graphOutputValue);

    // Stack limit
    qApp->getUndoStack()->setUndoLimit(settings->GetUndoCount());

    // Text under tool buton icon
    ToolBarStyles();

    isDockToolOptionsVisible = ui->dockWidgetToolOptions->isVisible();
    isDockGroupsVisible = ui->dockWidgetGroups->isVisible();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief WriteSettings save setting for app.
 */
void MainWindow::WriteSettings()
{
    ActionDraw(true);

    VSettings *setings = qApp->ValentinaSettings();
    setings->SetGeometry(saveGeometry());
    setings->SetWindowState(saveState());
    setings->SetToolbarsState(saveState(APP_VERSION));
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
        QMessageBox *messageBox = new QMessageBox(tr("Unsaved changes"),
                                                  tr("The pattern has been modified.\n"
                                                     "Do you want to save your changes?"),
                                                  QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No,
                                                  QMessageBox::Cancel, this, Qt::Sheet);

        messageBox->setDefaultButton(QMessageBox::Yes);
        messageBox->setEscapeButton(QMessageBox::Cancel);

        messageBox->setButtonText(QMessageBox::Yes,
                                  curFile.isEmpty() || doc->IsReadOnly() ? tr("Save...") : tr("Save"));
        messageBox->setButtonText(QMessageBox::No, tr("Don't Save"));

        messageBox->setWindowModality(Qt::ApplicationModal);
        const auto ret = static_cast<QMessageBox::StandardButton>(messageBox->exec());

        switch (ret)
        {
            case QMessageBox::Yes:
                if (doc->IsReadOnly())
                {
                    return SaveAs();
                }
                else
                {
                    return Save();
                }
            case QMessageBox::No:
                return true;
            case QMessageBox::Cancel:
                return false;
            default:
                break;
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
       QString text = QString("&%1. %2").arg(i + 1).arg(StrippedName(files.at(i)));
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
    connect(undoAction, &QAction::triggered, toolOptions, &VToolOptionsPropertyBrowser::RefreshOptions);
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setIcon(QIcon::fromTheme("edit-undo"));
    ui->menuPatternPiece->insertAction(ui->actionLast_tool, undoAction);
    ui->toolBarTools->addAction(undoAction);

    redoAction = qApp->getUndoStack()->createRedoAction(this, tr("&Redo"));
    connect(redoAction, &QAction::triggered, toolOptions, &VToolOptionsPropertyBrowser::RefreshOptions);
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setIcon(QIcon::fromTheme("edit-redo"));
    ui->menuPatternPiece->insertAction(ui->actionLast_tool, redoAction);
    ui->toolBarTools->addAction(redoAction);

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, separatorAct);

    AddDocks();
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
void MainWindow::LastUsedTool()
{
    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 45, "Not all tools was handled.");

    if (currentTool == lastUsedTool)
    {
        return;
    }

    switch ( lastUsedTool )
    {
        case Tool::Arrow:
            for (auto pointer : toolButtonPointerList)
            {
                pointer->setChecked(true);
            }
            ArrowTool();
            break;
        case Tool::BasePoint:
        case Tool::SinglePoint:
        case Tool::DoublePoint:
        case Tool::LinePoint:
        case Tool::AbstractSpline:
        case Tool::Cut:
        case Tool::LAST_ONE_DO_NOT_USE:
        case Tool::NodePoint:
        case Tool::NodeArc:
        case Tool::NodeSpline:
        case Tool::NodeSplinePath:
            Q_UNREACHABLE(); //-V501
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
        case Tool::Midpoint:
            ui->toolButtonMidpoint->setChecked(true);
            ToolMidpoint(true);
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
        case Tool::CubicBezier:
            ui->toolButtonCubicBezier->setChecked(true);
            ToolCubicBezier(true);
            break;
        case Tool::Arc:
            ui->toolButtonArc->setChecked(true);
            ToolArc(true);
            break;
        case Tool::SplinePath:
            ui->toolButtonSplinePath->setChecked(true);
            ToolSplinePath(true);
            break;
        case Tool::CubicBezierPath:
            ui->toolButtonCubicBezierPath->setChecked(true);
            ToolCubicBezierPath(true);
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
        case Tool::ArcIntersectAxis:
            ui->toolButtonArcIntersectAxis->setChecked(true);
            ToolArcIntersectAxis(true);
            break;
        case Tool::PointOfIntersectionCircles:
            ui->toolButtonPointOfIntersectionCircles->setChecked(true);
            ToolPointOfIntersectionCircles(true);
            break;
        case Tool::PointOfIntersectionCurves:
            ui->toolButtonIntersectionCurves->setChecked(true);
            ToolPointOfIntersectionCurves(true);
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
        case Tool::Group:
            ui->toolButtonGroup->setChecked(true);
            ToolGroup(true);
            break;
        case Tool::Rotation:
            ui->toolButtonRotation->setChecked(true);
            ToolRotation(true);
            break;
    }
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::AddDocks()
{
    //Add dock
    actionDockWidgetToolOptions = ui->dockWidgetToolOptions->toggleViewAction();
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, actionDockWidgetToolOptions);
    connect(ui->dockWidgetToolOptions, &QDockWidget::visibilityChanged, [this](bool visible){
        isDockToolOptionsVisible = visible;
    });

    actionDockWidgetGroups = ui->dockWidgetGroups->toggleViewAction();
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, actionDockWidgetGroups);
    connect(ui->dockWidgetGroups, &QDockWidget::visibilityChanged, [this](bool visible){
        isDockGroupsVisible = visible;
    });

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    ui->menuPatternPiece->insertAction(ui->actionPattern_properties, separatorAct);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitDocksContain()
{
    qCDebug(vMainWindow, "Initialization property browser.");
    toolOptions = new VToolOptionsPropertyBrowser(ui->dockWidgetToolOptions);

    connect(ui->view, &VMainGraphicsView::itemClicked, toolOptions, &VToolOptionsPropertyBrowser::itemClicked);
    connect(doc, &VPattern::FullUpdateFromFile, toolOptions, &VToolOptionsPropertyBrowser::UpdateOptions);

    qCDebug(vMainWindow, "Initialization groups dock.");
    groupsWidget = new VWidgetGroups(doc, this);
    ui->dockWidgetGroups->setWidget(groupsWidget);

    detailsWidget = new VWidgetDetails(pattern, doc, this);
    connect(doc, &VPattern::FullUpdateFromFile, detailsWidget, &VWidgetDetails::UpdateList);
    detailsWidget->setVisible(false);
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

    connect(ui->actionArrowTool, &QAction::triggered, this, &MainWindow::ArrowTool);
    connect(ui->actionDraw, &QAction::triggered, this, &MainWindow::ActionDraw);
    connect(ui->actionDetails, &QAction::triggered, this, &MainWindow::ActionDetails);
    connect(ui->actionLayout, &QAction::triggered, this, &MainWindow::ActionLayout);

    connect(ui->actionHistory, &QAction::triggered, [this](bool checked)
    {
        if (checked)
        {
            dialogHistory = new DialogHistory(pattern, doc, this);
            dialogHistory->setWindowFlags(Qt::Window);
            connect(this, &MainWindow::RefreshHistory, dialogHistory.data(), &DialogHistory::UpdateHistory);
            connect(dialogHistory.data(), &DialogHistory::DialogClosed, [this]()
            {
                ui->actionHistory->setChecked(false);
                delete dialogHistory;
            });
            // Fix issue #526. Dialog Detail is not on top after selection second object on Mac.
            dialogHistory->setWindowFlags(dialogHistory->windowFlags() | Qt::WindowStaysOnTopHint);
            dialogHistory->show();
        }
        else
        {
            ui->actionHistory->setChecked(true);
            dialogHistory->activateWindow();
        }
    });

    connect(ui->actionNewDraw, &QAction::triggered, [this]()
    {
        qCDebug(vMainWindow, "New PP.");
        QString patternPieceName = tr("Pattern piece %1").arg(comboBoxDraws->count()+1);
        qCDebug(vMainWindow, "Generated PP name: %s", qUtf8Printable(patternPieceName));

        qCDebug(vMainWindow, "PP count %d", comboBoxDraws->count());
        patternPieceName = PatternPieceName(patternPieceName);
        qCDebug(vMainWindow, "PP name: %s", qUtf8Printable(patternPieceName));
        if (patternPieceName.isEmpty())
        {
            qCDebug(vMainWindow, "Name empty.");
            return;
        }

        AddPP(patternPieceName);
    });

    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::SaveAs);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::Save);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::Open);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::New);

    connect(ui->actionTable, &QAction::triggered, [this](bool checked)
    {
        if (checked)
        {
            dialogTable = new DialogIncrements(pattern, doc, this);
            connect(dialogTable.data(), &DialogIncrements::DialogClosed, [this]()
            {
                ui->actionTable->setChecked(false);
                delete dialogTable;
            });
            dialogTable->show();
        }
        else
        {
            ui->actionTable->setChecked(true);
            dialogTable->activateWindow();
        }
    });

    connect(ui->actionAbout_Qt, &QAction::triggered, [this]()
    {
        QMessageBox::aboutQt(this, tr("About Qt"));
    });

    connect(ui->actionAbout_Valentina, &QAction::triggered, [this]()
    {
        DialogAboutApp *aboutDialog = new DialogAboutApp(this);
        aboutDialog->setAttribute(Qt::WA_DeleteOnClose, true);
        aboutDialog->show();
    });

    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::Preferences);
    connect(ui->actionReportBug, &QAction::triggered, [this]()
    {
        qCDebug(vMainWindow, "Reporting bug");
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://bitbucket.org/dismine/valentina/issues/new")));
    });

    connect(ui->actionOnlineHelp, &QAction::triggered, [this]()
    {
        qCDebug(vMainWindow, "Showing online help");
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://bitbucket.org/dismine/valentina/wiki/manual/Content")));
    });

    connect(ui->actionLast_tool, &QAction::triggered, this, &MainWindow::LastUsedTool);

    connect(ui->actionPattern_properties, &QAction::triggered, [this]()
    {
        DialogPatternProperties proper(curFile, doc, pattern, this);
        connect(&proper, &DialogPatternProperties::UpdateGradation, [this]()
        {
            UpdateHeightsList(VMeasurement::ListHeights(doc->GetGradationHeights(), qApp->patternUnit()));
            UpdateSizesList(VMeasurement::ListSizes(doc->GetGradationSizes(), qApp->patternUnit()));
        });
        proper.exec();
    });

    ui->actionPattern_properties->setEnabled(false);
    connect(ui->actionEdit_pattern_code, &QAction::triggered, [this]()
    {
        DialogPatternXmlEdit *pattern = new DialogPatternXmlEdit (this, doc);
        pattern->setAttribute(Qt::WA_DeleteOnClose, true);
        pattern->show();
    });

    connect(ui->actionClosePattern, &QAction::triggered, [this]()
    {
        if (MaybeSave())
        {
            FileClosedCorrect();
            Clear();
        }
    });

    connect(ui->actionShowCurveDetails, &QAction::triggered, [this](bool checked)
    {
        ui->view->itemClicked(nullptr);
        sceneDraw->EnableDetailsMode(checked);
    });

    connect(ui->actionLoadIndividual, &QAction::triggered, this, &MainWindow::LoadIndividual);
    connect(ui->actionLoadStandard, &QAction::triggered, this, &MainWindow::LoadStandard);

    connect(ui->actionCreateNew, &QAction::triggered, [this]()
    {
        const QString tape = qApp->TapeFilePath();
        const QString workingDirectory = QFileInfo(tape).absoluteDir().absolutePath();
        QProcess::startDetached(tape, QStringList(), workingDirectory);
    });

    connect(ui->actionShowM, &QAction::triggered, this, &MainWindow::ShowMeasurements);
    connect(ui->actionExportAs, &QAction::triggered, this, &MainWindow::ExportLayoutAs);
    connect(ui->actionPrintPreview, &QAction::triggered, this, &MainWindow::PrintPreviewOrigin);
    connect(ui->actionPrintPreviewTiled, &QAction::triggered, this, &MainWindow::PrintPreviewTiled);
    connect(ui->actionSaveAsTiledPDF, &QAction::triggered, this, &MainWindow::SaveAsTiledPDF);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::PrintOrigin);
    connect(ui->actionPrintTiled, &QAction::triggered, this, &MainWindow::PrintTiled);
    ui->actionEdit_pattern_code->setEnabled(false);

    //Actions for recent files loaded by a main window application.
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], &QAction::triggered, this, [this]()
        {
            if (QAction *action = qobject_cast<QAction *>(sender()))
            {
                const QString filePath = action->data().toString();
                if (not filePath.isEmpty())
                {
                    LoadPattern(filePath);
                }
            }
        });
    }

    connect(ui->actionSyncMeasurements, &QAction::triggered, this, &MainWindow::SyncMeasurements);
    connect(ui->actionUnloadMeasurements, &QAction::triggered, this, &MainWindow::UnloadMeasurements);
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
    qCDebug(vMainWindow, "Loading new file %s.", qUtf8Printable(fileName));

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

    try
    {
        // Here comes undocumented Valentina's feature.
        // Because app bundle in Mac OS X doesn't allow setup assosiation for Tape we must do this through Valentina
        VMeasurements m(pattern);
        m.setXMLContent(fileName);

        if (m.Type() == MeasurementsType::Standard || m.Type() == MeasurementsType::Individual)
        {
            const QString tape = qApp->TapeFilePath();
            const QString workingDirectory = QFileInfo(tape).absoluteDir().absolutePath();
            QProcess::startDetached(tape, QStringList(fileName), workingDirectory);
            qApp->exit(V_EX_OK);
            return false; // stop continue processing
        }
    }
    catch (VException &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        Clear();
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

    qCDebug(vMainWindow, "Loking file");
    VlpCreateLock(lock, fileName);

    if (lock->IsLocked())
    {
        qCDebug(vMainWindow, "Pattern file %s was locked.", qUtf8Printable(fileName));
    }
    else
    {
        if (not IgnoreLocking(lock->GetLockError(), fileName))
        {
            return false;
        }
    }

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
            doc->SetPath(RelativeMPath(fileName, customMeasureFile));
        }
        qApp->setPatternUnit(doc->MUnit());
        const QString path = AbsoluteMPath(fileName, doc->MPath());

        if (not path.isEmpty())
        {
            // Check if exist
            const QString newPath = CheckPathToMeasurements(fileName, path);
            if (newPath.isEmpty())
            {
                qApp->setOpeningPattern();// End opening file
                Clear();
                qCCritical(vMainWindow, "%s", qUtf8Printable(tr("The measurements file '%1' could not be found.")
                                                             .arg(path)));
                if (not VApplication::IsGUIMode())
                {
                    qApp->exit(V_EX_NOINPUT);
                }
                return false;
            }

            if (not LoadMeasurements(newPath))
            {
                qCCritical(vMainWindow, "%s", qUtf8Printable(tr("The measurements file '%1' could not be found.")
                                                             .arg(newPath)));
                qApp->setOpeningPattern();// End opening file
                Clear();
                if (not VApplication::IsGUIMode())
                {
                    qApp->exit(V_EX_NOINPUT);
                }
                return false;
            }
            else
            {
                ui->actionUnloadMeasurements->setEnabled(true);
                watcher->addPath(path);
                ui->actionShowM->setEnabled(true);
            }
        }

        if (qApp->patternType() == MeasurementsType::Unknown)
        {// Show toolbar only if was not uploaded any measurements.
            ToolBarOption();
        }
    }
    catch (VException &e)
    {
        qCCritical(vMainWindow, "%s\n\n%s\n\n%s", qUtf8Printable(tr("File error.")),
                   qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        qApp->setOpeningPattern();// End opening file
        Clear();
        if (not VApplication::IsGUIMode())
        {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

#ifdef Q_OS_WIN32
    qt_ntfs_permission_lookup--; // turn it off again
#endif /*Q_OS_WIN32*/

    FullParseFile();

    if (guiEnabled)
    { // No errors occurred
        setCurrentFile(fileName);
        helpLabel->setText(tr("File loaded"));
        qCDebug(vMainWindow, "File loaded.");

        //Fit scene size to best size for first show
        ZoomFirstShow();

        ActionDraw(true);
    }
    qApp->setOpeningPattern();// End opening file
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
            VLockGuard<char> tmp(files.at(i));
            if (tmp.IsLocked())
            {
                restoreFiles.append(files.at(i));
            }
        }

        // Clearing list after filtering
        for (int i = 0; i < restoreFiles.size(); ++i)
        {
            files.removeAll(restoreFiles.at(i));
        }

        // Clear all files that do not exist.
        QStringList filtered;
        for (int i = 0; i < files.size(); ++i)
        {
            if (QFileInfo(files.at(i)).exists())
            {
                filtered.append(files.at(i));
            }
        }

        qApp->ValentinaSettings()->SetRestoreFileList(filtered);
    }
    return restoreFiles;
#else
    return qApp->ValentinaSettings()->GetRestoreFileList();
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
    }
    else
    {
        ui->view->setScene(scenes.at(index));
    }

    ui->view->fitInView(ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
}

//---------------------------------------------------------------------------------------------------------------------
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
void MainWindow::CreateMeasurements()
{
    const QString tape = qApp->TapeFilePath();
    const QString workingDirectory = QFileInfo(tape).absoluteDir().absolutePath();
    QProcess::startDetached(tape, QStringList(), workingDirectory);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ExportLayoutAs()
{
    if (isLayoutStale)
    {
        if (ContinueIfLayoutStale() == QMessageBox::No)
        {
            ui->toolButtonLayoutExportAs->setChecked(false);
            return;
        }
    }

    try
    {
        DialogSaveLayout dialog(scenes.size(), FileName(), this);

        if (dialog.exec() == QDialog::Rejected)
        {
            ui->toolButtonLayoutExportAs->setChecked(false);
            return;
        }

        ExportLayout(dialog);
    }
    catch (const VException &e)
    {
        ui->toolButtonLayoutExportAs->setChecked(false);
        qCritical("%s\n\n%s\n\n%s", qUtf8Printable(tr("Export error.")),
                  qUtf8Printable(e.ErrorMessage()), qUtf8Printable(e.DetailedInformation()));
        return;
    }
    ui->toolButtonLayoutExportAs->setChecked(false);
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
            QFile file(files.at(i) + autosavePrefix);
            if (file.exists())
            {
                restoreFiles.append(files.at(i));
            }
        }

        if (restoreFiles.size() > 0)
        {
            QMessageBox::StandardButton reply;
            const QString mes = tr("Valentina didn't shut down correctly. Do you want reopen files (%1) you had open?")
                    .arg(restoreFiles.size());
            reply = QMessageBox::question(this, tr("Reopen files."), mes, QMessageBox::Yes|QMessageBox::No,
                                          QMessageBox::Yes);
            if (reply == QMessageBox::Yes)
            {
                qCDebug(vMainWindow, "User said Yes.");

                for (int i = 0; i < restoreFiles.size(); ++i)
                {
                    QString error;
                    if (VDomDocument::SafeCopy(restoreFiles.at(i) + autosavePrefix, restoreFiles.at(i), error))
                    {
                        QFile autoFile(restoreFiles.at(i) + autosavePrefix);
                        autoFile.remove();
                        LoadPattern(restoreFiles.at(i));
                        args.removeAll(restoreFiles.at(i));// Do not open file twice after we restore him.
                    }
                    else
                    {
                        qCDebug(vMainWindow, "Could not copy %s%s to %s %s",
                                qUtf8Printable(restoreFiles.at(i)), qUtf8Printable(autosavePrefix),
                                qUtf8Printable(restoreFiles.at(i)), qUtf8Printable(error));
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
        if (!qApp->IsGUIMode())
        {
            return QString();// console mode doesn't support fixing path to a measurement file
        }
        else
        {
            const QString text = tr("The measurements file <br/><br/> <b>%1</b> <br/><br/> could not be found. Do you "
                                    "want to update the file location?").arg(path);
            QMessageBox::StandardButton res = QMessageBox::question(this, tr("Loading measurements file"), text,
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
                        VException e(tr("Measurement file has unknown format."));
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

                    if (not m->IsDefinedKnownNamesValid())
                    {
                        VException e(tr("Measurement file contains invalid known measurement(s)."));
                        throw e;
                    }

                    const QStringList mList = m->ListAll();
                    const QStringList pList = doc->ListMeasurements();

                    delete m;

                    const QSet<QString> match = pList.toSet().subtract(mList.toSet());
                    if (not match.isEmpty())
                    {
                        VException e(tr("Measurement file doesn't include all required measurements."));
                        e.AddMoreInformation(tr("Please, additionaly provide: %1")
                                             .arg(QStringList(match.toList()).join(", ")));
                        throw e;
                    }

                    doc->SetPath(RelativeMPath(patternPath, mPath));
                    PatternChangesWereSaved(false);
                    qApp->setPatternType(patternType);
                    return mPath;
                }
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
        doc->setCurrentData();
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
        groupsWidget->UpdateGroups();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::EndVisualization(bool click)
{
    if (not dialogTool.isNull())
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

    if (pattern->DataDetails()->size() > 0)
    {
        ActionDetails(true);
        ui->view->ZoomFitBest();
    }

    if (not ui->actionDraw->isChecked())
    {
        ActionDraw(true);
    }
    ui->view->ZoomFitBest();

    VMainGraphicsView::NewSceneRect(sceneDraw, ui->view);
    VMainGraphicsView::NewSceneRect(sceneDetails, ui->view);

    if (pattern->DataDetails()->size() > 0)
    {
        ActionDetails(true);
        ui->view->ZoomFitBest();
    }

    if (not ui->actionDraw->isChecked())
    {
        ActionDraw(true);
    }
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
            qCCritical(vMainWindow, "%s", qUtf8Printable(tr("You can't export empty scene.")));
            qApp->exit(V_EX_DATAERR);
            return;
        }
    }
    PrepareDetailsForLayout(details);
    if (LayoutSettings(*settings.get()))
    {
        try
        {
            DialogSaveLayout dialog(scenes.size(), expParams->OptBaseName(), this);
            dialog.SetDestinationPath(expParams->OptDestinationPath());
            dialog.SelectFormate(expParams->OptExportType());
            ExportLayout(dialog);
        }
        catch (const VException &e)
        {
            qCCritical(vMainWindow, "%s\n\n%s", qUtf8Printable(tr("Export error.")), qUtf8Printable(e.ErrorMessage()));
            qApp->exit(V_EX_DATAERR);
            return;
        }
    }
    else
    {
        return;
    }

    qApp->exit(V_EX_OK);
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::SetSize(const QString &text)
{
    if (not qApp->IsGUIMode())
    {
        if (this->isWindowModified() || not curFile.isEmpty())
        {
            if (qApp->patternType() == MeasurementsType::Standard)
            {
                const int size = static_cast<int>(UnitConvertor(text.toInt(), Unit::Cm, *pattern->GetPatternUnit()));
                const qint32 index = gradationSizes->findText(QString().setNum(size));
                if (index != -1)
                {
                    gradationSizes->setCurrentIndex(index);
                }
                else
                {
                    qCCritical(vMainWindow, "%s",
                              qUtf8Printable(tr("Not supported size value '%1' for this pattern file.").arg(text)));
                    return false;
                }
            }
            else
            {
                qCCritical(vMainWindow, "%s",
                          qUtf8Printable(tr("Couldn't set size. Need a file with standard measurements.")));
                return false;
            }
        }
        else
        {
            qCCritical(vMainWindow, "%s", qUtf8Printable(tr("Couldn't set size. File wasn't opened.")));
            return false;
        }
    }
    else
    {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("The method %1 does nothing in GUI mode").arg(Q_FUNC_INFO)));
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::SetHeight(const QString &text)
{
    if (not qApp->IsGUIMode())
    {
        if (this->isWindowModified() || not curFile.isEmpty())
        {
            if (qApp->patternType() == MeasurementsType::Standard)
            {
                const int height = static_cast<int>(UnitConvertor(text.toInt(), Unit::Cm, *pattern->GetPatternUnit()));
                const qint32 index = gradationHeights->findText(QString().setNum(height));
                if (index != -1)
                {
                    gradationHeights->setCurrentIndex(index);
                }
                else
                {
                    qCCritical(vMainWindow, "%s",
                              qUtf8Printable(tr("Not supported height value '%1' for this pattern file.").arg(text)));
                    return false;
                }
            }
            else
            {
                qCCritical(vMainWindow, "%s",
                          qUtf8Printable(tr("Couldn't set height. Need a file with standard measurements.")));
                return false;
            }
        }
        else
        {
            qCCritical(vMainWindow, "%s", qUtf8Printable(tr("Couldn't set height. File wasn't opened.")));
            return false;
        }
    }
    else
    {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("The method %1 does nothing in GUI mode").arg(Q_FUNC_INFO)));
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ProcessCMD()
{
    const VCommandLinePtr cmd = qApp->CommandLine();
    auto args = cmd->OptInputFileNames();

    if (VApplication::IsGUIMode())
    {
        ReopenFilesAfterCrash(args);
    }
    else
    {
        if (args.size() != 1)
        {
            qCritical() << tr("Please, provide one input file.");
            qApp->exit(V_EX_NOINPUT);
            return;
        }
    }

    for (int i=0, sz = args.size(); i < sz; ++i)
    {
        const bool loaded = LoadPattern(args.at(static_cast<int>(i)), cmd->OptMeasurePath());

        if (not loaded && not VApplication::IsGUIMode())
        {
            return; // process only one input file
        }

        bool hSetted = true;
        bool sSetted = true;
        if (loaded && (cmd->IsTestModeEnabled() || cmd->IsExportEnabled()))
        {
            if (cmd->IsSetGradationSize())
            {
                sSetted = SetSize(cmd->OptGradationSize());
            }

            if (cmd->IsSetGradationHeight())
            {
                hSetted = SetHeight(cmd->OptGradationHeight());
            }
        }

        if (not cmd->IsTestModeEnabled())
        {
            if (cmd->IsExportEnabled())
            {
                if (loaded && hSetted && sSetted)
                {
                    DoExport(cmd);
                    return; // process only one input file
                }
                else
                {
                    qApp->exit(V_EX_DATAERR);
                    return;
                }
                break;
            }
        }
    }

    if (not VApplication::IsGUIMode())
    {
        qApp->exit(V_EX_OK);// close program after processing in console mode
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::GetPatternFileName()
{
    QString shownName = tr("untitled.val");
    if(not curFile.isEmpty())
    {
        shownName = StrippedName(curFile);
    }
    shownName += QLatin1String("[*]");
    return shownName;
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::GetMeasurementFileName()
{
    if(doc->MPath().isEmpty())
    {
        return "";
    }
    else
    {
        QString shownName(" [");
        shownName += StrippedName(AbsoluteMPath(curFile, doc->MPath()));

        if(mChanges)
        {
            shownName += QLatin1String("*");
        }

        shownName += QLatin1String("]");
        return shownName;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateWindowTitle()
{
    if (not patternReadOnly)
    {
        setWindowTitle(GetPatternFileName()+GetMeasurementFileName());
    }
    else
    {
        setWindowTitle(GetPatternFileName()+GetMeasurementFileName() + " " + tr("(read only)"));
    }
    setWindowFilePath(curFile);

#if defined(Q_OS_MAC)
    static QIcon fileIcon = QIcon(QApplication::applicationDirPath() +
                                  QLatin1String("/../Resources/Valentina.icns"));
    QIcon icon;
    if (not curFile.isEmpty())
    {
        if (not isWindowModified())
        {
            icon = fileIcon;
        }
        else
        {
            static QIcon darkIcon;

            if (darkIcon.isNull())
            {
                darkIcon = QIcon(darkenPixmap(fileIcon.pixmap(16, 16)));
            }
            icon = darkIcon;
        }
    }
    setWindowIcon(icon);
#endif //defined(Q_OS_MAC)
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::IgnoreLocking(int error, const QString &path)
{
    QMessageBox::StandardButton answer = QMessageBox::Abort;
    if (VApplication::IsGUIMode())
    {
        switch(error)
        {
            case QLockFile::LockFailedError:
                answer = QMessageBox::warning(this, tr("Locking file"),
                                               tr("This file already opened in another window. Ignore if you want "
                                                  "to continue (not recommended, can cause a data corruption)."),
                                               QMessageBox::Abort|QMessageBox::Ignore, QMessageBox::Abort);
                break;
            case QLockFile::PermissionError:
                answer = QMessageBox::question(this, tr("Locking file"),
                                               tr("The lock file could not be created, for lack of permissions. "
                                                  "Ignore if you want to continue (not recommended, can cause "
                                                  "a data corruption)."),
                                               QMessageBox::Abort|QMessageBox::Ignore, QMessageBox::Abort);
                break;
            case QLockFile::UnknownError:
                answer = QMessageBox::question(this, tr("Locking file"),
                                               tr("Unknown error happened, for instance a full partition prevented "
                                                  "writing out the lock file. Ignore if you want to continue (not "
                                                  "recommended, can cause a data corruption)."),
                                               QMessageBox::Abort|QMessageBox::Ignore, QMessageBox::Abort);
                break;
            default:
                answer = QMessageBox::Abort;
                break;
        }
    }

    if (answer == QMessageBox::Abort)
    {
        qCDebug(vMainWindow, "Failed to lock %s", qUtf8Printable(path));
        qCDebug(vMainWindow, "Error type: %d", error);
        Clear();
        if (not VApplication::IsGUIMode())
        {
            switch(error)
            {
                case QLockFile::LockFailedError:
                    qCCritical(vMainWindow, "%s",
                               qUtf8Printable(tr("This file already opened in another window.")));
                    break;
                case QLockFile::PermissionError:
                    qCCritical(vMainWindow, "%s",
                               qUtf8Printable(tr("The lock file could not be created, for lack of permissions.")));
                    break;
                case QLockFile::UnknownError:
                    qCCritical(vMainWindow, "%s",
                               qUtf8Printable(tr("Unknown error happened, for instance a full partition prevented "
                                                 "writing out the lock file.")));
                    break;
                default:
                    break;
            }

            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPoint() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(false);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPointByRelease() const
{
    ToolSelectPoint();
    emit ItemsSelection(SelectionType::ByMouseRelease);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPointByPress() const
{
    ToolSelectPoint();
    emit ItemsSelection(SelectionType::ByMousePress);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectSpline() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(false);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectSplinePath() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectArc() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPointArc() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectCurve() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectAllDrawObjects() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectOperationObjects() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(true);
    emit EnablePointSelection(true);
    emit EnableLineSelection(false);
    emit EnableArcSelection(true);
    emit EnableSplineSelection(true);
    emit EnableSplinePathSelection(true);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectGroupObjects() const
{
    ToolSelectOperationObjects();
    // Only true for rubber band selection
    emit EnableLineSelection(true);

    // Hovering
    emit EnableLineHover(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectDetail() const
{
    // Only true for rubber band selection
    emit EnableNodeLabelSelection(false);
    emit EnableNodePointSelection(false);
    emit EnableDetailSelection(true);// Disable when done visualization details

    // Hovering
    emit EnableNodeLabelHover(true);
    emit EnableNodePointHover(true);
    emit EnableDetailHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    ui->view->AllowRubberBand(false);
}
