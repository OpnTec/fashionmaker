/************************************************************************
 **
 **  @file   dialogarc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "dialogarc.h"

#include <limits.h>
#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QTimer>
#include <QToolButton>
#include <Qt>

#include "../../../ifc/xml/vdomdocument.h"
#include "../../../vpatterndb/vtranslatevars.h"
#include "../../visualization/path/vistoolarc.h"
#include "../support/dialogeditwrongformula.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "dialogs/tools/../../visualization/path/../visualization.h"
#include "ui_dialogarc.h"

class QCloseEvent;
class QWidget;
class VContainer;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogArc create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogArc::DialogArc(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogArc), flagRadius(false), flagF1(false), flagF2(false),
      timerRadius(nullptr), timerF1(nullptr), timerF2(nullptr), radius(QString()), f1(QString()), f2(QString()),
      formulaBaseHeight(0), formulaBaseHeightF1(0), formulaBaseHeightF2(0), angleF1(INT_MIN), angleF2(INT_MIN)
{
    ui->setupUi(this);

    plainTextEditFormula = ui->plainTextEditFormula;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    this->formulaBaseHeightF1 = ui->plainTextEditF1->height();
    this->formulaBaseHeightF2 = ui->plainTextEditF2->height();

    ui->plainTextEditFormula->installEventFilter(this);
    ui->plainTextEditF1->installEventFilter(this);
    ui->plainTextEditF2->installEventFilter(this);

    timerRadius = new QTimer(this);
    connect(timerRadius, &QTimer::timeout, this, &DialogArc::EvalRadius);

    timerF1 = new QTimer(this);
    connect(timerF1, &QTimer::timeout, this, &DialogArc::EvalF);

    timerF2 = new QTimer(this);
    connect(timerF2, &QTimer::timeout, this, &DialogArc::EvalF);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxLineColors(ui->comboBoxColor);

    CheckState();

    connect(ui->toolButtonExprRadius, &QPushButton::clicked, this, &DialogArc::FXRadius);
    connect(ui->toolButtonExprF1, &QPushButton::clicked, this, &DialogArc::FXF1);
    connect(ui->toolButtonExprF2, &QPushButton::clicked, this, &DialogArc::FXF2);

    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogArc::RadiusChanged);
    connect(ui->plainTextEditF1, &QPlainTextEdit::textChanged, this, &DialogArc::F1Changed);
    connect(ui->plainTextEditF2, &QPlainTextEdit::textChanged, this, &DialogArc::F2Changed);

    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogArc::DeployFormulaTextEdit);
    connect(ui->pushButtonGrowLengthF1, &QPushButton::clicked, this, &DialogArc::DeployF1TextEdit);
    connect(ui->pushButtonGrowLengthF2, &QPushButton::clicked, this, &DialogArc::DeployF2TextEdit);

    vis = new VisToolArc(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::DeployF1TextEdit()
{
    DeployFormula(ui->plainTextEditF1, ui->pushButtonGrowLengthF1, formulaBaseHeightF1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::DeployF2TextEdit()
{
    DeployFormula(ui->plainTextEditF2, ui->pushButtonGrowLengthF2, formulaBaseHeightF2);
}

//---------------------------------------------------------------------------------------------------------------------
DialogArc::~DialogArc()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetCenter set id of center point
 * @param value id
 */
