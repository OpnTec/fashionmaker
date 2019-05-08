/************************************************************************
 **
 **  @file   vgraphicssimpletextitem.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <QEvent>
#include <QFlags>
#include <QFont>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QList>
#include <QMessageLogger>
#include <QPalette>
#include <QPoint>
#include <QPolygonF>
#include <QRectF>
#include <Qt>

#include "vmaingraphicsscene.h"
#include "vmaingraphicsview.h"
#include "global.h"
#include "vscenepoint.h"
#include "../vmisc/vmath.h"
#include "../vmisc/vabstractapplication.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGraphicsSimpleTextItem default constructor.
 * @param parent parent object.
 */
VGraphicsSimpleTextItem::VGraphicsSimpleTextItem(QGraphicsItem * parent)
    : QGraphicsSimpleTextItem(parent),
      m_fontSize(0),
      selectionType(SelectionType::ByMouseRelease),
      m_oldScale(1),
      m_showParentTooltip(true)
{
    Init();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGraphicsSimpleTextItem constructor.
 * @param text text.
 * @param parent parent object.
 */
VGraphicsSimpleTextItem::VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent )
    : QGraphicsSimpleTextItem(text, parent),
      m_fontSize(0),
      selectionType(SelectionType::ByMouseRelease),
      m_oldScale(1),
      m_showParentTooltip(true)
{
    Init();
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    auto UpdateLine = [this]()
    {
        if (VScenePoint *parent = dynamic_cast<VScenePoint *>(parentItem()))
        {
            parent->RefreshLine();
        }
    };

    QGraphicsScene *scene = this->scene();
    const qreal scale = SceneScale(scene);
    if (scale > 1 && not VFuzzyComparePossibleNulls(m_oldScale, scale))
    {
        setScale(1/scale);
        CorrectLabelPosition();
        UpdateLine();
        m_oldScale = scale;
    }
    else if (scale <= 1 && not VFuzzyComparePossibleNulls(m_oldScale, 1.0))
    {
        setScale(1);
        CorrectLabelPosition();
        UpdateLine();
        m_oldScale = 1;
    }

    if (QGraphicsView *view = scene->views().at(0))
    {
        VMainGraphicsView::NewSceneRect(scene, view, this);
    }
    PaintWithFixItemHighlightSelected<QGraphicsSimpleTextItem>(this, painter, option, widget);
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
void VGraphicsSimpleTextItem::LabelSelectionType(const SelectionType &type)
{
    selectionType = type;
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::SetShowParentTooltip(bool show)
{
    m_showParentTooltip = show;
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::SetRealPos(QPointF pos)
{
    m_realPos = pos;

    CorrectLabelPosition();
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::CorrectLabelPosition()
{
    const qreal scale = SceneScale(scene());
    QPointF newPos = m_realPos;

    if (scale > 1)
    {
        QLineF line(QPointF(), m_realPos);
        line.setLength(line.length() / scale);
        newPos = line.p2();
    }

    blockSignals(true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    setPos(newPos);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    blockSignals(false);
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
            if (scene())
            {
                const QList<QGraphicsView *> viewList = scene()->views();
                if (not viewList.isEmpty())
                {
                    if (VMainGraphicsView *view = qobject_cast<VMainGraphicsView *>(viewList.at(0)))
                    {
                        const qreal scale = SceneScale(scene());
                        int xmargin = qCeil(50/scale);
                        int ymargin = qCeil(50/scale);

                        const QRectF viewRect = VMainGraphicsView::SceneVisibleArea(view);
                        const QRectF itemRect = mapToScene(boundingRect()).boundingRect();

                        // If item's rect is bigger than view's rect ensureVisible works very unstable.
                        if (itemRect.height() + 2*ymargin < viewRect.height() &&
                            itemRect.width() + 2*xmargin < viewRect.width())
                        {
                            view->EnsureVisibleWithDelay(itemRect, VMainGraphicsView::scrollDelay, xmargin, ymargin);
                        }
                        else
                        {
                            // Ensure visible only small rect around a cursor
                            VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(scene());
                            SCASSERT(currentScene)
                            const QPointF cursorPosition = currentScene->getScenePos();

                            view->EnsureVisibleWithDelay(QRectF(cursorPosition.x()-5/scale, cursorPosition.y()-5/scale,
                                                                10/scale, 10/scale),
                                                         VMainGraphicsView::scrollDelay);
                        }
                    }
                }
            }

            m_realPos = value.toPointF();
            const qreal scale = SceneScale(scene());
            if (scale > 1)
            {
                QLineF line(QPointF(), m_realPos);
                line.setLength(line.length() * scale);
                m_realPos = line.p2();
            }
            emit NameChangePosition(m_realPos + this->parentItem()->pos());

            changeFinished = true;
         }
     }
     if (change == QGraphicsItem::ItemSelectedHasChanged)
     {
         setFlag(QGraphicsItem::ItemIsFocusable, value.toBool());
         emit PointSelected(value.toBool());
     }
     return QGraphicsSimpleTextItem::itemChange(change, value);
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
        SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);
    }
    else
    {
        setCursor(qApp->getSceneView()->viewport()->cursor());
    }

    this->setBrush(Qt::green);

    QGraphicsItem *parent = parentItem();
    if(parent && m_showParentTooltip)
    {
        setToolTip(parent->toolTip());
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
    this->setBrush(m_baseColor);
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
    // Special for not selectable item first need to call standard mousePressEvent then accept event
    QGraphicsSimpleTextItem::mousePressEvent(event);

    // Somehow clicking on notselectable object do not clean previous selections.
    if (not (flags() & ItemIsSelectable) && scene())
    {
        scene()->clearSelection();
    }

    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            SetItemOverrideCursor(this, cursorArrowCloseHand, 1, 1);
            event->accept();
        }
    }
    if (selectionType == SelectionType::ByMouseRelease)
    {
        event->accept(); // This help for not selectable items still receive mouseReleaseEvent events
    }
    else
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            emit PointChoosed();
            event->accept();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        {
            SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);
        }
    }

    if (selectionType == SelectionType::ByMouseRelease && IsSelectedByReleaseEvent(this, event))
    {
        emit PointChoosed();
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

//---------------------------------------------------------------------------------------------------------------------
QColor VGraphicsSimpleTextItem::BaseColor() const
{
    return m_baseColor;
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::SetBaseColor(const QColor &baseColor)
{
    m_baseColor = baseColor;
    setBrush(m_baseColor);
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsSimpleTextItem::Init()
{
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus
    this->setAcceptHoverEvents(true);
    QFont font = this->font();
    font.setPointSize(font.pointSize()+20);
    m_fontSize = font.pointSize();
    this->setFont(font);
    m_oldScale = minVisibleFontSize / m_fontSize;
    setScale(m_oldScale);
}
