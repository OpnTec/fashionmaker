/************************************************************************
 **
 **  @file   vistoolpointofcontact.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vistoolpointofcontact.h"
#include "../geometry/vpointf.h"
#include "../tools/drawTools/vtoolpointofcontact.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfContact::VisToolPointOfContact(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent), lineP2Id(NULL_ID), radiusId(NULL_ID), point(nullptr), lineP1(nullptr), lineP2(nullptr),
      arc_point(nullptr), circle(nullptr), radius(0)
{
    arc_point = InitPoint(supportColor, this);
    lineP1 = InitPoint(supportColor, this);
    lineP2 = InitPoint(supportColor, this);
    circle = InitItem<QGraphicsEllipseItem>(supportColor, this);

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfContact::~VisToolPointOfContact()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfContact::RefreshGeometry()
{
    if (point1Id > 0)
    {
        const VPointF *first = Visualization::data->GeometricObject<const VPointF *>(point1Id);
        DrawPoint(lineP1, first->toQPointF(), supportColor);

        if (lineP2Id <= 0)
        {
            DrawLine(this, QLineF(first->toQPointF(), Visualization::scenePos), supportColor);
        }
        else
        {
            const VPointF *second = Visualization::data->GeometricObject<const VPointF *>(lineP2Id);
            DrawPoint(lineP2, second->toQPointF(), supportColor);
            DrawLine(this, QLineF(first->toQPointF(), second->toQPointF()), supportColor);

            if (radiusId <= 0)
            {
                return;
            }
            else
            {
                const VPointF *third = Visualization::data->GeometricObject<const VPointF *>(radiusId);
                DrawPoint(arc_point, third->toQPointF(), supportColor);

                if (qFuzzyCompare(1 + radius, 1 + 0) == false)
                {
                    QPointF fPoint = VToolPointOfContact::FindPoint(radius, third->toQPointF(), first->toQPointF(),
                                                                    second->toQPointF());
                    DrawPoint(point, fPoint, mainColor);

                    circle->setRect(PointRect(radius));
                    DrawPoint(circle, third->toQPointF(), supportColor, Qt::DashLine);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfContact::setLineP2Id(const quint32 &value)
{
    lineP2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfContact::setRadiusId(const quint32 &value)
{
    radiusId = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfContact::setRadius(const QString &expression)
{
    radius = FindLength(expression);
}
