/************************************************************************
 **
 **  @file   vcontrolpointspline.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include "vcontrolpointspline.h"

#include <QPen>

VControlPointSpline::VControlPointSpline(const qint32 &indexSpline, SplinePoint::Position position,
                                         const QPointF &controlPoint, const QPointF &splinePoint,
                                         QGraphicsItem *parent)
    :QGraphicsEllipseItem(parent), radius(toPixel(1.5)), controlLine(0), indexSpline(indexSpline), position(position)
{
    //create circle
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
    this->setPos(controlPoint);

    QPointF p1, p2;
    LineIntersectCircle(QPointF(), radius, QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
    controlLine = new QGraphicsLineItem(QLineF(splinePoint-controlPoint, p1), this);
    controlLine->setPen(QPen(Qt::red, widthHairLine));
    controlLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
}

void VControlPointSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthMainLine));
}

void VControlPointSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthHairLine));
}

QVariant VControlPointSpline::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        // value - new position.
        QPointF newPos = value.toPointF();
        emit ControlPointChangePosition(indexSpline, position, newPos);
    }
    return QGraphicsItem::itemChange(change, value);
}

qint32 VControlPointSpline::LineIntersectCircle(const QPointF &center, qreal radius, const QLineF &line, QPointF &p1,
                                                QPointF &p2) const
{
    const qreal eps = 1e-8;
    //coefficient for equation of segment
    qreal a = line.p2().y() - line.p1().y();
    qreal b = line.p1().x() - line.p2().x();
    // In this case does not used.
    //qreal c = - a * line.p1().x() - b * line.p1().y();
    // projection center of circle on to line
    QPointF p = ClosestPoint (line, center);
    // how many solutions?
    qint32 flag = 0;
    qreal d = QLineF (center, p).length();
    if (qAbs (d - radius) <= eps)
    {
        flag = 1;
    }
    else
    {
        if (radius > d)
        {
            flag = 2;
        }
        else
        {
            return 0;
        }
    }
    // find distance from projection to points of intersection
    qreal k = sqrt (radius * radius - d * d);
    qreal t = QLineF (QPointF (0, 0), QPointF (b, - a)).length();
    // add to projection a vectors aimed to points of intersection
    p1 = addVector (p, QPointF (0, 0), QPointF (- b, a), k / t);
    p2 = addVector (p, QPointF (0, 0), QPointF (b, - a), k / t);
    return flag;
}

QPointF VControlPointSpline::ClosestPoint(const QLineF &line, const QPointF &p) const
{
    QLineF lineP2pointFrom = QLineF(line.p2(), p);
    qreal angle = 180-line.angleTo(lineP2pointFrom)-90;
    QLineF pointFromlineP2 = QLineF(p, line.p2());
    pointFromlineP2.setAngle(pointFromlineP2.angle()+angle);
    QPointF point;
    QLineF::IntersectType type = pointFromlineP2.intersect(line, &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    else
    {
        if ( type == QLineF::NoIntersection || type == QLineF::UnboundedIntersection )
        {
            Q_ASSERT_X(type != QLineF::BoundedIntersection, Q_FUNC_INFO, "Немає точки перетину.");
            return point;
        }
    }
    return point;
}

QPointF VControlPointSpline::addVector(const QPointF &p, const QPointF &p1, const QPointF &p2, qreal k) const
{
    return QPointF (p.x() + (p2.x() - p1.x()) * k, p.y() + (p2.y() - p1.y()) * k);
}

void VControlPointSpline::RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos,
                                      const QPointF &controlPoint, const QPointF &splinePoint)
{
    if (this->indexSpline == indexSpline && this->position == pos)
    {
        QPointF p1, p2;
        LineIntersectCircle(QPointF(), radius, QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
        controlLine->setLine(QLineF(splinePoint-controlPoint, p1));
    }
}

void VControlPointSpline::setEnabledPoint(bool enable)
{
    if (enable == true)
    {
        this->setPen(QPen(Qt::black, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->setAcceptHoverEvents(true);
    }
    else
    {
        this->setPen(QPen(Qt::gray, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->setFlag(QGraphicsItem::ItemIsMovable, false);
        this->setAcceptHoverEvents(false);
    }
}
