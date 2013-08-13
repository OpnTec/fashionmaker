#pragma GCC diagnostic ignored "-Weffc++"
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

#include "tools/vtoolendline.h"
#include "tools/vtoolline.h"
#include "tools/vtoolalongline.h"
#include "tools/vtoolshoulderpoint.h"
#include "tools/vtoolnormal.h"
#include "tools/vtoolbisector.h"
#include "tools/vtoollineintersect.h"
#include "tools/vtoolspline.h"
#include "tools/vtoolarc.h"
#include "tools/vtoolsplinepath.h"
#pragma GCC diagnostic warning "-Weffc++"
#include "options.h"
#include "geometry/vspline.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tool = Tools::ArrowTool;
    isInitialized = false;
    ToolBarOption();
    ToolBarDraws();
    QRectF sceneRect = QRectF(0, 0, PaperSize, PaperSize);
    scene = new VMainGraphicsScene(sceneRect);
    view = new VMainGraphicsView();
    ui->LayoutView->addWidget(view);
    view->setScene(scene);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(12);
    view->setSizePolicy(policy);
    //view->setMinimumSize(800, 600);

    connect(scene, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    connect(ui->toolButtonSinglePoint, &QToolButton::clicked, this,
            &MainWindow::ToolSinglePoint);
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
    connect(ui->toolButtonEndLine, &QToolButton::clicked, this,
            &MainWindow::ToolEndLine);
    connect(ui->toolButtonLine, &QToolButton::clicked, this,
            &MainWindow::ToolLine);
    connect(ui->toolButtonAlongLine, &QToolButton::clicked, this,
            &MainWindow::ToolAlongLine);
    connect(ui->toolButtonShoulderPoint, &QToolButton::clicked, this,
            &MainWindow::ToolShoulderPoint);
    connect(ui->toolButtonNormal, &QToolButton::clicked, this,
            &MainWindow::ToolNormal);
    connect(ui->toolButtonBisector, &QToolButton::clicked, this,
            &MainWindow::ToolBisector);
    connect(ui->toolButtonLineIntersect, &QToolButton::clicked, this,
            &MainWindow::ToolLineIntersect);
    connect(ui->toolButtonSpline, &QToolButton::clicked, this,
            &MainWindow::ToolSpline);
    connect(ui->toolButtonArc, &QToolButton::clicked, this,
            &MainWindow::ToolArc);
    connect(ui->toolButtonSplinePath, &QToolButton::clicked, this,
            &MainWindow::ToolSplinePath);

    data = new VContainer;

    doc = new VDomDocument(data);
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
        bOk = false;
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
    comboBoxDraws->addItem(nameDraw, true);
    index = comboBoxDraws->findText(nameDraw);
    if ( index != -1 ) { // -1 for not found
        comboBoxDraws->setCurrentIndex(index);
    }
    SetEnableWidgets(true);
    SetEnableTool(false);
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
        bOk = false;
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
        dialog = QSharedPointer<Dialog>(new Dialog(data));
        connect(scene, &VMainGraphicsScene::ChoosedObject, dialog.data(), &Dialog::ChoosedObject);
        connect(dialog.data(), &Dialog::DialogClosed, this, closeDialogSlot);
        connect(doc, &VDomDocument::FullUpdateFromFile, dialog.data(), &Dialog::UpdateList);
    } else {
        if(QToolButton *tButton = qobject_cast< QToolButton * >(this->sender())){
            tButton->setChecked(true);
        }
    }
}

/*
 * Інструмет базова точка креслення.
 */
void MainWindow::ToolSinglePoint(bool checked){
    if(checked){
        CanselTool();
        tool = Tools::SinglePointTool;
        QPixmap pixmap(":/cursor/spoint_cursor.png");
        QCursor cur(pixmap, 2, 3);
        view->setCursor(cur);
        helpLabel->setText("Виберіть розташування для точки.");
        dialogSinglePoint = new DialogSinglePoint(data);
        //покажемо вікно як тільки буде вибрано місце розташування для точки
        connect(scene, &VMainGraphicsScene::mousePress, dialogSinglePoint,
                &DialogSinglePoint::mousePress);
        connect(dialogSinglePoint, &DialogSinglePoint::DialogClosed, this,
                &MainWindow::ClosedDialogSinglePoint);
    } else { //не даємо користувачу зняти виділення кнопки
        ui->toolButtonSinglePoint->setChecked(true);
    }
}

