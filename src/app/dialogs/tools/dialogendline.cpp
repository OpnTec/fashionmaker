/************************************************************************
 **
 **  @file   dialogendline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "dialogendline.h"
#include "ui_dialogendline.h"

#include <QPushButton>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogEndLine create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogEndLine::DialogEndLine(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogEndLine), pointName(QString()), typeLine(QString()),
    formula(QString()), angle(0), basePointId(0)
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

    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    InitArrow(ui);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogEndLine::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogEndLine::PutVal);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogEndLine::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogEndLine::NamePointChanged);
    connect(ui->lineEditFormula, &QLineEdit::textChanged, this, &DialogEndLine::FormulaChanged);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogEndLine::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        ChangeCurrentText(ui->comboBoxBasePoint, point->name());
        emit ToolTip("");
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogEndLine::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setTypeLine set type of line
 * @param value type
 */
void DialogEndLine::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string of formula
 * @param value formula
 */
void DialogEndLine::setFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    ui->lineEditFormula->setText(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setAngle set angle of line
 * @param value angle in degree
 */
void DialogEndLine::setAngle(const qreal &value)
{
    angle = value;
    ui->doubleSpinBoxAngle->setValue(angle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setBasePointId set id base point of line
 * @param value id
 * @param id don't show this id in list
 */
void DialogEndLine::setBasePointId(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxBasePoint, basePointId, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogEndLine::DialogAccepted()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->lineEditFormula->text();
    angle = ui->doubleSpinBoxAngle->value();
    basePointId = getCurrentObjectId(ui->comboBoxBasePoint);
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
DialogEndLine::~DialogEndLine()
{
    delete ui;
}
