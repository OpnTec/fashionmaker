/************************************************************************
 **
 **  @file   vcontrolpointspline.cpp
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

#include "vcontrolpointspline.h"

#include <QBrush>
#include <QEvent>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QLineF>
#include <QList>
#include <QMessageLogger>
#include <QPen>
#include <QPoint>
#include <QPolygonF>
#include <QRectF>
#include <Qt>

#include "../vwidgets/global.h"
#include "../vgeometry/vgobject.h"
#include "../vmisc/vabstractapplication.h"
#include "vmaingraphicsscene.h"
#include "vmaingraphicsview.h"
#include "vgraphicssimpletextitem.h"
#include "scalesceneitems.h"

//---------------------------------------------------------------------------------------------------------------------
VControlPointSpline::VControlPointSpline(const qint32 &indexSpline, SplinePointPosition position, QGraphicsItem *parent)
    : VScenePoint(parent),
      controlLine(nullptr),
      indexSpline(indexSpline),
      position(position),
      freeAngle(true),
      freeLength(true)
{
    Init();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VControlPointSpline constructor.
 * @param indexSpline index spline in list.
 * @param position position point in spline.
 * @param controlPoint control point.
 * @param parent parent object.
 */
VControlPointSpline::VControlPointSpline(const qint32 &indexSpline, SplinePointPosition position,
                                         const QPointF &controlPoint, bool freeAngle,
                                         bool freeLength, QGraphicsItem *parent)
    : VScenePoint(parent),
      controlLine(nullptr),
      indexSpline(indexSpline),
      position(position),
      freeAngle(freeAngle),
      freeLength(freeLength)
{
    Init();

    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
    this->setPos(controlPoint);

    // Do not draw control point in constructor. It creates artefacts if they enabled from start.
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen lPen = controlLine->pen();
    lPen.setColor(CorrectColor(controlLine, Qt::black));
    controlLine->setPen(lPen);

    QPointF p1, p2;
    VGObject::LineIntersectCircle(QPointF(), ScaledRadius(SceneScale(scene())),
                                  QLineF( QPointF(), controlLine->line().p1()), p1, p2);
    QLineF line(controlLine->line().p1(), p1);
    controlLine->setLine(line);

    VScenePoint::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover move event.
 */
void VControlPointSpline::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if ((flags() & QGraphicsItem::ItemIsMovable) && (freeAngle || freeLength))
    {
        SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);
    }
    VScenePoint::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VControlPointSpline::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
        case ItemPositionChange:
        {
            if (not freeAngle || not freeLength)
            {
                const QPointF splPoint = controlLine->line().p1() + pos();

                QLineF newLine(splPoint, value.toPointF());// value - new position.
                QLineF oldLine(splPoint, pos());// pos() - old position.

                if (not freeAngle)
                {
                    newLine.setAngle(oldLine.angle());
                }

                if (not freeLength)
                {
                    newLine.setLength(oldLine.length());
                }

                return newLine.p2();
            }

            break;
        }
        case ItemPositionHasChanged:
        {
            // Each time we move something we call recalculation scene rect. In some cases this can cause moving
            // objects positions. And this cause infinite redrawing. That's why we wait the finish of saving the last
            // move.
            static bool changeFinished = true;
            if (changeFinished)
            {
                changeFinished = false;
                // value - new position.
                emit ControlPointChangePosition(indexSpline, position, value.toPointF());
                if (scene())
                {
                    const QList<QGraphicsView *> viewList = scene()->views();
                    if (not viewList.isEmpty())
                    {
                        if (VMainGraphicsView *view = qobject_cast<VMainGraphicsView *>(viewList.at(0)))
                        {
                            const qreal scale = SceneScale(scene());
                            const int xmargin = qCeil(50/scale);
                            const int ymargin = qCeil(50/scale);

                            const QRectF viewRect = VMainGraphicsView::SceneVisibleArea(view);
                            const QRectF itemRect = sceneBoundingRect();

                            // If item's rect is bigger than view's rect ensureVisible works very unstable.
                            if (itemRect.height() + 2*ymargin < viewRect.height() &&
                                itemRect.width() + 2*xmargin < viewRect.width())
                            {
                                 view->EnsureVisibleWithDelay(itemRect, VMainGraphicsView::scrollDelay, xmargin,
                                                              ymargin);
                            }
                            else
                            {
                                // Ensure visible only small rect around a cursor
                                VMainGraphicsScene *currentScene = qobject_cast<VMainGraphicsScene *>(scene());
                                SCASSERT(currentScene)

                                const QPointF cursorPosition = currentScene->getScenePos();
                                view->EnsureVisibleWithDelay(QRectF(cursorPosition.x()-5/scale,
                                                                    cursorPosition.y()-5/scale, 10/scale, 10/scale),
                                                             VMainGraphicsView::scrollDelay);
                            }
                        }
                    }
                }
                changeFinished = true;
            }
            break;
        }
        case QGraphicsItem::ItemSelectedHasChanged:
            emit Selected(value.toBool());
            break;
        default:
            break;
    }
    return VScenePoint::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        if (freeAngle || freeLength)
        {
            SetItemOverrideCursor(this, cursorArrowCloseHand, 1, 1);
            event->accept();
        }

        // Somehow clicking on notselectable object do not clean previous selections.
        if (not (flags() & ItemIsSelectable) && scene())
        {
            scene()->clearSelection();
        }
    }
    VScenePoint::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        if (freeAngle || freeLength)
        {
            SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);
            emit Released();
        }
    }
    VScenePoint::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    emit ShowContextMenu(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::Init()
{
    m_baseColor = Qt::red;
    SetOnlyPoint(true);
    this->setBrush(QBrush(Qt::NoBrush));
    this->setZValue(100);

    controlLine = new VScaledLine(this);
    controlLine->SetBoldLine(false);
    controlLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    controlLine->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::SetCtrlLine(const QPointF &controlPoint, const QPointF &splinePoint)
{
    QPointF p1, p2;
    VGObject::LineIntersectCircle(QPointF(), ScaledRadius(SceneScale(scene())),
                                  QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
    QLineF line(splinePoint-controlPoint, p1);
    controlLine->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    controlLine->setLine(line);
    controlLine->setVisible(not line.isNull());
    controlLine->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshCtrlPoint refresh the control point.
 * @param indexSpline index spline in list.
 * @param pos position point in spline.
 * @param controlPoint control point.
 * @param splinePoint spline point.
 */
void VControlPointSpline::RefreshCtrlPoint(const qint32 &indexSpline, SplinePointPosition pos,
                                           const QPointF &controlPoint, const QPointF &splinePoint, bool freeAngle,
                                           bool freeLength)
{
    if (this->indexSpline == indexSpline && this->position == pos)
    {
        this->freeAngle = freeAngle;
        this->freeLength = freeLength;
        this->setPos(controlPoint);
        SetCtrlLine(controlPoint, splinePoint);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setEnabledPoint disable or enable control point.
 * @param enable true - enable.
 */
void VControlPointSpline::setEnabledPoint(bool enable)
{
    setEnabled(enable);
    controlLine->setEnabled(enable);
}
