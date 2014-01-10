/************************************************************************
 **
 **  @file   varc.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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
#include "vspline.h"
#include "../exception/vexception.h"

class QRectF;

VArc::VArc ()
    :VGObject(GObject::Arc), f1(0), formulaF1(QString()), f2(0), formulaF2(QString()), radius(0),
      formulaRadius(QString()), center(VPointF())
{
}

VArc::VArc (VPointF center, qreal radius, QString formulaRadius, qreal f1, QString formulaF1, qreal f2,
            QString formulaF2, qint64 idObject, Draw::Draws mode)
    : VGObject(GObject::Arc, idObject, mode), f1(f1), formulaF1(formulaF1), f2(f2), formulaF2(formulaF2),
      radius(radius), formulaRadius(formulaRadius), center(center)
{
    _name = QString ("Arc_%1").arg(this->GetCenter().name());
}

VArc::VArc(const VArc &arc)
    : VGObject(arc), f1(arc.GetF1()), formulaF1(arc.GetFormulaF1()), f2(arc.GetF2()),
    formulaF2(arc.GetFormulaF2()), radius(arc.GetRadius()), formulaRadius(arc.GetFormulaRadius()),
    center(arc.GetCenter())
{
}

VArc &VArc::operator =(const VArc &arc)
{
    VGObject::operator=(arc);
    this->f1 = arc.GetF1();
    this->formulaF1 = arc.GetFormulaF1();
    this->f2 = arc.GetF2();
    this->formulaF2 = arc.GetFormulaF2();
    this->radius = arc.GetRadius();
    this->formulaRadius = arc.GetFormulaRadius();
    this->center = arc.GetCenter();
    return *this;
}

qreal VArc::GetLength() const
{
    return (M_PI * radius)/180 * AngleArc();
}

QPointF VArc::GetP1() const
{
    QPointF p1 ( GetCenter().x () + radius, GetCenter().y () );
    QLineF centerP1(GetCenter().toQPointF(), p1);
    centerP1.setAngle(f1);
    return centerP1.p2();
}

QPointF VArc::GetP2 () const
{
    QPointF p2 ( GetCenter().x () + radius, GetCenter().y () );
    QLineF centerP2(GetCenter().toQPointF(), p2);
    centerP2.setAngle(f2);
    return centerP2.p2();
}

QPainterPath VArc::GetPath() const
{
    QPainterPath path;

    QVector<QPointF> points = GetPoints();
    if (points.count() >= 2)
    {
        for (qint32 i = 0; i < points.count()-1; ++i)
        {
            path.moveTo(points[i]);
            path.lineTo(points[i+1]);
        }
    }
    else
    {
        qWarning()<<"points.count() < 2"<<Q_FUNC_INFO;
    }
    return path;
}

qreal VArc::AngleArc() const
{
    QLineF l1(0, 0, 100, 100);
    l1.setAngle(f1);
    QLineF l2(0, 0, 100, 100);
    l2.setAngle(f2);
    return l1.angleTo(l2);
}

QVector<QPointF> VArc::GetPoints() const
{
    QVector<QPointF> points;
    qreal i = 0;
    qreal angle = qRound(AngleArc());
    do
    {
        QLineF line(center.toQPointF(), GetP1());
        line.setAngle(line.angle()+i);
        points.append(line.p2());
        i = i + 0.1;
    }
    while(i <= angle);
    return points;
}

QString VArc::name() const
{
    return _name;
}

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
                 idObject, mode);

    arc2 = VArc (center, radius, formulaRadius, line.angle(), QString().setNum(line.angle()), f2, formulaF2,
                 idObject, mode);
    return line.p2();
}

void VArc::setId(const qint64 &id)
{
    _id = id;
    _name = QString ("Arc_%1_%2").arg(center.name()).arg(id);
}
