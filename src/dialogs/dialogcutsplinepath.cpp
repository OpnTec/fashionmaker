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

DialogCutSplinePath::DialogCutSplinePath(const VContainer *data, Draw::Draws mode, QWidget *parent)
    :DialogTool(data, mode, parent), ui(new Ui::DialogCutSplinePath), pointName(QString()), formula(QString()),
      splinePathId(0)
{
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
    connect(bOk, &QPushButton::clicked, this, &DialogCutSplinePath::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogCutSplinePath::DialogRejected);
    if(mode == Draw::Calculation)
    {
        FillComboBoxSplines(ui->comboBoxSplinePath);
    }

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogCutSplinePath::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogCutSplinePath::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogCutSplinePath::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogCutSplinePath::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogCutSplinePath::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogCutSplinePath::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogCutSplinePath::LengthLines);
    connect(ui->radioButtonLengthArc, &QRadioButton::clicked, this, &DialogCutSplinePath::LengthArcs);
    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogCutSplinePath::LengthCurves);
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

void DialogCutSplinePath::setSplinePathId(const qint64 &value, const qint64 &id)
{
    setCurrentSplinePathId(ui->comboBoxSplinePath, splinePathId, value, id);
}

void DialogCutSplinePath::ChoosedObject(qint64 id, const Scene::Scenes &type)
{
    if (idDetail == 0 && mode == Draw::Modeling)
    {
        if (type == Scene::Detail)
        {
            idDetail = id;
            FillComboBoxSplines(ui->comboBoxSplinePath);
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
    if (type == Scene::Spline)
    {
        VSplinePath splPath;
        if (mode == Draw::Calculation)
        {
            splPath = data->GetSplinePath(id);
        }
        else
        {
            splPath = data->GetSplinePathModeling(id);
        }
        ChangeCurrentText(ui->comboBoxSplinePath, splPath.name());
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
