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
#include "../../vgeometry/vpointf.h"
#include "../../vgeometry/varc.h"
#include "../../vpatterndb/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolArcWithLength::VisToolArcWithLength(const VContainer *data, QGraphicsItem *parent)
    :VisPath(data, parent), arcCenter(nullptr), radius(0), f1(0), length(0)
{
    arcCenter = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolArcWithLength::~VisToolArcWithLength()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolArcWithLength::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(arcCenter, first->toQPointF(), supportColor);

        if (qFuzzyCompare(1 + radius, 1 + 0) == false && f1 >= 0 && qFuzzyCompare(1 + length, 1 + 0) == false)
        {
            VArc arc = VArc (length, *first, radius, f1);
            DrawPath(this, arc.GetPath(PathDirection::Show), mainColor, Qt::SolidLine, Qt::RoundCap);
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
