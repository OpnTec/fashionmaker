/************************************************************************
 **
 **  @file   vistoolspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 8, 2014
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

#include "vistoolspline.h"
#include "../libs/vgeometry/vpointf.h"
#include "../libs/vgeometry/vspline.h"
#include "../libs/vpatterndb/vcontainer.h"

const int EMPTY_ANGLE = -1;

//---------------------------------------------------------------------------------------------------------------------
VisToolSpline::VisToolSpline(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent), point4Id(NULL_ID), lineP1(nullptr), lineP4(nullptr), line(nullptr), angle1(EMPTY_ANGLE),
      angle2(EMPTY_ANGLE), kAsm1(1), kAsm2(1), kCurve(1)
{
    lineP1 = InitPoint(supportColor, this);
    lineP4 = InitPoint(supportColor, this);
    line = InitItem<QGraphicsLineItem>(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolSpline::~VisToolSpline()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::RefreshGeometry()
{
    if (point1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(point1Id);
        DrawPoint(lineP1, first->toQPointF(), supportColor);

        if (point4Id <= NULL_ID)
        {
            DrawLine(line, QLineF(first->toQPointF(), Visualization::scenePos), mainColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(point4Id);
            DrawPoint(lineP4, second->toQPointF(), supportColor);

            if (qFuzzyCompare(angle1, EMPTY_ANGLE) || qFuzzyCompare(angle2, EMPTY_ANGLE))
            {
                DrawLine(line, QLineF(first->toQPointF(), second->toQPointF()), mainColor);
            }
            else
            {
                VSpline spline(*first, *second, angle1, angle2, kAsm1, kAsm2, kCurve);
                DrawPath(this, spline.GetPath(PathDirection::Show), mainColor, Qt::SolidLine, Qt::RoundCap);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::setPoint4Id(const quint32 &value)
{
    point4Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetAngle1(const qreal &value)
{
    angle1 = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetAngle2(const qreal &value)
{
    angle2 = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetKAsm1(const qreal &value)
{
    kAsm1 = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetKAsm2(const qreal &value)
{
    kAsm2 = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetKCurve(const qreal &value)
{
    kCurve = value;
}
