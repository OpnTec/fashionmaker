#include "vpointf.h"

VPointF::VPointF():_name(QString()), _mx(0), _my(0), _x(0), _y(0){
}

VPointF::VPointF ( const VPointF & point ):_name(point.name()), _mx(point.mx()), _my(point.my()),
    _x(point.x()), _y(point.y()){
}

VPointF::VPointF (qreal x, qreal y , QString name, qreal mx, qreal my):_name(name), _mx(mx), _my(my), _x(x),
    _y(y){
}

VPointF::~VPointF(){
}

QString VPointF::name() const{
    return _name;
}

qreal VPointF::mx() const{
    return _mx;
}

qreal VPointF::my() const{
    return _my;
}

void VPointF::setName(const QString& name){
    _name = name;
}

void VPointF::setMx(qreal mx){
    _mx = mx;
}

void VPointF::setMy(qreal my){
    _my = my;
}

QPointF VPointF::toQPointF()const{
    return QPointF(_x, _y);
}

qreal VPointF::y() const{
    return _y;
}

void VPointF::setY(const qreal &value){
    _y = value;
}

qreal VPointF::x() const{
    return _x;
}

void VPointF::setX(const qreal &value){
    _x = value;
}
