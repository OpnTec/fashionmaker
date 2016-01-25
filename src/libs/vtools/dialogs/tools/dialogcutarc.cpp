/************************************************************************
 **
 **  @file   dialogcutarc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
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

#include "dialogcutarc.h"
#include "ui_dialogcutarc.h"

#include "../../../vgeometry/varc.h"
#include "../../../vpatterndb/vcontainer.h"
#include "../../../vpatterndb/vtranslatevars.h"
#include "../../visualization/vistoolcutarc.h"
#include "../../../vwidgets/vmaingraphicsscene.h"
#include "../support/dialogeditwrongformula.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogCutArc create dialog.
 * @param data container with data
 * @param parent parent widget
 */
DialogCutArc::DialogCutArc(const VContainer *data, const quint32 &toolId, QWidget *parent)
    : DialogTool(data, toolId, parent), ui(new Ui::DialogCutArc), formula(QString()), formulaBaseHeight(0),
      ch1(NULL_ID), ch2(NULL_ID)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditNamePoint->setClearButtonEnabled(true);
#endif

    InitFormulaUI(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancelApply(ui);
    flagFormula = false;
    CheckState();

    FillComboBoxArcs(ui->comboBoxArc, FillComboBox::NoChildren, ch1, ch2);
    FillComboBoxLineColors(ui->comboBoxColor);

    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogCutArc::FXLength);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutArc::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogCutArc::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogCutArc::DeployFormulaTextEdit);

    vis = new VisToolCutArc(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::FXLength()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit length"));
    dialog->SetFormula(GetFormula());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFormula(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::ShowVisualization()
{
    AddVisualization<VisToolCutArc>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogCutArc::~DialogCutArc()
{
    DeleteVisualization<VisToolCutArc>();
    delete ui;
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
            if (SetObject(id, ui->comboBoxArc, ""))
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
    formula.replace("\n", " ");

    VisToolCutArc *path = qobject_cast<VisToolCutArc *>(vis);
    SCASSERT(path != nullptr);

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
void DialogCutArc::setArcId(const quint32 &value)
{
    setCurrentArcId(ui->comboBoxArc, value, FillComboBox::NoChildren, ch1, ch2);

    VisToolCutArc *path = qobject_cast<VisToolCutArc *>(vis);
    SCASSERT(path != nullptr);
    path->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCutArc::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutArc::SetChildrenId(const quint32 &ch1, const quint32 &ch2)
{
    this->ch1 = ch1;
    this->ch2 = ch2;
    FillComboBoxArcs(ui->comboBoxArc, FillComboBox::NoChildren, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFormula set string with formula length
 * @param value string with formula
 */
void DialogCutArc::SetFormula(const QString &value)
{
    formula = qApp->TrVars()->FormulaToUser(value);
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);

    VisToolCutArc *path = qobject_cast<VisToolCutArc *>(vis);
    SCASSERT(path != nullptr);
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
    return qApp->TrVars()->FormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
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
