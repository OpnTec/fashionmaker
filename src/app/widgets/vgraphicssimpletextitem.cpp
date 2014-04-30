/************************************************************************
 **
 **  @file   vgraphicssimpletextitem.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "vgraphicssimpletextitem.h"

#include <QtWidgets>

VGraphicsSimpleTextItem::VGraphicsSimpleTextItem(QGraphicsItem * parent)
    :QGraphicsSimpleTextItem(parent), fontSize(0)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
    QFont font = this->font();
    font.setPointSize(font.pointSize()+3);
    fontSize = font.pointSize();
    this->setFont(font);
}

VGraphicsSimpleTextItem::VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent )
    :QGraphicsSimpleTextItem(text, parent), fontSize(0)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

QVariant VGraphicsSimpleTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
     if (change == ItemPositionChange && scene())
     {
         QPointF newPos = value.toPointF() + this->parentItem()->pos();
         emit NameChangePosition(newPos);
     }
     return QGraphicsItem::itemChange(change, value);
}

void VGraphicsSimpleTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setBrush(Qt::green);
}

void VGraphicsSimpleTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setBrush(Qt::black);
}

void VGraphicsSimpleTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    emit ShowContextMenu(event);
}
