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
 * @brief The VControlPointSpline class control spline point.
 */
class VControlPointSpline : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
                      /**
                       * @brief VControlPointSpline constructor.
                       * @param indexSpline index spline in list.
                       * @param position position point in spline.
                       * @param controlPoint control point.
                       * @param splinePoint spline point.
                       * @param parent parent object.
                       */
                      VControlPointSpline(const qint32 &indexSpline, SplinePoint::Position position,
                                          const QPointF &controlPoint, const QPointF &splinePoint,
                                          QGraphicsItem * parent = nullptr);
signals:
    /**
     * @brief ControlPointChangePosition emit when control point change position.
     * @param indexSpline index spline in list.
     * @param position position point in spline.
     * @param pos new posotion.
     */
    void              ControlPointChangePosition(const qint32 &indexSpline, SplinePoint::Position position,
                                                 const QPointF pos);
public slots:
    /**
     * @brief RefreshLine refresh line control point.
     * @param indexSpline index spline in list.
     * @param pos position point in spline.
     * @param controlPoint control point.
     * @param splinePoint spline point.
     */
    void              RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos, const QPointF &controlPoint,
                                  const QPointF &splinePoint);
    /**
     * @brief setEnabledPoint disable or enable control point.
     * @param enable true - enable.
     */
    void              setEnabledPoint(bool enable);
protected:
    /**
     * @brief radius radius circle.
     */
    qreal             radius;
    /**
     * @brief controlLine pointer to line control point.
     */
    QGraphicsLineItem *controlLine;
    /**
     * @brief hoverMoveEvent handle hover move events.
     * @param event hover move event.
     */
    virtual void      hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent handle hover leave events.
     * @param event hover leave event.
     */
    virtual void      hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief itemChange handle item change.
     * @param change change.
     * @param value value.
     * @return value.
     */
    QVariant          itemChange ( GraphicsItemChange change, const QVariant &value );
private:
    Q_DISABLE_COPY(VControlPointSpline)
    /**
     * @brief indexSpline index spline in list..
     */
    qint32            indexSpline;
    /**
     * @brief position position point in spline.
     */
    SplinePoint::Position position;
};

#endif // VCONTROLPOINTSPLINE_H
