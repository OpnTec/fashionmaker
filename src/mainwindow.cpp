/************************************************************************
 **
 **  @file   mainwindow.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include <QInputDialog>
#include <QtCore>
#include <QDebug>
#include <QMessageBox>
#include <QShowEvent>
#include <QScrollBar>
#include <QFileDialog>
#include <QSourceLocation>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent), ui(new Ui::MainWindow), pattern(nullptr), doc(nullptr), tool(Valentina::ArrowTool),
      currentScene(nullptr), sceneDraw(nullptr), sceneDetails(nullptr), mouseCoordinate(nullptr), helpLabel(nullptr),
      view(nullptr), isInitialized(false), dialogTable(0), dialogTool(nullptr), dialogHistory(nullptr),
      comboBoxDraws(nullptr), curFile(QString()), mode(Valentina::Calculation), currentDrawIndex(0), currentToolBoxIndex(0),
      drawMode(true), recentFileActs{0, 0, 0, 0, 0}, separatorAct(nullptr), autoSaveTimer(nullptr)
{
    CreateActions();
    CreateMenus();
    ToolBarOption();
    ToolBarDraws();

    sceneDraw = new VMainGraphicsScene();
    currentScene = sceneDraw;
    connect(sceneDraw, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    sceneDetails = new VMainGraphicsScene();
    connect(sceneDetails, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    view = new VMainGraphicsView();
    ui->LayoutView->addWidget(view);
    view->setScene(currentScene);

    sceneDraw->setTransform(view->transform());
    sceneDetails->setTransform(view->transform());

    connect(view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(12);
    view->setSizePolicy(policy);
    helpLabel = new QLabel(QObject::tr("Create new pattern piece to start working."));
    ui->statusBar->addWidget(helpLabel);

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

    pattern = new VContainer();

    doc = new VPattern(pattern, comboBoxDraws, &mode);
    doc->CreateEmptyFile();
    connect(doc, &VPattern::patternChanged, this, &MainWindow::PatternWasModified);

    InitAutoSave();

    ui->toolBox->setCurrentIndex(0);

    ReadSettings();

    setCurrentFile("");
}

void MainWindow::ActionNewDraw()
{
    QString nameDraw;
    if (comboBoxDraws->count() == 0)
    {
        DialogMeasurements measurements(this);
        measurements.exec();
        if (measurements.type() == Measurements::Standard)
        {
            DialogStandardMeasurements stMeasurements(pattern, this);
            if (stMeasurements.exec() == QDialog::Accepted)
            {
                nameDraw = stMeasurements.name();
            }
            else
            {
                return;
            }
        }
        else
        {
            DialogIndividualMeasurements indMeasurements(this);
            if (indMeasurements.exec() == QDialog::Accepted)
            {
                nameDraw = indMeasurements.name();
            }
            else
            {
                return;
            }
        }
    }
    else
    {
        nameDraw = PatternPieceName(QString(tr("Pattern piece %1")).arg(comboBoxDraws->count()+1));
        if (nameDraw.isEmpty())
        {
            return;
        }
    }
    if (doc->appendDraw(nameDraw) == false)
    {
        qWarning()<<tr("Error creating pattern with the name ")<<nameDraw<<".";
        return;
    }
    disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               this, &MainWindow::currentDrawChanged);
    comboBoxDraws->addItem(nameDraw);

    pattern->ClearGObjects();
    //Create single point
    const quint32 id = pattern->AddGObject(new VPointF(toPixel((10+comboBoxDraws->count()*5)), toPixel(10), "А", 5,
                                                       10));
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

    const qint32 index = comboBoxDraws->findText(nameDraw);
    if ( index != -1 )
    { // -1 for not found
        comboBoxDraws->setCurrentIndex(index);
        currentDrawChanged( index );
    }
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &MainWindow::currentDrawChanged);
}

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

void MainWindow::ToolEndLine(bool checked)
{
    SetToolButton<DialogEndLine>(checked, Valentina::EndLineTool, ":/cursor/endline_cursor.png", tr("Select point"),
                                 &MainWindow::ClosedDialogEndLine);
}

void MainWindow::ClosedDialogEndLine(int result)
{
    ClosedDialog<VToolEndLine>(result);
}

void MainWindow::ToolLine(bool checked)
{
    SetToolButton<DialogLine>(checked, Valentina::LineTool, ":/cursor/line_cursor.png", tr("Select first point"),
                              &MainWindow::ClosedDialogLine);
}

void MainWindow::ClosedDialogLine(int result)
{
    ClosedDialog<VToolLine>(result);
}

void MainWindow::ToolAlongLine(bool checked)
{
    SetToolButton<DialogAlongLine>(checked, Valentina::AlongLineTool, ":/cursor/alongline_cursor.png", tr("Select point"),
                                   &MainWindow::ClosedDialogAlongLine);
}

void MainWindow::ClosedDialogAlongLine(int result)
{
    ClosedDialog<VToolAlongLine>(result);
}

void MainWindow::ToolShoulderPoint(bool checked)
{
    SetToolButton<DialogShoulderPoint>(checked, Valentina::ShoulderPointTool, ":/cursor/shoulder_cursor.png",
                  tr("Select first point of line"), &MainWindow::ClosedDialogShoulderPoint);
}

void MainWindow::ClosedDialogShoulderPoint(int result)
{
    ClosedDialog<VToolShoulderPoint>(result);
}

void MainWindow::ToolNormal(bool checked)
{
    SetToolButton<DialogNormal>(checked, Valentina::NormalTool, ":/cursor/normal_cursor.png",
                  tr("Select first point of line"), &MainWindow::ClosedDialogNormal);
}

void MainWindow::ClosedDialogNormal(int result)
{
    ClosedDialog<VToolNormal>(result);
}

void MainWindow::ToolBisector(bool checked)
{
    SetToolButton<DialogBisector>(checked, Valentina::BisectorTool, ":/cursor/bisector_cursor.png",
                  tr("Select first point of angle"), &MainWindow::ClosedDialogBisector);
}

void MainWindow::ClosedDialogBisector(int result)
{
    ClosedDialog<VToolBisector>(result);
}

void MainWindow::ToolLineIntersect(bool checked)
{
    SetToolButton<DialogLineIntersect>(checked, Valentina::LineIntersectTool, ":/cursor/intersect_cursor.png",
                  tr("Select first point of first line"), &MainWindow::ClosedDialogLineIntersect);
}

void MainWindow::ClosedDialogLineIntersect(int result)
{
    ClosedDialog<VToolLineIntersect>(result);
}

void MainWindow::ToolSpline(bool checked)
{
    SetToolButton<DialogSpline>(checked, Valentina::SplineTool, ":/cursor/spline_cursor.png",
                  tr("Select first point curve"), &MainWindow::ClosedDialogSpline);
}

void MainWindow::ClosedDialogSpline(int result)
{
    ClosedDialog<VToolSpline>(result);
}

void MainWindow::ToolCutSpline(bool checked)
{
    SetToolButton<DialogCutSpline>(checked, Valentina::CutSplineTool, ":/cursor/spline_cut_point_cursor.png",
                  tr("Select simple curve"), &MainWindow::ClosedDialogCutSpline);
}

void MainWindow::ClosedDialogCutSpline(int result)
{
    ClosedDialog<VToolCutSpline>(result);
}

void MainWindow::ToolArc(bool checked)
{
    SetToolButton<DialogArc>(checked, Valentina::ArcTool, ":/cursor/arc_cursor.png",
                  tr("Select point of center of arc"), &MainWindow::ClosedDialogArc);
}

void MainWindow::ClosedDialogArc(int result)
{
    ClosedDialog<VToolArc>(result);
}

void MainWindow::ToolSplinePath(bool checked)
{
    SetToolButton<DialogSplinePath>(checked, Valentina::SplinePathTool, ":/cursor/splinepath_cursor.png",
                  tr("Select point of curve path"), &MainWindow::ClosedDialogSplinePath);
}

void MainWindow::ClosedDialogSplinePath(int result)
{
    ClosedDialog<VToolSplinePath>(result);
}

void MainWindow::ToolCutSplinePath(bool checked)
{
    SetToolButton<DialogCutSplinePath>(checked, Valentina::CutSplinePathTool, ":/cursor/splinepath_cut_point_cursor.png",
                  tr("Select curve path"), &MainWindow::ClosedDialogCutSplinePath);
}

void MainWindow::ClosedDialogCutSplinePath(int result)
{
    ClosedDialog<VToolCutSplinePath>(result);
}

void MainWindow::ToolPointOfContact(bool checked)
{
    SetToolButton<DialogPointOfContact>(checked, Valentina::PointOfContact, ":/cursor/pointcontact_cursor.png",
                  tr("Select first point of line"), &MainWindow::ClosedDialogPointOfContact);
}

void MainWindow::ClosedDialogPointOfContact(int result)
{
    ClosedDialog<VToolPointOfContact>(result);
}

void MainWindow::ToolDetail(bool checked)
{
    SetToolButton<DialogDetail>(checked, Valentina::DetailTool, "://cursor/new_detail_cursor.png",
                                tr("Select points, arcs, curves clockwise."), &MainWindow::ClosedDialogDetail);
}

void MainWindow::ClosedDialogDetail(int result)
{
    if (result == QDialog::Accepted)
    {
        VToolDetail::Create(dialogTool, sceneDetails, doc, pattern);
    }
    ArrowTool();
    doc->FullUpdateTree();
}

void MainWindow::ToolHeight(bool checked)
{
    SetToolButton<DialogHeight>(checked, Valentina::Height, ":/cursor/height_cursor.png", tr("Select base point"),
                                &MainWindow::ClosedDialogHeight);
}

void MainWindow::ClosedDialogHeight(int result)
{
    ClosedDialog<VToolHeight>(result);
}

void MainWindow::ToolTriangle(bool checked)
{
    SetToolButton<DialogTriangle>(checked, Valentina::Triangle, ":/cursor/triangle_cursor.png",
                                  tr("Select first point of axis"), &MainWindow::ClosedDialogTriangle);
}

void MainWindow::ClosedDialogTriangle(int result)
{
    ClosedDialog<VToolTriangle>(result);
}

void MainWindow::ToolPointOfIntersection(bool checked)
{
    SetToolButton<DialogPointOfIntersection>(checked, Valentina::PointOfIntersection, ":/cursor/pointofintersect_cursor.png",
                  tr("Select point vertically"), &MainWindow::ClosedDialogPointOfIntersection);
}

void MainWindow::ClosedDialogPointOfIntersection(int result)
{
    ClosedDialog<VToolPointOfIntersection>(result);
}

void MainWindow::ToolUnionDetails(bool checked)
{
    SetToolButton<DialogUnionDetails>(checked, Valentina::UnionDetails, ":/cursor/union_cursor.png", tr("Select detail"),
                                      &MainWindow::ClosedDialogUnionDetails);
    //Must disconnect this signal here.
    disconnect(doc, &VPattern::FullUpdateFromFile, dialogTool, &DialogTool::UpdateList);
}

void MainWindow::ClosedDialogUnionDetails(int result)
{
    ClosedDialog<VToolUnionDetails>(result);
    doc->FullUpdateTree();
}

void MainWindow::ToolCutArc(bool checked)
{
    SetToolButton<DialogCutArc>(checked, Valentina::CutArcTool, ":/cursor/arc_cut_cursor.png", tr("Select arc"),
                  &MainWindow::ClosedDialogCutArc);
}

void MainWindow::ClosedDialogCutArc(int result)
{
    ClosedDialog<VToolCutArc>(result);
}

void MainWindow::About()
{
    QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));

    QString fullName = QString("Valentina %1").arg(APP_VERSION);
    QString qtBase(tr("Based on Qt %2 (32 bit)").arg(QT_VERSION_STR));
    QString buildOn(tr("Built on %3 at %4").arg(date.toString()).arg(__TIME__));
    QString about = QString(tr("<h1>%1</h1> %2 <br/><br/> %3 <br/><br/> %4")).arg(fullName).arg(qtBase).arg(buildOn)
            .arg(WARRANTY);
    QMessageBox::about(this, tr("About Valentina"), about);
}

void MainWindow::AboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::ShowToolTip(const QString &toolTip)
{
    helpLabel->setText(toolTip);
}

void MainWindow::tableClosed()
{
    show();
    MinimumScrollBar();
}

void MainWindow::OpenRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        LoadPattern(action->data().toString());
    }
}

void MainWindow::PatternProperties()
{
    DialogPatternProperties proper(doc, this);
    proper.exec();
}

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

void MainWindow::ToolBarOption()
{
    QLabel * labelGrowth = new QLabel;
    labelGrowth->setText(tr("Height: "));
    ui->toolBarOption->addWidget(labelGrowth);

    QStringList list;
    list <<"92"<<"98"<<"104"<<"110"<<"116"<<"122"<<"128"<<"134"<<"140"<<"146"<<"152"<<"158"<<"164"<<"170"<<"176"
         <<"182"<<"188";
    QComboBox *comboBoxGrow = new QComboBox;
    comboBoxGrow->clear();
    comboBoxGrow->addItems(list);
    comboBoxGrow->setCurrentIndex(14);
    ui->toolBarOption->addWidget(comboBoxGrow);
    connect(comboBoxGrow, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::ChangedHeight);

    QLabel * labelSize = new QLabel;
    labelSize->setText(tr(" Size: "));
    ui->toolBarOption->addWidget(labelSize);

    list.clear();
    list <<"22"<<"24"<<"26"<<"28"<<"30"<<"32"<<"34"<<"36"<<"38"<<"40"<<"42"<<"44"<<"46"<<"48"<<"50"<<"52"<<"54"<<"56";
    QComboBox *comboBoxSize = new QComboBox;
    comboBoxSize->clear();
    comboBoxSize->addItems(list);
    comboBoxSize->setCurrentIndex(14);
    ui->toolBarOption->addWidget(comboBoxSize);
    connect(comboBoxSize, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::ChangedSize);

    ui->toolBarOption->addSeparator();

    mouseCoordinate = new QLabel;
    mouseCoordinate ->setText("0, 0");
    ui->toolBarOption->addWidget(mouseCoordinate);
}

void MainWindow::ToolBarDraws()
{
    QLabel * labelNameDraw = new QLabel;
    labelNameDraw ->setText(tr("Pattern Piece: "));
    ui->toolBarDraws->addWidget(labelNameDraw);

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

void MainWindow::mouseMove(const QPointF &scenePos)
{
    QString string = QString("%1, %2")
                            .arg(static_cast<qint32>(fromPixel(scenePos.x())))
                            .arg(static_cast<qint32>(fromPixel(scenePos.y())));
    mouseCoordinate->setText(string);
}

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
            qWarning()<<"Got wrong tool type. Ignored.";
            break;
    }
}

void  MainWindow::ArrowTool()
{
    CancelTool();
    ui->actionArrowTool->setChecked(true);
    tool = Valentina::ArrowTool;
    QCursor cur(Qt::ArrowCursor);
    view->setCursor(cur);
    helpLabel->setText("");
}

void MainWindow::ActionAroowTool()
{
    ArrowTool();
}

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

void MainWindow::Options()
{
    ConfigDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        InitAutoSave();
    }
}

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
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void MainWindow::NewPattern()
{
    QProcess *v = new QProcess(this);
    v->startDetached(QCoreApplication::applicationFilePath ());
    delete v;
}

void MainWindow::PatternWasModified()
{
    setWindowModified(doc->isPatternModified());
    ui->actionSave->setEnabled(true);
}

void MainWindow::ChangedSize(const QString & text)
{
    qint32 size = text.toInt();
    pattern->SetSize(size*10);
    doc->FullUpdateTree();
}

void MainWindow::ChangedHeight(const QString &text)
{
    qint32 growth = text.toInt();
    pattern->SetHeight(growth*10);
    doc->FullUpdateTree();
}

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

void MainWindow::ClosedActionTable()
{
    ui->actionTable->setChecked(false);
    delete dialogTable;
    dialogTable = nullptr;
}

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

void MainWindow::ClosedActionHistory()
{
    ui->actionHistory->setChecked(false);
    delete dialogHistory;
}

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

void MainWindow::MinimumScrollBar()
{
    QScrollBar *horScrollBar = view->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar *verScrollBar = view->verticalScrollBar();
    verScrollBar->setValue(verScrollBar->minimum());
}

bool MainWindow::SavePattern(const QString &fileName)
{
    try
    {
        doc->TestUniqueId();
    }
    catch (const VExceptionWrongId &e)
    {
        e.CriticalMessageBox(tr("Error no unique id."), this);
        return false;
    }
    if (fileName.isEmpty())
    {
        qWarning()<<tr("Got empty file name.");
        return false;
    }
    //Writing in temporary file
    QFileInfo tempInfo(fileName);
    QString temp = tempInfo.absolutePath() + "/" + tempInfo.baseName() + ".tmp";
    QFile tempFile(temp);
    if (tempFile.open(QIODevice::WriteOnly| QIODevice::Truncate))
    {
        const int Indent = 4;
        QTextStream out(&tempFile);
        doc->save(out, Indent);
        tempFile.close();
    }
    //Replace temp file our
    bool result = false;
    QFile patternFile(fileName);
    // We need here temporary file because we need restore pattern after error of copying temp file.
    QTemporaryFile tempOfPattern;
    if (tempOfPattern.open())
    {
        patternFile.copy(tempOfPattern.fileName());
    }
    if ( patternFile.exists() == false || patternFile.remove() )
    {
        if ( tempFile.copy(patternFile.fileName()) == false )
        {
            qWarning()<<tr("Could not copy temp file to pattern file")<<Q_FUNC_INFO;
            tempOfPattern.copy(fileName);
            result = false;
        }
        else
        {
            result = true;
        }
    }
    else
    {
        qWarning()<<tr("Could not remove pattern file")<<Q_FUNC_INFO;
        result = false;
    }
    if (result)
    {
        tempFile.remove();
        if (tempInfo.suffix() != "autosave")
        {
            setCurrentFile(fileName);
            helpLabel->setText(tr("File saved"));
        }
    }
    return result;
}

void MainWindow::AutoSavePattern()
{
    if (curFile.isEmpty() == false && doc->isPatternModified() == true)
    {
        QString autofile = curFile +".autosave";
        if (SavePattern(autofile) == false)
        {
            qWarning()<<tr("Can not save pattern")<<Q_FUNC_INFO;
        }
    }
}

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

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::ReadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    QPoint pos = settings.value("pos", QPoint(10, 10)).toPoint();
    QSize size = settings.value("size", QSize(1000, 800)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::WriteSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

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

MainWindow::~MainWindow()
{
    CancelTool();
    delete ui;

    delete pattern;
    delete doc;
    delete sceneDetails;
    delete sceneDraw;
}

void MainWindow::LoadPattern(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        try
        {
            VDomDocument::ValidatePattern("://schema/pattern.xsd", fileName);
        }
        catch(VException &e)
        {
            e.CriticalMessageBox(tr("Validation file error."), this);
            file.close();
            Clear();
            return;
        }

        try
        {
            doc->setContent(&file);
        }
        catch(VException &e)
        {
            e.CriticalMessageBox(tr("Parsing pattern file error."), this);
            file.close();
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
            file.close();
            Clear();
            return;
        }
        catch (const VExceptionConversionError &e)
        {
            e.CriticalMessageBox(tr("Error can't convert value."), this);
            file.close();
            Clear();
            return;
        }
        catch (const VExceptionEmptyParameter &e)
        {
            e.CriticalMessageBox(tr("Error empty parameter."), this);
            file.close();
            Clear();
            return;
        }
        catch (const VExceptionWrongId &e)
        {
            e.CriticalMessageBox(tr("Error wrong id."), this);
            file.close();
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

        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Valentina"), tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
    }
    setCurrentFile(fileName);
    helpLabel->setText(tr("File loaded"));
}
