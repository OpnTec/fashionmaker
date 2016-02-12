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
            QString formulaF2,  qreal rotationAngle, quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::EllipticalArc, idObject, mode),
      d (new VEllipticalArcData(center, radius1, radius2, formulaRadius1, formulaRadius2,
      f1, formulaF1, f2, formulaF2, rotationAngle))
{
    EllipticalArcName();
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(VPointF center, qreal radius1, qreal radius2, qreal f1, qreal f2, qreal rotationAngle)
    : VAbstractCurve(GOType::EllipticalArc, NULL_ID, Draw::Calculation),
      d (new VEllipticalArcData(center, radius1, radius2, f1, f2, rotationAngle))
{
    EllipticalArcName();
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(qreal length, QString formulaLength, VPointF center, qreal radius1, qreal radius2,
           QString formulaRadius1, QString formulaRadius2, qreal f1, QString formulaF1, quint32 idObject, Draw mode)
    : VAbstractCurve(GOType::EllipticalArc, idObject, mode),
      d (new VEllipticalArcData(formulaLength, center, radius1, radius2, formulaRadius1, formulaRadius2, f1, formulaF1))
{
    EllipticalArcName();
    FindF2(length);
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArc::VEllipticalArc(qreal length, VPointF center, qreal radius1, qreal radius2, qreal f1)
    : VAbstractCurve(GOType::EllipticalArc, NULL_ID, Draw::Calculation),
      d (new VEllipticalArcData(center, radius1, radius2, f1))
{
    EllipticalArcName();
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

    QVector<qreal> sectionAngle = GetAngles();

    for (int i = 0; i < sectionAngle.size()-1; ++i)
    {
        QPointF firstPoint = GetPoint(sectionAngle.at(i));
        QPointF point2 = GetPoint((2*sectionAngle.at(i) + sectionAngle.at(i+1))/3);
        QPointF point3 = GetPoint((sectionAngle.at(i) + 2*sectionAngle.at(i+1))/3);
        QPointF lastPoint = GetPoint(sectionAngle.at(i+1));

        qreal dx1 = point2.rx() - firstPoint.rx();
        qreal dy1 = point2.ry() - firstPoint.ry();
        qreal dx2 = point3.rx() - point2.rx();
        qreal dy2 = point3.ry() - point2.ry();
        qreal dx3 = lastPoint.rx() - point3.rx();
        qreal dy3 = lastPoint.ry() - point3.ry();
        length += qSqrt(dx1*dx1 + dy1*dy1) + qSqrt(dx2*dx2 + dy2*dy2) + qSqrt(dx3*dx3 + dy3*dy3);
    }

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
    // we ignore the rotation!
    QPointF p1 ( GetCenter().x () + qFloor((d->radius1 + d->radius2)*cos(d->f1*M_PI/180)),
                 GetCenter().y () + qFloor((d->radius1 + d->radius2)*sin(d->f1*M_PI/180)));
    return p1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2 return point associated with end angle.
 * @return point.
 */
QPointF VEllipticalArc::GetP2 () const
{
    // we ignore the rotation!
    QPointF p2 ( GetCenter().x () + qFloor((d->radius1 + d->radius2)*cos(d->f2*M_PI/180)),
                 GetCenter().y () + qFloor((d->radius1 + d->radius2)*sin(d->f2*M_PI/180)));
    return p2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoint return point associated with angle.
 * @return point.
 */
QPointF VEllipticalArc::GetPoint (qreal angle) const
{
    // we ignore the rotation!
    QPointF p ( GetCenter().x () + qFloor((d->radius1 + d->radius2)*cos(angle*M_PI/180)),
                 GetCenter().y () + qFloor((d->radius1 + d->radius2)*sin(angle*M_PI/180)));
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AngleArc calculate arc angle.
 * @return angle in degree.
 */
qreal VEllipticalArc::AngleArc() const
{
    if ((qFuzzyCompare(d->f1+1, 0+1) && qFuzzyCompare(d->f2, 360)) ||
        (qFuzzyCompare(d->f1, 360) && qFuzzyCompare(d->f2+1, 0+1)))
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
    {
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

    QPointF pStart;
    d->isFlipped ? pStart = GetP2() : pStart = GetP1();

    // if angle1 == angle2 and we have just one point of arc
    if(sectionAngle.size() == 1)
    {
        points.append(GetP1());
    }
    else
    {
        for (int i = 0; i < sectionAngle.size()-1; ++i)
        {
            QPointF firstPoint = GetPoint(sectionAngle.at(i));
            QPointF point2 = GetPoint((2*sectionAngle.at(i) + sectionAngle.at(i+1))/3);
            QPointF point3 = GetPoint((sectionAngle.at(i) + 2*sectionAngle.at(i+1))/3);
            QPointF lastPoint = GetPoint(sectionAngle.at(i+1));

            VSpline spl(VPointF(firstPoint), point2, point3, VPointF(lastPoint), 1.0);

            QVector<QPointF> splPoints = spl.GetPoints();

            if (not splPoints.isEmpty() && i != sectionAngle.size() - 1)
            {
                splPoints.removeLast();
            }
            points << splPoints;
        }
        points << splPoints;
    }
    // we ignore the rotation of ellipse.
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
}


//---------------------------------------------------------------------------------------------------------------------
QPointF VEllipticalArc::CutArc(const qreal &length) const
{
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setId keep id arc in data.
 * @param id id arc in data.
 */
void VEllipticalArc::setId(const quint32 &id)
{
    VAbstractCurve::setId(id);
    setName(EARC_ + QString("%1_%2").arg(d->center.name()).arg(id));
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::EllipticalArcName()
{
    setName(EARC_ + QString("%1").arg(this->GetCenter().name()));
}

//---------------------------------------------------------------------------------------------------------------------
void VEllipticalArc::FindF2(qreal length)
{
    // We need to calculate the second angle,
}

//---------------------------------------------------------------------------------------------------------------------
qreal VEllipticalArc::MaxLength() const
{
    return M_PI*(d->radius1+d->radius2);
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
