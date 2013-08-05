#define _USE_MATH_DEFINES
#include <cmath>
#include "varc.h"
#include <QDebug>

VArc::VArc (){
    f1     = 0;
    f2     = 0;
    length = 0;
    radius = 0;
    center = 0;
    p1     = QPointF( 0, 0 );
    p2     = QPointF( 0, 0 );
    points = 0;
}

VArc::VArc (const QMap<qint64, VPointF> *points, qint64 center, qreal radius, qreal f1, qreal f2 ){
    this->points = points;
    ModifiArc( center, radius, f1, f2 );
}

qreal VArc::GetF1 () const{
    return f1;
}

qreal VArc::GetF2 () const{
    return f2;
}

qreal VArc::GetLength () const{
    return length;
}

qreal VArc::GetRadius () const{
    return radius;
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
    return p1;
}

QPointF VArc::GetP2 () const{
    return p2;
}

const QMap<qint64, VPointF> *VArc::GetDataPoints() const{
    return points;
}

void VArc::ModifiArc (qint64 center, qreal radius, qreal f1, qreal f2 ){
    this->f1 = f1;
    this->f2 = f2;
    this->radius = radius;
    this->center = center;
    QPointF p1 ( GetCenterPoint().x () + radius, GetCenterPoint().y () );
    QLineF centerP1(GetCenterPoint(), p1);
    centerP1.setAngle(f1);
    p1 = centerP1.p2();
    QPointF p2 ( GetCenterPoint().x () + radius, GetCenterPoint().y () );
    QLineF centerP2(GetCenterPoint(), p2);
    centerP2.setAngle(f2);
    p2 = centerP2.p2();
    this->p1 = p1;
    this->p2 = p2;
    // визначимо довжину дуги за формулою.
    // МОЖЛИВО НЕ ПРАВИЛЬНО. ПОТРІБНО ПЕРЕВІРИТИ ФОРМУЛУ, ОСКІЛЬКИ КУТ ПОВЕРТАЄТЬСЯ В ГРАДУСАХ!!!!!
    //qreal ang = AngleArc ( f1, f2 ) ;
    qreal ang = centerP1.angleTo(centerP2);
    this->length = ang * radius * M_PI/180;
    //qDebug()<<"ang = "<<ang<<"length ="<<length;
}

//void VArc::BiasArc ( qreal mx, qreal my ){
//    this->center = QPointF( center.x()+mx, center.y()+my );
//    this->p1 = QPointF( p1.x()+mx, p1.y()+my );
//    this->p2 = QPointF( p2.x()+mx, p2.y()+my );
//}

//void VArc::RotationArc (QPointF pRotate, qreal angle ){
//    QLineF pRotateCenter( pRotate, center );
//    pRotateCenter.setAngle( angle );
//    center = pRotateCenter.p2();
//    QLineF pRotateP1( pRotate, p1 );
//    pRotateP1.setAngle( angle );
//    p1 = pRotateP1.p2();
//    QLineF pRotateP2( pRotate, p2 );
//    pRotateP2.setAngle( angle );
//    p2 = pRotateP2.p2();
//    QLineF centerP1 ( center, p1 );
//    QLineF centerP2 ( center, p2 );
//    f1 = centerP1.angle();
//    f2 = centerP2.angle();
//}

VSpline VArc::SplOfArc ( qint32 number ) const{
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
//            return VSpline (GetDataPoints(), GetP1 (), GetP2 (), anglF1, anglF2, 1., 1., 1. );
            f1 = f2;
        }
    }
    return VSpline();
}

//void VArc::CutArc ( qreal length, VArc &arcFir, VArc &arcSec ) const{
//    if ( length > GetLength () ){
//        qDebug()<<"Не правильна довжина дуги/n";
//        exit(-1);
//    }
//    qreal angle;
//    angle = GetF1 () + (length * 180) / (radius * M_PI);
//    arcFir.ModifiArc ( GetCenter (), GetRadius (), GetF1 (), angle );
//    arcSec.ModifiArc ( GetCenter (), GetRadius (), angle, GetF2 () );
//}

//void VArc::CutArc ( QPointF point, VArc &arcFir, VArc &arcSec ) const{
//    VArc arc = VArc(center,radius, f1, QLineF(center, point).angle());
//    if(this->length<arc.GetLength()){
//        throw"Не можу розрізати дугу. Не правильна точка.";
//    } else {
//        CutArc ( arc.GetLength(), arcFir, arcSec );
//    }
//}

QLineF::IntersectType VArc::CrossingArcLine ( const QLineF &line, QPointF *intersectionPoint ) const{
    qint32 i, n;
    QLineF::IntersectType type = QLineF::NoIntersection;
    n = NumberSplOfArc ();
    for ( i = 0; i < n; ++i ){
        VSpline curve = SplOfArc ( i + 1 );
        QPointF crosPoint;
        type = curve.CrossingSplLine ( line, &crosPoint );
        if ( type == QLineF::BoundedIntersection ){
            *intersectionPoint = crosPoint;
            return type;
        } else{
            if ( type == QLineF::NoIntersection || type == QLineF::UnboundedIntersection ){
                throw "Не можу знайти точку перетину сплайну з лінією.";
            }
        }
    }
}

