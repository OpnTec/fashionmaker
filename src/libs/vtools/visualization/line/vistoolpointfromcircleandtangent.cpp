/************************************************************************
 **
 **  @file   vistoolpointfromcircleandtangent.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vistoolpointfromcircleandtangent.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/vtoolpointfromcircleandtangent.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPointFromCircleAndTangent::VisToolPointFromCircleAndTangent(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), object2Id(NULL_ID), cRadius(0), crossPoint(CrossCirclesPoint::FirstPoint),
      point(nullptr), tangent(nullptr), cCenter(nullptr), cPath(nullptr), tangent2(nullptr)
{
    cPath = InitItem<QGraphicsEllipseItem>(Qt::darkGreen, this);
    point = InitPoint(mainColor, this);
    tangent = InitPoint(supportColor, this);
    cCenter = InitPoint(supportColor, this); //-V656
    tangent2 = InitItem<QGraphicsLineItem>(supportColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolPointFromCircleAndTangent::~VisToolPointFromCircleAndTangent()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointFromCircleAndTangent::RefreshGeometry()
{
    if (object1Id > NULL_ID)// tangent point
    {
        const QSharedPointer<VPointF> tan = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(tangent, *tan, supportColor);

        if (object2Id > NULL_ID)// circle center
        {
            const QSharedPointer<VPointF> center = Visualization::data->GeometricObject<VPointF>(object2Id);
            DrawPoint(cCenter, *center, supportColor);

            if (cRadius > 0)
            {
                cPath->setRect(PointRect(cRadius));
                DrawPoint(cPath, *center, Qt::darkGreen, Qt::DashLine);

                FindRays(*tan, *center, cRadius);

                const QPointF fPoint = VToolPointFromCircleAndTangent::FindPoint(*tan, *center, cRadius, crossPoint);
                DrawPoint(point, fPoint, mainColor);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointFromCircleAndTangent::setObject2Id(const quint32 &value)
{
    object2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointFromCircleAndTangent::setCRadius(const QString &value)
{
    cRadius = FindLength(value, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointFromCircleAndTangent::setCrossPoint(const CrossCirclesPoint &value)
{
    crossPoint = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointFromCircleAndTangent::FindRays(const QPointF &p, const QPointF &center, qreal radius)
{
    QPointF p1, p2;
    const int res = VGObject::ContactPoints (p, center, radius, p1, p2);

    switch(res)
    {
        case 2:
            DrawRay(this, p, p1, supportColor, Qt::DashLine);
            DrawRay(tangent2, p, p2, supportColor, Qt::DashLine);
            break;
        case 1:
            DrawRay(this, p, p1, supportColor, Qt::DashLine);
            tangent2->setVisible(false);
            break;
        default:
            this->setVisible(false);
            tangent2->setVisible(false);
            break;
    }
}
