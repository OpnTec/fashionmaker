/************************************************************************
 **
 **  @file   vequidistant.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   28 1, 2014
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

#ifndef VEQUIDISTANT_H
#define VEQUIDISTANT_H

#include <QPainterPath>
#include "../container/vcontainer.h"

/**
 * @brief The VEquidistant class calculate equidistant for detail.
 */
class VEquidistant
{
public:
    /**
     * @brief ContourPath create painter path for detail.
     * @param idDetail id of detail.
     * @param data container with objects (points, arcs, splines).
     * @return return painter path of contour detail.
     */
    QPainterPath     ContourPath(const quint32 &idDetail, const VContainer *data) const;
private:
    /**
     * @brief GetLengthContour return length of contour.
     * @param contour container with points of contour.
     * @param newPoints point whos we try to add to contour.
     * @return length length of contour.
     */
    qreal            GetLengthContour(const QVector<QPointF> &contour, const QVector<QPointF> &newPoints) const;
    /**
     * @brief biasPoints bias point.
     * @param points vector of points.
     * @param mx offset respect to x.
     * @param my offset respect to y.
     * @return new vector biased points.
     */
    QVector<QPointF> biasPoints(const QVector<QPointF> &points, const qreal &mx, const qreal &my) const;
    /**
     * @brief CorrectEquidistantPoints clear equivalent points and remove point on line from equdistant.
     * @param points list of points equdistant.
     * @return corrected list.
     */
    QVector<QPointF> CorrectEquidistantPoints(const QVector<QPointF> &points) const;
    /**
     * @brief Equidistant create equidistant painter path for detail.
     * @param points vector of points.
     * @param eqv type of equidistant.
     * @param width width of equidistant.
     * @return return painter path of equidistant.
     */
    QPainterPath     Equidistant(QVector<QPointF> points, const Detail::Equidistant &eqv, const qreal &width) const;
    /**
     * @brief CheckLoops seek and delete loops in equidistant.
     * @param points vector of points of equidistant.
     * @return vector of points of equidistant.
     */
    QVector<QPointF> CheckLoops(const QVector<QPointF> &points) const;
    /**
     * @brief GetReversePoint return revers container of points.
     * @param points container with points.
     * @return reverced points.
     */
    QVector<QPointF> GetReversePoint(const QVector<QPointF> &points) const;
    /**
     * @brief EkvPoint return vector of points of equidistant two lines. Last point of two lines must be equal.
     * @param line1 first line.
     * @param line2 second line.
     * @param width width of equidistant.
     * @return vector of points.
     */
    QVector<QPointF> EkvPoint(const QLineF &line1, const QLineF &line2, const qreal &width) const;
    /**
     * @brief ParallelLine create parallel line.
     * @param line starting line.
     * @param width width to parallel line.
     * @return parallel line.
     */
    QLineF           ParallelLine(const QLineF &line, qreal width ) const;
    /**
     * @brief SingleParallelPoint return point of parallel line.
     * @param line starting line.
     * @param angle angle in degree.
     * @param width width to parallel line.
     * @return point of parallel line.
     */
    QPointF          SingleParallelPoint(const QLineF &line, const qreal &angle, const qreal &width) const;
};

#endif // VEQUIDISTANT_H
