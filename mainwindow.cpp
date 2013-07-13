#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QSpinBox>
#include <QScrollBar>
#include <QShowEvent>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>

#include "options.h"

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
    ui->graphicsView->setScene(scene);

    connect(scene, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    connect(ui->toolButtonSinglePoint, &QToolButton::clicked, this,
            &MainWindow::clickedToolButtonSinglePoint);
    helpLabel = new QLabel("Створіть новий файл для початку роботи.");
    ui->statusBar->addWidget(helpLabel);

    connect(ui->actionArrowTool, &QAction::triggered, this, &MainWindow::triggeredActionAroowTool);
    connect(ui->actionDraw, &QAction::triggered, this, &MainWindow::triggeredActionDraw);
    connect(ui->actionDetails, &QAction::triggered, this, &MainWindow::triggeredActionDetails);
    connect(ui->actionNewDraw, &QAction::triggered, this, &MainWindow::triggeredActionNewDraw);
    connect(ui->actionOptionDraw, &QAction::triggered, this, &MainWindow::triggeredOptionDraw);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::triggeredActionSaveAs);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::triggeredActionSave);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::triggeredActionOpen);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::triggeredActionNew);

    data = new VContainer;

    doc = new VDomDocument(data);
    doc->CreateEmptyFile();
    connect(doc, &VDomDocument::haveChange, this, &MainWindow::haveChange);

    fileName.clear();
    changeInFile = false;
}

void MainWindow::triggeredActionNewDraw(){
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
    ui->actionSaveAs->setEnabled(true);
    ui->actionDraw->setEnabled(true);
    ui->actionDetails->setEnabled(true);
    ui->toolButtonSinglePoint->setEnabled(true);
    ui->actionOptionDraw->setEnabled(true);
    ui->actionSave->setEnabled(true);
}

void MainWindow::triggeredOptionDraw(){
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

/*
 * Інструмет базова точка креслення.
 */
void MainWindow::clickedToolButtonSinglePoint(bool checked){
    if(checked){
        CanselTool();
        tool = Tools::SinglePointTool;
        QPixmap pixmap(":/cursor/spoint_cursor.png");
        QCursor cur(pixmap, 2, 3);
        ui->graphicsView->setCursor(cur);
        helpLabel->setText("Виберіть розташування для точки.");
        dialogSinglePoint = new DialogSinglePoint;
        //покажемо вікно як тільки буде вибрано місце розташування для точки
        connect(scene, &VMainGraphicsScene::mousePress, dialogSinglePoint,
                &DialogSinglePoint::mousePress);
        //головне вікно отримає сигнал відміни створення точки
        connect(dialogSinglePoint, &DialogSinglePoint::ToolCanseled, this, &MainWindow::ToolCanseled);
        connect(dialogSinglePoint, &DialogSinglePoint::SinglePointCreated, this,
                &MainWindow::SinglePointCreated);
    } else { //не даємо користувачу зняти виділення кнопки
        ui->toolButtonSinglePoint->setChecked(true);
    }
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
    QScrollBar *horScrollBar = ui->graphicsView->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar *verScrollBar = ui->graphicsView->verticalScrollBar();
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
    QComboBox* comboBoxGrow = new QComboBox;
    comboBoxGrow->clear();
    comboBoxGrow->addItems(list);
    comboBoxGrow->setCurrentIndex(12);
    ui->toolBarOption->addWidget(comboBoxGrow);

    QLabel * labelSize = new QLabel;
    labelSize->setText(" Розмір: ");
    ui->toolBarOption->addWidget(labelSize);

    list.clear();
    list << "28"<<"30"<<"32"<<"34"<<"36"<<"38"<<"40"<<"42"<<"44"<<"46"<<"48"<<"50" << "52" << "54" << "56";
    QComboBox* comboBoxSize = new QComboBox;
    comboBoxSize->clear();
    comboBoxSize->addItems(list);
    comboBoxSize->setCurrentIndex(11);
    ui->toolBarOption->addWidget(comboBoxSize);

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
}

void MainWindow::currentDrawChanged( int index ){
    if(index != -1) {
        bool status = qvariant_cast<bool>(comboBoxDraws->itemData(index));
        ui->toolButtonSinglePoint->setEnabled(status);
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
    }
}

void  MainWindow::ArrowTool(){
    CanselTool();
    ui->actionArrowTool->setChecked(true);
    tool = Tools::ArrowTool;
    QCursor cur(Qt::ArrowCursor);
    ui->graphicsView->setCursor(cur);
    helpLabel->setText("");
}

void MainWindow::triggeredActionAroowTool(){
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

void MainWindow::ToolCanseled(){
    ArrowTool();
}

void MainWindow::SinglePointCreated(const QString name, const QPointF point){
    qint64 id = data->AddPoint(VPointF(point.x(), point.y(), name, 5, 10));
    VToolSimplePoint *spoint = new VToolSimplePoint(doc, data, id, Tool::FromGui);
    scene->addItem(spoint);
    ArrowTool();
    ui->toolButtonSinglePoint->setEnabled(false);
    qint32 index = comboBoxDraws->currentIndex();
    comboBoxDraws->setItemData(index, false);
    ui->actionSave->setEnabled(true);
}

void MainWindow::triggeredActionDraw(bool checked){
    if(checked){
        ui->actionDetails->setChecked(false);
    } else {
        ui->actionDraw->setChecked(true);
    }
}

void MainWindow::triggeredActionDetails(bool checked){
    if(checked){
        ui->actionDraw->setChecked(false);
    } else {
        ui->actionDetails->setChecked(true);
    }
}

void MainWindow::triggeredActionSaveAs(){
    QString filters("Lekalo files (*.xml);;All files (*.*)");
    QString defaultFilter("Lekalo files (*.xml)");
    QString fName = QFileDialog::getSaveFileName(this, "Зберегти файл як", QDir::homePath(),
                                                    filters, &defaultFilter);
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

void MainWindow::triggeredActionSave(){
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

void MainWindow::triggeredActionOpen(){
    QString filter("Lekalo files (*.xml)");
    QString fName = QFileDialog::getOpenFileName(this, tr("Відкрити файл"), QDir::homePath(), filter);
    fileName = fName;
    QFileInfo info(fileName);
    QString title(info.fileName());
    title.append("-Valentina");
    setWindowTitle(title);
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)){
        if(doc->setContent(&file)){
            comboBoxDraws->clear();
            disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                    this, &MainWindow::currentDrawChanged);
            doc->Parse(Document::FullParse, scene, comboBoxDraws);
            connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                    this, &MainWindow::currentDrawChanged);
            ui->actionSave->setEnabled(true);
            ui->actionSaveAs->setEnabled(true);
            QString nameDraw = doc->GetNameActivDraw();
            qint32 index = comboBoxDraws->findText(nameDraw);
            if ( index != -1 ) { // -1 for not found
                comboBoxDraws->setCurrentIndex(index);
            }

        }
        file.close();
    }
}

void MainWindow::triggeredActionNew(){
    setWindowTitle("Valentina");
    data->Clear();
    doc->clear();
    CanselTool();
    comboBoxDraws->clear();
    fileName.clear();
    ui->toolButtonSinglePoint->setEnabled(true);
    ui->actionOptionDraw->setEnabled(false);
    ui->actionSave->setEnabled(false);
}

void MainWindow::haveChange(){
    if(!fileName.isEmpty()){
        ui->actionSave->setEnabled(true);
    }
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
