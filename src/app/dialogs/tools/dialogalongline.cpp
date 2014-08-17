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
#include "../../visualization/vistoolalongline.h"
#include "../../tools/vabstracttool.h"
#include "../../widgets/vmaingraphicsscene.h"

#include <QPushButton>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAlongLine create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogAlongLine::DialogAlongLine(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogAlongLine), number(0), pointName(QString()),
      typeLine(QString()), formula(QString()), firstPointId(NULL_ID), secondPointId(NULL_ID), formulaBaseHeight(0),
      line(nullptr)
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

    line = new VisToolAlongLine(data);
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
void DialogAlongLine::ShowVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisToolAlongLine::SetFactor);
        scene->addItem(line);
        line->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogAlongLine::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogAlongLine::~DialogAlongLine()
{
    delete line;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogAlongLine::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select second point of line")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (SetObject(id, ui->comboBoxSecondPoint, ""))
                    {
                        line->setPoint2Id(id);
                        line->RefreshGeometry();
                        prepare = true;
                        this->setModal(true);
                        this->show();
                    }
                    break;
                default:
                    break;
            }
        }
    }
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

    line->setPoint1Id(firstPointId);
    line->setPoint2Id(secondPointId);
    line->setLength(formula);
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSecondPointId set id second point of line
 * @param value id
 */
void DialogAlongLine::setSecondPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondPoint, secondPointId, value);
    line->setPoint2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPointId set id first point of line
 * @param value id
 */
void DialogAlongLine::setFirstPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstPoint, firstPointId, value);
    line->setPoint1Id(value);
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
    line->setLength(formula);
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
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
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
