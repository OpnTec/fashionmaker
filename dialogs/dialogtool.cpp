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
    radioButtonLengthLine(0), radioButtonLengthArc(0), radioButtonLengthCurve(0), idDetail(0), mode(mode){
    Q_ASSERT(data != 0);
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
    Q_ASSERT(box != 0);
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
    } else {
        qWarning()<<tr("Can't find point by name")<<value;
    }
}

void DialogTool::ChangeCurrentData(QComboBox *box, const qint64 &value) const{
    qint32 index = box->findData(value);
    if(index != -1){
        box->setCurrentIndex(index);
    }
}

void DialogTool::PutValHere(QLineEdit *lineEdit, QListWidget *listWidget){
    Q_ASSERT(lineEdit != 0);
    Q_ASSERT(listWidget != 0);
    QListWidgetItem *item = listWidget->currentItem();
    int pos = lineEdit->cursorPosition();
    lineEdit->setText(lineEdit->text().insert(lineEdit->cursorPosition(), item->text()));
    lineEdit->setFocus();
    lineEdit->setCursorPosition(pos + item->text().size());
}

void DialogTool::ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer *timer){
    Q_ASSERT(edit != 0);
    Q_ASSERT(timer != 0);
    Q_ASSERT(labelEditFormula != 0);
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
    Q_ASSERT(edit != 0);
    Q_ASSERT(timer != 0);
    Q_ASSERT(label != 0);
    Q_ASSERT(labelEditFormula != 0);
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
    Q_ASSERT(box != 0);
    FillComboBoxPoints(box, id);
    pointId = value;
    ChangeCurrentData(box, value);
}

qint64 DialogTool::getCurrentPointId(QComboBox *box) const{
    Q_ASSERT(box != 0);
    qint32 index = box->currentIndex();
    Q_ASSERT(index != -1);
    if(index != -1){
        return qvariant_cast<qint64>(box->itemData(index));
    } else {
        return -1;
    }
}

void DialogTool::CheckState(){
    Q_ASSERT(bOk != 0);
    bOk->setEnabled(flagFormula && flagName);
}


void DialogTool::ChoosedObject(qint64 id, Scene::Scenes type){
    Q_UNUSED(id);
    Q_UNUSED(type);
}

void DialogTool::NamePointChanged(){
    Q_ASSERT(labelEditNamePoint != 0);
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
    Q_ASSERT(spinBoxAngle != 0);
    spinBoxAngle->setValue(90);
}

void DialogTool::ArrowDown(){
    Q_ASSERT(spinBoxAngle != 0);
    spinBoxAngle->setValue(270);
}

void DialogTool::ArrowLeft(){
    Q_ASSERT(spinBoxAngle != 0);
    spinBoxAngle->setValue(180);
}

void DialogTool::ArrowRight(){
    Q_ASSERT(spinBoxAngle != 0);
    spinBoxAngle->setValue(0);
}

void DialogTool::ArrowLeftUp(){
    Q_ASSERT(spinBoxAngle != 0);
    spinBoxAngle->setValue(135);
}

void DialogTool::ArrowLeftDown(){
    Q_ASSERT(spinBoxAngle != 0);
    spinBoxAngle->setValue(225);
}

void DialogTool::ArrowRightUp(){
    Q_ASSERT(spinBoxAngle != 0);
    spinBoxAngle->setValue(45);
}

void DialogTool::ArrowRightDown(){
    Q_ASSERT(spinBoxAngle != 0);
    spinBoxAngle->setValue(315);
}

void DialogTool::EvalFormula(){
    Q_ASSERT(lineEditFormula != 0);
    Q_ASSERT(labelResultCalculation != 0);
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

void DialogTool::LengthArcs(){
    ShowVariable(data->DataLengthArcs());
}

void DialogTool::LengthCurves(){
    ShowVariable(data->DataLengthSplines());
}

void DialogTool::Increments(){
    ShowVariable(data->DataIncrementTable());
}

void DialogTool::PutHere(){
    PutValHere(lineEditFormula, listWidget);
}

void DialogTool::PutVal(QListWidgetItem *item){
    Q_ASSERT(lineEditFormula != 0);
    Q_ASSERT(item != 0);
    int pos = lineEditFormula->cursorPosition();
    lineEditFormula->setText(lineEditFormula->text().insert(lineEditFormula->cursorPosition(),
                                                            item->text()));
    lineEditFormula->setFocus();
    lineEditFormula->setCursorPosition(pos + item->text().size());
}

void DialogTool::ValChenged(int row){
    Q_ASSERT(listWidget != 0);
    Q_ASSERT(labelDescription != 0);
    Q_ASSERT(radioButtonSizeGrowth != 0);
    Q_ASSERT(radioButtonStandartTable != 0);
    Q_ASSERT(radioButtonIncrements != 0);
    Q_ASSERT(radioButtonLengthLine != 0);
    Q_ASSERT(radioButtonLengthArc != 0);
    Q_ASSERT(radioButtonLengthCurve != 0);
    if(listWidget->count() == 0){
        return;
    }
    QListWidgetItem *item = listWidget->item( row );
    if(radioButtonSizeGrowth->isChecked()){
        if(item->text()=="Р"){
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->growth()).arg(tr("Growth"));
            labelDescription->setText(desc);
        }
        if(item->text()=="Сг"){
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->size()).arg(tr("Size"));
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
    if(radioButtonLengthArc->isChecked()){
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLengthArc(item->text()))
                .arg(tr("Arc length"));
        labelDescription->setText(desc);
        return;
    }
    if(radioButtonLengthCurve->isChecked()){
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLengthSpline(item->text()))
                .arg(tr("Curve length"));
        labelDescription->setText(desc);
        return;
    }
}

void DialogTool::UpdateList(){
    Q_ASSERT(radioButtonSizeGrowth != 0);
    Q_ASSERT(radioButtonStandartTable != 0);
    Q_ASSERT(radioButtonIncrements != 0);
    Q_ASSERT(radioButtonLengthLine != 0);
    Q_ASSERT(radioButtonLengthArc != 0);
    Q_ASSERT(radioButtonLengthCurve != 0);

    if(radioButtonSizeGrowth->isChecked()){
        ShowVariable(data->DataBase());
    }
    if(radioButtonStandartTable->isChecked()){
        ShowVariable(data->DataStandartTable());
    }
    if(radioButtonIncrements->isChecked()){
        ShowVariable(data->DataIncrementTable());
    }
    if(radioButtonLengthLine->isChecked()){
        ShowVariable(data->DataLengthLines());
    }
    if(radioButtonLengthArc->isChecked()){
        ShowVariable(data->DataLengthArcs());
    }
    if(radioButtonLengthCurve->isChecked()){
        ShowVariable(data->DataLengthSplines());
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
    Q_ASSERT(listWidget != 0);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();

    QHashIterator<key, val> i(*var);
    QMap<key, val> map;
    while (i.hasNext()) {
        i.next();
        map.insert(i.key(), i.value());
    }

    QMapIterator<key, val> iMap(map);
    while (iMap.hasNext()) {
        iMap.next();
        QListWidgetItem *item = new QListWidgetItem(iMap.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        listWidget->addItem(item);
    }
    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->setCurrentRow (0);
}

qint64 DialogTool::getIdDetail() const{
    return idDetail;
}

void DialogTool::setIdDetail(const qint64 &value){
    idDetail = value;
}
