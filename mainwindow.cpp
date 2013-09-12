#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QSpinBox>
#include <QScrollBar>
#include <QShowEvent>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>
#include <QDebug>
#include "geometry/vspline.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), tool(Tools::ArrowTool), currentScene(0), sceneDraw(0),
    sceneDetails(0), mouseCoordinate(0), helpLabel(0), view(0), isInitialized(false), dialogTable(0),
    dialogEndLine(QSharedPointer<DialogEndLine>()), dialogLine(QSharedPointer<DialogLine>()),
    dialogAlongLine(QSharedPointer<DialogAlongLine>()),
    dialogShoulderPoint(QSharedPointer<DialogShoulderPoint>()), dialogNormal(QSharedPointer<DialogNormal>()),
    dialogBisector(QSharedPointer<DialogBisector>()),
    dialogLineIntersect(QSharedPointer<DialogLineIntersect>()),
    dialogSpline(QSharedPointer<DialogSpline>()),
    dialogArc(QSharedPointer<DialogArc>()), dialogSplinePath(QSharedPointer<DialogSplinePath>()),
    dialogPointOfContact(QSharedPointer<DialogPointOfContact>()), dialogDetail(QSharedPointer<DialogDetail>()),
    dialogHistory(0), doc(0), data(0), comboBoxDraws(0), fileName(QString()), changeInFile(false),
    mode(Draw::Calculation){
    ui->setupUi(this);
    ToolBarOption();
    ToolBarDraws();
    QRectF sceneRect = QRectF(0, 0, PaperSize, PaperSize);
    sceneDraw = new VMainGraphicsScene(sceneRect);
    currentScene = sceneDraw;
    connect(sceneDraw, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    sceneDetails = new VMainGraphicsScene(sceneRect);
    connect(sceneDetails, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    view = new VMainGraphicsView();
    ui->LayoutView->addWidget(view);
    view->setScene(currentScene);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(12);
    view->setSizePolicy(policy);
    helpLabel = new QLabel("Створіть новий файл для початку роботи.");
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

    data = new VContainer;

    doc = new VDomDocument(data, comboBoxDraws, &mode);
    doc->CreateEmptyFile();
    connect(doc, &VDomDocument::haveChange, this, &MainWindow::haveChange);

    fileName.clear();
    changeInFile = false;

}

void MainWindow::ActionNewDraw(){
    QString nameDraw;
    bool bOk;
    qint32 index;
    QString nDraw = QString("Креслення %1").arg(comboBoxDraws->count()+1);
    QInputDialog *dlg = new QInputDialog(this);
    dlg->setInputMode( QInputDialog::TextInput );
    dlg->setLabelText("Креслення:");
    dlg->setTextEchoMode(QLineEdit::Normal);
    dlg->setWindowTitle("Введіть назву креслення.");
    dlg->resize(300,100);
    dlg->setTextValue(nDraw);
    while(1){
        bOk = dlg->exec();
        nameDraw = dlg->textValue();
        if(!bOk || nameDraw.isEmpty()){
            delete dlg;
            return;
        }
        index = comboBoxDraws->findText(nameDraw);
        if(index != -1){//we already have this name
            qCritical()<<"Помилка. Креслення з таким ім'ям вже існує.";
        } else {
            break;
        }
    }
    delete dlg;
    bOk = doc->appendDraw(nameDraw);
    if(bOk == false){
        qCritical()<<"Помилка створення креслення з ім'ям"<<nameDraw<<".";
        return;//не змогли додати креслення.
    }
    disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               this, &MainWindow::currentDrawChanged);
    comboBoxDraws->addItem(nameDraw);
    index = comboBoxDraws->findText(nameDraw);
    if ( index != -1 ) { // -1 for not found
        comboBoxDraws->setCurrentIndex(index);
        currentDrawChanged( index );
    }
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::currentDrawChanged);
    data->ClearObject();
    //Create single point
    qint64 id = data->AddPoint(VPointF((10+comboBoxDraws->count()*5)*PrintDPI/25.4, 10*PrintDPI/25.4, "А", 5,
                                       10));
    VToolSinglePoint *spoint = new VToolSinglePoint(doc, data, id, Tool::FromGui);
    sceneDraw->addItem(spoint);
    connect(spoint, &VToolPoint::ChoosedTool, sceneDraw, &VMainGraphicsScene::ChoosedItem);
    QMap<qint64, VDataTool*>* tools = doc->getTools();
    tools->insert(id, spoint);
    VDrawTool::AddRecord(id, Tools::SinglePointTool, doc);
    SetEnableTool(true);
    SetEnableWidgets(true);
}

