/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 5, 2017
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

#include "vcurvepathitem.h"
#include "../vwidgets/global.h"

#include <QPainter>

//---------------------------------------------------------------------------------------------------------------------
VCurvePathItem::VCurvePathItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent),
      m_direction()
{
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VCurvePathItem::shape() const
{
    QPainterPath itemPath = path();

    if (m_direction != QPainterPath())
    {
        itemPath.addPath(m_direction);
    }

    itemPath.setFillRule(Qt::WindingFill);

    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (itemPath == QPainterPath() || pen() == Qt::NoPen)
    {
        return itemPath;
    }

    QPainterPathStroker ps;
    ps.setCapStyle(pen().capStyle());
    if (pen().widthF() <= 0.0)
    {
        ps.setWidth(penWidthZero);
    }
    else
    {
        ps.setWidth(pen().widthF());
    }
    ps.setJoinStyle(pen().joinStyle());
    ps.setMiterLimit(pen().miterLimit());
    QPainterPath p = ps.createStroke(itemPath);
    p.addPath(itemPath);
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
void VCurvePathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    ScalePenWidth();

    if (m_direction != QPainterPath())
    {
        painter->save();

        QPen arrowPen(pen());
        arrowPen.setStyle(Qt::SolidLine);

        painter->setPen(arrowPen);
        painter->setBrush(brush());
        painter->drawPath(m_direction);

        painter->restore();
    }

    QGraphicsPathItem::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VCurvePathItem::SetDirectionPath(const QPainterPath &path)
{
    m_direction = path;
}

//---------------------------------------------------------------------------------------------------------------------
void VCurvePathItem::ScalePenWidth()
{
    const qreal width = ScaleWidth(widthMainLine, SceneScale(scene()));

    QPen toolPen = pen();
    toolPen.setWidthF(width);

    setPen(toolPen);
}
