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
#include "exception/vexceptionobjecterror.h"
#include "exception/vexceptionconversionerror.h"
#include "exception/vexceptionemptyparameter.h"
#include "exception/vexceptionwrongparameterid.h"
#include "exception/vexceptionuniqueid.h"
#include "version.h"

#include <QInputDialog>
#include <QtCore>
#include <QDebug>
#include <QMessageBox>
#include <QShowEvent>
#include <QScrollBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent), ui(new Ui::MainWindow), pattern(0), doc(0), tool(Tool::ArrowTool), currentScene(0),
      sceneDraw(0), sceneDetails(0), mouseCoordinate(0), helpLabel(0), view(0), isInitialized(false), dialogTable(0),
    dialogEndLine(QSharedPointer<DialogEndLine>()), dialogLine(QSharedPointer<DialogLine>()),
    dialogAlongLine(QSharedPointer<DialogAlongLine>()),
    dialogShoulderPoint(QSharedPointer<DialogShoulderPoint>()), dialogNormal(QSharedPointer<DialogNormal>()),
    dialogBisector(QSharedPointer<DialogBisector>()),
    dialogLineIntersect(QSharedPointer<DialogLineIntersect>()), dialogSpline(QSharedPointer<DialogSpline>()),
    dialogArc(QSharedPointer<DialogArc>()), dialogSplinePath(QSharedPointer<DialogSplinePath>()),
    dialogPointOfContact(QSharedPointer<DialogPointOfContact>()),
    dialogDetail(QSharedPointer<DialogDetail>()), dialogHeight(QSharedPointer<DialogHeight>()),
    dialogTriangle(QSharedPointer<DialogTriangle>()),
    dialogPointOfIntersection(QSharedPointer<DialogPointOfIntersection>()),
    dialogCutSpline(QSharedPointer<DialogCutSpline>()), dialogCutSplinePath (QSharedPointer<DialogCutSplinePath>()),
    dialogUnionDetails(QSharedPointer<DialogUnionDetails>()), dialogCutArc(QSharedPointer<DialogCutArc>()),
    dialogHistory(0), comboBoxDraws(0), fileName(QString()), changeInFile(false),
    mode(Draw::Calculation), currentDrawIndex(0), currentToolBoxIndex(0), drawMode(true)
{
    ui->setupUi(this);
    static const char * GENERIC_ICON_TO_CHECK = "document-open";
    static const char * FALLBACK_ICON_THEME = "win.icon.theme";
    if (!QIcon::hasThemeIcon(GENERIC_ICON_TO_CHECK)) {
        //If there is no default working icon theme then we should
        //use an icon theme that we provide via a .qrc file
        //This case happens under Windows and Mac OS X
        //This does not happen under GNOME or KDE
        QIcon::setThemeName(FALLBACK_ICON_THEME);
        ui->actionNew->setIcon(QIcon::fromTheme("document-new"));
        ui->actionOpen->setIcon(QIcon::fromTheme("document-open"));
        ui->actionSave->setIcon(QIcon::fromTheme("document-save"));
        ui->actionSaveAs->setIcon(QIcon::fromTheme("document-save-as"));
    }
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
    helpLabel = new QLabel(tr("Create new pattern piece to start working."));
    ui->statusBar->addWidget(helpLabel);

    connect(ui->actionArrowTool, &QAction::triggered, this, &MainWindow::ActionAroowTool);
    connect(ui->actionDraw, &QAction::triggered, this, &MainWindow::ActionDraw);
    connect(ui->actionDetails, &QAction::triggered, this, &MainWindow::ActionDetails);
    connect(ui->actionNewDraw, &QAction::triggered, this, &MainWindow::ActionNewDraw);
    connect(ui->actionOptionDraw, &QAction::triggered, this, &MainWindow::OptionDraw);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::ActionSaveAs);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::ActionSave);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::ActionOpen);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::ActionNew);
    connect(ui->actionTable, &QAction::triggered, this, &MainWindow::ActionTable);
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

    doc = new VDomDocument(pattern, comboBoxDraws, &mode);
    doc->CreateEmptyFile();
    connect(doc, &VDomDocument::haveChange, this, &MainWindow::haveChange);

    //Autosaving file each 5 minutes
    QTimer *timer = new QTimer(this);
    timer->setTimerType(Qt::VeryCoarseTimer);
    connect(timer, &QTimer::timeout, this, &MainWindow::AutoSavePattern);
    timer->start(300000);

    connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::AboutQt);
    connect(ui->actionAbout_Valentina, &QAction::triggered, this, &MainWindow::About);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    ui->toolBox->setCurrentIndex(0);
}