void MainWindow::OptionDraw(){
    QString nameDraw;
    bool bOk;
    qint32 index;
    QString nDraw = doc->GetNameActivDraw();
    QInputDialog *dlg = new QInputDialog(this);
    dlg->setInputMode( QInputDialog::TextInput );
    dlg->setLabelText("Креслення:");
    dlg->setTextEchoMode(QLineEdit::Normal);
    dlg->setWindowTitle("Введіть нову назву креслення.");
    dlg->resize(300,100);
    dlg->setTextValue(nDraw);
    while(1){
        bOk = dlg->exec();
        nameDraw = dlg->textValue();
        if(!bOk || nameDraw.isEmpty()){
            delete dlg;
            return;
        }
        index = comboBoxDraws->findText(nameDraw);
        if(index != -1){//we already have this name
            qCritical()<<"Помилка. Креслення з таким ім'ям вже існує.";
        } else {
            break;
        }
    }
    delete dlg;
    index = comboBoxDraws->findText(doc->GetNameActivDraw());
    doc->SetNameDraw(nameDraw);
    comboBoxDraws->setItemText(index, nameDraw);
}

template <typename Dialog, typename Func>
void MainWindow::SetToolButton(bool checked, Tools::Enum t, const QString &cursor,
                               QSharedPointer<Dialog> &dialog, Func closeDialogSlot){
    if(checked){
        CanselTool();
        tool = t;
        QPixmap pixmap(cursor);
        QCursor cur(pixmap, 2, 3);
        view->setCursor(cur);
        helpLabel->setText("Виберіть точки.");
        dialog = QSharedPointer<Dialog>(new Dialog(data, mode));
        connect(currentScene, &VMainGraphicsScene::ChoosedObject, dialog.data(), &Dialog::ChoosedObject);
        connect(dialog.data(), &Dialog::DialogClosed, this, closeDialogSlot);
        connect(doc, &VDomDocument::FullUpdateFromFile, dialog.data(), &Dialog::UpdateList);
    } else {
        if(QToolButton *tButton = qobject_cast< QToolButton * >(this->sender())){
            tButton->setChecked(true);
        }
    }
}

template <typename Tool>
void MainWindow::AddToolToDetail(Tool *tool, const qint64 &id, Tools::Enum typeTool,
                                 const qint64 &idDetail){
    QMap<qint64, VDataTool*>* tools = doc->getTools();
    VToolDetail *det = qobject_cast<VToolDetail*>(tools->value(idDetail));
    Q_CHECK_PTR(det);
    det->AddTool(tool, id, typeTool);
}

void MainWindow::ToolEndLine(bool checked){
    SetToolButton(checked, Tools::EndLineTool, ":/cursor/endline_cursor.png", dialogEndLine,
                  &MainWindow::ClosedDialogEndLine);
}

