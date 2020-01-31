/************************************************************************
 **
 **  @file   dialogshoulderpoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include "dialogshoulderpoint.h"

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QSet>
#include <QTimer>
#include <QToolButton>

#include "../vpatterndb/vtranslatevars.h"
#include "../../tools/vabstracttool.h"
#include "../../visualization/visualization.h"
#include "../../visualization/line/vistoolshoulderpoint.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../support/dialogeditwrongformula.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "ui_dialogshoulderpoint.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogShoulderPoint create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogShoulderPoint::DialogShoulderPoint(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogShoulderPoint),
      formula(),
      formulaBaseHeight(0),
      timerFormula(new QTimer(this)),
      pointName(),
      flagFormula(false),
      flagName(true),
      flagError(true)
{
    ui->setupUi(this);

    timerFormula->setSingleShot(true);
    connect(timerFormula, &QTimer::timeout, this, &DialogShoulderPoint::EvalFormula);

    ui->lineEditNamePoint->setClearButtonEnabled(true);

    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancelApply(ui);

    FillComboBoxTypeLine(ui->comboBoxLineType, LineStylesPics());
    FillComboBoxPoints(ui->comboBoxP1Line);
    FillComboBoxPoints(ui->comboBoxP2Line);
    FillComboBoxPoints(ui->comboBoxP3);
    FillComboBoxLineColors(ui->comboBoxLineColor);

    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogShoulderPoint::FXLength);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, [this]()
    {
        CheckPointLabel(this, ui->lineEditNamePoint, ui->labelEditNamePoint, pointName, this->data, flagName);
        CheckState();
    });
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerFormula->start(formulaTimerTimeout);
    });
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogShoulderPoint::DeployFormulaTextEdit);
    connect(ui->comboBoxP1Line, &QComboBox::currentTextChanged,
            this, &DialogShoulderPoint::PointNameChanged);
    connect(ui->comboBoxP2Line, &QComboBox::currentTextChanged,
            this, &DialogShoulderPoint::PointNameChanged);
    connect(ui->comboBoxP3, &QComboBox::currentTextChanged,
            this, &DialogShoulderPoint::PointNameChanged);

    vis = new VisToolShoulderPoint(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxP1Line));
    set.insert(getCurrentObjectId(ui->comboBoxP2Line));
    set.insert(getCurrentObjectId(ui->comboBoxP3));

    QColor color;
    if (set.size() != 3)
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = OkColor(this);
    }
    ChangeColor(ui->labelFirstPoint, color);
    ChangeColor(ui->labelSecondPoint, color);
    ChangeColor(ui->labelThirdPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::FXLength()
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
void DialogShoulderPoint::EvalFormula()
{
    FormulaData formulaData;
    formulaData.formula = ui->plainTextEditFormula->toPlainText();
    formulaData.variables = data->DataVariables();
    formulaData.labelEditFormula = ui->labelEditFormula;
    formulaData.labelResult = ui->labelResultCalculation;
    formulaData.postfix = UnitsToStr(qApp->patternUnit(), true);

    Eval(formulaData, flagFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::ShowVisualization()
{
    AddVisualization<VisToolShoulderPoint>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::DeployFormulaTextEdit()
{
    DeployFormula(this, ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogShoulderPoint::~DialogShoulderPoint()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogShoulderPoint::GetPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogShoulderPoint::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)
    {
        if (type == SceneObject::Point)
        {
            VisToolShoulderPoint *line = qobject_cast<VisToolShoulderPoint *>(vis);
            SCASSERT(line != nullptr)

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxP3, tr("Select first point of line")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxP3) != id)
                    {
                        if (SetObject(id, ui->comboBoxP1Line, tr("Select second point of line")))
                        {
                            number++;
                            line->setLineP1Id(id);
                            line->RefreshGeometry();
                        }
                    }
                    break;
                case 2:
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxP3));
                    set.insert(getCurrentObjectId(ui->comboBoxP1Line));
                    set.insert(id);

                    if (set.size() == 3)
                    {
                        if (SetObject(id, ui->comboBoxP2Line, QString()))
                        {
                            line->setLineP2Id(id);
                            line->RefreshGeometry();
                            prepare = true;
                            this->setModal(true);
                            this->show();
                        }
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
void DialogShoulderPoint::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->plainTextEditFormula->toPlainText();

    VisToolShoulderPoint *line = qobject_cast<VisToolShoulderPoint *>(vis);
    SCASSERT(line != nullptr)

    line->setObject1Id(GetP3());
    line->setLineP1Id(GetP1Line());
    line->setLineP2Id(GetP2Line());
    line->setLength(formula);
    line->setLineStyle(LineStyleToPenStyle(GetTypeLine()));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPShoulder set id shoulder point
 * @param value id
 */
void DialogShoulderPoint::SetP3(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP3, value);

    VisToolShoulderPoint *line = qobject_cast<VisToolShoulderPoint *>(vis);
    SCASSERT(line != nullptr)
    line->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogShoulderPoint::GetLineColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogShoulderPoint::SetLineColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP2Line set id second point of line
 * @param value id
 */
void DialogShoulderPoint::SetP2Line(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP2Line, value);

    VisToolShoulderPoint *line = qobject_cast<VisToolShoulderPoint *>(vis);
    SCASSERT(line != nullptr)
    line->setLineP2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP1Line set id first point of line
 * @param value id
 */
void DialogShoulderPoint::SetP1Line(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxP1Line, value);

    VisToolShoulderPoint *line = qobject_cast<VisToolShoulderPoint *>(vis);
    SCASSERT(line != nullptr)
    line->setLineP1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFormula set string of formula
 * @param value formula
 */
void DialogShoulderPoint::SetFormula(const QString &value)
{
    formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);

    VisToolShoulderPoint *line = qobject_cast<VisToolShoulderPoint *>(vis);
    SCASSERT(line != nullptr)
    line->setLength(formula);
    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetTypeLine set type of line
 * @param value type
 */
void DialogShoulderPoint::SetTypeLine(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineType, value);
    vis->setLineStyle(LineStyleToPenStyle(value));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name of point
 * @param value name
 */
void DialogShoulderPoint::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetTypeLine return type of line
 * @return type
 */
QString DialogShoulderPoint::GetTypeLine() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineType, TypeLineLine);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormula return string of formula
 * @return formula
 */
QString DialogShoulderPoint::GetFormula() const
{
    return qApp->TrVars()->TryFormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP1Line return id first point of line
 * @return id
 */
quint32 DialogShoulderPoint::GetP1Line() const
{
    return getCurrentObjectId(ui->comboBoxP1Line);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2Line return id second point of line
 * @return id
 */
quint32 DialogShoulderPoint::GetP2Line() const
{
    return getCurrentObjectId(ui->comboBoxP2Line);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPShoulder return id shoulder point
 * @return id
 */
quint32 DialogShoulderPoint::GetP3() const
{
    return getCurrentObjectId(ui->comboBoxP3);
}
