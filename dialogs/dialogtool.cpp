#include "dialogtool.h"
#include <QListWidgetItem>
#include <QCloseEvent>
#include "../container/calculator.h"

DialogTool::DialogTool(const VContainer *data, QWidget *parent):QDialog(parent){
    Q_CHECK_PTR(data);
    this->data = data;
    flagName = true;
    flagFormula = true;
    timerFormula = new QTimer(this);
    connect(timerFormula, &QTimer::timeout, this, &DialogTool::EvalFormula);
    isInitialized = false;

    bOk = 0;
    spinBoxAngle = 0;
    lineEditFormula = 0;
    listWidget = 0;
    labelResultCalculation = 0;
    labelDescription = 0;
    radioButtonSizeGrowth = 0;
    radioButtonStandartTable = 0;
    radioButtonIncrements = 0;
    radioButtonLengthLine = 0;
}

DialogTool::~DialogTool()
{
}

void DialogTool::closeEvent(QCloseEvent *event){
    DialogClosed(QDialog::Rejected);
    event->accept();
}

void DialogTool::showEvent(QShowEvent *event){
    QDialog::showEvent( event );
    if( event->spontaneous() ){
        return;
    }

    if(isInitialized){
        return;
    }

    // do your init stuff here
    isInitialized = true;//перший показ вікна вже відбувся
}

void DialogTool::FillComboBoxPoints(QComboBox *box) const{
    const QMap<qint64, VPointF> *points = data->DataPoints();
    QMapIterator<qint64, VPointF> i(*points);
    while (i.hasNext()) {
        i.next();
        VPointF point = i.value();
        box->addItem(point.name(), i.key());
    }
}

void DialogTool::FillComboBoxTypeLine(QComboBox *box) const{
    QStringList list;
    list<<"Лінія"<<"Без лінії";
    box->addItems(list);
}

QString DialogTool::GetTypeLine(const QComboBox *box) const{
    if(box->currentText()=="Лінія"){
        return QString("hair");
    } else {
        return QString("none");
    }
}

void DialogTool::ShowBase(){
    Q_CHECK_PTR(listWidget);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();
    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    const QMap<QString, qint32> *base = data->DataBase();
    QMapIterator<QString, qint32> i(*base);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        listWidget->addItem(item);
    }
    listWidget->setCurrentRow (0);
}

void DialogTool::ShowStandartTable(){
    Q_CHECK_PTR(listWidget);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();
    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    const QMap<QString, VStandartTableCell> *standartTable = data->DataStandartTable();
    QMapIterator<QString, VStandartTableCell> i(*standartTable);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        listWidget->addItem(item);
    }
    listWidget->setCurrentRow (0);
}

void DialogTool::ShowIncrementTable(){
    Q_CHECK_PTR(listWidget);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();
    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    const QMap<QString, VIncrementTableRow> *incrementTable = data->DataIncrementTable();
    QMapIterator<QString, VIncrementTableRow> i(*incrementTable);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        listWidget->addItem(item);
    }
    listWidget->setCurrentRow (0);
}

void DialogTool::ShowLengthLines(){
    Q_CHECK_PTR(listWidget);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();
    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    const QMap<QString, qreal> *linesTable = data->DataLengthLines();
    QMapIterator<QString, qreal> i(*linesTable);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        listWidget->addItem(item);
    }
    listWidget->setCurrentRow (0);
}

void DialogTool::SetupTypeLine(QComboBox *box, const QString &value){
    if(value == "hair"){
        qint32 index = box->findText("Лінія");
        if(index != -1){
            box->setCurrentIndex(index);
        }
    }
    if(value == "none"){
        qint32 index = box->findText("Без лінії");
        if(index != -1){
            box->setCurrentIndex(index);
        }
    }
}

void DialogTool::ChangeCurrentText(QComboBox *box, const QString &value){
    qint32 index = box->findText(value);
    if(index != -1){
        box->setCurrentIndex(index);
    }
}

void DialogTool::ChangeCurrentData(QComboBox *box, const qint64 &value){
    qint32 index = box->findData(value);
    if(index != -1){
        box->setCurrentIndex(index);
    }
}

void DialogTool::CheckState(){
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagFormula & flagName);
}


void DialogTool::ChoosedPoint(qint64 id, Scene::Type type){
    Q_UNUSED(id);
    Q_UNUSED(type);
}

