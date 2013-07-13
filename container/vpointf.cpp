#include "vpointf.h"

VPointF::VPointF():QPointF(){
    _mx = 0;
    _my = 0;
}

VPointF::VPointF ( const VPointF & point ):QPointF(point){
    _name = point.name();
    _mx = point.mx();
    _my = point.my();
}

VPointF::VPointF (qreal x, qreal y , QString name, qreal mx, qreal my):QPointF(x, y){
    _name = name;
    _mx = mx;
    _my = my;
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
    return QPointF(this->x(), this->y());
}