void MainWindow::ClosedDialogEndLine(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolEndLine::Create(dialogEndLine, currentScene, doc, data);
        } else {
            VModelingEndLine *endLine = VModelingEndLine::Create(dialogEndLine, doc, data);
            AddToolToDetail(endLine, endLine->getId(), Tools::EndLineTool, dialogEndLine->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolLine(bool checked){
    SetToolButton(checked, Tools::LineTool, ":/cursor/line_cursor.png", dialogLine,
                  &MainWindow::ClosedDialogLine);
}

void MainWindow::ClosedDialogLine(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolLine::Create(dialogLine, currentScene, doc, data);
        } else {
            VModelingLine *line = VModelingLine::Create(dialogLine, doc, data);
            AddToolToDetail(line, line->getId(), Tools::LineTool, dialogLine->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolAlongLine(bool checked){
    SetToolButton(checked, Tools::AlongLineTool, ":/cursor/alongline_cursor.png", dialogAlongLine,
                  &MainWindow::ClosedDialogAlongLine);
}

void MainWindow::ClosedDialogAlongLine(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolAlongLine::Create(dialogAlongLine, currentScene, doc, data);
        } else{
            VModelingAlongLine *point = VModelingAlongLine::Create(dialogAlongLine, doc, data);
            AddToolToDetail(point, point->getId(), Tools::AlongLineTool, dialogAlongLine->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolShoulderPoint(bool checked){
    SetToolButton(checked, Tools::ShoulderPointTool, ":/cursor/shoulder_cursor.png", dialogShoulderPoint,
                  &MainWindow::ClosedDialogShoulderPoint);
}

void MainWindow::ClosedDialogShoulderPoint(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolShoulderPoint::Create(dialogShoulderPoint, currentScene, doc, data);
        } else {
            VModelingShoulderPoint *point = VModelingShoulderPoint::Create(dialogShoulderPoint, doc, data);
            AddToolToDetail(point, point->getId(), Tools::ShoulderPointTool,
                            dialogShoulderPoint->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolNormal(bool checked){
    SetToolButton(checked, Tools::NormalTool, ":/cursor/normal_cursor.png", dialogNormal,
                  &MainWindow::ClosedDialogNormal);
}

void MainWindow::ClosedDialogNormal(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolNormal::Create(dialogNormal, currentScene, doc, data);
        } else {
            VModelingNormal *point = VModelingNormal::Create(dialogNormal, doc, data);
            AddToolToDetail(point, point->getId(), Tools::NormalTool, dialogNormal->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolBisector(bool checked){
    SetToolButton(checked, Tools::BisectorTool, ":/cursor/bisector_cursor.png", dialogBisector,
                  &MainWindow::ClosedDialogBisector);
}

void MainWindow::ClosedDialogBisector(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolBisector::Create(dialogBisector, currentScene, doc, data);
        } else {
            VModelingBisector *point = VModelingBisector::Create(dialogBisector, doc, data);
            AddToolToDetail(point, point->getId(), Tools::BisectorTool, dialogBisector->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolLineIntersect(bool checked){
    SetToolButton(checked, Tools::LineIntersectTool, ":/cursor/intersect_cursor.png", dialogLineIntersect,
                  &MainWindow::ClosedDialogLineIntersect);
}

void MainWindow::ClosedDialogLineIntersect(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolLineIntersect::Create(dialogLineIntersect, currentScene, doc, data);
        } else {
            VModelingLineIntersect *point = VModelingLineIntersect::Create(dialogLineIntersect, doc, data);
            AddToolToDetail(point, point->getId(), Tools::LineIntersectTool,
                            dialogLineIntersect->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolSpline(bool checked){
    SetToolButton(checked, Tools::SplineTool, ":/cursor/spline_cursor.png", dialogSpline,
                  &MainWindow::ClosedDialogSpline);
}

void MainWindow::ClosedDialogSpline(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolSpline::Create(dialogSpline, currentScene, doc, data);
        } else {
            VModelingSpline *spl = VModelingSpline::Create(dialogSpline, doc, data);
            AddToolToDetail(spl, spl->getId(), Tools::SplineTool, dialogSpline->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolArc(bool checked){
    SetToolButton(checked, Tools::ArcTool, ":/cursor/arc_cursor.png", dialogArc,
                  &MainWindow::ClosedDialogArc);
}

void MainWindow::ClosedDialogArc(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolArc::Create(dialogArc, currentScene, doc, data);
        } else {
            VModelingArc *arc = VModelingArc::Create(dialogArc, doc, data);
            AddToolToDetail(arc, arc->getId(), Tools::ArcTool, dialogArc->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolSplinePath(bool checked){
    SetToolButton(checked, Tools::SplinePathTool, ":/cursor/splinepath_cursor.png", dialogSplinePath,
                  &MainWindow::ClosedDialogSplinePath);
}

void MainWindow::ClosedDialogSplinePath(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolSplinePath::Create(dialogSplinePath, currentScene, doc, data);
        } else {
            VModelingSplinePath *spl = VModelingSplinePath::Create(dialogSplinePath, doc, data);
            AddToolToDetail(spl, spl->getId(), Tools::SplinePathTool, dialogSplinePath->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolPointOfContact(bool checked){
    SetToolButton(checked, Tools::PointOfContact, ":/cursor/pointcontact_cursor.png", dialogPointOfContact,
                  &MainWindow::ClosedDialogPointOfContact);
}

void MainWindow::ClosedDialogPointOfContact(int result){
    if(result == QDialog::Accepted){
        if(mode == Draw::Calculation){
            VToolPointOfContact::Create(dialogPointOfContact, currentScene, doc, data);
        } else {
            VModelingPointOfContact *point = VModelingPointOfContact::Create(dialogPointOfContact, doc,
                                                                             data);
            AddToolToDetail(point, point->getId(), Tools::PointOfContact,
                            dialogPointOfContact->getIdDetail());
        }
    }
    ArrowTool();
}

void MainWindow::ToolDetail(bool checked){
    if(checked){
        CanselTool();
        tool = Tools::Detail;
        QPixmap pixmap("://cursor/new_detail_cursor.png");
        QCursor cur(pixmap, 2, 3);
        view->setCursor(cur);
        helpLabel->setText("Виберіть точки.");
        dialogDetail = QSharedPointer<DialogDetail>(new DialogDetail(data, mode));
        connect(currentScene, &VMainGraphicsScene::ChoosedObject, dialogDetail.data(),
                &DialogDetail::ChoosedObject);
        connect(dialogDetail.data(), &DialogDetail::DialogClosed, this, &MainWindow::ClosedDialogDetail);
        connect(doc, &VDomDocument::FullUpdateFromFile, dialogDetail.data(), &DialogDetail::UpdateList);
    } else {
        if(QToolButton *tButton = qobject_cast< QToolButton * >(this->sender())){
            tButton->setChecked(true);
        }
    }
}

void MainWindow::ClosedDialogDetail(int result){
    if(result == QDialog::Accepted){
        VToolDetail::Create(dialogDetail, sceneDetails, doc, data);
    }
    ArrowTool();
}

void MainWindow::tableClosed(){
    show();
    MinimumScrollBar();
}

void MainWindow::showEvent( QShowEvent *event ){
    QMainWindow::showEvent( event );
    if( event->spontaneous() ){
        return;
    }

    if(isInitialized){
        return;
    }
    // do your init stuff here
    MinimumScrollBar();

    isInitialized = true;//перший показ вікна вже відбувся
}

void MainWindow::ToolBarOption(){
    QLabel * labelGrowth = new QLabel;
    labelGrowth->setText("Зріст: ");
    ui->toolBarOption->addWidget(labelGrowth);

    QStringList list;
    list << "104"<<"110"<<"116"<<"122"<<"128"<<"134"<<"140"<<"146"<<"152"<<"158"<<"164"<<"170"<<"176"
         << "182" << "188";
    QComboBox *comboBoxGrow = new QComboBox;
    comboBoxGrow->clear();
    comboBoxGrow->addItems(list);
    comboBoxGrow->setCurrentIndex(12);
    ui->toolBarOption->addWidget(comboBoxGrow);
    connect(comboBoxGrow,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::ChangedGrowth);

    QLabel * labelSize = new QLabel;
    labelSize->setText(" Розмір: ");
    ui->toolBarOption->addWidget(labelSize);

    list.clear();
    list << "28"<<"30"<<"32"<<"34"<<"36"<<"38"<<"40"<<"42"<<"44"<<"46"<<"48"<<"50" << "52" << "54" << "56";
    QComboBox *comboBoxSize = new QComboBox;
    comboBoxSize->clear();
    comboBoxSize->addItems(list);
    comboBoxSize->setCurrentIndex(11);
    ui->toolBarOption->addWidget(comboBoxSize);
    connect(comboBoxSize,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::ChangedSize);

    ui->toolBarOption->addSeparator();

    mouseCoordinate = new QLabel;
    mouseCoordinate ->setText("0, 0");
    ui->toolBarOption->addWidget(mouseCoordinate);

}

void MainWindow::ToolBarDraws(){
    QLabel * labelNameDraw = new QLabel;
    labelNameDraw ->setText("Креслення: ");
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
}

void MainWindow::currentDrawChanged( int index ){
    if(index != -1) {
        doc->setCurrentData();
        doc->ChangeActivDraw(comboBoxDraws->itemText(index));
    }
}

void MainWindow::mouseMove(QPointF scenePos){
    QString string = QString("%1, %2")
                            .arg(static_cast<qint32>(scenePos.x()/PrintDPI*25.4))
                            .arg(static_cast<qint32>(scenePos.y()/PrintDPI*25.4));
    mouseCoordinate->setText(string);
}

void MainWindow::CanselTool(){
    switch( tool )
    {
        case Tools::ArrowTool:
            ui->actionArrowTool->setChecked(false);
            break;
        case Tools::SinglePointTool:
            //Nothing to do here because we can't create this tool from main window.
            break;
        case Tools::EndLineTool:
            dialogEndLine.clear();
            ui->toolButtonEndLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::LineTool:
            dialogLine.clear();
            ui->toolButtonLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearFocus();
            break;
        case Tools::AlongLineTool:
            dialogAlongLine.clear();
            ui->toolButtonAlongLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::ShoulderPointTool:
            dialogShoulderPoint.clear();
            ui->toolButtonShoulderPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::NormalTool:
            dialogNormal.clear();
            ui->toolButtonNormal->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::BisectorTool:
            dialogBisector.clear();
            ui->toolButtonBisector->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::LineIntersectTool:
            dialogLineIntersect.clear();
            ui->toolButtonLineIntersect->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::SplineTool:
            dialogSpline.clear();
            ui->toolButtonSpline->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::ArcTool:
            dialogArc.clear();
            ui->toolButtonArc->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::SplinePathTool:
            dialogSplinePath.clear();
            ui->toolButtonSplinePath->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::PointOfContact:
            dialogPointOfContact.clear();
            ui->toolButtonPointOfContact->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tools::Detail:
            dialogDetail.clear();
            ui->toolButtonNewDetail->setChecked(false);
            break;
    }
}

void  MainWindow::ArrowTool(){
    CanselTool();
    ui->actionArrowTool->setChecked(true);
    tool = Tools::ArrowTool;
    QCursor cur(Qt::ArrowCursor);
    view->setCursor(cur);
    helpLabel->setText("");
}

void MainWindow::ActionAroowTool(){
    ArrowTool();
}

void MainWindow::keyPressEvent ( QKeyEvent * event ){
    switch(event->key()){
        case Qt::Key_Escape:
            ArrowTool();
            break;
    }
    QMainWindow::keyPressEvent ( event );
}

void MainWindow::ActionDraw(bool checked){
    if(checked){
        ui->actionDetails->setChecked(false);
        currentScene = sceneDraw;
        view->setScene(currentScene);
        mode = Draw::Calculation;
        doc->setCurrentData();
    } else {
        ui->actionDraw->setChecked(true);
    }
}

void MainWindow::ActionDetails(bool checked){
    if(checked){
        ui->actionDraw->setChecked(false);
        currentScene = sceneDetails;
        view->setScene(sceneDetails);
        mode = Draw::Modeling;
    } else {
        ui->actionDetails->setChecked(true);
    }
}

void MainWindow::ActionSaveAs(){
    QString filters("Lekalo files (*.xml);;All files (*.*)");
    QString defaultFilter("Lekalo files (*.xml)");
    QString fName = QFileDialog::getSaveFileName(this, "Зберегти файл як", QDir::homePath(),
                                                    filters, &defaultFilter);
    if(fName.isEmpty())
        return;
    if(fName.indexOf(".xml",fName.size()-4)<0){
        fName.append(".xml");
    }
    fileName = fName;
    QFileInfo info(fileName);
    QString title(info.fileName());
    title.append("-Valentina");
    setWindowTitle(title);

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly| QIODevice::Truncate)){
        doc->GarbageCollector();
        const int Indent = 4;
        QTextStream out(&file);
        doc->save(out, Indent);
        file.close();
    }
    ui->actionSave->setEnabled(false);
    changeInFile = false;
}

void MainWindow::ActionSave(){
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly| QIODevice::Truncate)){
            doc->GarbageCollector();
            const int Indent = 4;
            QTextStream out(&file);
            doc->save(out, Indent);
            file.close();
        }
        ui->actionSave->setEnabled(false);
        changeInFile = false;
    }
}

void MainWindow::ActionOpen(){
    QString filter("Lekalo files (*.xml)");
    QString fName = QFileDialog::getOpenFileName(this, tr("Відкрити файл"), QDir::homePath(), filter);
    if(fName.isEmpty())
        return;
    fileName = fName;
    QFileInfo info(fileName);
    QString title(info.fileName());
    title.append("-Valentina");
    setWindowTitle(title);
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)){
        if(doc->setContent(&file)){
            disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                    this, &MainWindow::currentDrawChanged);
            doc->Parse(Document::FullParse, sceneDraw, sceneDetails);
            connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                    this, &MainWindow::currentDrawChanged);
            QString nameDraw = doc->GetNameActivDraw();
            qint32 index = comboBoxDraws->findText(nameDraw);
            if ( index != -1 ) { // -1 for not found
                comboBoxDraws->setCurrentIndex(index);
            }
            if(comboBoxDraws->count() > 0){
                SetEnableTool(true);
            } else {
                SetEnableTool(false);
            }
            SetEnableWidgets(true);
        }
        file.close();
    }
}

void MainWindow::ActionNew(){
    setWindowTitle("Valentina");
    fileName.clear();
    data->Clear();
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

void MainWindow::haveChange(){
    if(!fileName.isEmpty()){
        ui->actionSave->setEnabled(true);
    }
}

void MainWindow::ChangedSize(const QString & text){
    qint32 size = text.toInt();
    data->SetSize(size*10);
    doc->FullUpdateTree();
}

void MainWindow::ChangedGrowth(const QString &text){
    qint32 growth = text.toInt();
    data->SetGrowth(growth*10);
    doc->FullUpdateTree();
}

void MainWindow::SetEnableWidgets(bool enable){
    ui->actionSaveAs->setEnabled(enable);
    ui->actionDraw->setEnabled(enable);
    ui->actionDetails->setEnabled(enable);
    ui->actionOptionDraw->setEnabled(enable);
    if(enable == true && !fileName.isEmpty()){
        ui->actionSave->setEnabled(enable);
    }
    ui->actionTable->setEnabled(enable);
    ui->actionHistory->setEnabled(enable);
}

void MainWindow::ActionTable(bool checked){
    if(checked){
        dialogTable = new DialogIncrements(data, doc, this);
        connect(dialogTable, &DialogIncrements::DialogClosed, this,
                &MainWindow::ClosedActionTable);
        dialogTable->show();
    } else {
        ui->actionTable->setChecked(true);
        dialogTable->activateWindow();
    }
}

void MainWindow::ClosedActionTable(){
    ui->actionTable->setChecked(false);
    delete dialogTable;
}

void MainWindow::ActionHistory(bool checked){
    if(checked){
        dialogHistory = new DialogHistory(data, doc, this);
        dialogHistory->setWindowFlags(Qt::Window);
        connect(dialogHistory, &DialogHistory::DialogClosed, this,
                &MainWindow::ClosedActionHistory);
        dialogHistory->show();
    } else {
        ui->actionHistory->setChecked(true);
        dialogHistory->activateWindow();
    }
}

void MainWindow::ActionLayout(bool checked){
    Q_UNUSED(checked);
    hide();
    QVector<VItem*> listDetails;
    data->PrepareDetails(listDetails);
    emit ModelChosen(listDetails);
}

void MainWindow::ClosedActionHistory(){
    ui->actionHistory->setChecked(false);
    delete dialogHistory;
}

void MainWindow::SetEnableTool(bool enable){
    ui->toolButtonEndLine->setEnabled(enable);
    ui->toolButtonLine->setEnabled(enable);
    ui->toolButtonAlongLine->setEnabled(enable);
    ui->toolButtonShoulderPoint->setEnabled(enable);
    ui->toolButtonNormal->setEnabled(enable);
    ui->toolButtonBisector->setEnabled(enable);
    ui->toolButtonLineIntersect->setEnabled(enable);
    ui->toolButtonSpline->setEnabled(enable);
    ui->toolButtonArc->setEnabled(enable);
    ui->toolButtonSplinePath->setEnabled(enable);
    ui->toolButtonPointOfContact->setEnabled(enable);
    ui->toolButtonNewDetail->setEnabled(enable);
}

void MainWindow::MinimumScrollBar(){
    QScrollBar *horScrollBar = view->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar *verScrollBar = view->verticalScrollBar();
    verScrollBar->setValue(verScrollBar->minimum());
}

MainWindow::~MainWindow(){
    CanselTool();
    delete ui;

    QFile file("lekalo.xml");
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        const int Indent = 4;
        QTextStream out(&file);
        doc->save(out, Indent);
        file.close();
    }

    delete data;
    if(!doc->isNull()){
        delete doc;
    } 
}
