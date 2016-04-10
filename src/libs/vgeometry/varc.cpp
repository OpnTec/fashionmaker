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
#include "varc_p.h"
#include "../ifc/ifcdef.h"
#include "vspline.h"

#include <QDebug>
#include <QLineF>
#include <QPainterPath>
#include <QPointF>
#include <QtCore/qmath.h>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VArc default constructor.
 */
VArc::VArc ()
    :VAbstractCurve(GOType::Arc), d (new VArcData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VArc constructor.
 * @param center center point.
 * @param radius arc radius.
 * @param f1 start angle (degree).
 * @param f2 end angle (degree).
 */
VArc::VArc (VPointF center, qreal radius, QString formulaRadius, qreal f1, QString formulaF1, qreal f2,
            QString formulaF2, quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::Arc, idObject, mode),
      d (new VArcData(center, radius, formulaRadius, f1, formulaF1, f2, formulaF2))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VArc::VArc(VPointF center, qreal radius, qreal f1, qreal f2)
    : VAbstractCurve(GOType::Arc, NULL_ID, Draw::Calculation), d (new VArcData(center, radius, f1, f2))
{
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VArc::VArc(qreal length, QString formulaLength, VPointF center, qreal radius, QString formulaRadius, qreal f1,
           QString formulaF1, quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::Arc, idObject, mode),
      d (new VArcData(formulaLength, center, radius, formulaRadius, f1, formulaF1))
{
    CreateName();
    FindF2(length);
}

//---------------------------------------------------------------------------------------------------------------------
VArc::VArc(qreal length, VPointF center, qreal radius, qreal f1)
    : VAbstractCurve(GOType::Arc, NULL_ID, Draw::Calculation), d (new VArcData(center, radius, f1))
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
    : VAbstractCurve(arc), d (arc.d)
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
    VAbstractCurve::operator=(arc);
    d = arc.d;
    return *this;
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
    qreal length = (M_PI * d->radius)/180 * AngleArc();
    if (d->isFlipped)
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
    QLineF centerP1(GetCenter().toQPointF(), p1);
    centerP1.setAngle(d->f1);
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
    QLineF centerP2(GetCenter().toQPointF(), p2);
    centerP2.setAngle(d->f2);
    return centerP2.p2();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AngleArc calculate arc angle.
 * @return angle in degree.
 */
qreal VArc::AngleArc() const
{
    {
        const qreal angleDiff = qAbs(d->f1 - d->f2);
        if (VFuzzyComparePossibleNulls(angleDiff, 0) || VFuzzyComparePossibleNulls(angleDiff, 360))
        {
            return 360;
        }
    }
    QLineF l1(0, 0, 100, 0);
    l1.setAngle(d->f1);
    QLineF l2(0, 0, 100, 0);
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
 * @brief GetPoints return list of points needed for drawing arc.
 * @return list of points
 */
QVector<QPointF> VArc::GetPoints() const
{
    QVector<QPointF> points;
    QVector<qreal> sectionAngle;

    QPointF pStart;
    d->isFlipped ? pStart = GetP2() : pStart = GetP1();

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
        const qreal lDistance = GetRadius() * 4.0/3.0 * qTan(M_PI/180.0 * sectionAngle.at(i) * 0.25);

        const QPointF center = GetCenter().toQPointF();

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
            splPoints.removeLast();
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

    qreal n = (len*180)/(M_PI*d->radius); // n - is angle in degrees
    QLineF line(GetCenter().toQPointF(), GetP1());
    line.setAngle(line.angle()+n);

    arc1 = VArc (d->center, d->radius, d->formulaRadius, d->f1, d->formulaF1, line.angle(),
                 QString().setNum(line.angle()), getIdObject(), getMode());

    arc2 = VArc (d->center, d->radius, d->formulaRadius, line.angle(), QString().setNum(line.angle()), d->f2,
                 d->formulaF2, getIdObject(), getMode());
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
/**
 * @brief setId keep id arc in data.
 * @param id id arc in data.
 */
void VArc::setId(const quint32 &id)
{
    VAbstractCurve::setId(id);
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
QString VArc::NameForHistory(const QString &toolName) const
{
    QString name = toolName + QString(" %1").arg(this->GetCenter().name());

    if (VAbstractCurve::id() != NULL_ID)
    {
        name += QString("_%1").arg(VAbstractCurve::id());
    }

    if (GetDuplicate() > 0)
    {
        name += QString("_%1").arg(GetDuplicate());
    }
    return name;
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
    length < 0 ? d->isFlipped = true : d->isFlipped = false;

    if (length >= MaxLength())
    {
        length = MaxLength();
    }

    qreal arcAngle = (qAbs(length)*180)/(M_PI*d->radius);

    if (d->isFlipped)
    {
        arcAngle = arcAngle * -1;
    }

    QLineF startAngle(0, 0, 100, 0);
    startAngle.setAngle(d->f1 + arcAngle);// We use QLineF just because it is easy way correct angle value

    d->f2 = startAngle.angle();
    d->formulaF2 = QString().number(d->f2);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VArc::MaxLength() const
{
    return M_2PI*d->radius;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return start angle.
 * @return angle in degree.
 */
QString VArc::GetFormulaF1() const
{
    return d->formulaF1;
}

//---------------------------------------------------------------------------------------------------------------------
void VArc::SetFormulaF1(const QString &formula, qreal value)
{
    d->formulaF1 = formula;
    d->f1 = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return formula for start angle.
 * @return string with formula.
 */
qreal VArc::GetStartAngle() const
{
    return d->f1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return end angle.
 * @return angle in degree.
 */
QString VArc::GetFormulaF2() const
{
    return d->formulaF2;
}

//---------------------------------------------------------------------------------------------------------------------
void VArc::SetFormulaF2(const QString &formula, qreal value)
{
    d->formulaF2 = formula;
    d->f2 = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return formula for end angle.
 * @return string with formula.
 */
qreal VArc::GetEndAngle() const
{
    return d->f2;
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetCenter return center point.
 * @return center point.
 */
VPointF VArc::GetCenter() const
{
    return d->center;
}

//---------------------------------------------------------------------------------------------------------------------
void VArc::SetCenter(const VPointF &value)
{
    d->center = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VArc::GetFormulaLength() const
{
    return d->formulaLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VArc::SetFormulaLength(const QString &formula, qreal value)
{
    d->formulaLength = formula;
    FindF2(value);
}
