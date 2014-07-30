/************************************************************************
 **
 **  @file   vsimplesplinepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "vsimplesplinepath.h"
#include "../widgets/vapplication.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QStyleOptionGraphicsItem>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSimpleSplinePath constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id spline path id.
 * @param factor scale factor.
 */
VSimpleSplinePath::VSimpleSplinePath(VPattern *doc, VContainer *data, quint32 id, qreal *factor)
    :VAbstractTool(doc, data, id), QGraphicsPathItem(), factor(factor)
{}

//---------------------------------------------------------------------------------------------------------------------
void VSimpleSplinePath::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /* From question on StackOverflow
     * https://stackoverflow.com/questions/10985028/how-to-remove-border-around-qgraphicsitem-when-selected
     *
     * There's no interface to disable the drawing of the selection border for the build-in QGraphicsItems. The only way
     * I can think of is derive your own items from the build-in ones and override the paint() function:*/
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected;
    QGraphicsPathItem::paint(painter, &myOption, widget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent handle mouse release events.
 * @param event mouse release event.
 */
void VSimpleSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, SceneObject::SplinePath);
    }
    QGraphicsPathItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverMoveEvent handle hover move events.
 * @param event hover move event.
 */
void VSimpleSplinePath::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthMainLine())/ *factor));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VSimpleSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/ *factor));
}
