#define _USE_MATH_DEFINES
#include <cmath>
#include "vspline.h"
#include <QDebug>

VSpline::VSpline():p1(0), p2(QPointF()), p3(QPointF()), p4(0), angle1(0), angle2(0), kAsm1(1), kAsm2(1),
    kCurve(1), points(0), _referens(0), mode(Draw::Calculation), idObject(0){
}

VSpline::VSpline ( const VSpline & spline ):p1(spline.GetP1 ()), p2(spline.GetP2 ()), p3(spline.GetP3 ()),
    p4(spline.GetP4 ()), angle1(spline.GetAngle1 ()), angle2(spline.GetAngle2 ()), kAsm1(spline.GetKasm1()),
    kAsm2(spline.GetKasm2()), kCurve(spline.GetKcurve()), points(spline.GetDataPoints()), _referens(0),
    mode(spline.getMode()), idObject(spline.getIdObject()){
}

VSpline::VSpline (const QMap<qint64, VPointF> *points, qint64 p1, qint64 p4, qreal angle1, qreal angle2,
                  qreal kAsm1, qreal kAsm2 , qreal kCurve, Draw::Mode mode, qint64 idObject):p1(p1), p2(QPointF()), p3(QPointF()),
    p4(p4), angle1(angle1), angle2(angle2), kAsm1(kAsm1), kAsm2(kAsm2), kCurve(kCurve), points(points),
    _referens(0), mode(mode), idObject(idObject){
    ModifiSpl ( p1, p4, angle1, angle2, kAsm1, kAsm2, kCurve );
}

VSpline::VSpline (const QMap<qint64, VPointF> *points, qint64 p1, QPointF p2, QPointF p3, qint64 p4,
                  qreal kCurve, Draw::Mode mode, qint64 idObject):p1(p1), p2(p2), p3(p3), p4(p4), angle1(0), angle2(0), kAsm1(1), kAsm2(1),
    kCurve(1), points(points), _referens(0), mode(mode), idObject(idObject){
    ModifiSpl ( p1, p2, p3, p4, kCurve);
}

void VSpline::ModifiSpl ( qint64 p1, qint64 p4, qreal angle1, qreal angle2,
                          qreal kAsm1, qreal kAsm2, qreal kCurve){
    this->p1 = p1;
    this->p4 = p4;
    this->angle1 = angle1;
    this->angle2 = angle2;
    this->kAsm1 = kAsm1;
    this->kAsm2 = kAsm2;
    this->kCurve = kCurve;
    QLineF p1pX(GetPointP1().x(), GetPointP1().y(), GetPointP1().x() + 100, GetPointP1().y());
    p1pX.setAngle( angle1 );
    qreal L = 0, radius = 0, angle = 90;
    //    angle = QLineF(GetPointP1(), p1pX.p2()).angleTo(QLineF(GetPointP1(), GetPointP4()));
    //    if ( angle > 180 ){
    //        angle = 360 - angle;
    //    }
    QPointF point1 = GetPointP1().toQPointF();
    QPointF point4 = GetPointP4().toQPointF();
    radius = QLineF(QPointF(point1.x(), point4.y()),point4).length();
    //    radius = QLineF(GetPointP1(), GetPointP4()).length() / 2 / sin( angle * M_PI / 180.0 );
    L = kCurve * radius * 4 / 3 * tan( angle * M_PI / 180.0 / 4 );
    QLineF p1p2(GetPointP1().x(), GetPointP1().y(), GetPointP1().x() + L * kAsm1, GetPointP1().y());
    p1p2.setAngle(angle1);
    QLineF p4p3(GetPointP4().x(), GetPointP4().y(), GetPointP4().x() + L * kAsm2, GetPointP4().y());
    p4p3.setAngle(angle2);
    this->p2 = p1p2.p2();
    this->p3 = p4p3.p2();
}

