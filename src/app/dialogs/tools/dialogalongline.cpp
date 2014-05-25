/************************************************************************
 **
 **  @file   dialogalongline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "dialogalongline.h"
#include "ui_dialogalongline.h"

#include <QPushButton>

//---------------------------------------------------------------------------------------------------------------------
DialogAlongLine::DialogAlongLine(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogAlongLine), number(0), pointName(QString()),
    typeLine(QString()), formula(QString()), firstPointId(0), secondPointId(0)
{
    ui->setupUi(this);
    labelResultCalculation = ui->labelResultCalculation;
    lineEditFormula = ui->lineEditFormula;
    labelEditFormula = ui->labelEditFormula;

    flagFormula = false;
    flagName = false;
    InitOkCancel(ui);
    CheckState();

    FillComboBoxTypeLine(ui->comboBoxLineType);
    ui->comboBoxLineType->setCurrentIndex(0);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);

    labelEditNamePoint = ui->labelEditNamePoint;
    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogAlongLine::PutHere);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogAlongLine::NamePointChanged);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogAlongLine::EvalFormula);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogAlongLine::FormulaChanged);
    InitVariables(ui);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &DialogTool::PutVal);
}

//---------------------------------------------------------------------------------------------------------------------
DialogAlongLine::~DialogAlongLine()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Point)
    {
        if (number == 0)
        {
            if (ChoosedPoint(id, ui->comboBoxFirstPoint, tr("Select second point of line")))
            {
                number++;
                return;
            }
        }
        if (number == 1)
        {
            if (ChoosedPoint(id, ui->comboBoxSecondPoint, ""))
            {
                number = 0;
                if (isInitialized == false)
                {
                    this->show();
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    firstPointId = getCurrentObjectId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentObjectId(ui->comboBoxSecondPoint);
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::setSecondPointId(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::setFirstPointId(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::setFormula(const QString &value)
{
    formula = value;
    ui->lineEditFormula->setText(formula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
