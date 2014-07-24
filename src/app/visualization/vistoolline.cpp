/************************************************************************
 **
 **  @file   vgraphicslineitem.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2014
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

#include "vistoolline.h"
#include "../container/vcontainer.h"
#include "../geometry/vpointf.h"
#include "../widgets/vapplication.h"
#include "../tools/drawTools/vdrawtool.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolLine::VisToolLine(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent), point2Id(0)
{
    this->mainColor = Qt::red;
}

//---------------------------------------------------------------------------------------------------------------------
VisToolLine::~VisToolLine()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLine::RefreshGeometry()
{
    QLineF line;
    const VPointF *first = data->GeometricObject<const VPointF *>(point1Id);
    if (point2Id == 0)
    {
        line = QLineF(first->toQPointF(), scenePos);
    }
    else
    {
        const VPointF *second = data->GeometricObject<const VPointF *>(point2Id);
        line = QLineF(first->toQPointF(), second->toQPointF());
    }
    DrawLine(this, line, mainColor, lineStyle);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLine::setPoint2Id(const quint32 &value)
{
    point2Id = value;
}
