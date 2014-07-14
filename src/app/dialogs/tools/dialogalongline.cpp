/************************************************************************
 **
 **  @file   dialogalongline.cpp
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

#include "dialogalongline.h"
#include "ui_dialogalongline.h"

#include <QPushButton>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAlongLine create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogAlongLine::DialogAlongLine(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogAlongLine), number(0), pointName(QString()), typeLine(QString()),
      formula(QString()), firstPointId(0), secondPointId(0), formulaBaseHeight(0)
{
    ui->setupUi(this);
    InitVariables(ui);
    InitFormulaUI(ui);
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();

    InitOkCancelApply(ui);
    flagFormula = false;
    flagName = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);
    ui->comboBoxLineType->setCurrentIndex(0);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogAlongLine::PutHere);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogAlongLine::NamePointChanged);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogAlongLine::EvalFormula);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogAlongLine::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogAlongLine::DeployFormulaTextEdit);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &DialogTool::PutVal);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogAlongLine::PointChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogAlongLine::PointChanged);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::PointChanged()
{
    QColor color = okColor;
    if (getCurrentObjectId(ui->comboBoxFirstPoint) == getCurrentObjectId(ui->comboBoxSecondPoint))
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelFirstPoint, color);
    ChangeColor(ui->labelSecondPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogAlongLine::~DialogAlongLine()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogAlongLine::ChoosedObject(quint32 id, const SceneObject &type)
{
    if (type == SceneObject::Point)
    {
        if (number == 0)
        {
            if (ChoosedPoint(id, ui->comboBoxFirstPoint, tr("Select second point of line")))
            {
                number++;
                return;
            }
        }
        if (number == 1)
        {
            if (ChoosedPoint(id, ui->comboBoxSecondPoint, ""))
            {
                number = 0;
                if (isInitialized == false)
                {
                    this->setModal(true);
                    this->show();
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogAlongLine::DialogAccepted()
{
    this->SaveData();
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::DialogApply()
{
    this->SaveData();
    emit DialogApplied();
}
//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");
    firstPointId = getCurrentObjectId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentObjectId(ui->comboBoxSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSecondPointId set id second point of line
 * @param value id
 * @param id id of current point
 */
void DialogAlongLine::setSecondPointId(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPointId set id first point of line
 * @param value id
 * @param id id of current point
 */
void DialogAlongLine::setFirstPointId(const quint32 &value, const quint32 &id)
{
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string of formula
 * @param value formula
 */
void DialogAlongLine::setFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setTypeLine set type of line
 * @param value type
 */
void DialogAlongLine::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogAlongLine::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
