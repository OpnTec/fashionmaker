/************************************************************************
 **
 **  @file   dialogpointofintersection.cpp
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

#include "dialogpointofintersection.h"

#include <QColor>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPointer>

#include "../../visualization/visualization.h"
#include "../../visualization/line/vistoolpointofintersection.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/ifcdef.h"
#include "../vmisc/vabstractapplication.h"
#include "dialogtool.h"
#include "ui_dialogpointofintersection.h"

class QWidget;
class VContainer;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogPointOfIntersection create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogPointOfIntersection::DialogPointOfIntersection(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogPointOfIntersection)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditNamePoint->setClearButtonEnabled(true);
#endif

    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;

    InitOkCancelApply(ui);
    CheckState();

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfIntersection::NamePointChanged);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfIntersection::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfIntersection::PointNameChanged);

    vis = new VisToolPointOfIntersection(data);
    vis->VisualMode(NULL_ID);//Show vertical axis
}

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfIntersection::~DialogPointOfIntersection()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSecondPointId set id of second point
 * @param value id
 */
void DialogPointOfIntersection::SetSecondPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondPoint, value);

    VisToolPointOfIntersection *line = qobject_cast<VisToolPointOfIntersection *>(vis);
    SCASSERT(line != nullptr)
    line->setPoint2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogPointOfIntersection::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            VisToolPointOfIntersection *line = qobject_cast<VisToolPointOfIntersection *>(vis);
            SCASSERT(line != nullptr)

            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select point for Y value (horizontal)")))
                    {
                        number++;
                        line->setObject1Id(id);
                        line->RefreshGeometry();
                    }
                    break;
                case 1:
                    if (getCurrentObjectId(ui->comboBoxFirstPoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondPoint, ""))
                        {
                            line->setPoint2Id(id);
                            line->RefreshGeometry();
                            prepare = true;
                            DialogAccepted();
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
void DialogPointOfIntersection::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    VisToolPointOfIntersection *line = qobject_cast<VisToolPointOfIntersection *>(vis);
    SCASSERT(line != nullptr)

    line->setObject1Id(GetFirstPointId());
    line->setPoint2Id(GetSecondPointId());
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersection::PointNameChanged()
{
    QColor color = okColor;
    if (getCurrentObjectId(ui->comboBoxFirstPoint) == getCurrentObjectId(ui->comboBoxSecondPoint))
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
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfIntersection::ShowVisualization()
{
    AddVisualization<VisToolPointOfIntersection>();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFirstPointId set id of first point
 * @param value id
 */
void DialogPointOfIntersection::SetFirstPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstPoint, value);

    VisToolPointOfIntersection *line = qobject_cast<VisToolPointOfIntersection *>(vis);
    SCASSERT(line != nullptr)
    line->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name of point
 * @param value name
 */
void DialogPointOfIntersection::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFirstPointId return id of first point
 * @return id
 */
quint32 DialogPointOfIntersection::GetFirstPointId() const
{
    return getCurrentObjectId(ui->comboBoxFirstPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetSecondPointId return id of second point
 * @return id
 */
quint32 DialogPointOfIntersection::GetSecondPointId() const
{
    return getCurrentObjectId(ui->comboBoxSecondPoint);
}
