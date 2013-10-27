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

#ifndef VCONTROLPOINTSPLINE_H
#define VCONTROLPOINTSPLINE_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include "geometry/vsplinepath.h"

class VControlPointSpline : public QObject, public QGraphicsEllipseItem{
    Q_OBJECT
public:
                      VControlPointSpline(const qint32 &indexSpline, SplinePoint::Position position,
                                          const QPointF &controlPoint, const QPointF &splinePoint,
                                          QGraphicsItem * parent = 0);
signals:
    void              ControlPointChangePosition(const qint32 &indexSpline, SplinePoint::Position position,
                                                 const QPointF pos);
public slots:
    void              RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos, const QPointF &controlPoint,
                                  const QPointF &splinePoint);
    void              setEnabledPoint(bool enable);
protected:
    qreal             radius;
    QGraphicsLineItem *controlLine;
    virtual void      hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void      hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    QVariant          itemChange ( GraphicsItemChange change, const QVariant &value );
private:
    Q_DISABLE_COPY(VControlPointSpline)
    qint32            indexSpline;
    SplinePoint::Position position;
    qint32            LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1, QPointF &p2) const;
    QPointF           ClosestPoint(QLineF line, QPointF p) const;
    QPointF           addVector (QPointF p, QPointF p1, QPointF p2, qreal k) const;

};

#endif // VCONTROLPOINTSPLINE_H
