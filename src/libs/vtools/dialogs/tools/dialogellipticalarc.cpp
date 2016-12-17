/************************************************************************
 **
 **  @file   dialogellipticalarc.cpp
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   15 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "dialogellipticalarc.h"

#include <limits.h>
#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QTimer>
#include <QToolButton>
#include <Qt>

#include "../ifc/xml/vdomdocument.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../../visualization/path/vistoolellipticalarc.h"
#include "../../visualization/visualization.h"
#include "../support/dialogeditwrongformula.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "ui_dialogellipticalarc.h"

class QCloseEvent;
class QWidget;
class VContainer;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogEllipticalArc create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogEllipticalArc::DialogEllipticalArc(const VContainer *data, const quint32 &toolId, QWidget *parent)
:DialogTool(data, toolId, parent), ui(new Ui::DialogEllipticalArc)
{
    ui->setupUi(this);

    this->formulaBaseHeightRadius1 = ui->plainTextEditRadius1->height();
    this->formulaBaseHeightRadius2 = ui->plainTextEditRadius2->height();
    this->formulaBaseHeightF1 = ui->plainTextEditF1->height();
    this->formulaBaseHeightF2 = ui->plainTextEditF2->height();
    this->formulaBaseHeightRotationAngle = ui->plainTextEditRotationAngle->height();

    ui->plainTextEditRadius1->installEventFilter(this);
    ui->plainTextEditRadius2->installEventFilter(this);
    ui->plainTextEditF1->installEventFilter(this);
    ui->plainTextEditF2->installEventFilter(this);
    ui->plainTextEditRotationAngle->installEventFilter(this);

    timerRadius1 = new QTimer(this);
    connect(timerRadius1, &QTimer::timeout, this, &DialogEllipticalArc::EvalRadiuses);

    timerRadius2 = new QTimer(this);
    connect(timerRadius2, &QTimer::timeout, this, &DialogEllipticalArc::EvalRadiuses);

    timerF1 = new QTimer(this);
    connect(timerF1, &QTimer::timeout, this, &DialogEllipticalArc::EvalAngles);

    timerF2 = new QTimer(this);
    connect(timerF2, &QTimer::timeout, this, &DialogEllipticalArc::EvalAngles);

    timerRotationAngle = new QTimer(this);
    connect(timerRotationAngle, &QTimer::timeout, this, &DialogEllipticalArc::EvalAngles);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxLineColors(ui->comboBoxColor);

    CheckState();

    connect(ui->toolButtonExprRadius1, &QPushButton::clicked, this, &DialogEllipticalArc::FXRadius1);
    connect(ui->toolButtonExprRadius2, &QPushButton::clicked, this, &DialogEllipticalArc::FXRadius2);
    connect(ui->toolButtonExprF1, &QPushButton::clicked, this, &DialogEllipticalArc::FXF1);
    connect(ui->toolButtonExprF2, &QPushButton::clicked, this, &DialogEllipticalArc::FXF2);
    connect(ui->toolButtonExprRotationAngle, &QPushButton::clicked, this, &DialogEllipticalArc::FXRotationAngle);

    connect(ui->plainTextEditRadius1, &QPlainTextEdit::textChanged, this, &DialogEllipticalArc::Radius1Changed);
    connect(ui->plainTextEditRadius2, &QPlainTextEdit::textChanged, this, &DialogEllipticalArc::Radius2Changed);
    connect(ui->plainTextEditF1, &QPlainTextEdit::textChanged, this, &DialogEllipticalArc::F1Changed);
    connect(ui->plainTextEditF2, &QPlainTextEdit::textChanged, this, &DialogEllipticalArc::F2Changed);
    connect(ui->plainTextEditRotationAngle, &QPlainTextEdit::textChanged,
            this, &DialogEllipticalArc::RotationAngleChanged);

    connect(ui->pushButtonGrowLengthRadius1, &QPushButton::clicked, this, &DialogEllipticalArc::DeployRadius1TextEdit);
    connect(ui->pushButtonGrowLengthRadius2, &QPushButton::clicked, this, &DialogEllipticalArc::DeployRadius2TextEdit);
    connect(ui->pushButtonGrowLengthF1, &QPushButton::clicked, this, &DialogEllipticalArc::DeployF1TextEdit);
    connect(ui->pushButtonGrowLengthF2, &QPushButton::clicked, this, &DialogEllipticalArc::DeployF2TextEdit);
    connect(ui->pushButtonGrowLengthRotationAngle, &QPushButton::clicked,
            this, &DialogEllipticalArc::DeployRotationAngleTextEdit);

    vis = new VisToolEllipticalArc(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogEllipticalArc::~DialogEllipticalArc()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetCenter return id of center point
 * @return id id
 */
