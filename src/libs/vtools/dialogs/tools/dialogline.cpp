/************************************************************************
 **
 **  @file   dialogline.cpp
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

#include "dialogline.h"

#include <QColor>
#include <QComboBox>
#include <QIcon>
#include <QLabel>
#include <QMap>
#include <QPointer>
#include <QVariant>

#include "../../visualization/visualization.h"
#include "../../visualization/line/vistoolline.h"
#include "../ifc/ifcdef.h"
#include "dialogs/tools/dialogtool.h"
#include "ui_dialogline.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogLine create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogLine::DialogLine(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogLine),
      flagError(true)
{
    ui->setupUi(this);
    InitOkCancelApply(ui);

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxLineColors(ui->comboBoxLineColor);

    QMap<QString, QIcon> stylesPics = LineStylesPics();
    stylesPics.remove(TypeLineNone);// Prevent hiding line
    FillComboBoxTypeLine(ui->comboBoxLineType, stylesPics);

    number = 0;

    connect(ui->comboBoxFirstPoint, &QComboBox::currentTextChanged,
            this, &DialogLine::PointNameChanged);
    connect(ui->comboBoxSecondPoint, &QComboBox::currentTextChanged,
            this, &DialogLine::PointNameChanged);

    vis = new VisToolLine(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogLine::~DialogLine()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSecondPoint set id second point
 * @param value id
 */
void DialogLine::SetSecondPoint(quint32 value)
{
    setCurrentPointId(ui->comboBoxSecondPoint, value);

    VisToolLine *line = qobject_cast<VisToolLine *>(vis);
    SCASSERT(line != nullptr)
    line->setPoint2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetTypeLine set type of line
 * @param value type
 */
void DialogLine::SetTypeLine(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineType, value);
    vis->setLineStyle(LineStyleToPenStyle(value));
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogLine::GetLineColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLine::SetLineColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFirstPoint set id first point
 * @param value id
 */
void DialogLine::SetFirstPoint(quint32 value)
{
    setCurrentPointId(ui->comboBoxFirstPoint, value);

    VisToolLine *line = qobject_cast<VisToolLine *>(vis);
    SCASSERT(line != nullptr)
    line->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLine::PointNameChanged()
{
    QColor color;
    if (getCurrentObjectId(ui->comboBoxFirstPoint) == getCurrentObjectId(ui->comboBoxSecondPoint))
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
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLine::ShowVisualization()
{
    AddVisualization<VisToolLine>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLine::SaveData()
{
    VisToolLine *line = qobject_cast<VisToolLine *>(vis);
    SCASSERT(line != nullptr)

    line->setObject1Id(GetFirstPoint());
    line->setPoint2Id(GetSecondPoint());
    line->setLineStyle(LineStyleToPenStyle(GetTypeLine()));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogLine::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select second point")))
                    {
                        number++;
                        vis->VisualMode(id);
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxFirstPoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondPoint, QString()))
                        {
                            if (flagError)
                            {
                                number = 0;
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
/**
 * @brief GetFirstPoint return id first point
 * @return id
 */
quint32 DialogLine::GetFirstPoint() const
{
    return qvariant_cast<quint32>(ui->comboBoxFirstPoint->currentData());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetSecondPoint return id second point
 * @return id
 */
quint32 DialogLine::GetSecondPoint() const
{
    return qvariant_cast<quint32>(ui->comboBoxSecondPoint->currentData());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetTypeLine return type of line
 * @return type
 */
QString DialogLine::GetTypeLine() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineType, TypeLineLine);
}
