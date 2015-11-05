/************************************************************************
 **
 **  @file   dialogpointfromcircleandtangent.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "dialogpointfromcircleandtangent.h"
#include "ui_dialogpointfromcircleandtangent.h"

#include "../../../vgeometry/vpointf.h"
#include "../../../vpatterndb/vcontainer.h"
#include "../../../vpatterndb/vtranslatevars.h"
#include "../../visualization/vistoolpointfromcircleandtangent.h"
#include "../../../vwidgets/vmaingraphicsscene.h"
#include "../support/dialogeditwrongformula.h"

#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
DialogPointFromCircleAndTangent::DialogPointFromCircleAndTangent(const VContainer *data, const quint32 &toolId,
                                                                 QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogPointFromCircleAndTangent), flagCircleRadius(false),
      timerCircleRadius(nullptr), circleRadius(), formulaBaseHeightCircleRadius(0), angleCircleRadius(INT_MIN)
{
    ui->setupUi(this);

    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;

    plainTextEditFormula = ui->plainTextEditRadius;
    this->formulaBaseHeightCircleRadius = ui->plainTextEditRadius->height();

    ui->plainTextEditRadius->installEventFilter(this);

    timerCircleRadius = new QTimer(this);
    connect(timerCircleRadius, &QTimer::timeout, this, &DialogPointFromCircleAndTangent::EvalCircleRadius);

    InitOkCancelApply(ui);
    CheckState();

    FillComboBoxPoints(ui->comboBoxCircleCenter);
    FillComboBoxPoints(ui->comboBoxTangentPoint);
    FillComboBoxCrossCirclesPoints(ui->comboBoxResult);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointFromCircleAndTangent::NamePointChanged);
    connect(ui->comboBoxCircleCenter,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointFromCircleAndTangent::PointChanged);

    connect(ui->toolButtonExprRadius, &QPushButton::clicked, this,
            &DialogPointFromCircleAndTangent::FXCircleRadius);

    connect(ui->plainTextEditRadius, &QPlainTextEdit::textChanged, this,
            &DialogPointFromCircleAndTangent::CircleRadiusChanged);

    connect(ui->pushButtonGrowRadius, &QPushButton::clicked, this,
            &DialogPointFromCircleAndTangent::DeployCircleRadiusTextEdit);

    vis = new VisToolPointFromCircleAndTangent(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPointFromCircleAndTangent::~DialogPointFromCircleAndTangent()
{
    DeleteVisualization<VisToolPointFromCircleAndTangent>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPointFromCircleAndTangent::GetCircleCenterId() const
{
    return getCurrentObjectId(ui->comboBoxCircleCenter);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::SetCircleCenterId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxCircleCenter, value);

    VisToolPointFromCircleAndTangent *point = qobject_cast<VisToolPointFromCircleAndTangent *>(vis);
    SCASSERT(point != nullptr);
    point->setPoint2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPointFromCircleAndTangent::GetCircleRadius() const
{
    return qApp->TrVars()->FormulaFromUser(ui->plainTextEditRadius->toPlainText(),
                                           qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::SetCircleRadius(const QString &value)
{
    const QString formula = qApp->TrVars()->FormulaToUser(value);
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployCircleRadiusTextEdit();
    }
    ui->plainTextEditRadius->setPlainText(formula);

    VisToolPointFromCircleAndTangent *point = qobject_cast<VisToolPointFromCircleAndTangent *>(vis);
    SCASSERT(point != nullptr);
    point->setCRadius(formula);

    MoveCursorToEnd(ui->plainTextEditRadius);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPointFromCircleAndTangent::GetTangentPointId() const
{
    return getCurrentObjectId(ui->comboBoxTangentPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::SetTangentPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxTangentPoint, value);

    VisToolPointFromCircleAndTangent *point = qobject_cast<VisToolPointFromCircleAndTangent *>(vis);
    SCASSERT(point != nullptr);
    point->setPoint1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
CrossCirclesPoint DialogPointFromCircleAndTangent::GetCrossCirclesPoint() const
{
    return getCurrentCrossPoint(ui->comboBoxResult);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::SetCrossCirclesPoint(const CrossCirclesPoint &p)
{
    const qint32 index = ui->comboBoxResult->findData(static_cast<int>(p));
    if (index != -1)
    {
        ui->comboBoxResult->setCurrentIndex(index);

        VisToolPointFromCircleAndTangent *point = qobject_cast<VisToolPointFromCircleAndTangent *>(vis);
        SCASSERT(point != nullptr);
        point->setCrossPoint(p);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            VisToolPointFromCircleAndTangent *point = qobject_cast<VisToolPointFromCircleAndTangent *>(vis);
            SCASSERT(point != nullptr);

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxTangentPoint, tr("Select a circle center")))
                    {
                        number++;
                        point->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxTangentPoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxCircleCenter, ""))
                        {
                            number = 0;
                            point->setPoint2Id(id);
                            point->RefreshGeometry();
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
void DialogPointFromCircleAndTangent::PointChanged()
{
    QColor color = okColor;
    if (getCurrentObjectId(ui->comboBoxCircleCenter) == getCurrentObjectId(ui->comboBoxTangentPoint))
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelCircleCenter, color);
    ChangeColor(ui->labelTangentPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::DeployCircleRadiusTextEdit()
{
    DeployFormula(ui->plainTextEditRadius, ui->pushButtonGrowRadius, formulaBaseHeightCircleRadius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::CircleRadiusChanged()
{
    labelEditFormula = ui->labelEditRadius;
    labelResultCalculation = ui->labelResultCircleRadius;
    ValFormulaChanged(flagCircleRadius, ui->plainTextEditRadius, timerCircleRadius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::FXCircleRadius()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit radius"));
    dialog->SetFormula(GetCircleRadius());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetCircleRadius(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::EvalCircleRadius()
{
    labelEditFormula = ui->labelEditRadius;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal radius = Eval(ui->plainTextEditRadius->toPlainText(), flagCircleRadius,
                              ui->labelResultCircleRadius, postfix);

    if (radius < 0)
    {
        flagCircleRadius = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultCircleRadius->setText(tr("Error"));
        ui->labelResultCircleRadius->setToolTip(tr("Radius can't be negative"));

        DialogPointFromCircleAndTangent::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::ShowVisualization()
{
    AddVisualization<VisToolPointFromCircleAndTangent>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    QString radius = ui->plainTextEditRadius->toPlainText();
    radius.replace("\n", " ");

    VisToolPointFromCircleAndTangent *point = qobject_cast<VisToolPointFromCircleAndTangent *>(vis);
    SCASSERT(point != nullptr);

    point->setPoint1Id(GetTangentPointId());
    point->setPoint2Id(GetCircleCenterId());
    point->setCRadius(radius);
    point->setCrossPoint(GetCrossCirclesPoint());
    point->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditRadius->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointFromCircleAndTangent::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagFormula && flagName && flagError && flagCircleRadius);
    // In case dialog hasn't apply button
    if ( bApply != nullptr)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}
