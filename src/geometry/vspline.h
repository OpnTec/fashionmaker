/************************************************************************
 **
 **  @file   vspline.h
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

#ifndef VSPLINE_H
#define VSPLINE_H

#include "vpointf.h"
#include "vgobject.h"

#include <QHash>
#include <QLineF>
#include <QPainterPath>

class QString;

#define M_2PI   6.28318530717958647692528676655900576

/**
 * @brief VSpline class that implements the spline.
 */
class VSpline :public VGObject
{
public:
    /**
     * @brief VSpline default constructor
     */
    VSpline();
    /**
     * @brief VSpline constructor.
     * @param spline spline from which the copy.
     */
    VSpline (const VSpline &spline );
    /**
     * @brief VSpline constructor.
     * @param p1 first point spline.
     * @param p4 last point spline.
     * @param angle1 angle from first point to first control point.
     * @param angle2 angle from second point to second control point.
     * @param kCurve coefficient of curvature spline.
     * @param kAsm1 coefficient of length first control line.
     * @param kAsm2 coefficient of length second control line.
     */
    VSpline (VPointF p1, VPointF p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2, qreal kCurve,
             quint32 idObject = 0, Draw::Draws mode = Draw::Calculation);
    /**
     * @brief VSpline constructor.
     * @param p1 first point spline.
     * @param p2 first control point.
     * @param p3 second control point.
     * @param p4 second point spline.
     */
    VSpline (VPointF p1, QPointF p2, QPointF p3, VPointF p4, qreal kCurve, quint32 idObject = 0,
             Draw::Draws mode = Draw::Calculation);
    /**
     * @brief GetP1 return first spline point.
     * @return first point.
     */
    VPointF        GetP1 () const {return p1;}
    /**
     * @brief GetP2 return first control point.
     * @return first control point.
     */
    inline QPointF GetP2 () const {return p2;}
    /**
     * @brief GetP3 return second control point.
     * @return second control point.
     */
    inline QPointF GetP3 () const {return p3;}
    /**
     * @brief GetP4 return last spline point.
     * @return остання точка сплайну.
     */
    inline VPointF GetP4 () const {return p4;}
    /**
     * @brief GetAngle1 return first angle control line.
     * @return angle.
     */
    inline qreal   GetAngle1 () const {return angle1;}
    /**
     * @brief GetAngle2 return second angle control line.
     * @return angle.
     */
    inline qreal   GetAngle2() const {return angle2;}
    /**
     * @brief GetLength return length of spline.
     * @return length.
     */
    qreal          GetLength () const;
    /**
     * @brief name return spline name. Used for variables.
     * @return name.
     */
    QString        name () const;
    /**
     * @brief GetKasm1 return coefficient of length first control line.
     * @return coefficient.
     */
    inline qreal   GetKasm1() const {return kAsm1;}
    /**
     * @brief GetKasm2 return coefficient of length second control line.
     * @return coefficient.
     */
    inline qreal   GetKasm2() const {return kAsm2;}
    /**
     * @brief GetKcurve return coefficient of curvature spline.
     * @return coefficient
     */
    inline qreal   GetKcurve() const {return kCurve;}
    /**
     * @brief CrossingSplLine check intersection spline with line.
     * @param line line.
     * @param intersectionPoint intersection point.
     * @return result intersection.
     */
    QLineF::IntersectType CrossingSplLine(const QLineF &line, QPointF *intersectionPoint ) const;
    qreal           LengthT(qreal t) const;
    /**
     * @brief CutSpline cut spline. GetPointP1() of base spline will return first point for first spline, GetPointP4()
     * of base spline will return forth point of second spline.
     * @param length length first spline
     * @param spl1p2 second point of first spline
     * @param spl1p3 third point of first spline
     * @param spl2p2 second point of second spline
     * @param spl2p3 third point of second spline
     * @return point of cutting. This point is forth point of first spline and first point of second spline.
     */
    QPointF         CutSpline ( qreal length, QPointF &spl1p2, QPointF &spl1p3, QPointF &spl2p2, QPointF &spl2p3) const;
    /**
     * @brief GetPoints return list with spline points.
     * @return list of points.
     */
    QVector<QPointF> GetPoints () const;
    /**
     * @brief GetPath return QPainterPath for this spline.
     * @return path.
     */
    QPainterPath   GetPath() const;
    /**
     * @brief SplinePoints return list with spline points.
     * @param p1 first spline point.
     * @param p4 last spline point.
     * @param angle1 angle from first point to first control point.
     * @param angle2 angle from second point to second control point.
     * @param kAsm1 coefficient of length first control line.
     * @param kAsm2 coefficient of length second control line.
     * @param kCurve coefficient of curvature spline.
     * @return
     */
    static QVector<QPointF> SplinePoints(const QPointF &p1, const QPointF &p4, qreal angle1, qreal angle2, qreal kAsm1,
                                         qreal kAsm2, qreal kCurve);
    /**
     * @brief operator = assignmeant operator
     * @param spl spline
     * @return spline
     */
    VSpline        &operator=(const VSpline &spl);
protected:
    /**
     * @brief GetPoints return list with spline points.
     * @param p1 first spline point.
     * @param p2 first control point.
     * @param p3 second control point.
     * @param p4 last spline point.
     * @return list of points.
     */
    static QVector<QPointF> GetPoints (const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4 );
private:
    /**
     * @brief p1 first spline point.
     */
    VPointF        p1;
    /**
     * @brief p2 first control point.
     */
    QPointF        p2;
    /**
     * @brief p3 second control point.
     */
    QPointF        p3;
    /**
     * @brief p4 last spline point.
     */
    VPointF        p4;
    /**
     * @brief angle1 first angle control line.
     */
    qreal          angle1;
    /**
     * @brief angle2 second angle control line.
     */
    qreal          angle2;
    /**
     * @brief kAsm1 coefficient of length first control line.
     */
    qreal          kAsm1;
    /**
     * @brief kAsm2 coefficient of length second control line.
     */
    qreal          kAsm2;
    /**
     * @brief kCurve coefficient of curvature spline.
     */
    qreal          kCurve;
    /**
     * @brief LengthBezier return spline length using 4 spline point.
     * @param p1 first spline point
     * @param p2 first control point.
     * @param p3 second control point.
     * @param p4 last spline point.
     * @return length.
     */
    qreal          LengthBezier (const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4 ) const;
    /**
     * @brief PointBezier_r find spline point using four point of spline.
     * @param x1 х coordinate first point.
     * @param y1 у coordinate first point.
     * @param x2 х coordinate first control point.
     * @param y2 у coordinate first control point.
     * @param x3 х coordinate second control point.
     * @param y3 у coordinate second control point.
     * @param x4 х coordinate last point.
     * @param y4 у coordinate last point.
     * @param level level of recursion. In the begin 0.
     * @param px list х coordinat spline points.
     * @param py list у coordinat spline points.
     */
    static void    PointBezier_r ( qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4,
                                  qint16 level, QVector<qreal> &px, QVector<qreal> &py);
    /**
     * @brief CalcSqDistance calculate squared distance.
     * @param x1 х coordinate first point.
     * @param y1 у coordinate first point.
     * @param x2 х coordinate second point.
     * @param y2 у coordinate second point.
     * @return squared length.
     */
    static qreal   CalcSqDistance ( qreal x1, qreal y1, qreal x2, qreal y2);
    /**
     * @brief CreateName create spline name.
     */
    void           CreateName();
};

#endif // VSPLINE_H
