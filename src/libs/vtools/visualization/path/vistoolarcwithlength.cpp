/************************************************************************
 **
 **  @file   vistoolarcwithlength.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2015
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

#include "vistoolarcwithlength.h"

#include <QPainterPath>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "vispath.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolArcWithLength::VisToolArcWithLength(const VContainer *data, QGraphicsItem *parent)
    :VisPath(data, parent), arcCenter(nullptr), radius(0), f1(0), length(0)
{
    arcCenter = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolArcWithLength::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(arcCenter, static_cast<QPointF>(*first), supportColor);

        if (not qFuzzyIsNull(radius) && f1 >= 0 && not qFuzzyIsNull(length))
        {
            VArc arc = VArc (length, *first, radius, f1);
            DrawPath(this, arc.GetPath(), arc.GetDirectionPath(), mainColor, lineStyle, Qt::RoundCap);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolArcWithLength::setRadius(const QString &expression)
{
    radius = FindLength(expression, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolArcWithLength::setF1(const QString &expression)
{
    f1 = FindVal(expression, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolArcWithLength::setLength(const QString &expression)
{
    length = FindLength(expression, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolArcWithLength::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    ScalePoint(arcCenter, scale);

    VisPath::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VisToolArcWithLength::boundingRect() const
{
    QRectF rect = VisPath::boundingRect();
    rect = rect.united(arcCenter->boundingRect());
    return rect;
}
