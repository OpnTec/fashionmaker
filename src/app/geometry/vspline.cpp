/************************************************************************
 **
 **  @file   vspline.cpp
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

#include "vspline.h"
#include <cmath>
#include <QDebug>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSpline default constructor
 */
VSpline::VSpline()
    :VGObject(GObject::Spline), p1(VPointF()), p2(QPointF()), p3(QPointF()), p4(VPointF()), angle1(0), angle2(0),
      kAsm1(1), kAsm2(1), kCurve(1)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSpline constructor.
 * @param spline spline from which the copy.
 */
VSpline::VSpline ( const VSpline & spline )
    :VGObject(spline), p1(spline.GetP1 ()), p2(spline.GetP2 ()), p3(spline.GetP3 ()), p4(spline.GetP4 ()),
      angle1(spline.GetAngle1 ()), angle2(spline.GetAngle2 ()), kAsm1(spline.GetKasm1()), kAsm2(spline.GetKasm2()),
      kCurve(spline.GetKcurve())
{}

//---------------------------------------------------------------------------------------------------------------------
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
VSpline::VSpline (VPointF p1, VPointF p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2, qreal kCurve,
                  quint32 idObject, Valentina::Draws mode)
    :VGObject(GObject::Spline, idObject, mode), p1(p1), p2(QPointF()), p3(QPointF()), p4(p4), angle1(angle1),
      angle2(angle2), kAsm1(kAsm1), kAsm2(kAsm2), kCurve(kCurve)
{
    CreateName();

    this->p1 = p1;
    this->p4 = p4;
    this->angle1 = angle1;
    this->angle2 = angle2;
    this->kAsm1 = kAsm1;
    this->kAsm2 = kAsm2;
    this->kCurve = kCurve;

    qreal L = 0, radius = 0, angle = 90;
    QPointF point1 = GetP1().toQPointF();
    QPointF point4 = GetP4().toQPointF();
    radius = QLineF(point1, point4).length()/1.414213;//1.414213=sqrt(2);
    L = kCurve * radius * 4 / 3 * tan( angle * M_PI / 180.0 / 4 );
    QLineF p1p2(GetP1().x(), GetP1().y(), GetP1().x() + L * kAsm1, GetP1().y());
    p1p2.setAngle(angle1);
    QLineF p4p3(GetP4().x(), GetP4().y(), GetP4().x() + L * kAsm2, GetP4().y());
    p4p3.setAngle(angle2);
    this->p2 = p1p2.p2();
    this->p3 = p4p3.p2();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSpline constructor.
 * @param p1 first point spline.
 * @param p2 first control point.
 * @param p3 second control point.
 * @param p4 second point spline.
 */
VSpline::VSpline (VPointF p1, QPointF p2, QPointF p3, VPointF p4, qreal kCurve, quint32 idObject, Valentina::Draws mode)
    :VGObject(GObject::Spline, idObject, mode), p1(p1), p2(p2), p3(p3), p4(p4), angle1(0), angle2(0), kAsm1(1),
      kAsm2(1), kCurve(1)
{
    CreateName();

    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->p4 = p4;
    this->angle1 = QLineF ( GetP1().toQPointF(), p2 ).angle();
    this->angle2 = QLineF ( GetP4().toQPointF(), p3 ).angle();

    qreal L = 0, radius = 0, angle = 90;
    QPointF point1 = GetP1().toQPointF();
    QPointF point4 = GetP4().toQPointF();
    radius = QLineF(point1, point4).length()/1.414213;//1.414213=sqrt(2);
    L = kCurve * radius * 4 / 3 * tan( angle * M_PI / 180.0 / 4 );

    this->kCurve = kCurve;
    this->kAsm1 = QLineF ( GetP1().toQPointF(), p2 ).length()/L;
    this->kAsm2 = QLineF ( GetP4().toQPointF(), p3 ).length()/L;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLength return length of spline.
 * @return length.
 */
qreal VSpline::GetLength () const
{
    return LengthBezier ( GetP1().toQPointF(), this->p2, this->p3, GetP4().toQPointF());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief name return spline name. Used for variables.
 * @return name.
 */
QString VSpline::name() const
{
    return _name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CrossingSplLine check intersection spline with line.
 * @param line line.
 * @param intersectionPoint intersection point.
 * @return result intersection.
 */
// cppcheck-suppress unusedFunction
QLineF::IntersectType VSpline::CrossingSplLine ( const QLineF &line, QPointF *intersectionPoint ) const
{
    QVector<qreal> px;
    QVector<qreal> py;
    px.append ( GetP1 ().x () );
    py.append ( GetP1 ().y () );
    QVector<qreal>& wpx = px;
    QVector<qreal>& wpy = py;
    PointBezier_r ( GetP1 ().x (), GetP1 ().y (), GetP2 ().x (), GetP2 ().y (),
                    GetP3 ().x (), GetP3 ().y (), GetP4 ().x (), GetP4 ().y (),
                    0, wpx, wpy);
    px.append ( GetP4 ().x () );
    py.append ( GetP4 ().y () );
    qint32 i = 0;
    QPointF crosPoint;
    QLineF::IntersectType type = QLineF::NoIntersection;
    for ( i = 0; i < px.count()-1; ++i )
    {
        type = line.intersect(QLineF ( QPointF ( px[i], py[i] ),
                                       QPointF ( px[i+1], py[i+1] )), &crosPoint);
        if ( type == QLineF::BoundedIntersection )
        {
            *intersectionPoint = crosPoint;
            return type;
        }
    }
    throw "Can't found point of intersection spline and line.";
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSpline::LengthT(qreal t) const
{
    if (t < 0 || t > 1)
    {
        qDebug()<<"Wrong value t.";
        return 0;
    }
    QLineF seg1_2 ( GetP1 ().toQPointF(), GetP2 () );
    seg1_2.setLength(seg1_2.length () * t);
    QPointF p12 = seg1_2.p2();

    QLineF seg2_3 ( GetP2 (), GetP3 () );
    seg2_3.setLength(seg2_3.length () * t);
    QPointF p23 = seg2_3.p2();

    QLineF seg12_23 ( p12, p23 );
    seg12_23.setLength(seg12_23.length () * t);
    QPointF p123 = seg12_23.p2();

    QLineF seg3_4 ( GetP3 (), GetP4 ().toQPointF() );
    seg3_4.setLength(seg3_4.length () * t);
    QPointF p34 = seg3_4.p2();

    QLineF seg23_34 ( p23, p34 );
    seg23_34.setLength(seg23_34.length () * t);
    QPointF p234 = seg23_34.p2();

    QLineF seg123_234 ( p123, p234 );
    seg123_234.setLength(seg123_234.length () * t);
    QPointF p1234 = seg123_234.p2();

    return LengthBezier ( GetP1().toQPointF(), p12, p123, p1234);
}

//---------------------------------------------------------------------------------------------------------------------
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
QPointF VSpline::CutSpline ( qreal length, QPointF &spl1p2, QPointF &spl1p3, QPointF &spl2p2, QPointF &spl2p3 ) const
{
    //Always need return two splines, so we must correct wrong length.
    if (length < GetLength()*0.02)
    {
        length = GetLength()*0.02;
    }
    else if ( length > GetLength()*0.98)
    {
        length = GetLength()*0.98;
    }

    // Very stupid way find correct value of t.
    // Better first compare with t = 0.5. Find length of spline.
    // If length larger, take t = 0.75 and so on.
    // If length less, take t = 0.25 and so on.
    qreal parT = 0;
    qreal step = 0.001;
    while (1)
    {
        parT = parT + step;
        qreal splLength = LengthT(parT);
        if (splLength >= length || parT > 1)
        {
            break;
        }
    }

    QLineF seg1_2 ( GetP1 ().toQPointF(), GetP2 () );
    seg1_2.setLength(seg1_2.length () * parT);
    QPointF p12 = seg1_2.p2();

    QLineF seg2_3 ( GetP2 (), GetP3 () );
    seg2_3.setLength(seg2_3.length () * parT);
    QPointF p23 = seg2_3.p2();

    QLineF seg12_23 ( p12, p23 );
    seg12_23.setLength(seg12_23.length () * parT);
    QPointF p123 = seg12_23.p2();

    QLineF seg3_4 ( GetP3 (), GetP4 ().toQPointF() );
    seg3_4.setLength(seg3_4.length () * parT);
    QPointF p34 = seg3_4.p2();

    QLineF seg23_34 ( p23, p34 );
    seg23_34.setLength(seg23_34.length () * parT);
    QPointF p234 = seg23_34.p2();

    QLineF seg123_234 ( p123, p234 );
    seg123_234.setLength(seg123_234.length () * parT);
    QPointF p1234 = seg123_234.p2();

    spl1p2 = p12;
    spl1p3 = p123;
    spl2p2 = p234;
    spl2p3 = p34;
    return p1234;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoints return list with spline points.
 * @return list of points.
 */
QVector<QPointF> VSpline::GetPoints () const
{
    return GetPoints(GetP1().toQPointF(), p2, p3, GetP4().toQPointF());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoints return list with spline points.
 * @param p1 first spline point.
 * @param p2 first control point.
 * @param p3 second control point.
 * @param p4 last spline point.
 * @return list of points.
 */
QVector<QPointF> VSpline::GetPoints (const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    QVector<QPointF> pvector;
    QVector<qreal> x;
    QVector<qreal> y;
    QVector<qreal>& wx = x;
    QVector<qreal>& wy = y;
    x.append ( p1.x () );
    y.append ( p1.y () );
    PointBezier_r ( p1.x (), p1.y (), p2.x (), p2.y (),
                    p3.x (), p3.y (), p4.x (), p4.y (), 0, wx, wy );
    x.append ( p4.x () );
    y.append ( p4.y () );
    for ( qint32 i = 0; i < x.count(); ++i )
    {
        pvector.append( QPointF ( x[i], y[i] ) );
    }
    return pvector;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LengthBezier return spline length using 4 spline point.
 * @param p1 first spline point
 * @param p2 first control point.
 * @param p3 second control point.
 * @param p4 last spline point.
 * @return length.
 */
qreal VSpline::LengthBezier ( const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4 ) const
{
    QPainterPath splinePath;
    QVector<QPointF> points = GetPoints (p1, p2, p3, p4);
    splinePath.moveTo(points[0]);
    for (qint32 i = 1; i < points.count(); ++i)
    {
        splinePath.lineTo(points[i]);
    }
    return splinePath.length();
}

//---------------------------------------------------------------------------------------------------------------------
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
void VSpline::PointBezier_r ( qreal x1, qreal y1, qreal x2, qreal y2,
                              qreal x3, qreal y3, qreal x4, qreal y4,
                              qint16 level, QVector<qreal> &px, QVector<qreal> &py)
{
    const double curve_collinearity_epsilon                 = 1e-30;
    const double curve_angle_tolerance_epsilon              = 0.01;
    const double m_angle_tolerance = 0.0;
    enum curve_recursion_limit_e { curve_recursion_limit = 32 };
    const double m_cusp_limit = 0.0;
    double m_approximation_scale = 1.0;
    double m_distance_tolerance_square;

    m_distance_tolerance_square = 0.5 / m_approximation_scale;
    m_distance_tolerance_square *= m_distance_tolerance_square;

    if (level > curve_recursion_limit)
    {
        return;
    }

    // Calculate all the mid-points of the line segments
    //----------------------
    const double x12   = (x1 + x2) / 2;
    const double y12   = (y1 + y2) / 2;
    const double x23   = (x2 + x3) / 2;
    const double y23   = (y2 + y3) / 2;
    const double x34   = (x3 + x4) / 2;
    const double y34   = (y3 + y4) / 2;
    const double x123  = (x12 + x23) / 2;
    const double y123  = (y12 + y23) / 2;
    const double x234  = (x23 + x34) / 2;
    const double y234  = (y23 + y34) / 2;
    const double x1234 = (x123 + x234) / 2;
    const double y1234 = (y123 + y234) / 2;


    // Try to approximate the full cubic curve by a single straight line
    //------------------
    const double dx = x4-x1;
    const double dy = y4-y1;

    double d2 = fabs((x2 - x4) * dy - (y2 - y4) * dx);
    double d3 = fabs((x3 - x4) * dy - (y3 - y4) * dx);

    switch ((static_cast<int>(d2 > curve_collinearity_epsilon) << 1) +
             static_cast<int>(d3 > curve_collinearity_epsilon))
    {
        case 0:
        {
            // All collinear OR p1==p4
            //----------------------
            double k = dx*dx + dy*dy;
            if (k < 0.000000001)
            {
                d2 = CalcSqDistance(x1, y1, x2, y2);
                d3 = CalcSqDistance(x4, y4, x3, y3);
            }
            else
            {
                k   = 1 / k;
                {
                    const double da1 = x2 - x1;
                    const double da2 = y2 - y1;
                    d2  = k * (da1*dx + da2*dy);
                }
                {
                    const double da1 = x3 - x1;
                    const double da2 = y3 - y1;
                    d3  = k * (da1*dx + da2*dy);
                }
                // cppcheck-suppress incorrectLogicOperator
                if (d2 > 0 && d2 < 1 && d3 > 0 && d3 < 1)
                {
                    // Simple collinear case, 1---2---3---4
                    // We can leave just two endpoints
                    return;
                }
                if (d2 <= 0)
                {
                    d2 = CalcSqDistance(x2, y2, x1, y1);
                }
                else if (d2 >= 1)
                {
                    d2 = CalcSqDistance(x2, y2, x4, y4);
                }
                else
                {
                    d2 = CalcSqDistance(x2, y2, x1 + d2*dx, y1 + d2*dy);
                }

                if (d3 <= 0)
                {
                    d3 = CalcSqDistance(x3, y3, x1, y1);
                }
                else if (d3 >= 1)
                {
                    d3 = CalcSqDistance(x3, y3, x4, y4);
                }
                else
                {
                    d3 = CalcSqDistance(x3, y3, x1 + d3*dx, y1 + d3*dy);
                }
            }
            if (d2 > d3)
            {
                if (d2 < m_distance_tolerance_square)
                {
                    px.append(x2);
                    py.append(y2);
                    return;
                }
            }
            else
            {
                if (d3 < m_distance_tolerance_square)
                {
                    px.append(x3);
                    py.append(y3);
                    return;
                }
            }
            break;
        }
        case 1:
        {
            // p1,p2,p4 are collinear, p3 is significant
            //----------------------
            if (d3 * d3 <= m_distance_tolerance_square * (dx*dx + dy*dy))
            {
                if (m_angle_tolerance < curve_angle_tolerance_epsilon)
                {
                    px.append(x23);
                    py.append(y23);
                    return;
                }

                // Angle Condition
                //----------------------
                double da1 = fabs(atan2(y4 - y3, x4 - x3) - atan2(y3 - y2, x3 - x2));
                if (da1 >= M_PI)
                {
                    da1 = 2*M_PI - da1;
                }

                if (da1 < m_angle_tolerance)
                {
                    px.append(x2);
                    py.append(y2);

                    px.append(x3);
                    py.append(y3);
                    return;
                }

                if (m_cusp_limit > 0.0 || m_cusp_limit < 0.0)
                {
                    if (da1 > m_cusp_limit)
                    {
                        px.append(x3);
                        py.append(y3);
                        return;
                    }
                }
            }
            break;
        }
        case 2:
        {
            // p1,p3,p4 are collinear, p2 is significant
            //----------------------
            if (d2 * d2 <= m_distance_tolerance_square * (dx*dx + dy*dy))
            {
                if (m_angle_tolerance < curve_angle_tolerance_epsilon)
                {
                    px.append(x23);
                    py.append(y23);
                    return;
                }

                // Angle Condition
                //----------------------
                double da1 = fabs(atan2(y3 - y2, x3 - x2) - atan2(y2 - y1, x2 - x1));
                if (da1 >= M_PI)
                {
                    da1 = 2*M_PI - da1;
                }

                if (da1 < m_angle_tolerance)
                {
                    px.append(x2);
                    py.append(y2);

                    px.append(x3);
                    py.append(y3);
                    return;
                }

                if (m_cusp_limit > 0.0 || m_cusp_limit < 0.0)
                {
                    if (da1 > m_cusp_limit)
                    {
                        px.append(x2);
                        py.append(y2);
                        return;
                    }
                }
            }
            break;
        }
        case 3:
        {
            // Regular case
            //-----------------
            if ((d2 + d3)*(d2 + d3) <= m_distance_tolerance_square * (dx*dx + dy*dy))
            {
                // If the curvature doesn't exceed the distance_tolerance value
                // we tend to finish subdivisions.
                //----------------------
                if (m_angle_tolerance < curve_angle_tolerance_epsilon)
                {
                    px.append(x23);
                    py.append(y23);
                    return;
                }

                // Angle & Cusp Condition
                //----------------------
                const double k   = atan2(y3 - y2, x3 - x2);
                double da1 = fabs(k - atan2(y2 - y1, x2 - x1));
                double da2 = fabs(atan2(y4 - y3, x4 - x3) - k);
                if (da1 >= M_PI)
                {
                    da1 = 2*M_PI - da1;
                }
                if (da2 >= M_PI)
                {
                    da2 = 2*M_PI - da2;
                }

                if (da1 + da2 < m_angle_tolerance)
                {
                    // Finally we can stop the recursion
                    //----------------------

                    px.append(x23);
                    py.append(y23);
                    return;
                }

                if (m_cusp_limit > 0.0 || m_cusp_limit < 0.0)
                {
                    if (da1 > m_cusp_limit)
                    {
                        px.append(x2);
                        py.append(y2);
                        return;
                    }

                    if (da2 > m_cusp_limit)
                    {
                        px.append(x3);
                        py.append(y3);
                        return;
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    // Continue subdivision
    //----------------------
    PointBezier_r(x1, y1, x12, y12, x123, y123, x1234, y1234, static_cast<qint16>(level + 1), px, py);
    PointBezier_r(x1234, y1234, x234, y234, x34, y34, x4, y4, static_cast<qint16>(level + 1), px, py);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CalcSqDistance calculate squared distance.
 * @param x1 х coordinate first point.
 * @param y1 у coordinate first point.
 * @param x2 х coordinate second point.
 * @param y2 у coordinate second point.
 * @return squared length.
 */
qreal VSpline::CalcSqDistance (qreal x1, qreal y1, qreal x2, qreal y2)
{
    qreal dx = x2 - x1;
    qreal dy = y2 - y1;
    return dx * dx + dy * dy;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CreateName create spline name.
 */
void VSpline::CreateName()
{
    _name = QString(spl_+"%1_%2").arg(this->GetP1().name(), this->GetP4().name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPath return QPainterPath for this spline.
 * @return path.
 */
QPainterPath VSpline::GetPath() const
{
    QPainterPath splinePath;
    QVector<QPointF> points = GetPoints ();
    if (points.count() >= 2)
    {
        for (qint32 i = 0; i < points.count()-1; ++i)
        {
            splinePath.moveTo(points[i]);
            splinePath.lineTo(points[i+1]);
        }
    }
    else
    {
        qDebug()<<"points.count() < 2"<<Q_FUNC_INFO;
    }
    return splinePath;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SplinePoints return list with spline points.
 * @param p1 first spline point.
 * @param p4 last spline point.
 * @param angle1 angle from first point to first control point.
 * @param angle2 angle from second point to second control point.
 * @param kAsm1 coefficient of length first control line.
 * @param kAsm2 coefficient of length second control line.
 * @param kCurve coefficient of curvature spline.
 * @return list with spline points.
 */
// cppcheck-suppress unusedFunction
QVector<QPointF> VSpline::SplinePoints(const QPointF &p1, const QPointF &p4, qreal angle1, qreal angle2, qreal kAsm1,
                                       qreal kAsm2, qreal kCurve)
{
    QLineF p1pX(p1.x(), p1.y(), p1.x() + 100, p1.y());
    p1pX.setAngle( angle1 );
    qreal L = 0, radius = 0, angle = 90;
    radius = QLineF(QPointF(p1.x(), p4.y()), p4).length();
    L = kCurve * radius * 4 / 3 * tan( angle * M_PI / 180.0 / 4 );
    QLineF p1p2(p1.x(), p1.y(), p1.x() + L * kAsm1, p1.y());
    p1p2.setAngle(angle1);
    QLineF p4p3(p4.x(), p4.y(), p4.x() + L * kAsm2, p4.y());
    p4p3.setAngle(angle2);
    QPointF p2 = p1p2.p2();
    QPointF p3 = p4p3.p2();
    return GetPoints(p1, p2, p3, p4);
}

//---------------------------------------------------------------------------------------------------------------------
VSpline &VSpline::operator =(const VSpline &spline)
{
    VGObject::operator=(spline);
    this->p1 = spline.GetP1 ();
    this->p2 = spline.GetP2 ();
    this->p3 = spline.GetP3 ();
    this->p4 = spline.GetP4 ();
    this->angle1 = spline.GetAngle1 ();
    this->angle2 = spline.GetAngle2 ();
    this->kAsm1 = spline.GetKasm1();
    this->kAsm2 = spline.GetKasm2();
    this->kCurve = spline.GetKcurve();
    return *this;
}
