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
#include "../options.h"

VSimpleSpline::VSimpleSpline(qint64 id, Qt::GlobalColor *currentColor, qreal *factor, QObject *parent)
    :QObject(parent), QGraphicsPathItem(), id (id), factor(factor), currentColor(currentColor)
{
    if (factor == 0)
    {
        setPen(QPen(Qt::black, widthHairLine));
    }
    else
    {
        setPen(QPen(Qt::black, widthHairLine/ *factor));
    }
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
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
    if (factor == 0)
    {
        this->setPen(QPen(*currentColor, widthMainLine));
    }
    else
    {
        this->setPen(QPen(*currentColor, widthMainLine/ *factor));
    }
}

void VSimpleSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (factor == 0)
    {
        this->setPen(QPen(*currentColor, widthHairLine));
    }
    else
    {
        this->setPen(QPen(*currentColor, widthHairLine/ *factor));
    }
}
