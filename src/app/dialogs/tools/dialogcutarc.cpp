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

#include "../../geometry/varc.h"
#include "../../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogCutArc create dialog.
 * @param data container with data
 * @param parent parent widget
 */
DialogCutArc::DialogCutArc(const VContainer *data, QWidget *parent)
    : DialogTool(data, parent), ui(new Ui::DialogCutArc), pointName(QString()), formula(QString()), arcId(0),
      formulaBaseHeight(0)
{
    ui->setupUi(this);
    InitVariables(ui);
    labelResultCalculation = ui->labelResultCalculation;
    plainTextEditFormula = ui->plainTextEditFormula;
    labelEditFormula = ui->labelEditFormula;
    labelEditNamePoint = ui->labelEditNamePoint;

    InitOkCancelApply(ui);
    flagFormula = false;
    flagName = false;
    CheckState();
    this->formulaBaseHeight=ui->plainTextEditFormula->height();

    FillComboBoxArcs(ui->comboBoxArc);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogCutArc::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogCutArc::PutVal);

    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogCutArc::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutArc::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogCutArc::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogCutArc::DeployFormulaTextEdit);

     ui->pushButtonGrowLength->setIcon(QIcon::fromTheme("go-down",
             QIcon(":/icons/win.icon.theme/icons/win.icon.theme/16x16/actions/go-down.png")));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::DeployFormulaTextEdit()
{
    if (ui->plainTextEditFormula->height() < DIALOGCUTARC_MAX_FORMULA_HEIGHT)
    {
        ui->plainTextEditFormula->setFixedHeight(DIALOGCUTARC_MAX_FORMULA_HEIGHT);
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
void DialogCutArc::ChoosedObject(quint32 id, const SceneObject &type)
{
    if (type == SceneObject::Arc)
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
    this->SaveData();
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::DialogApply()
{
    this->SaveData();
    emit DialogApplied();
}
//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");
    arcId = getCurrentObjectId(ui->comboBoxArc);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setArcId set id of arc
 * @param value id
 * @param id don't show this id in list
 */
void DialogCutArc::setArcId(const quint32 &value, const quint32 &id)
{
    setCurrentArcId(ui->comboBoxArc, arcId, value, id, ComboBoxCutArc::CutArc);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string with formula length
 * @param value string with formula
 */
void DialogCutArc::setFormula(const QString &value)
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
