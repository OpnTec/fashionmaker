/************************************************************************
 **
 **  @file vcontrolpointspline.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
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

#ifndef VCONTROLPOINTSPLINE_H
#define VCONTROLPOINTSPLINE_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include "../geometry/vsplinepath.h"

class VControlPointSpline : public QObject, public QGraphicsEllipseItem
{
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
    qint32            LineIntersectCircle(const QPointF &center, qreal radius, const QLineF &line, QPointF &p1,
                                          QPointF &p2) const;
    QPointF           ClosestPoint(const QLineF &line, const QPointF &p) const;
    QPointF           addVector (const QPointF &p, const QPointF &p1, const QPointF &p2, qreal k) const;

};

#endif // VCONTROLPOINTSPLINE_H
