/************************************************************************
 **
 **  @file   dialogarcwithlength.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2015
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

#include "dialogarcwithlength.h"
#include "ui_dialogarcwithlength.h"

#include <QPushButton>
#include <QTimer>

#include "../../../vgeometry/vpointf.h"
#include "../../../vpatterndb/vcontainer.h"
#include "../../../vpatterndb/vtranslatevars.h"
#include "../../../ifc/xml/vdomdocument.h"
#include "../../visualization/path/vistoolarcwithlength.h"
#include "../support/dialogeditwrongformula.h"

//---------------------------------------------------------------------------------------------------------------------
DialogArcWithLength::DialogArcWithLength(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogArcWithLength), flagRadius(false), flagF1(false),
      flagLength(false), timerRadius(nullptr), timerF1(nullptr), timerLength(nullptr), radius(QString()), f1(QString()),
      length(QString()),formulaBaseHeightRadius(0), formulaBaseHeightF1(0), formulaBaseHeightLength(0), angleF1(INT_MIN)
{
    ui->setupUi(this);

    plainTextEditFormula = ui->plainTextEditRadius;
    this->formulaBaseHeightLength = ui->plainTextEditRadius->height();
    this->formulaBaseHeightF1 = ui->plainTextEditF1->height();
    this->formulaBaseHeightLength = ui->plainTextEditLength->height();

    ui->plainTextEditRadius->installEventFilter(this);
    ui->plainTextEditF1->installEventFilter(this);
    ui->plainTextEditLength->installEventFilter(this);

    timerRadius = new QTimer(this);
    connect(timerRadius, &QTimer::timeout, this, &DialogArcWithLength::Radius);

    timerF1 = new QTimer(this);
    connect(timerF1, &QTimer::timeout, this, &DialogArcWithLength::EvalF);

    timerLength = new QTimer(this);
    connect(timerLength, &QTimer::timeout, this, &DialogArcWithLength::Length);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxCenter);
    FillComboBoxLineColors(ui->comboBoxColor);

    CheckState();

    connect(ui->toolButtonExprRadius, &QPushButton::clicked, this, &DialogArcWithLength::FXRadius);
    connect(ui->toolButtonExprF1, &QPushButton::clicked, this, &DialogArcWithLength::FXF1);
    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogArcWithLength::FXLength);

    connect(ui->plainTextEditRadius, &QPlainTextEdit::textChanged, this, &DialogArcWithLength::RadiusChanged);
    connect(ui->plainTextEditF1, &QPlainTextEdit::textChanged, this, &DialogArcWithLength::F1Changed);
    connect(ui->plainTextEditLength, &QPlainTextEdit::textChanged, this, &DialogArcWithLength::LengthChanged);

    connect(ui->pushButtonGrowLengthRadius, &QPushButton::clicked, this, &DialogArcWithLength::DeployRadiusTextEdit);
    connect(ui->pushButtonGrowLengthF1, &QPushButton::clicked, this, &DialogArcWithLength::DeployF1TextEdit);
    connect(ui->pushButtonGrowLengthArcLength, &QPushButton::clicked, this, &DialogArcWithLength::DeployLengthTextEdit);

    vis = new VisToolArcWithLength(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogArcWithLength::~DialogArcWithLength()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogArcWithLength::GetCenter() const
{
    return getCurrentObjectId(ui->comboBoxCenter);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::SetCenter(const quint32 &value)
{
    ChangeCurrentData(ui->comboBoxCenter, value);
    vis->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogArcWithLength::GetRadius() const
{
    return qApp->TrVars()->TryFormulaFromUser(radius, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::SetRadius(const QString &value)
{
    radius = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (radius.length() > 80)
    {
        this->DeployRadiusTextEdit();
    }
    ui->plainTextEditRadius->setPlainText(radius);

    VisToolArcWithLength *path = qobject_cast<VisToolArcWithLength *>(vis);
    SCASSERT(path != nullptr);
    path->setRadius(radius);

    MoveCursorToEnd(ui->plainTextEditRadius);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogArcWithLength::GetF1() const
{
    return qApp->TrVars()->TryFormulaFromUser(f1, qApp->Settings()->GetOsSeparator());
}

void DialogArcWithLength::SetF1(const QString &value)
{
    f1 = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (f1.length() > 80)
    {
        this->DeployF1TextEdit();
    }
    ui->plainTextEditF1->setPlainText(f1);

    VisToolArcWithLength *path = qobject_cast<VisToolArcWithLength *>(vis);
    SCASSERT(path != nullptr);
    path->setF1(f1);

    MoveCursorToEnd(ui->plainTextEditF1);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogArcWithLength::GetLength() const
{
    return qApp->TrVars()->TryFormulaFromUser(length, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::SetLength(const QString &value)
{
    length = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (length.length() > 80)
    {
        this->DeployLengthTextEdit();
    }
    ui->plainTextEditLength->setPlainText(length);

    VisToolArcWithLength *path = qobject_cast<VisToolArcWithLength *>(vis);
    SCASSERT(path != nullptr);
    path->setLength(radius);

    MoveCursorToEnd(ui->plainTextEditLength);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogArcWithLength::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            if (SetObject(id, ui->comboBoxCenter, ""))
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
void DialogArcWithLength::DeployRadiusTextEdit()
{
    DeployFormula(ui->plainTextEditRadius, ui->pushButtonGrowLengthArcLength, formulaBaseHeightRadius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::DeployF1TextEdit()
{
    DeployFormula(ui->plainTextEditF1, ui->pushButtonGrowLengthF1, formulaBaseHeightF1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::DeployLengthTextEdit()
{
    DeployFormula(ui->plainTextEditLength, ui->pushButtonGrowLengthArcLength, formulaBaseHeightLength);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::RadiusChanged()
{
    labelEditFormula = ui->labelEditRadius;
    labelResultCalculation = ui->labelResultRadius;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagRadius, ui->plainTextEditRadius, timerRadius, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::F1Changed()
{
    labelEditFormula = ui->labelEditF1;
    labelResultCalculation = ui->labelResultF1;
    ValFormulaChanged(flagF1, ui->plainTextEditF1, timerF1, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::LengthChanged()
{
    labelEditFormula = ui->labelEditLength;
    labelResultCalculation = ui->labelResultLength;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(flagLength, ui->plainTextEditLength, timerLength, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::FXRadius()
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
void DialogArcWithLength::FXF1()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit the first angle"));
    dialog->SetFormula(GetF1());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetF1(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::FXLength()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit the arc length"));
    dialog->SetFormula(GetLength());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetLength(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagRadius && flagF1 && flagLength);
    // In case dialog hasn't apply button
    if ( bApply != nullptr)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::ShowVisualization()
{
    AddVisualization<VisToolArcWithLength>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::SaveData()
{
    radius = ui->plainTextEditRadius->toPlainText();
    radius.replace("\n", " ");

    f1 = ui->plainTextEditF1->toPlainText();
    f1.replace("\n", " ");

    length = ui->plainTextEditLength->toPlainText();
    length.replace("\n", " ");

    VisToolArcWithLength *path = qobject_cast<VisToolArcWithLength *>(vis);
    SCASSERT(path != nullptr);

    path->setObject1Id(GetCenter());
    path->setRadius(radius);
    path->setF1(f1);
    path->setLength(length);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditRadius->blockSignals(true);
    ui->plainTextEditF1->blockSignals(true);
    ui->plainTextEditLength->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::Radius()
{
    labelEditFormula = ui->labelEditRadius;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal radius = Eval(ui->plainTextEditRadius->toPlainText(), flagRadius, ui->labelResultRadius, postfix);

    if (radius < 0)
    {
        flagRadius = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultRadius->setText(tr("Error"));
        ui->labelResultRadius->setToolTip(tr("Radius can't be negative"));

        DialogArcWithLength::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::Length()
{
    labelEditFormula = ui->labelEditLength;
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    const qreal length = Eval(ui->plainTextEditLength->toPlainText(), flagLength, ui->labelResultLength, postfix);

    if (qFuzzyIsNull(length))
    {
        flagLength = false;
        ChangeColor(labelEditFormula, Qt::red);
        ui->labelResultLength->setText(tr("Error"));
        ui->labelResultLength->setToolTip(tr("Length can't be equal 0"));

        DialogArcWithLength::CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::EvalF()
{
    labelEditFormula = ui->labelEditF1;
    angleF1 = Eval(ui->plainTextEditF1->toPlainText(), flagF1, ui->labelResultF1, degreeSymbol, false);
}
