/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "varc.h"
#include <QDebug>

VArc::VArc () : f1(0), formulaF1(QString()), f2(0), formulaF2(QString()), radius(0), formulaRadius(QString()),
    center(0), points(0), mode(Draw::Calculation), idObject(0){
}

VArc::VArc (const QHash<qint64, VPointF> *points, qint64 center, qreal radius, QString formulaRadius,
            qreal f1, QString formulaF1, qreal f2, QString formulaF2, Draw::Draws mode, qint64 idObject)
    : f1(f1), formulaF1(formulaF1), f2(f2), formulaF2(formulaF2), radius(radius), formulaRadius(formulaRadius),
      center(center), points(points), mode(mode), idObject(idObject){
}

VArc::VArc(const VArc &arc): f1(arc.GetF1()), formulaF1(arc.GetFormulaF1()), f2(arc.GetF2()),
    formulaF2(arc.GetFormulaF2()), radius(arc.GetRadius()), formulaRadius(arc.GetFormulaRadius()),
    center(arc.GetCenter()), points(arc.GetDataPoints()), mode(arc.getMode()),
    idObject(arc.getIdObject()){
}

VArc &VArc::operator =(const VArc &arc){
    this->points = arc.GetDataPoints();
    this->f1 = arc.GetF1();
    this->formulaF1 = arc.GetFormulaF1();
    this->f2 = arc.GetF2();
    this->formulaF2 = arc.GetFormulaF2();
    this->radius = arc.GetRadius();
    this->formulaRadius = arc.GetFormulaRadius();
    this->center = arc.GetCenter();
    this->mode = arc.getMode();
    this->idObject = arc.getIdObject();
    return *this;
}

qreal VArc::GetF1() const{
    return f1;
}

QString VArc::GetFormulaF1() const{
    return formulaF1;
}

qreal VArc::GetF2() const{
    return f2;
}

QString VArc::GetFormulaF2() const{
    return formulaF2;
}

qreal VArc::GetLength () const{
    return M_PI * radius/180 * (f2-f1);
}

qreal VArc::GetRadius() const{
    return radius;
}

QString VArc::GetFormulaRadius() const{
    return formulaRadius;
}

qint64 VArc::GetCenter() const{
    return center;
}

QPointF VArc::GetCenterPoint() const{
    if(points->contains(center)){
        return points->value(center).toQPointF();
    } else {
        qCritical()<<"Не можу знайти id = "<<center<<" в таблиці.";
        throw"Не можу знайти точку за id.";
    }
    return QPointF();
}

QPointF VArc::GetP1() const{
    QPointF p1 ( GetCenterPoint().x () + radius, GetCenterPoint().y () );
    QLineF centerP1(GetCenterPoint(), p1);
    centerP1.setAngle(f1);
    return centerP1.p2();
}

QPointF VArc::GetP2 () const{
    QPointF p2 ( GetCenterPoint().x () + radius, GetCenterPoint().y () );
    QLineF centerP2(GetCenterPoint(), p2);
    centerP2.setAngle(f2);
    return centerP2.p2();
}

const QHash<qint64, VPointF> *VArc::GetDataPoints() const{
    return points;
}

QPainterPath VArc::GetPath() const{
    QPainterPath Path;
    QPointF center = GetCenterPoint();
    QRectF rect(center.x()-radius, center.y()-radius, radius*2, radius*2);
    Path.moveTo(GetP1());
    qreal angle = QLineF(center, GetP1()).angleTo(QLineF(center, GetP2()));
    Path.arcTo(rect, GetF1(), angle);
    return Path;
}

qreal VArc::AngleArc() const{
    QLineF l1(0,0, 100, 100);
    l1.setAngle(f1);
    QLineF l2(0,0, 100, 100);
    l2.setAngle(f2);
    return l1.angleTo(l2);
}

qint32 VArc::NumberSplOfArc() const{
    qint32 angArc = static_cast<qint32> (AngleArc ());
    switch( angArc ){
    case   0:
        throw "Кут дуги не може бути 0 градусів.";
        break;
    case  90:
        return 1;
    case 180:
        return 2;
    case 270:
        return 3;
    case 360:
        return 4;
    default :
        return static_cast<qint32> (AngleArc ( ) / 90 + 1);
    }
}

QVector<QPointF> VArc::GetPoints() const{
    QVector<QPointF> points;
    qint32 numberSpl =  NumberSplOfArc();
    for(qint32 i = 1; i <= numberSpl; ++i){
        points<<SplOfArc ( i );
    }
    return points;
}

QVector<QPointF> VArc::SplOfArc(qint32 number) const{
    qint32 n = NumberSplOfArc ();
    if( number > n ){
        throw "Дуга не складається з такої кількості сплайнів.";
    }
    qreal f1 = GetF1 ();
    qreal f2 = GetF2 ();
    qint32 i;
    for ( i = 0; i < n; ++i ){
        if ( i == n - 1 ){
            f2 = GetF2 ();
        } else {
            if ( f1 + 90 > 360 ){
                f2 = f1 + 90 - 360;
            } else {
                f2 = f1 + 90;
            }
        }
        qreal anglF1, anglF2;
        if ( f1 + 90 > 360 ){
            anglF1 =  f1 + 90 - 360 ;
        } else {
            anglF1 = f1 + 90 ;
        }
        if ( f2 - 90 < 0 ){
            anglF2 = 360 + f2 - 90 ;
        } else {
            anglF2 = f2 - 90 ;
        }
        if ( i + 1 == number ){
            f1 = f2;
            return VSpline::SplinePoints(GetP1 (), GetP2 (), anglF1, anglF2, 1., 1., 1.);
        }
    }
    return QVector<QPointF>();
}

Draw::Draws VArc::getMode() const{
    return mode;
}

void VArc::setMode(const Draw::Draws &value){
    mode = value;
}

qint64 VArc::getIdObject() const{
    return idObject;
}

void VArc::setIdObject(const qint64 &value){
    idObject = value;
}
