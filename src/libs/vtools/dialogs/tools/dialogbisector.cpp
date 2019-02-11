/************************************************************************
 **
 **  @file   dialogbisector.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "dialogbisector.h"

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

#include "../../visualization/line/vistoolbisector.h"
#include "../../visualization/visualization.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../support/dialogeditwrongformula.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vpatterndb/vcontainer.h"
#include "ui_dialogbisector.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogBisector create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogBisector::DialogBisector(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogBisector),
      formula(),
      pointName(),
      formulaBaseHeight(0),
      timerFormula(new QTimer(this)),
      flagFormula(false),
      flagError(true),
      flagName(true)
{
    ui->setupUi(this);

    timerFormula->setSingleShot(true);
    connect(timerFormula, &QTimer::timeout, this, &DialogBisector::EvalFormula);

    ui->lineEditNamePoint->setClearButtonEnabled(true);

    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType, LineStylesPics());
    FillComboBoxPoints(ui->comboBoxThirdPoint);
    FillComboBoxLineColors(ui->comboBoxLineColor);

    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogBisector::FXLength);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, [this]()
    {
        CheckPointLabel(this, ui->lineEditNamePoint, ui->labelEditNamePoint, pointName, this->data, flagName);
        CheckState();
    });
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, [this]()
    {
        timerFormula->start(formulaTimerTimeout);
    });
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogBisector::DeployFormulaTextEdit);
    connect(ui->comboBoxFirstPoint, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
            this, &DialogBisector::PointNameChanged);
    connect(ui->comboBoxSecondPoint, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
            this, &DialogBisector::PointNameChanged);
    connect(ui->comboBoxThirdPoint, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
            this, &DialogBisector::PointNameChanged);

    vis = new VisToolBisector(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
    set.insert(getCurrentObjectId(ui->comboBoxThirdPoint));

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
void DialogBisector::FXLength()
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
void DialogBisector::EvalFormula()
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
void DialogBisector::ShowVisualization()
{
    AddVisualization<VisToolBisector>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::DeployFormulaTextEdit()
{
    DeployFormula(this, ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogBisector::~DialogBisector()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogBisector::GetPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogBisector::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            VisToolBisector *line = qobject_cast<VisToolBisector *>(vis);
            SCASSERT(line != nullptr)

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select second point of angle")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxFirstPoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondPoint, tr("Select third point of angle")))
                        {
                            number++;
                            line->setObject2Id(id);
                            line->RefreshGeometry();
                        }
                    }
                    break;
                case 2:
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
                    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
                    set.insert(id);

                    if (set.size() == 3)
                    {
                        if (SetObject(id, ui->comboBoxThirdPoint, QString()))
                        {
                            line->setObject3Id(id);
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
/**
 * @brief SetPointName set name of point
 * @param value name
 */
void DialogBisector::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetTypeLine set type of line
 * @param value type
 */
void DialogBisector::SetTypeLine(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineType, value);
    vis->setLineStyle(LineStyleToPenStyle(value));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFormula set string of formula
 * @param value formula
 */
void DialogBisector::SetFormula(const QString &value)
{
    formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);

    VisToolBisector *line = qobject_cast<VisToolBisector *>(vis);
    SCASSERT(line != nullptr)
    line->setLength(formula);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFirstPointId set id of first point
 * @param value id
 */
void DialogBisector::SetFirstPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstPoint, value);

    VisToolBisector *line = qobject_cast<VisToolBisector *>(vis);
    SCASSERT(line != nullptr)
    line->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSecondPointId set id of second point
 * @param value id
 */
void DialogBisector::SetSecondPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondPoint, value);

    VisToolBisector *line = qobject_cast<VisToolBisector *>(vis);
    SCASSERT(line != nullptr)
    line->setObject2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetThirdPointId set id of third point
 * @param value id
 */
void DialogBisector::SetThirdPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxThirdPoint, value);

    VisToolBisector *line = qobject_cast<VisToolBisector *>(vis);
    SCASSERT(line != nullptr)
    line->setObject3Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogBisector::GetLineColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::SetLineColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    formula = ui->plainTextEditFormula->toPlainText();

    VisToolBisector *line = qobject_cast<VisToolBisector *>(vis);
    SCASSERT(line != nullptr)

    line->setObject1Id(GetFirstPointId());
    line->setObject2Id(GetSecondPointId());
    line->setObject3Id(GetThirdPointId());
    line->setLength(formula);
    line->setLineStyle(LineStyleToPenStyle(GetTypeLine()));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetTypeLine return type of line
 * @return type
 */
QString DialogBisector::GetTypeLine() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineType, TypeLineLine);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormula return string of formula
 * @return formula
 */
QString DialogBisector::GetFormula() const
{
    return qApp->TrVars()->TryFormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFirstPointId return id of first point
 * @return id
 */
quint32 DialogBisector::GetFirstPointId() const
{
    return getCurrentObjectId(ui->comboBoxFirstPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetSecondPointId return id of second point
 * @return id
 */
quint32 DialogBisector::GetSecondPointId() const
{
    return getCurrentObjectId(ui->comboBoxSecondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetThirdPointId return id of third point
 * @return id
 */
quint32 DialogBisector::GetThirdPointId() const
{
    return getCurrentObjectId(ui->comboBoxThirdPoint);
}
