/************************************************************************
 **
 **  @file   vellipticalarc.cpp
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   February 1, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../vmisc/compatibility.h"
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
                                const QString &formulaF2, qreal rotationAngle, const QString &formulaRotationAngle,
                                quint32 idObject, Draw mode)
    : VAbstractArc(GOType::EllipticalArc, center, f1, formulaF1, f2, formulaF2, idObject, mode),
      d (new VEllipticalArcData(radius1, radius2, formulaRadius1, formulaRadius2, rotationAngle, formulaRotationAngle))
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
                               const QString &formulaF1, qreal rotationAngle, const QString &formulaRotationAngle,
                               quint32 idObject, Draw mode)
    : VAbstractArc(GOType::EllipticalArc, formulaLength, center, f1, formulaF1, idObject, mode),
      d (new VEllipticalArcData(radius1, radius2, formulaRadius1, formulaRadius2, rotationAngle, formulaRotationAngle))
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

#ifdef Q_COMPILER_RVALUE_REFS
//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(const VEllipticalArc &&arc) Q_DECL_NOTHROW
    : VAbstractArc(arc), d (arc.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc &VEllipticalArc::operator=(VEllipticalArc &&arc) Q_DECL_NOTHROW
{
    VAbstractArc::operator=(arc);
    std::swap(d, arc.d);
    return *this;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc VEllipticalArc::Rotate(QPointF originPoint, qreal degrees, const QString &prefix) const
{
    originPoint = d->m_transform.inverted().map(originPoint);

    QTransform t = d->m_transform;
    t.translate(originPoint.x(), originPoint.y());
    t.rotate(IsFlipped() ? degrees : -degrees);
    t.translate(-originPoint.x(), -originPoint.y());

    VEllipticalArc elArc(VAbstractArc::GetCenter(), GetRadius1(), GetRadius2(), VAbstractArc::GetStartAngle(),
                         VAbstractArc::GetEndAngle(), GetRotationAngle());
    elArc.setName(name() + prefix);
    elArc.SetColor(GetColor());
    elArc.SetPenStyle(GetPenStyle());
    elArc.SetFlipped(IsFlipped());
    elArc.SetTransform(t);
    return elArc;
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc VEllipticalArc::Flip(const QLineF &axis, const QString &prefix) const
{
    VEllipticalArc elArc(VAbstractArc::GetCenter(), GetRadius1(), GetRadius2(), VAbstractArc::GetStartAngle(),
                         VAbstractArc::GetEndAngle(), GetRotationAngle());
    elArc.setName(name() + prefix);
    elArc.SetColor(GetColor());
    elArc.SetPenStyle(GetPenStyle());
    elArc.SetFlipped(not IsFlipped());
    elArc.SetTransform(d->m_transform * VGObject::FlippingMatrix(d->m_transform.inverted().map(axis)));
    return elArc;
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc VEllipticalArc::Move(qreal length, qreal angle, const QString &prefix) const
{
    const VPointF oldCenter = VAbstractArc::GetCenter();
    const VPointF center = oldCenter.Move(length, angle);

    const QPointF position = d->m_transform.inverted().map(center.toQPointF()) -
            d->m_transform.inverted().map(oldCenter.toQPointF());

    QTransform t = d->m_transform;
    t.translate(position.x(), position.y());

    VEllipticalArc elArc(oldCenter, GetRadius1(), GetRadius2(), VAbstractArc::GetStartAngle(),
                         VAbstractArc::GetEndAngle(), GetRotationAngle());
    elArc.setName(name() + prefix);
    elArc.SetColor(GetColor());
    elArc.SetPenStyle(GetPenStyle());
    elArc.SetFlipped(IsFlipped());
    elArc.SetTransform(t);
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
    return GetTransform().map(GetP(VAbstractArc::GetStartAngle()));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2 return point associated with end angle.
 * @return point.
 */
QPointF VEllipticalArc::GetP2 () const
{
    return GetTransform().map(GetP(VAbstractArc::GetEndAngle()));
}

