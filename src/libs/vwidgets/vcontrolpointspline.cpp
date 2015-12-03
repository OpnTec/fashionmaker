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
                                         QGraphicsItem *parent)
    :QGraphicsEllipseItem(parent), radius(0), controlLine(nullptr), indexSpline(indexSpline), position(position),
      patternUnit(patternUnit)
{
    //create circle
    radius = (1.5/*mm*/ / 25.4) * PrintDPI;
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPen(QPen(Qt::black, ToPixel(WidthHairLine(patternUnit), patternUnit)));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
    this->setPos(controlPoint);
    this->setZValue(100);

    QPointF p1, p2;
    VGObject::LineIntersectCircle(QPointF(), radius, QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
    controlLine = new QGraphicsLineItem(QLineF(splinePoint-controlPoint, p1), this);
    controlLine->setPen(QPen(Qt::red, ToPixel(WidthHairLine(patternUnit), patternUnit)));
    controlLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
}

//---------------------------------------------------------------------------------------------------------------------
VControlPointSpline::~VControlPointSpline()
{
    //Disable cursor-arrow-openhand
    RestoreOverrideCursor(cursorArrowOpenHand);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /* From question on StackOverflow
     * https://stackoverflow.com/questions/10985028/how-to-remove-border-around-qgraphicsitem-when-selected
     *
     * There's no interface to disable the drawing of the selection border for the build-in QGraphicsItems. The only way
     * I can think of is derive your own items from the build-in ones and override the paint() function:*/
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected;
    QGraphicsEllipseItem::paint(painter, &myOption, widget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover move event.
 */
void VControlPointSpline::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen(Qt::black, ToPixel(WidthMainLine(patternUnit), patternUnit)));
    SetOverrideCursor(cursorArrowOpenHand, 1, 1);
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setPen(QPen(Qt::black, ToPixel(WidthHairLine(patternUnit), patternUnit)));
    //Disable cursor-arrow-openhand
    RestoreOverrideCursor(cursorArrowOpenHand);
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
    if (change == ItemPositionChange && scene())
    {
        // Each time we move something we call recalculation scene rect. In some cases this can cause moving
        // objects positions. And this cause infinite redrawing. That's why we wait the finish of saving the last move.
        static bool changeFinished = true;
        if (changeFinished)
        {
            changeFinished = false;
            // value - new position.
            QPointF newPos = value.toPointF();
            emit ControlPointChangePosition(indexSpline, position, newPos);
            if (scene())
            {
                if (QGraphicsView *view = scene()->views().at(0))
                {
                    view->ensureVisible(this);
                }
            }
            changeFinished = true;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        SetOverrideCursor(cursorArrowCloseHand, 1, 1);
    }
    QGraphicsEllipseItem::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->type() != QEvent::GraphicsSceneMouseDoubleClick)
    {
        //Disable cursor-arrow-closehand
        RestoreOverrideCursor(cursorArrowCloseHand);
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    emit ShowContextMenu(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshLine refresh line control point.
 * @param indexSpline index spline in list.
 * @param pos position point in spline.
 * @param controlPoint control point.
 * @param splinePoint spline point.
 */
void VControlPointSpline::RefreshLine(const qint32 &indexSpline, SplinePointPosition pos,
                                      const QPointF &controlPoint, const QPointF &splinePoint)
{
    if (this->indexSpline == indexSpline && this->position == pos)
    {
        QPointF p1, p2;
        VGObject::LineIntersectCircle(QPointF(), radius, QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
        controlLine->setLine(QLineF(splinePoint-controlPoint, p1));
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