quint32 DialogEllipticalArc::GetCenter() const
{
    return getCurrentObjectId(ui->comboBoxBasePoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetCenter set id of center point
 * @param value id
 */
void DialogEllipticalArc::SetCenter(const quint32 &value)
{
    ChangeCurrentData(ui->comboBoxBasePoint, value);
    vis->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius1 return formula of radius1
 * @return formula
 */
QString DialogEllipticalArc::GetRadius1() const
{
    return qApp->TrVars()->TryFormulaFromUser(radius1, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetRadius1 set formula of radius1
 * @param value formula
 */
void DialogEllipticalArc::SetRadius1(const QString &value)
{
    radius1 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (radius1.length() > 80)
    {
        this->DeployRadius1TextEdit();
    }
    ui->plainTextEditRadius1->setPlainText(radius1);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr);
    path->setRadius1(radius1);

    MoveCursorToEnd(ui->plainTextEditRadius1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius2 return formula of radius2
 * @return formula
 */
QString DialogEllipticalArc::GetRadius2() const
{
    return qApp->TrVars()->TryFormulaFromUser(radius2, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetRadius2 set formula of radius2
 * @param value formula
 */
void DialogEllipticalArc::SetRadius2(const QString &value)
{
    radius2 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (radius2.length() > 80)
    {
        this->DeployRadius2TextEdit();
    }
    ui->plainTextEditRadius2->setPlainText(radius2);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr);
    path->setRadius2(radius2);

    MoveCursorToEnd(ui->plainTextEditRadius2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return formula first angle of elliptical arc
 * @return formula
 */
QString DialogEllipticalArc::GetF1() const
{
    return qApp->TrVars()->TryFormulaFromUser(f1, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF1 set formula first angle of elliptical arc
 * @param value formula
 */
void DialogEllipticalArc::SetF1(const QString &value)
{
    f1 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (f1.length() > 80)
    {
        this->DeployF1TextEdit();
    }
    ui->plainTextEditF1->setPlainText(f1);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr);
    path->setF1(f1);

    MoveCursorToEnd(ui->plainTextEditF1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return formula second angle of elliptical arc
 * @return formula
 */
QString DialogEllipticalArc::GetF2() const
{
    return qApp->TrVars()->TryFormulaFromUser(f2, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF2 set formula second angle of elliptical arc
 * @param value formula
 */
void DialogEllipticalArc::SetF2(const QString &value)
{
    f2 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (f2.length() > 80)
    {
        this->DeployF2TextEdit();
    }
    ui->plainTextEditF2->setPlainText(f2);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr);
    path->setF2(f2);

    MoveCursorToEnd(ui->plainTextEditF2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRotationAngle return formula rotation angle of elliptical arc
 * @return formula
 */
QString DialogEllipticalArc::GetRotationAngle() const
{
    return qApp->TrVars()->TryFormulaFromUser(rotationAngle, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetRotationAngle set formula rotation angle of elliptical arc
 * @param value formula
 */
void DialogEllipticalArc::SetRotationAngle(const QString &value)
{
    rotationAngle = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (rotationAngle.length() > 80)
    {
        this->DeployRotationAngleTextEdit();
    }
    ui->plainTextEditRotationAngle->setPlainText(rotationAngle);

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr);
    path->setRotationAngle(rotationAngle);

    MoveCursorToEnd(ui->plainTextEditRotationAngle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetColor return color of elliptical arc
 * @return formula
 */
QString DialogEllipticalArc::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetColor set color of elliptical arc
 * @param value formula
 */
void DialogEllipticalArc::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalRadiuses calculate value of radiuses
 */
void DialogEllipticalArc::EvalRadiuses()
{
    labelEditFormula = ui->labelEditRadius1;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal radius_1 = Eval(ui->plainTextEditRadius1->toPlainText(), flagRadius1, ui->labelResultRadius1, postfix);

    if (radius_1 < 0)
    {
        flagRadius1 = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultRadius1->setText(tr("Error"));
        ui->labelResultRadius1->setToolTip(tr("Radius can't be negative"));

        DialogEllipticalArc::CheckState();
    }

    labelEditFormula = ui->labelEditRadius2;
    const qreal radius_2 = Eval(ui->plainTextEditRadius2->toPlainText(), flagRadius2, ui->labelResultRadius2, postfix);
    if (radius_2 < 0)
    {
        flagRadius2 = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultRadius2->setText(tr("Error"));
        ui->labelResultRadius2->setToolTip(tr("Radius can't be negative"));

        DialogEllipticalArc::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalAngles calculate value of angles
 */
void DialogEllipticalArc::EvalAngles()
{
    labelEditFormula = ui->labelEditF1;
    angleF1 = Eval(ui->plainTextEditF1->toPlainText(), flagF1, ui->labelResultF1, degreeSymbol, false);

    labelEditFormula = ui->labelEditF2;
    angleF2 = Eval(ui->plainTextEditF2->toPlainText(), flagF2, ui->labelResultF2, degreeSymbol, false);

    labelEditFormula = ui->labelEditRotationAngle;
    rotationAngle = Eval(ui->plainTextEditRotationAngle->toPlainText(), flagRotationAngle,
                         ui->labelResultRotationAngle, degreeSymbol, false);

    CheckAngles();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::CheckAngles()
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

    DialogEllipticalArc::CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::FXRadius1()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit radius1"));
    dialog->SetFormula(GetRadius1());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetRadius1(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::FXRadius2()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit radius2"));
    dialog->SetFormula(GetRadius2());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetRadius2(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::FXF1()
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
void DialogEllipticalArc::FXF2()
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
void DialogEllipticalArc::FXRotationAngle()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit rotation angle"));
    dialog->SetFormula(GetRotationAngle());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetRotationAngle(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Radius1Changed after change formula of radius1 calculate value and show result
 */
void DialogEllipticalArc::Radius1Changed()
{
    labelEditFormula = ui->labelEditRadius1;
    labelResultCalculation = ui->labelResultRadius1;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagRadius1, ui->plainTextEditRadius1, timerRadius1, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Radius2Changed after change formula of radius2 calculate value and show result
 */
void DialogEllipticalArc::Radius2Changed()
{
    labelEditFormula = ui->labelEditRadius2;
    labelResultCalculation = ui->labelResultRadius2;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagRadius2, ui->plainTextEditRadius2, timerRadius2, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief F1Changed after change formula of first angle calculate value and show result
 */
void DialogEllipticalArc::F1Changed()
{
    labelEditFormula = ui->labelEditF1;
    labelResultCalculation = ui->labelResultF1;
    ValFormulaChanged(flagF1, ui->plainTextEditF1, timerF1, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief F2Changed after change formula of second angle calculate value and show result
 */
void DialogEllipticalArc::F2Changed()
{
    labelEditFormula = ui->labelEditF2;
    labelResultCalculation = ui->labelResultF2;
    ValFormulaChanged(flagF2, ui->plainTextEditF2, timerF2, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RotationAngleChanged after change formula of rotation angle calculate value and show result
 */
void DialogEllipticalArc::RotationAngleChanged()
{
    labelEditFormula = ui->labelEditRotationAngle;
    labelResultCalculation = ui->labelResultF2;
    ValFormulaChanged(flagRotationAngle, ui->plainTextEditRotationAngle, timerRotationAngle, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployRadius1TextEdit()
{
    DeployFormula(ui->plainTextEditRadius1, ui->pushButtonGrowLengthRadius1, formulaBaseHeightRadius1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployRadius2TextEdit()
{
    DeployFormula(ui->plainTextEditRadius2, ui->pushButtonGrowLengthRadius2, formulaBaseHeightRadius2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployF1TextEdit()
{
    DeployFormula(ui->plainTextEditF1, ui->pushButtonGrowLengthF1, formulaBaseHeightF1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployF2TextEdit()
{
    DeployFormula(ui->plainTextEditF2, ui->pushButtonGrowLengthF2, formulaBaseHeightF2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::DeployRotationAngleTextEdit()
{
    DeployFormula(ui->plainTextEditRotationAngle, ui->pushButtonGrowLengthRotationAngle,formulaBaseHeightRotationAngle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogEllipticalArc::ChosenObject(quint32 id, const SceneObject &type)
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
/**
 * @brief CheckState if all is right enable button ok
 */
void DialogEllipticalArc::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagRadius1 && flagRadius2 && flagF1 && flagF2 && flagRotationAngle);
    SCASSERT(bApply != nullptr);
    bApply->setEnabled(flagRadius1 && flagRadius2 && flagF1 && flagF2 && flagRotationAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::ShowVisualization()
{
    AddVisualization<VisToolEllipticalArc>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::SaveData()
{
    radius1 = ui->plainTextEditRadius1->toPlainText();
    radius1.replace("\n", " ");
    radius2 = ui->plainTextEditRadius2->toPlainText();
    radius2.replace("\n", " ");
    f1 = ui->plainTextEditF1->toPlainText();
    f1.replace("\n", " ");
    f2 = ui->plainTextEditF2->toPlainText();
    f2.replace("\n", " ");
    rotationAngle = ui->plainTextEditRotationAngle->toPlainText();
    rotationAngle.replace("\n", " ");

    VisToolEllipticalArc *path = qobject_cast<VisToolEllipticalArc *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(GetCenter());
    path->setRadius1(radius1);
    path->setRadius2(radius2);
    path->setF1(f1);
    path->setF2(f2);
    path->setRotationAngle(rotationAngle);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEllipticalArc::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditRadius1->blockSignals(true);
    ui->plainTextEditRadius2->blockSignals(true);
    ui->plainTextEditF1->blockSignals(true);
    ui->plainTextEditF2->blockSignals(true);
    ui->plainTextEditRotationAngle->blockSignals(true);
    DialogTool::closeEvent(event);
}