void VSpline::ModifiSpl (qint64 p1, QPointF p2, QPointF p3, qint64 p4, qreal kCurve){
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->p4 = p4;
    this->angle1 = QLineF ( GetPointP1().toQPointF(), p2 ).angle();
    this->angle2 = QLineF ( GetPointP4().toQPointF(), p3 ).angle();

    QLineF p1pX(GetPointP1().x(), GetPointP1().y(), GetPointP1().x() + 100, GetPointP1().y());
    p1pX.setAngle( angle1 );
    qreal L = 0, radius = 0, angle = 90;
    //    angle = QLineF(GetPointP1(), p1pX.p2()).angleTo(QLineF(GetPointP1(), GetPointP4()));
    //    if ( angle >= 180 ){
    //        angle = 360 - angle;
    //    }
    QPointF point1 = GetPointP1().toQPointF();
    QPointF point4 = GetPointP4().toQPointF();
    radius = QLineF(QPointF(point1.x(), point4.y()),point4).length();
    //    radius = QLineF(GetPointP1(), GetPointP4()).length() / 2 / sin( angle * M_PI / 180.0 );
    L = kCurve * radius * 4 / 3 * tan( angle * M_PI / 180.0 / 4 );

    this->kCurve = kCurve;
    this->kAsm1 = QLineF ( GetPointP1().toQPointF(), p2 ).length()/L;
    this->kAsm2 = QLineF ( GetPointP4().toQPointF(), p3 ).length()/L;
}

//void VSpline::RotationSpl (QPointF pRotate, qreal angle ){
//    QLineF pRotateP1 (pRotate, p1);
//    pRotateP1.setAngle(angle);
//    p1 = pRotateP1.p2();
//    QLineF pRotateP2 (pRotate, p2);
//    pRotateP2.setAngle(angle);
//    p2 = pRotateP2.p2();
//    QLineF pRotateP3 (pRotate, p3);
//    pRotateP3.setAngle(angle);
//    p3 = pRotateP3.p2();
//    QLineF pRotateP4 (pRotate, p4);
//    pRotateP4.setAngle(angle);
//    p4 = pRotateP4.p2();
//    angle1 = QLineF(p1, p2).angle();
//    angle2 = QLineF(p4, p2).angle();
//}

//void VSpline::BiasSpl ( qreal mx, qreal my ){
//    p1 = QPointF(p1.x()+mx, p1.y()+my);
//    p2 = QPointF(p2.x()+mx, p2.y()+my);
//    p3 = QPointF(p3.x()+mx, p3.y()+my);
//    p4 = QPointF(p4.x()+mx, p4.y()+my);
//}

qint64 VSpline::GetP1 () const{
    return p1;
}

VPointF VSpline::GetPointP1() const{
    if(points->contains(p1)){
        return points->value(p1);
    } else {
        qCritical()<<"Не можу знайти id = "<<p1<<" в таблиці.";
        throw"Не можу знайти точку за id.";
    }
    return VPointF();
}

QPointF VSpline::GetP2 () const{
    return p2;
}

QPointF VSpline::GetP3 () const{
    return p3;
}

qint64 VSpline::GetP4() const{
    return p4;
}

VPointF VSpline::GetPointP4() const{
    if(points->contains(p4)){
        return points->value(p4);
    } else {
        qCritical()<<"Не можу знайти id = "<<p4<<" в таблиці.";
        throw"Не можу знайти точку за id.";
    }
    return VPointF();
}

qreal VSpline::GetAngle1() const{
    return angle1;
}

qreal VSpline::GetAngle2 () const{
    return angle2;
}

qreal VSpline::GetLength () const{
    return LengthBezier ( GetPointP1().toQPointF(), this->p2, this->p3, GetPointP4().toQPointF());
}

QString VSpline::GetName() const{
    VPointF first = GetPointP1();
    VPointF second = GetPointP4();
    return QString("Spl_%1_%2").arg(first.name(), second.name());
}

qreal VSpline::GetKasm1() const{
    return kAsm1;
}

