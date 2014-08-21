/************************************************************************
 **
 **  @file   vistoolshoulderpoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 7, 2014
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

#include "vistoolshoulderpoint.h"
#include "../tools/drawTools/vtoolshoulderpoint.h"
#include "../geometry/vpointf.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolShoulderPoint::VisToolShoulderPoint(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent), lineP1Id(NULL_ID), lineP2Id(NULL_ID), point(nullptr), line1P1(nullptr), line1P2(nullptr),
      line1(nullptr), line2P2(nullptr), line2(nullptr), line3(nullptr), length(0)
{
    line1P1 = InitPoint(supportColor, this);
    line1P2 = InitPoint(supportColor, this);
    line1 = InitItem<QGraphicsLineItem>(supportColor, this);

    line2P2 = InitPoint(supportColor, this);
    line2 = InitItem<QGraphicsLineItem>(supportColor, this);
    line3 = InitItem<QGraphicsLineItem>(supportColor, this);

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolShoulderPoint::~VisToolShoulderPoint()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolShoulderPoint::RefreshGeometry()
{
    if (point1Id > 0)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(point1Id);
        DrawPoint(line1P1, first->toQPointF(), supportColor);

        if (lineP1Id <= 0)
        {
            DrawLine(line1, QLineF(first->toQPointF(), Visualization::scenePos), supportColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(lineP1Id);
            DrawPoint(line1P2, second->toQPointF(), supportColor);

            DrawLine(line1, QLineF(first->toQPointF(), second->toQPointF()), supportColor);

            if (lineP2Id <= 0)
            {
                DrawLine(line2, QLineF(second->toQPointF(), Visualization::scenePos), supportColor);
            }
            else
            {
                const QSharedPointer<VPointF> third = Visualization::data->GeometricObject<VPointF>(lineP2Id);
                DrawPoint(line2P2, third->toQPointF(), supportColor);

                DrawLine(line2, QLineF(second->toQPointF(), third->toQPointF()), supportColor);

                if (qFuzzyCompare(1 + length, 1 + 0) == false)
                {
                    QPointF fPoint = VToolShoulderPoint::FindPoint(second->toQPointF(), third->toQPointF(),
                                                                   first->toQPointF(), length);
                    QLineF mainLine = QLineF(second->toQPointF(), fPoint);
                    DrawLine(this, mainLine, mainColor, lineStyle);

                    DrawPoint(point, mainLine.p2(), mainColor);
                    DrawLine(line3, QLineF(first->toQPointF(), mainLine.p2()), supportColor, Qt::DashLine);
                }
                else
                {
                    qreal angle = QLineF(second->toQPointF(), third->toQPointF()).angle();
                    QPointF endRay = Ray(second->toQPointF(), angle);
                    QLineF mainLine = Line(second->toQPointF(), QLineF(second->toQPointF(), endRay).length(), angle);
                    DrawLine(this, mainLine, mainColor, lineStyle);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolShoulderPoint::setLineP1Id(const quint32 &value)
{
    lineP1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolShoulderPoint::setLineP2Id(const quint32 &value)
{
    lineP2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolShoulderPoint::setLength(const QString &expression)
{
    length = FindLength(expression);
}
