/************************************************************************
 **
 **  @file   vistoolpointofintersection.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 8, 2014
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

#include "vistoolpointofintersection.h"
#include "../geometry/vpointf.h"
#include "../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfIntersection::VisToolPointOfIntersection(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), point2Id(0), point(nullptr), axisP1(nullptr), axisP2(nullptr), axis2(nullptr)
{
    axisP1 = InitPoint(supportColor);
    axisP2 = InitPoint(supportColor);
    axis2 = InitLine(supportColor);

    point = InitPoint(mainColor);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfIntersection::~VisToolPointOfIntersection()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersection::RefreshGeometry()
{
    if (point1Id > 0)
    {
        const VPointF *first = data->GeometricObject<const VPointF *>(point1Id);
        DrawPoint(axisP1, first->toQPointF(), supportColor);

        QLineF axisL1 = Axis(first->toQPointF(), 90);
        DrawLine(this, axisL1, supportColor, Qt::DashLine);

        QLineF axisL2;
        if (point2Id <= 0)
        {
            DrawPoint(axisP2, scenePos, supportColor);
            axisL2 = Axis(scenePos, 180);
        }
        else
        {
            const VPointF *second = data->GeometricObject<const VPointF *>(point2Id);
            DrawPoint(axisP2, second->toQPointF(), supportColor);
            axisL2 = Axis(second->toQPointF(), 180);
        }
        DrawLine(axis2, axisL2, supportColor, Qt::DashLine);
        ShowIntersection(axisL1, axisL2);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersection::setPoint2Id(const quint32 &value)
{
    point2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VisToolPointOfIntersection::Axis(const QPointF &p, const qreal &angle) const
{
    QPointF endP1 = Ray(p, angle);
    QPointF endP2 = Ray(p, angle+180);
    return QLineF(endP1, endP2);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersection::ShowIntersection(const QLineF &axis1, const QLineF &axis2)
{
    QPointF p;
    QLineF::IntersectType intersect = axis1.intersect(axis2, &p);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        point->setVisible(true);
        DrawPoint(point, p, mainColor);
    }
    else
    {
        point->setVisible(true);
    }
}
