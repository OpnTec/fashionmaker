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

#include "dialogspline.h"
#include "ui_dialogspline.h"

DialogSpline::DialogSpline(const VContainer *data, Draw::Mode mode, QWidget *parent) :
    DialogTool(data, mode, parent), ui(new Ui::DialogSpline), number(0), p1(0), p4(0), angle1(0), angle2(0),
    kAsm1(1), kAsm2(1), kCurve(1){
    ui->setupUi(this);
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogSpline::DialogAccepted);

    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogSpline::DialogRejected);
    if(mode == Draw::Calculation){
        FillComboBoxPoints(ui->comboBoxP1);
        FillComboBoxPoints(ui->comboBoxP4);
    }
}

DialogSpline::~DialogSpline(){
    delete ui;
}

void DialogSpline::ChoosedObject(qint64 id, Scene::Type type){
    if(idDetail == 0 && mode == Draw::Modeling){
        if(type == Scene::Detail){
            idDetail = id;
            FillComboBoxPoints(ui->comboBoxP1);
            FillComboBoxPoints(ui->comboBoxP4);
            return;
        }
    }
    if(mode == Draw::Modeling){
        if(!CheckObject(id)){
            return;
        }
    }
    if(type == Scene::Point){
        VPointF point;
        if(mode == Draw::Calculation){
            point = data->GetPoint(id);
        } else {
            point = data->GetModelingPoint(id);
        }
        if(number == 0){
            qint32 index = ui->comboBoxP1->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxP1->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 1){
            qint32 index = ui->comboBoxP4->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxP4->setCurrentIndex(index);
                number = 0;
                index = ui->comboBoxP1->currentIndex();
                qint64 p1Id = qvariant_cast<qint64>(ui->comboBoxP1->itemData(index));
                QPointF p1;
                QPointF p4;
                if(mode == Draw::Calculation){
                    p1 = data->GetPoint(p1Id).toQPointF();
                    p4 = data->GetPoint(id).toQPointF();
                } else {
                    p1 = data->GetModelingPoint(p1Id).toQPointF();
                    p4 = data->GetModelingPoint(id).toQPointF();
                }
                ui->spinBoxAngle1->setValue(static_cast<qint32>(QLineF(p1, p4).angle()));
                ui->spinBoxAngle2->setValue(static_cast<qint32>(QLineF(p4, p1).angle()));
            }
            if(!isInitialized){
                this->show();
            }
        }
    }
}

void DialogSpline::DialogAccepted(){
    p1 = getCurrentPointId(ui->comboBoxP1);
    p4 = getCurrentPointId(ui->comboBoxP4);
    angle1 = ui->spinBoxAngle1->value();
    angle2 = ui->spinBoxAngle2->value();
    kAsm1 = ui->doubleSpinBoxKasm1->value();
    kAsm2 = ui->doubleSpinBoxKasm2->value();
    kCurve = ui->doubleSpinBoxKcurve->value();
    emit DialogClosed(QDialog::Accepted);
}

qreal DialogSpline::getKCurve() const{
    return kCurve;
}

void DialogSpline::setKCurve(const qreal &value){
    kCurve = value;
    ui->doubleSpinBoxKcurve->setValue(value);
}

qreal DialogSpline::getKAsm2() const{
    return kAsm2;
}

void DialogSpline::setKAsm2(const qreal &value){
    kAsm2 = value;
    ui->doubleSpinBoxKasm2->setValue(value);
}

qreal DialogSpline::getKAsm1() const{
    return kAsm1;
}

void DialogSpline::setKAsm1(const qreal &value){
    kAsm1 = value;
    ui->doubleSpinBoxKasm1->setValue(value);
}

qreal DialogSpline::getAngle2() const{
    return angle2;
}

void DialogSpline::setAngle2(const qreal &value){
    angle2 = value;
    ui->spinBoxAngle2->setValue(static_cast<qint32>(value));
}

qreal DialogSpline::getAngle1() const{
    return angle1;
}

void DialogSpline::setAngle1(const qreal &value){
    angle1 = value;
    ui->spinBoxAngle1->setValue(static_cast<qint32>(value));
}

qint64 DialogSpline::getP4() const{
    return p4;
}

void DialogSpline::setP4(const qint64 &value){
    p4 = value;
    ChangeCurrentData(ui->comboBoxP4, value);
}

qint64 DialogSpline::getP1() const{
    return p1;
}

void DialogSpline::setP1(const qint64 &value){
    p1 = value;
    ChangeCurrentData(ui->comboBoxP1, value);
}
