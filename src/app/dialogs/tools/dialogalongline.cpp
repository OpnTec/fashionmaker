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
    :DialogTool(data, parent), ui(new Ui::DialogAlongLine), number(0), pointName(QString()),
    typeLine(QString()), formula(QString()), firstPointId(0), secondPointId(0)
{
    ui->setupUi(this);
    labelResultCalculation = ui->labelResultCalculation;
    plainTextEditFormula = ui->plainTextEditFormula;
    labelEditFormula = ui->labelEditFormula;

    this->formulaBaseHeight=ui->plainTextEditFormula->height();

    flagFormula = false;
    flagName = false;
    InitOkCancelApply(ui);
    CheckState();

    FillComboBoxTypeLine(ui->comboBoxLineType);
    ui->comboBoxLineType->setCurrentIndex(0);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);

    labelEditNamePoint = ui->labelEditNamePoint;
    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogAlongLine::PutHere);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogAlongLine::NamePointChanged);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogAlongLine::EvalFormula);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogAlongLine::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogAlongLine::DeployFormulaTextEdit);
    InitVariables(ui);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &DialogTool::PutVal);

    ui->pushButtonGrowLength->setIcon(QIcon::fromTheme("go-down",
                              QIcon(":/icons/win.icon.theme/icons/win.icon.theme/16x16/actions/go-down.png")));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::DeployFormulaTextEdit()
{
    if (ui->plainTextEditFormula->height() < DIALOGALONLINE_MAX_FORMULA_HEIGHT)
    {
        ui->plainTextEditFormula->setFixedHeight(DIALOGALONLINE_MAX_FORMULA_HEIGHT);
        //Set icon from theme (internal for Windows system)
        ui->pushButtonGrowLength->setIcon(QIcon::fromTheme("go-next",
                                  QIcon(":/icons/win.icon.theme/icons/win.icon.theme/16x16/actions/go-next.png")));
    }
    else
    {
       ui->plainTextEditFormula->setFixedHeight(this->formulaBaseHeight);
       //Set icon from theme (internal for Windows system)
       ui->pushButtonGrowLength->setIcon(QIcon::fromTheme("go-down",
                                   QIcon(":/icons/win.icon.theme/icons/win.icon.theme/16x16/actions/go-down.png")));
    }
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
void DialogAlongLine::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Point)
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
    formula.replace("\n"," ");
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
