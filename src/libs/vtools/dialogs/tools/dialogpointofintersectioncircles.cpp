/************************************************************************
 **
 **  @file   dialogpointofintersectioncircles.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2015
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

#include "dialogpointofintersectioncircles.h"
#include "ui_dialogpointofintersectioncircles.h"

#include "../../../vgeometry/vpointf.h"
#include "../../../vpatterndb/vcontainer.h"
#include "../../../vpatterndb/vtranslatevars.h"
#include "../../visualization/vistoolpointofintersectioncircles.h"
#include "../../../vwidgets/vmaingraphicsscene.h"
#include "../support/dialogeditwrongformula.h"

#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfIntersectionCircles::DialogPointOfIntersectionCircles(const VContainer *data, const quint32 &toolId,
                                                                   QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogPointOfIntersectionCircles), flagCircle1Radius(false),
      flagCircle2Radius(false), timerCircle1Radius(nullptr), timerCircle2Radius(nullptr), circle1Radius(),
      circle2Radius(), formulaBaseHeightCircle1Radius(0), formulaBaseHeightCircle2Radius(0),
      angleCircle1Radius(INT_MIN), angleCircle2Radius(INT_MIN)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditNamePoint->setClearButtonEnabled(true);
#endif

    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;

    plainTextEditFormula = ui->plainTextEditCircle1Radius;
    this->formulaBaseHeightCircle1Radius = ui->plainTextEditCircle1Radius->height();
    this->formulaBaseHeightCircle2Radius = ui->plainTextEditCircle2Radius->height();

    ui->plainTextEditCircle1Radius->installEventFilter(this);
    ui->plainTextEditCircle2Radius->installEventFilter(this);

    timerCircle1Radius = new QTimer(this);
    connect(timerCircle1Radius, &QTimer::timeout, this, &DialogPointOfIntersectionCircles::EvalCircle1Radius);

    timerCircle2Radius = new QTimer(this);
    connect(timerCircle2Radius, &QTimer::timeout, this, &DialogPointOfIntersectionCircles::EvalCircle2Radius);

    InitOkCancelApply(ui);
    CheckState();

    FillComboBoxPoints(ui->comboBoxCircle1Center);
    FillComboBoxPoints(ui->comboBoxCircle2Center);
    FillComboBoxCrossCirclesPoints(ui->comboBoxResult);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfIntersectionCircles::NamePointChanged);
    connect(ui->comboBoxCircle1Center,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfIntersectionCircles::PointChanged);
    connect(ui->comboBoxCircle2Center,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfIntersectionCircles::PointChanged);

    connect(ui->toolButtonExprCircle1Radius, &QPushButton::clicked, this,
            &DialogPointOfIntersectionCircles::FXCircle1Radius);
    connect(ui->toolButtonExprCircle2Radius, &QPushButton::clicked, this,
            &DialogPointOfIntersectionCircles::FXCircle2Radius);

    connect(ui->plainTextEditCircle1Radius, &QPlainTextEdit::textChanged, this,
            &DialogPointOfIntersectionCircles::Circle1RadiusChanged);
    connect(ui->plainTextEditCircle2Radius, &QPlainTextEdit::textChanged, this,
            &DialogPointOfIntersectionCircles::Circle2RadiusChanged);

    connect(ui->pushButtonGrowCircle1Radius, &QPushButton::clicked, this,
            &DialogPointOfIntersectionCircles::DeployCircle1RadiusTextEdit);
    connect(ui->pushButtonGrowCircle2Radius, &QPushButton::clicked, this,
            &DialogPointOfIntersectionCircles::DeployCircle2RadiusTextEdit);

    vis = new VisToolPointOfIntersectionCircles(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfIntersectionCircles::~DialogPointOfIntersectionCircles()
{
    DeleteVisualization<VisToolPointOfIntersectionCircles>();
    delete ui;
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
    SCASSERT(point != nullptr);
    point->setPoint1Id(value);
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
    SCASSERT(point != nullptr);
    point->setPoint2Id(value);
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
    SCASSERT(point != nullptr);
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
    SCASSERT(point != nullptr);
    point->setC2Radius(formula);

    MoveCursorToEnd(ui->plainTextEditCircle2Radius);
}

//---------------------------------------------------------------------------------------------------------------------
CrossCirclesPoint DialogPointOfIntersectionCircles::GetCrossCirclesPoint() const
{
    return getCurrentCrossPoint(ui->comboBoxResult);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::SetCrossCirclesPoint(const CrossCirclesPoint &p)
{
    const qint32 index = ui->comboBoxResult->findData(static_cast<int>(p));
    if (index != -1)
    {
        ui->comboBoxResult->setCurrentIndex(index);

        VisToolPointOfIntersectionCircles *point = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
        SCASSERT(point != nullptr);
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
            SCASSERT(point != nullptr);

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
                        if (SetObject(id, ui->comboBoxCircle2Center, ""))
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
void DialogPointOfIntersectionCircles::PointChanged()
{
    QColor color = okColor;
    if (getCurrentObjectId(ui->comboBoxCircle1Center) == getCurrentObjectId(ui->comboBoxCircle2Center))
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelCircle1Center, color);
    ChangeColor(ui->labelCircle1Center, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::DeployCircle1RadiusTextEdit()
{
    DeployFormula(ui->plainTextEditCircle1Radius, ui->pushButtonGrowCircle1Radius, formulaBaseHeightCircle1Radius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::DeployCircle2RadiusTextEdit()
{
    DeployFormula(ui->plainTextEditCircle2Radius, ui->pushButtonGrowCircle2Radius, formulaBaseHeightCircle2Radius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::Circle1RadiusChanged()
{
    labelEditFormula = ui->labelEditCircle1Radius;
    labelResultCalculation = ui->labelResultCircle1Radius;
    ValFormulaChanged(flagCircle1Radius, ui->plainTextEditCircle1Radius, timerCircle1Radius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::Circle2RadiusChanged()
{
    labelEditFormula = ui->labelEditCircle2Radius;
    labelResultCalculation = ui->labelResultCircle2Radius;
    ValFormulaChanged(flagCircle2Radius, ui->plainTextEditCircle2Radius, timerCircle2Radius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::FXCircle1Radius()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit first circle radius"));
    dialog->SetFormula(GetFirstCircleRadius());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
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
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetSecondCircleRadius(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::EvalCircle1Radius()
{
    labelEditFormula = ui->labelEditCircle1Radius;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal radius = Eval(ui->plainTextEditCircle1Radius->toPlainText(), flagCircle1Radius,
                              ui->labelResultCircle1Radius, postfix);

    if (radius < 0)
    {
        flagCircle2Radius = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultCircle1Radius->setText(tr("Error"));
        ui->labelResultCircle1Radius->setToolTip(tr("Radius can't be negative"));

        DialogPointOfIntersectionCircles::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::EvalCircle2Radius()
{
    labelEditFormula = ui->labelEditCircle2Radius;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal radius = Eval(ui->plainTextEditCircle2Radius->toPlainText(), flagCircle2Radius,
                              ui->labelResultCircle2Radius, postfix);

    if (radius < 0)
    {
        flagCircle2Radius = false;
        ChangeColor(labelEditFormula, Qt::red);
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

    QString c1Radius = ui->plainTextEditCircle2Radius->toPlainText();
    c1Radius.replace("\n", " ");

    QString c2Radius = ui->plainTextEditCircle2Radius->toPlainText();
    c2Radius.replace("\n", " ");

    VisToolPointOfIntersectionCircles *point = qobject_cast<VisToolPointOfIntersectionCircles *>(vis);
    SCASSERT(point != nullptr);

    point->setPoint1Id(GetFirstCircleCenterId());
    point->setPoint2Id(GetSecondCircleCenterId());
    point->setC1Radius(c1Radius);
    point->setC2Radius(c2Radius);
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

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersectionCircles::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagFormula && flagName && flagError && flagCircle1Radius && flagCircle2Radius);
    // In case dialog hasn't apply button
    if ( bApply != nullptr)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}