qreal VSpline::GetKasm2() const{
    return kAsm2;
}

qreal VSpline::GetKcurve() const{
    return kCurve;
}

const QMap<qint64, VPointF> *VSpline::GetDataPoints() const{
    return points;
}

QLineF::IntersectType VSpline::CrossingSplLine ( const QLineF &line, QPointF *intersectionPoint ) const{
    QVector<qreal> px;
    QVector<qreal> py;
    px.append ( GetPointP1 ().x () );
    py.append ( GetPointP1 ().y () );
    QVector<qreal>& wpx = px;
    QVector<qreal>& wpy = py;
    PointBezier_r ( GetPointP1 ().x (), GetPointP1 ().y (), GetP2 ().x (), GetP2 ().y (),
                    GetP3 ().x (), GetP3 ().y (), GetPointP4 ().x (), GetPointP4 ().y (),
                    0, wpx, wpy);
    px.append ( GetPointP4 ().x () );
    py.append ( GetPointP4 ().y () );
    qint32 i = 0;
    QPointF crosPoint;
    QLineF::IntersectType type = QLineF::NoIntersection;
    for ( i = 0; i < px.count()-1; ++i ){
        type = line.intersect(QLineF ( QPointF ( px[i], py[i] ),
                                       QPointF ( px[i+1], py[i+1] )), &crosPoint);
        if ( type == QLineF::BoundedIntersection ){
            *intersectionPoint = crosPoint;
            return type;
        }
    }
    throw "Не можу знайти точку перетину сплайну з лінією.";
}

//void VSpline::CutSpline ( qreal length, VSpline* curFir, VSpline* curSec ) const{
//    if ( length > GetLength()){
//        throw"Не правильна довжина нового сплайну\n";
//    }
//    qreal parT = length / GetLength();
//    QLineF seg1_2 ( GetPointP1 (), GetP2 () );
//    seg1_2.setLength(seg1_2.length () * parT);
//    QPointF p12 = seg1_2.p2();
//    QLineF seg2_3 ( GetP2 (), GetP3 () );
//    seg2_3.setLength(seg2_3.length () * parT);
//    QPointF p23 = seg2_3.p2();
//    QLineF seg12_23 ( p12, p23 );
//    seg12_23.setLength(seg12_23.length () * parT);
//    QPointF p123 = seg12_23.p2();
//    QLineF seg3_4 ( GetP3 (), GetPointP4 () );
//    seg3_4.setLength(seg3_4.length () * parT);
//    QPointF p34 = seg3_4.p2();
//    QLineF seg23_34 ( p23, p34 );
//    seg23_34.setLength(seg23_34.length () * parT);
//    QPointF p234 = seg23_34.p2();
//    QLineF seg123_234 ( p123, p234 );
//    seg123_234.setLength(seg123_234.length () * parT);
//    QPointF p1234 = seg123_234.p2();
//    curFir->ModifiSpl ( GetPointP1 (), p12, p123, p1234 );
//    curSec->ModifiSpl ( p1234, p234, p34, GetPointP4 () );
//}

//void VSpline::CutSpline ( QPointF point, VSpline* curFir, VSpline* curSec ) const{
//    qreal t = param_t (point);
//    qreal length = t*this->GetLength();
//    CutSpline ( length, curFir, curSec );
//}

