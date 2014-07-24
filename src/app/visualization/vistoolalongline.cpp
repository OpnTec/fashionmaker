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

#include "vistoolalongline.h"
#include "../container/vcontainer.h"
#include "../geometry/vpointf.h"
#include "../widgets/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolAlongLine::VisToolAlongLine(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), point2Id(0), point(nullptr), lineP1(nullptr), lineP2(nullptr), line(nullptr), length(0)
{
    this->mainColor = Qt::red;
    this->setZValue(2);// Show on top real tool

    lineP1 = InitPoint(supportColor);
    lineP2 = InitPoint(supportColor);
    line = InitLine(supportColor);
    point = InitPoint(mainColor);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolAlongLine::~VisToolAlongLine()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolAlongLine::setPoint2Id(const quint32 &value)
{
    point2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolAlongLine::setLength(const QString &expression)
{
    length = FindLength(expression);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolAlongLine::RefreshGeometry()
{
    if (point1Id > 0)
    {
        const VPointF *first = data->GeometricObject<const VPointF *>(point1Id);
        DrawPoint(lineP1, first->toQPointF(), supportColor);

        if (point2Id <= 0)
        {
            DrawLine(line, QLineF(first->toQPointF(), scenePos), supportColor);
        }
        else
        {
            const VPointF *second = data->GeometricObject<const VPointF *>(point2Id);
            DrawPoint(lineP2, second->toQPointF(), supportColor);

            DrawLine(line, QLineF(first->toQPointF(), second->toQPointF()), supportColor);

            if (qFuzzyCompare(1 + length, 1 + 0) == false)
            {
                QLineF mainLine = Line(first->toQPointF(), length, line->line().angle());
                DrawLine(this, mainLine, mainColor, lineStyle);

                DrawPoint(point, mainLine.p2(), mainColor);
            }
        }
    }
}