qint32 VArc::NumberSplOfArc () const{
    qint32 angArc = ( qint32 ) AngleArc ( GetF1 (), GetF2 () );
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
        return ( qint32 ) AngleArc ( GetF1 (), GetF2 () ) / 90 + 1;
    }
}

qreal VArc::AngleArc( qreal f1, qreal f2 ) const{
    QLineF normal = QLineF( 0, 0, 100, 0 );
    normal.setAngle(f1);
    QLineF normal2 = QLineF( 0, 0, 100, 0 );
    normal2.setAngle(f2);
    return normal.angleTo(normal2);
}

QPainterPath VArc::GetPath() const{
    QPainterPath Path;
    qint32 numberSpl =  NumberSplOfArc();
    for(qint32 i = 1; i <= numberSpl; ++i){
        VSpline spl = SplOfArc ( i );
        Path.addPath( spl.GetPath() );
    }
    return Path;
}

QVector<QPointF> VArc::GetPoints () const{
    QVector<QPointF> points;
    qint32 numberSpl =  NumberSplOfArc();
    for(qint32 i = 1; i <= numberSpl; ++i){
        VSpline spl = SplOfArc ( i );
        points<<spl.GetPoints();
    }
    return points;
}

QPointF VArc::Tangent(const QPointF P, Tangent_e tan)const{
    QLineF CP = QLineF(GetCenterPoint(),P);
    CP.setLength(CP.length()/2);
    QPointF O = CP.p2();
    QLineF CO = QLineF(GetCenterPoint(),O);
    qreal d = CO.length();
    if ( d > GetRadius() + CO.length() ){
        throw"Немає перетину кіл для знаходження дотичних.";
    }
    if ( d < qAbs(GetRadius() - CO.length()) ){
        throw"Не можу знайти дотичну. Одне коло знаходиться в іншому.";
    }
    qreal a = (pow(GetRadius(),2) - pow(CO.length(),2) + pow(d,2) ) / (2*d);
    qreal h = sqrt(pow(GetRadius(),2) - pow(a,2));
    qreal x2 = GetCenterPoint().x() + a * (O.x() - GetCenterPoint().x())/d;
    qreal y2 = GetCenterPoint().y() + a * (O.y() - GetCenterPoint().y())/d;
    if(tan == VArc::FirstTangent){
        qreal x3 = x2 - h * ( O.y() - GetCenterPoint().y() ) / d;
        qreal y3 = y2 + h * ( O.x() - GetCenterPoint().x() ) / d;
        return QPointF(x3,y3);
    } else {
        qreal x3 = x2 + h * ( O.y() - GetCenterPoint().y() ) / d;
        qreal y3 = y2 - h * ( O.x() - GetCenterPoint().x() ) / d;
        return QPointF(x3,y3);
    }
}

bool VArc::Contains(const QPointF P)const{
    QVector<QPointF> points = GetPoints ();
    for(qint32 i=0; i< points.size();++i){
        if((qint32)points[i].x()==(qint32)P.x() && (qint32)points[i].y()==(qint32)P.y()){
            return true;
        }
    }
    return false;
}

QPointF VArc::PutAlongArc(qreal length) const{
    if(GetLength()<length){
        throw"Довжина дуги менше заданої довжини";
    } else {
        qreal n = (length*180)/(M_PI*GetRadius());
        QLineF CP1 = QLineF(GetCenterPoint(),GetP1());
        CP1.setAngle(CP1.angle()+n);
        return CP1.p2();
    }
}

//void VArc::Mirror(const QPointF Pmirror){
//    QPointF Center = center;
//    Center = QPointF(Center.x() - Pmirror.x(), Center.y() - Pmirror.y());
//    Center = QPointF(Center.x() * -1.0, Center.y() * 1.0);
//    Center = QPointF(Center.x() + Pmirror.x(), Center.y() + Pmirror.y());
//    QPointF P1 = p1;
//    P1 = QPointF(P1.x() - Pmirror.x(), P1.y() - Pmirror.y());
//    P1 = QPointF(P1.x() * -1.0, P1.y() * 1.0);
//    P1 = QPointF(P1.x() + Pmirror.x(), P1.y() + Pmirror.y());
//    QPointF P2 = p2;
//    P2 = QPointF(P2.x() - Pmirror.x(), P2.y() - Pmirror.y());
//    P2 = QPointF(P2.x() * -1.0, P2.y() * 1.0);
//    P2 = QPointF(P2.x() + Pmirror.x(), P2.y() + Pmirror.y());
//    this->ModifiArc ( Center, radius, QLineF(Center,P2).angle(), QLineF(Center,P1).angle() );
//}