void MainWindow::ActionNewDraw()
{
    QString nameDraw;
    bool bOk;
    qint32 index;
    QString nDraw = QString(tr("Pattern piece %1")).arg(comboBoxDraws->count()+1);
    QInputDialog *dlg = new QInputDialog(this);
    dlg->setInputMode( QInputDialog::TextInput );
    dlg->setLabelText(tr("Pattern piece:"));
    dlg->setTextEchoMode(QLineEdit::Normal);
    dlg->setWindowTitle(tr("Enter a label for the pattern piece."));
    dlg->resize(300, 100);
    dlg->setTextValue(nDraw);
    while (1)
    {
        bOk = dlg->exec();
        nameDraw = dlg->textValue();
        if (bOk == false || nameDraw.isEmpty())
        {
            delete dlg;
            return;
        }
        index = comboBoxDraws->findText(nameDraw);
        if (index != -1)
        {//we already have this name
            qCritical()<<tr("Error. Pattern piece of same label already exists.");
        }
        else
        {
            break;
        }
    }
    delete dlg;
    bOk = doc->appendDraw(nameDraw);
    if (bOk == false)
    {
        qCritical()<<tr("Error creating pattern with the name ")<<nameDraw<<".";
        return;
    }
    disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               this, &MainWindow::currentDrawChanged);
    comboBoxDraws->addItem(nameDraw);
    index = comboBoxDraws->findText(nameDraw);
    if ( index != -1 )
    { // -1 for not found
        comboBoxDraws->setCurrentIndex(index);
        currentDrawChanged( index );
    }
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &MainWindow::currentDrawChanged);
    pattern->ClearGObjects();
    //Create single point
    qint64 id = pattern->AddGObject(new VPointF(toPixel((10+comboBoxDraws->count()*5)), toPixel(10), "А", 5, 10));
    VToolSinglePoint *spoint = new VToolSinglePoint(doc, pattern, id, Tool::FromGui);
    sceneDraw->addItem(spoint);
    connect(spoint, &VToolPoint::ChoosedTool, sceneDraw, &VMainGraphicsScene::ChoosedItem);
    connect(sceneDraw, &VMainGraphicsScene::NewFactor, spoint, &VToolSinglePoint::SetFactor);
    QHash<qint64, VDataTool*>* tools = doc->getTools();
    tools->insert(id, spoint);
    VDrawTool::AddRecord(id, Tool::SinglePointTool, doc);
    SetEnableTool(true);
    SetEnableWidgets(true);
    changeInFile = true;
}

