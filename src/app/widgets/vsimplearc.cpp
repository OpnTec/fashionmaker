/************************************************************************
 **
 **  @file   vsimplearc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
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

#include "vsimplearc.h"
#include "../widgets/vapplication.h"
#include <QPen>
#include <QGraphicsSceneMouseEvent>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSimpleArc constructor.
 * @param id arc id.
 * @param currentColor current color.
 * @param factor scale factor.
 * @param parent parent object.
 */
VSimpleArc::VSimpleArc(quint32 id, Qt::GlobalColor *currentColor, qreal *factor, QObject *parent)
    :QObject(parent), QGraphicsPathItem(), id (id), factor(factor), currentColor(currentColor)
{
    if (factor == nullptr)
    {
        setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())));
    }
    else
    {
        setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/ *factor));
    }
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent handle mouse release events.
 * @param event mouse release event.
 */
void VSimpleArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit Choosed(id);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverMoveEvent handle hover move events.
 * @param event hover move event.
 */
void VSimpleArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (factor == nullptr)
    {
        this->setPen(QPen(*currentColor, qApp->toPixel(qApp->widthMainLine())));
    }
    else
    {
        this->setPen(QPen(*currentColor, qApp->toPixel(qApp->widthMainLine())/ *factor));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VSimpleArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if (factor == nullptr)
    {
        this->setPen(QPen(*currentColor, qApp->toPixel(qApp->widthHairLine())));
    }
    else
    {
        this->setPen(QPen(*currentColor, qApp->toPixel(qApp->widthHairLine())/ *factor));
    }
}