void MainWindow::ClosedDialogSinglePoint(int result){
    if(result == QDialog::Accepted){
        QPointF point = dialogSinglePoint->getPoint();
        QString name = dialogSinglePoint->getName();

        qint64 id = data->AddPoint(VPointF(point.x(), point.y(), name, 5, 10));
        VToolSinglePoint *spoint = new VToolSinglePoint(doc, data, id, Tool::FromGui);
        scene->addItem(spoint);
        connect(spoint, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id, spoint);
        ArrowTool();
        ui->toolButtonSinglePoint->setEnabled(false);
        qint32 index = comboBoxDraws->currentIndex();
        comboBoxDraws->setItemData(index, false);
        ui->actionSave->setEnabled(true);
        SetEnableTool(true);
    }
    ArrowTool();
}

void MainWindow::ToolEndLine(bool checked){
    SetToolButton(checked, Tools::EndLineTool, ":/cursor/endline_cursor.png", dialogEndLine,
                  &MainWindow::ClosedDialogEndLine);
}

void MainWindow::ClosedDialogEndLine(int result){
    if(result == QDialog::Accepted){
        VToolEndLine::Create(dialogEndLine, scene, doc, data);
    }
    ArrowTool();
}

void MainWindow::ToolLine(bool checked){
    SetToolButton(checked, Tools::LineTool, ":/cursor/line_cursor.png", dialogLine,
                  &MainWindow::ClosedDialogLine);
}

void MainWindow::ClosedDialogLine(int result){
    if(result == QDialog::Accepted){
        VToolLine::Create(dialogLine, scene, doc, data);
    }
    ArrowTool();
}

void MainWindow::ToolAlongLine(bool checked){
    SetToolButton(checked, Tools::AlongLineTool, ":/cursor/alongline_cursor.png", dialogAlongLine,
                  &MainWindow::ClosedDialogAlongLine);
}

void MainWindow::ClosedDialogAlongLine(int result){
    if(result == QDialog::Accepted){
        VToolAlongLine::Create(dialogAlongLine, scene, doc, data);
    }
    ArrowTool();
}

void MainWindow::ToolShoulderPoint(bool checked){
    SetToolButton(checked, Tools::ShoulderPointTool, ":/cursor/shoulder_cursor.png", dialogShoulderPoint,
                  &MainWindow::ClosedDialogShoulderPoint);
}

void MainWindow::ClosedDialogShoulderPoint(int result){
    if(result == QDialog::Accepted){
        VToolShoulderPoint::Create(dialogShoulderPoint, scene, doc, data);
    }
    ArrowTool();
}

void MainWindow::ToolNormal(bool checked){
    SetToolButton(checked, Tools::NormalTool, ":/cursor/normal_cursor.png", dialogNormal,
                  &MainWindow::ClosedDialogNormal);
}

void MainWindow::ClosedDialogNormal(int result){
    if(result == QDialog::Accepted){
        VToolNormal::Create(dialogNormal, scene, doc, data);
    }
    ArrowTool();
}

void MainWindow::ToolBisector(bool checked){
    SetToolButton(checked, Tools::BisectorTool, ":/cursor/bisector_cursor.png", dialogBisector,
                  &MainWindow::ClosedDialogBisector);
}

void MainWindow::ClosedDialogBisector(int result){
    if(result == QDialog::Accepted){
        VToolBisector::Create(dialogBisector, scene, doc, data);
    }
    ArrowTool();
}

void MainWindow::ToolLineIntersect(bool checked){
    SetToolButton(checked, Tools::LineIntersectTool, ":/cursor/intersect_cursor.png", dialogLineIntersect,
                  &MainWindow::ClosedDialogLineIntersect);
}

void MainWindow::ClosedDialogLineIntersect(int result){
    if(result == QDialog::Accepted){
        VToolLineIntersect::Create(dialogLineIntersect, scene, doc, data);
    }
    ArrowTool();
}

void MainWindow::ToolSpline(bool checked){
    SetToolButton(checked, Tools::SplineTool, ":/cursor/spline_cursor.png", dialogSpline,
                  &MainWindow::ClosedDialogSpline);
}

void MainWindow::ClosedDialogSpline(int result){
    if(result == QDialog::Accepted){
        VToolSpline::Create(dialogSpline, scene, doc, data);
    }
    ArrowTool();
}

void MainWindow::ToolArc(bool checked){
    SetToolButton(checked, Tools::ArcTool, ":/cursor/arc_cursor.png", dialogArc,
                  &MainWindow::ClosedDialogArc);
}

void MainWindow::ClosedDialogArc(int result){
    if(result == QDialog::Accepted){
        VToolArc::Create(dialogArc, scene, doc, data);
    }
    ArrowTool();
}

void MainWindow::ToolSplinePath(bool checked){
    SetToolButton(checked, Tools::SplinePathTool, ":/cursor/splinepath_cursor.png", dialogSplinePath,
                  &MainWindow::ClosedDialogSplinePath);
}

