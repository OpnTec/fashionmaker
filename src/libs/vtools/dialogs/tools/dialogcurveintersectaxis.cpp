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
 **  Copyright (C) 2013-2015 Valentina project
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

#include "../../../vgeometry/vpointf.h"
#include "../../../vpatterndb/vcontainer.h"
#include "../../../vpatterndb/vtranslatevars.h"
#include "../../visualization/vistoolcurveintersectaxis.h"
#include "../../../vwidgets/vmaingraphicsscene.h"
#include "../../tools/vabstracttool.h"
#include "../support/dialogeditwrongformula.h"

#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
DialogCurveIntersectAxis::DialogCurveIntersectAxis(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogCurveIntersectAxis), formulaAngle(QString()),
      formulaBaseHeightAngle(0)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditNamePoint->setClearButtonEnabled(true);
#endif

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

    connect(ui->toolButtonExprAngle, &QPushButton::clicked, this, &DialogCurveIntersectAxis::FXAngle);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCurveIntersectAxis::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogCurveIntersectAxis::AngleTextChanged);
    connect(ui->pushButtonGrowLengthAngle, &QPushButton::clicked, this, &DialogCurveIntersectAxis::DeployAngleTextEdit);
    connect(timerFormula, &QTimer::timeout, this, &DialogCurveIntersectAxis::EvalAngle);

    vis = new VisToolCurveIntersectAxis(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogCurveIntersectAxis::~DialogCurveIntersectAxis()
{
    DeleteVisualization<VisToolCurveIntersectAxis>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCurveIntersectAxis::GetTypeLine() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineType);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::SetTypeLine(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineType, value);
    vis->setLineStyle(VAbstractTool::LineStyleToPenStyle(value));
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCurveIntersectAxis::GetAngle() const
{
    return qApp->TrVars()->FormulaFromUser(formulaAngle, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::SetAngle(const QString &value)
{
    formulaAngle = qApp->TrVars()->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formulaAngle.length() > 80)
    {
        this->DeployAngleTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formulaAngle);

    VisToolCurveIntersectAxis *line = qobject_cast<VisToolCurveIntersectAxis *>(vis);
    SCASSERT(line != nullptr);
    line->SetAngle(formulaAngle);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogCurveIntersectAxis::GetBasePointId() const
{
    return getCurrentObjectId(ui->comboBoxAxisPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::SetBasePointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxAxisPoint, value);

    VisToolCurveIntersectAxis *line = qobject_cast<VisToolCurveIntersectAxis *>(vis);
    SCASSERT(line != nullptr);
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

    VisToolCurveIntersectAxis *line = qobject_cast<VisToolCurveIntersectAxis *>(vis);
    SCASSERT(line != nullptr);
    line->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCurveIntersectAxis::GetLineColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::SetLineColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::ShowDialog(bool click)
{
    if (prepare)
    {
        if (click)
        {
            /*We will ignore click if poinet is in point circle*/
            VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
            SCASSERT(scene != nullptr);
            const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(GetBasePointId());
            QLineF line = QLineF(point->toQPointF(), scene->getScenePos());

            //Radius of point circle, but little bigger. Need handle with hover sizes.
            qreal radius = ToPixel(DefPointRadius/*mm*/, Unit::Mm)*1.5;
            if (line.length() <= radius)
            {
                return;
            }
        }

        VisToolCurveIntersectAxis *line = qobject_cast<VisToolCurveIntersectAxis *>(vis);
        SCASSERT(line != nullptr);

        this->SetAngle(line->Angle());//Show in dialog angle what user choose
        emit ToolTip("");

        DialogAccepted();// Just set default values and don't show dialog
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        VisToolCurveIntersectAxis *line = qobject_cast<VisToolCurveIntersectAxis *>(vis);
        SCASSERT(line != nullptr);

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
void DialogCurveIntersectAxis::EvalAngle()
{
    Eval(ui->plainTextEditFormula->toPlainText(), flagError, ui->labelResultCalculation, degreeSymbol, false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::AngleTextChanged()
{
    ValFormulaChanged(flagError, ui->plainTextEditFormula, timerFormula, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::DeployAngleTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLengthAngle, formulaBaseHeightAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::FXAngle()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit angle"));
    dialog->SetFormula(GetAngle());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetAngle(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::ShowVisualization()
{
    AddVisualization<VisToolCurveIntersectAxis>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    formulaAngle = ui->plainTextEditFormula->toPlainText();
    formulaAngle.replace("\n", " ");

    VisToolCurveIntersectAxis *line = qobject_cast<VisToolCurveIntersectAxis *>(vis);
    SCASSERT(line != nullptr);

    line->setObject1Id(getCurveId());
    line->setAxisPointId(GetBasePointId());
    line->SetAngle(formulaAngle);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(GetTypeLine()));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCurveIntersectAxis::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}
