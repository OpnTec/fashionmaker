/************************************************************************
 **
 **  @file   dialogtriangle.cpp
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

#include "dialogtriangle.h"
#include "ui_dialogtriangle.h"
#include "../../../vgeometry/vpointf.h"
#include "../../../vpatterndb/vcontainer.h"
#include "../../visualization/line/vistooltriangle.h"
#include "../../../vwidgets/vmaingraphicsscene.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogTriangle create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogTriangle::DialogTriangle(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogTriangle)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditNamePoint->setClearButtonEnabled(true);
#endif

    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;

    InitOkCancelApply(ui);
    CheckState();

    FillComboBoxPoints(ui->comboBoxAxisP1);
    FillComboBoxPoints(ui->comboBoxAxisP2);
    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogTriangle::NamePointChanged);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTriangle::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTriangle::PointNameChanged);
    connect(ui->comboBoxAxisP1, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTriangle::PointNameChanged);
    connect(ui->comboBoxAxisP2, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogTriangle::PointNameChanged);

    vis = new VisToolTriangle(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogTriangle::~DialogTriangle()
{
    DeleteVisualization<VisToolTriangle>();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogTriangle::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            VisToolTriangle *line = qobject_cast<VisToolTriangle *>(vis);
            SCASSERT(line != nullptr);

            switch (number)
            {
                case (0):
                    if (SetObject(id, ui->comboBoxAxisP1, tr("Select second point of axis")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case (1):
                    if (getCurrentObjectId(ui->comboBoxAxisP1) != id)
                    {
                        if (SetObject(id, ui->comboBoxAxisP2, tr("Select first point")))
                        {
                            number++;
                            line->setObject2Id(id);
                            line->RefreshGeometry();
                        }
                    }
                    break;
                case (2):
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxAxisP1));
                    set.insert(getCurrentObjectId(ui->comboBoxAxisP2));
                    set.insert(id);

                    if (set.size() == 3)
                    {
                        if (SetObject(id, ui->comboBoxFirstPoint, tr("Select second point")))
                        {
                            number++;
                            line->setHypotenuseP1Id(id);
                            line->RefreshGeometry();
                        }
                    }
                }
                    break;
                case (3):
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxAxisP1));
                    set.insert(getCurrentObjectId(ui->comboBoxAxisP2));
                    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
                    set.insert(id);

                    if (set.size() == 4)
                    {
                        if (SetObject(id, ui->comboBoxSecondPoint, ""))
                        {
                            line->setHypotenuseP2Id(id);
                            line->RefreshGeometry();
                            prepare = true;
                            DialogAccepted();
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
void DialogTriangle::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    VisToolTriangle *line = qobject_cast<VisToolTriangle *>(vis);
    SCASSERT(line != nullptr);

    line->setObject1Id(GetAxisP1Id());
    line->setObject2Id(GetAxisP2Id());
    line->setHypotenuseP1Id(GetFirstPointId());
    line->setHypotenuseP2Id(GetSecondPointId());
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTriangle::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
    set.insert(getCurrentObjectId(ui->comboBoxAxisP1));
    set.insert(getCurrentObjectId(ui->comboBoxAxisP2));

    QColor color = okColor;
    if (set.size() < 3)//Need tree or more unique points for creation triangle
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
    ChangeColor(ui->labelAxisP1, color);
    ChangeColor(ui->labelAxisP2, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTriangle::ShowVisualization()
{
    AddVisualization<VisToolTriangle>();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name of point
 * @param value name
 */
void DialogTriangle::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSecondPointId set id of second point
 * @param value id
 */
void DialogTriangle::SetSecondPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondPoint, value);

    VisToolTriangle *line = qobject_cast<VisToolTriangle *>(vis);
    SCASSERT(line != nullptr);
    line->setHypotenuseP2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFirstPointId set id of first point
 * @param value id
 */
void DialogTriangle::SetFirstPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstPoint, value);

    VisToolTriangle *line = qobject_cast<VisToolTriangle *>(vis);
    SCASSERT(line != nullptr);
    line->setHypotenuseP1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetAxisP2Id set id second point of axis
 * @param value id
 */
void DialogTriangle::SetAxisP2Id(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxAxisP2, value);

    VisToolTriangle *line = qobject_cast<VisToolTriangle *>(vis);
    SCASSERT(line != nullptr);
    line->setObject2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetAxisP1Id set id first point of axis
 * @param value id
 */
void DialogTriangle::SetAxisP1Id(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxAxisP1, value);

    VisToolTriangle *line = qobject_cast<VisToolTriangle *>(vis);
    SCASSERT(line != nullptr);
    line->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAxisP1Id return id first point of axis
 * @return id
 */
quint32 DialogTriangle::GetAxisP1Id() const
{
    return getCurrentObjectId(ui->comboBoxAxisP1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAxisP2Id return id second point of axis
 * @return id
 */
quint32 DialogTriangle::GetAxisP2Id() const
{
    return getCurrentObjectId(ui->comboBoxAxisP2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFirstPointId return id of first point
 * @return id
 */
quint32 DialogTriangle::GetFirstPointId() const
{
    return getCurrentObjectId(ui->comboBoxFirstPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetSecondPointId return id of second point
 * @return id
 */
quint32 DialogTriangle::GetSecondPointId() const
{
    return getCurrentObjectId(ui->comboBoxSecondPoint);
}
