/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 6, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../vmisc/vabstractapplication.h"

#include <QPen>

//---------------------------------------------------------------------------------------------------------------------
VScaledLine::VScaledLine(QGraphicsItem *parent)
    : QGraphicsLineItem(parent),
      m_isBoldLine(true)
{}

//---------------------------------------------------------------------------------------------------------------------
VScaledLine::VScaledLine(const QLineF &line, QGraphicsItem *parent)
    : QGraphicsLineItem(line, parent),
      m_isBoldLine(true)
{}

//---------------------------------------------------------------------------------------------------------------------
void VScaledLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen lPen = pen();
    const qreal width = ScaleWidth(m_isBoldLine ? qApp->Settings()->WidthMainLine() : qApp->Settings()->WidthHairLine(),
                                   SceneScale(scene()));
    lPen.setWidthF(qRound(width*100.)/100.);
    setPen(lPen);

    PaintWithFixItemHighlightSelected<QGraphicsLineItem>(this, painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
bool VScaledLine::IsBoldLine() const
{
    return m_isBoldLine;
}

//---------------------------------------------------------------------------------------------------------------------
void VScaledLine::SetBoldLine(bool bold)
{
    m_isBoldLine = bold;
}

//---------------------------------------------------------------------------------------------------------------------
VScaledEllipse::VScaledEllipse(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{}

//---------------------------------------------------------------------------------------------------------------------
void VScaledEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());
    const qreal width = ScaleWidth(qApp->Settings()->WidthMainLine(), scale);

    QPen visPen = pen();
    visPen.setWidthF(width);

    setPen(visPen);
    ScaleCircleSize(this, scale);

    PaintWithFixItemHighlightSelected<QGraphicsEllipseItem>(this, painter, option, widget);
}
