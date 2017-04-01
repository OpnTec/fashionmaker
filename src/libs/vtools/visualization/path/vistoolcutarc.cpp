/************************************************************************
 **
 **  @file   vistoolcutarc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 8, 2014
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

#include "vistoolcutarc.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "vispath.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolCutArc::VisToolCutArc(const VContainer *data, QGraphicsItem *parent)
    :VisPath(data, parent), point(nullptr), arc1(nullptr), arc2(nullptr), length(0)
{
    arc1 = InitItem<QGraphicsPathItem>(Qt::darkGreen, this);
    arc1->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
    arc2 = InitItem<QGraphicsPathItem>(Qt::darkRed, this);
    arc2->setFlag(QGraphicsItem::ItemStacksBehindParent, false);

    point = InitPoint(mainColor, this);
    point->setZValue(2);
    point->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCutArc::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VArc> arc = Visualization::data->GeometricObject<VArc>(object1Id);
        DrawPath(this, arc->GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);

        if (not qFuzzyIsNull(length))
        {
            VArc ar1;
            VArc ar2;
            QPointF p = arc->CutArc(length, ar1, ar2);
            DrawPoint(point, p, mainColor);

            DrawPath(arc1, ar1.GetPath(PathDirection::Show), Qt::darkGreen, Qt::SolidLine, Qt::RoundCap);
            DrawPath(arc2, ar2.GetPath(PathDirection::Show), Qt::darkRed, Qt::SolidLine, Qt::RoundCap);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCutArc::setLength(const QString &expression)
{
    length = FindLength(expression, Visualization::data->PlainVariables());
}
