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
    :VisPath(data, parent), point(nullptr), length(0)
{
    point = InitPoint(mainColor, this);
    point->setZValue(2);
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
            DrawPoint(point, arc->CutArc(length), mainColor);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCutArc::setLength(const QString &expression)
{
    length = FindLength(expression);
}
