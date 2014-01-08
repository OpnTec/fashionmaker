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
    connect(bOk, &QPushButton::clicked, this, &DialogCutArc::DialogAccepted);
    flagName = false;
    CheckState();
    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogCutArc::DialogRejected);

    FillComboBoxArcs(ui->comboBoxArc);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogCutArc::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogCutArc::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogCutArc::ValChenged);

    ShowVariable(data->DataBase());
    connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogCutArc::SizeGrowth);
    connect(ui->radioButtonStandartTable, &QRadioButton::clicked, this, &DialogCutArc::StandartTable);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogCutArc::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogCutArc::LengthLines);
    connect(ui->radioButtonLengthArc, &QRadioButton::clicked, this, &DialogCutArc::LengthArcs);
    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogCutArc::LengthCurves);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogCutArc::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutArc::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogCutArc::FormulaChanged);
}

DialogCutArc::~DialogCutArc()
{
    delete ui;
}

void DialogCutArc::ChoosedObject(qint64 id, const Scene::Scenes &type)
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

void DialogCutArc::setArcId(const qint64 &value, const qint64 &id)
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