//---------------------------------------------------------------------------------------------------------------------
QTransform VEllipticalArc::GetTransform() const
{
    return d->m_transform;
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::SetTransform(const QTransform &matrix, bool combine)
{
    d->m_transform = combine ? d->m_transform * matrix : matrix;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VEllipticalArc::GetCenter() const
{
    VPointF center = VAbstractArc::GetCenter();
    const QPointF p = d->m_transform.map(center.toQPointF());
    center.setX(p.x());
    center.setY(p.y());
    return center;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoints return list of points needed for drawing arc.
 * @return list of points
 */
QVector<QPointF> VEllipticalArc::GetPoints() const
{
    const QPointF center = VAbstractArc::GetCenter().toQPointF();
    QRectF box(center.x() - d->radius1, center.y() - d->radius2, d->radius1*2, d->radius2*2);

    QLineF startLine(center.x(), center.y(), center.x() + d->radius1, center.y());
    QLineF endLine = startLine;

    startLine.setAngle(VAbstractArc::GetStartAngle());
    endLine.setAngle(VAbstractArc::GetEndAngle());
    qreal sweepAngle = startLine.angleTo(endLine);

    if (qFuzzyIsNull(sweepAngle))
    {
        sweepAngle = 360;
    }

    QPainterPath path;
    path.moveTo(GetP1());
    path.arcTo(box, VAbstractArc::GetStartAngle(), sweepAngle);
    path.moveTo(GetP2());

    QTransform t = d->m_transform;
    t.translate(center.x(), center.y());
    t.rotate(-GetRotationAngle());
    t.translate(-center.x(), -center.y());

    path = t.map(path);

    QPolygonF polygon;
    const QList<QPolygonF> sub = path.toSubpathPolygons();
    if (not sub.isEmpty())
    {
        polygon = ConstFirst (path.toSubpathPolygons());

        if (not polygon.isEmpty() && not VFuzzyComparePoints(GetP1(), ConstFirst<QPointF> (polygon)))
        {
            polygon.removeFirst(); // remove point (0;0)
        }
    }

    return static_cast<QVector<QPointF>>(polygon);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VEllipticalArc::GetStartAngle() const
{
    return QLineF(GetCenter().toQPointF(), GetP1()).angle() - GetRotationAngle();
}

//---------------------------------------------------------------------------------------------------------------------
qreal VEllipticalArc::GetEndAngle() const
{
    return QLineF(GetCenter().toQPointF(), GetP2()).angle() - GetRotationAngle();
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
                           GetFormulaRotationAngle(), getIdObject(), getMode());
    // the second arc has startAngle just like endAngle of the first arc
    // and it has endAngle just like endAngle of the origin arc
    arc2 = VEllipticalArc (GetCenter(), d->radius1, d->radius2, d->formulaRadius1, d->formulaRadius2,
                           arc1.GetEndAngle(), arc1.GetFormulaF2(), GetEndAngle(), GetFormulaF2(), d->rotationAngle,
                           GetFormulaRotationAngle(), getIdObject(), getMode());
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
    QString name = ELARC_ + QString("%1").arg(this->GetCenter().name());

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
        length = MaxLength();
    }

    // We need to calculate the second angle
    // first approximation of angle between start and end angles

    QLineF radius1(GetCenter().x(), GetCenter().y(), GetCenter().x() + d->radius1, GetCenter().y());
    radius1.setAngle(GetStartAngle());
    radius1.setAngle(radius1.angle() + gap);
    qreal endAngle = radius1.angle();

    // we need to set the end angle, because we want to use GetLength()
    SetFormulaF2(QString::number(endAngle), endAngle);

    qreal lenBez = GetLength(); // first approximation of length

    const qreal eps = ToPixel(0.001, Unit::Mm);

    while (qAbs(lenBez - length) > eps)
    {
        gap = gap/2;
        if (gap < 0.0001)
        {
            break;
        }
        if (lenBez > length)
        { // we selected too big end angle
            radius1.setAngle(endAngle - qAbs(gap));
        }
        else
        { // we selected too little end angle
            radius1.setAngle(endAngle + qAbs(gap));
        }
        endAngle = radius1.angle();
        // we need to set d->f2, because we use it when we calculate GetLength
        SetFormulaF2(QString::number(endAngle), endAngle);
        lenBez = GetLength();
    }
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
QPointF VEllipticalArc::GetP(qreal angle) const
{
    QLineF line(0, 0, 100, 0);
    line.setAngle(angle);

    const qreal a = line.p2().x() / GetRadius1();
    const qreal b = line.p2().y() / GetRadius2();
    const qreal k = qSqrt(a*a + b*b);
    QPointF p(line.p2().x() / k, line.p2().y() / k);

    QLineF line2(QPointF(), p);
    SCASSERT(VFuzzyComparePossibleNulls(line2.angle(), line.angle()))

    line2.setAngle(line2.angle() + GetRotationAngle());
    return line2.p2() + VAbstractArc::GetCenter().toQPointF();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormulaRadius1 return formula for major radius.
 * @return radius.
 */
QString VEllipticalArc::GetFormulaRadius1() const
{
    return d->formulaRadius1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormulaRadius2 return formula for minor radius.
 * @return radius.
 */
QString VEllipticalArc::GetFormulaRadius2() const
{
    return d->formulaRadius2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormulaRotationAngle return formula for rotation angle.
 * @return rotationAngle.
 */
QString VEllipticalArc::GetFormulaRotationAngle() const
{
    return d->formulaRotationAngle;
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
void VEllipticalArc::SetFormulaRotationAngle(const QString &formula, qreal value)
{
    d->formulaRotationAngle = formula;
    d->rotationAngle = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius1 return elliptical arc major radius.
 * @return string with formula.
 */
qreal VEllipticalArc::GetRadius1() const
{
    return d->radius1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius2 return elliptical arc minor radius.
 * @return string with formula.
 */
qreal VEllipticalArc::GetRadius2() const
{
    return d->radius2;
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
