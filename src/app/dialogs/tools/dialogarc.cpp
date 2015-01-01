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

#include "dialogarc.h"
#include "ui_dialogarc.h"

#include <QPushButton>
#include <QTimer>

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../libs/ifc/xml/vdomdocument.h"
#include "../../visualization/vistoolarc.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogArc create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogArc::DialogArc(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogArc), flagRadius(false), flagF1(false), flagF2(false),
      timerRadius(nullptr), timerF1(nullptr), timerF2(nullptr), center(NULL_ID), radius(QString()),
      f1(QString()), f2(QString()), formulaBaseHeight(0), formulaBaseHeightF1(0), formulaBaseHeightF2(0), path(nullptr),
      angleF1(INT_MIN), angleF2(INT_MIN)
{
    ui->setupUi(this);

    InitVariables(ui);

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

    CheckState();

    connect(ui->toolButtonPutHereRadius, &QPushButton::clicked, this, &DialogArc::PutRadius);
    connect(ui->toolButtonPutHereF1, &QPushButton::clicked, this, &DialogArc::PutF1);
    connect(ui->toolButtonPutHereF2, &QPushButton::clicked, this, &DialogArc::PutF2);

    connect(ui->toolButtonEqualRadius, &QPushButton::clicked, this, &DialogArc::EvalRadius);
    connect(ui->toolButtonEqualF1, &QPushButton::clicked, this, &DialogArc::EvalF);
    connect(ui->toolButtonEqualF2, &QPushButton::clicked, this, &DialogArc::EvalF);

    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogArc::RadiusChanged);
    connect(ui->plainTextEditF1, &QPlainTextEdit::textChanged, this, &DialogArc::F1Changed);
    connect(ui->plainTextEditF2, &QPlainTextEdit::textChanged, this, &DialogArc::F2Changed);

    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogArc::DeployFormulaTextEdit);
    connect(ui->pushButtonGrowLengthF1, &QPushButton::clicked, this, &DialogArc::DeployF1TextEdit);
    connect(ui->pushButtonGrowLengthF2, &QPushButton::clicked, this, &DialogArc::DeployF2TextEdit);

    path = new VisToolArc(data);
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
    if (qApp->getCurrentScene()->items().contains(path))
    { // In some cases scene delete object yourself. If not make check program will crash.
        delete path;
    }
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetCenter set id of center point
 * @param value id
 */
void DialogArc::SetCenter(const quint32 &value)
{
    center = value;
    ChangeCurrentData(ui->comboBoxBasePoint, center);
    path->setPoint1Id(center);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF2 set formula second angle of arc
 * @param value formula
 */
void DialogArc::SetF2(const QString &value)
{
    f2 = qApp->FormulaToUser(value);
    // increase height if needed.
    if (f2.length() > 80)
    {
        this->DeployF2TextEdit();
    }
    ui->plainTextEditF2->setPlainText(f2);
    path->setF2(f2);
    MoveCursorToEnd(ui->plainTextEditF2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetF1 set formula first angle of arc
 * @param value formula
 */
void DialogArc::SetF1(const QString &value)
{
    f1 = qApp->FormulaToUser(value);
    // increase height if needed.
    if (f1.length() > 80)
    {
        this->DeployF1TextEdit();
    }
    ui->plainTextEditF1->setPlainText(f1);
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
    radius = value;
    // increase height if needed.
    if (radius.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(radius);
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
                path->VisualMode(id);
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
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, path, &Visualization::SetFactor);
        scene->addItem(path);
        path->RefreshGeometry();
    }
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
    center = getCurrentObjectId(ui->comboBoxBasePoint);

    path->setPoint1Id(center);
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
 * @brief ValChenged show description angles of lines
 * @param row number of row
 */
void DialogArc::ValChenged(int row)
{
    if (ui->listWidget->count() == 0)
    {
        return;
    }
    QListWidgetItem *item = ui->listWidget->item( row );
    if (ui->radioButtonAngleLine->isChecked())
    {
        qreal angle = *data->GetVariable<VLineAngle>(item->text())->GetValue();
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(angle).arg(tr("Value of angle of line."));
        ui->labelDescription->setText(desc);
        return;
    }
    DialogTool::ValChenged(row);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutRadius put variable into formula of radius
 */
void DialogArc::PutRadius()
{
    PutValHere(ui->plainTextEditFormula, ui->listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutF1 put variable into formula of first angle
 */
void DialogArc::PutF1()
{
    PutValHere(ui->plainTextEditF1, ui->listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PutF2 put variable into formula of second angle
 */
void DialogArc::PutF2()
{
    PutValHere(ui->plainTextEditF2, ui->listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LineAngles show variable angles of lines
 */
// cppcheck-suppress unusedFunction
void DialogArc::LineAngles()
{
    ShowLineAngles();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RadiusChanged after change formula of radius calculate value and show result
 */
void DialogArc::RadiusChanged()
{
    labelEditFormula = ui->labelEditRadius;
    labelResultCalculation = ui->labelResultRadius;
    ValFormulaChanged(flagRadius, ui->plainTextEditFormula, timerRadius);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief F1Changed after change formula of first angle calculate value and show result
 */
void DialogArc::F1Changed()
{
    labelEditFormula = ui->labelEditF1;
    labelResultCalculation = ui->labelResultF1;
    ValFormulaChanged(flagF1, ui->plainTextEditF1, timerF1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief F2Changed after change formula of second angle calculate value and show result
 */
void DialogArc::F2Changed()
{
    labelEditFormula = ui->labelEditF2;
    labelResultCalculation = ui->labelResultF2;
    ValFormulaChanged(flagF2, ui->plainTextEditF2, timerF2);
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
/**
 * @brief ShowLineAngles show varibles angles of lines
 */
void DialogArc::ShowLineAngles()
{
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    ui->listWidget->blockSignals(false);
    const QMap<QString, QSharedPointer<VLineAngle> > lineAnglesTable = data->DataAngleLines();
    QMap<QString, QSharedPointer<VLineAngle> >::const_iterator i;
    for (i = lineAnglesTable.constBegin(); i != lineAnglesTable.constEnd(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(i.key());

        item->setFont(QFont("Times", 12, QFont::Bold));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow (0);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogArc::CheckAngles()
{
    if (angleF1 == INT_MIN || angleF2 == INT_MIN)
    {
        return;
    }

    if (qFuzzyCompare(angleF1 + 1, angleF2 + 1))
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
