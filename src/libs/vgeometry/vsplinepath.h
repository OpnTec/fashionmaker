/************************************************************************
 **
 **  @file   vsplinepath.h
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

#ifndef VSPLINEPATH_H
#define VSPLINEPATH_H

#include "vabstractcurve.h"
#include "vspline.h"
#include "vsplinepoint.h"
#include <QCoreApplication>
#include <QVector>
#include <QPainterPath>

class VSplinePathData;

/**
 * @brief The VSplinePath class keep information about splinePath.
 */
class VSplinePath :public VAbstractCurve
{
    Q_DECLARE_TR_FUNCTIONS(VSplinePath)
public:
    /**
     * @brief VSplinePath constructor.
     * @param kCurve coefficient of curvature spline path.
     * @param idObject parent id.
     * @param mode mode creation spline path.
     */
    VSplinePath(qreal kCurve = 1, quint32 idObject = 0, Draw mode = Draw::Calculation);
    /**
     * @brief VSplinePath copy constructor.
     * @param splPath spline path.
     */
    VSplinePath(const VSplinePath& splPath);
    virtual ~VSplinePath();
    /**
     * @brief append add point in the end of list points.
     * @param point new point.
     */
    void          append(const VSplinePoint &point);
    /**
     * @brief Count return count point.
     * @return count.
     */
    qint32        Count() const;
    /**
     * @brief CountPoint return count point.
     * @return count.
     */
    qint32        CountPoint() const;
    /**
     * @brief GetSpline return spline by index.
     * @param index index spline in spline path.
     * @return spline
     */
    VSpline       GetSpline(qint32 index) const;
    /**
     * @brief GetPath return QPainterPath which reprezent spline path.
     * @return path.
     */
    QPainterPath     GetPath(PathDirection direction = PathDirection::Hide) const;
    /**
     * @brief GetPathPoints return list of points what located on path.
     * @return list.
     */
    QVector<QPointF> GetPoints() const;
    /**
     * @brief GetSplinePath return list with spline points.
     * @return list.
     */
    QVector<VSplinePoint> GetSplinePath() const;
    /**
     * @brief GetLength return length of spline path.
     * @return length.
     */
    qreal         GetLength() const;
    /**
     * @brief UpdatePoint update spline point in list.
     * @param indexSpline spline index in list.
     * @param pos position point in spline.
     * @param point point.
     */
    void          UpdatePoint(qint32 indexSpline, const SplinePointPosition &pos, const VSplinePoint &point);
    /**
     * @brief GetSplinePoint return spline point from list.
     * @param indexSpline spline index in list.
     * @param pos position point in spline.
     * @return spline point.
     */
    VSplinePoint  GetSplinePoint(qint32 indexSpline, SplinePointPosition pos) const;
    /**
     * @brief Clear clear list of points.
     */
    void          Clear();
    /**
     * @brief GetKCurve return coefficient of curvature spline path.
     * @return coefficient of curvature spline.
     */
    qreal         GetKCurve() const;
    /**
     * @brief SetKCurve set coefficient of curvature spline path.
     * @param value coefficient of curvature spline path.
     */
    void          SetKCurve(const qreal &value);
    /**
     * @brief GetPoint pointer to list spline point.
     * @return list.
     */
    const  QVector<VSplinePoint> *GetPoint() const;
    /**
     * @brief operator = assignment operator.
     * @param path spline path.
     * @return spline path.
     */
    VSplinePath   &operator=(const VSplinePath &path);
    /**
     * @brief operator [] return spline point by index.
     * @param indx index in list.
     * @return spline point.
     */
    VSplinePoint  &operator[](int indx);
    /**
     * @brief at return spline point by index.
     * @param indx index in list.
     * @return spline point.
     */
    const VSplinePoint &at(int indx) const;
    /**
     * @brief CutSplinePath cut spline path into two. This method don't return two spline path. You must create spline
     * paths by yourself.
     * Example:
     * QPointF spl1p2, spl1p3, spl2p2, spl2p3;
     * qint32 p1 = 0, p2 = 0;
     * QPointF point = splPath->CutSplinePath(length, p1, p2, spl1p2, spl1p3, spl2p2, spl2p3);
     *
     * VSplinePoint splP1 = splPath->at(p1);
     * VSplinePoint splP2 = splPath->at(p2);
     * VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, *p, splPath->GetKCurve());
     * VSpline spl2 = VSpline(*p, spl2p2, spl2p3, splP2.P(), splPath->GetKCurve());
     * @param length length first spline path.
     * @param p1 index first spline point in list.
     * @param p2 index second spline point in list.
     * @param spl1p2 first control point first spline.
     * @param spl1p3 second control point first spline.
     * @param spl2p2 first control point second spline.
     * @param spl2p3 second control point second spline.
     * @return cutting point.
     */
    QPointF       CutSplinePath(qreal length, qint32 &p1, qint32 &p2, QPointF &spl1p2, QPointF &spl1p3, QPointF &spl2p2,
                                QPointF &spl2p3) const;

    int Segment(const QPointF &p) const;
private:
    QSharedDataPointer<VSplinePathData> d;
};

#endif // VSPLINEPATH_H
