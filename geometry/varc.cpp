#define _USE_MATH_DEFINES
#include <cmath>
#include "varc.h"
#include <QDebug>

VArc::VArc (){
    f1     = 0;
    formulaF1 = QString();
    f2     = 0;
    formulaF2 = QString();
    radius = 0;
    formulaRadius = QString();
    center = 0;
    points = 0;
}

VArc::VArc (const QMap<qint64, VPointF> *points, qint64 center, qreal radius, QString formulaRadius,
            qreal f1, QString formulaF1, qreal f2, QString formulaF2 ){
    this->points = points;
    this->f1 = f1;
    this->formulaF1 = formulaF1;
    this->f2 = f2;
    this->formulaF2 = formulaF2;
    this->radius = radius;
    this->formulaRadius = formulaRadius;
    this->center = center;
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
        return points->value(center);
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

const QMap<qint64, VPointF> *VArc::GetDataPoints() const{
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

