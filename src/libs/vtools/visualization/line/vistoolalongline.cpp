/************************************************************************
 **
 **  @file   vistoolalongline.cpp
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

#include "vistoolalongline.h"

#include <QColor>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolAlongLine::VisToolAlongLine(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), object2Id(NULL_ID), point(nullptr), lineP1(nullptr), lineP2(nullptr), line(nullptr),
      length(0)
{
    this->mainColor = Qt::red;
    this->setZValue(2);// Show on top real tool

    lineP1 = InitPoint(supportColor, this);
    lineP2 = InitPoint(supportColor, this); //-V656
    line = InitItem<QGraphicsLineItem>(supportColor, this);
    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolAlongLine::~VisToolAlongLine()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolAlongLine::setObject2Id(const quint32 &value)
{
    object2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolAlongLine::setLength(const QString &expression)
{
    length = FindLength(expression, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolAlongLine::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(lineP1, static_cast<QPointF>(*first), supportColor);

        if (object2Id <= NULL_ID)
        {
            DrawLine(line, QLineF(static_cast<QPointF>(*first), Visualization::scenePos), supportColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(object2Id);
            DrawPoint(lineP2, static_cast<QPointF>(*second), supportColor);

            DrawLine(line, QLineF(static_cast<QPointF>(*first), static_cast<QPointF>(*second)), supportColor);

            if (not qFuzzyIsNull(length))
            {
                QLineF mainLine = VGObject::BuildLine(static_cast<QPointF>(*first), length, line->line().angle());
                DrawLine(this, mainLine, mainColor, lineStyle);

                DrawPoint(point, mainLine.p2(), mainColor);
            }
        }
    }
}