void MainWindow::OptionDraw()
{
    QString nameDraw;
    qint32 index;
    QString nDraw = doc->GetNameActivDraw();
    QInputDialog *dlg = new QInputDialog(this);
    dlg->setInputMode( QInputDialog::TextInput );
    dlg->setLabelText(tr("Pattern piece:"));
    dlg->setTextEchoMode(QLineEdit::Normal);
    dlg->setWindowTitle(tr("Enter a new label for the pattern piece."));
    dlg->resize(300, 100);
    dlg->setTextValue(nDraw);
    while (1)
    {
        bool bOk = dlg->exec();
        nameDraw = dlg->textValue();
        if (bOk == false || nameDraw.isEmpty())
        {
            delete dlg;
            return;
        }
        index = comboBoxDraws->findText(nameDraw);
        if (index != -1)
        {//we already have this name
            qCritical()<<tr("Error. Pattern piece of same name already exists.");
        }
        else
        {
            break;
        }
    }
    delete dlg;
    index = comboBoxDraws->findText(doc->GetNameActivDraw());
    if (doc->SetNameDraw(nameDraw))
    {
        comboBoxDraws->setItemText(index, nameDraw);
    }
    else
    {
        QMessageBox::warning(this, tr("Error saving change!!!"), tr("Can't save new label of pattern piece"));
    }

}

template <typename Dialog, typename Func>
void MainWindow::SetToolButton(bool checked, Tool::Tools t, const QString &cursor, const QString &toolTip,
                               QSharedPointer<Dialog> &dialog, Func closeDialogSlot)
{
    if (checked)
    {
        CanselTool();
        tool = t;
        QPixmap pixmap(cursor);
        QCursor cur(pixmap, 2, 3);
        view->setCursor(cur);
        helpLabel->setText(toolTip);
        dialog = QSharedPointer<Dialog>(new Dialog(pattern));
        connect(currentScene, &VMainGraphicsScene::ChoosedObject, dialog.data(), &Dialog::ChoosedObject);
        connect(dialog.data(), &Dialog::DialogClosed, this, closeDialogSlot);
        connect(dialog.data(), &Dialog::ToolTip, this, &MainWindow::ShowToolTip);
        connect(doc, &VDomDocument::FullUpdateFromFile, dialog.data(), &Dialog::UpdateList);
    }
    else
    {
        if (QToolButton *tButton = qobject_cast< QToolButton * >(this->sender()))
        {
            Q_ASSERT(tButton != 0);
            tButton->setChecked(true);
        }
    }
}

template <typename T>
void MainWindow::AddToolToDetail(T *tool, const qint64 &id, Tool::Tools typeTool, const qint64 &idDetail)
{
    QHash<qint64, VDataTool*>* tools = doc->getTools();
    Q_ASSERT(tools != 0);
    VToolDetail *det = qobject_cast<VToolDetail*>(tools->value(idDetail));
    Q_ASSERT(det != 0);
    det->AddTool(tool, id, typeTool);
}

template <typename DrawTool,  typename Dialog>
void MainWindow::ClosedDialog(QSharedPointer<Dialog> &dialog, int result)
{
    if (result == QDialog::Accepted)
    {
        DrawTool::Create(dialog, currentScene, doc, pattern);
    }
    ArrowTool();
}

void MainWindow::ToolEndLine(bool checked)
{
    SetToolButton(checked, Tool::EndLineTool, ":/cursor/endline_cursor.png", tr("Select point"),
                  dialogEndLine, &MainWindow::ClosedDialogEndLine);
}

void MainWindow::ClosedDialogEndLine(int result)
{
    ClosedDialog<VToolEndLine>(dialogEndLine, result);
}

void MainWindow::ToolLine(bool checked)
{
    SetToolButton(checked, Tool::LineTool, ":/cursor/line_cursor.png", tr("Select first point"),
                  dialogLine, &MainWindow::ClosedDialogLine);
}

void MainWindow::ClosedDialogLine(int result)
{
    ClosedDialog<VToolLine>(dialogLine, result);
}

void MainWindow::ToolAlongLine(bool checked)
{
    SetToolButton(checked, Tool::AlongLineTool, ":/cursor/alongline_cursor.png", tr("Select point"),
                  dialogAlongLine, &MainWindow::ClosedDialogAlongLine);
}

void MainWindow::ClosedDialogAlongLine(int result)
{
    ClosedDialog<VToolAlongLine>(dialogAlongLine, result);
}