//void VSpline::PutAlongSpl (QPointF &moveP, qreal move ) const{
//    if ( GetLength () < move ){
//        qDebug()<<"Довжина більше довжини сплайну.";
//        qDebug()<<GetLength()<<"<"<<move;
//        throw "Довжина більше довжини сплайну.";
//    }
//    if ( move <= 0 ){
//        qDebug()<<"Довжина менше дорівнює нулю.";
//        throw "Довжина менше дорівнює нулю.";
//    }
//    qreal t = 0;
//    if ( move == 0 ){
//        t = 0;
//    } else {
//        t = move / GetLength ();
//        moveP.setX ( pow ( 1 - t, 3 ) * GetPointP1 ().x () + 3 * t * pow ( 1 - t, 2 ) *
//                     GetP2 ().x () + 3 * t * t * ( 1 - t ) * GetP3 ().x () +
//                     pow ( t, 3 ) * GetPointP4 ().x () );
//        moveP.setY ( pow ( 1 - t, 3 ) * GetPointP1 ().y () + 3 * t * pow ( 1 - t, 2 ) *
//                     GetP2 ().y () + 3 * t * t * ( 1 - t ) * GetP3 ().y () +
//                     pow ( t, 3 ) * GetPointP4 ().y () );
//    }
//}

QVector<QPointF> VSpline::GetPoints () const{
    return GetPoints(GetPointP1().toQPointF(), p2, p3, GetPointP4().toQPointF());
//    QLineF line1(points.at(0).toPoint(), points.at(1).toPoint());
//    line1.setLength(500);
//    QLineF line2 = line1;
//    line2.setAngle(line2.angle()+90);
//    qreal xk1 = line1.p2().x();
//    qreal xk0 = line1.p1().x();
//    qreal y = line2.p2().y();
//    qreal yk0 = line1.p1().y();
//    qreal yk1 = line1.p2().y();
//    qreal x = line2.p2().x();
//    qreal check = (xk1 - xk0) * (y - yk0) - (yk1 - yk0) * (x - xk0);
//    if(check > 0){
//        return points;
//    } else {
//        QVector<QPointF> reversePoints;
//        for (qint32 i = points.size() - 1; i >= 0; --i) {
//            reversePoints.append(points.at(i));
//        }
//        return reversePoints;
//    }
}

QVector<QPointF> VSpline::GetPoints (QPointF p1, QPointF p2, QPointF p3, QPointF p4){
    QVector<QPointF> pvector;
    QVector<qreal> x;
    QVector<qreal> y;
    QVector<qreal>& wx = x;
    QVector<qreal>& wy = y;
    x.append ( p1.x () );
    y.append ( p1.y () );
    PointBezier_r ( p1.x (), p1.y (), p2.x (), p2.y (),
                    p3.x (), p3.y (), p4.x (), p4.y (), 0, wx, wy );
    x.append ( p4.x () );
    y.append ( p4.y () );
    for ( qint32 i = 0; i < x.count(); ++i ){
        pvector.append( QPointF ( x[i], y[i] ) );
    }
    return pvector;
}

qreal VSpline::LengthBezier ( QPointF p1, QPointF p2, QPointF p3, QPointF p4 ) const{
    /*QVector<qreal> px;
    QVector<qreal> py;
    QVector<qreal>& wpx = px;
    QVector<qreal>& wpy = py;
    px.append ( p1.x () );
    py.append ( p1.y () );
    PointBezier_r ( p1.x (), p1.y (), p2.x (), p2.y (),
                  p3.x (), p3.y (), p4.x (), p4.y (), 0, wpx, wpy);
    px.append ( p4.x () );
    py.append ( p4.y () );
    qint32 i = 0;
    qreal length = 0.0;
     *
     * Наприклад маємо 10 точок. Від 0 до 9  і останню точку не опрацьовуємо.
     * Тому від 0 до 8(<10-1).
     *
    for ( i = 0; i < px.count() - 1; ++i ){
        length += QLineF ( QPointF ( px[i], py[i] ), QPointF ( px[i+1], py[i+1] ) ).length ();
    }*/
    QPainterPath splinePath;
    QVector<QPointF> points = GetPoints (p1, p2, p3, p4);
    splinePath.moveTo(points[0]);
    for (qint32 i = 1; i < points.count(); ++i){
        splinePath.lineTo(points[i]);
    }
    return splinePath.length();
}

