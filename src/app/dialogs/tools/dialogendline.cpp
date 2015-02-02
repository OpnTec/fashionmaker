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
#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogEndLine create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogEndLine::DialogEndLine(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogEndLine), typeLine(QString()),
      formulaLength(QString()), formulaAngle(QString()), basePointId(NULL_ID), formulaBaseHeight(0),
      formulaBaseHeightAngle(0), line(nullptr)
{
    ui->setupUi(this);
    InitVariables(ui);
    InitFormulaUI(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    this->formulaBaseHeightAngle = ui->plainTextEditAngle->height();

    ui->plainTextEditFormula->installEventFilter(this);
    ui->plainTextEditAngle->installEventFilter(this);

    InitOkCancelApply(ui);
    flagFormula = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxTypeLine(ui->comboBoxLineType, VAbstractTool::LineStylesPics());
    FillComboBoxLineColors(ui->comboBoxLineColor);

    connect(ui->toolButtonPutHereLength, &QPushButton::clicked, this, &DialogEndLine::PutHere);
    connect(ui->toolButtonPutHereAngle, &QPushButton::clicked, this, &DialogEndLine::PutAngle);

    connect(ui->toolButtonEqualLength, &QPushButton::clicked, this, &DialogEndLine::EvalFormula);
    connect(ui->toolButtonEqualAngle, &QPushButton::clicked, this, &DialogEndLine::EvalAngle);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogEndLine::NamePointChanged);

    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogEndLine::FormulaTextChanged);
    connect(ui->plainTextEditAngle, &QPlainTextEdit::textChanged, this, &DialogEndLine::AngleTextChanged);

    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogEndLine::DeployFormulaTextEdit);
    connect(ui->pushButtonGrowLengthAngle, &QPushButton::clicked, this, &DialogEndLine::DeployAngleTextEdit);

    connect(timerFormula, &QTimer::timeout, this, &DialogEndLine::EvalAngle);

    line = new VisToolEndLine(data);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutAngle put variable into formula of angle
 */
void DialogEndLine::PutAngle()
{
    PutValHere(ui->plainTextEditAngle, ui->listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalAngle calculate value of angle
 */
void DialogEndLine::EvalAngle()
{
    labelEditFormula = ui->labelEditAngle;
    Eval(ui->plainTextEditAngle->toPlainText(), flagError, ui->labelResultCalculationAngle, degreeSymbol, false);
    labelEditFormula = ui->labelEditFormula;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::AngleTextChanged()
{
    labelEditFormula = ui->labelEditAngle;
    ValFormulaChanged(flagError, ui->plainTextEditAngle, timerFormula);
    labelEditFormula = ui->labelEditFormula;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::DeployAngleTextEdit()
{
    DeployFormula(ui->plainTextEditAngle, ui->pushButtonGrowLengthAngle, formulaBaseHeightAngle);
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
            if (SetObject(id, ui->comboBoxBasePoint, ""))
            {
                basePointId = id;
                line->VisualMode(id);
                connect(line, &VisToolEndLine::ToolTip, this, &DialogTool::ShowVisToolTip);
                prepare = true;
            }
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
    ChangeCurrentData(ui->comboBoxLineType, value);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFormula set string of formula
 * @param value formula
 */
void DialogEndLine::setFormula(const QString &value)
{
    formulaLength = qApp->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formulaLength.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formulaLength);
    line->setLength(formulaLength);
    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setAngle set angle of line
 * @param value angle in degree
 */
void DialogEndLine::setAngle(const QString &value)
{
    formulaAngle = qApp->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formulaAngle.length() > 80)
    {
        this->DeployAngleTextEdit();
    }
    ui->plainTextEditAngle->setPlainText(formulaAngle);
    line->setAngle(formulaAngle);
    MoveCursorToEnd(ui->plainTextEditAngle);
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
            const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(basePointId);
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
        timerFormula->start();
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
void DialogEndLine::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    typeLine = GetTypeLine(ui->comboBoxLineType);

    formulaLength = ui->plainTextEditFormula->toPlainText();
    formulaLength.replace("\n", " ");

    formulaAngle = ui->plainTextEditAngle->toPlainText();
    formulaAngle.replace("\n", " ");

    basePointId = getCurrentObjectId(ui->comboBoxBasePoint);

    line->setPoint1Id(basePointId);
    line->setLength(formulaLength);
    line->setAngle(formulaAngle);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(typeLine));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    ui->plainTextEditAngle->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
DialogEndLine::~DialogEndLine()
{
    if (qApp->getCurrentScene()->items().contains(line))
    { // In some cases scene delete object yourself. If not make check program will crash.
        delete line;
    }
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPointName return name of point
 * @return name
 */
QString DialogEndLine::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTypeLine return type of line
 * @return type
 */
QString DialogEndLine::getTypeLine() const
{
    return typeLine;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFormula return string of formula
 * @return formula
 */
QString DialogEndLine::getFormula() const
{
    return qApp->FormulaFromUser(formulaLength);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getAngle return formula angle of line
 * @return angle formula
 */
QString DialogEndLine::getAngle() const
{
    return qApp->FormulaFromUser(formulaAngle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getBasePointId return id base point of line
 * @return id
 */
quint32 DialogEndLine::getBasePointId() const
{
    return basePointId;
}
