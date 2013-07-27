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

#include "options.h"
#include "tools/vtoolendline.h"
#include "tools/vtoolline.h"
#include "tools/vtoolalongline.h"

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

    data = new VContainer;
    CreateManTableIGroup ();

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

/*
 * Інструмет базова точка креслення.
 */
void MainWindow::ToolSinglePoint(bool checked){
    if(checked){
        CanselTool();
        tool = Tools::SinglePointTool;
        QPixmap pixmap(":/cursor/spoint_cursor.png");
        QCursor cur(pixmap, 2, 3);
        ui->graphicsView->setCursor(cur);
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

void MainWindow::ToolEndLine(bool checked){
    if(checked){
        CanselTool();
        tool = Tools::EndLineTool;
        QPixmap pixmap(":/cursor/endline_cursor.png");
        QCursor cur(pixmap, 2, 3);
        ui->graphicsView->setCursor(cur);
        helpLabel->setText("Заповніть усі поля.");
        dialogEndLine = new DialogEndLine(data, this);
        connect(scene, &VMainGraphicsScene::ChoosedObject, dialogEndLine,
                &DialogEndLine::ChoosedPoint);
        connect(dialogEndLine, &DialogEndLine::DialogClosed, this,
                &MainWindow::ClosedDialogEndLine);
        connect(doc, &VDomDocument::FullUpdateFromFile, dialogEndLine, &DialogEndLine::UpdateList);
    } else {
        ui->toolButtonEndLine->setChecked(true);
    }
}

void MainWindow::ClosedDialogEndLine(int result){
    if(result == QDialog::Accepted){
        QString pointName = dialogEndLine->getPointName();
        QString typeLine = dialogEndLine->getTypeLine();
        QString formula = dialogEndLine->getFormula();
        qint32 angle = dialogEndLine->getAngle();
        qint64 basePointId = dialogEndLine->getBasePointId();

        VPointF basePoint = data->GetPoint(basePointId);
        QLineF line = QLineF(basePoint.toQPointF(), QPointF(basePoint.x()+100, basePoint.y()));
        Calculator cal(data);
        QString errorMsg;
        qreal result = cal.eval(formula, &errorMsg);
        if(errorMsg.isEmpty()){
            line.setLength(result*PrintDPI/25.4);
            line.setAngle(angle);
            qint64 id = data->AddPoint(VPointF(line.p2().x(), line.p2().y(), pointName, 5, 10));
            VToolEndLine *point = new VToolEndLine(doc, data, id, typeLine, formula, angle, basePointId,
                                                   Tool::FromGui);
            scene->addItem(point);
            connect(point, &VToolPoint::ChoosedPoint, scene, &VMainGraphicsScene::ChoosedItem);
        }

    }
    ArrowTool();
}

void MainWindow::ToolLine(bool checked){
    if(checked){
        CanselTool();
        tool = Tools::LineTool;
        QPixmap pixmap(":/cursor/line_cursor.png");
        QCursor cur(pixmap, 2, 3);
        ui->graphicsView->setCursor(cur);
        helpLabel->setText("Виберіть точки.");
        dialogLine = new DialogLine(data, this);
        connect(scene, &VMainGraphicsScene::ChoosedObject, dialogLine, &DialogLine::ChoosedPoint);
        connect(dialogLine, &DialogLine::DialogClosed, this, &MainWindow::ClosedDialogLine);
    } else {
        ui->toolButtonLine->setChecked(true);
    }
}

void MainWindow::ClosedDialogLine(int result){
    if(result == QDialog::Accepted){
        qint64 firstPoint = dialogLine->getFirstPoint();
        qint64 secondPoint = dialogLine->getSecondPoint();

        qint64 id = data->getNextId();
        VToolLine *line = new VToolLine(doc, data, id, firstPoint, secondPoint, Tool::FromGui);
        scene->addItem(line);
        connect(line, &VToolLine::ChoosedPoint, scene, &VMainGraphicsScene::ChoosedItem);

    }
    ArrowTool();
}

void MainWindow::ToolAlongLine(bool checked){
    if(checked){
        CanselTool();
        tool = Tools::AlongLineTool;
        QPixmap pixmap(":/cursor/alongline_cursor.png");
        QCursor cur(pixmap, 2, 3);
        ui->graphicsView->setCursor(cur);
        helpLabel->setText("Виберіть точки.");
        dialogAlongLine = new DialogAlongLine(data, this);
        connect(scene, &VMainGraphicsScene::ChoosedObject, dialogAlongLine, &DialogAlongLine::ChoosedPoint);
        connect(dialogAlongLine, &DialogLine::DialogClosed, this, &MainWindow::ClosedDialogAlongLine);
    } else {
        ui->toolButtonAlongLine->setChecked(true);
    }
}

void MainWindow::ClosedDialogAlongLine(int result){
    if(result == QDialog::Accepted){
        QString formula = dialogAlongLine->getFormula();
        qint64 firstPointId = dialogAlongLine->getFirstPointId();
        qint64 secondPointId = dialogAlongLine->getSecondPointId();
        QString typeLine = dialogAlongLine->getTypeLine();
        QString pointName = dialogAlongLine->getPointName();

        VPointF firstPoint = data->GetPoint(firstPointId);
        VPointF secondPoint = data->GetPoint(secondPointId);
        QLineF line = QLineF(firstPoint.toQPointF(), secondPoint.toQPointF());
        Calculator cal(data);
        QString errorMsg;
        qreal result = cal.eval(formula, &errorMsg);
        if(errorMsg.isEmpty()){
            line.setLength(result*PrintDPI/25.4);
            qint64 id = data->AddPoint(VPointF(line.p2().x(), line.p2().y(), pointName, 5, 10));
            VToolAlongLine *point = new VToolAlongLine(doc, data, id, formula, firstPointId, secondPointId,
                                                       typeLine, Tool::FromGui);
            scene->addItem(point);
            connect(point, &VToolAlongLine::ChoosedPoint, scene, &VMainGraphicsScene::ChoosedItem);
        }
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
        if(ui->toolButtonSinglePoint->isEnabled()==false){
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
            delete dialogEndLine;
            ui->toolButtonEndLine->setChecked(false);
            scene->clearSelection();
            break;
        case Tools::LineTool:
            delete dialogLine;
            ui->toolButtonLine->setChecked(false);
            scene->clearFocus();
            break;
        case Tools::AlongLineTool:
            delete dialogAlongLine;
            ui->toolButtonAlongLine->setChecked(false);
            scene->clearFocus();
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

void MainWindow::ClosedDialogSinglePoint(int result){
    if(result == QDialog::Accepted){
        QPointF point = dialogSinglePoint->getPoint();
        QString name = dialogSinglePoint->getName();
        qint64 id = data->AddPoint(VPointF(point.x(), point.y(), name, 5, 10));
        VToolSimplePoint *spoint = new VToolSimplePoint(doc, data, id, Tool::FromGui);
        scene->addItem(spoint);
        connect(spoint, &VToolPoint::ChoosedPoint, scene, &VMainGraphicsScene::ChoosedItem);
        ArrowTool();
        ui->toolButtonSinglePoint->setEnabled(false);
        qint32 index = comboBoxDraws->currentIndex();
        comboBoxDraws->setItemData(index, false);
        ui->actionSave->setEnabled(true);
        SetEnableTool(true);
    }
    ArrowTool();
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
    if(ui->toolButtonSinglePoint->isEnabled()==false){
        SetEnableTool(true);
    } else {
        SetEnableTool(false);
    }
}

void MainWindow::ActionNew(){
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
    SetEnableTool(false);
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
