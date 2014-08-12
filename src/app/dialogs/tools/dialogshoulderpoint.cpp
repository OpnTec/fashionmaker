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
    :DialogTool(data, toolId, parent), ui(new Ui::DialogShoulderPoint), number(0), pointName(QString()),
    typeLine(QString()), formula(QString()), p1Line(0), p2Line(0), pShoulder(0), formulaBaseHeight(0), line (nullptr)
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

    FillComboBoxTypeLine(ui->comboBoxLineType);
    FillComboBoxPoints(ui->comboBoxP1Line);
    FillComboBoxPoints(ui->comboBoxP2Line);
    FillComboBoxPoints(ui->comboBoxPShoulder);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogShoulderPoint::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogShoulderPoint::PutVal);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogShoulderPoint::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogShoulderPoint::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogShoulderPoint::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogShoulderPoint::DeployFormulaTextEdit);
    connect(ui->comboBoxP1Line, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogShoulderPoint::PointNameChanged);
    connect(ui->comboBoxP2Line, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogShoulderPoint::PointNameChanged);
    connect(ui->comboBoxPShoulder, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
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
    set.insert(getCurrentObjectId(ui->comboBoxPShoulder));

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
    ChangeColor(ui->labelPointShoulder, color);
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
    delete line;
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
            const VPointF *point = data->GeometricObject<const VPointF *>(id);
            if (number == 0)
            {
                qint32 index = ui->comboBoxPShoulder->findText(point->name());
                if ( index != -1 )
                { // -1 for not found
                    ui->comboBoxPShoulder->setCurrentIndex(index);
                    number++;
                    line->VisualMode(id);
                    emit ToolTip(tr("Select first point of line"));
                    return;
                }
            }
            if (number == 1)
            {
                qint32 index = ui->comboBoxP1Line->findText(point->name());
                if ( index != -1 )
                { // -1 for not found
                    ui->comboBoxP1Line->setCurrentIndex(index);
                    number++;
                    line->setLineP1Id(id);
                    line->RefreshGeometry();
                    emit ToolTip(tr("Select second point of line"));
                    return;
                }
            }
            if (number == 2)
            {
                qint32 index = ui->comboBoxP2Line->findText(point->name());

                if ( index != -1 )
                { // -1 for not found
                    ui->comboBoxP2Line->setCurrentIndex(index);
                    number = 0;
                    line->setLineP2Id(id);
                    line->RefreshGeometry();
                    prepare = true;
                    emit ToolTip("");
                }
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
void DialogShoulderPoint::DialogAccepted()
{
    this->SaveData();
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::DialogApply()
{
    this->SaveData();
    emit DialogApplied();
}
//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");
    p1Line = getCurrentObjectId(ui->comboBoxP1Line);
    p2Line = getCurrentObjectId(ui->comboBoxP2Line);
    pShoulder = getCurrentObjectId(ui->comboBoxPShoulder);

    line->setPoint1Id(pShoulder);
    line->setLineP1Id(p1Line);
    line->setLineP2Id(p2Line);
    line->setLength(formula);
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPShoulder set id shoulder point
 * @param value id
 */
void DialogShoulderPoint::setPShoulder(const quint32 &value)
{
    setPointId(ui->comboBoxPShoulder, pShoulder, value);
    line->setPoint1Id(pShoulder);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP2Line set id second point of line
 * @param value id
 */
void DialogShoulderPoint::setP2Line(const quint32 &value)
{
    setPointId(ui->comboBoxP2Line, p2Line, value);
    line->setLineP2Id(p2Line);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setP1Line set id first point of line
 * @param value id
 */
void DialogShoulderPoint::setP1Line(const quint32 &value)
{
    setPointId(ui->comboBoxP1Line, p1Line, value);
    line->setLineP1Id(p1Line);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string of formula
 * @param value formula
 */
void DialogShoulderPoint::setFormula(const QString &value)
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
void DialogShoulderPoint::setTypeLine(const QString &value)
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
void DialogShoulderPoint::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
