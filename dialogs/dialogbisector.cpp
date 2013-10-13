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

#include "dialogbisector.h"
#include "ui_dialogbisector.h"

DialogBisector::DialogBisector(const VContainer *data, Draw::Draws mode, QWidget *parent) :
    DialogTool(data, mode, parent), ui(new Ui::DialogBisector), number(0), pointName(QString()),
    typeLine(QString()), formula(QString()), firstPointId(0), secondPointId(0), thirdPointId(0){
    ui->setupUi(this);
    listWidget = ui->listWidget;
    labelResultCalculation = ui->labelResultCalculation;
    labelDescription = ui->labelDescription;
    radioButtonSizeGrowth = ui->radioButtonSizeGrowth;
    radioButtonStandartTable = ui->radioButtonStandartTable;
    radioButtonIncrements = ui->radioButtonIncrements;
    radioButtonLengthLine = ui->radioButtonLengthLine;
    radioButtonLengthArc = ui->radioButtonLengthArc;
    radioButtonLengthCurve = ui->radioButtonLengthSpline;
    lineEditFormula = ui->lineEditFormula;
    labelEditFormula = ui->labelEditFormula;
    labelEditNamePoint = ui->labelEditNamePoint;
    flagFormula = false;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogBisector::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogBisector::DialogRejected);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxPoints(ui->comboBoxThirdPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogBisector::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogBisector::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogBisector::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogBisector::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogBisector::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogBisector::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogBisector::LengthLines);
    connect(ui->radioButtonLengthArc, &QRadioButton::clicked, this, &DialogBisector::LengthArcs);
    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogBisector::LengthCurves);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogBisector::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogBisector::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogBisector::FormulaChanged);
}

DialogBisector::~DialogBisector(){
    delete ui;
}

void DialogBisector::ChoosedObject(qint64 id, Scene::Scenes type){
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
                emit ToolTip(tr("Select second point of angle"));
                return;
            }
        }
        if(number == 1){
            qint32 index = ui->comboBoxSecondPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxSecondPoint->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select third point of angle"));
                return;
            }
        }
        if(number == 2){
            qint32 index = ui->comboBoxThirdPoint->findText(point.name());
            if ( index != -1 ) { // -1 for not found
                ui->comboBoxThirdPoint->setCurrentIndex(index);
                number = 0;
                emit ToolTip("");
            }
            if(!isInitialized){
                this->show();
            }
        }
    }
}

QString DialogBisector::getPointName() const{
    return pointName;
}

void DialogBisector::setPointName(const QString &value){
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

QString DialogBisector::getTypeLine() const{
    return typeLine;
}

void DialogBisector::setTypeLine(const QString &value){
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

QString DialogBisector::getFormula() const{
    return formula;
}

void DialogBisector::setFormula(const QString &value){
    formula = value;
    ui->lineEditFormula->setText(formula);
}

qint64 DialogBisector::getFirstPointId() const{
    return firstPointId;
}

void DialogBisector::setFirstPointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

qint64 DialogBisector::getSecondPointId() const{
    return secondPointId;
}

void DialogBisector::setSecondPointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

qint64 DialogBisector::getThirdPointId() const{
    return thirdPointId;
}

void DialogBisector::setThirdPointId(const qint64 &value, const qint64 &id){
    setCurrentPointId(ui->comboBoxThirdPoint, thirdPointId, value, id);
}

void DialogBisector::DialogAccepted(){
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    firstPointId = getCurrentPointId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentPointId(ui->comboBoxSecondPoint);
    thirdPointId = getCurrentPointId(ui->comboBoxThirdPoint);
    emit DialogClosed(QDialog::Accepted);
}
