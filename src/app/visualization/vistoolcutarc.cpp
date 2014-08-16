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

#include "vistoolcutarc.h"
#include "../geometry/varc.h"
#include "../container/vcontainer.h"

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
VisToolCutArc::~VisToolCutArc()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCutArc::RefreshGeometry()
{
    if (point1Id > 0)
    {
        const VArc *arc = Visualization::data->GeometricObject<const VArc *>(point1Id);
        DrawPath(this, arc->GetPath(), supportColor);

        if (qFuzzyCompare(1 + length, 1 + 0) == false)
        {
            VArc ar1;
            VArc ar2;
            QPointF p = arc->CutArc(length, ar1, ar2);
            DrawPoint(point, p, mainColor);

            DrawPath(arc1, ar1.GetPath(), Qt::darkGreen);
            DrawPath(arc2, ar2.GetPath(), Qt::darkRed);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCutArc::setLength(const QString &expression)
{
    length = FindLength(expression);
}
