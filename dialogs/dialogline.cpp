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

#include "dialogline.h"
#include "ui_dialogline.h"
#include <QPushButton>
#include <QCloseEvent>

DialogLine::DialogLine(const VContainer *data, Draw::Draws mode, QWidget *parent) :
    DialogTool(data, mode, parent), ui(new Ui::DialogLine), number(0), firstPoint(0), secondPoint(0){
    ui->setupUi(this);
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogLine::DialogAccepted);
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogLine::DialogRejected);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    number = 0;
}

DialogLine::~DialogLine(){
    delete ui;
}

qint64 DialogLine::getSecondPoint() const{
    return secondPoint;
}

void DialogLine::setSecondPoint(const qint64 &value){
    secondPoint = value;
    VPointF point = data->GetPoint(value);
    qint32 index = ui->comboBoxSecondPoint->findText(point.name());
    if(index != -1){
        ui->comboBoxSecondPoint->setCurrentIndex(index);
    }
}

qint64 DialogLine::getFirstPoint() const{
    return firstPoint;
}

void DialogLine::setFirstPoint(const qint64 &value){
    firstPoint = value;
    VPointF point = data->GetPoint(value);
    qint32 index = ui->comboBoxFirstPoint->findText(point.name());
    if(index != -1){
        ui->comboBoxFirstPoint->setCurrentIndex(index);
    }
}


void DialogLine::DialogAccepted(){
    qint32 index = ui->comboBoxFirstPoint->currentIndex();
    firstPoint = qvariant_cast<qint64>(ui->comboBoxFirstPoint->itemData(index));
    index = ui->comboBoxSecondPoint->currentIndex();
    secondPoint = qvariant_cast<qint64>(ui->comboBoxSecondPoint->itemData(index));
    DialogClosed(QDialog::Accepted);
}

void DialogLine::ChoosedObject(qint64 id, Scene::Scenes type){
    if(idDetail == 0 && mode == Draw::Modeling){
        if(type == Scene::Detail){
            idDetail = id;
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
            qint32 index = ui->comboBoxFirstPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxFirstPoint->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 1){
            qint32 index = ui->comboBoxSecondPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxSecondPoint->setCurrentIndex(index);
                number = 0;
            }
            if(!isInitialized){
                this->show();
            }
        }
    }
}