void MainWindow::ToolShoulderPoint(bool checked)
{
    SetToolButton(checked, Tool::ShoulderPointTool, ":/cursor/shoulder_cursor.png",
                  tr("Select first point of line"),  dialogShoulderPoint,
                  &MainWindow::ClosedDialogShoulderPoint);
}

void MainWindow::ClosedDialogShoulderPoint(int result)
{
    ClosedDialog<VToolShoulderPoint>(dialogShoulderPoint, result);
}

void MainWindow::ToolNormal(bool checked)
{
    SetToolButton(checked, Tool::NormalTool, ":/cursor/normal_cursor.png",
                  tr("Select first point of line"), dialogNormal, &MainWindow::ClosedDialogNormal);
}

void MainWindow::ClosedDialogNormal(int result)
{
    ClosedDialog<VToolNormal>(dialogNormal, result);
}

void MainWindow::ToolBisector(bool checked)
{
    SetToolButton(checked, Tool::BisectorTool, ":/cursor/bisector_cursor.png",
                  tr("Select first point of angle"), dialogBisector, &MainWindow::ClosedDialogBisector);
}

void MainWindow::ClosedDialogBisector(int result)
{
    ClosedDialog<VToolBisector>(dialogBisector, result);
}

void MainWindow::ToolLineIntersect(bool checked)
{
    SetToolButton(checked, Tool::LineIntersectTool, ":/cursor/intersect_cursor.png",
                  tr("Select first point of first line"), dialogLineIntersect,
                  &MainWindow::ClosedDialogLineIntersect);
}

void MainWindow::ClosedDialogLineIntersect(int result)
{
    ClosedDialog<VToolLineIntersect>(dialogLineIntersect, result);
}

void MainWindow::ToolSpline(bool checked)
{
    SetToolButton(checked, Tool::SplineTool, ":/cursor/spline_cursor.png",
                  tr("Select first point curve"), dialogSpline, &MainWindow::ClosedDialogSpline);
}

void MainWindow::ClosedDialogSpline(int result)
{
    ClosedDialog<VToolSpline>(dialogSpline, result);
}

void MainWindow::ToolCutSpline(bool checked)
{
    SetToolButton(checked, Tool::CutSplineTool, ":/cursor/spline_cut_point_cursor.png",
                  tr("Select simple curve"), dialogCutSpline, &MainWindow::ClosedDialogCutSpline);
}

void MainWindow::ClosedDialogCutSpline(int result)
{
    ClosedDialog<VToolCutSpline>(dialogCutSpline, result);
}

void MainWindow::ToolArc(bool checked)
{
    SetToolButton(checked, Tool::ArcTool, ":/cursor/arc_cursor.png",
                  tr("Select point of center of arc"), dialogArc, &MainWindow::ClosedDialogArc);
}

void MainWindow::ClosedDialogArc(int result)
{
    ClosedDialog<VToolArc>(dialogArc, result);
}

void MainWindow::ToolSplinePath(bool checked)
{
    SetToolButton(checked, Tool::SplinePathTool, ":/cursor/splinepath_cursor.png",
                  tr("Select point of curve path"), dialogSplinePath, &MainWindow::ClosedDialogSplinePath);
}

void MainWindow::ClosedDialogSplinePath(int result)
{
    ClosedDialog<VToolSplinePath>(dialogSplinePath, result);
}

void MainWindow::ToolCutSplinePath(bool checked)
{
    SetToolButton(checked, Tool::CutSplinePathTool, ":/cursor/splinepath_cut_point_cursor.png",
                  tr("Select curve path"), dialogCutSplinePath, &MainWindow::ClosedDialogCutSplinePath);
}

void MainWindow::ClosedDialogCutSplinePath(int result)
{
    ClosedDialog<VToolCutSplinePath>(dialogCutSplinePath, result);
}

