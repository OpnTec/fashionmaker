/************************************************************************
 **
 **  @file   vellipticalarc.cpp
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   February 1, 2016
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

#include "vellipticalarc.h"

#include <QLineF>
#include <QPoint>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"
#include "../ifc/ifcdef.h"
#include "../vmisc/vabstractapplication.h"
#include "vabstractcurve.h"
#include "vellipticalarc_p.h"
#include "vspline.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VEllipticalArc default constructor.
 */
VEllipticalArc::VEllipticalArc()
    : VAbstractArc(GOType::EllipticalArc), d (new VEllipticalArcData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VEllipticalArc constructor.
 * @param center center point.
 * @param radius1 arc major radius.
 * @param radius2 arc minor radius.
 * @param f1 start angle (degree).
 * @param f2 end angle (degree).
 */
VEllipticalArc::VEllipticalArc (const VPointF &center, qreal radius1, qreal radius2, const QString &formulaRadius1,
                                const QString &formulaRadius2, qreal f1, const QString &formulaF1, qreal f2,
                                const QString &formulaF2, qreal rotationAngle, quint32 idObject, Draw mode)
    : VAbstractArc(GOType::EllipticalArc, center, f1, formulaF1, f2, formulaF2, idObject, mode),
      d (new VEllipticalArcData(radius1, radius2, formulaRadius1, formulaRadius2, rotationAngle))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(const VPointF &center, qreal radius1, qreal radius2, qreal f1, qreal f2,
                               qreal rotationAngle)
    : VAbstractArc(GOType::EllipticalArc, center, f1, f2, NULL_ID, Draw::Calculation),
      d (new VEllipticalArcData(radius1, radius2, rotationAngle))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(qreal length, const QString &formulaLength, const VPointF &center, qreal radius1,
                               qreal radius2, const QString &formulaRadius1, const QString &formulaRadius2, qreal f1,
                               const QString &formulaF1, qreal rotationAngle, quint32 idObject, Draw mode)
    : VAbstractArc(GOType::EllipticalArc, formulaLength, center, f1, formulaF1, idObject, mode),
      d (new VEllipticalArcData(radius1, radius2, formulaRadius1, formulaRadius2, rotationAngle))
{
    CreateName();
    FindF2(length);
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(qreal length, const VPointF &center, qreal radius1, qreal radius2, qreal f1,
                               qreal rotationAngle)
    : VAbstractArc(GOType::EllipticalArc, center, f1, NULL_ID, Draw::Calculation),
      d (new VEllipticalArcData(radius1, radius2, rotationAngle))
{
    CreateName();
    FindF2(length);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VEllipticalArc copy constructor
 * @param arc arc
 */
VEllipticalArc::VEllipticalArc(const VEllipticalArc &arc)
    : VAbstractArc(arc), d (arc.d)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator
 * @param arc arc
 * @return arc
 */
VEllipticalArc &VEllipticalArc::operator =(const VEllipticalArc &arc)
{
    if ( &arc == this )
    {
        return *this;
    }
    VAbstractArc::operator=(arc);
    d = arc.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc VEllipticalArc::Rotate(const QPointF &originPoint, qreal degrees, const QString &prefix) const
{
    const VPointF center = GetCenter().Rotate(originPoint, degrees);
    const QPointF p1 = VPointF::RotatePF(originPoint, GetP1(), degrees);
    const QPointF p2 = VPointF::RotatePF(originPoint, GetP2(), degrees);
    const qreal f1 = QLineF(center, p1).angle() - GetRotationAngle();
    const qreal f2 = QLineF(center, p2).angle() - GetRotationAngle();
    VEllipticalArc elArc(center, GetRadius1(), GetRadius2(), f1, f2, GetRotationAngle());
    elArc.setName(name() + prefix);
    return elArc;
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::~VEllipticalArc()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLength return arc length.
 * @return length.
 */
qreal VEllipticalArc::GetLength() const
{
    qreal length = PathLength(GetPoints());

    if (IsFlipped())
    {
        length = length * -1;
    }

    return length;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP1 return point associated with start angle.
 * @return point.
 */
QPointF VEllipticalArc::GetP1() const
{
    return GetPoint(GetStartAngle());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2 return point associated with end angle.
 * @return point.
 */
QPointF VEllipticalArc::GetP2 () const
{
    return GetPoint(GetEndAngle());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoint return point associated with angle.
 * @return point.
 */
QPointF VEllipticalArc::GetPoint (qreal angle) const
{
    // Original idea http://alex-black.ru/article.php?content=109#head_3
    if (angle > 360 || angle < 0)
    {// Filter incorect value of angle
        QLineF dummy(0, 0, 100, 0);
        dummy.setAngle(angle);
        angle = dummy.angle();
    }

    // p - point without rotation
    qreal x = 0;
    qreal y = 0;

    qreal angleRad = qDegreesToRadians(angle);
    const int n = GetQuadransRad(angleRad);
    if (VFuzzyComparePossibleNulls(angleRad, 0) || VFuzzyComparePossibleNulls(angleRad, M_2PI) ||
        VFuzzyComparePossibleNulls(angleRad, -M_2PI))
    { // 0 (360, -360) degress
        x = d->radius1;
        y = 0;
    }
    else if (VFuzzyComparePossibleNulls(angleRad, M_PI_2) || VFuzzyComparePossibleNulls(angleRad, -3 * M_PI_2))
    { // 90 (-270) degress
        x = 0;
        y = d->radius2;
    }
    else if (VFuzzyComparePossibleNulls(angleRad, M_PI) || VFuzzyComparePossibleNulls(angleRad, -M_PI))
    { // 180 (-180) degress
        x = -d->radius1;
        y = 0;
    }
    else if (VFuzzyComparePossibleNulls(angleRad, 3 * M_PI_2) || VFuzzyComparePossibleNulls(angleRad, -M_PI_2))
    { // 270 (-90) degress
        x = 0;
        y = -d->radius2;
    }
    else
    { // cases between
        const qreal r1Pow = qPow(d->radius1, 2);
        const qreal r2Pow = qPow(d->radius2, 2);
        const qreal angleTan = qTan(angleRad);
        const qreal angleTan2 = qPow(angleTan, 2);
        x = qSqrt((r1Pow * r2Pow) / (r1Pow * angleTan2 + r2Pow));
        y = angleTan * x;
    }

    switch (n)
    {
        case 1:
            x = +x;
            y = +y;
            break;
        case 2:
            x = -x;
            y = +y;
            break;
        case 3:
            x = -x;
            y = -y;
            break;
        case 4:
            x = +x;
            y = -y;
            break;
        default:
            break;
    }

    QPointF p (GetCenter().x() + x, GetCenter().y() + y);
    // rotation of point
    QLineF line(GetCenter(), p);
    line.setAngle(line.angle() + GetRotationAngle());

    return line.p2();
}

//---------------------------------------------------------------------------------------------------------------------
int VEllipticalArc::GetQuadransRad(qreal &rad)
{
    if (rad > M_PI)
    {
        rad = rad - M_2PI;
    }

    if (rad < -M_PI)
    {
        rad = rad + M_2PI;
    }

    int n = 0;
    if (rad > 0)
    {
        if (rad >= 0 && rad <= M_PI_2)
        {
            n = 1;
            rad = -rad;
        }
        else if (rad > M_PI_2 && rad <= M_PI)
        {
            n = 2;
            rad = M_PI+rad;
        }
    }
    else
    {
        if (rad <= 0 && rad >= -M_PI_2)
        {
            n = 4;
        }
        else if (rad < -M_PI_2 && rad >= -M_PI)
        {
            n = 3;
            rad = M_PI-rad;
        }
    }
    return n;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAngles return list of angles needed for drawing arc.
 * @return list of angles
 */
QVector<qreal> VEllipticalArc::GetAngles() const
{
    QVector<qreal> sectionAngle;
    qreal angle = AngleArc();

    if (qFuzzyIsNull(angle))
    {// Return the array that includes one angle
        sectionAngle.append(GetStartAngle());
        return sectionAngle;
    }

    if (angle > 360 || angle < 0)
    {// Filter incorect value of angle
        QLineF dummy(0,0, 100, 0);
        dummy.setAngle(angle);
        angle = dummy.angle();
    }

    const qreal angleInterpolation = 45; //degree
    const int sections = qFloor(angle / angleInterpolation);
    for (int i = 0; i < sections; ++i)
    {
        sectionAngle.append(angleInterpolation);
    }

    const qreal tail = angle - sections * angleInterpolation;
    if (tail > 0)
    {
        sectionAngle.append(tail);
    }
    return sectionAngle;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoints return list of points needed for drawing arc.
 * @return list of points
 */
QVector<QPointF> VEllipticalArc::GetPoints() const
{
    QVector<QPointF> points;
    QVector<qreal> sectionAngle = GetAngles();

    qreal currentAngle;
    IsFlipped() ? currentAngle = GetEndAngle() : currentAngle = GetStartAngle();
    for (int i = 0; i < sectionAngle.size(); ++i)
    {
        QPointF startPoint = GetPoint(currentAngle);
        QPointF ellipsePoint2 = GetPoint(currentAngle + sectionAngle.at(i)/3);
        QPointF ellipsePoint3 = GetPoint(currentAngle + 2*sectionAngle.at(i)/3);
        QPointF lastPoint = GetPoint(currentAngle + sectionAngle.at(i));
        // four points that are on ellipse

        QPointF bezierPoint1 = ( -5*startPoint + 18*ellipsePoint2 -9*ellipsePoint3 + 2*lastPoint )/6;
        QPointF bezierPoint2 = ( 2*startPoint - 9*ellipsePoint2 + 18*ellipsePoint3 - 5*lastPoint )/6;

        VSpline spl(VPointF(startPoint), bezierPoint1, bezierPoint2, VPointF(lastPoint), 1.0);

        QVector<QPointF> splPoints = spl.GetPoints();

        if (not splPoints.isEmpty() && i != sectionAngle.size() - 1)
        {
            splPoints.removeLast();
        }
        points << splPoints;
        currentAngle += sectionAngle.at(i);
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CutArc cut arc into two arcs.
 * @param length length first arc.
 * @param arc1 first arc.
 * @param arc2 second arc.
 * @return point cutting
 */
QPointF VEllipticalArc::CutArc(const qreal &length, VEllipticalArc &arc1, VEllipticalArc &arc2) const
{
    //Always need return two arcs, so we must correct wrong length.
    qreal len = 0;
    const qreal minLength = ToPixel(1, Unit::Mm);
    const qreal fullLength = GetLength();

    if (fullLength <= minLength)
    {
        arc1 = VEllipticalArc();
        arc2 = VEllipticalArc();
        return QPointF();
    }

    const qreal maxLength = fullLength - minLength;

    if (length < minLength)
    {
        len = minLength;
    }
    else if (length > maxLength)
    {
        len = maxLength;
    }
    else
    {
        len = length;
    }

    // the first arc has given length and startAngle just like in the origin arc
    arc1 = VEllipticalArc (len, QString().setNum(length), GetCenter(), d->radius1, d->radius2,
                           d->formulaRadius1, d->formulaRadius2, GetStartAngle(), GetFormulaF1(), d->rotationAngle,
                           getIdObject(), getMode());
    // the second arc has startAngle just like endAngle of the first arc
    // and it has endAngle just like endAngle of the origin arc
    arc2 = VEllipticalArc (GetCenter(), d->radius1, d->radius2, d->formulaRadius1, d->formulaRadius2,
                           arc1.GetEndAngle(), arc1.GetFormulaF2(), GetEndAngle(), GetFormulaF2(), d->rotationAngle,
                           getIdObject(), getMode());
    return arc1.GetP1();
}


//---------------------------------------------------------------------------------------------------------------------
QPointF VEllipticalArc::CutArc(const qreal &length) const
{
    VEllipticalArc arc1;
    VEllipticalArc arc2;
    return this->CutArc(length, arc1, arc2);
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::CreateName()
{
    QString name = EARC_ + QString("%1").arg(this->GetCenter().name());

    if (VAbstractCurve::id() != NULL_ID)
    {
        name += QString("_%1").arg(VAbstractCurve::id());
    }

    if (GetDuplicate() > 0)
    {
        name += QString("_%1").arg(GetDuplicate());
    }

    setName(name);
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::FindF2(qreal length)
{
    qreal gap = 180;
    if (length < 0)
    {
        SetFlipped(true);
        gap = -gap;
    }
    while (length > MaxLength())
    {
        length = length - MaxLength();
    }

    // We need to calculate the second angle
    // first approximation of angle between start and end angles

    qreal endAngle = GetStartAngle() + gap;

    // we need to set the end angle, because we want to use GetLength()
    SetFormulaF2(QString::number(endAngle), endAngle);

    qreal lenBez = GetLength(); // first approximation of length

    const qreal eps = ToPixel(0.1, Unit::Mm);

    while (qAbs(lenBez - length) > eps)
    {
        gap = gap/2;
        if (gap < 0.0001)
        {
            break;
        }
        if (lenBez > length)
        { // we selected too big end angle
            endAngle = endAngle - qAbs(gap);
        }
        else
        { // we selected too little end angle
            endAngle = endAngle + qAbs(gap);
        }
        // we need to set d->f2, because we use it when we calculate GetLength
        SetFormulaF2(QString::number(endAngle), endAngle);
        lenBez = GetLength();
    }
    SetFormulaF2(QString::number(endAngle), endAngle);
    SetFormulaLength(QString::number(qApp->fromPixel(lenBez)));
}

//---------------------------------------------------------------------------------------------------------------------
qreal VEllipticalArc::MaxLength() const
{
    const qreal h = qPow(d->radius1 - d->radius2, 2) / qPow(d->radius1 + d->radius2, 2);
    const qreal ellipseLength = M_PI * (d->radius1 + d->radius2) * (1+3*h/(10+qSqrt(4-3*h)));
    return ellipseLength;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return arc major radius.
 * @return radius.
 */
QString VEllipticalArc::GetFormulaRadius1() const
{
    return d->formulaRadius1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return arc minor radius.
 * @return radius.
 */
QString VEllipticalArc::GetFormulaRadius2() const
{
    return d->formulaRadius2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRotationAngle return rotation angle.
 * @return rotationAngle.
 */
qreal VEllipticalArc::GetRotationAngle() const
{
    return d->rotationAngle;
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::SetFormulaRadius1(const QString &formula, qreal value)
{
    d->formulaRadius1 = formula;
    d->radius1 = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::SetFormulaRadius2(const QString &formula, qreal value)
{
    d->formulaRadius2 = formula;
    d->radius2 = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return formula for major radius.
 * @return string with formula.
 */
qreal VEllipticalArc::GetRadius1() const
{
    return d->radius1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return formula for minor radius.
 * @return string with formula.
 */
qreal VEllipticalArc::GetRadius2() const
{
    return d->radius2;
}
