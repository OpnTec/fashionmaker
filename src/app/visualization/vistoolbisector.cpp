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
#include "../libs/vgeometry/vpointf.h"
#include "../libs/vpatterndb/vcontainer.h"
#include "../tools/drawTools/vtoolbisector.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolBisector::VisToolBisector(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent), point2Id(NULL_ID), point3Id(NULL_ID), point(nullptr), line1P1(nullptr), line1P2(nullptr),
      line1(nullptr), line2P2(nullptr), line2(nullptr), length(0)
{
    line1P1 = InitPoint(supportColor, this);
    line1P2 = InitPoint(supportColor, this);
    line1 = InitItem<QGraphicsLineItem>(supportColor, this);

    line2P2 = InitPoint(supportColor, this);
    line2 = InitItem<QGraphicsLineItem>(supportColor, this);

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolBisector::setPoint2Id(const quint32 &value)
{
    point2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolBisector::setPoint3Id(const quint32 &value)
{
    point3Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolBisector::setLength(const QString &expression)
{
    length = FindLength(expression);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolBisector::~VisToolBisector()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolBisector::RefreshGeometry()
{
    if (point1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(point1Id);
        DrawPoint(line1P1, first->toQPointF(), supportColor);

        if (point2Id <= NULL_ID)
        {
            DrawLine(line1, QLineF(first->toQPointF(), Visualization::scenePos), supportColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(point2Id);
            DrawPoint(line1P2, second->toQPointF(), supportColor);

            DrawLine(line1, QLineF(first->toQPointF(), second->toQPointF()), supportColor);

            if (point3Id <= NULL_ID)
            {
                DrawLine(line2, QLineF(second->toQPointF(), Visualization::scenePos), supportColor);
            }
            else
            {
                const QSharedPointer<VPointF> third = Visualization::data->GeometricObject<VPointF>(point3Id);
                DrawPoint(line2P2, third->toQPointF(), supportColor);

                DrawLine(line2, QLineF(second->toQPointF(), third->toQPointF()), supportColor);

                if (qFuzzyCompare(1 + length, 1 + 0) == false)
                {
                    qreal angle = VToolBisector::BisectorAngle(first->toQPointF(), second->toQPointF(),
                                                               third->toQPointF());
                    QLineF mainLine = VGObject::BuildLine(second->toQPointF(), length, angle);
                    DrawLine(this, mainLine, mainColor, lineStyle);

                    DrawPoint(point, mainLine.p2(), mainColor);
                }
                else
                {
                    qreal angle = VToolBisector::BisectorAngle(first->toQPointF(), second->toQPointF(),
                                                               third->toQPointF());
                    QPointF endRay = Ray(second->toQPointF(), angle);
                    QLineF mainLine = VGObject::BuildLine(second->toQPointF(),
                                                          QLineF(second->toQPointF(), endRay).length(), angle);
                    DrawLine(this, mainLine, mainColor, lineStyle);
                }
            }
        }
    }
}
