/************************************************************************
 **
 **  @file   dialogcutspline.cpp
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

#include "dialogcutspline.h"
#include "ui_dialogcutspline.h"

DialogCutSpline::DialogCutSpline(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogCutSpline), pointName(QString()), formula(QString()), splineId(0)
{
    ui->setupUi(this);
    listWidget = ui->listWidget;
    labelResultCalculation = ui->labelResultCalculation;
    labelDescription = ui->labelDescription;
    radioButtonSizeGrowth = ui->radioButtonSizeGrowth;
    radioButtonStandardTable = ui->radioButtonStandardTable;
    radioButtonIncrements = ui->radioButtonIncrements;
    radioButtonLengthLine = ui->radioButtonLengthLine;
    radioButtonLengthArc = ui->radioButtonLengthArc;
    radioButtonLengthCurve = ui->radioButtonLengthSpline;
    lineEditFormula = ui->lineEditFormula;
    labelEditFormula = ui->labelEditFormula;
    labelEditNamePoint = ui->labelEditNamePoint;
    flagFormula = false;
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogCutSpline::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogCutSpline::DialogRejected);

    FillComboBoxSplines(ui->comboBoxSpline);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogCutSpline::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogCutSpline::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogCutSpline::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogCutSpline::SizeGrowth);
    connect(ui->radioButtonStandardTable, &QRadioButton::clicked, this, &DialogCutSpline::StandardTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogCutSpline::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogCutSpline::LengthLines);
    connect(ui->radioButtonLengthArc, &QRadioButton::clicked, this, &DialogCutSpline::LengthArcs);
    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogCutSpline::LengthCurves);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogCutSpline::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutSpline::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogCutSpline::FormulaChanged);
}

DialogCutSpline::~DialogCutSpline()
{
    delete ui;
}

void DialogCutSpline::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

void DialogCutSpline::setFormula(const QString &value)
{
    formula = value;
    ui->lineEditFormula->setText(formula);
}

void DialogCutSpline::setSplineId(const quint32 &value, const quint32 &id)
{
    setCurrentSplineId(ui->comboBoxSpline, splineId, value, id, ComboMode::CutSpline);
}

void DialogCutSpline::ChoosedObject(quint32 id, const Scene::Scenes &type)
{
    if (type == Scene::Spline)
    {
        const VSpline *spl = data->GeometricObject<const VSpline *>(id);
        ChangeCurrentText(ui->comboBoxSpline, spl->name());
        emit ToolTip("");
        this->show();
    }
}

void DialogCutSpline::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->lineEditFormula->text();
    splineId = getCurrentObjectId(ui->comboBoxSpline);
    emit DialogClosed(QDialog::Accepted);
}
