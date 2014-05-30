/************************************************************************
 **
 **  @file   dialogcutarc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogCutArc create dialog.
 * @param data container with data
 * @param parent parent widget
 */
DialogCutArc::DialogCutArc(const VContainer *data, QWidget *parent) :
    DialogTool(data, parent), ui(new Ui::DialogCutArc), pointName(QString()), formula(QString()), arcId(0)
{
    ui->setupUi(this);
    InitVariables(ui);
    labelResultCalculation = ui->labelResultCalculation;
    lineEditFormula = ui->lineEditFormula;
    labelEditFormula = ui->labelEditFormula;
    labelEditNamePoint = ui->labelEditNamePoint;

    InitOkCancelApply(ui);
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

//---------------------------------------------------------------------------------------------------------------------
DialogCutArc::~DialogCutArc()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogCutArc::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Arc)
    {
        const VArc *arc = data->GeometricObject<const VArc *>(id);
        ChangeCurrentText(ui->comboBoxArc, arc->name());
        emit ToolTip("");
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogCutArc::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->lineEditFormula->text();
    arcId = getCurrentObjectId(ui->comboBoxArc);
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setArcId set id of arc
 * @param value id
 * @param id don't show this id in list
 */
void DialogCutArc::setArcId(const quint32 &value, const quint32 &id)
{
    setCurrentArcId(ui->comboBoxArc, arcId, value, id, ComboMode::CutArc);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string with formula length
 * @param value string with formula
 */
void DialogCutArc::setFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    ui->lineEditFormula->setText(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name point on arc
 * @param value name
 */
void DialogCutArc::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFormula return string with formula length
 * @return formula
 */
QString DialogCutArc::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}
