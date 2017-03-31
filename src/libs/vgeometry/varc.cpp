/************************************************************************
 **
 **  @file   varc.cpp
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

#include "varc.h"

#include <QLineF>
#include <QPointF>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"
#include "../ifc/ifcdef.h"
#include "vabstractcurve.h"
#include "varc_p.h"
#include "vspline.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VArc default constructor.
 */
VArc::VArc ()
    : VAbstractArc(GOType::Arc),
      d (new VArcData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VArc constructor.
 * @param center center point.
 * @param radius arc radius.
 * @param f1 start angle (degree).
 * @param f2 end angle (degree).
 */
VArc::VArc (const VPointF &center, qreal radius, const QString &formulaRadius, qreal f1, const QString &formulaF1,
            qreal f2, const QString &formulaF2, quint32 idObject, Draw mode)
    : VAbstractArc(GOType::Arc, center, f1, formulaF1, f2, formulaF2, idObject, mode),
      d (new VArcData(radius, formulaRadius))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VArc::VArc(const VPointF &center, qreal radius, qreal f1, qreal f2)
    : VAbstractArc(GOType::Arc, center, f1, f2, NULL_ID, Draw::Calculation),
      d (new VArcData(radius))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VArc::VArc(qreal length, const QString &formulaLength, const VPointF &center, qreal radius,
           const QString &formulaRadius, qreal f1, const QString &formulaF1, quint32 idObject, Draw mode)
    : VAbstractArc(GOType::Arc, formulaLength, center, f1, formulaF1, idObject, mode),
      d (new VArcData(radius, formulaRadius))
{
    CreateName();
    FindF2(length);
}

//---------------------------------------------------------------------------------------------------------------------
VArc::VArc(qreal length, const VPointF &center, qreal radius, qreal f1)
    : VAbstractArc(GOType::Arc, center, f1, NULL_ID, Draw::Calculation),
      d (new VArcData(radius))
{
    CreateName();
    FindF2(length);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VArc copy constructor
 * @param arc arc
 */
VArc::VArc(const VArc &arc)
    : VAbstractArc(arc), d (arc.d)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator
 * @param arc arc
 * @return arc
 */
VArc &VArc::operator =(const VArc &arc)
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
VArc VArc::Rotate(const QPointF &originPoint, qreal degrees, const QString &prefix) const
{
    const VPointF center = GetCenter().Rotate(originPoint, degrees);

    const QPointF p1 = VPointF::RotatePF(originPoint, GetP1(), degrees);
    const QPointF p2 = VPointF::RotatePF(originPoint, GetP2(), degrees);

    VArc arc(center, GetRadius(), QLineF(static_cast<QPointF>(center), p1).angle(),
             QLineF(static_cast<QPointF>(center), p2).angle());
    arc.setName(name() + prefix);
    return arc;
}

//---------------------------------------------------------------------------------------------------------------------
VArc VArc::Flip(const QLineF &axis, const QString &prefix) const
{
    const VPointF center = GetCenter().Flip(axis);

    const QPointF p1 = VPointF::FlipPF(axis, GetP1());
    const QPointF p2 = VPointF::FlipPF(axis, GetP2());

    VArc arc(center, GetRadius(), QLineF(static_cast<QPointF>(center), p1).angle(),
             QLineF(static_cast<QPointF>(center), p2).angle());
    arc.setName(name() + prefix);
    arc.SetFlipped(true);
    return arc;
}

//---------------------------------------------------------------------------------------------------------------------
VArc VArc::Move(qreal length, qreal angle, const QString &prefix) const
{
    const VPointF center = GetCenter().Move(length, angle);

    const QPointF p1 = VPointF::MovePF(GetP1(), length, angle);
    const QPointF p2 = VPointF::MovePF(GetP2(), length, angle);

    VArc arc(center, GetRadius(), QLineF(static_cast<QPointF>(center), p1).angle(),
             QLineF(static_cast<QPointF>(center), p2).angle());
    arc.setName(name() + prefix);
    return arc;
}

//---------------------------------------------------------------------------------------------------------------------
VArc::~VArc()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLength return arc length.
 * @return length.
 */
qreal VArc::GetLength() const
{
    qreal length = d->radius * qDegreesToRadians(AngleArc());
    if (IsFlipped())
    {
        length *= -1;
    }

    return length;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP1 return point associated with start angle.
 * @return point.
 */
QPointF VArc::GetP1() const
{
    QPointF p1 ( GetCenter().x () + d->radius, GetCenter().y () );
    QLineF centerP1(static_cast<QPointF>(GetCenter()), p1);
    centerP1.setAngle(GetStartAngle());
    return centerP1.p2();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2 return point associated with end angle.
 * @return точку point.
 */
QPointF VArc::GetP2 () const
{
    QPointF p2 ( GetCenter().x () + d->radius, GetCenter().y () );
    QLineF centerP2(static_cast<QPointF>(GetCenter()), p2);
    centerP2.setAngle(GetEndAngle());
    return centerP2.p2();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoints return list of points needed for drawing arc.
 * @return list of points
 */
QVector<QPointF> VArc::GetPoints() const
{
    QVector<QPointF> points;
    QVector<qreal> sectionAngle;

    QPointF pStart;
    IsFlipped() ? pStart = GetP2() : pStart = GetP1();

    {
        qreal angle = AngleArc();

        if (qFuzzyIsNull(angle))
        {
            points.append(pStart);
            return points;
        }

        if (angle > 360 || angle < 0)
        {// Filter incorect value
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
    }

    for (int i = 0; i < sectionAngle.size(); ++i)
    {
        const qreal lDistance = GetRadius() * 4.0/3.0 * qTan(qDegreesToRadians(sectionAngle.at(i)) * 0.25);

        const QPointF center = static_cast<QPointF>(GetCenter());

        QLineF lineP1P2(pStart, center);
        lineP1P2.setAngle(lineP1P2.angle() - 90.0);
        lineP1P2.setLength(lDistance);

        QLineF lineP4P3(center, pStart);
        lineP4P3.setAngle(lineP4P3.angle() + sectionAngle.at(i));
        lineP4P3.setLength(GetRadius());//in case of computing error
        lineP4P3 = QLineF(lineP4P3.p2(), center);
        lineP4P3.setAngle(lineP4P3.angle() + 90.0);
        lineP4P3.setLength(lDistance);

        VSpline spl(VPointF(pStart), lineP1P2.p2(), lineP4P3.p2(), VPointF(lineP4P3.p1()), 1.0);
        QVector<QPointF> splPoints = spl.GetPoints();
        if (not splPoints.isEmpty() && i != sectionAngle.size() - 1)
        {
#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
            splPoints.remove(splPoints.size() - 1);
#else
            splPoints.removeLast();
#endif
        }

        points << splPoints;
        pStart = lineP4P3.p1();
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
QPointF VArc::CutArc(const qreal &length, VArc &arc1, VArc &arc2) const
{
    //Always need return two arcs, so we must correct wrong length.
    qreal len = 0;
    const qreal minLength = ToPixel(1, Unit::Mm);
    const qreal fullLength = GetLength();

    if (fullLength <= minLength)
    {
        arc1 = VArc();
        arc2 = VArc();
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

    qreal n = qRadiansToDegrees(len/d->radius); // n - is angle in degrees

    QLineF line(static_cast<QPointF>(GetCenter()), GetP1());
    line.setAngle(line.angle()+n);

    arc1 = VArc (GetCenter(), d->radius, d->formulaRadius, GetStartAngle(), GetFormulaF1(), line.angle(),
                 QString().setNum(line.angle()), getIdObject(), getMode());

    arc2 = VArc (GetCenter(), d->radius, d->formulaRadius, line.angle(), QString().setNum(line.angle()), GetEndAngle(),
                 GetFormulaF2(), getIdObject(), getMode());
    return line.p2();
}


//---------------------------------------------------------------------------------------------------------------------
QPointF VArc::CutArc(const qreal &length) const
{
    VArc arc1;
    VArc arc2;
    return this->CutArc(length, arc1, arc2);
}

//---------------------------------------------------------------------------------------------------------------------
void VArc::CreateName()
{
    QString name = ARC_ + QString("%1").arg(this->GetCenter().name());

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
void VArc::FindF2(qreal length)
{
    SetFlipped(length < 0);

    if (length >= MaxLength())
    {
        length = MaxLength();
    }

    qreal arcAngle = qAbs(qRadiansToDegrees(length/d->radius));

    if (IsFlipped())
    {
        arcAngle = arcAngle * -1;
    }

    QLineF startAngle(0, 0, 100, 0);
    startAngle.setAngle(GetStartAngle() + arcAngle);// We use QLineF just because it is easy way correct angle value

    SetFormulaF2(QString().number(startAngle.angle()), startAngle.angle());
}

//---------------------------------------------------------------------------------------------------------------------
qreal VArc::MaxLength() const
{
    return M_2PI*d->radius;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return arc radius.
 * @return radius.
 */
QString VArc::GetFormulaRadius() const
{
    return d->formulaRadius;
}

//---------------------------------------------------------------------------------------------------------------------
void VArc::SetFormulaRadius(const QString &formula, qreal value)
{
    d->formulaRadius = formula;
    d->radius = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return formula for radius.
 * @return string with formula.
 */
qreal VArc::GetRadius() const
{
    return d->radius;
}
