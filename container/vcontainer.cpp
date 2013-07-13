#include "vcontainer.h"
#include <QDebug>

VContainer::VContainer(){
    _id = 0;
}

VPointF VContainer::GetPoint(qint64 id) const{
    if(points.contains(id)){
        return points.value(id);
    } else {
        qCritical()<<"Не можу знайти id = "<<id<<" в таблиці.";
        throw"Не можу знайти точку за id.";
    }
    return VPointF();
}

qint64 VContainer::AddPoint(const VPointF& point){
    qint64 id = getNextId();
    points[id] = point;
    return id;
}

qint64 VContainer::getId(){
    return _id;
}

qint64 VContainer::getNextId(){
    ++_id;
    return _id;
}

void VContainer::UpdatePoint(qint64 id, const VPointF& point){
    points[id] = point;
    if(id > _id){
        _id = id;
    }
}

void VContainer::Clear(){
    _id = 0;
    points.clear();
}
