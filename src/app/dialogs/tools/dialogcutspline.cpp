/************************************************************************
 **
 **  @file   dialogcutspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "dialogcutspline.h"
#include "ui_dialogcutspline.h"

#include "../../geometry/vspline.h"
#include "../../container/vcontainer.h"
#include "../../xml/vpattern.h"
#include "../../visualization/vistoolcutspline.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogCutSpline create dialog.
 * @param data container with data
 * @param parent parent widget
 */
DialogCutSpline::DialogCutSpline(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogCutSpline), formula(QString()), formulaBaseHeight(0),
      path(nullptr)
{
    ui->setupUi(this);
    InitVariables(ui);
    InitFormulaUI(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancelApply(ui);
    flagFormula = false;
    CheckState();

    FillComboBoxSplines(ui->comboBoxSpline);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogCutSpline::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogCutSpline::PutVal);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogCutSpline::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogCutSpline::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogCutSpline::FormulaChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogCutSpline::DeployFormulaTextEdit);

    path = new VisToolCutSpline(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogCutSpline::~DialogCutSpline()
{
    if (qApp->getCurrentScene()->items().contains(path))
    { // In some cases scene delete object yourself. If not make check program will crash.
        delete path;
    }
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name of point
 * @param value name
 */
void DialogCutSpline::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFormula set string of formula
 * @param value formula
 */
void DialogCutSpline::SetFormula(const QString &value)
{
    formula = qApp->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);
    path->setLength(formula);
    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSplineId set id spline
 * @param value id
 */
void DialogCutSpline::setSplineId(const quint32 &value)
{
    setCurrentSplineId(ui->comboBoxSpline, value, ComboBoxCutSpline::CutSpline);
    path->setPoint1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogCutSpline::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Spline)
        {
            if (SetObject(id, ui->comboBoxSpline, ""))
            {
                path->VisualMode(id);
                prepare = true;
                this->setModal(true);
                this->show();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSpline::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->plainTextEditFormula->toPlainText();
    formula.replace("\n", " ");

    path->setPoint1Id(getSplineId());
    path->setLength(formula);
    path->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSpline::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSpline::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogCutSpline::ShowVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, path, &Visualization::SetFactor);
        scene->addItem(path);
        path->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormula return string of formula
 * @return formula
 */
QString DialogCutSpline::GetFormula() const
{
    return qApp->FormulaFromUser(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSplineId return id base point of line
 * @return id
 */
quint32 DialogCutSpline::getSplineId() const
{
    return getCurrentObjectId(ui->comboBoxSpline);
}
