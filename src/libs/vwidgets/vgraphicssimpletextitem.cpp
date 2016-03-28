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
 **  Copyright (C) 2013-2015 Valentina project
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
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "vmaingraphicsscene.h"
#include "vmaingraphicsview.h"

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
VGraphicsSimpleTextItem::~VGraphicsSimpleTextItem()
{
    //Disable cursor-arrow-openhand
    RestoreOverrideCursor(cursorArrowOpenHand);
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::setEnabled(bool enabled)
{
    QGraphicsSimpleTextItem::setEnabled(enabled);
    const QPalette palet = this->scene()->palette();
    if (enabled)
    {
        setBrush(palet.brush(QPalette::Active, QPalette::Text));
    }
    else
    {
        setBrush(palet.brush(QPalette::Disabled, QPalette::Text));
    }
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
         // Each time we move something we call recalculation scene rect. In some cases this can cause moving
         // objects positions. And this cause infinite redrawing. That's why we wait the finish of saving the last move.
         static bool changeFinished = true;
         if (changeFinished)
         {
            changeFinished = false;
            QPointF newPos = value.toPointF() + this->parentItem()->pos();
            emit NameChangePosition(newPos);
            if (scene())
            {
                const QList<QGraphicsView *> viewList = scene()->views();
                if (not viewList.isEmpty())
                {
                    if (QGraphicsView *view = viewList.at(0))
                    {
                        const int xmargin = 50;
                        const int ymargin = 50;

                        const QRectF viewRect = VMainGraphicsView::SceneVisibleArea(view);
                        const QRectF itemRect = mapToScene(boundingRect()).boundingRect();

                        // If item's rect is bigger than view's rect ensureVisible works very unstable.
                        if (itemRect.height() + 2*ymargin < viewRect.height() &&
                            itemRect.width() + 2*xmargin < viewRect.width())
                        {
                             view->ensureVisible(itemRect, xmargin, ymargin);
                        }
                        else
                        {
                            // Ensure visible only small rect around a cursor
                            VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(scene());
                            SCASSERT(currentScene);
                            const QPointF cursorPosition = currentScene->getScenePos();
                            view->ensureVisible(QRectF(cursorPosition.x()-5, cursorPosition.y()-5, 10, 10));
                        }
                    }
                }
            }
            changeFinished = true;
         }
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
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        this->setBrush(Qt::green);

        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
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
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        this->setBrush(Qt::black);

        //Disable cursor-arrow-openhand
        RestoreOverrideCursor(cursorArrowOpenHand);
    }
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
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            SetOverrideCursor(cursorArrowCloseHand, 1, 1);
        }
    }

    emit PointChoosed();

    QGraphicsSimpleTextItem::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            //Disable cursor-arrow-closehand
            RestoreOverrideCursor(cursorArrowCloseHand);
        }
    }

    QGraphicsSimpleTextItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            emit DeleteTool();
            return; //Leave this method immediately after call!!!
        default:
            break;
    }
    QGraphicsSimpleTextItem::keyReleaseEvent ( event );
}
