/************************************************************************
 **
 **  @file   dialogbisector.cpp
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

#include "dialogbisector.h"
#include "ui_dialogbisector.h"

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../visualization/vistoolbisector.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../tools/vabstracttool.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogBisector create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogBisector::DialogBisector(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogBisector), number(0), typeLine(QString()),
      formula(QString()), firstPointId(NULL_ID), secondPointId(NULL_ID), thirdPointId(NULL_ID), formulaBaseHeight(0),
      line(nullptr)
{
    ui->setupUi(this);
    InitVariables(ui);
    InitFormulaUI(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancelApply(ui);
    flagFormula = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);
    FillComboBoxPoints(ui->comboBoxThirdPoint);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogBisector::PutHere);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &DialogBisector::PutVal);

    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogBisector::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogBisector::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogBisector::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogBisector::DeployFormulaTextEdit);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogBisector::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogBisector::PointNameChanged);
    connect(ui->comboBoxThirdPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogBisector::PointNameChanged);

    line = new VisToolBisector(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
    set.insert(getCurrentObjectId(ui->comboBoxThirdPoint));

    QColor color = okColor;
    if (set.size() != 3)
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
    ChangeColor(ui->labelThirdPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::ShowVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisToolBisector::SetFactor);
        scene->addItem(line);
        line->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogBisector::~DialogBisector()
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
void DialogBisector::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select second point of angle")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (SetObject(id, ui->comboBoxSecondPoint, tr("Select third point of angle")))
                    {
                        number++;
                        line->setPoint2Id(id);
                        line->RefreshGeometry();
                    }
                    break;
                case 2:
                    if (SetObject(id, ui->comboBoxThirdPoint, ""))
                    {
                        line->setPoint3Id(id);
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
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogBisector::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setTypeLine set type of line
 * @param value type
 */
void DialogBisector::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string of formula
 * @param value formula
 */
void DialogBisector::setFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);
    line->setLength(formula);
    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPointId set id of first point
 * @param value id
 */
void DialogBisector::setFirstPointId(const quint32 &value)
{
    setPointId(ui->comboBoxFirstPoint, firstPointId, value);
    line->setPoint1Id(firstPointId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSecondPointId set id of second point
 * @param value id
 */
void DialogBisector::setSecondPointId(const quint32 &value)
{
    setPointId(ui->comboBoxSecondPoint, secondPointId, value);
    line->setPoint2Id(secondPointId);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setThirdPointId set id of third point
 * @param value id
 */
void DialogBisector::setThirdPointId(const quint32 &value)
{
    setPointId(ui->comboBoxThirdPoint, thirdPointId, value);
    line->setPoint3Id(thirdPointId);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");
    firstPointId = getCurrentObjectId(ui->comboBoxFirstPoint);
    secondPointId = getCurrentObjectId(ui->comboBoxSecondPoint);
    thirdPointId = getCurrentObjectId(ui->comboBoxThirdPoint);

    line->setPoint1Id(firstPointId);
    line->setPoint2Id(secondPointId);
    line->setPoint3Id(thirdPointId);
    line->setLength(formula);
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
    line->RefreshGeometry();
}
