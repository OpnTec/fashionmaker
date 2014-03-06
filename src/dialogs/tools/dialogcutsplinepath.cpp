/************************************************************************
 **
 **  @file   dialogcutsplinrpath.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

DialogCutSplinePath::DialogCutSplinePath(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogCutSplinePath), pointName(QString()), formula(QString()),
      splinePathId(0)
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

    FillComboBoxSplinesPath(ui->comboBoxSplinePath);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogCutSplinePath::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogCutSplinePath::PutVal);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogCutSplinePath::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutSplinePath::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogCutSplinePath::FormulaChanged);
}

DialogCutSplinePath::~DialogCutSplinePath()
{
    delete ui;
}

void DialogCutSplinePath::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

void DialogCutSplinePath::setFormula(const QString &value)
{
    formula = value;
    ui->lineEditFormula->setText(formula);
}

void DialogCutSplinePath::setSplinePathId(const quint32 &value, const quint32 &id)
{
    setCurrentSplinePathId(ui->comboBoxSplinePath, splinePathId, value, id, ComboMode::CutSpline);
}

void DialogCutSplinePath::ChoosedObject(quint32 id, const Scene::Scenes &type)
{
    if (type == Scene::SplinePath)
    {
        const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(id);
        ChangeCurrentText(ui->comboBoxSplinePath, splPath->name());
        emit ToolTip("");
        this->show();
    }
}

void DialogCutSplinePath::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->lineEditFormula->text();
    splinePathId = getCurrentObjectId(ui->comboBoxSplinePath);
    emit DialogClosed(QDialog::Accepted);
}
