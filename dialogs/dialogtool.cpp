/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "dialogtool.h"
#include <QListWidgetItem>
#include <QCloseEvent>
#include "container/calculator.h"
#include "geometry/vdetail.h"
#include <QDebug>

DialogTool::DialogTool(const VContainer *data, Draw::Draws mode, QWidget *parent):QDialog(parent), data(data),
    isInitialized(false), flagName(true), flagFormula(true), timerFormula(0), bOk(0), spinBoxAngle(0),
    lineEditFormula(0), listWidget(0), labelResultCalculation(0), labelDescription(0), labelEditNamePoint(0),
    labelEditFormula(0), radioButtonSizeGrowth(0), radioButtonStandartTable(0), radioButtonIncrements(0),
    radioButtonLengthLine(0), idDetail(0), mode(mode){
    Q_CHECK_PTR(data);
    timerFormula = new QTimer(this);
    connect(timerFormula, &QTimer::timeout, this, &DialogTool::EvalFormula);
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
    isInitialized = true;//first show windows are held
}

void DialogTool::FillComboBoxPoints(QComboBox *box, const qint64 &id) const{
    box->clear();
    if(mode == Draw::Calculation){
        const QHash<qint64, VPointF> *points = data->DataPoints();
        QHashIterator<qint64, VPointF> i(*points);
        while (i.hasNext()) {
            i.next();
            if(i.key() != id){
                VPointF point = i.value();
                box->addItem(point.name(), i.key());
            }
        }
    } else {
        if(idDetail <= 0){
            qWarning()<<tr("Wrong details id.")<<Q_FUNC_INFO;
            return;
        }
        VDetail det = data->GetDetail(idDetail);
        for(qint32 i = 0; i< det.CountNode(); ++i){
            if(det[i].getTypeTool() == Tool::NodePoint ||
               det[i].getTypeTool() == Tool::AlongLineTool ||
               det[i].getTypeTool() == Tool::BisectorTool ||
               det[i].getTypeTool() == Tool::EndLineTool ||
               det[i].getTypeTool() == Tool::LineIntersectTool ||
               det[i].getTypeTool() == Tool::NormalTool ||
               det[i].getTypeTool() == Tool::PointOfContact ||
               det[i].getTypeTool() == Tool::ShoulderPointTool){
                if(det[i].getId() != id){
                    VPointF point = data->GetModelingPoint(det[i].getId());
                    box->addItem(point.name(), det[i].getId());
                }
            }
        }
    }
}

void DialogTool::FillComboBoxTypeLine(QComboBox *box) const{
    QStringList list;
    list<<tr("Line")<<tr("No line");
    box->addItems(list);
}

QString DialogTool::GetTypeLine(const QComboBox *box) const{
    if(box->currentText()==tr("Line")){
        return QString("hair");
    } else {
        return QString("none");
    }
}

void DialogTool::SetupTypeLine(QComboBox *box, const QString &value){
    if(value == "hair"){
        qint32 index = box->findText(tr("Line"));
        if(index != -1){
            box->setCurrentIndex(index);
        }
    }
    if(value == "none"){
        qint32 index = box->findText(tr("No line"));
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

void DialogTool::ChangeCurrentData(QComboBox *box, const qint64 &value) const{
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
    lineEdit->setFocus();
}

void DialogTool::ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer *timer){
    Q_CHECK_PTR(edit);
    Q_CHECK_PTR(timer);
    Q_CHECK_PTR(labelEditFormula);
    if(edit->text().isEmpty()){
        flag = false;
        CheckState();
        QPalette palette = labelEditFormula->palette();
        palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
        labelEditFormula->setPalette(palette);
        return;
    }
    timer->start(1000);
}

void DialogTool::Eval(QLineEdit *edit, bool &flag, QTimer *timer, QLabel *label){
    Q_CHECK_PTR(edit);
    Q_CHECK_PTR(timer);
    Q_CHECK_PTR(label);
    Q_CHECK_PTR(labelEditFormula);
    QPalette palette = labelEditFormula->palette();
    if(edit->text().isEmpty()){
        flag = false;
        palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
    } else {
        Calculator cal(data);
        QString errorMsg;
        qreal result = cal.eval(edit->text(),&errorMsg);
        if(!errorMsg.isEmpty()){
            label->setText(tr("Error"));
            flag = false;
            palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
        } else {
            label->setText(QString().setNum(result));
            flag = true;
            palette.setColor(labelEditFormula->foregroundRole(), QColor(76,76,76));
        }
    }
    CheckState();
    timer->stop();
    labelEditFormula->setPalette(palette);
}

void DialogTool::setCurrentPointId(QComboBox *box, qint64 &pointId, const qint64 &value,
                                   const qint64 &id) const{
    Q_CHECK_PTR(box);
    FillComboBoxPoints(box, id);
    pointId = value;
    ChangeCurrentData(box, value);
}

qint64 DialogTool::getCurrentPointId(QComboBox *box) const{
    Q_CHECK_PTR(box);
    qint32 index = box->currentIndex();
    Q_ASSERT(index != -1);
    if(index != -1){
        return qvariant_cast<qint64>(box->itemData(index));
    } else {
        return -1;
    }
}

void DialogTool::CheckState(){
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagFormula && flagName);
}


void DialogTool::ChoosedObject(qint64 id, Scene::Scenes type){
    Q_UNUSED(id);
    Q_UNUSED(type);
}

void DialogTool::NamePointChanged(){
    Q_CHECK_PTR(labelEditNamePoint);
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit){
        QString name = edit->text();
        if(name.isEmpty() || name.contains(" ")){
            flagName = false;
            QPalette palette = labelEditNamePoint->palette();
            palette.setColor(labelEditNamePoint->foregroundRole(), Qt::red);
            labelEditNamePoint->setPalette(palette);
        } else {
            flagName = true;
            QPalette palette = labelEditNamePoint->palette();
            palette.setColor(labelEditNamePoint->foregroundRole(), QColor(76,76,76));
            labelEditNamePoint->setPalette(palette);
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
    lineEditFormula->setFocus();
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
                .arg(tr("Line length"));
        labelDescription->setText(desc);
        return;
    }
}

void DialogTool::UpdateList(){
    if(radioButtonSizeGrowth == 0 || radioButtonStandartTable == 0 || radioButtonIncrements == 0){
        return;
    }
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

bool DialogTool::CheckObject(const qint64 &id){
    if(mode == Draw::Calculation || idDetail == 0){
        return false;
    }
    VDetail det = data->GetDetail(idDetail);
    return det.Containes(id);
}

template <class key, class val>
void DialogTool::ShowVariable(const QHash<key, val> *var){
    Q_CHECK_PTR(listWidget);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();
    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    QHashIterator<key, val> i(*var);
    while (i.hasNext()) {
        i.next();
        QListWidgetItem *item = new QListWidgetItem(i.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        listWidget->addItem(item);
    }
    listWidget->setCurrentRow (0);
}

qint64 DialogTool::getIdDetail() const{
    return idDetail;
}

void DialogTool::setIdDetail(const qint64 &value){
    idDetail = value;
}
