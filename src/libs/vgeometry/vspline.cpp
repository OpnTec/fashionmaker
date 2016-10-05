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

#include "vspline.h"

#include <QLineF>

#include "vabstractcurve.h"
#include "vspline_p.h"
#include "../vmisc/vmath.h"

class QPointF;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSpline default constructor
 */
VSpline::VSpline()
    :VAbstractCubicBezier(GOType::Spline), d(new VSplineData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSpline constructor.
 * @param spline spline from which the copy.
 */
VSpline::VSpline ( const VSpline & spline )
    :VAbstractCubicBezier(spline), d(spline.d)
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
VSpline::VSpline (const VPointF &p1, const VPointF &p4, qreal angle1, qreal angle2, qreal kAsm1, qreal kAsm2,
                  qreal kCurve, quint32 idObject, Draw mode)
    : VAbstractCubicBezier(GOType::Spline, idObject, mode),
      d(new VSplineData(p1, p4, angle1, angle2, kAsm1, kAsm2, kCurve))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSpline constructor.
 * @param p1 first point spline.
 * @param p2 first control point.
 * @param p3 second control point.
 * @param p4 second point spline.
 */
VSpline::VSpline (const VPointF &p1, const QPointF &p2, const QPointF &p3, const VPointF &p4, quint32 idObject,
                  Draw mode)
    :VAbstractCubicBezier(GOType::Spline, idObject, mode), d(new VSplineData(p1, p2, p3, p4))
{
    CreateName();
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSpline constructor
 * @param p1 first point spline.
 * @param p4 first control point.
 * @param angle1 angle from first point to first control point.
 * @param angle1Formula formula angle from first point to first control point.
 * @param angle2 angle from second point to second control point.
 * @param angle2Formula formula angle from second point to second control point.
 * @param c1Length length from first point to first control point.
 * @param c1LengthFormula formula length from first point to first control point.
 * @param c2Length length from second point to first control point.
 * @param c2LengthFormula formula length from second point to first control point.
 */
VSpline::VSpline(const VPointF &p1, const VPointF &p4, qreal angle1, const QString &angle1Formula, qreal angle2,
                 const QString &angle2Formula, qreal c1Length, const QString &c1LengthFormula, qreal c2Length,
                 const QString &c2LengthFormula, quint32 idObject, Draw mode)
    : VAbstractCubicBezier(GOType::Spline, idObject, mode),
      d(new VSplineData(p1, p4, angle1, angle1Formula, angle2, angle2Formula, c1Length, c1LengthFormula, c2Length,
                        c2LengthFormula))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VSpline VSpline::Rotate(const QPointF &originPoint, qreal degrees, const QString &prefix) const
{
    const VPointF p1 = GetP1().Rotate(originPoint, degrees);
    const VPointF p4 = GetP4().Rotate(originPoint, degrees);

    const QPointF p2 = VPointF::RotatePF(originPoint, GetP2(), degrees);
    const QPointF p3 = VPointF::RotatePF(originPoint, GetP3(), degrees);

    VSpline spl(p1, p2, p3, p4);
    spl.setName(name() + prefix);
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
VSpline VSpline::Flip(const QLineF &axis, const QString &prefix) const
{
    const VPointF p1 = GetP1().Flip(axis);
    const VPointF p4 = GetP4().Flip(axis);

    const QPointF p2 = VPointF::FlipPF(axis, GetP2());
    const QPointF p3 = VPointF::FlipPF(axis, GetP3());

    VSpline spl(p1, p2, p3, p4);
    spl.setName(name() + prefix);
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
VSpline VSpline::Move(qreal length, qreal angle, const QString &prefix) const
{
    const VPointF p1 = GetP1().Move(length, angle);
    const VPointF p4 = GetP4().Move(length, angle);

    const QPointF p2 = VPointF::MovePF(GetP2(), length, angle);
    const QPointF p3 = VPointF::MovePF(GetP3(), length, angle);

    VSpline spl(p1, p2, p3, p4);
    spl.setName(name() + prefix);
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
VSpline::~VSpline()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLength return length of spline.
 * @return length.
 */
qreal VSpline::GetLength () const
{
    return LengthBezier ( GetP1(), GetP2(), GetP3(), GetP4());
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VSpline::CutSpline(qreal length, VSpline &spl1, VSpline &spl2) const
{
    QPointF spl1p2;
    QPointF spl1p3;
    QPointF spl2p2;
    QPointF spl2p3;
    const QPointF cutPoint = CutSpline (length, spl1p2, spl1p3, spl2p2, spl2p3 );

    spl1 = VSpline(GetP1(), spl1p2, spl1p3, cutPoint);
    spl2 = VSpline(cutPoint, spl2p2, spl2p3, GetP4());
    return cutPoint;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoints return list with spline points.
 * @return list of points.
 */
QVector<QPointF> VSpline::GetPoints () const
{
    return GetCubicBezierPoints(GetP1(), GetP2(), GetP3(), GetP4());
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
    L = kCurve * radius * 4 / 3 * tan( angle * M_PI_4 / 180.0 );
    QLineF p1p2(p1.x(), p1.y(), p1.x() + L * kAsm1, p1.y());
    p1p2.setAngle(angle1);
    QLineF p4p3(p4.x(), p4.y(), p4.x() + L * kAsm2, p4.y());
    p4p3.setAngle(angle2);
    QPointF p2 = p1p2.p2();
    QPointF p3 = p4p3.p2();
    return GetCubicBezierPoints(p1, p2, p3, p4);
}

//---------------------------------------------------------------------------------------------------------------------
VSpline &VSpline::operator =(const VSpline &spline)
{
    if ( &spline == this )
    {
        return *this;
    }
    VAbstractCubicBezier::operator=(spline);
    d = spline.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP1 return first spline point.
 * @return first point.
 */
VPointF VSpline::GetP1() const
{
    return d->p1;
}

//---------------------------------------------------------------------------------------------------------------------
void VSpline::SetP1(const VPointF &p)
{
    d->p1 = p;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2 return first control point.
 * @return first control point.
 */
VPointF VSpline::GetP2() const
{
    QLineF p1p2(d->p1.x(), d->p1.y(), d->p1.x() + d->c1Length, d->p1.y());
    p1p2.setAngle(d->angle1);
    return VPointF(p1p2.p2());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP3 return second control point.
 * @return second control point.
 */
VPointF VSpline::GetP3() const
{
    QLineF p4p3(d->p4.x(), d->p4.y(), d->p4.x() + d->c2Length, d->p4.y());
    p4p3.setAngle(d->angle2);
    return VPointF(p4p3.p2());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP4 return last spline point.
 * @return остання точка сплайну.
 */
VPointF VSpline::GetP4() const
{
    return d->p4;
}

//---------------------------------------------------------------------------------------------------------------------
void VSpline::SetP4(const VPointF &p)
{
    d->p4 = p;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAngle1 return first angle control line.
 * @return angle.
 */
qreal VSpline::GetStartAngle() const
{
    return d->angle1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAngle2 return second angle control line.
 * @return angle.
 */
qreal VSpline::GetEndAngle() const
{
    return d->angle2;
}

//---------------------------------------------------------------------------------------------------------------------
QString VSpline::GetStartAngleFormula() const
{
    return d->angle1F;
}

//---------------------------------------------------------------------------------------------------------------------
QString VSpline::GetEndAngleFormula() const
{
    return d->angle2F;
}

//---------------------------------------------------------------------------------------------------------------------
void VSpline::SetStartAngle(qreal angle, const QString &formula)
{
    d->angle1 = angle;
    d->angle1F = formula;
}

//---------------------------------------------------------------------------------------------------------------------
void VSpline::SetEndAngle(qreal angle, const QString &formula)
{
    d->angle2 = angle;
    d->angle2F = formula;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSpline::GetC1Length() const
{
    return d->c1Length;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSpline::GetC2Length() const
{
    return d->c2Length;
}

//---------------------------------------------------------------------------------------------------------------------
QString VSpline::GetC1LengthFormula() const
{
    return d->c1LengthF;
}

//---------------------------------------------------------------------------------------------------------------------
QString VSpline::GetC2LengthFormula() const
{
    return d->c2LengthF;
}

//---------------------------------------------------------------------------------------------------------------------
void VSpline::SetC1Length(qreal length, const QString &formula)
{
    d->c1Length = length;
    d->c1LengthF = formula;
}

//---------------------------------------------------------------------------------------------------------------------
void VSpline::SetC2Length(qreal length, const QString &formula)
{
    d->c2Length = length;
    d->c2LengthF = formula;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKasm1 return coefficient of length first control line.
 * @return coefficient.
 */
qreal VSpline::GetKasm1() const
{
    return QLineF(d->p1, GetP2()).length() / VSplineData::GetL(d->p1, d->p4,
                                                                           d->kCurve);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKasm2 return coefficient of length second control line.
 * @return coefficient.
 */
qreal VSpline::GetKasm2() const
{
    return QLineF(d->p4, GetP3()).length() / VSplineData::GetL(d->p1, d->p4,
                                                                           d->kCurve);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKcurve return coefficient of curvature spline.
 * @return coefficient
 */
qreal VSpline::GetKcurve() const
{
    return d->kCurve;
}

//---------------------------------------------------------------------------------------------------------------------
int VSpline::Sign(long double ld)
{
    if (qAbs(ld)<0.00000000001)
    {
        return 0;
    }
    return (ld>0) ? 1 : -1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Cubic Cubic equation solution. Real coefficients case.
 *
 * This method use method Vieta-Cardano for eval cubic equations.
 * Cubic equation write in form x3+a*x2+b*x+c=0.
 *
 * Output:
 * 3 real roots -> then x is filled with them;
 * 1 real + 2 complex -> x[0] is real, x[1] is real part of complex roots, x[2] - non-negative imaginary part.
 *
 * @param x solution array (size 3).
 * @param a coefficient
 * @param b coefficient
 * @param c coefficient
 * @return 3 - 3 real roots;
 *         1 - 1 real root + 2 complex;
 *         2 - 1 real root + complex roots imaginary part is zero (i.e. 2 real roots).
 */
qint32 VSpline::Cubic(QVector<qreal> &x, qreal a, qreal b, qreal c)
{
    //To find cubic equation roots in the case of real coefficients, calculated at the beginning
    const qreal q = (pow(a, 2) - 3*b)/9.;
    const qreal r = (2*pow(a, 3) - 9*a*b + 27.*c)/54.;
    if (pow(r, 2) < pow(q, 3))
    { // equation has three real roots, use formula Vieta
        const qreal t = acos(r/sqrt(pow(q, 3)))/3.;
        x.insert(0, -2.*sqrt(q)*cos(t)-a/3);
        x.insert(1, -2.*sqrt(q)*cos(t + (2*M_2PI/3.)) - a/3.);
        x.insert(2, -2.*sqrt(q)*cos(t - (2*M_2PI/3.)) - a/3.);
        return(3);
    }
    else
    { // 1 real root + 2 complex
        //Formula Cardano
        const qreal aa = -Sign(r)*pow(fabs(r)+sqrt(pow(r, 2)-pow(q, 3)), 1./3.);
        const qreal bb = Sign(aa) == 0 ? 0 : q/aa;

        x.insert(0, aa+bb-a/3.); // Real root
        x.insert(1, (-0.5)*(aa+bb)-a/3.); //Complex root
        x.insert(2, (sqrt(3.)*0.5)*fabs(aa-bb)); // Complex root
        if (qFuzzyIsNull(x.at(2)))
        {
            return(2);
        }
        return(1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<qreal> VSpline::CalcT (qreal curveCoord1, qreal curveCoord2, qreal curveCoord3,
                               qreal curveCoord4, qreal pointCoord) const
{
    const qreal a = -curveCoord1 + 3*curveCoord2 - 3*curveCoord3 + curveCoord4;
    const qreal b = 3*curveCoord1 - 6*curveCoord2 + 3*curveCoord3;
    const qreal c = -3*curveCoord1 + 3*curveCoord2;
    const qreal d = -pointCoord + curveCoord1;

    QVector<qreal> t = QVector<qreal>(3, -1);
    Cubic(t, b/a, c/a, d/a);

    QVector<qreal> retT;
    for (int i=0; i < t.size(); ++i)
    {
        if ( t.at(i) >= 0 && t.at(i) <= 1 )
        {
            retT.append(t.at(i));
        }
    }

    return retT;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSpline::ParamT calculate t coeffient that reprezent point on curve.
 *
 * Each point that belongs to Cubic Bézier curve can be shown by coefficient in interval [0; 1].
 *
 * @param pBt point on curve
 * @return t coeffient that reprezent this point on curve. Return -1 if point doesn't belongs to curve.
 */
qreal VSpline::ParamT (const QPointF &pBt) const
{
    QVector<qreal> ts;
    // Calculate t coefficient for each axis
    ts += CalcT (GetP1().x(), GetP2().x(), GetP3().x(), GetP4().x(), pBt.x());
    ts += CalcT (GetP1().y(), GetP2().y(), GetP3().y(), GetP4().y(), pBt.y());

    if (ts.isEmpty())
    {
        return -1; // We don't have candidates
    }

    qreal tx = -1;
    qreal eps = 3; // Error calculation

    // In morst case we will have 6 result in interval [0; 1].
    // Here we try find closest to our point.
    for (int i=0; i< ts.size(); ++i)
    {
        const qreal t = ts.at(i);
        const QPointF p0 = GetP1();
        const QPointF p1 = GetP2();
        const QPointF p2 = GetP3();
        const QPointF p3 = GetP4();
        //The explicit form of the Cubic Bézier curve
        const qreal pointX = pow(1-t, 3)*p0.x() + 3*pow(1-t, 2)*t*p1.x() + 3*(1-t)*pow(t, 2)*p2.x() + pow(t, 3)*p3.x();
        const qreal pointY = pow(1-t, 3)*p0.y() + 3*pow(1-t, 2)*t*p1.y() + 3*(1-t)*pow(t, 2)*p2.y() + pow(t, 3)*p3.y();

        const QLineF line(pBt, QPointF(pointX, pointY));
        if (line.length() <= eps)
        {
            tx = t;
            eps = line.length(); //Next point should be even closest
        }
    }

    return tx;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VSpline::GetControlPoint1() const
{
    return GetP2 ();
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VSpline::GetControlPoint2() const
{
    return GetP3 ();
}