void MainWindow::ClosedDialogSplinePath(int result){
    if(result == QDialog::Accepted){
        VToolSplinePath::Create(dialogSplinePath, scene, doc, data);
    }
    ArrowTool();
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
    QScrollBar *horScrollBar = view->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar *verScrollBar = view->verticalScrollBar();
    verScrollBar->setValue(verScrollBar->minimum());

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
}

void MainWindow::currentDrawChanged( int index ){
    if(index != -1) {
        bool status = qvariant_cast<bool>(comboBoxDraws->itemData(index));
        ui->toolButtonSinglePoint->setEnabled(status);
        if(ui->toolButtonSinglePoint->isEnabled() == false){
            SetEnableTool(true);
        } else {
            SetEnableTool(false);
        }
        doc->ChangeActivDraw(comboBoxDraws->itemText(index));
    }
}

void MainWindow::mouseMove(QPointF scenePos){
    QString string = QString("%1, %2")
                            .arg((qint32)(scenePos.x()/PrintDPI*25.4))
                            .arg((qint32)(scenePos.y()/PrintDPI*25.4));
    mouseCoordinate->setText(string);
}

void MainWindow::CanselTool(){
    switch( tool )
    {
        case Tools::ArrowTool:
            ui->actionArrowTool->setChecked(false);
            break;
        case Tools::SinglePointTool:
            //Знищимо діалогове вікно.
            delete dialogSinglePoint;
            ui->toolButtonSinglePoint->setChecked(false);
            break;
        case Tools::EndLineTool:
            dialogEndLine.clear();
            ui->toolButtonEndLine->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearSelection();
            break;
        case Tools::LineTool:
            dialogLine.clear();
            ui->toolButtonLine->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearFocus();
            break;
        case Tools::AlongLineTool:
            dialogAlongLine.clear();
            ui->toolButtonAlongLine->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearSelection();
            break;
        case Tools::ShoulderPointTool:
            dialogShoulderPoint.clear();
            ui->toolButtonShoulderPoint->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearSelection();
            break;
        case Tools::NormalTool:
            dialogNormal.clear();
            ui->toolButtonNormal->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearSelection();
            break;
        case Tools::BisectorTool:
            dialogBisector.clear();
            ui->toolButtonBisector->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearSelection();
            break;
        case Tools::LineIntersectTool:
            dialogLineIntersect.clear();
            ui->toolButtonLineIntersect->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearSelection();
            break;
        case Tools::SplineTool:
            dialogSpline.clear();
            ui->toolButtonSpline->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearSelection();
            break;
        case Tools::ArcTool:
            dialogArc.clear();
            ui->toolButtonArc->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearSelection();
            break;
        case Tools::SplinePathTool:
            dialogSplinePath.clear();
            ui->toolButtonSplinePath->setChecked(false);
            scene->setFocus(Qt::OtherFocusReason);
            scene->clearSelection();
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
    } else {
        ui->actionDraw->setChecked(true);
    }
}

void MainWindow::ActionDetails(bool checked){
    if(checked){
        ui->actionDraw->setChecked(false);
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
            scene->clear();
            comboBoxDraws->clear();
//            ui->toolButtonSinglePoint->setEnabled(true);
            disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                    this, &MainWindow::currentDrawChanged);
            doc->Parse(Document::FullParse, scene, comboBoxDraws);
            connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                    this, &MainWindow::currentDrawChanged);
            ui->actionSave->setEnabled(true);
            ui->actionSaveAs->setEnabled(true);
            ui->actionTable->setEnabled(true);
            QString nameDraw = doc->GetNameActivDraw();
            qint32 index = comboBoxDraws->findText(nameDraw);
            if ( index != -1 ) { // -1 for not found
                comboBoxDraws->setCurrentIndex(index);
            }
            if(comboBoxDraws->count() == 1){
                if(ui->toolButtonSinglePoint->isEnabled()==false){
                    SetEnableTool(true);
                } else {
                    SetEnableTool(false);
                }
            }

        }
        file.close();
    }
}

void MainWindow::ActionNew(){
    setWindowTitle("Valentina");
    fileName.clear();
    data->Clear();
    doc->clear();
    scene->clear();
    CanselTool();
    comboBoxDraws->clear();
    fileName.clear();
    ui->toolButtonSinglePoint->setEnabled(true);
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
    ui->toolButtonSinglePoint->setEnabled(enable);
    ui->actionOptionDraw->setEnabled(enable);
    if(enable == true && !fileName.isEmpty()){
        ui->actionSave->setEnabled(enable);
    }
    ui->actionTable->setEnabled(enable);
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
