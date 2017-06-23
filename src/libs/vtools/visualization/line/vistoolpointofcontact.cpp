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

#include "vistoolpointofcontact.h"

#include <QGraphicsEllipseItem>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/vtoolpointofcontact.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfContact::VisToolPointOfContact(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent), lineP2Id(NULL_ID), radiusId(NULL_ID), point(nullptr), lineP1(nullptr), lineP2(nullptr),
      arc_point(nullptr), circle(nullptr), radius(0)
{
    arc_point = InitPoint(supportColor, this);
    lineP1 = InitPoint(supportColor, this);
    lineP2 = InitPoint(supportColor, this);
    circle = InitItem<VScaledEllipse>(supportColor, this);

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfContact::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(lineP1, static_cast<QPointF>(*first), supportColor);

        if (lineP2Id <= NULL_ID)
        {
            DrawLine(this, QLineF(static_cast<QPointF>(*first), Visualization::scenePos), supportColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(lineP2Id);
            DrawPoint(lineP2, static_cast<QPointF>(*second), supportColor);
            DrawLine(this, QLineF(static_cast<QPointF>(*first), static_cast<QPointF>(*second)), supportColor);

            if (radiusId <= NULL_ID)
            {
                return;
            }
            else
            {
                const QSharedPointer<VPointF> third = Visualization::data->GeometricObject<VPointF>(radiusId);
                DrawPoint(arc_point, static_cast<QPointF>(*third), supportColor);

                if (not qFuzzyIsNull(radius))
                {
                    QPointF fPoint = VToolPointOfContact::FindPoint(radius, static_cast<QPointF>(*third),
                                                                    static_cast<QPointF>(*first),
                                                                    static_cast<QPointF>(*second));
                    DrawPoint(point, fPoint, mainColor);

                    circle->setRect(PointRect(radius));
                    DrawPoint(circle, static_cast<QPointF>(*third), supportColor, Qt::DashLine);
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
    radius = FindLength(expression, Visualization::data->PlainVariables());
}