void DialogArc::SetCenter(const quint32 &value)
{
    ChangeCurrentData(ui->comboBoxBasePoint, value);
    vis->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF2 set formula second angle of arc
 * @param value formula
 */
void DialogArc::SetF2(const QString &value)
{
    f2 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (f2.length() > 80)
    {
        this->DeployF2TextEdit();
    }
    ui->plainTextEditF2->setPlainText(f2);

    VisToolArc *path = qobject_cast<VisToolArc *>(vis);
    SCASSERT(path != nullptr);
    path->setF2(f2);

    MoveCursorToEnd(ui->plainTextEditF2);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogArc::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF1 set formula first angle of arc
 * @param value formula
 */
void DialogArc::SetF1(const QString &value)
{
    f1 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (f1.length() > 80)
    {
        this->DeployF1TextEdit();
    }
    ui->plainTextEditF1->setPlainText(f1);

    VisToolArc *path = qobject_cast<VisToolArc *>(vis);
    SCASSERT(path != nullptr);
    path->setF1(f1);

    MoveCursorToEnd(ui->plainTextEditF1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetRadius set formula of radius
 * @param value formula
 */
void DialogArc::SetRadius(const QString &value)
{
    radius = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (radius.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(radius);

    VisToolArc *path = qobject_cast<VisToolArc *>(vis);
    SCASSERT(path != nullptr);
    path->setRadius(radius);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogArc::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            if (SetObject(id, ui->comboBoxBasePoint, ""))
            {
                vis->VisualMode(id);
                prepare = true;
                this->setModal(true);
                this->show();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::ShowVisualization()
{
    AddVisualization<VisToolArc>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::SaveData()
{
    radius = ui->plainTextEditFormula->toPlainText();
    radius.replace("\n", " ");
    f1 = ui->plainTextEditF1->toPlainText();
    f1.replace("\n", " ");
    f2 = ui->plainTextEditF2->toPlainText();
    f2.replace("\n", " ");

    VisToolArc *path = qobject_cast<VisToolArc *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(GetCenter());
    path->setRadius(radius);
    path->setF1(f1);
    path->setF2(f2);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    ui->plainTextEditF1->blockSignals(true);
    ui->plainTextEditF2->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RadiusChanged after change formula of radius calculate value and show result
 */
void DialogArc::RadiusChanged()
{
    labelEditFormula = ui->labelEditRadius;
    labelResultCalculation = ui->labelResultRadius;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagRadius, ui->plainTextEditFormula, timerRadius, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief F1Changed after change formula of first angle calculate value and show result
 */
void DialogArc::F1Changed()
{
    labelEditFormula = ui->labelEditF1;
    labelResultCalculation = ui->labelResultF1;
    ValFormulaChanged(flagF1, ui->plainTextEditF1, timerF1, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief F2Changed after change formula of second angle calculate value and show result
 */
void DialogArc::F2Changed()
{
    labelEditFormula = ui->labelEditF2;
    labelResultCalculation = ui->labelResultF2;
    ValFormulaChanged(flagF2, ui->plainTextEditF2, timerF2, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::FXRadius()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit radius"));
    dialog->SetFormula(GetRadius());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetRadius(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::FXF1()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit first angle"));
    dialog->SetFormula(GetF1());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetF1(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::FXF2()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit second angle"));
    dialog->SetFormula(GetF2());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetF2(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckState if all is right enable button ok
 */
void DialogArc::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagRadius && flagF1 && flagF2);
    SCASSERT(bApply != nullptr);
    bApply->setEnabled(flagRadius && flagF1 && flagF2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalRadius calculate value of radius
 */
void DialogArc::EvalRadius()
{
    labelEditFormula = ui->labelEditRadius;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal radius = Eval(ui->plainTextEditFormula->toPlainText(), flagRadius, ui->labelResultRadius, postfix);

    if (radius < 0)
    {
        flagRadius = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultRadius->setText(tr("Error"));
        ui->labelResultRadius->setToolTip(tr("Radius can't be negative"));

        DialogArc::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalF1 calculate value of angle
 */
void DialogArc::EvalF()
{
    labelEditFormula = ui->labelEditF1;
    angleF1 = Eval(ui->plainTextEditF1->toPlainText(), flagF1, ui->labelResultF1, degreeSymbol, false);

    labelEditFormula = ui->labelEditF2;
    angleF2 = Eval(ui->plainTextEditF2->toPlainText(), flagF2, ui->labelResultF2, degreeSymbol, false);

    CheckAngles();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::CheckAngles()
{
    if (static_cast<int>(angleF1) == INT_MIN || static_cast<int>(angleF2) == INT_MIN)
    {
        return;
    }

    if (VFuzzyComparePossibleNulls(angleF1, angleF2))
    {
        flagF1 = false;
        ChangeColor(ui->labelEditF1, Qt::red);
        ui->labelResultF1->setText(tr("Error"));
        ui->labelResultF1->setToolTip(tr("Angles equal"));

        flagF2 = false;
        ChangeColor(ui->labelEditF2, Qt::red);
        ui->labelResultF2->setText(tr("Error"));
        ui->labelResultF2->setToolTip(tr("Angles equal"));
    }

    DialogArc::CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetCenter return id of center point
 * @return id id
 */
quint32 DialogArc::GetCenter() const
{
    return getCurrentObjectId(ui->comboBoxBasePoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return formula of radius
 * @return formula
 */
QString DialogArc::GetRadius() const
{
    return qApp->TrVars()->TryFormulaFromUser(radius, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return formula first angle of arc
 * @return formula
 */
QString DialogArc::GetF1() const
{
    return qApp->TrVars()->TryFormulaFromUser(f1, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return formula second angle of arc
 * @return formula
 */
QString DialogArc::GetF2() const
{
    return qApp->TrVars()->TryFormulaFromUser(f2, qApp->Settings()->GetOsSeparator());
}