void VSpline::PointBezier_r ( qreal x1, qreal y1, qreal x2, qreal y2,
                              qreal x3, qreal y3, qreal x4, qreal y4,
                              qint16 level, QVector<qreal> &px, QVector<qreal> &py){
    const double curve_collinearity_epsilon              	= 1e-30;
    const double curve_angle_tolerance_epsilon           	= 0.01;
    const double m_angle_tolerance = 0.0;
    enum curve_recursion_limit_e { curve_recursion_limit = 32 };
    const double m_cusp_limit = 0.0;
    double m_approximation_scale = 1.0;
    double m_distance_tolerance_square;
    
    m_distance_tolerance_square = 0.5 / m_approximation_scale;
    m_distance_tolerance_square *= m_distance_tolerance_square;
    
    if(level > curve_recursion_limit)
    {
        return;
    }
    
    // Calculate all the mid-points of the line segments
    //----------------------
    double x12   = (x1 + x2) / 2;
    double y12   = (y1 + y2) / 2;
    double x23   = (x2 + x3) / 2;
    double y23   = (y2 + y3) / 2;
    double x34   = (x3 + x4) / 2;
    double y34   = (y3 + y4) / 2;
    double x123  = (x12 + x23) / 2;
    double y123  = (y12 + y23) / 2;
    double x234  = (x23 + x34) / 2;
    double y234  = (y23 + y34) / 2;
    double x1234 = (x123 + x234) / 2;
    double y1234 = (y123 + y234) / 2;
    
    
    // Try to approximate the full cubic curve by a single straight line
    //------------------
    double dx = x4-x1;
    double dy = y4-y1;
    
    double d2 = fabs(((x2 - x4) * dy - (y2 - y4) * dx));
    double d3 = fabs(((x3 - x4) * dy - (y3 - y4) * dx));
    double da1, da2, k;
    
    switch((static_cast<int>(d2 > curve_collinearity_epsilon) << 1) +
           static_cast<int>(d3 > curve_collinearity_epsilon))
    {
    case 0:
        // All collinear OR p1==p4
        //----------------------
        k = dx*dx + dy*dy;
        if(k == 0)
        {
            d2 = CalcSqDistance(x1, y1, x2, y2);
            d3 = CalcSqDistance(x4, y4, x3, y3);
        }
        else
        {
            k   = 1 / k;
            da1 = x2 - x1;
            da2 = y2 - y1;
            d2  = k * (da1*dx + da2*dy);
            da1 = x3 - x1;
            da2 = y3 - y1;
            d3  = k * (da1*dx + da2*dy);
            if(d2 > 0 && d2 < 1 && d3 > 0 && d3 < 1)
            {
                // Simple collinear case, 1---2---3---4
                // We can leave just two endpoints
                return;
            }
            if(d2 <= 0)
                d2 = CalcSqDistance(x2, y2, x1, y1);
            else if(d2 >= 1)
                d2 = CalcSqDistance(x2, y2, x4, y4);
            else
                d2 = CalcSqDistance(x2, y2, x1 + d2*dx, y1 + d2*dy);
            
            if(d3 <= 0)
                d3 = CalcSqDistance(x3, y3, x1, y1);
            else if(d3 >= 1)
                d3 = CalcSqDistance(x3, y3, x4, y4);
            else
                d3 = CalcSqDistance(x3, y3, x1 + d3*dx, y1 + d3*dy);
        }
        if(d2 > d3)
        {
            if(d2 < m_distance_tolerance_square)
            {
                
                px.append(x2);
                py.append(y2);
                //m_points.add(point_d(x2, y2));
                return;
            }
        }
        else
        {
            if(d3 < m_distance_tolerance_square)
            {
                
                px.append(x3);
                py.append(y3);
                //m_points.add(point_d(x3, y3));
                return;
            }
        }
        break;
    case 1:
        // p1,p2,p4 are collinear, p3 is significant
        //----------------------
        if(d3 * d3 <= m_distance_tolerance_square * (dx*dx + dy*dy))
        {
            if(m_angle_tolerance < curve_angle_tolerance_epsilon)
            {
                
                px.append(x23);
                py.append(y23);
                //m_points.add(point_d(x23, y23));
                return;
            }
            
            // Angle Condition
            //----------------------
            da1 = fabs(atan2(y4 - y3, x4 - x3) - atan2(y3 - y2, x3 - x2));
            if(da1 >= M_PI)
                da1 = 2*M_PI - da1;
            
            if(da1 < m_angle_tolerance)
            {
                
                px.append(x2);
                py.append(y2);
                
                px.append(x3);
                py.append(y3);
                //m_points.add(point_d(x2, y2));
                //m_points.add(point_d(x3, y3));
                return;
            }
            
            if(m_cusp_limit != 0.0)
            {
                if(da1 > m_cusp_limit)
                {
                    
                    px.append(x3);
                    py.append(y3);
                    //m_points.add(point_d(x3, y3));
                    return;
                }
            }
        }
        break;
        
    case 2:
        // p1,p3,p4 are collinear, p2 is significant
        //----------------------
        if(d2 * d2 <= m_distance_tolerance_square * (dx*dx + dy*dy))
        {
            if(m_angle_tolerance < curve_angle_tolerance_epsilon)
            {
                
                px.append(x23);
                py.append(y23);
                //m_points.add(point_d(x23, y23));
                return;
            }
            
            // Angle Condition
            //----------------------
            da1 = fabs(atan2(y3 - y2, x3 - x2) - atan2(y2 - y1, x2 - x1));
            if(da1 >= M_PI) da1 = 2*M_PI - da1;
            
            if(da1 < m_angle_tolerance)
            {
                
                px.append(x2);
                py.append(y2);
                
                px.append(x3);
                py.append(y3);
                //m_points.add(point_d(x2, y2));
                //m_points.add(point_d(x3, y3));
                return;
            }
            
            if(m_cusp_limit != 0.0)
            {
                if(da1 > m_cusp_limit)
                {
                    px.append(x2);
                    py.append(y2);
                    
                    //m_points.add(point_d(x2, y2));
                    return;
                }
            }
        }
        break;
        
    case 3:
        // Regular case
        //-----------------
        if((d2 + d3)*(d2 + d3) <= m_distance_tolerance_square * (dx*dx + dy*dy))
        {
            // If the curvature doesn't exceed the distance_tolerance value
            // we tend to finish subdivisions.
            //----------------------
            if(m_angle_tolerance < curve_angle_tolerance_epsilon)
            {
                
                px.append(x23);
                py.append(y23);
                //m_points.add(point_d(x23, y23));
                return;
            }
            
            // Angle & Cusp Condition
            //----------------------
            k   = atan2(y3 - y2, x3 - x2);
            da1 = fabs(k - atan2(y2 - y1, x2 - x1));
            da2 = fabs(atan2(y4 - y3, x4 - x3) - k);
            if(da1 >= M_PI) da1 = 2*M_PI - da1;
            if(da2 >= M_PI) da2 = 2*M_PI - da2;
            
            if(da1 + da2 < m_angle_tolerance)
            {
                // Finally we can stop the recursion
                //----------------------
                
                px.append(x23);
                py.append(y23);
                //m_points.add(point_d(x23, y23));
                return;
            }
            
            if(m_cusp_limit != 0.0)
            {
                if(da1 > m_cusp_limit)
                {
                    px.append(x2);
                    py.append(y2);
                    return;
                }
                
                if(da2 > m_cusp_limit)
                {
                    px.append(x3);
                    py.append(y3);
                    return;
                }
            }
        }
        break;
    }
    
    // Continue subdivision
    //----------------------
    PointBezier_r(x1, y1, x12, y12, x123, y123, x1234, y1234, static_cast<qint16>(level + 1), px, py);
    PointBezier_r(x1234, y1234, x234, y234, x34, y34, x4, y4, static_cast<qint16>(level + 1), px, py);
}

