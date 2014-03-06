/************************************************************************
 **
 **  @file   dialogcutarc.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   7 1, 2014
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

#include "dialogcutarc.h"
#include "ui_dialogcutarc.h"

DialogCutArc::DialogCutArc(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogCutArc), pointName(QString()), formula(QString()), arcId(0)
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

    FillComboBoxArcs(ui->comboBoxArc);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogCutArc::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogCutArc::PutVal);

    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogCutArc::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutArc::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogCutArc::FormulaChanged);
}

DialogCutArc::~DialogCutArc()
{
    delete ui;
}

void DialogCutArc::ChoosedObject(quint32 id, const Scene::Scenes &type)
{
    if (type == Scene::Arc)
    {
        const VArc *arc = data->GeometricObject<const VArc *>(id);
        ChangeCurrentText(ui->comboBoxArc, arc->name());
        emit ToolTip("");
        this->show();
    }
}

void DialogCutArc::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->lineEditFormula->text();
    arcId = getCurrentObjectId(ui->comboBoxArc);
    emit DialogClosed(QDialog::Accepted);
}

void DialogCutArc::setArcId(const quint32 &value, const quint32 &id)
{
    setCurrentArcId(ui->comboBoxArc, arcId, value, id, ComboMode::CutArc);
}

void DialogCutArc::setFormula(const QString &value)
{
    formula = value;
    ui->lineEditFormula->setText(formula);
}

void DialogCutArc::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
