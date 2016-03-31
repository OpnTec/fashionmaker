/************************************************************************
 **
 **  @file   vcontrolpointspline.cpp
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

#include "vcontrolpointspline.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPen>
#include <QStyleOptionGraphicsItem>

#include "vmaingraphicsview.h"
#include "vmaingraphicsscene.h"

//---------------------------------------------------------------------------------------------------------------------
VControlPointSpline::VControlPointSpline(const qint32 &indexSpline, SplinePointPosition position, Unit patternUnit,
                                         QGraphicsItem *parent)
    :QGraphicsEllipseItem(parent),
      radius(CircleRadius()),
      controlLine(nullptr),
      indexSpline(indexSpline),
      position(position),
      patternUnit(patternUnit),
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
 * @param splinePoint spline point.
 * @param parent parent object.
 */
VControlPointSpline::VControlPointSpline(const qint32 &indexSpline, SplinePointPosition position,
                                         const QPointF &controlPoint, const QPointF &splinePoint, Unit patternUnit,
                                         bool freeAngle, bool freeLength, QGraphicsItem *parent)
    :QGraphicsEllipseItem(parent),
      radius(CircleRadius()),
      controlLine(nullptr),
      indexSpline(indexSpline),
      position(position),
      patternUnit(patternUnit),
      freeAngle(freeAngle),
      freeLength(freeLength)
{
    Init();

    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
    this->setPos(controlPoint);

    SetCtrlLine(controlPoint, splinePoint);
}

//---------------------------------------------------------------------------------------------------------------------
VControlPointSpline::~VControlPointSpline()
{
    //Disable cursor-arrow-openhand
    RestoreOverrideCursor(cursorArrowOpenHand);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover move event.
 */
void VControlPointSpline::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen(Qt::black, ToPixel(WidthMainLine(patternUnit), patternUnit)));
    if (freeAngle || freeLength)
    {
        SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    }
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen(Qt::black, ToPixel(WidthHairLine(patternUnit), patternUnit)));
    if (freeAngle || freeLength)
    {
        //Disable cursor-arrow-openhand
        RestoreOverrideCursor(cursorArrowOpenHand);
    }
    QGraphicsEllipseItem::hoverLeaveEvent(event);
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
            break;
        }
        default:
            break;
    }
    return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        if (freeAngle || freeLength)
        {
            SetOverrideCursor(cursorArrowCloseHand, 1, 1);
        }
    }
    QGraphicsEllipseItem::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        if (freeAngle || freeLength)
        {
            //Disable cursor-arrow-closehand
            RestoreOverrideCursor(cursorArrowCloseHand);
        }
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    emit ShowContextMenu(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::Init()
{
    auto rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPen(QPen(Qt::black, ToPixel(WidthHairLine(patternUnit), patternUnit)));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setZValue(100);

    controlLine = new QGraphicsLineItem(this);
    controlLine->setPen(QPen(Qt::red, ToPixel(WidthHairLine(patternUnit), patternUnit)));
    controlLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::SetCtrlLine(const QPointF &controlPoint, const QPointF &splinePoint)
{
    QPointF p1, p2;
    VGObject::LineIntersectCircle(QPointF(), radius, QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
    controlLine->setLine(QLineF(splinePoint-controlPoint, p1));
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
    if (enable == true)
    {
        this->setPen(QPen(Qt::black, ToPixel(WidthHairLine(patternUnit), patternUnit)));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->setAcceptHoverEvents(true);
    }
    else
    {
        this->setPen(QPen(Qt::gray, ToPixel(WidthHairLine(patternUnit), patternUnit)));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->setFlag(QGraphicsItem::ItemIsMovable, false);
        this->setAcceptHoverEvents(false);
    }
}
