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

#include "dialogpointofcontact.h"

DialogPointOfContact::DialogPointOfContact(const VContainer *data, Draw::Draws mode, QWidget *parent) :
    DialogTool(data, mode, parent), ui(), number(0), pointName(QString()), radius(QString()), center(0),
    firstPoint(0), secondPoint(0){
    ui.setupUi(this);
    listWidget = ui.listWidget;
    labelResultCalculation = ui.labelResultCalculation;
    labelDescription = ui.labelDescription;
    radioButtonSizeGrowth = ui.radioButtonSizeGrowth;
    radioButtonStandartTable = ui.radioButtonStandartTable;
    radioButtonIncrements = ui.radioButtonIncrements;
    radioButtonLengthLine = ui.radioButtonLengthLine;
    lineEditFormula = ui.lineEditFormula;
    labelEditFormula = ui.labelEditFormula;
    labelEditNamePoint = ui.labelEditNamePoint;
    flagFormula = false;
    bOk = ui.buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogPointOfContact::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui.buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogPointOfContact::DialogRejected);
    FillComboBoxPoints(ui.comboBoxCenter);
    FillComboBoxPoints(ui.comboBoxFirstPoint);
    FillComboBoxPoints(ui.comboBoxSecondPoint);

    connect(ui.toolButtonPutHere, &QPushButton::clicked, this, &DialogPointOfContact::PutHere);
    connect(ui.listWidget, &QListWidget::itemDoubleClicked, this, &DialogPointOfContact::PutVal);
    connect(ui.listWidget, &QListWidget::currentRowChanged, this, &DialogPointOfContact::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui.radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogPointOfContact::SizeGrowth);
    connect(ui.radioButtonStandartTable, &QRadioButton::clicked, this, &DialogPointOfContact::StandartTable);
    connect(ui.radioButtonIncrements, &QRadioButton::clicked, this, &DialogPointOfContact::Increments);
    connect(ui.radioButtonLengthLine, &QRadioButton::clicked, this, &DialogPointOfContact::LengthLines);
    connect(ui.toolButtonEqual, &QPushButton::clicked, this, &DialogPointOfContact::EvalFormula);
    connect(ui.lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfContact::NamePointChanged);
    connect(ui.lineEditFormula, &QLineEdit::textChanged, this, &DialogPointOfContact::FormulaChanged);
}

void DialogPointOfContact::ChoosedObject(qint64 id, Scene::Scenes type){
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
            qint32 index = ui.comboBoxFirstPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui.comboBoxFirstPoint->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 1){
            qint32 index = ui.comboBoxSecondPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui.comboBoxSecondPoint->setCurrentIndex(index);
                number++;
                return;
            }
        }
        if(number == 2){
            qint32 index = ui.comboBoxCenter->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui.comboBoxCenter->setCurrentIndex(index);
                number = 0;
            }
            if(!isInitialized){
                this->show();
            }
        }
    }
}


void DialogPointOfContact::DialogAccepted(){
    pointName = ui.lineEditNamePoint->text();
    radius = ui.lineEditFormula->text();
    center = getCurrentPointId(ui.comboBoxCenter);
    firstPoint = getCurrentPointId(ui.comboBoxFirstPoint);
    secondPoint = getCurrentPointId(ui.comboBoxSecondPoint);
    emit DialogClosed(QDialog::Accepted);
}

qint64 DialogPointOfContact::getSecondPoint() const{
    return secondPoint;
}

void DialogPointOfContact::setSecondPoint(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui.comboBoxSecondPoint, secondPoint, value, id);
}

qint64 DialogPointOfContact::getFirstPoint() const{
    return firstPoint;
}

void DialogPointOfContact::setFirstPoint(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui.comboBoxFirstPoint, firstPoint, value, id);
}

qint64 DialogPointOfContact::getCenter() const{
    return center;
}

void DialogPointOfContact::setCenter(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui.comboBoxCenter, center, value, id);
    center = value;
}

QString DialogPointOfContact::getRadius() const{
    return radius;
}

void DialogPointOfContact::setRadius(const QString &value){
    radius = value;
    ui.lineEditFormula->setText(radius);
}

QString DialogPointOfContact::getPointName() const{
    return pointName;
}

void DialogPointOfContact::setPointName(const QString &value){
    pointName = value;
    ui.lineEditNamePoint->setText(pointName);
}
