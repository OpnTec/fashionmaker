/************************************************************************
 **
 **  @file   dialogshoulderpoint.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
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
 *************************************************************************/

#include "dialogshoulderpoint.h"
#include "ui_dialogshoulderpoint.h"

#include <QPushButton>

DialogShoulderPoint::DialogShoulderPoint(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogShoulderPoint), number(0), pointName(QString()),
    typeLine(QString()), formula(QString()), p1Line(0), p2Line(0), pShoulder(0)
{
    ui->setupUi(this);
    number = 0;
    InitVariables(ui);
    labelResultCalculation = ui->labelResultCalculation;
    lineEditFormula = ui->lineEditFormula;
    labelEditFormula = ui->labelEditFormula;
    labelEditNamePoint = ui->labelEditNamePoint;
    InitOkCansel(ui);

    flagFormula = false;
    flagName = false;
    CheckState();

    FillComboBoxTypeLine(ui->comboBoxLineType);
    FillComboBoxPoints(ui->comboBoxP1Line);
    FillComboBoxPoints(ui->comboBoxP2Line);
    FillComboBoxPoints(ui->comboBoxPShoulder);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogShoulderPoint::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogShoulderPoint::PutVal);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogShoulderPoint::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogShoulderPoint::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogShoulderPoint::FormulaChanged);
}

DialogShoulderPoint::~DialogShoulderPoint()
{
    delete ui;
}

void DialogShoulderPoint::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        if (number == 0)
        {
            qint32 index = ui->comboBoxP1Line->findText(point->name());
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
            qint32 index = ui->comboBoxP2Line->findText(point->name());
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
            qint32 index = ui->comboBoxPShoulder->findText(point->name());
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
    p1Line = getCurrentObjectId(ui->comboBoxP1Line);
    p2Line = getCurrentObjectId(ui->comboBoxP2Line);
    pShoulder = getCurrentObjectId(ui->comboBoxPShoulder);
    emit DialogClosed(QDialog::Accepted);
}

void DialogShoulderPoint::setPShoulder(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxPShoulder, pShoulder, value, id);
}

void DialogShoulderPoint::setP2Line(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxP2Line, p2Line, value, id);
}

void DialogShoulderPoint::setP1Line(const quint32 &value, const quint32 &id)
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