qreal VSpline::CalcSqDistance (qreal x1, qreal y1, qreal x2, qreal y2){
    qreal dx = x2 - x1;
    qreal dy = y2 - y1;
    return dx * dx + dy * dy;
}

QPainterPath VSpline::GetPath() const{
    QPainterPath splinePath;
    QVector<QPointF> points = GetPoints ();
    splinePath.moveTo(points[0]);
    for (qint32 i = 1; i < points.count(); ++i){
        splinePath.lineTo(points[i]);
    }
    return splinePath;
}

qint32 VSpline::referens() const{
    return _referens;
}

void VSpline::incrementReferens(){
    ++_referens;
}

void VSpline::decrementReferens(){
    if(_referens > 0){
        --_referens;
    }
}

/* Cubic equation solution. Real coefficients case.
  
   int Cubic(double *x,double a,double b,double c);
   Parameters:
   x - solution array (size 3). On output:
       3 real roots -> then x is filled with them;
       1 real + 2 complex -> x[0] is real, x[1] is real part of
                             complex roots, x[2] - non-negative
                             imaginary part.
   a, b, c - coefficients, as described
   Returns: 3 - 3 real roots;
            1 - 1 real root + 2 complex;
            2 - 1 real root + complex roots imaginary part is zero
                (i.e. 2 real roots).
*/
qint32 VSpline::Cubic(qreal *x, qreal a, qreal b, qreal c){
    qreal q,r,r2,q3;
    
    q = (a*a - 3.*b)/9.;
    r = (a*(2.*a*a - 9.*b) + 27.*c)/54.;
    r2 = r*r;
    q3 = pow(q,3);
    if(r2<q3) {
        qreal t = acos(r/sqrt(q3));
        a/=3.;
        q = -2.*sqrt(q);
        x[0] = q*cos(t/3.)-a;
        x[1] = q*cos((t + M_2PI)/3.) - a;
        x[2] = q*cos((t - M_2PI)/3.) - a;
        return(3);
    } else {
        qreal aa,bb;
        if(r<=0.){
            r=-r;
        }
        aa = -pow(r + sqrt(r2-q3),1./3.);
        if(aa!=0.){
            bb=q/aa;
        } else {
            bb=0.;
        }
        a/=3.;
        q = aa+bb;
        r = aa-bb;
        x[0] = q-a;
        x[1] = (-0.5)*q-a;
        x[2] = (sqrt(3.)*0.5)*fabs(r);
        if(x[2]==0.){
            return(2);
        }
        return(1);
    }
}