void MainWindow::ToolPointOfContact(bool checked)
{
    SetToolButton(checked, Tool::PointOfContact, ":/cursor/pointcontact_cursor.png",
                  tr("Select first point of line"), dialogPointOfContact,
                  &MainWindow::ClosedDialogPointOfContact);
}

void MainWindow::ClosedDialogPointOfContact(int result)
{
    ClosedDialog<VToolPointOfContact>(dialogPointOfContact, result);
}

void MainWindow::ToolDetail(bool checked)
{
    if (checked)
    {
        CanselTool();
        tool = Tool::Detail;
        QPixmap pixmap("://cursor/new_detail_cursor.png");
        QCursor cur(pixmap, 2, 3);
        view->setCursor(cur);
        helpLabel->setText(tr("Select points, arcs, curves clockwise."));
        dialogDetail = QSharedPointer<DialogDetail>(new DialogDetail(pattern));
        connect(currentScene, &VMainGraphicsScene::ChoosedObject, dialogDetail.data(),
                &DialogDetail::ChoosedObject);
        connect(dialogDetail.data(), &DialogDetail::DialogClosed, this, &MainWindow::ClosedDialogDetail);
        connect(doc, &VDomDocument::FullUpdateFromFile, dialogDetail.data(), &DialogDetail::UpdateList);
    }
    else
    {
        if (QToolButton *tButton = qobject_cast< QToolButton * >(this->sender()))
        {
            tButton->setChecked(true);
        }
    }
}

void MainWindow::ClosedDialogDetail(int result)
{
    if (result == QDialog::Accepted)
    {
        VToolDetail::Create(dialogDetail, sceneDetails, doc, pattern);
    }
    ArrowTool();
    doc->FullUpdateTree();
}

void MainWindow::ToolHeight(bool checked)
{
    SetToolButton(checked, Tool::Height, ":/cursor/height_cursor.png", tr("Select base point"),
                  dialogHeight, &MainWindow::ClosedDialogHeight);
}

void MainWindow::ClosedDialogHeight(int result)
{
    ClosedDialog<VToolHeight>(dialogHeight, result);
}

void MainWindow::ToolTriangle(bool checked)
{
    SetToolButton(checked, Tool::Triangle, ":/cursor/triangle_cursor.png", tr("Select first point of axis"),
                  dialogTriangle, &MainWindow::ClosedDialogTriangle);
}

void MainWindow::ClosedDialogTriangle(int result)
{
    ClosedDialog<VToolTriangle>(dialogTriangle, result);
}

void MainWindow::ToolPointOfIntersection(bool checked)
{
    SetToolButton(checked, Tool::PointOfIntersection, ":/cursor/pointofintersect_cursor.png",
                  tr("Select point vertically"),
                  dialogPointOfIntersection, &MainWindow::ClosedDialogPointOfIntersection);
}

void MainWindow::ClosedDialogPointOfIntersection(int result)
{
    ClosedDialog<VToolPointOfIntersection>(dialogPointOfIntersection, result);
}

void MainWindow::ToolUnionDetails(bool checked)
{
    SetToolButton(checked, Tool::UnionDetails, ":/cursor/union_cursor.png",
                  tr("Select detail"), dialogUnionDetails, &MainWindow::ClosedDialogUnionDetails);
    //Must disconnect this signal here.
    disconnect(doc, &VDomDocument::FullUpdateFromFile, dialogUnionDetails.data(), &DialogUnionDetails::UpdateList);
}

void MainWindow::ToolCutArc(bool checked)
{
    SetToolButton(checked, Tool::CutArcTool, ":/cursor/arc_cut_cursor.png",
                  tr("Select arc"), dialogCutArc, &MainWindow::ClosedDialogCutArc);
}

void MainWindow::ClosedDialogCutArc(int result)
{
    ClosedDialog<VToolCutArc>(dialogCutArc, result);
}

