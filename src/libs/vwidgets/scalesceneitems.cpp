/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 6, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "scalesceneitems.h"
#include "global.h"

#include <QPen>

//---------------------------------------------------------------------------------------------------------------------
VScaledLine::VScaledLine(QGraphicsItem *parent)
    : QGraphicsLineItem(parent),
      basicWidth(widthMainLine)
{}

//---------------------------------------------------------------------------------------------------------------------
VScaledLine::VScaledLine(const QLineF &line, QGraphicsItem *parent)
    : QGraphicsLineItem(line, parent),
      basicWidth(widthMainLine)
{}

//---------------------------------------------------------------------------------------------------------------------
void VScaledLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen lPen = pen();
    lPen.setWidthF(ScaleWidth(basicWidth, SceneScale(scene())));
    setPen(lPen);

    QGraphicsLineItem::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VScaledLine::GetBasicWidth() const
{
    return basicWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VScaledLine::SetBasicWidth(const qreal &value)
{
    basicWidth = value;
}

//---------------------------------------------------------------------------------------------------------------------
VScaledEllipse::VScaledEllipse(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{}

//---------------------------------------------------------------------------------------------------------------------
void VScaledEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());
    const qreal width = ScaleWidth(widthMainLine, scale);

    QPen visPen = pen();
    visPen.setWidthF(width);

    setPen(visPen);
    ScaleCircleSize(this, scale);

    QGraphicsEllipseItem::paint(painter, option, widget);
}
