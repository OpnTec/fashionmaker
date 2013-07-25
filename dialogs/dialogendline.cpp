#include "dialogendline.h"
#include "ui_dialogendline.h"
#include <QCloseEvent>
#include <QString>

#include "../container/vpointf.h"
#include "../container/calculator.h"

DialogEndLine::DialogEndLine(const VContainer *data, QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogEndLine)
{
    ui->setupUi(this);
    flagFormula = false;
    flagName = false;
    this->data = data;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogEndLine::DialogAccepted);
    bOk->setEnabled(false);
    FillComboBoxBasePoint();
    QStringList list;
    list<<"Лінія"<<"Без лінії";
    ui->comboBoxLineType->addItems(list);

    connect(ui->toolButtonArrowDown, &QPushButton::clicked, this,
            &DialogEndLine::ArrowDown);
    connect(ui->toolButtonArrowUp, &QPushButton::clicked, this,
            &DialogEndLine::ArrowUp);
    connect(ui->toolButtonArrowLeft, &QPushButton::clicked, this,
            &DialogEndLine::ArrowLeft);
    connect(ui->toolButtonArrowRight, &QPushButton::clicked, this,
            &DialogEndLine::ArrowRight);
    connect(ui->toolButtonArrowLeftUp, &QPushButton::clicked, this,
            &DialogEndLine::ArrowLeftUp);
    connect(ui->toolButtonArrowLeftDown, &QPushButton::clicked, this,
            &DialogEndLine::ArrowLeftDown);
    connect(ui->toolButtonArrowRightUp, &QPushButton::clicked, this,
            &DialogEndLine::ArrowRightUp);
    connect(ui->toolButtonArrowRightDown, &QPushButton::clicked, this,
            &DialogEndLine::ArrowRightDown);
    connect(ui->toolButtonPutHere, &QPushButton::clicked, this,
            &DialogEndLine::ClickedToolButtonPutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogEndLine::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);

    ShowBase();
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this,
            &DialogEndLine::ClickedSizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this,
            &DialogEndLine::ClickedStandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this,
            &DialogEndLine::ClickedIncrements);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this,
            &DialogEndLine::ClickedLengthLines);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this,
            &DialogEndLine::ClickedToolButtonEqual);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogEndLine::NameChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogEndLine::FormulaChanged);

    timerFormula = new QTimer(this);
    connect(timerFormula, &QTimer::timeout, this, &DialogEndLine::ClickedToolButtonEqual);
}

void DialogEndLine::ShowBase(){
    disconnect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);
    ui->listWidget->clear();
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);
    const QMap<QString, qint32> *base = data->DataBase();
    QMapIterator<QString, qint32> i(*base);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow (0);
}

void DialogEndLine::ShowStandartTable(){
    disconnect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);
    ui->listWidget->clear();
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);
    const QMap<QString, VStandartTableCell> *standartTable = data->DataStandartTable();
    QMapIterator<QString, VStandartTableCell> i(*standartTable);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow (0);
}

void DialogEndLine::ShowIncrementTable(){
    disconnect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);
    ui->listWidget->clear();
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);
    const QMap<QString, VIncrementTableRow> *incrementTable = data->DataIncrementTable();
    QMapIterator<QString, VIncrementTableRow> i(*incrementTable);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow (0);
}

void DialogEndLine::ShowLengthLines(){
    disconnect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);
    ui->listWidget->clear();
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogEndLine::ValChenged);
    const QMap<QString, qreal> *linesTable = data->DataLengthLines();
    QMapIterator<QString, qreal> i(*linesTable);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow (0);
}

void DialogEndLine::CheackState(){
    bOk->setEnabled(flagFormula & flagName);
}

void DialogEndLine::ClickedSizeGrowth(){
    ShowBase();
}

void DialogEndLine::ClickedStandartTable(){
    ShowStandartTable();
}

void DialogEndLine::ClickedLengthLines(){
    ShowLengthLines();
}

void DialogEndLine::ClickedIncrements(){
    ShowIncrementTable();
}

void DialogEndLine::ChoosedPoint(qint64 id, Scene::Type type){
    if(type == Scene::Point){
        VPointF point = data->GetPoint(id);
        qint32 index = ui->comboBoxBasePoint->findText(point.name());
        if ( index != -1 ) { // -1 for not found
            ui->comboBoxBasePoint->setCurrentIndex(index);
        }
        this->show();
    }
}

QString DialogEndLine::getPointName() const{
    return pointName;
}

