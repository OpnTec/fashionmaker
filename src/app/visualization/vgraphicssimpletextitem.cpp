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
#include <QFont>
#include <QBrush>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "../widgets/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGraphicsSimpleTextItem default constructor.
 * @param parent parent object.
 */
VGraphicsSimpleTextItem::VGraphicsSimpleTextItem(QGraphicsItem * parent)
    :QGraphicsSimpleTextItem(parent), fontSize(0)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
    QFont font = this->font();
    font.setPointSize(font.pointSize()+20);
    fontSize = font.pointSize();
    this->setFont(font);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGraphicsSimpleTextItem constructor.
 * @param text text.
 * @param parent parent object.
 */
VGraphicsSimpleTextItem::VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent )
    :QGraphicsSimpleTextItem(text, parent), fontSize(0)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /* From question on StackOverflow
     * https://stackoverflow.com/questions/10985028/how-to-remove-border-around-qgraphicsitem-when-selected
     *
     * There's no interface to disable the drawing of the selection border for the build-in QGraphicsItems. The only way
     * I can think of is derive your own items from the build-in ones and override the paint() function:*/
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected;
    QGraphicsSimpleTextItem::paint(painter, &myOption, widget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VGraphicsSimpleTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
     if (change == ItemPositionChange && scene())
     {
         QPointF newPos = value.toPointF() + this->parentItem()->pos();
         emit NameChangePosition(newPos);
     }
     return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover enter event.
 */
void VGraphicsSimpleTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(Qt::green);

    VApplication::setOverrideCursor(QStringLiteral("://cursor/cursor-arrow-openhand.png"), 1, 1);
    QGraphicsSimpleTextItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VGraphicsSimpleTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setBrush(Qt::black);

    //Disable cursor-arrow-openhand
    VApplication::restoreOverrideCursor(QStringLiteral("://cursor/cursor-arrow-openhand.png"), 1, 1);
    QGraphicsSimpleTextItem::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VGraphicsSimpleTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    emit ShowContextMenu(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        VApplication::setOverrideCursor(QStringLiteral("://cursor/cursor-arrow-closehand.png"), 1, 1);
    }
    QGraphicsSimpleTextItem::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        //Disable cursor-arrow-closehand
        VApplication::restoreOverrideCursor(QStringLiteral("://cursor/cursor-arrow-closehand.png"), 1, 1);
    }
    QGraphicsSimpleTextItem::mouseReleaseEvent(event);
}
