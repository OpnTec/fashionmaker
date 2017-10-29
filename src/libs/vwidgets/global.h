/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 6, 2017
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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGlobal>
#include <QStyleOptionGraphicsItem>

extern const qreal widthMainLine;
extern const qreal widthHairLine;
extern const qreal minVisibleFontSize;

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsEllipseItem;
class QGraphicsLineItem;
class QColor;
class QRectF;
class QPainterPath;
class QPen;

qreal SceneScale(QGraphicsScene *scene);

QColor CorrectColor(const QGraphicsItem *item, const QColor &color);

QRectF PointRect(qreal radius);
qreal  ScaledRadius(qreal scale);
void   ScaleCircleSize(QGraphicsEllipseItem *item, qreal scale);
qreal  ScaleWidth(qreal width, qreal scale);

QPainterPath ItemShapeFromPath(const QPainterPath &path, const QPen &pen);

void GraphicsItemHighlightSelected(const QRectF &boundingRect, qreal itemPenWidth, QPainter *painter,
                                   const QStyleOptionGraphicsItem *option);

/* Helps fix bug with item highlight selected.
 * Unfortunatelly qt_graphicsItem_highlightSelected was designed in way that doesn't support custom QGraphicsItem
 * classes. Such classes always get pen width 1.0. This make imposible to draw selection rect around very small objects.
 */
template<class Parent, class Item>
void PaintWithFixItemHighlightSelected(Item *item, QPainter *painter, const QStyleOptionGraphicsItem *option,
                                       QWidget *widget)
{
    QStyleOptionGraphicsItem myoption = (*option);
    if (myoption.state & QStyle::State_Selected)
    {
        myoption.state &= !QStyle::State_Selected;
    }
    item->Parent::paint(painter, &myoption, widget);

    if (option->state & QStyle::State_Selected)
    {
        GraphicsItemHighlightSelected(item->boundingRect(), item->pen().widthF(), painter, option);
    }
}

#endif // GLOBAL_H