void MainWindow::ClosedDialogUnionDetails(int result)
{
    ClosedDialog<VToolUnionDetails>(dialogUnionDetails, result);
    doc->FullUpdateTree();
}

void MainWindow::About()
{
    QString fullName = QString("Valentina %1").arg(APP_VERSION);
    QString qtBase(tr("Based on Qt %2 (32 bit)").arg(QT_VERSION_STR));
    QString buildOn(tr("Built on %3 at %4").arg(__DATE__).arg(__TIME__));
    QString about = QString(tr("<h1>%1</h1> %2 <br/><br/> %3 <br/><br/> %4")).arg(fullName).arg(qtBase).arg(
                buildOn).arg(WARRANTY);
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
    if (changeInFile == true)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The pattern has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setIcon(QMessageBox::Question);
        int ret = msgBox.exec();
        switch (ret)
        {
            case QMessageBox::Save:
                // Save was clicked
                if (fileName.isEmpty())
                {
                    ActionSaveAs();
                }
                else
                {
                    ActionSave();
                }
                if (changeInFile)
                {
                    // We did't save file
                    event->ignore();
                }
                else
                {
                    // We have successfully saved the file
                    event->accept();
                }
                break;
            case QMessageBox::Discard:
                // Don't Save was clicked
                event->accept();
                break;
            case QMessageBox::Cancel:
                // Cancel was clicked
                event->ignore();
                break;
            default:
                // should never be reached
                event->accept();
                break;
        }
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
            this, &MainWindow::ChangedGrowth);

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
        if(drawMode)
        {
            ArrowTool();
            qint64 id = doc->SPointActiveDraw();
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
                            .arg(static_cast<qint32>(toMM(scenePos.x())))
                            .arg(static_cast<qint32>(toMM(scenePos.y())));
    mouseCoordinate->setText(string);
}

