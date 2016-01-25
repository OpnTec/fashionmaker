/************************************************************************
 **
 **  @file   dialogpointofcontact.cpp
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

#include "dialogpointofcontact.h"

#include "../../../vgeometry/vpointf.h"
#include "../../../vpatterndb/vcontainer.h"
#include "../../../vpatterndb/vtranslatevars.h"
#include "../../visualization/vistoolpointofcontact.h"
#include "../../../vwidgets/vmaingraphicsscene.h"
#include "../support/dialogeditwrongformula.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogPointOfContact create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogPointOfContact::DialogPointOfContact(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogPointOfContact), radius(QString()), formulaBaseHeight(0)
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

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxPoints(ui->comboBoxCenter);

    connect(ui->toolButtonExprRadius, &QPushButton::clicked, this, &DialogPointOfContact::FXRadius);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfContact::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogPointOfContact::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogPointOfContact::DeployFormulaTextEdit);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfContact::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfContact::PointNameChanged);
    connect(ui->comboBoxCenter, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfContact::PointNameChanged);

    vis = new VisToolPointOfContact(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfContact::~DialogPointOfContact()
{
    DeleteVisualization<VisToolPointOfContact>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
    set.insert(getCurrentObjectId(ui->comboBoxCenter));

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
    ChangeColor(ui->labelArcCenter, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::FXRadius()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit radius"));
    dialog->SetFormula(getRadius());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        setRadius(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::ShowVisualization()
{
    AddVisualization<VisToolPointOfContact>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogPointOfContact::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            VisToolPointOfContact *line = qobject_cast<VisToolPointOfContact *>(vis);
            SCASSERT(line != nullptr);

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select second point of line")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxFirstPoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondPoint, tr("Select point of center of arc")))
                        {
                            number++;
                            line->setLineP2Id(id);
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
                        if (SetObject(id, ui->comboBoxCenter, ""))
                        {
                            line->setRadiusId(id);
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
void DialogPointOfContact::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    radius = ui->plainTextEditFormula->toPlainText();
    radius.replace("\n", " ");

    VisToolPointOfContact *line = qobject_cast<VisToolPointOfContact *>(vis);
    SCASSERT(line != nullptr);

    line->setObject1Id(GetFirstPoint());
    line->setLineP2Id(GetSecondPoint());
    line->setRadiusId(getCenter());
    line->setRadius(radius);
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSecondPoint set id second point
 * @param value id
 */
void DialogPointOfContact::SetSecondPoint(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondPoint, value);

    VisToolPointOfContact *line = qobject_cast<VisToolPointOfContact *>(vis);
    SCASSERT(line != nullptr);
    line->setLineP2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFirstPoint set id first point
 * @param value id
 */
void DialogPointOfContact::SetFirstPoint(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstPoint, value);

    VisToolPointOfContact *line = qobject_cast<VisToolPointOfContact *>(vis);
    SCASSERT(line != nullptr);
    line->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetCenter set id of center point
 * @param value id
 */
void DialogPointOfContact::setCenter(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxCenter, value);

    VisToolPointOfContact *line = qobject_cast<VisToolPointOfContact *>(vis);
    SCASSERT(line != nullptr);
    line->setRadiusId(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setRadius set formula radius of arc
 * @param value formula
 */
void DialogPointOfContact::setRadius(const QString &value)
{
    radius = qApp->TrVars()->FormulaToUser(value);
    // increase height if needed.
    if (radius.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(radius);

    VisToolPointOfContact *line = qobject_cast<VisToolPointOfContact *>(vis);
    SCASSERT(line != nullptr);
    line->setRadius(radius);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name of point
 * @param value name
 */
void DialogPointOfContact::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getRadius return formula radius of arc
 * @return formula
 */
QString DialogPointOfContact::getRadius() const
{
    return qApp->TrVars()->FormulaFromUser(radius, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetCenter return id of center point
 * @return id
 */
quint32 DialogPointOfContact::getCenter() const
{
    return getCurrentObjectId(ui->comboBoxCenter);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFirstPoint return id first point
 * @return id
 */
quint32 DialogPointOfContact::GetFirstPoint() const
{
    return getCurrentObjectId(ui->comboBoxFirstPoint);;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetSecondPoint return id second point
 * @return id
 */
quint32 DialogPointOfContact::GetSecondPoint() const
{
    return getCurrentObjectId(ui->comboBoxSecondPoint);
}
