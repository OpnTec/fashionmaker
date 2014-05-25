/************************************************************************
 **
 **  @file   dialogcutspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#include "dialogcutspline.h"
#include "ui_dialogcutspline.h"

//---------------------------------------------------------------------------------------------------------------------
DialogCutSpline::DialogCutSpline(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogCutSpline), pointName(QString()), formula(QString()), splineId(0)
{
    ui->setupUi(this);
    InitVariables(ui);
    labelResultCalculation = ui->labelResultCalculation;
    lineEditFormula = ui->lineEditFormula;
    labelEditFormula = ui->labelEditFormula;
    labelEditNamePoint = ui->labelEditNamePoint;
    InitOkCancel(ui);

    flagFormula = false;
    flagName = false;
    CheckState();

    FillComboBoxSplines(ui->comboBoxSpline);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogCutSpline::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogCutSpline::PutVal);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogCutSpline::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutSpline::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogCutSpline::FormulaChanged);
}

//---------------------------------------------------------------------------------------------------------------------
DialogCutSpline::~DialogCutSpline()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSpline::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSpline::setFormula(const QString &value)
{
    formula = value;
    ui->lineEditFormula->setText(formula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSpline::setSplineId(const quint32 &value, const quint32 &id)
{
    setCurrentSplineId(ui->comboBoxSpline, splineId, value, id, ComboMode::CutSpline);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSpline::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Spline)
    {
        const VSpline *spl = data->GeometricObject<const VSpline *>(id);
        ChangeCurrentText(ui->comboBoxSpline, spl->name());
        emit ToolTip("");
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSpline::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->lineEditFormula->text();
    splineId = getCurrentObjectId(ui->comboBoxSpline);
    emit DialogClosed(QDialog::Accepted);
}
