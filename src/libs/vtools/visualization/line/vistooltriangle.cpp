/************************************************************************
 **
 **  @file   vistooltriangle.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 8, 2014
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

#include "vistooltriangle.h"

#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPen>
#include <QPointF>
#include <QSharedPointer>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/vtooltriangle.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vmath.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolTriangle::VisToolTriangle(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent), object2Id(NULL_ID), hypotenuseP1Id(NULL_ID), hypotenuseP2Id(NULL_ID), point(nullptr),
      axisP1(nullptr), axisP2(nullptr), axis(nullptr), hypotenuseP1(nullptr), hypotenuseP2(nullptr), foot1(nullptr),
      foot2(nullptr)
{
    axisP1 = InitPoint(supportColor, this);
    axisP2 = InitPoint(supportColor, this);
    axis = InitItem<VCurvePathItem>(supportColor, this);
    hypotenuseP1 = InitPoint(supportColor, this);
    hypotenuseP2 = InitPoint(supportColor, this);
    foot1 = InitItem<VScaledLine>(supportColor, this);
    foot2 = InitItem<VScaledLine>(supportColor, this); //-V656

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTriangle::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(axisP1, static_cast<QPointF>(*first), supportColor);

        if (object2Id <= NULL_ID)
        {
            DrawAimedAxis(axis, QLineF(static_cast<QPointF>(*first), Visualization::scenePos), supportColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(object2Id);
            DrawPoint(axisP2, static_cast<QPointF>(*second), supportColor);

            DrawAimedAxis(axis, QLineF(static_cast<QPointF>(*first), static_cast<QPointF>(*second)), supportColor);

            if (hypotenuseP1Id > NULL_ID)
            {
                const QSharedPointer<VPointF> third = Visualization::data->GeometricObject<VPointF>(hypotenuseP1Id);
                DrawPoint(hypotenuseP1, static_cast<QPointF>(*third), supportColor);

                if (hypotenuseP2Id <= NULL_ID)
                {
                    DrawLine(this, QLineF(static_cast<QPointF>(*third), Visualization::scenePos), supportColor,
                             Qt::DashLine);

                    QPointF trPoint;
                    VToolTriangle::FindPoint(static_cast<QPointF>(*first), static_cast<QPointF>(*second),
                                             static_cast<QPointF>(*third), Visualization::scenePos, &trPoint);
                    DrawPoint(point, trPoint, mainColor);

                    DrawLine(foot1, QLineF(static_cast<QPointF>(*third), trPoint), supportColor, Qt::DashLine);
                    DrawLine(foot2, QLineF(Visualization::scenePos, trPoint), supportColor, Qt::DashLine);
                }
                else
                {
                    const QSharedPointer<VPointF> forth = Visualization::data->GeometricObject<VPointF>(hypotenuseP2Id);
                    DrawPoint(hypotenuseP2, static_cast<QPointF>(*forth), supportColor);

                    DrawLine(this, QLineF(static_cast<QPointF>(*third), static_cast<QPointF>(*forth)), supportColor,
                                          Qt::DashLine);

                    QPointF trPoint;
                    VToolTriangle::FindPoint(static_cast<QPointF>(*first), static_cast<QPointF>(*second),
                                             static_cast<QPointF>(*third), static_cast<QPointF>(*forth), &trPoint);
                    DrawPoint(point, trPoint, mainColor);

                    DrawLine(foot1, QLineF(static_cast<QPointF>(*third), trPoint), supportColor, Qt::DashLine);
                    DrawLine(foot2, QLineF(static_cast<QPointF>(*forth), trPoint), supportColor, Qt::DashLine);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTriangle::setObject2Id(const quint32 &value)
{
    object2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTriangle::setHypotenuseP1Id(const quint32 &value)
{
    hypotenuseP1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTriangle::setHypotenuseP2Id(const quint32 &value)
{
    hypotenuseP2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTriangle::DrawAimedAxis(VCurvePathItem *item, const QLineF &line, const QColor &color,
                                    Qt::PenStyle style)
{
    SCASSERT (item != nullptr)

    QPen visPen = item->pen();
    visPen.setColor(color);
    visPen.setStyle(style);

    item->setPen(visPen);

    QPainterPath path;
    path.moveTo(line.p1());
    path.lineTo(line.p2());

    qreal arrow_step = 60;
    qreal arrow_size = 10;

    if (line.length() < arrow_step)
    {
        DrawArrow(line, path, arrow_size);
    }

    QLineF axis;
    axis.setP1(line.p1());
    axis.setAngle(line.angle());
    axis.setLength(arrow_step);

    int steps = qFloor(line.length()/arrow_step);
    for (int i=0; i<steps; ++i)
    {
        DrawArrow(axis, path, arrow_size);
        axis.setLength(axis.length()+arrow_step);
    }
    item->setPath(path);
    item->setVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTriangle::DrawArrow(const QLineF &axis, QPainterPath &path, const qreal &arrow_size)
{
    QLineF arrowPart1;
    arrowPart1.setP1(axis.p2());
    arrowPart1.setLength(arrow_size);
    arrowPart1.setAngle(axis.angle()+180+35);

    path.moveTo(arrowPart1.p1());
    path.lineTo(arrowPart1.p2());

    QLineF arrowPart2;
    arrowPart2.setP1(axis.p2());
    arrowPart2.setLength(arrow_size);
    arrowPart2.setAngle(axis.angle()+180-35);

    path.moveTo(arrowPart2.p1());
    path.lineTo(arrowPart2.p2());
}
