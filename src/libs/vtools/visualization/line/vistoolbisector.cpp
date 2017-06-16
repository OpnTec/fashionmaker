/************************************************************************
 **
 **  @file   vistoolbisector.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 7, 2014
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

#include "vistoolbisector.h"

#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolbisector.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolBisector::VisToolBisector(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent), object2Id(NULL_ID), object3Id(NULL_ID), point(nullptr), line1P1(nullptr), line1P2(nullptr),
      line1(nullptr), line2P2(nullptr), line2(nullptr), length(0)
{
    line1P1 = InitPoint(supportColor, this);
    line1P2 = InitPoint(supportColor, this); //-V656
    line1 = InitItem<QGraphicsLineItem>(supportColor, this);

    line2P2 = InitPoint(supportColor, this);
    line2 = InitItem<QGraphicsLineItem>(supportColor, this);

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolBisector::setObject2Id(const quint32 &value)
{
    object2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolBisector::setObject3Id(const quint32 &value)
{
    object3Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolBisector::setLength(const QString &expression)
{
    length = FindLength(expression, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolBisector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    ScalePoint(point, scale);
    ScalePoint(line1P1, scale);
    ScalePoint(line1P2, scale);
    ScalePenWidth(line1, scale);
    ScalePoint(line2P2, scale);
    ScalePenWidth(line2, scale);

    VisLine::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolBisector::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(line1P1, static_cast<QPointF>(*first), supportColor);

        if (object2Id <= NULL_ID)
        {
            DrawLine(line1, QLineF(static_cast<QPointF>(*first), Visualization::scenePos), supportColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(object2Id);
            DrawPoint(line1P2, static_cast<QPointF>(*second), supportColor);

            DrawLine(line1, QLineF(static_cast<QPointF>(*first), static_cast<QPointF>(*second)), supportColor);

            if (object3Id <= NULL_ID)
            {
                DrawLine(line2, QLineF(static_cast<QPointF>(*second), Visualization::scenePos), supportColor);
            }
            else
            {
                const QSharedPointer<VPointF> third = Visualization::data->GeometricObject<VPointF>(object3Id);
                DrawPoint(line2P2, static_cast<QPointF>(*third), supportColor);

                DrawLine(line2, QLineF(static_cast<QPointF>(*second), static_cast<QPointF>(*third)), supportColor);

                if (not qFuzzyIsNull(length))
                {
                    qreal angle = VToolBisector::BisectorAngle(static_cast<QPointF>(*first),
                                                               static_cast<QPointF>(*second),
                                                               static_cast<QPointF>(*third));
                    QLineF mainLine = VGObject::BuildLine(static_cast<QPointF>(*second), length, angle);
                    DrawLine(this, mainLine, mainColor, lineStyle);

                    DrawPoint(point, mainLine.p2(), mainColor);
                }
                else
                {
                    qreal angle = VToolBisector::BisectorAngle(static_cast<QPointF>(*first),
                                                               static_cast<QPointF>(*second),
                                                               static_cast<QPointF>(*third));
                    QPointF endRay = Ray(static_cast<QPointF>(*second), angle);
                    QLineF mainLine = VGObject::BuildLine(static_cast<QPointF>(*second),
                                                          QLineF(static_cast<QPointF>(*second), endRay).length(),
                                                          angle);
                    DrawLine(this, mainLine, mainColor, lineStyle);
                }
            }
        }
    }
}
