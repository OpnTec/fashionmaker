/************************************************************************
 **
 **  @file   vcontrolpointspline.h
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

#ifndef VCONTROLPOINTSPLINE_H
#define VCONTROLPOINTSPLINE_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include "../geometry/vsplinepath.h"

/**
 * @brief The VControlPointSpline class
 */
class VControlPointSpline : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
                      /**
                       * @brief VControlPointSpline
                       * @param indexSpline
                       * @param position
                       * @param controlPoint
                       * @param splinePoint
                       * @param parent
                       */
                      VControlPointSpline(const qint32 &indexSpline, SplinePoint::Position position,
                                          const QPointF &controlPoint, const QPointF &splinePoint,
                                          QGraphicsItem * parent = 0);
signals:
    /**
     * @brief ControlPointChangePosition
     * @param indexSpline
     * @param position
     * @param pos
     */
    void              ControlPointChangePosition(const qint32 &indexSpline, SplinePoint::Position position,
                                                 const QPointF pos);
public slots:
    /**
     * @brief RefreshLine
     * @param indexSpline
     * @param pos
     * @param controlPoint
     * @param splinePoint
     */
    void              RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos, const QPointF &controlPoint,
                                  const QPointF &splinePoint);
    /**
     * @brief setEnabledPoint
     * @param enable
     */
    void              setEnabledPoint(bool enable);
protected:
    /**
     * @brief radius
     */
    qreal             radius;
    /**
     * @brief controlLine
     */
    QGraphicsLineItem *controlLine;
    /**
     * @brief hoverMoveEvent handle hover move events.
     * @param event
     */
    virtual void      hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent handle hover leave events.
     * @param event
     */
    virtual void      hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief itemChange
     * @param change
     * @param value
     * @return
     */
    QVariant          itemChange ( GraphicsItemChange change, const QVariant &value );
private:
    Q_DISABLE_COPY(VControlPointSpline)
    /**
     * @brief indexSpline
     */
    qint32            indexSpline;
    /**
     * @brief position
     */
    SplinePoint::Position position;
    /**
     * @brief LineIntersectCircle
     * @param center
     * @param radius
     * @param line
     * @param p1
     * @param p2
     * @return
     */
    qint32            LineIntersectCircle(const QPointF &center, qreal radius, const QLineF &line, QPointF &p1,
                                          QPointF &p2) const;
    /**
     * @brief ClosestPoint
     * @param line
     * @param p
     * @return
     */
    QPointF           ClosestPoint(const QLineF &line, const QPointF &p) const;
    /**
     * @brief addVector
     * @param p
     * @param p1
     * @param p2
     * @param k
     * @return
     */
    QPointF           addVector (const QPointF &p, const QPointF &p1, const QPointF &p2, qreal k) const;

};

#endif // VCONTROLPOINTSPLINE_H
