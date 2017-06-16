/************************************************************************
 **
 **  @file   vistoollineintersect.cpp
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

#include "vistoollineintersect.h"

#include <QGraphicsLineItem>
#include <QLine>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <new>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolLineIntersect::VisToolLineIntersect(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent), line1P2Id(NULL_ID), line2P1Id(NULL_ID), line2P2Id(NULL_ID), point(nullptr),
      line1P1(nullptr), line1P2(nullptr), line1(nullptr), line2P1(nullptr), line2P2(nullptr)
{
    line1P1 = InitPoint(supportColor, this);
    line1P2 = InitPoint(supportColor, this);
    line1 = InitItem<QGraphicsLineItem>(supportColor, this);

    line2P1 = InitPoint(supportColor, this);
    line2P2 = InitPoint(supportColor, this);

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersect::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(line1P1, static_cast<QPointF>(*first), supportColor);

        if (line1P2Id <= NULL_ID)
        {
            DrawLine(line1, QLineF(static_cast<QPointF>(*first), Visualization::scenePos), supportColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(line1P2Id);
            DrawPoint(line1P2, static_cast<QPointF>(*second), supportColor);

            DrawLine(line1, QLineF(static_cast<QPointF>(*first), static_cast<QPointF>(*second)), supportColor);

            if (line2P1Id <= NULL_ID)
            {
                return;
            }
            else
            {
                const QSharedPointer<VPointF> third = Visualization::data->GeometricObject<VPointF>(line2P1Id);
                DrawPoint(line2P1, static_cast<QPointF>(*third), supportColor);

                if (line2P2Id <= NULL_ID)
                {
                    DrawLine(this, QLineF(static_cast<QPointF>(*third), Visualization::scenePos), supportColor);

                    QLineF l1(static_cast<QPointF>(*first), static_cast<QPointF>(*second));
                    QLineF l2(static_cast<QPointF>(*third), Visualization::scenePos);
                    QPointF fPoint;
                    QLineF::IntersectType intersect = l1.intersect(l2, &fPoint);
                    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
                    {
                        DrawPoint(point, fPoint, mainColor);
                    }
                }
                else
                {
                    const QSharedPointer<VPointF> forth = Visualization::data->GeometricObject<VPointF>(line2P2Id);
                    DrawPoint(line2P2, static_cast<QPointF>(*forth), supportColor);

                    DrawLine(this, QLineF(static_cast<QPointF>(*third), static_cast<QPointF>(*forth)), supportColor);

                    QLineF l1(static_cast<QPointF>(*first), static_cast<QPointF>(*second));
                    QLineF l2(static_cast<QPointF>(*third), static_cast<QPointF>(*forth));
                    QPointF fPoint;
                    QLineF::IntersectType intersect = l1.intersect(l2, &fPoint);
                    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
                    {
                        DrawPoint(point, fPoint, mainColor);
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersect::setLine1P2Id(const quint32 &value)
{
    line1P2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersect::setLine2P1Id(const quint32 &value)
{
    line2P1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersect::setLine2P2Id(const quint32 &value)
{
    line2P2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    ScalePoint(point, scale);
    ScalePoint(line1P1, scale);
    ScalePoint(line1P2, scale);
    ScalePenWidth(line1, scale);
    ScalePoint(line2P1, scale);
    ScalePoint(line2P2, scale);

    VisLine::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VisToolLineIntersect::boundingRect() const
{
    QRectF rect = VisLine::boundingRect();
    rect = rect.united(point->boundingRect());
    rect = rect.united(line1P1->boundingRect());
    rect = rect.united(line1P2->boundingRect());
    rect = rect.united(line1->boundingRect());
    rect = rect.united(line2P1->boundingRect());
    rect = rect.united(line2P2->boundingRect());
    return rect;
}
