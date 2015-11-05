/************************************************************************
 **
 **  @file   dialogbisector.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "dialogbisector.h"
#include "ui_dialogbisector.h"

#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../../visualization/vistoolbisector.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../../tools/vabstracttool.h"
#include "../support/dialogeditwrongformula.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogBisector create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogBisector::DialogBisector(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogBisector), formula(QString()), formulaBaseHeight(0)
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

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType, VAbstractTool::LineStylesPics());
    FillComboBoxPoints(ui->comboBoxThirdPoint);
    FillComboBoxLineColors(ui->comboBoxLineColor);

    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogBisector::FXLength);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogBisector::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogBisector::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogBisector::DeployFormulaTextEdit);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogBisector::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogBisector::PointNameChanged);
    connect(ui->comboBoxThirdPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogBisector::PointNameChanged);

    vis = new VisToolBisector(data);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
    set.insert(getCurrentObjectId(ui->comboBoxThirdPoint));

    QColor color = okColor;
    if (set.size() != 3)
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
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
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFormula(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::ShowVisualization()
{
    AddVisualization<VisToolBisector>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogBisector::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
DialogBisector::~DialogBisector()
{
    DeleteVisualization<VisToolBisector>();
    delete ui;
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
            SCASSERT(line != nullptr);

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
                            line->setPoint2Id(id);
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
                        if (SetObject(id, ui->comboBoxThirdPoint, ""))
                        {
                            line->setPoint3Id(id);
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
    vis->setLineStyle(VAbstractTool::LineStyleToPenStyle(value));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFormula set string of formula
 * @param value formula
 */
void DialogBisector::SetFormula(const QString &value)
{
    formula = qApp->TrVars()->FormulaToUser(value);
    // increase height if needed.
    if (formula.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formula);

    VisToolBisector *line = qobject_cast<VisToolBisector *>(vis);
    SCASSERT(line != nullptr);
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
    SCASSERT(line != nullptr);
    line->setPoint1Id(value);
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
    SCASSERT(line != nullptr);
    line->setPoint2Id(value);
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
    SCASSERT(line != nullptr);
    line->setPoint3Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogBisector::GetLineColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineColor);
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
    formula.replace("\n", " ");

    VisToolBisector *line = qobject_cast<VisToolBisector *>(vis);
    SCASSERT(line != nullptr);

    line->setPoint1Id(GetFirstPointId());
    line->setPoint2Id(GetSecondPointId());
    line->setPoint3Id(GetThirdPointId());
    line->setLength(formula);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(GetTypeLine()));
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
    return GetComboBoxCurrentData(ui->comboBoxLineType);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormula return string of formula
 * @return formula
 */
QString DialogBisector::GetFormula() const
{
    return qApp->TrVars()->FormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
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
