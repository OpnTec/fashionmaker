/************************************************************************
 **
 **  @file   dialogbisector.cpp
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

#include "dialogbisector.h"
#include "ui_dialogbisector.h"

#include <QPushButton>

DialogBisector::DialogBisector(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogBisector), number(0), pointName(QString()),
    typeLine(QString()), formula(QString()), firstPointId(0), secondPointId(0), thirdPointId(0)
{
    ui->setupUi(this);
    InitVariables(ui);
    labelResultCalculation = ui->labelResultCalculation;
    lineEditFormula = ui->lineEditFormula;
    labelEditFormula = ui->labelEditFormula;
    labelEditNamePoint = ui->labelEditNamePoint;
    InitOkCansel(ui);
    flagFormula = false;
    flagName = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxPoints(ui->comboBoxThirdPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogBisector::PutHere);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &DialogBisector::PutVal);

    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogBisector::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogBisector::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogBisector::FormulaChanged);
}

DialogBisector::~DialogBisector()
{
    delete ui;
}

void DialogBisector::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        if (number == 0)
        {
            qint32 index = ui->comboBoxFirstPoint->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxFirstPoint->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select second point of angle"));
                return;
            }
        }
        if (number == 1)
        {
            qint32 index = ui->comboBoxSecondPoint->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxSecondPoint->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select third point of angle"));
                return;
            }
        }
        if (number == 2)
        {
            qint32 index = ui->comboBoxThirdPoint->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxThirdPoint->setCurrentIndex(index);
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

void DialogBisector::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

void DialogBisector::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

void DialogBisector::setFormula(const QString &value)
{
    formula = value;
    ui->lineEditFormula->setText(formula);
}

void DialogBisector::setFirstPointId(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

void DialogBisector::setSecondPointId(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

void DialogBisector::setThirdPointId(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxThirdPoint, thirdPointId, value, id);
}

void DialogBisector::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    firstPointId = getCurrentObjectId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentObjectId(ui->comboBoxSecondPoint);
    thirdPointId = getCurrentObjectId(ui->comboBoxThirdPoint);
    emit DialogClosed(QDialog::Accepted);
}
