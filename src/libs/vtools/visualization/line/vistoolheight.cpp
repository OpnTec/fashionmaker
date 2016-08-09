/************************************************************************
 **
 **  @file   vistoolheight.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 8, 2014
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

#include "vistoolheight.h"

#include <QGraphicsLineItem>
#include <QLine>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolheight.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolHeight::VisToolHeight(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), lineP1Id(NULL_ID), lineP2Id(NULL_ID), point(nullptr), base_point(nullptr), lineP1(nullptr),
      lineP2(nullptr), line(nullptr), line_intersection(nullptr)
{
    base_point = InitPoint(supportColor, this);
    lineP1 = InitPoint(supportColor, this);
    lineP2 = InitPoint(supportColor, this);
    line = InitItem<QGraphicsLineItem>(supportColor, this);
    line_intersection = InitItem<QGraphicsLineItem>(supportColor, this); //-V656

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolHeight::~VisToolHeight()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolHeight::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(base_point, *first, supportColor);

        if (lineP1Id <= NULL_ID)
        {
            DrawLine(this, QLineF(*first, Visualization::scenePos), mainColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(lineP1Id);
            DrawPoint(lineP1, *second, supportColor);

            QLineF base_line;
            if (lineP2Id <= NULL_ID)
            {
                base_line = QLineF(*second, Visualization::scenePos);
                DrawLine(line, base_line, supportColor);
            }
            else
            {
                const QSharedPointer<VPointF> third = Visualization::data->GeometricObject<VPointF>(lineP2Id);
                DrawPoint(lineP2, *third, supportColor);

                base_line = QLineF(*second, *third);
            }

            DrawLine(line, base_line, supportColor);

            QPointF height = VToolHeight::FindPoint(base_line, *first);
            DrawPoint(point, height, mainColor);

            QLineF height_line(*first, height);
            DrawLine(this, height_line, mainColor, lineStyle);

            ShowIntersection(height_line, base_line);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolHeight::setLineP1Id(const quint32 &value)
{
    lineP1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolHeight::setLineP2Id(const quint32 &value)
{
    lineP2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolHeight::ShowIntersection(const QLineF &height_line, const QLineF &base_line)
{
    QPointF p;
    QLineF::IntersectType intersect = height_line.intersect(base_line, &p);
    if (intersect == QLineF::UnboundedIntersection)
    {
        line_intersection->setVisible(true);
        DrawLine(line_intersection, QLineF(base_line.p1(), height_line.p2()), supportColor, Qt::DashLine);
    }
    else if (intersect == QLineF::BoundedIntersection)
    {
        line_intersection->setVisible(false);
    }
}
