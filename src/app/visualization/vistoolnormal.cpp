/************************************************************************
 **
 **  @file   vistoolnormal.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 8, 2014
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

#include "vistoolnormal.h"
#include "../geometry/vpointf.h"
#include "../tools/drawTools/vtoolnormal.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolNormal::VisToolNormal(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), point2Id(0), point(nullptr), lineP1(nullptr), lineP2(nullptr), line(nullptr), length(0),
      angle(0)
{
    this->mainColor = Qt::red;

    lineP1 = InitPoint(supportColor, this);
    lineP2 = InitPoint(supportColor, this);
    line = InitItem<QGraphicsLineItem>(supportColor);

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolNormal::~VisToolNormal()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolNormal::RefreshGeometry()
{
    if (point1Id > 0)
    {
        const VPointF *first = Visualization::data->GeometricObject<const VPointF *>(point1Id);
        DrawPoint(lineP1, first->toQPointF(), supportColor);

        if (point2Id <= 0)
        {
            QLineF line_mouse(first->toQPointF(), Visualization::scenePos);
            DrawLine(line, line_mouse, supportColor);

            QLineF normal = line_mouse.normalVector();
            QPointF endRay = Ray(normal.p1(), normal.angle());
            DrawLine(this, QLineF(normal.p1(), endRay), mainColor);
        }
        else
        {
            const VPointF *second = Visualization::data->GeometricObject<const VPointF *>(point2Id);
            DrawPoint(lineP2, second->toQPointF(), supportColor);

            QLineF line_mouse(first->toQPointF(), second->toQPointF());
            DrawLine(line, line_mouse, supportColor);

            if (qFuzzyCompare(1 + length, 1 + 0))
            {
                QLineF normal = line_mouse.normalVector();
                QPointF endRay = Ray(normal.p1(), normal.angle());
                DrawLine(this, QLineF(normal.p1(), endRay), mainColor);
            }
            else
            {
                QPointF fPoint = VToolNormal::FindPoint(first->toQPointF(), second->toQPointF(), length, angle);
                QLineF mainLine = QLineF(first->toQPointF(), fPoint);
                DrawLine(this, mainLine, mainColor, lineStyle);

                DrawPoint(point, mainLine.p2(), mainColor);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolNormal::setPoint2Id(const quint32 &value)
{
    point2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolNormal::setLength(const QString &expression)
{
    length = FindLength(expression);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VisToolNormal::getAngle() const
{
    return angle;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolNormal::setAngle(const qreal &value)
{
    angle = value;
}