//qreal VSpline::calc_t (qreal curve_coord1, qreal curve_coord2, qreal curve_coord3,
//                       qreal curve_coord4, qreal point_coord) const{
//    qreal P1, P2, P3, P4, Bt;
//    qreal a, b, c, d, ret_t;
    
//    qreal *t = static_cast<qreal *>(malloc(3*sizeof(qreal)));
//    P1 = curve_coord1;
//    P2 = curve_coord2;
//    P3 = curve_coord3;
//    P4 = curve_coord4;
//    Bt = point_coord;
    
//    a = -P1 + 3*P2 - 3*P3 + P4;
//    b = 3*P1 - 6*P2 + 3*P3;
//    c = -3*P1 + 3*P2;
//    d = -Bt + P1;
    
//    if(Cubic(t, b/a, c/a, d/a) == 3){
//        ret_t = t[2];
//    } else {
//        ret_t = t[0];
//    }
//    /*
//      * Повертається три значення, але експереминтально знайдено що шукане
//      * значення знаходиться в третьому.
//      */
    
//    free(t);
//    if(ret_t<0 || ret_t>1){
//        qDebug()<<"Неправильне значення параметра. фунція calc_t";
//        throw"Неправильне значення параметра. фунція calc_t";
//    }
//    return ret_t;
//}
/*
  * Функція знаходить підходяще значення параметна t якому відповідає точка на сплайні.
  */