void DialogEndLine::setPointName(const QString &value){
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

QString DialogEndLine::getTypeLine() const{
    return typeLine;
}

void DialogEndLine::setTypeLine(const QString &value){
    typeLine = value;
    if(typeLine == "hair"){
        qint32 index = ui->comboBoxLineType->findText("Лінія");
        if(index != -1){
            ui->comboBoxLineType->setCurrentIndex(index);
        }
    }
    if(typeLine == "none"){
        qint32 index = ui->comboBoxLineType->findText("Без лінії");
        if(index != -1){
            ui->comboBoxLineType->setCurrentIndex(index);
        }
    }
}

QString DialogEndLine::getFormula() const{
    return formula;
}

void DialogEndLine::setFormula(const QString &value){
    formula = value;
    ui->lineEditFormula->setText(formula);
}

qint32 DialogEndLine::getAngle() const{
    return angle;
}

void DialogEndLine::setAngle(const qint32 &value){
    angle = value;
    ui->spinBoxAngle->setValue(angle);
}

void DialogEndLine::closeEvent ( QCloseEvent * event ){
    DialogClosed(QDialog::Rejected);
    event->accept();
}

qint64 DialogEndLine::getBasePointId() const
{
    return basePointId;
}

void DialogEndLine::setBasePointId(const qint64 &value)
{
    basePointId = value;
    qint32 index = ui->comboBoxBasePoint->findData(basePointId);
    if(index != -1){
        ui->comboBoxBasePoint->setCurrentIndex(index);
    }
}

void DialogEndLine::FillComboBoxBasePoint(){
    const QMap<qint64, VPointF> *points = data->DataPoints();
    QMapIterator<qint64, VPointF> i(*points);
    while (i.hasNext()) {
        i.next();
        VPointF point = i.value();
        ui->comboBoxBasePoint->addItem(point.name(), i.key());
    }
}

void DialogEndLine::ArrowUp(){
    ui->spinBoxAngle->setValue(90);
}

void DialogEndLine::ArrowDown(){
    ui->spinBoxAngle->setValue(270);
}

void DialogEndLine::ArrowLeft(){
    ui->spinBoxAngle->setValue(180);
}

void DialogEndLine::ArrowRight(){
    ui->spinBoxAngle->setValue(0);
}

void DialogEndLine::ArrowLeftUp(){
    ui->spinBoxAngle->setValue(135);
}

void DialogEndLine::ArrowLeftDown(){
    ui->spinBoxAngle->setValue(225);
}

void DialogEndLine::ArrowRightUp(){
    ui->spinBoxAngle->setValue(45);
}

void DialogEndLine::ArrowRightDown(){
    ui->spinBoxAngle->setValue(315);
}

void DialogEndLine::ClickedToolButtonPutHere(){
    QListWidgetItem *item = ui->listWidget->currentItem();
    QString val = item->text();
    ui->lineEditFormula->setText(ui->lineEditFormula->text().append(val));
}

void DialogEndLine::PutVal(QListWidgetItem *item){
    QString val = item->text();
    ui->lineEditFormula->setText(ui->lineEditFormula->text().append(val));
}

void DialogEndLine::ValChenged(int row){
    if(ui->listWidget->count() == 0){
        return;
    }
    QListWidgetItem *item = ui->listWidget->item( row );
    if(ui->radioButtonSizeGrowth->isChecked()){
        if(item->text()=="Р"){
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->growth()).arg("Зріст");
            ui->labelDescription->setText(desc);
        }
        if(item->text()=="Сг"){
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->size()).arg("Розмір");
            ui->labelDescription->setText(desc);
        }
    }
    if(ui->radioButtonStandartTable->isChecked()){
        VStandartTableCell stable = data->GetStandartTableCell(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueStandartTableCell(item->text()))
                .arg(stable.GetDescription());
        ui->labelDescription->setText(desc);
    }
    if(ui->radioButtonIncrements->isChecked()){
        VIncrementTableRow itable = data->GetIncrementTableRow(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueIncrementTableRow(item->text()))
                .arg(itable.getDescription());
        ui->labelDescription->setText(desc);
    }
}

void DialogEndLine::ClickedToolButtonEqual(){
    if(ui->lineEditFormula->text().isEmpty()){
        flagFormula = false;
    } else {
        Calculator cal(data);
        QString errorMsg;
        qreal result = cal.eval(ui->lineEditFormula->text(),&errorMsg);
        if(!errorMsg.isEmpty()){
            ui->labelResultCalculation->setText("Помилка.");
            flagFormula = false;
        } else {
            ui->labelResultCalculation->setText(QString().setNum(result));
            flagFormula = true;
        }
    }
    CheackState();
    timerFormula->stop();
}

void DialogEndLine::NameChanged(){
    QString name = ui->lineEditNamePoint->text();
    if(name.isEmpty() || name.contains(" ")){
        flagName = false;
    } else {
        flagName = true;
    }
    CheackState();
}

void DialogEndLine::FormulaChanged(){
    if(ui->lineEditFormula->text().isEmpty()){
        flagFormula = false;
        CheackState();
        return;
    }
    timerFormula->start(1000);
}

void DialogEndLine::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    if(ui->comboBoxLineType->currentText()=="Лінія"){
        typeLine = QString("hair");
    } else {
        typeLine = QString("none");
    }
    formula = ui->lineEditFormula->text();
    angle = ui->spinBoxAngle->value();
    qint32 index = ui->comboBoxBasePoint->currentIndex();
    basePointId = qvariant_cast<qint64>(ui->comboBoxBasePoint->itemData(index));
    DialogClosed(QDialog::Accepted);
}

void DialogEndLine::UpdateList(){
    if(ui->radioButtonSizeGrowth->isChecked()){
        ShowBase();
    }
    if(ui->radioButtonStandartTable->isChecked()){
        ShowStandartTable();
    }
    if(ui->radioButtonIncrements->isChecked()){
        ShowIncrementTable();
    }
}

DialogEndLine::~DialogEndLine()
{
    delete ui;
}
