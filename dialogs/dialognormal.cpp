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

#include "dialognormal.h"
#include "ui_dialognormal.h"
#include <QMenu>

DialogNormal::DialogNormal(const VContainer *data, Draw::Draws mode, QWidget *parent) :
    DialogTool(data, mode, parent), ui(new Ui::DialogNormal), number(0), pointName(QString()),
    typeLine(QString()), formula(QString()), angle(0), firstPointId(0), secondPointId(0){
    ui->setupUi(this);
    spinBoxAngle = ui->doubleSpinBoxAngle;
    listWidget = ui->listWidget;
    labelResultCalculation = ui->labelResultCalculation;
    labelDescription = ui->labelDescription;
    radioButtonSizeGrowth = ui->radioButtonSizeGrowth;
    radioButtonStandartTable = ui->radioButtonStandartTable;
    radioButtonIncrements = ui->radioButtonIncrements;
    radioButtonLengthLine = ui->radioButtonLengthLine;
    lineEditFormula = ui->lineEditFormula;
    flagFormula = false;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogNormal::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogNormal::DialogRejected);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    connect(ui->toolButtonArrowDown, &QPushButton::clicked, this,
            &DialogNormal::ArrowDown);
    connect(ui->toolButtonArrowUp, &QPushButton::clicked, this,
            &DialogNormal::ArrowUp);
    connect(ui->toolButtonArrowLeft, &QPushButton::clicked, this,
            &DialogNormal::ArrowLeft);
    connect(ui->toolButtonArrowRight, &QPushButton::clicked, this,
            &DialogNormal::ArrowRight);
    connect(ui->toolButtonArrowLeftUp, &QPushButton::clicked, this,
            &DialogNormal::ArrowLeftUp);
    connect(ui->toolButtonArrowLeftDown, &QPushButton::clicked, this,
            &DialogNormal::ArrowLeftDown);
    connect(ui->toolButtonArrowRightUp, &QPushButton::clicked, this,
            &DialogNormal::ArrowRightUp);
    connect(ui->toolButtonArrowRightDown, &QPushButton::clicked, this,
            &DialogNormal::ArrowRightDown);
    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogNormal::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogNormal::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogNormal::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogNormal::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogNormal::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogNormal::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogNormal::LengthLines);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogNormal::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogNormal::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogNormal::FormulaChanged);
}

DialogNormal::~DialogNormal()
{
    delete ui;
}

void DialogNormal::ChoosedObject(qint64 id, Scene::Scenes type){
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

void DialogNormal::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    angle = ui->doubleSpinBoxAngle->value();
    firstPointId = getCurrentPointId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentPointId(ui->comboBoxSecondPoint);
    emit DialogClosed(QDialog::Accepted);
}

qint64 DialogNormal::getSecondPointId() const{
    return secondPointId;
}

void DialogNormal::setSecondPointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

qint64 DialogNormal::getFirstPointId() const{
    return firstPointId;
}

void DialogNormal::setFirstPointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

qreal DialogNormal::getAngle() const{
    return angle;
}

void DialogNormal::setAngle(const qreal &value){
    angle = value;
    ui->doubleSpinBoxAngle->setValue(angle);
}

QString DialogNormal::getFormula() const{
    return formula;
}

void DialogNormal::setFormula(const QString &value){
    formula = value;
    ui->lineEditFormula->setText(formula);
}

QString DialogNormal::getTypeLine() const{
    return typeLine;
}

void DialogNormal::setTypeLine(const QString &value){
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

QString DialogNormal::getPointName() const{
    return pointName;
}

void DialogNormal::setPointName(const QString &value){
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
