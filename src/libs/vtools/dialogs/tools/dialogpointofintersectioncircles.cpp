/************************************************************************
 **
 **  @file   dialogpointofintersectioncircles.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "dialogpointofintersectioncircles.h"

#include <limits.h>
#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QTimer>
#include <QToolButton>
#include <Qt>

#include "../vpatterndb/vtranslatevars.h"
#include "../vpatterndb/vcontainer.h"
#include "../../visualization/visualization.h"
#include "../../visualization/line/vistoolpointofintersectioncircles.h"
#include "../ifc/xml/vdomdocument.h"
#include "../support/dialogeditwrongformula.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "ui_dialogpointofintersectioncircles.h"

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfIntersectionCircles::DialogPointOfIntersectionCircles(const VContainer *data, quint32 toolId,
                                                                   QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogPointOfIntersectionCircles),
      timerCircle1Radius(new QTimer(this)),
      timerCircle2Radius(new QTimer(this)),
      circle1Radius(),
      circle2Radius(),
      formulaBaseHeightCircle1Radius(0),
      formulaBaseHeightCircle2Radius(0),
      pointName(),
      flagCircle1Radius(false),
      flagCircle2Radius(false),
      flagName(true),
      flagError(true)
{
    ui->setupUi(this);

    ui->lineEditNamePoint->setClearButtonEnabled(true);

    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));

    this->formulaBaseHeightCircle1Radius = ui->plainTextEditCircle1Radius->height();
    this->formulaBaseHeightCircle2Radius = ui->plainTextEditCircle2Radius->height();

    ui->plainTextEditCircle1Radius->installEventFilter(this);
    ui->plainTextEditCircle2Radius->installEventFilter(this);

    timerCircle1Radius->setSingleShot(true);
    connect(timerCircle1Radius, &QTimer::timeout, this, &DialogPointOfIntersectionCircles::EvalCircle1Radius);

    timerCircle2Radius->setSingleShot(true);
    connect(timerCircle2Radius, &QTimer::timeout, this, &DialogPointOfIntersectionCircles::EvalCircle2Radius);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxCircle1Center);
    FillComboBoxPoints(ui->comboBoxCircle2Center);
    FillComboBoxCrossCirclesPoints(ui->comboBoxResult);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, [this]()
    {
        CheckPointLabel(this, ui->lineEditNamePoint, ui->labelEditNamePoint, pointName, this->data, flagName);
        CheckState();
    });
    connect(ui->comboBoxCircle1Center, &QComboBox::currentTextChanged,
            this, &DialogPointOfIntersectionCircles::PointChanged);
    connect(ui->comboBoxCircle2Center, &QComboBox::currentTextChanged,
            this, &DialogPointOfIntersectionCircles::PointChanged);

    connect(ui->toolButtonExprCircle1Radius, &QPushButton::clicked, this,
            &DialogPointOfIntersectionCircles::FXCircle1Radius);
    connect(ui->toolButtonExprCircle2Radius, &QPushButton::clicked, this,
            &DialogPointOfIntersectionCircles::FXCircle2Radius);

    connect(ui->plainTextEditCircle1Radius, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerCircle1Radius->start(formulaTimerTimeout);
    });

    connect(ui->plainTextEditCircle2Radius, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerCircle2Radius->start(formulaTimerTimeout);
    });

    connect(ui->pushButtonGrowCircle1Radius, &QPushButton::clicked, this,
            &DialogPointOfIntersectionCircles::DeployCircle1RadiusTextEdit);
    connect(ui->pushButtonGrowCircle2Radius, &QPushButton::clicked, this,
            &DialogPointOfIntersectionCircles::DeployCircle2RadiusTextEdit);

    vis = new VisToolPointOfIntersectionCircles(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfIntersectionCircles::~DialogPointOfIntersectionCircles()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPointOfIntersectionCircles::GetPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPointOfIntersectionCircles::GetFirstCircleCenterId() const
{
    return getCurrentObjectId(ui->comboBoxCircle1Center);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::SetFirstCircleCenterId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxCircle1Center, value);

    VisToolPointOfIntersectionCircles *point = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
    SCASSERT(point != nullptr)
    point->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPointOfIntersectionCircles::GetSecondCircleCenterId() const
{
    return getCurrentObjectId(ui->comboBoxCircle2Center);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::SetSecondCircleCenterId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxCircle2Center, value);

    VisToolPointOfIntersectionCircles *point = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
    SCASSERT(point != nullptr)
    point->setObject2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPointOfIntersectionCircles::GetFirstCircleRadius() const
{
    return qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditCircle1Radius->toPlainText(),
                                              qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::SetFirstCircleRadius(const QString &value)
{
    const QString formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployCircle1RadiusTextEdit();
    }
    ui->plainTextEditCircle1Radius->setPlainText(formula);

    VisToolPointOfIntersectionCircles *point = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
    SCASSERT(point != nullptr)
    point->setC1Radius(formula);

    MoveCursorToEnd(ui->plainTextEditCircle1Radius);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPointOfIntersectionCircles::GetSecondCircleRadius() const
{
    return qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditCircle2Radius->toPlainText(),
                                              qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::SetSecondCircleRadius(const QString &value)
{
    const QString formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployCircle2RadiusTextEdit();
    }
    ui->plainTextEditCircle2Radius->setPlainText(formula);

    VisToolPointOfIntersectionCircles *point = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
    SCASSERT(point != nullptr)
    point->setC2Radius(formula);

    MoveCursorToEnd(ui->plainTextEditCircle2Radius);
}

//---------------------------------------------------------------------------------------------------------------------
CrossCirclesPoint DialogPointOfIntersectionCircles::GetCrossCirclesPoint() const
{
    return getCurrentCrossPoint<CrossCirclesPoint>(ui->comboBoxResult);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::SetCrossCirclesPoint(const CrossCirclesPoint &p)
{
    const qint32 index = ui->comboBoxResult->findData(static_cast<int>(p));
    if (index != -1)
    {
        ui->comboBoxResult->setCurrentIndex(index);

        VisToolPointOfIntersectionCircles *point = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
        SCASSERT(point != nullptr)
        point->setCrossPoint(p);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            VisToolPointOfIntersectionCircles *point = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
            SCASSERT(point != nullptr)

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxCircle1Center, tr("Select second circle center")))
                    {
                        number++;
                        point->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxCircle1Center) != id)
                    {
                        if (SetObject(id, ui->comboBoxCircle2Center, QString()))
                        {
                            number = 0;
                            point->setObject2Id(id);
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
void DialogPointOfIntersectionCircles::PointChanged()
{
    QColor color;
    if (getCurrentObjectId(ui->comboBoxCircle1Center) == getCurrentObjectId(ui->comboBoxCircle2Center))
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = OkColor(this);
    }
    ChangeColor(ui->labelCircle1Center, color);
    ChangeColor(ui->labelCircle1Center, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::DeployCircle1RadiusTextEdit()
{
    DeployFormula(this, ui->plainTextEditCircle1Radius, ui->pushButtonGrowCircle1Radius, formulaBaseHeightCircle1Radius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::DeployCircle2RadiusTextEdit()
{
    DeployFormula(this, ui->plainTextEditCircle2Radius, ui->pushButtonGrowCircle2Radius, formulaBaseHeightCircle2Radius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::FXCircle1Radius()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit first circle radius"));
    dialog->SetFormula(GetFirstCircleRadius());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFirstCircleRadius(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::FXCircle2Radius()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit second circle radius"));
    dialog->SetFormula(GetSecondCircleRadius());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetSecondCircleRadius(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::EvalCircle1Radius()
{
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditCircle1Radius->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditCircle1Radius;
    formulaData.labelResult = ui->labelResultCircle1Radius;
    formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);

    const qreal radius = Eval(formulaData, flagCircle1Radius);

    if (radius < 0)
    {
        flagCircle2Radius = false;
        ChangeColor(ui->labelEditCircle1Radius, errorColor);
        ui->labelResultCircle1Radius->setText(tr("Error"));
        ui->labelResultCircle1Radius->setToolTip(tr("Radius can't be negative"));

        DialogPointOfIntersectionCircles::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::EvalCircle2Radius()
{
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditCircle2Radius->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditCircle2Radius;
    formulaData.labelResult = ui->labelResultCircle2Radius;
    formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);

    const qreal radius = Eval(formulaData, flagCircle2Radius);

    if (radius < 0)
    {
        flagCircle2Radius = false;
        ChangeColor(ui->labelEditCircle2Radius, errorColor);
        ui->labelResultCircle2Radius->setText(tr("Error"));
        ui->labelResultCircle2Radius->setToolTip(tr("Radius can't be negative"));

        DialogPointOfIntersectionCircles::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::ShowVisualization()
{
    AddVisualization<VisToolPointOfIntersectionCircles>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    VisToolPointOfIntersectionCircles *point = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
    SCASSERT(point != nullptr)

    point->setObject1Id(GetFirstCircleCenterId());
    point->setObject2Id(GetSecondCircleCenterId());
    point->setC1Radius(ui->plainTextEditCircle2Radius->toPlainText());
    point->setC2Radius(ui->plainTextEditCircle2Radius->toPlainText());
    point->setCrossPoint(GetCrossCirclesPoint());
    point->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditCircle1Radius->blockSignals(true);
    ui->plainTextEditCircle2Radius->blockSignals(true);
    DialogTool::closeEvent(event);
}
