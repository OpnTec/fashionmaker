/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vcontrolpointspline.h"

VControlPointSpline::VControlPointSpline(const qint32 &indexSpline, SplinePoint::Position position,
                                         const QPointF &controlPoint, const QPointF &splinePoint,
                                         QGraphicsItem *parent):QGraphicsEllipseItem(parent),
    radius(1.5*PrintDPI/25.4), controlLine(0), indexSpline(indexSpline), position(position){
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

    //Лінія, що з'єднує дві точки
    QPointF p1, p2;
    LineIntersectCircle(QPointF(), radius, QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
    controlLine = new QGraphicsLineItem(QLineF(splinePoint-controlPoint, p1), this);
    controlLine->setPen(QPen(Qt::red, widthHairLine));
    controlLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
}

void VControlPointSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthMainLine));
}

void VControlPointSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthHairLine));
}

QVariant VControlPointSpline::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value){
    if (change == ItemPositionChange && scene()) {
        // value - new position.
        QPointF newPos = value.toPointF();
        emit ControlPointChangePosition(indexSpline, position, newPos);
    }
    return QGraphicsItem::itemChange(change, value);
}

qint32 VControlPointSpline::LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                                QPointF &p2) const{
    const qreal eps = 1e-8;
    //коефіцієнти для рівняння відрізку
    qreal a = line.p2().y() - line.p1().y();
    qreal b = line.p1().x() - line.p2().x();
    // В даному випадку не використовується.
    //qreal c = - a * line.p1().x() - b * line.p1().y();
    // проекция центра окружности на прямую
    QPointF p = ClosestPoint (line, center);
    // сколько всего решений?
    qint32 flag = 0;
    qreal d = QLineF (center, p).length();
    if (qAbs (d - radius) <= eps){
        flag = 1;
    } else {
        if (radius > d){
            flag = 2;
        } else {
            return 0;
        }
    }
    // находим расстояние от проекции до точек пересечения
    qreal k = sqrt (radius * radius - d * d);
    qreal t = QLineF (QPointF (0, 0), QPointF (b, - a)).length();
    // добавляем к проекции векторы направленные к точкам пеерсечения
    p1 = addVector (p, QPointF (0, 0), QPointF (- b, a), k / t);
    p2 = addVector (p, QPointF (0, 0), QPointF (b, - a), k / t);
    return flag;
}

QPointF VControlPointSpline::ClosestPoint(QLineF line, QPointF p) const{
    QLineF lineP2pointFrom = QLineF(line.p2(), p);
    qreal angle = 180-line.angleTo(lineP2pointFrom)-90;
    QLineF pointFromlineP2 = QLineF(p, line.p2());
    pointFromlineP2.setAngle(pointFromlineP2.angle()+angle);
    QPointF point;
    QLineF::IntersectType type = pointFromlineP2.intersect(line,&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    } else{
        if ( type == QLineF::NoIntersection || type == QLineF::UnboundedIntersection ){
            Q_ASSERT_X(type != QLineF::BoundedIntersection, Q_FUNC_INFO, "Немає точки перетину.");
            return point;
        }
    }
    return point;
}

QPointF VControlPointSpline::addVector(QPointF p, QPointF p1, QPointF p2, qreal k) const{
    return QPointF (p.x() + (p2.x() - p1.x()) * k, p.y() + (p2.y() - p1.y()) * k);
}

void VControlPointSpline::RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos,
                                      const QPointF &controlPoint, const QPointF &splinePoint){
    if(this->indexSpline == indexSpline && this->position == pos){
        QPointF p1, p2;
        LineIntersectCircle(QPointF(), radius, QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
        controlLine->setLine(QLineF(splinePoint-controlPoint, p1));
    }
}

void VControlPointSpline::setEnabledPoint(bool enable){
    if(enable == true){
        this->setPen(QPen(Qt::black, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->setAcceptHoverEvents(true);
    } else {
        this->setPen(QPen(Qt::gray, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->setFlag(QGraphicsItem::ItemIsMovable, false);
        this->setAcceptHoverEvents(false);
    }
}