void DialogTool::NamePointChanged(){
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit){
        QString name = edit->text();
        if(name.isEmpty() || name.contains(" ")){
            flagName = false;
        } else {
            flagName = true;
        }
    }
    CheckState();
}

void DialogTool::DialogAccepted(){
    emit DialogClosed(QDialog::Accepted);
}

void DialogTool::DialogRejected(){
    emit DialogClosed(QDialog::Rejected);
}

void DialogTool::FormulaChanged(){
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if(edit){
        if(edit->text().isEmpty()){
            flagFormula = false;
            CheckState();
            return;
        }
        timerFormula->start(1000);
    }
}

void DialogTool::ArrowUp(){
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(90);
}

void DialogTool::ArrowDown(){
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(270);
}

void DialogTool::ArrowLeft(){
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(180);
}

void DialogTool::ArrowRight(){
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(0);
}

void DialogTool::ArrowLeftUp(){
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(135);
}

void DialogTool::ArrowLeftDown(){
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(225);
}

void DialogTool::ArrowRightUp(){
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(45);
}

void DialogTool::ArrowRightDown(){
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(315);
}

void DialogTool::EvalFormula(){
    Q_CHECK_PTR(lineEditFormula);
    Q_CHECK_PTR(labelResultCalculation);
    if(lineEditFormula->text().isEmpty()){
        flagFormula = false;
    } else {
        Calculator cal(data);
        QString errorMsg;
        qreal result = cal.eval(lineEditFormula->text(),&errorMsg);
        if(!errorMsg.isEmpty()){
            labelResultCalculation->setText("Помилка.");
            flagFormula = false;
        } else {
            labelResultCalculation->setText(QString().setNum(result));
            flagFormula = true;
        }
    }
    CheckState();
    timerFormula->stop();
}

void DialogTool::SizeGrowth(){
    ShowBase();
}

void DialogTool::StandartTable(){
    ShowStandartTable();
}

void DialogTool::LengthLines(){
    ShowLengthLines();
}

void DialogTool::Increments(){
    ShowIncrementTable();
}

void DialogTool::PutHere(){
    Q_CHECK_PTR(lineEditFormula);
    Q_CHECK_PTR(listWidget);
    QListWidgetItem *item = listWidget->currentItem();
    QString val = item->text();
    lineEditFormula->setText(lineEditFormula->text().append(val));
}

void DialogTool::PutVal(QListWidgetItem *item){
    Q_CHECK_PTR(lineEditFormula);
    QString val = item->text();
    lineEditFormula->setText(lineEditFormula->text().append(val));
}

void DialogTool::ValChenged(int row){
    Q_CHECK_PTR(listWidget);
    Q_CHECK_PTR(labelDescription);
    Q_CHECK_PTR(radioButtonSizeGrowth);
    Q_CHECK_PTR(radioButtonStandartTable);
    Q_CHECK_PTR(radioButtonIncrements);
    if(listWidget->count() == 0){
        return;
    }
    QListWidgetItem *item = listWidget->item( row );
    if(radioButtonSizeGrowth->isChecked()){
        if(item->text()=="Р"){
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->growth()).arg("Зріст");
            labelDescription->setText(desc);
        }
        if(item->text()=="Сг"){
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->size()).arg("Розмір");
            labelDescription->setText(desc);
        }
    }
    if(radioButtonStandartTable->isChecked()){
        VStandartTableCell stable = data->GetStandartTableCell(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueStandartTableCell(item->text()))
                .arg(stable.GetDescription());
        labelDescription->setText(desc);
    }
    if(radioButtonIncrements->isChecked()){
        VIncrementTableRow itable = data->GetIncrementTableRow(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueIncrementTableRow(item->text()))
                .arg(itable.getDescription());
        labelDescription->setText(desc);
    }
    if(radioButtonLengthLine->isChecked()){
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLine(item->text()))
                .arg("Довжина лінії");
        labelDescription->setText(desc);
    }
}

void DialogTool::UpdateList(){
    Q_CHECK_PTR(radioButtonSizeGrowth);
    Q_CHECK_PTR(radioButtonStandartTable);
    Q_CHECK_PTR(radioButtonIncrements);
    if(radioButtonSizeGrowth->isChecked()){
        ShowBase();
    }
    if(radioButtonStandartTable->isChecked()){
        ShowStandartTable();
    }
    if(radioButtonIncrements->isChecked()){
        ShowIncrementTable();
    }
}
