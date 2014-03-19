/************************************************************************
 **
 **  @file   vsimplespline.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   17 12, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vsimplespline.h"
#include "../widgets/vapplication.h"

VSimpleSpline::VSimpleSpline(quint32 id, Qt::GlobalColor *currentColor, qreal *factor, QObject *parent)
    :QObject(parent), QGraphicsPathItem(), id (id), factor(factor), currentColor(currentColor)
{
    if (factor == nullptr)
    {
        setPen(QPen(Qt::black, qApp->toPixel(widthHairLine)));
    }
    else
    {
        setPen(QPen(Qt::black, qApp->toPixel(widthHairLine)/ *factor));
    }
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
}

void VSimpleSpline::ChangedActivDraw(const bool &flag)
{
    setFlag(QGraphicsItem::ItemIsSelectable, flag);
    setAcceptHoverEvents(flag);
    setPen(QPen(*currentColor, qApp->toPixel(widthHairLine)/ *factor));
}

void VSimpleSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit Choosed(id);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VSimpleSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (factor == nullptr)
    {
        this->setPen(QPen(*currentColor, qApp->toPixel(widthMainLine)));
    }
    else
    {
        this->setPen(QPen(*currentColor, qApp->toPixel(widthMainLine)/ *factor));
    }
}

void VSimpleSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (factor == nullptr)
    {
        this->setPen(QPen(*currentColor, qApp->toPixel(widthHairLine)));
    }
    else
    {
        this->setPen(QPen(*currentColor, qApp->toPixel(widthHairLine)/ *factor));
    }
}
