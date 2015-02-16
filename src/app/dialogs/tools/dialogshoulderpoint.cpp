/************************************************************************
 **
 **  @file   dialogshoulderpoint.cpp
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

#include "dialogshoulderpoint.h"
#include "ui_dialogshoulderpoint.h"

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../visualization/vistoolshoulderpoint.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../tools/vabstracttool.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogShoulderPoint create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogShoulderPoint::DialogShoulderPoint(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogShoulderPoint), formula(QString()),
      formulaBaseHeight(0), line (nullptr)
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

    FillComboBoxTypeLine(ui->comboBoxLineType, VAbstractTool::LineStylesPics());
    FillComboBoxPoints(ui->comboBoxP1Line);
    FillComboBoxPoints(ui->comboBoxP2Line);
    FillComboBoxPoints(ui->comboBoxP3);
    FillComboBoxLineColors(ui->comboBoxLineColor);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogShoulderPoint::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogShoulderPoint::PutVal);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogShoulderPoint::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogShoulderPoint::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogShoulderPoint::DeployFormulaTextEdit);
    connect(ui->comboBoxP1Line, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogShoulderPoint::PointNameChanged);
    connect(ui->comboBoxP2Line, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogShoulderPoint::PointNameChanged);
    connect(ui->comboBoxP3, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogShoulderPoint::PointNameChanged);

    line = new VisToolShoulderPoint(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxP1Line));
    set.insert(getCurrentObjectId(ui->comboBoxP2Line));
    set.insert(getCurrentObjectId(ui->comboBoxP3));

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
void DialogShoulderPoint::ShowVisualization()
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
void DialogShoulderPoint::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogShoulderPoint::~DialogShoulderPoint()
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
void DialogShoulderPoint::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxP3, tr("Select first point of line")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxP3) != id)
                    {
                        if (SetObject(id, ui->comboBoxP1Line, tr("Select second point of line")))
                        {
                            number++;
                            line->setLineP1Id(id);
                            line->RefreshGeometry();
                        }
                    }
                    break;
                case 2:
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxP3));
                    set.insert(getCurrentObjectId(ui->comboBoxP1Line));
                    set.insert(id);

                    if (set.size() == 3)
                    {
                        if (SetObject(id, ui->comboBoxP2Line, ""))
                        {
                            line->setLineP2Id(id);
                            line->RefreshGeometry();
                            prepare = true;
                            this->setModal(true);
                            this->show();
                        }
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
void DialogShoulderPoint::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");

    line->setPoint1Id(GetP3());
    line->setLineP1Id(GetP1Line());
    line->setLineP2Id(GetP2Line());
    line->setLength(formula);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(GetTypeLine()));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPShoulder set id shoulder point
 * @param value id
 */
void DialogShoulderPoint::SetP3(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP3, value);
    line->setPoint1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogShoulderPoint::GetLineColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::SetLineColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP2Line set id second point of line
 * @param value id
 */
void DialogShoulderPoint::SetP2Line(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP2Line, value);
    line->setLineP2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP1Line set id first point of line
 * @param value id
 */
void DialogShoulderPoint::SetP1Line(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP1Line, value);
    line->setLineP1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFormula set string of formula
 * @param value formula
 */
void DialogShoulderPoint::SetFormula(const QString &value)
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
 * @brief SetTypeLine set type of line
 * @param value type
 */
void DialogShoulderPoint::SetTypeLine(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineType, value);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(value));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name of point
 * @param value name
 */
void DialogShoulderPoint::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetTypeLine return type of line
 * @return type
 */
QString DialogShoulderPoint::GetTypeLine() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineType);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormula return string of formula
 * @return formula
 */
QString DialogShoulderPoint::GetFormula() const
{
    return qApp->FormulaFromUser(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP1Line return id first point of line
 * @return id
 */
quint32 DialogShoulderPoint::GetP1Line() const
{
    return getCurrentObjectId(ui->comboBoxP1Line);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2Line return id second point of line
 * @return id
 */
quint32 DialogShoulderPoint::GetP2Line() const
{
    return getCurrentObjectId(ui->comboBoxP2Line);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPShoulder return id shoulder point
 * @return id
 */
quint32 DialogShoulderPoint::GetP3() const
{
    return getCurrentObjectId(ui->comboBoxP3);
}