//qreal VSpline::param_t (QPointF pBt)const{
//    qreal t_x, t_y;
//    t_x = calc_t (GetPointP1().x(), p2.x(), p3.x(), GetPointP4().x(), pBt.x());
//    t_y = calc_t (GetPointP1().y(), p2.y(), p3.y(), GetPointP4().y(), pBt.y());
//    /*
//      * Порівнюємо значення по х і по у і визначаємо найбільше. Це значення і
//      * буде шуканим.
//      */
//    if(t_x>t_y)
//        return t_x;
//    else
//        return t_y;
//}

//void VSpline::Mirror(const QPointF Pmirror){
//    QPointF P1 = p1;
//    P1 = QPointF(P1.x() - Pmirror.x(), P1.y() - Pmirror.y());
//    P1 = QPointF(P1.x() * -1.0, P1.y() * 1.0);
//    P1 = QPointF(P1.x() + Pmirror.x(), P1.y() + Pmirror.y());
//    QPointF P2 = p2;
//    P2 = QPointF(P2.x() - Pmirror.x(), P2.y() - Pmirror.y());
//    P2 = QPointF(P2.x() * -1.0, P2.y() * 1.0);
//    P2 = QPointF(P2.x() + Pmirror.x(), P2.y() + Pmirror.y());
//    QPointF P3 = p3;
//    P3 = QPointF(P3.x() - Pmirror.x(), P3.y() - Pmirror.y());
//    P3 = QPointF(P3.x() * -1.0, P3.y() * 1.0);
//    P3 = QPointF(P3.x() + Pmirror.x(), P3.y() + Pmirror.y());
//    QPointF P4 = p4;
//    P4 = QPointF(P4.x() - Pmirror.x(), P4.y() - Pmirror.y());
//    P4 = QPointF(P4.x() * -1.0, P4.y() * 1.0);
//    P4 = QPointF(P4.x() + Pmirror.x(), P4.y() + Pmirror.y());
//    this->ModifiSpl(P1, P2, P3, P4);
//}

Draw::Mode VSpline::getMode() const{
    return mode;
}

void VSpline::setMode(const Draw::Mode &value){
    mode = value;
}

QVector<QPointF> VSpline::SplinePoints(QPointF p1, QPointF p4, qreal angle1, qreal angle2, qreal kAsm1,
                                       qreal kAsm2, qreal kCurve){
    QLineF p1pX(p1.x(), p1.y(), p1.x() + 100, p1.y());
    p1pX.setAngle( angle1 );
    qreal L = 0, radius = 0, angle = 90;
    radius = QLineF(QPointF(p1.x(), p4.y()),p4).length();
    L = kCurve * radius * 4 / 3 * tan( angle * M_PI / 180.0 / 4 );
    QLineF p1p2(p1.x(), p1.y(), p1.x() + L * kAsm1, p1.y());
    p1p2.setAngle(angle1);
    QLineF p4p3(p4.x(), p4.y(), p4.x() + L * kAsm2, p4.y());
    p4p3.setAngle(angle2);
    QPointF p2 = p1p2.p2();
    QPointF p3 = p4p3.p2();
    return GetPoints(p1, p2, p3, p4);
}

qint64 VSpline::getIdObject() const{
    return idObject;
}

void VSpline::setIdObject(const qint64 &value){
    idObject = value;
}

VSpline &VSpline::operator =(const VSpline &spline){
    this->p1 = spline.GetP1 ();
    this->p2 = spline.GetP2 ();
    this->p3 = spline.GetP3 ();
    this->p4 = spline.GetP4 ();
    this->angle1 = spline.GetAngle1 ();
    this->angle2 = spline.GetAngle2 ();
    this->kAsm1 = spline.GetKasm1();
    this->kAsm2 = spline.GetKasm2();
    this->kCurve = spline.GetKcurve();
    this->points = spline.GetDataPoints();
    this->_referens = 0;
    this->mode = spline.getMode();
    this->idObject = spline.getIdObject();
    return *this;
}
