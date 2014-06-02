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
/**
 * @brief DialogCutSpline create dialog.
 * @param data container with data
 * @param parent parent widget
 */
DialogCutSpline::DialogCutSpline(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogCutSpline), pointName(QString()), formula(QString()), splineId(0)
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
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogCutSpline::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string of formula
 * @param value formula
 */
void DialogCutSpline::setFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    ui->lineEditFormula->setText(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSplineId set id spline
 * @param value id
 * @param id don't show this id in list
 */
void DialogCutSpline::setSplineId(const quint32 &value, const quint32 &id)
{
    setCurrentSplineId(ui->comboBoxSpline, splineId, value, id, ComboMode::CutSpline);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
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
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogCutSpline::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->lineEditFormula->text();
    splineId = getCurrentObjectId(ui->comboBoxSpline);
    emit DialogClosed(QDialog::Accepted);
}
