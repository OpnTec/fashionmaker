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
#include "vellipticalarc_p.h"
#include "vspline.h"
#include "../ifc/ifcdef.h"
#include <QtCore/qmath.h>
#include <QLineF>
#include <QPainterPath>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VEllipticalArc default constructor.
 */
VEllipticalArc::VEllipticalArc()
    :VAbstractCurve(GOType::EllipticalArc), d (new VEllipticalArcData)
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
VEllipticalArc::VEllipticalArc (VPointF center, qreal radius1, qreal radius2,
            QString formulaRadius1, QString formulaRadius2, qreal f1, QString formulaF1, qreal f2,
            QString formulaF2, qreal rotationAngle, quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::EllipticalArc, idObject, mode),
      d (new VEllipticalArcData(center, radius1, radius2, formulaRadius1, formulaRadius2,
      f1, formulaF1, f2, formulaF2, rotationAngle))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(VPointF center, qreal radius1, qreal radius2, qreal f1, qreal f2, qreal rotationAngle)
    : VAbstractCurve(GOType::EllipticalArc, NULL_ID, Draw::Calculation),
      d (new VEllipticalArcData(center, radius1, radius2, f1, f2, rotationAngle))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(qreal length, QString formulaLength, VPointF center, qreal radius1, qreal radius2,
           QString formulaRadius1, QString formulaRadius2, qreal f1, QString formulaF1, qreal rotationAngle,
           quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::EllipticalArc, idObject, mode),
      d (new VEllipticalArcData(formulaLength, center, radius1, radius2, formulaRadius1, formulaRadius2,
           f1, formulaF1, rotationAngle))
{
    CreateName();
    FindF2(length);
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(qreal length, VPointF center, qreal radius1, qreal radius2, qreal f1,
           qreal rotationAngle)
    : VAbstractCurve(GOType::EllipticalArc, NULL_ID, Draw::Calculation),
      d (new VEllipticalArcData(center, radius1, radius2, f1, rotationAngle))
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
    : VAbstractCurve(arc), d (arc.d)
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
    VAbstractCurve::operator=(arc);
    d = arc.d;
    return *this;
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
    qreal length = 0;
    QPainterPath elArc;
    QVector<QPointF> points = GetPoints();
    elArc.moveTo(points.at(0));
    for (qint32 i = 1; i < points.count(); ++i)
    {
        elArc.lineTo(points.at(i));
    }
    length = elArc.length();

    if (d->isFlipped)
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
    return GetPoint(d->f1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2 return point associated with end angle.
 * @return point.
 */
QPointF VEllipticalArc::GetP2 () const
{
    return GetPoint(d->f2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoint return point associated with angle.
 * @return point.
 */
QPointF VEllipticalArc::GetPoint (qreal angle) const
{
    if (angle > 360 || angle < 0)
    {// Filter incorect value of angle
        QLineF dummy(0,0, 100, 0);
        dummy.setAngle(angle);
        angle = dummy.angle();
    }

    // p - point without rotation
    qreal x = qAbs((d->radius1 * d->radius2)/
            (qSqrt(d->radius2*d->radius2+d->radius1*d->radius1*qTan(M_PI*angle/180)*qTan(M_PI*angle/180))));
    qreal y = qAbs(qTan(M_PI*angle/180) * x);

    if (angle > 90 && angle <= 180)
    {
        x = -x;
    }
    else if (angle > 180 && angle < 270)
    {
        x = -x;
        y = -y;
    }
    else if (angle > 270)
    {
        y = -y;
    }
    else if (angle == 90)
    {
        x = 0;
        y = d->radius2;
    }
    else if (angle == 270)
    {
        x = 0;
        y = -d->radius2;
    }
    QPointF p ( GetCenter().x () + x, GetCenter().y () + y);
    // rotation of point
    QLineF line(GetCenter().toQPointF(), p);
    line.setAngle(line.angle() + GetRotationAngle());

    return line.p2();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AngleArc calculate arc angle.
 * @return angle in degree.
 */
qreal VEllipticalArc::AngleArc() const
{
    if ((qFuzzyIsNull(d->f1) && qFuzzyCompare(d->f2, 360)) ||
        (qFuzzyCompare(d->f1, 360) && qFuzzyIsNull(d->f2)))
    {
        return 360;
    }
    QLineF l1(0, 0, 100, 100);
    l1.setAngle(d->f1);
    QLineF l2(0, 0, 100, 100);
    l2.setAngle(d->f2);

    qreal ang = l1.angleTo(l2);

    if (d->isFlipped)
    {
        ang = 360 - ang;
    }

    return ang;
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
        sectionAngle.append(d->f1);
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
    d->isFlipped ? currentAngle = GetEndAngle() : currentAngle = GetStartAngle();
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
    if (length < this->GetLength()*0.02)
    {
        len = this->GetLength()*0.02;
    }
    else if ( length > this->GetLength()*0.98)
    {
        len = this->GetLength()*0.98;
    }
    else
    {
        len = length;
    }

    // the first arc has given length and startAngle just like in the origin arc
    arc1 = VEllipticalArc (len, QString().setNum(length), d->center, d->radius1, d->radius2,
                           d->formulaRadius1, d->formulaRadius2, d->f1, d->formulaF1, d->rotationAngle,
                           getIdObject(), getMode());
    // the second arc has startAngle just like endAngle of the first arc
    // and it has endAngle just like endAngle of the origin arc
    arc2 = VEllipticalArc (d->center, d->radius1, d->radius2, d->formulaRadius1, d->formulaRadius2,
                           arc1.GetEndAngle(), arc1.GetFormulaF2(), d->f2, d->formulaF2, d->rotationAngle,
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
/**
 * @brief setId keep id arc in data.
 * @param id id arc in data.
 */
void VEllipticalArc::setId(const quint32 &id)
{
    VAbstractCurve::setId(id);
    CreateName();
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
        d->isFlipped = true;
        gap = -gap;
    }
    while (length > MaxLength())
    {
        length = length - MaxLength();
    }

    // We need to calculate the second angle
    // first approximation of angle between start and end angles

    qreal endAngle = GetStartAngle() + gap;
    d->f2 = endAngle; // we need to set the end anngle, because we want to use GetLength()

    qreal lenBez = GetLength(); // first approximation of length

    qreal eps = 0.001 * qAbs(length);

    while (qAbs(lenBez - length) > eps)
    {
        gap = gap/2;
        if (lenBez > length)
        { // we selected too big end angle
            endAngle = endAngle - qAbs(gap);
        }
        else
        { // we selected too little end angle
            endAngle = endAngle + qAbs(gap);
        }
        // we need to set d->f2, because we use it when we calculate GetLength
        d->f2 = endAngle;
        lenBez = GetLength();
    }
    d->formulaF2 = QString().number(d->f2);
    d->formulaLength = QString().number(qApp->fromPixel(lenBez));
}

//---------------------------------------------------------------------------------------------------------------------
qreal VEllipticalArc::MaxLength() const
{
    const qreal h = ((d->radius1-d->radius2)*(d->radius1-d->radius2))/((d->radius1+d->radius2)*(d->radius1+d->radius2));
    const qreal ellipseLength =  M_PI*(d->radius1+d->radius2)*(1+3*h/(10+qSqrt(4-3*h)));
    return ellipseLength;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return start angle.
 * @return angle in degree.
 */
QString VEllipticalArc::GetFormulaF1() const
{
    return d->formulaF1;
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::SetFormulaF1(const QString &formula, qreal value)
{
    d->formulaF1 = formula;
    d->f1 = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return formula for start angle.
 * @return string with formula.
 */
qreal VEllipticalArc::GetStartAngle() const
{
    return d->f1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return end angle.
 * @return angle in degree.
 */
QString VEllipticalArc::GetFormulaF2() const
{
    return d->formulaF2;
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::SetFormulaF2(const QString &formula, qreal value)
{
    d->formulaF2 = formula;
    d->f2 = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return formula for end angle.
 * @return string with formula.
 */
qreal VEllipticalArc::GetEndAngle() const
{
    return d->f2;
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetCenter return center point.
 * @return center point.
 */
VPointF VEllipticalArc::GetCenter() const
{
    return d->center;
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::SetCenter(const VPointF &value)
{
    d->center = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VEllipticalArc::GetFormulaLength() const
{
    return d->formulaLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::SetFormulaLength(const QString &formula, qreal value)
{
    d->formulaLength = formula;
    FindF2(value);
}
