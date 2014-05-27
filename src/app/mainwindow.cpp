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
 **  Copyright (C) 2013 Valentina project
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
#include "geometry/vspline.h"
#include "geometry/vequidistant.h"
#include "exception/vexceptionobjecterror.h"
#include "exception/vexceptionconversionerror.h"
#include "exception/vexceptionemptyparameter.h"
#include "exception/vexceptionwrongid.h"
#include "version.h"
#include "xml/vstandardmeasurements.h"

#include <QInputDialog>
#include <QtCore>
#include <QDebug>
#include <QMessageBox>
#include <QShowEvent>
#include <QScrollBar>
#include <QFileDialog>
#include <QSourceLocation>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MainWindow constructor.
 * @param parent parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent), ui(new Ui::MainWindow), pattern(nullptr), doc(nullptr), tool(Valentina::ArrowTool),
      currentScene(nullptr), sceneDraw(nullptr), sceneDetails(nullptr), mouseCoordinate(nullptr), helpLabel(nullptr),
      view(nullptr), isInitialized(false), dialogTable(0), dialogTool(nullptr), dialogHistory(nullptr),
      comboBoxDraws(nullptr), curFile(QString()), mode(Valentina::Calculation), currentDrawIndex(0),
      currentToolBoxIndex(0), drawMode(true), recentFileActs{0, 0, 0, 0, 0}, separatorAct(nullptr),
      autoSaveTimer(nullptr)
{
    CreateActions();
    CreateMenus();
    ToolBarDraws();
    InitToolButtons();

    sceneDraw = new VMainGraphicsScene();
    currentScene = sceneDraw;
    connect(sceneDraw, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    sceneDetails = new VMainGraphicsScene();
    connect(sceneDetails, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    view = new VMainGraphicsView();
    ui->LayoutView->addWidget(view);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setScene(currentScene);

    sceneDraw->setTransform(view->transform());
    sceneDetails->setTransform(view->transform());

    connect(view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(12);
    view->setSizePolicy(policy);
    helpLabel = new QLabel(QObject::tr("Create new pattern piece to start working."));
    ui->statusBar->addWidget(helpLabel);

    pattern = new VContainer();

    doc = new VPattern(pattern, comboBoxDraws, &mode);
    connect(doc, &VPattern::patternChanged, this, &MainWindow::PatternWasModified);

    InitAutoSave();

    ui->toolBox->setCurrentIndex(0);

    ReadSettings();

    setCurrentFile("");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionNewDraw add to scene new pattern peace.
 */
void MainWindow::ActionNewDraw()
{
    QString patternPieceName = QString(tr("Pattern piece %1")).arg(comboBoxDraws->count()+1);
    if (comboBoxDraws->count() == 0)
    {
        QString path;
        DialogMeasurements measurements(this);
        measurements.exec();
        if (measurements.type() == Measurements::Standard)
        {
            qApp->setPatternType(Pattern::Standard);
            DialogStandardMeasurements stMeasurements(pattern, patternPieceName, this);
            if (stMeasurements.exec() == QDialog::Accepted)
            {
                patternPieceName = stMeasurements.name();
                path = stMeasurements.tablePath();
                VStandardMeasurements m(pattern);
                m.setContent(path);
                m.SetSize();
                m.SetHeight();
                m.Measurements();
            }
            else
            {
                return;
            }
        }
        else
        {
            qApp->setPatternType(Pattern::Individual);
            DialogIndividualMeasurements indMeasurements(pattern, patternPieceName, this);
            if (indMeasurements.exec() == QDialog::Accepted)
            {
                patternPieceName = indMeasurements.name();
                path = indMeasurements.tablePath();
                VIndividualMeasurements m(pattern);
                m.setContent(path);
                m.Measurements();
            }
            else
            {
                return;
            }
        }
        doc->CreateEmptyFile(path);
    }
    else
    {
        patternPieceName = PatternPieceName(patternPieceName);
        if (patternPieceName.isEmpty())
        {
            return;
        }
    }
    if (doc->appendDraw(patternPieceName) == false)
    {
        qDebug()<<"Error creating pattern with the name "<<patternPieceName<<".";
        return;
    }
    disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               this, &MainWindow::currentDrawChanged);
    comboBoxDraws->addItem(patternPieceName);

    pattern->ClearGObjects();
    //Create single point
    const quint32 id = pattern->AddGObject(new VPointF(qApp->toPixel((10+comboBoxDraws->count()*5)), qApp->toPixel(10),
                                                       "А", 5, 10));
    VToolSinglePoint *spoint = new VToolSinglePoint(doc, pattern, id, Valentina::FromGui);
    sceneDraw->addItem(spoint);
    connect(spoint, &VToolPoint::ChoosedTool, sceneDraw, &VMainGraphicsScene::ChoosedItem);
    connect(sceneDraw, &VMainGraphicsScene::NewFactor, spoint, &VToolSinglePoint::SetFactor);
    QHash<quint32, VDataTool*>* tools = doc->getTools();
    Q_CHECK_PTR(tools);
    tools->insert(id, spoint);
    VDrawTool::AddRecord(id, Valentina::SinglePointTool, doc);
    SetEnableTool(true);
    SetEnableWidgets(true);

    const qint32 index = comboBoxDraws->findText(patternPieceName);
    if ( index != -1 )
    { // -1 for not found
        comboBoxDraws->setCurrentIndex(index);
        currentDrawChanged( index );
    }
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &MainWindow::currentDrawChanged);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief OptionDraw help change name of pattern peace.
 */
void MainWindow::OptionDraw()
{
    const QString activDraw = doc->GetNameActivDraw();
    const QString nameDraw = PatternPieceName(activDraw);
    if (nameDraw.isEmpty())
    {
        return;
    }
    if (doc->SetNameDraw(nameDraw))
    {
        comboBoxDraws->setItemText(comboBoxDraws->findText(activDraw), nameDraw);
    }
    else
    {
        QMessageBox::warning(this, tr("Error saving change!!!"), tr("Can't save new label of pattern piece"));
    }
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
void MainWindow::SetToolButton(bool checked, Valentina::Tools t, const QString &cursor, const QString &toolTip,
                               Func closeDialogSlot)
{
    if (checked)
    {
        CancelTool();
        tool = t;
        QPixmap pixmap(cursor);
        QCursor cur(pixmap, 2, 3);
        view->setCursor(cur);
        helpLabel->setText(toolTip);
        dialogTool = new Dialog(pattern, this);
        connect(currentScene, &VMainGraphicsScene::ChoosedObject, dialogTool, &DialogTool::ChoosedObject);
        connect(dialogTool, &DialogTool::DialogClosed, this, closeDialogSlot);
        connect(dialogTool, &DialogTool::ToolTip, this, &MainWindow::ShowToolTip);
        connect(doc, &VPattern::FullUpdateFromFile, dialogTool, &DialogTool::UpdateList);
    }
    else
    {
        if (QToolButton *tButton = qobject_cast< QToolButton * >(this->sender()))
        {
            Q_CHECK_PTR(tButton);
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
    Q_CHECK_PTR(dialogTool);
    if (result == QDialog::Accepted)
    {
        DrawTool::Create(dialogTool, currentScene, doc, pattern);
    }
    ArrowTool();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolEndLine handler tool endLine.
 * @param checked true - button checked.
 */
void MainWindow::ToolEndLine(bool checked)
{
    SetToolButton<DialogEndLine>(checked, Valentina::EndLineTool, ":/cursor/endline_cursor.png", tr("Select point"),
                                 &MainWindow::ClosedDialogEndLine);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogEndLine actions after closing DialogEndLine.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogEndLine(int result)
{
    ClosedDialog<VToolEndLine>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolLine handler tool line.
 * @param checked true - button checked.
 */
void MainWindow::ToolLine(bool checked)
{
    SetToolButton<DialogLine>(checked, Valentina::LineTool, ":/cursor/line_cursor.png", tr("Select first point"),
                              &MainWindow::ClosedDialogLine);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogLine actions after closing DialogLine.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogLine(int result)
{
    ClosedDialog<VToolLine>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolAlongLine handler tool alongLine.
 * @param checked true - button checked.
 */
void MainWindow::ToolAlongLine(bool checked)
{
    SetToolButton<DialogAlongLine>(checked, Valentina::AlongLineTool, ":/cursor/alongline_cursor.png",
                                   tr("Select point"), &MainWindow::ClosedDialogAlongLine);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogAlongLine actions after closing DialogAlongLine.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogAlongLine(int result)
{
    ClosedDialog<VToolAlongLine>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolShoulderPoint handler tool shoulderPoint.
 * @param checked true - button checked.
 */
void MainWindow::ToolShoulderPoint(bool checked)
{
    SetToolButton<DialogShoulderPoint>(checked, Valentina::ShoulderPointTool, ":/cursor/shoulder_cursor.png",
                  tr("Select first point of line"), &MainWindow::ClosedDialogShoulderPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogShoulderPoint actions after closing DialogShoulderPoint.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogShoulderPoint(int result)
{
    ClosedDialog<VToolShoulderPoint>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolNormal handler tool normal.
 * @param checked true - button checked.
 */
void MainWindow::ToolNormal(bool checked)
{
    SetToolButton<DialogNormal>(checked, Valentina::NormalTool, ":/cursor/normal_cursor.png",
                  tr("Select first point of line"), &MainWindow::ClosedDialogNormal);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogNormal actions after closing DialogNormal.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogNormal(int result)
{
    ClosedDialog<VToolNormal>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBisector handler tool bisector.
 * @param checked true - button checked.
 */
void MainWindow::ToolBisector(bool checked)
{
    SetToolButton<DialogBisector>(checked, Valentina::BisectorTool, ":/cursor/bisector_cursor.png",
                  tr("Select first point of angle"), &MainWindow::ClosedDialogBisector);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogBisector actions after closing DialogBisector.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogBisector(int result)
{
    ClosedDialog<VToolBisector>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolLineIntersect handler tool lineIntersect.
 * @param checked true - button checked.
 */
void MainWindow::ToolLineIntersect(bool checked)
{
    SetToolButton<DialogLineIntersect>(checked, Valentina::LineIntersectTool, ":/cursor/intersect_cursor.png",
                  tr("Select first point of first line"), &MainWindow::ClosedDialogLineIntersect);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogLineIntersect actions after closing DialogLineIntersect.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogLineIntersect(int result)
{
    ClosedDialog<VToolLineIntersect>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolSpline handler tool spline.
 * @param checked true - button checked.
 */
void MainWindow::ToolSpline(bool checked)
{
    SetToolButton<DialogSpline>(checked, Valentina::SplineTool, ":/cursor/spline_cursor.png",
                  tr("Select first point curve"), &MainWindow::ClosedDialogSpline);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogSpline actions after closing DialogSpline.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogSpline(int result)
{
    ClosedDialog<VToolSpline>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutSpline handler tool CutSpline.
 * @param checked true - button is checked
 */
void MainWindow::ToolCutSpline(bool checked)
{
    SetToolButton<DialogCutSpline>(checked, Valentina::CutSplineTool, ":/cursor/spline_cut_point_cursor.png",
                  tr("Select simple curve"), &MainWindow::ClosedDialogCutSpline);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogCutSpline actions after closing DialogCutSpline.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogCutSpline(int result)
{
    ClosedDialog<VToolCutSpline>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolArc handler tool arc.
 * @param checked true - button checked.
 */
void MainWindow::ToolArc(bool checked)
{
    SetToolButton<DialogArc>(checked, Valentina::ArcTool, ":/cursor/arc_cursor.png",
                  tr("Select point of center of arc"), &MainWindow::ClosedDialogArc);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogArc actions after closing DialogArc.
 * @param result result of dialog working..
 */
void MainWindow::ClosedDialogArc(int result)
{
    ClosedDialog<VToolArc>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolSplinePath handler tool splinePath.
 * @param checked true - button checked.
 */
void MainWindow::ToolSplinePath(bool checked)
{
    SetToolButton<DialogSplinePath>(checked, Valentina::SplinePathTool, ":/cursor/splinepath_cursor.png",
                  tr("Select point of curve path"), &MainWindow::ClosedDialogSplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogSplinePath actions after closing DialogSplinePath.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogSplinePath(int result)
{
    ClosedDialog<VToolSplinePath>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutSplinePath handler tool CutSplinePath.
 * @param checked true - button is checked
 */
void MainWindow::ToolCutSplinePath(bool checked)
{
    SetToolButton<DialogCutSplinePath>(checked, Valentina::CutSplinePathTool,
                                       ":/cursor/splinepath_cut_point_cursor.png", tr("Select curve path"),
                                       &MainWindow::ClosedDialogCutSplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogCutSplinePath actions after closing DialogCutSplinePath.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogCutSplinePath(int result)
{
    ClosedDialog<VToolCutSplinePath>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolPointOfContact handler tool pointOfContact.
 * @param checked true - button checked.
 */
void MainWindow::ToolPointOfContact(bool checked)
{
    SetToolButton<DialogPointOfContact>(checked, Valentina::PointOfContact, ":/cursor/pointcontact_cursor.png",
                  tr("Select first point of line"), &MainWindow::ClosedDialogPointOfContact);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogPointOfContact actions after closing DialogPointOfContact.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogPointOfContact(int result)
{
    ClosedDialog<VToolPointOfContact>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolDetail handler tool detail.
 * @param checked true - button checked.
 */
void MainWindow::ToolDetail(bool checked)
{
    SetToolButton<DialogDetail>(checked, Valentina::DetailTool, "://cursor/new_detail_cursor.png",
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
    doc->FullUpdateTree();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolHeight handler tool height.
 * @param checked true - button checked.
 */
void MainWindow::ToolHeight(bool checked)
{
    SetToolButton<DialogHeight>(checked, Valentina::Height, ":/cursor/height_cursor.png", tr("Select base point"),
                                &MainWindow::ClosedDialogHeight);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogHeight actions after closing DialogHeight.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogHeight(int result)
{
    ClosedDialog<VToolHeight>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolTriangle handler tool triangle.
 * @param checked true - button checked.
 */
void MainWindow::ToolTriangle(bool checked)
{
    SetToolButton<DialogTriangle>(checked, Valentina::Triangle, ":/cursor/triangle_cursor.png",
                                  tr("Select first point of axis"), &MainWindow::ClosedDialogTriangle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogTriangle actions after closing DialogTriangle.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogTriangle(int result)
{
    ClosedDialog<VToolTriangle>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolPointOfIntersection handler tool pointOfIntersection.
 * @param checked true - button checked.
 */
void MainWindow::ToolPointOfIntersection(bool checked)
{
    SetToolButton<DialogPointOfIntersection>(checked, Valentina::PointOfIntersection,
                                             ":/cursor/pointofintersect_cursor.png", tr("Select point vertically"),
                                             &MainWindow::ClosedDialogPointOfIntersection);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogPointOfIntersection actions after closing DialogPointOfIntersection.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogPointOfIntersection(int result)
{
    ClosedDialog<VToolPointOfIntersection>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolUnionDetails handler tool unionDetails.
 * @param checked true - button checked.
 */
void MainWindow::ToolUnionDetails(bool checked)
{
    SetToolButton<DialogUnionDetails>(checked, Valentina::UnionDetails, ":/cursor/union_cursor.png",
                                      tr("Select detail"), &MainWindow::ClosedDialogUnionDetails);
    //Must disconnect this signal here.
    disconnect(doc, &VPattern::FullUpdateFromFile, dialogTool, &DialogTool::UpdateList);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogUnionDetails actions after closing DialogUnionDetails.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogUnionDetails(int result)
{
    ClosedDialog<VToolUnionDetails>(result);
    doc->FullUpdateTree();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolCutArc handler tool cutArc.
 * @param checked true - button checked.
 */
void MainWindow::ToolCutArc(bool checked)
{
    SetToolButton<DialogCutArc>(checked, Valentina::CutArcTool, ":/cursor/arc_cut_cursor.png", tr("Select arc"),
                  &MainWindow::ClosedDialogCutArc);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogCutArc actions after closing DialogCutArc.
 * @param result result of dialog working.
 */
void MainWindow::ClosedDialogCutArc(int result)
{
    ClosedDialog<VToolCutArc>(result);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief About show widows about.
 */
void MainWindow::About()
{
    DialogAboutApp * about_dialog = new DialogAboutApp(this);
    about_dialog->setAttribute(Qt::WA_DeleteOnClose,true);
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
/**
 * @brief tableClosed handle after close layout window.
 */
void MainWindow::tableClosed()
{
    show();
    MinimumScrollBar();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::OpenRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        LoadPattern(action->data().toString());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::PatternProperties()
{
    DialogPatternProperties proper(doc, this);
    proper.exec();
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
    if (MaybeSave())
    {
        WriteSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBarOption enable option toolbar.
 */
void MainWindow::ToolBarOption()
{
    if (qApp->patternType() == Pattern::Standard)
    {
        ui->toolBarOption->addWidget(new QLabel(tr("Height: ")));

        QStringList list;
        list <<"92"<<"98"<<"104"<<"110"<<"116"<<"122"<<"128"<<"134"<<"140"<<"146"<<"152"<<"158"<<"164"<<"170"<<"176"
             <<"182"<<"188";
        QComboBox *comboBoxHeight = new QComboBox;
        comboBoxHeight->addItems(list);
        comboBoxHeight->setCurrentIndex(14);//176
        ui->toolBarOption->addWidget(comboBoxHeight);
        connect(comboBoxHeight, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::ChangedHeight);

        ui->toolBarOption->addWidget(new QLabel(tr(" Size: ")));

        list.clear();
        list <<"22"<<"24"<<"26"<<"28"<<"30"<<"32"<<"34"<<"36"<<"38"<<"40"<<"42"<<"44"<<"46"<<"48"<<"50"<<"52"<<"54"
             <<"56";
        QComboBox *comboBoxSize = new QComboBox;
        comboBoxSize->addItems(list);
        comboBoxSize->setCurrentIndex(14);//50
        ui->toolBarOption->addWidget(comboBoxSize);
        connect(comboBoxSize, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::ChangedSize);

        ui->toolBarOption->addSeparator();
    }

    mouseCoordinate = new QLabel("0, 0");
    ui->toolBarOption->addWidget(mouseCoordinate);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ToolBarDraws enable draw toolbar.
 */
void MainWindow::ToolBarDraws()
{
    QLabel *labelPtternPieceName = new QLabel(tr("Pattern Piece: "));
    ui->toolBarDraws->addWidget(labelPtternPieceName);

    comboBoxDraws = new QComboBox;
    ui->toolBarDraws->addWidget(comboBoxDraws);
    comboBoxDraws->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::currentDrawChanged);

    ui->toolBarDraws->addAction(ui->actionOptionDraw);
    ui->actionOptionDraw->setEnabled(false);

    ui->toolBarDraws->addAction(ui->actionTable);
    ui->actionTable->setEnabled(false);

    ui->toolBarDraws->addAction(ui->actionHistory);
    ui->actionHistory->setEnabled(false);
    connect(ui->actionHistory, &QAction::triggered, this, &MainWindow::ActionHistory);

    ui->toolBarDraws->addAction(ui->actionLayout);
    connect(ui->actionLayout, &QAction::triggered, this, &MainWindow::ActionLayout);
    ui->actionLayout->setEnabled(false);
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
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief currentDrawChanged change active pattern peace.
 * @param index index in combobox.
 */
void MainWindow::currentDrawChanged( int index )
{
    if (index != -1)
    {
        doc->setCurrentData();
        doc->ChangeActivDraw(comboBoxDraws->itemText(index));
        if (drawMode)
        {
            ArrowTool();
            quint32 id = doc->SPointActiveDraw();
            if (id != 0)
            {
                const VPointF *p = pattern->GeometricObject<const VPointF *>(id);
                view->centerOn(p->toQPointF());
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseMove save mouse position and show user.
 * @param scenePos position mouse.
 */
void MainWindow::mouseMove(const QPointF &scenePos)
{
    QString string = QString("%1, %2").arg(static_cast<qint32>(qApp->fromPixel(scenePos.x())))
                                      .arg(static_cast<qint32>(qApp->fromPixel(scenePos.y())));
    if (mouseCoordinate != nullptr)
    {
        mouseCoordinate->setText(string);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CanselTool cansel tool.
 */
void MainWindow::CancelTool()
{
    delete dialogTool;
    dialogTool = nullptr;
    switch ( tool )
    {
        case Valentina::ArrowTool:
            ui->actionArrowTool->setChecked(false);
            helpLabel->setText("");
            break;
        case Valentina::SinglePointTool:
            Q_UNREACHABLE();
            //Nothing to do here because we can't create this tool from main window.
            break;
        case Valentina::EndLineTool:
            ui->toolButtonEndLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::LineTool:
            ui->toolButtonLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearFocus();
            break;
        case Valentina::AlongLineTool:
            ui->toolButtonAlongLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::ShoulderPointTool:
            ui->toolButtonShoulderPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::NormalTool:
            ui->toolButtonNormal->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::BisectorTool:
            ui->toolButtonBisector->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::LineIntersectTool:
            ui->toolButtonLineIntersect->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::SplineTool:
            ui->toolButtonSpline->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::ArcTool:
            ui->toolButtonArc->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::SplinePathTool:
            ui->toolButtonSplinePath->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::PointOfContact:
            ui->toolButtonPointOfContact->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::DetailTool:
            ui->toolButtonNewDetail->setChecked(false);
            break;
        case Valentina::Height:
            ui->toolButtonHeight->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::Triangle:
            ui->toolButtonTriangle->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::PointOfIntersection:
            ui->toolButtonPointOfIntersection->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::CutSplineTool:
            ui->toolButtonSplineCutPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::CutSplinePathTool:
            ui->toolButtonSplinePathCutPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::UnionDetails:
            ui->toolButtonUnionDetails->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Valentina::CutArcTool:
            ui->toolButtonArcCutPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        default:
            qDebug()<<"Got wrong tool type. Ignored.";
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowTool enable arrow tool.
 */
void  MainWindow::ArrowTool()
{
    CancelTool();
    ui->actionArrowTool->setChecked(true);
    tool = Valentina::ArrowTool;
    QCursor cur(Qt::ArrowCursor);
    view->setCursor(cur);
    helpLabel->setText("");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ActionAroowTool set arrow tool. Cansel tool what was before.
 */
void MainWindow::ActionAroowTool()
{
    ArrowTool();
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
    /*Save transform*/
    currentScene->setTransform(view->transform());
    /*Save scroll bars value for previous scene.*/
    QScrollBar *horScrollBar = view->horizontalScrollBar();
    currentScene->setHorScrollBar(horScrollBar->value());
    QScrollBar *verScrollBar = view->verticalScrollBar();
    currentScene->setVerScrollBar(verScrollBar->value());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RestoreCurrentScene restore scene options after change.
 */
void MainWindow::RestoreCurrentScene()
{
    /*Set transform for current scene*/
    view->setTransform(currentScene->transform());
    /*Set value for current scene scroll bar.*/
    QScrollBar *horScrollBar = view->horizontalScrollBar();
    horScrollBar->setValue(currentScene->getHorScrollBar());
    QScrollBar *verScrollBar = view->verticalScrollBar();
    verScrollBar->setValue(currentScene->getVerScrollBar());
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
        ui->actionDetails->setChecked(false);
        SaveCurrentScene();

        currentScene = sceneDraw;
        view->setScene(currentScene);
        connect(view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
        RestoreCurrentScene();

        mode = Valentina::Calculation;
        comboBoxDraws->setEnabled(true);
        comboBoxDraws->setCurrentIndex(currentDrawIndex);//restore current pattern peace
        drawMode = true;

        SetEnableTool(true);
        doc->setCurrentData();
        ui->toolBox->setCurrentIndex(currentToolBoxIndex);

        ui->actionHistory->setEnabled(true);
        ui->actionLayout->setEnabled(false);
        ui->actionOptionDraw->setEnabled(true);
        ui->actionNewDraw->setEnabled(true);
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
        ui->actionDraw->setChecked(false);
        SaveCurrentScene();

        currentScene = sceneDetails;
        view->setScene(sceneDetails);
        disconnect(view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
        RestoreCurrentScene();

        drawMode = false;
        currentDrawIndex = comboBoxDraws->currentIndex();//save current pattern peace
        comboBoxDraws->setCurrentIndex(comboBoxDraws->count()-1);
        comboBoxDraws->setEnabled(false);


        mode = Valentina::Modeling;
        SetEnableTool(true);
        currentToolBoxIndex = ui->toolBox->currentIndex();
        ui->toolBox->setCurrentIndex(4);

        ui->actionHistory->setEnabled(false);
        ui->actionLayout->setEnabled(true);
        ui->actionOptionDraw->setEnabled(false);
        ui->actionNewDraw->setEnabled(false);
    }
    else
    {
        ui->actionDetails->setChecked(true);
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
        dir = QDir::homePath() + tr("/pattern.val");
    }
    else
    {
        dir = QFileInfo(curFile).absolutePath() + tr("/pattern.val");
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
    return SavePattern(fileName);
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
        bool result = SavePattern(curFile);
        if (result)
        {
            QString autofile = curFile +".autosave";
            QFile file(autofile);
            file.remove();
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
    if (MaybeSave())
    {
        QString filter(tr("Pattern files (*.val)"));
        QString dir;
        if (curFile.isEmpty())
        {
            dir = QDir::homePath();
        }
        else
        {
            dir = QFileInfo(curFile).absolutePath();
        }
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), dir, filter);
        if (fileName.isEmpty() == false)
        {
            LoadPattern(fileName);

            VAbstractTool::NewSceneRect(sceneDraw, view);
            VAbstractTool::NewSceneRect(sceneDetails, view);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Options config dialog.
 */
void MainWindow::Options()
{
    ConfigDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        InitAutoSave();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear reset to default window.
 */
void MainWindow::Clear()
{
    setCurrentFile("");
    pattern->Clear();
    doc->clear();
    sceneDraw->clear();
    sceneDetails->clear();
    CancelTool();
    comboBoxDraws->clear();
    ui->actionOptionDraw->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionPattern_properties->setEnabled(false);
    SetEnableTool(false);
    qApp->setPatternUnit(Valentina::Cm);
    qApp->setPatternType(Pattern::Individual);
    ui->toolBarOption->clear();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewPattern create new empty pattern.
 */
void MainWindow::NewPattern()
{
    QProcess *v = new QProcess(this);
    v->startDetached(QCoreApplication::applicationFilePath ());
    delete v;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief haveChange enable action save if we have unsaved change.
 */
void MainWindow::PatternWasModified()
{
    setWindowModified(doc->isPatternModified());
    ui->actionSave->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedSize change new size value.
 * @param text value size.
 */
void MainWindow::ChangedSize(const QString & text)
{
    qint32 size = text.toInt();
    pattern->SetSize(size);
    doc->FullUpdateTree();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedGrowth change new height value.
 * @param text value height.
 */
void MainWindow::ChangedHeight(const QString &text)
{
    qint32 growth = text.toInt();
    pattern->SetHeight(growth);
    doc->FullUpdateTree();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetEnableWidgets enable action button.
 * @param enable enable value.
 */
void MainWindow::SetEnableWidgets(bool enable)
{
    ui->actionSaveAs->setEnabled(enable);
    ui->actionDraw->setEnabled(enable);
    ui->actionDetails->setEnabled(enable);
    ui->actionOptionDraw->setEnabled(enable);
    if (enable == true && curFile.isEmpty() == false)
    {
        ui->actionSave->setEnabled(enable);
    }
    ui->actionTable->setEnabled(enable);
    ui->actionHistory->setEnabled(enable);
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
/**
 * @brief ActionLayout begin creation layout.
 * @param checked true - button checked.
 */
void MainWindow::ActionLayout(bool checked)
{
    Q_UNUSED(checked);
    hide();
    QVector<VItem*> listDetails;
    const QHash<quint32, VDetail> *details = pattern->DataDetails();
    QHashIterator<quint32, VDetail> idetail(*details);
    while (idetail.hasNext())
    {
        idetail.next();
        QPainterPath path = VEquidistant().ContourPath(idetail.key(), pattern);
        listDetails.append(new VItem(path, listDetails.size()));
    }
    QString description = doc->UniqueTagText("description");
    emit ModelChosen(listDetails, curFile, description);
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
    if (mode == Valentina::Calculation)
    {
        drawTools = enable;
    }
    else
    {
        modelingTools = enable;
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

    //Modeling Tools
    ui->toolButtonUnionDetails->setEnabled(modelingTools);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MinimumScrollBar set scroll bar to minimum.
 */
void MainWindow::MinimumScrollBar()
{
    QScrollBar *horScrollBar = view->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar *verScrollBar = view->verticalScrollBar();
    verScrollBar->setValue(verScrollBar->minimum());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SavePattern save pattern file.
 * @param fileName pattern file name.
 * @return true if all is good.
 */
bool MainWindow::SavePattern(const QString &fileName)
{
    QFileInfo tempInfo(fileName);
    const bool result = doc->SaveDocument(fileName);
    if (result)
    {
        if (tempInfo.suffix() != "autosave")
        {
            setCurrentFile(fileName);
            helpLabel->setText(tr("File saved"));
        }
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AutoSavePattern start safe saving.
 */
void MainWindow::AutoSavePattern()
{
    if (curFile.isEmpty() == false && doc->isPatternModified() == true)
    {
        QString autofile = curFile +".autosave";
        if (SavePattern(autofile) == false)
        {
            qDebug()<<"Can not save pattern"<<Q_FUNC_INFO;
        }
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
    curFile = fileName;
    doc->setPatternModified(false);
    setWindowModified(false);

    QString shownName = strippedName(curFile);
    if (curFile.isEmpty())
    {
        shownName = tr("untitled.val");
    }
    else
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                           QApplication::applicationName());
        QStringList files = settings.value("recentFileList").toStringList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while (files.size() > MaxRecentFiles)
        {
            files.removeLast();
        }

        settings.setValue("recentFileList", files);
        UpdateRecentFileActions();
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
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    QPoint pos = settings.value("pos", QPoint(10, 10)).toPoint();
    QSize size = settings.value("size", QSize(1000, 800)).toSize();
    resize(size);
    move(pos);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief WriteSettings save setting for app.
 */
void MainWindow::WriteSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MaybeSave The function is called to save pending changes.
 * @return returns true in all cases, except when the user clicks Cancel.
 */
bool MainWindow::MaybeSave()
{
    if (doc->isPatternModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Unsaved change"), tr("The pattern has been modified.\n"
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
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), static_cast<int>(MaxRecentFiles));

    for (int i = 0; i < numRecentFiles; ++i)
    {
       QString text = QString("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
       recentFileActs[i]->setText(text);
       recentFileActs[i]->setData(files[i]);
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
        ui->menuFile->insertAction(ui->actionExit, recentFileActs[i]);
    }
    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
    ui->menuFile->insertAction(ui->actionExit, separatorAct);
    UpdateRecentFileActions();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::CreateActions()
{
    ui->setupUi(this);

    connect(ui->actionArrowTool, &QAction::triggered, this, &MainWindow::ActionAroowTool);
    connect(ui->actionDraw, &QAction::triggered, this, &MainWindow::ActionDraw);
    connect(ui->actionDetails, &QAction::triggered, this, &MainWindow::ActionDetails);
    connect(ui->actionNewDraw, &QAction::triggered, this, &MainWindow::ActionNewDraw);
    connect(ui->actionOptionDraw, &QAction::triggered, this, &MainWindow::OptionDraw);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::SaveAs);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::Save);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::Open);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::NewPattern);
    connect(ui->actionTable, &QAction::triggered, this, &MainWindow::ActionTable);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::AboutQt);
    connect(ui->actionAbout_Valentina, &QAction::triggered, this, &MainWindow::About);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::Options);
    connect(ui->actionPattern_properties, &QAction::triggered, this, &MainWindow::PatternProperties);
    ui->actionPattern_properties->setEnabled(false);

    //Actions for recent files loaded by a main window application.
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], &QAction::triggered, this, &MainWindow::OpenRecentFile);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::InitAutoSave()
{
    //Autosaving file each 5 minutes
    delete autoSaveTimer;
    autoSaveTimer = nullptr;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    bool autoSave = settings.value("configuration/autosave/state", 1).toBool();
    if (autoSave)
    {
        bool ok = true;
        qint32 autoTime = settings.value("configuration/autosave/time", 5).toInt(&ok);
        if (ok == false)
        {
            autoTime = 5;
        }

        autoSaveTimer = new QTimer(this);
        autoSaveTimer->setTimerType(Qt::VeryCoarseTimer);
        connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::AutoSavePattern);
        autoSaveTimer->start(autoTime*60000);
    }
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
            break;
        }
    }
    delete dlg;
    return nameDraw;
}

//---------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    CancelTool();
    delete ui;

    delete pattern;
    delete doc;
    delete sceneDetails;
    delete sceneDraw;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LoadPattern open pattern file.
 * @param fileName name of file.
 */
void MainWindow::LoadPattern(const QString &fileName)
{
    try
    {
        VDomDocument::ValidateXML("://schema/pattern.xsd", fileName);
        doc->setContent(fileName);

        qApp->setPatternUnit(doc->MUnit());
        qApp->setPatternType(doc->MType());
        QString path = doc->MPath();

        path = CheckPathToMeasurements(path, qApp->patternType());
        if (path.isEmpty())
        {
            Clear();
            return;
        }

        QString text = tr("Measurements use different units than pattern. This pattern required measurements in %1")
                .arg(doc->UnitsToStr(qApp->patternUnit()));
        if (qApp->patternType() == Pattern::Standard)
        {
            VStandardMeasurements m(pattern);
            m.setContent(path);
            Valentina::Units mUnit = m.Unit();
            if (qApp->patternUnit() != mUnit)
            {
                QMessageBox::critical(this, tr("Wrong units."), text);
                Clear();
                return;
            }
            m.SetSize();
            m.SetHeight();
            ToolBarOption();
        }
        else
        {
            VIndividualMeasurements m(pattern);
            m.setContent(path);
            Valentina::Units mUnit = m.Unit();
            if (qApp->patternUnit() != mUnit)
            {
                QMessageBox::critical(this, tr("Wrong units."), text);
                Clear();
                return;
            }
        }
    }
    catch (VException &e)
    {
        e.CriticalMessageBox(tr("File error."), this);
        Clear();
        return;
    }

    disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               this, &MainWindow::currentDrawChanged);
    try
    {
        #ifndef QT_NO_CURSOR
            QApplication::setOverrideCursor(Qt::WaitCursor);
        #endif

        doc->Parse(Document::FullParse, sceneDraw, sceneDetails);

        #ifndef QT_NO_CURSOR
            QApplication::restoreOverrideCursor();
        #endif
            ui->actionPattern_properties->setEnabled(true);
    }
    catch (const VExceptionObjectError &e)
    {
        e.CriticalMessageBox(tr("Error parsing file."), this);
        Clear();
        return;
    }
    catch (const VExceptionConversionError &e)
    {
        e.CriticalMessageBox(tr("Error can't convert value."), this);
        Clear();
        return;
    }
    catch (const VExceptionEmptyParameter &e)
    {
        e.CriticalMessageBox(tr("Error empty parameter."), this);
        Clear();
        return;
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error wrong id."), this);
        Clear();
        return;
    }
    catch (VException &e)
    {
        e.CriticalMessageBox(tr("Error parsing file."), this);
        Clear();
        return;
    }
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::currentDrawChanged);
    QString nameDraw = doc->GetNameActivDraw();
    qint32 index = comboBoxDraws->findText(nameDraw);
    if ( index != -1 )
    { // -1 for not found
        comboBoxDraws->setCurrentIndex(index);
    }
    if (comboBoxDraws->count() > 0)
    {
        SetEnableTool(true);
    }
    else
    {
        SetEnableTool(false);
    }
    SetEnableWidgets(true);

    setCurrentFile(fileName);
    helpLabel->setText(tr("File loaded"));
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::CheckPathToMeasurements(const QString &path, const Pattern::Measurements &patternType)
{
    QFile table(path);
    if (table.exists() == false)
    {
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
            QString filter;
            if (patternType == Pattern::Standard)
            {
                filter = tr("Standard measurements (*.vst)");
            }
            else
            {
                filter = tr("Individual measurements (*.vit)");
            }
            QString mPath = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), filter);

            if (mPath.isEmpty())
            {
                return mPath;
            }
            else
            {
                if (patternType == Pattern::Standard)
                {
                    VDomDocument::ValidateXML("://schema/standard_measurements.xsd", mPath);
                }
                else
                {
                    VDomDocument::ValidateXML("://schema/individual_measurements.xsd", mPath);
                }
                doc->SetPath(mPath);
                return mPath;
            }
        }
    }
    return path;
}
