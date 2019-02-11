/************************************************************************
 **
 **  @file   dialogarcwithlength.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2015
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

#include "dialogarcwithlength.h"

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
#include "../vpatterndb/vcontainer.h"
#include "../../visualization/path/vistoolarcwithlength.h"
#include "../support/dialogeditwrongformula.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../../visualization/visualization.h"
#include "ui_dialogarcwithlength.h"

//---------------------------------------------------------------------------------------------------------------------
DialogArcWithLength::DialogArcWithLength(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogArcWithLength),
      flagRadius(false),
      flagF1(false),
      flagLength(false),
      timerRadius(new QTimer(this)),
      timerF1(new QTimer(this)),
      timerLength(new QTimer(this)),
      radius(),
      f1(),
      length(),
      formulaBaseHeightRadius(0),
      formulaBaseHeightF1(0),
      formulaBaseHeightLength(0)
{
    ui->setupUi(this);

    this->formulaBaseHeightRadius = ui->plainTextEditRadius->height();
    this->formulaBaseHeightF1 = ui->plainTextEditF1->height();
    this->formulaBaseHeightLength = ui->plainTextEditLength->height();

    ui->plainTextEditRadius->installEventFilter(this);
    ui->plainTextEditF1->installEventFilter(this);
    ui->plainTextEditLength->installEventFilter(this);

    timerRadius->setSingleShot(true);
    connect(timerRadius, &QTimer::timeout, this, &DialogArcWithLength::Radius);

    timerF1->setSingleShot(true);
    connect(timerF1, &QTimer::timeout, this, &DialogArcWithLength::EvalF);

    timerLength->setSingleShot(true);
    connect(timerLength, &QTimer::timeout, this, &DialogArcWithLength::Length);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxCenter);
    FillComboBoxLineColors(ui->comboBoxColor);
    FillComboBoxTypeLine(ui->comboBoxPenStyle, CurvePenStylesPics());

    ui->doubleSpinBoxApproximationScale->setMaximum(maxCurveApproximationScale);

    connect(ui->toolButtonExprRadius, &QPushButton::clicked, this, &DialogArcWithLength::FXRadius);
    connect(ui->toolButtonExprF1, &QPushButton::clicked, this, &DialogArcWithLength::FXF1);
    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogArcWithLength::FXLength);

    connect(ui->plainTextEditRadius, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerRadius->start(formulaTimerTimeout);
    });

    connect(ui->plainTextEditF1, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerF1->start(formulaTimerTimeout);
    });

    connect(ui->plainTextEditLength, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerLength->start(formulaTimerTimeout);
    });

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
    SCASSERT(path != nullptr)
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
    SCASSERT(path != nullptr)
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
    SCASSERT(path != nullptr)
    path->setLength(radius);

    MoveCursorToEnd(ui->plainTextEditLength);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogArcWithLength::GetPenStyle() const
{
    return GetComboBoxCurrentData(ui->comboBoxPenStyle, TypeLineLine);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::SetPenStyle(const QString &value)
{
    ChangeCurrentData(ui->comboBoxPenStyle, value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogArcWithLength::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
qreal DialogArcWithLength::GetApproximationScale() const
{
    return ui->doubleSpinBoxApproximationScale->value();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::SetApproximationScale(qreal value)
{
    ui->doubleSpinBoxApproximationScale->setValue(value);

    VisToolArcWithLength *path = qobject_cast<VisToolArcWithLength *>(vis);
    SCASSERT(path != nullptr)
    path->setApproximationScale(value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            if (SetObject(id, ui->comboBoxCenter, QString()))
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
    DeployFormula(this, ui->plainTextEditRadius, ui->pushButtonGrowLengthArcLength, formulaBaseHeightRadius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::DeployF1TextEdit()
{
    DeployFormula(this, ui->plainTextEditF1, ui->pushButtonGrowLengthF1, formulaBaseHeightF1);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::DeployLengthTextEdit()
{
    DeployFormula(this, ui->plainTextEditLength, ui->pushButtonGrowLengthArcLength, formulaBaseHeightLength);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::FXRadius()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit radius"));
    dialog->SetFormula(GetRadius());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
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
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetLength(dialog->GetFormula());
    }
    delete dialog;
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
    f1 = ui->plainTextEditF1->toPlainText();
    length = ui->plainTextEditLength->toPlainText();

    VisToolArcWithLength *path = qobject_cast<VisToolArcWithLength *>(vis);
    SCASSERT(path != nullptr)

    path->setObject1Id(GetCenter());
    path->setRadius(radius);
    path->setF1(f1);
    path->setLength(length);
    path->setApproximationScale(ui->doubleSpinBoxApproximationScale->value());
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
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditRadius->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditRadius;
    formulaData.labelResult = ui->labelResultRadius;
    formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);
    formulaData.checkLessThanZero = true;

    Eval(formulaData, flagRadius);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::Length()
{
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditLength->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditLength;
    formulaData.labelResult = ui->labelResultLength;
    formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);

    Eval(formulaData, flagLength);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArcWithLength::EvalF()
{
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditF1->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditF1;
    formulaData.labelResult = ui->labelResultF1;
    formulaData.postfix = degreeSymbol;
    formulaData.checkZero = false;

    Eval(formulaData, flagF1);
}
