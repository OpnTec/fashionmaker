/************************************************************************
 **
 **  @file   dialogcutarc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "dialogcutarc.h"

#include <QDialog>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QTimer>
#include <QToolButton>

#include "../vpatterndb/vtranslatevars.h"
#include "../vpatterndb/vcontainer.h"
#include "../../visualization/path/vistoolcutarc.h"
#include "../../visualization/visualization.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../support/dialogeditwrongformula.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "ui_dialogcutarc.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogCutArc create dialog.
 * @param data container with data
 * @param parent parent widget
 */
DialogCutArc::DialogCutArc(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogCutArc),
      formula(),
      pointName(),
      formulaBaseHeight(0),
      timerFormula(new QTimer(this)),
      flagFormula(false),
      flagName(true)
{
    ui->setupUi(this);

    timerFormula->setSingleShot(true);
    connect(timerFormula, &QTimer::timeout, this, &DialogCutArc::EvalFormula);

    ui->lineEditNamePoint->setClearButtonEnabled(true);

    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancelApply(ui);

    FillComboBoxArcs(ui->comboBoxArc);

    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogCutArc::FXLength);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, [this]()
    {
        CheckPointLabel(this, ui->lineEditNamePoint, ui->labelEditNamePoint, pointName, this->data, flagName);
        CheckState();
    });
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerFormula->start(formulaTimerTimeout);
    });
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogCutArc::DeployFormulaTextEdit);

    vis = new VisToolCutArc(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::FXLength()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit length"));
    dialog->SetFormula(GetFormula());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFormula(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::EvalFormula()
{
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditFormula->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditFormula;
    formulaData.labelResult = ui->labelResultCalculation;
    formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);
    formulaData.checkZero = false;

    Eval(formulaData, flagFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::ShowVisualization()
{
    AddVisualization<VisToolCutArc>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::DeployFormulaTextEdit()
{
    DeployFormula(this, ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogCutArc::~DialogCutArc()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCutArc::GetPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogCutArc::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Arc)
        {
            if (SetObject(id, ui->comboBoxArc, QString()))
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
void DialogCutArc::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->plainTextEditFormula->toPlainText();

    VisToolCutArc *path = qobject_cast<VisToolCutArc *>(vis);
    SCASSERT(path != nullptr)

    path->setObject1Id(getArcId());
    path->setLength(formula);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setArcId set id of arc
 * @param value id
 */
void DialogCutArc::setArcId(quint32 value)
{
    setCurrentArcId(ui->comboBoxArc, value);

    VisToolCutArc *path = qobject_cast<VisToolCutArc *>(vis);
    SCASSERT(path != nullptr)
    path->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFormula set string with formula length
 * @param value string with formula
 */
void DialogCutArc::SetFormula(const QString &value)
{
    formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);

    VisToolCutArc *path = qobject_cast<VisToolCutArc *>(vis);
    SCASSERT(path != nullptr)
    path->setLength(formula);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name point on arc
 * @param value name
 */
void DialogCutArc::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormula return string with formula length
 * @return formula
 */
QString DialogCutArc::GetFormula() const
{
    return qApp->TrVars()->TryFormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getArcId return id of arc
 * @return id
 */
quint32 DialogCutArc::getArcId() const
{
    return getCurrentObjectId(ui->comboBoxArc);
}
