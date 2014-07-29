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

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../visualization/vistoolendline.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../tools/vabstracttool.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogEndLine create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogEndLine::DialogEndLine(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogEndLine), pointName(QString()), typeLine(QString()),
      formula(QString()), angle(0), basePointId(0), formulaBaseHeight(0), line(nullptr), prepare(false)
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

    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    InitArrow(ui);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogEndLine::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogEndLine::PutVal);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogEndLine::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogEndLine::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogEndLine::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogEndLine::DeployFormulaTextEdit);

    line = new VisToolEndLine(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogEndLine::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            const VPointF *point = data->GeometricObject<const VPointF *>(id);
            basePointId = id;
            ChangeCurrentText(ui->comboBoxBasePoint, point->name());

            VMainGraphicsScene *scene = qApp->getCurrentScene();
            SCASSERT(scene != nullptr);
            scene->addItem(line);
            line->VisualMode(id, scene->getScenePos());
            connect(scene, &VMainGraphicsScene::NewFactor, line, &VisToolEndLine::SetFactor);
            connect(scene, &VMainGraphicsScene::mouseMove, line, &VisToolEndLine::MousePos);
            connect(line, &VisToolEndLine::ToolTip, this, &DialogTool::ShowVisToolTip);

            emit ToolTip("");
            prepare = true;
        }
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
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string of formula
 * @param value formula
 */
void DialogEndLine::setFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);
    line->setLength(formula);
    //QTextCursor cursor = ui->plainTextEditFormula->textCursor();
    //cursor.insertText(value);
    //ui->plainTextEditFormula->setCursor(cursor);
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
    line->setAngle(angle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setBasePointId set id base point of line
 * @param value id
 */
void DialogEndLine::setBasePointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxBasePoint, basePointId, value);
    line->setPoint1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogEndLine::ShowDialog show dialog after finish working with visual part
 * @param click true if need show dialog after click mouse
 */
void DialogEndLine::ShowDialog(bool click)
{
    if (prepare)
    {
        if (click)
        {
            /*We will ignore click if poinet is in point circle*/
            VMainGraphicsScene *scene = qApp->getCurrentScene();
            SCASSERT(scene != nullptr);
            const VPointF *point = data->GeometricObject<const VPointF *>(basePointId);
            QLineF line = QLineF(point->toQPointF(), scene->getScenePos());

            //Radius of point circle, but little bigger. Need handle with hover sizes.
            qreal radius = qApp->toPixel(DefPointRadius/*mm*/, Unit::Mm)*1.5;
            if (line.length() <= radius)
            {
                return;
            }
        }
        this->setModal(true);
        this->setAngle(line->Angle());//Show in dialog angle what user choose
        emit ToolTip("");
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::ShowVisualization()
{
    if (prepare == false)
    {
        //TODO move to parent class!
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisToolEndLine::SetFactor);
        scene->addItem(line);
        line->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogEndLine::DialogAccepted()
{
    this->SaveData();
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::DialogApply()
{
    this->SaveData();
    emit DialogApplied();
}
//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");
    angle = ui->doubleSpinBoxAngle->value();
    basePointId = getCurrentObjectId(ui->comboBoxBasePoint);

    line->setPoint1Id(basePointId);
    line->setLength(formula);
    line->setAngle(angle);
    line->setLineStyle(VAbstractTool::LineStyle(typeLine));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
DialogEndLine::~DialogEndLine()
{
    delete line;
    delete ui;
}
