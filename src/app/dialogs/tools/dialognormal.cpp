/************************************************************************
 **
 **  @file   dialognormal.cpp
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

#include "dialognormal.h"
#include "ui_dialognormal.h"
#include "../visualization/vistoolnormal.h"
#include "../widgets/vmaingraphicsscene.h"
#include "../../tools/vabstracttool.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogNormal create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogNormal::DialogNormal(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogNormal), number(0), typeLine(QString()),
      formula(QString()), angle(0), formulaBaseHeight(0), line(nullptr)
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
    FillComboBoxTypeLine(ui->comboBoxLineType, VAbstractTool::LineStylesPics());
    FillComboBoxLineColors(ui->comboBoxLineColor);

    InitArrow(ui);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogNormal::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogNormal::PutVal);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogNormal::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogNormal::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogNormal::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogNormal::DeployFormulaTextEdit);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogNormal::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogNormal::PointNameChanged);

    line = new VisToolNormal(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNormal::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNormal::PointNameChanged()
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
void DialogNormal::ShowVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisLine::SetFactor);
        scene->addItem(line);
        line->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNormal::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogNormal::~DialogNormal()
{
    if (qApp->getCurrentScene()->items().contains(line))
    { // In some cases scene delete object yourself. If not make check program will crash.
        delete line;
    }
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogNormal::ChosenObject(quint32 id, const SceneObject &type)
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
                    if (getCurrentObjectId(ui->comboBoxFirstPoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondPoint, ""))
                        {
                            line->setPoint2Id(id);
                            line->RefreshGeometry();
                            prepare = true;
                            this->setModal(true);
                            this->show();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNormal::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");
    angle = ui->doubleSpinBoxAngle->value();

    line->setPoint1Id(getFirstPointId());
    line->setPoint2Id(getSecondPointId());
    line->setLength(formula);
    line->setAngle(angle);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogNormal::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSecondPointId set id of second point
 * @param value id
 */
void DialogNormal::setSecondPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondPoint, value);
    line->setPoint2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPointId set id of first point
 * @param value id
 */
void DialogNormal::setFirstPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstPoint, value);
    line->setPoint1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setAngle set aditional angle of normal
 * @param value angle in degree
 */
void DialogNormal::setAngle(const qreal &value)
{
    angle = value;
    ui->doubleSpinBoxAngle->setValue(angle);
    line->setAngle(angle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string of formula
 * @param value formula
 */
void DialogNormal::setFormula(const QString &value)
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
 * @brief setTypeLine set type of line
 * @param value type
 */
void DialogNormal::setTypeLine(const QString &value)
{
    typeLine = value;
    ChangeCurrentData(ui->comboBoxLineType, value);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogNormal::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTypeLine return type of line
 * @return type
 */
QString DialogNormal::getTypeLine() const
{
    return typeLine;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFormula return string of formula
 * @return formula
 */
QString DialogNormal::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getAngle return aditional angle of normal
 * @return angle in degree
 */
qreal DialogNormal::getAngle() const
{
    return angle;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFirstPointId return id of first point
 * @return id
 */
quint32 DialogNormal::getFirstPointId() const
{
    return getCurrentObjectId(ui->comboBoxFirstPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSecondPointId return id of second point
 * @return id
 */
quint32 DialogNormal::getSecondPointId() const
{
    return getCurrentObjectId(ui->comboBoxSecondPoint);
}
