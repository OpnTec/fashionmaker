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

DialogTool::~DialogTool(){
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

void DialogTool::PutValHere(QLineEdit *lineEdit, QListWidget *listWidget){
    Q_CHECK_PTR(lineEdit);
    Q_CHECK_PTR(listWidget);
    QListWidgetItem *item = listWidget->currentItem();
    QString val = item->text();
    lineEdit->setText(lineEdit->text().append(val));
}

void DialogTool::ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer *timer){
    Q_CHECK_PTR(edit);
    Q_CHECK_PTR(timer);
    if(edit->text().isEmpty()){
        flag = false;
        CheckState();
        return;
    }
    timer->start(1000);
}

void DialogTool::Eval(QLineEdit *edit, bool &flag, QTimer *timer, QLabel *label){
    Q_CHECK_PTR(edit);
    Q_CHECK_PTR(timer);
    Q_CHECK_PTR(label);
    if(edit->text().isEmpty()){
        flag = false;
    } else {
        Calculator cal(data);
        QString errorMsg;
        qreal result = cal.eval(edit->text(),&errorMsg);
        if(!errorMsg.isEmpty()){
            label->setText("Помилка.");
            flag = false;
        } else {
            label->setText(QString().setNum(result));
            flag = true;
        }
    }
    CheckState();
    timer->stop();
}

void DialogTool::CheckState(){
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagFormula & flagName);
}


void DialogTool::ChoosedObject(qint64 id, Scene::Type type){
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
        ValFormulaChanged(flagFormula, edit, timerFormula);
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
    Eval(lineEditFormula, flagFormula, timerFormula, labelResultCalculation);
}

void DialogTool::SizeGrowth(){
    ShowVariable(data->DataBase());
}

void DialogTool::StandartTable(){
    ShowVariable(data->DataStandartTable());
}

void DialogTool::LengthLines(){
    ShowVariable(data->DataLengthLines());
}

void DialogTool::Increments(){
    ShowVariable(data->DataIncrementTable());
}

void DialogTool::PutHere(){
    PutValHere(lineEditFormula, listWidget);
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
    Q_CHECK_PTR(radioButtonLengthLine);
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
        return;
    }
    if(radioButtonStandartTable->isChecked()){
        VStandartTableCell stable = data->GetStandartTableCell(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueStandartTableCell(item->text()))
                .arg(stable.GetDescription());
        labelDescription->setText(desc);
        return;
    }
    if(radioButtonIncrements->isChecked()){
        VIncrementTableRow itable = data->GetIncrementTableRow(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueIncrementTableRow(item->text()))
                .arg(itable.getDescription());
        labelDescription->setText(desc);
        return;
    }
    if(radioButtonLengthLine->isChecked()){
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLine(item->text()))
                .arg("Довжина лінії");
        labelDescription->setText(desc);
        return;
    }
}

void DialogTool::UpdateList(){
    Q_CHECK_PTR(radioButtonSizeGrowth);
    Q_CHECK_PTR(radioButtonStandartTable);
    Q_CHECK_PTR(radioButtonIncrements);
    if(radioButtonSizeGrowth->isChecked()){
        ShowVariable(data->DataBase());
    }
    if(radioButtonStandartTable->isChecked()){
        ShowVariable(data->DataStandartTable());
    }
    if(radioButtonIncrements->isChecked()){
        ShowVariable(data->DataIncrementTable());
    }
}

export template <class key, class val>
void DialogTool::ShowVariable(const QMap<key, val> *var){
    Q_CHECK_PTR(listWidget);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();
    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    QMapIterator<key, val> i(*var);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        listWidget->addItem(item);
    }
    listWidget->setCurrentRow (0);
}
