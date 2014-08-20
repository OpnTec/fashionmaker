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

#include "varc.h"
#include <QDebug>
#include <QLineF>
#include <QPainterPath>
#include <QPointF>

#ifdef Q_OS_WIN32
#   include <QtMath> // for M_PI on Windows
#endif /*Q_OS_WIN32*/


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VArc default constructor.
 */
VArc::VArc ()
    :VAbstractCurve(GOType::Arc), f1(0), formulaF1(QString()), f2(0), formulaF2(QString()), radius(0),
      formulaRadius(QString()), center(VPointF())
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
    : VAbstractCurve(GOType::Arc, idObject, mode), f1(f1), formulaF1(formulaF1), f2(f2), formulaF2(formulaF2),
      radius(radius), formulaRadius(formulaRadius), center(center)
{
    setName(QString (arc_+"%1").arg(this->GetCenter().name()));
}

//---------------------------------------------------------------------------------------------------------------------
VArc::VArc(VPointF center, qreal radius, qreal f1, qreal f2)
    : VAbstractCurve(GOType::Arc, 0, Draw::Calculation), f1(f1), formulaF1(QStringLiteral("")), f2(f2),
      formulaF2(QStringLiteral("")), radius(radius), formulaRadius(QStringLiteral("")), center(center)
{
    setName(QString (arc_+"%1").arg(this->GetCenter().name()));
    formulaF1 = QString("%1").arg(f1);
    formulaF2 = QString("%1").arg(f2);
    formulaRadius = QString("%1").arg(radius);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VArc copy constructor
 * @param arc arc
 */
VArc::VArc(const VArc &arc)
    : VAbstractCurve(arc), f1(arc.GetF1()), formulaF1(arc.GetFormulaF1()), f2(arc.GetF2()),
    formulaF2(arc.GetFormulaF2()), radius(arc.GetRadius()), formulaRadius(arc.GetFormulaRadius()),
    center(arc.GetCenter())
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
    this->f1 = arc.GetF1();
    this->formulaF1 = arc.GetFormulaF1();
    this->f2 = arc.GetF2();
    this->formulaF2 = arc.GetFormulaF2();
    this->radius = arc.GetRadius();
    this->formulaRadius = arc.GetFormulaRadius();
    this->center = arc.GetCenter();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLength return arc length.
 * @return length.
 */
qreal VArc::GetLength() const
{
    return (M_PI * radius)/180 * AngleArc();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP1 return point associated with start angle.
 * @return point.
 */
QPointF VArc::GetP1() const
{
    QPointF p1 ( GetCenter().x () + radius, GetCenter().y () );
    QLineF centerP1(GetCenter().toQPointF(), p1);
    centerP1.setAngle(f1);
    return centerP1.p2();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetP2 return point associated with end angle.
 * @return точку point.
 */
QPointF VArc::GetP2 () const
{
    QPointF p2 ( GetCenter().x () + radius, GetCenter().y () );
    QLineF centerP2(GetCenter().toQPointF(), p2);
    centerP2.setAngle(f2);
    return centerP2.p2();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AngleArc calculate arc angle.
 * @return angle in degree.
 */
qreal VArc::AngleArc() const
{
    QLineF l1(0, 0, 100, 100);
    l1.setAngle(f1);
    QLineF l2(0, 0, 100, 100);
    l2.setAngle(f2);
    return l1.angleTo(l2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPoints return list of points needed for drawing arc.
 * @return list of points
 */
QVector<QPointF> VArc::GetPoints() const
{
    QVector<QPointF> points;
    qreal i = 0;
    qreal angle = AngleArc();
    qint32 k = static_cast<qint32>(angle);
    qreal s = angle/(k/4);
    do
    {
        QLineF line(center.toQPointF(), GetP1());
        line.setAngle(line.angle()+i);
        points.append(line.p2());
        i = i + s;
        if (i > angle)
        {
            QLineF line(center.toQPointF(), GetP1());
            line.setAngle(line.angle()+angle);
            points.append(line.p2());
        }
    } while (i <= angle);
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

    qreal n = (len*180)/(M_PI*radius);
    QLineF line(GetCenter().toQPointF(), GetP1());
    line.setAngle(line.angle()+n);

    arc1 = VArc (center, radius, formulaRadius, f1, formulaF1, line.angle(), QString().setNum(line.angle()),
                 getIdObject(), getMode());

    arc2 = VArc (center, radius, formulaRadius, line.angle(), QString().setNum(line.angle()), f2, formulaF2,
                 getIdObject(), getMode());
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
    setName(QString (arc_+"%1_%2").arg(center.name()).arg(id));
}
