#include "vpointf.h"

VPointF::VPointF():_name(QString()), _mx(0), _my(0), _x(0), _y(0), _referens(0), mode(Draw::Calculation),
    idObject(0){
}

VPointF::VPointF ( const VPointF & point ):_name(point.name()), _mx(point.mx()), _my(point.my()),
    _x(point.x()), _y(point.y()), _referens(0), mode(point.getMode()), idObject(point.getIdObject()){
}

VPointF::VPointF (qreal x, qreal y , QString name, qreal mx, qreal my, Draw::Mode mode, qint64 idObject):_name(name), _mx(mx),
    _my(my), _x(x), _y(y), _referens(0), mode(mode), idObject(idObject){
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

qint32 VPointF::referens() const{
    return _referens;
}

void VPointF::incrementReferens(){
    ++_referens;
}

void VPointF::decrementReferens(){
    if(_referens > 0){
        --_referens;
    }
}

Draw::Mode VPointF::getMode() const
{
    return mode;
}

void VPointF::setMode(const Draw::Mode &value)
{
    mode = value;
}

qint64 VPointF::getIdObject() const{
    return idObject;
}

void VPointF::setIdObject(const qint64 &value){
    idObject = value;
}