void MainWindow::CanselTool()
{
    switch ( tool )
    {
        case Tool::ArrowTool:
            ui->actionArrowTool->setChecked(false);
            helpLabel->setText("");
            break;
        case Tool::SinglePointTool:
            Q_UNREACHABLE();
            //Nothing to do here because we can't create this tool from main window.
            break;
        case Tool::EndLineTool:
            dialogEndLine.clear();
            ui->toolButtonEndLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::LineTool:
            dialogLine.clear();
            ui->toolButtonLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearFocus();
            break;
        case Tool::AlongLineTool:
            dialogAlongLine.clear();
            ui->toolButtonAlongLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::ShoulderPointTool:
            dialogShoulderPoint.clear();
            ui->toolButtonShoulderPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::NormalTool:
            dialogNormal.clear();
            ui->toolButtonNormal->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::BisectorTool:
            dialogBisector.clear();
            ui->toolButtonBisector->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::LineIntersectTool:
            dialogLineIntersect.clear();
            ui->toolButtonLineIntersect->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::SplineTool:
            dialogSpline.clear();
            ui->toolButtonSpline->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::ArcTool:
            dialogArc.clear();
            ui->toolButtonArc->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::SplinePathTool:
            dialogSplinePath.clear();
            ui->toolButtonSplinePath->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::PointOfContact:
            dialogPointOfContact.clear();
            ui->toolButtonPointOfContact->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::Detail:
            dialogDetail.clear();
            ui->toolButtonNewDetail->setChecked(false);
            break;
        case Tool::Height:
            dialogHeight.clear();
            ui->toolButtonHeight->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::Triangle:
            dialogTriangle.clear();
            ui->toolButtonTriangle->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::PointOfIntersection:
            dialogPointOfIntersection.clear();
            ui->toolButtonPointOfIntersection->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::CutSplineTool:
            dialogCutSpline.clear();
            ui->toolButtonSplineCutPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::CutSplinePathTool:
            dialogCutSplinePath.clear();
            ui->toolButtonSplinePathCutPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::UnionDetails:
            dialogUnionDetails.clear();
            ui->toolButtonUnionDetails->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::CutArcTool:
            dialogCutArc.clear();
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
    CanselTool();
    ui->actionArrowTool->setChecked(true);
    tool = Tool::ArrowTool;
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

        mode = Draw::Calculation;
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


        mode = Draw::Modeling;
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

void MainWindow::ActionSaveAs()
{
    QString filters(tr("Pattern files (*.val)"));
    QString fName = QFileDialog::getSaveFileName(this, tr("Save as"), QDir::homePath(), filters);

    if (fName.isEmpty())
    {
        return;
    }
    QFileInfo f( fName );
    if (f.suffix().isEmpty() && f.suffix() != "val")
    {
        fName += ".val";
    }
    fileName = fName;

    ActionSave();
}

void MainWindow::ActionSave()
{
    if (fileName.isEmpty() == false)
    {
        bool result = SafeSaveing(fileName);
        if (result)
        {
            ui->actionSave->setEnabled(false);
            changeInFile = false;
            QFileInfo info(fileName);
            QString title(info.fileName());
            title.append("-Valentina");
            setWindowTitle(title);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Error!"));
            msgBox.setText(tr("Error saving file. Can't save file."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }
}

void MainWindow::ActionOpen()
{
    QString filter(tr("Pattern files (*.val)"));
    QString fName = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), filter);
    if (fName.isEmpty())
    {
        return;
    }
    if (fileName.isEmpty() && changeInFile == false)
    {
        OpenPattern(fName);

        VAbstractTool::NewSceneRect(sceneDraw, view);
        VAbstractTool::NewSceneRect(sceneDetails, view);
    }
    else
    {
        /*Open new copy application*/
        QProcess *v = new QProcess(this);
        QStringList arguments;
        arguments << "-o" << fName;
        v->startDetached(QCoreApplication::applicationFilePath (), arguments);
        delete v;
    }
}

void MainWindow::Clear()
{
    setWindowTitle("Valentina");
    fileName.clear();
    pattern->Clear();
    doc->clear();
    sceneDraw->clear();
    sceneDetails->clear();
    CanselTool();
    comboBoxDraws->clear();
    fileName.clear();
    ui->actionOptionDraw->setEnabled(false);
    ui->actionSave->setEnabled(false);
    SetEnableTool(false);
}

void MainWindow::ActionNew()
{
    QProcess *v = new QProcess(this);
    v->startDetached(QCoreApplication::applicationFilePath ());
    delete v;
}

void MainWindow::haveChange()
{
    if (fileName.isEmpty() == false)
    {
        ui->actionSave->setEnabled(true);
        changeInFile = true;
        QFileInfo info(fileName);
        QString title(info.fileName());
        title.append("*-Valentina");
        setWindowTitle(title);
    }
}

void MainWindow::ChangedSize(const QString & text)
{
    qint32 size = text.toInt();
    pattern->SetSize(size*10);
    doc->FullUpdateTree();
}

void MainWindow::ChangedGrowth(const QString &text)
{
    qint32 growth = text.toInt();
    pattern->SetGrowth(growth*10);
    doc->FullUpdateTree();
}

void MainWindow::SetEnableWidgets(bool enable)
{
    ui->actionSaveAs->setEnabled(enable);
    ui->actionDraw->setEnabled(enable);
    ui->actionDetails->setEnabled(enable);
    ui->actionOptionDraw->setEnabled(enable);
    if (enable == true && fileName.isEmpty() == false)
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
        connect(dialogTable, &DialogIncrements::DialogClosed, this,
                &MainWindow::ClosedActionTable);
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
}

void MainWindow::ActionHistory(bool checked)
{
    if (checked)
    {
        dialogHistory = new DialogHistory(pattern, doc, this);
        dialogHistory->setWindowFlags(Qt::Window);
        connect(dialogHistory, &DialogHistory::DialogClosed, this,
                &MainWindow::ClosedActionHistory);
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
    pattern->PrepareDetails(listDetails);
    emit ModelChosen(listDetails);
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
    if (mode == Draw::Calculation)
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

bool MainWindow::SafeSaveing(const QString &fileName) const
{
    try
    {
        doc->TestUniqueId();
    }
    catch (const VExceptionUniqueId &e)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error!"));
        msgBox.setText(tr("Error no unique id."));
        msgBox.setInformativeText(e.ErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(e.DetailedInformation());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
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
    qDebug()<<"file "<<fileName<<"temp file "<<temp;
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
            qCritical()<<tr("Could not copy temp file to pattern file")<<Q_FUNC_INFO;
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
        qCritical()<<tr("Could not remove pattern file")<<Q_FUNC_INFO;
        result = false;
    }
    if (result)
    {
        tempFile.remove();
    }
    return result;
}

void MainWindow::AutoSavePattern()
{
    if (fileName.isEmpty() == false && changeInFile == true)
    {
        bool result = SafeSaveing(fileName);
        if (result)
        {
            ui->actionSave->setEnabled(false);
            changeInFile = false;
            QFileInfo info(fileName);
            QString title(info.fileName());
            title.append("-Valentina");
            setWindowTitle(title);
        }
    }
}

MainWindow::~MainWindow()
{
    CanselTool();
    delete ui;

    delete pattern;
    delete doc;
    delete sceneDetails;
    delete sceneDraw;
}

void MainWindow::OpenPattern(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        qWarning()<<tr("Can't open pattern file. File name is empty")<<Q_FUNC_INFO;
        return;
    }
    QFile file(fileName);
    QString errorMsg;
    qint32 errorLine = 0;
    qint32 errorColumn = 0;
    if (file.open(QIODevice::ReadOnly))
    {
        if (doc->setContent(&file, &errorMsg, &errorLine, &errorColumn))
        {
            disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                       this, &MainWindow::currentDrawChanged);
            try
            {
                doc->Parse(Document::FullParse, sceneDraw, sceneDetails);
            }
            catch (const VExceptionObjectError &e)
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Error!"));
                msgBox.setText(tr("Error parsing file."));
                msgBox.setInformativeText(e.ErrorMessage());
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setDetailedText(e.DetailedInformation());
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
                file.close();
                Clear();
                return;
            }
            catch (const VExceptionConversionError &e)
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Error!"));
                msgBox.setText(tr("Error can't convert value."));
                msgBox.setInformativeText(e.ErrorMessage());
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
                file.close();
                Clear();
                return;
            }
            catch (const VExceptionEmptyParameter &e)
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Error!"));
                msgBox.setText(tr("Error empty parameter."));
                msgBox.setInformativeText(e.ErrorMessage());
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setDetailedText(e.DetailedInformation());
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
                file.close();
                Clear();
                return;
            }
            catch (const VExceptionWrongParameterId &e)
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Error!"));
                msgBox.setText(tr("Error wrong id."));
                msgBox.setInformativeText(e.ErrorMessage());
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setDetailedText(e.DetailedInformation());
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
                file.close();
                Clear();
                return;
            }
            catch (const VExceptionUniqueId &e)
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Error!"));
                msgBox.setText(tr("Error no unique id."));
                msgBox.setInformativeText(e.ErrorMessage());
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setDetailedText(e.DetailedInformation());
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
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
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Error!"));
            msgBox.setText(tr("Error parsing pattern file."));
            msgBox.setInformativeText(errorMsg);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            QString error = QString(tr("Error in line %1 column %2")).arg(errorLine, errorColumn);
            msgBox.setDetailedText(error);
            msgBox.exec();
            file.close();
            Clear();
            return;
        }
        file.close();
    }
    this->fileName = fileName;
    QFileInfo info(fileName);
    QString title(info.fileName());
    title.append("-Valentina");
    setWindowTitle(title);
    helpLabel->setText("");
}
