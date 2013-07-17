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
    connect(ui->actionTable, &QAction::triggered, this, &MainWindow::triggeredActionTable);

    data = new VContainer;
    CreateManTableIGroup ();

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
    SetEnableWidgets(true);
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
            CreateManTableIGroup ();
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

        }
        file.close();
    }
}

void MainWindow::triggeredActionNew(){
    setWindowTitle("Valentina");
    data->Clear();
    doc->clear();
    CreateManTableIGroup ();
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

void MainWindow::CreateManTableIGroup () const{
    data->AddStandartTableCell("Pkor", VStandartTableCell(84, 0, 3));
    data->AddStandartTableCell("Pkor", VStandartTableCell(84, 0, 3));
    data->AddStandartTableCell("Vtos", VStandartTableCell(1450, 2, 51));
    data->AddStandartTableCell("Vtosh", VStandartTableCell(1506, 2, 54));
    data->AddStandartTableCell("Vpt", VStandartTableCell(1438, 3, 52));
    data->AddStandartTableCell("Vst", VStandartTableCell(1257, -1, 49));
    data->AddStandartTableCell("Vlt", VStandartTableCell(1102, 0, 43));
    data->AddStandartTableCell("Vk", VStandartTableCell(503, 0, 22));
    data->AddStandartTableCell("Vsht", VStandartTableCell(1522, 2, 54));
    data->AddStandartTableCell("Vzy", VStandartTableCell(1328, 0, 49));
    data->AddStandartTableCell("Vlop", VStandartTableCell(1320, 0, 49));
    data->AddStandartTableCell("Vps", VStandartTableCell(811, -1, 36));
    data->AddStandartTableCell("Osh", VStandartTableCell(404,8, 2));
    data->AddStandartTableCell("OgI", VStandartTableCell(1034, 36, 4));
    data->AddStandartTableCell("OgII", VStandartTableCell(1044, 38, 2));
    data->AddStandartTableCell("OgIII", VStandartTableCell(1000, 40, 0));
    data->AddStandartTableCell("Ot", VStandartTableCell(780, 40, 0));
    data->AddStandartTableCell("Ob", VStandartTableCell(984, 30, 10));
    data->AddStandartTableCell("ObI", VStandartTableCell(964, 24, 12));
    data->AddStandartTableCell("Obed", VStandartTableCell(566, 18, 6));
    data->AddStandartTableCell("Ok", VStandartTableCell(386, 8, 8));
    data->AddStandartTableCell("Oi", VStandartTableCell(380, 8, 6));
    data->AddStandartTableCell("Osch", VStandartTableCell(234, 4, 4));
    data->AddStandartTableCell("Os", VStandartTableCell(350, 2, 8));
    data->AddStandartTableCell("Dsb", VStandartTableCell(1120, 0, 44));
    data->AddStandartTableCell("Dsp", VStandartTableCell(1110, 0, 43));
    data->AddStandartTableCell("Dn", VStandartTableCell(826, -3, 37));
    data->AddStandartTableCell("Dps", VStandartTableCell(316, 4, 7));
    data->AddStandartTableCell("Dpob", VStandartTableCell(783, 14, 15));
    data->AddStandartTableCell("Ds", VStandartTableCell(260, 1, 6));
    data->AddStandartTableCell("Op", VStandartTableCell(316, 12, 0));
    data->AddStandartTableCell("Ozap", VStandartTableCell(180, 4, 0));
    data->AddStandartTableCell("Pkis", VStandartTableCell(250, 4, 0));
    data->AddStandartTableCell("SHp", VStandartTableCell(160, 1, 4));
    data->AddStandartTableCell("Dlych", VStandartTableCell(500, 2, 15));
    data->AddStandartTableCell("Dzap", VStandartTableCell(768, 2, 24));
    data->AddStandartTableCell("DIIIp", VStandartTableCell(970, 2, 29));
    data->AddStandartTableCell("Vprp", VStandartTableCell(214, 3, 3));
    data->AddStandartTableCell("Vg", VStandartTableCell(262, 8, 3));
    data->AddStandartTableCell("Dtp", VStandartTableCell(460, 7, 9));
    data->AddStandartTableCell("Dp", VStandartTableCell(355, 5, 5));
    data->AddStandartTableCell("Vprz", VStandartTableCell(208, 3, 5));
    data->AddStandartTableCell("Dts", VStandartTableCell(438, 2, 10));
    data->AddStandartTableCell("DtsI", VStandartTableCell(469, 2, 10));
    data->AddStandartTableCell("Dvcht", VStandartTableCell(929, 9, 19));
    data->AddStandartTableCell("SHg", VStandartTableCell(370, 14, 4));
    data->AddStandartTableCell("Cg", VStandartTableCell(224, 6, 0));
    data->AddStandartTableCell("SHs", VStandartTableCell(416, 10, 2));
    data->AddStandartTableCell("dpzr", VStandartTableCell(121, 6, 0));
    data->AddStandartTableCell("Ogol", VStandartTableCell(576, 4, 4));
    data->AddStandartTableCell("Ssh1", VStandartTableCell(205, 5, 0));
    data->AddStandartTableCell("St", VStandartTableCell(410, 20, 0));
    data->AddStandartTableCell("Drzap", VStandartTableCell(594, 3, 19));
    data->AddStandartTableCell("DbII", VStandartTableCell(1020, 0, 44));
    data->AddStandartTableCell("Sb", VStandartTableCell(504, 15, 4));
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
    ui->actionSave->setEnabled(enable);
    ui->actionTable->setEnabled(enable);
}

void MainWindow::triggeredActionTable(bool checked){
    if(checked){
        dialogTable = new DialogIncrements(data, doc, 0);
        connect(dialogTable, &DialogIncrements::closedActionTable, this,
                &MainWindow::closedActionTable);
        dialogTable->show();
    } else {
        ui->actionTable->setChecked(true);
        dialogTable->activateWindow();
    }
}

void MainWindow::closedActionTable(){
    ui->actionTable->setChecked(false);
    delete dialogTable;
}

void MainWindow::closeEvent ( QCloseEvent * event ){
    if(ui->actionTable->isChecked()==true){
        delete dialogTable;
    }
    event->accept();
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
