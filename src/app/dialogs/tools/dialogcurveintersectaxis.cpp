/************************************************************************
 **
 **  @file   dialogcurveintersectaxis.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 10, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "dialogcurveintersectaxis.h"
#include "ui_dialogcurveintersectaxis.h"

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../visualization/vistoolcurveintersectaxis.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../tools/vabstracttool.h"
#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
DialogCurveIntersectAxis::DialogCurveIntersectAxis(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogCurveIntersectAxis),
      formulaAngle(QString()), formulaBaseHeightAngle(0), line(nullptr)
{
    ui->setupUi(this);

    InitVariables(ui);
    InitFormulaUI(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeightAngle = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancelApply(ui);
    flagFormula = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxAxisPoint);
    FillComboBoxCurves(ui->comboBoxCurve);
    FillComboBoxTypeLine(ui->comboBoxLineType, VAbstractTool::LineStylesPics());
    FillComboBoxLineColors(ui->comboBoxLineColor);

    connect(ui->toolButtonPutHereAngle, &QPushButton::clicked, this, &DialogCurveIntersectAxis::PutAngle);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &DialogCurveIntersectAxis::PutVal);
    connect(ui->toolButtonEqualAngle, &QPushButton::clicked, this, &DialogCurveIntersectAxis::EvalAngle);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCurveIntersectAxis::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogCurveIntersectAxis::AngleTextChanged);
    connect(ui->pushButtonGrowLengthAngle, &QPushButton::clicked, this, &DialogCurveIntersectAxis::DeployAngleTextEdit);
    connect(timerFormula, &QTimer::timeout, this, &DialogCurveIntersectAxis::EvalAngle);

    line = new VisToolCurveIntersectAxis(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogCurveIntersectAxis::~DialogCurveIntersectAxis()
{
    if (qApp->getCurrentScene()->items().contains(line))
    { // In some cases scene delete object yourself. If not make check program will crash.
        delete line;
    }
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCurveIntersectAxis::getTypeLine() const
{
    return GetTypeLine(ui->comboBoxLineType);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::setTypeLine(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineType, value);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(value));
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCurveIntersectAxis::getAngle() const
{
    return qApp->FormulaFromUser(formulaAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::setAngle(const QString &value)
{
    formulaAngle = qApp->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formulaAngle.length() > 80)
    {
        this->DeployAngleTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formulaAngle);
    line->setAngle(formulaAngle);
    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogCurveIntersectAxis::getBasePointId() const
{
    return getCurrentObjectId(ui->comboBoxAxisPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::setBasePointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxAxisPoint, value);
    line->setAxisPointId(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogCurveIntersectAxis::getCurveId() const
{
    return getCurrentObjectId(ui->comboBoxCurve);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::setCurveId(const quint32 &value)
{
    setCurrentCurveId(ui->comboBoxCurve, value);
    line->setPoint1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::ShowDialog(bool click)
{
    if (prepare)
    {
        if (click)
        {
            /*We will ignore click if poinet is in point circle*/
            VMainGraphicsScene *scene = qApp->getCurrentScene();
            SCASSERT(scene != nullptr);
            const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(getBasePointId());
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
void DialogCurveIntersectAxis::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        switch (number)
        {
            case (0):
                if (type == SceneObject::Spline || type == SceneObject::Arc || type == SceneObject::SplinePath)
                {
                    if (SetObject(id, ui->comboBoxCurve, tr("Select axis point")))
                    {
                        number++;
                        line->VisualMode(id);
                        connect(line, &VisToolCurveIntersectAxis::ToolTip, this, &DialogTool::ShowVisToolTip);
                    }
                }
                break;
            case (1):
                if (type == SceneObject::Point)
                {
                    if (SetObject(id, ui->comboBoxAxisPoint, ""))
                    {
                        line->setAxisPointId(id);
                        line->RefreshGeometry();
                        prepare = true;
                    }
                }
                break;
            default:
                break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::PutAngle()
{
    PutValHere(ui->plainTextEditFormula, ui->listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::EvalAngle()
{
    Eval(ui->plainTextEditFormula->toPlainText(), flagError, ui->labelResultCalculation, degreeSymbol, false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::AngleTextChanged()
{
    ValFormulaChanged(flagError, ui->plainTextEditFormula, timerFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::DeployAngleTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLengthAngle, formulaBaseHeightAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::ShowVisualization()
{
    if (prepare == false)
    {
        //TODO move to parent class!
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisToolCurveIntersectAxis::SetFactor);
        scene->addItem(line);
        line->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    formulaAngle = ui->plainTextEditFormula->toPlainText();
    formulaAngle.replace("\n", " ");

    line->setPoint1Id(getCurveId());
    line->setAxisPointId(getBasePointId());
    line->setAngle(formulaAngle);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(getTypeLine()));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}
