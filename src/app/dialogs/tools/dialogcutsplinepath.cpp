/************************************************************************
 **
 **  @file   dialogcutsplinrpath.cpp
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

#include "dialogcutsplinepath.h"
#include "ui_dialogcutsplinepath.h"

#include "../../geometry/vsplinepath.h"
#include "../../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogCutSplinePath create dialog.
 * @param data container with data
 * @param parent parent widget
 */
DialogCutSplinePath::DialogCutSplinePath(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogCutSplinePath), pointName(QString()), formula(QString()),
      splinePathId(0), formulaBaseHeight(0)
{
    ui->setupUi(this);
    InitVariables(ui);
    InitFormulaUI(ui);
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();

    InitOkCancel(ui);
    flagFormula = false;
    flagName = false;
    CheckState();

    FillComboBoxSplinesPath(ui->comboBoxSplinePath);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogCutSplinePath::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogCutSplinePath::PutVal);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogCutSplinePath::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutSplinePath::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogCutSplinePath::FormulaChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogCutSplinePath::DeployFormulaTextEdit);
}

//---------------------------------------------------------------------------------------------------------------------
DialogCutSplinePath::~DialogCutSplinePath()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogCutSplinePath::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string of formula
 * @param value formula
 */
void DialogCutSplinePath::setFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSplineId set id spline
 * @param value id
 */
void DialogCutSplinePath::setSplinePathId(const quint32 &value)
{
    setCurrentSplinePathId(ui->comboBoxSplinePath, splinePathId, value, ComboBoxCutSpline::CutSpline);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogCutSplinePath::ChosenObject(quint32 id, const SceneObject &type)
{
    if (type == SceneObject::SplinePath)
    {
        const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(id);
        ChangeCurrentText(ui->comboBoxSplinePath, splPath->name());
        emit ToolTip("");
        this->setModal(true);
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSplinePath::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");
    splinePathId = getCurrentObjectId(ui->comboBoxSplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSplinePath::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}
