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
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "dialogshoulderpoint.h"
#include "ui_dialogshoulderpoint.h"

DialogShoulderPoint::DialogShoulderPoint(const VContainer *data, Draw::Draws mode, QWidget *parent)
    :DialogTool(data, mode, parent), ui(new Ui::DialogShoulderPoint), number(0), pointName(QString()),
    typeLine(QString()), formula(QString()), p1Line(0), p2Line(0), pShoulder(0)
{
    ui->setupUi(this);
    number = 0;
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
    connect(bOk, &QPushButton::clicked, this, &DialogShoulderPoint::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogShoulderPoint::DialogRejected);
    FillComboBoxTypeLine(ui->comboBoxLineType);
    FillComboBoxPoints(ui->comboBoxP1Line);
    FillComboBoxPoints(ui->comboBoxP2Line);
    FillComboBoxPoints(ui->comboBoxPShoulder);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogShoulderPoint::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogShoulderPoint::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogShoulderPoint::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogShoulderPoint::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogShoulderPoint::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogShoulderPoint::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogShoulderPoint::LengthLines);
    connect(ui->radioButtonLengthArc, &QRadioButton::clicked, this, &DialogShoulderPoint::LengthArcs);
    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogShoulderPoint::LengthCurves);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogShoulderPoint::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogShoulderPoint::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogShoulderPoint::FormulaChanged);
}

DialogShoulderPoint::~DialogShoulderPoint()
{
    delete ui;
}

void DialogShoulderPoint::ChoosedObject(qint64 id, Scene::Scenes type)
{
    if (idDetail == 0 && mode == Draw::Modeling)
    {
        if (type == Scene::Detail)
        {
            idDetail = id;
            return;
        }
    }
    if (mode == Draw::Modeling)
    {
        if (CheckObject(id) == false)
        {
            return;
        }
    }
    if (type == Scene::Point)
    {
        VPointF point;
        if (mode == Draw::Calculation)
        {
            point = data->GetPoint(id);
        }
        else
        {
            point = data->GetModelingPoint(id);
        }
        if (number == 0)
        {
            qint32 index = ui->comboBoxP1Line->findText(point.name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP1Line->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select second point of line"));
                return;
            }
        }
        if (number == 1)
        {
            qint32 index = ui->comboBoxP2Line->findText(point.name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxP2Line->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select point of shoulder"));
                return;
            }
        }
        if (number == 2)
        {
            qint32 index = ui->comboBoxPShoulder->findText(point.name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxPShoulder->setCurrentIndex(index);
                number = 0;
                emit ToolTip("");
            }
            if (isInitialized == false)
            {
                this->show();
            }
        }
    }
}

void DialogShoulderPoint::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    p1Line = getCurrentPointId(ui->comboBoxP1Line);
    p2Line = getCurrentPointId(ui->comboBoxP2Line);
    pShoulder = getCurrentPointId(ui->comboBoxPShoulder);
    emit DialogClosed(QDialog::Accepted);
}

void DialogShoulderPoint::setPShoulder(const qint64 &value, const qint64 &id)
{
    setCurrentPointId(ui->comboBoxPShoulder, pShoulder, value, id);
}

void DialogShoulderPoint::setP2Line(const qint64 &value, const qint64 &id)
{
    setCurrentPointId(ui->comboBoxP2Line, p2Line, value, id);
}

void DialogShoulderPoint::setP1Line(const qint64 &value, const qint64 &id)
{
    setCurrentPointId(ui->comboBoxP1Line, p1Line, value, id);
}

void DialogShoulderPoint::setFormula(const QString &value)
{
    formula = value;
    ui->lineEditFormula->setText(formula);
}

void DialogShoulderPoint::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

void DialogShoulderPoint::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
