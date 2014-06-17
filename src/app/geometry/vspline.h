/************************************************************************
 **
 **  @file   vspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
#include <QLineF>
#include <QPointF>

class QPainterPath;

#define M_2PI   6.28318530717958647692528676655900576

/**
 * @brief VSpline class that implements the spline.
 */
class VSpline :public VGObject
{
public:
    VSpline();
    VSpline (const VSpline &spline );
    VSpline (VPointF p1, VPointF p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2, qreal kCurve,
             quint32 idObject = 0, Draw mode = Draw::Calculation);
    VSpline (VPointF p1, QPointF p2, QPointF p3, VPointF p4, qreal kCurve, quint32 idObject = 0,
             Draw mode = Draw::Calculation);
    VSpline &operator=(const VSpline &spl);
    VPointF GetP1 () const;
    QPointF GetP2 () const;
    QPointF GetP3 () const;
    VPointF GetP4 () const;
    qreal   GetAngle1 () const;
    qreal   GetAngle2() const;
    qreal   GetLength () const;
    QString name () const;
    qreal   GetKasm1() const;
    qreal   GetKasm2() const;
    qreal   GetKcurve() const;
    // cppcheck-suppress unusedFunction
    QLineF::IntersectType CrossingSplLine(const QLineF &line, QPointF *intersectionPoint ) const;
    qreal           LengthT(qreal t) const;
    QPointF         CutSpline ( qreal length, QPointF &spl1p2, QPointF &spl1p3, QPointF &spl2p2, QPointF &spl2p3) const;
    QVector<QPointF> GetPoints () const;
    QPainterPath    GetPath() const;
    // cppcheck-suppress unusedFunction
    static QVector<QPointF> SplinePoints(const QPointF &p1, const QPointF &p4, qreal angle1, qreal angle2, qreal kAsm1,
                                         qreal kAsm2, qreal kCurve);
protected:
    static QVector<QPointF> GetPoints (const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4 );
private:
    /** @brief p1 first spline point. */
    VPointF        p1;

    /** @brief p2 first control point. */
    QPointF        p2;

    /** @brief p3 second control point. */
    QPointF        p3;

    /** @brief p4 last spline point. */
    VPointF        p4;

    /** @brief angle1 first angle control line. */
    qreal          angle1;

    /** @brief angle2 second angle control line. */
    qreal          angle2;

    /** @brief kAsm1 coefficient of length first control line. */
    qreal          kAsm1;

    /** @brief kAsm2 coefficient of length second control line. */
    qreal          kAsm2;

    /** @brief kCurve coefficient of curvature spline. */
    qreal          kCurve;
    qreal          LengthBezier (const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4 ) const;
    static void    PointBezier_r ( qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4,
                                  qint16 level, QVector<qreal> &px, QVector<qreal> &py);
    static qreal   CalcSqDistance ( qreal x1, qreal y1, qreal x2, qreal y2);
    void           CreateName();
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP1 return first spline point.
 * @return first point.
 */
inline VPointF VSpline::GetP1() const
{
    return p1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2 return first control point.
 * @return first control point.
 */
inline QPointF VSpline::GetP2() const
{
    return p2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP3 return second control point.
 * @return second control point.
 */
inline QPointF VSpline::GetP3() const
{
    return p3;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP4 return last spline point.
 * @return остання точка сплайну.
 */
inline VPointF VSpline::GetP4() const
{
    return p4;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAngle1 return first angle control line.
 * @return angle.
 */
inline qreal VSpline::GetAngle1() const
{
    return angle1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAngle2 return second angle control line.
 * @return angle.
 */
inline qreal VSpline::GetAngle2() const
{
    return angle2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKasm1 return coefficient of length first control line.
 * @return coefficient.
 */
inline qreal VSpline::GetKasm1() const
{
    return kAsm1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKasm2 return coefficient of length second control line.
 * @return coefficient.
 */
inline qreal VSpline::GetKasm2() const
{
    return kAsm2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKcurve return coefficient of curvature spline.
 * @return coefficient
 */
inline qreal VSpline::GetKcurve() const
{
    return kCurve;
}

#endif // VSPLINE_H
