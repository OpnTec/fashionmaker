/************************************************************************
 **
 **  @file   dialogcutsplinrpath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#include "dialogcutsplinepath.h"
#include "ui_dialogcutsplinepath.h"

#include "../../../vgeometry/vsplinepath.h"
#include "../../../vpatterndb/vcontainer.h"
#include "../../../vpatterndb/vtranslatevars.h"
#include "../../visualization/vistoolcutsplinepath.h"
#include "../support/dialogeditwrongformula.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogCutSplinePath create dialog.
 * @param data container with data
 * @param parent parent widget
 */
DialogCutSplinePath::DialogCutSplinePath(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogCutSplinePath), formula(QString()), formulaBaseHeight(0),
      ch1(NULL_ID), ch2(NULL_ID)
{
    ui->setupUi(this);
    InitFormulaUI(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancelApply(ui);
    flagFormula = false;
    CheckState();

    FillComboBoxSplinesPath(ui->comboBoxSplinePath, FillComboBox::NoChildren, ch1, ch2);
    FillComboBoxLineColors(ui->comboBoxColor);

    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogCutSplinePath::FXLength);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutSplinePath::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogCutSplinePath::FormulaChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogCutSplinePath::DeployFormulaTextEdit);

    vis = new VisToolCutSplinePath(data);

    FixateSize();
}

//---------------------------------------------------------------------------------------------------------------------
DialogCutSplinePath::~DialogCutSplinePath()
{
    DeleteVisualization<VisToolCutSplinePath>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name of point
 * @param value name
 */
void DialogCutSplinePath::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFormula set string of formula
 * @param value formula
 */
void DialogCutSplinePath::SetFormula(const QString &value)
{
    formula = qApp->TrVars()->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);

    VisToolCutSplinePath *path = qobject_cast<VisToolCutSplinePath *>(vis);
    SCASSERT(path != nullptr);
    path->setLength(formula);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSplineId set id spline
 * @param value id
 */
void DialogCutSplinePath::setSplinePathId(const quint32 &value)
{
    setCurrentSplinePathId(ui->comboBoxSplinePath, value, FillComboBox::NoChildren, ch1, ch2);

    VisToolCutSplinePath *path = qobject_cast<VisToolCutSplinePath *>(vis);
    SCASSERT(path != nullptr);
    path->setPoint1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogCutSplinePath::GetColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSplinePath::SetColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSplinePath::SetChildrenId(const quint32 &ch1, const quint32 &ch2)
{
    this->ch1 = ch1;
    this->ch2 = ch2;
    FillComboBoxSplinesPath(ui->comboBoxSplinePath, FillComboBox::NoChildren, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogCutSplinePath::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::SplinePath)
        {
            if (SetObject(id, ui->comboBoxSplinePath, ""))
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
void DialogCutSplinePath::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");

    VisToolCutSplinePath *path = qobject_cast<VisToolCutSplinePath *>(vis);
    SCASSERT(path != nullptr);

    path->setPoint1Id(getSplinePathId());
    path->setLength(formula);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSplinePath::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSplinePath::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSplinePath::FXLength()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit length"));
    dialog->SetFormula(GetFormula());
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFormula(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSplinePath::ShowVisualization()
{
    AddVisualization<VisToolCutSplinePath>();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormula return string of formula
 * @return formula
 */
QString DialogCutSplinePath::GetFormula() const
{
    return qApp->TrVars()->FormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSplineId return id base point of line
 * @return id
 */
quint32 DialogCutSplinePath::getSplinePathId() const
{
    return getCurrentObjectId(ui->comboBoxSplinePath);
}
