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
#include "../vgeometry/vabstractcurve.h"

#include <QPainter>

//---------------------------------------------------------------------------------------------------------------------
VCurvePathItem::VCurvePathItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent),
      m_directionArrows(),
      m_points()
{
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VCurvePathItem::shape() const
{
    QPainterPath itemPath;

    if (not m_points.isEmpty())
    {
        for (qint32 i = 0; i < m_points.count()-1; ++i)
        {
            itemPath.moveTo(m_points.at(i));
            itemPath.lineTo(m_points.at(i+1));
        }
    }
    else
    {
        itemPath = path();
    }

    const QPainterPath arrowsPath = VAbstractCurve::ShowDirection(m_directionArrows,
                                                                  ScaleWidth(VAbstractCurve::lengthCurveDirectionArrow,
                                                                             SceneScale(scene())));
    if (arrowsPath != QPainterPath())
    {
        itemPath.addPath(arrowsPath);
    }
    itemPath.setFillRule(Qt::WindingFill);
    return ItemShapeFromPath(itemPath, pen());
}

//---------------------------------------------------------------------------------------------------------------------
void VCurvePathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    ScalePenWidth();

    const QPainterPath arrowsPath = VAbstractCurve::ShowDirection(m_directionArrows,
                                                                  ScaleWidth(VAbstractCurve::lengthCurveDirectionArrow,
                                                                             SceneScale(scene())));

    if (arrowsPath != QPainterPath())
    {
        painter->save();

        QPen arrowPen(pen());
        arrowPen.setStyle(Qt::SolidLine);

        painter->setPen(arrowPen);
        painter->setBrush(brush());
        painter->drawPath(arrowsPath);

        painter->restore();
    }

    QGraphicsPathItem::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VCurvePathItem::SetDirectionArrows(const QVector<QPair<QLineF, QLineF> > &arrows)
{
    m_directionArrows = arrows;
}

//---------------------------------------------------------------------------------------------------------------------
void VCurvePathItem::SetPoints(const QVector<QPointF> &points)
{
    m_points = points;
}

//---------------------------------------------------------------------------------------------------------------------
void VCurvePathItem::ScalePenWidth()
{
    const qreal width = ScaleWidth(widthMainLine, SceneScale(scene()));

    QPen toolPen = pen();
    toolPen.setWidthF(width);

    setPen(toolPen);
}
