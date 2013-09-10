#include "vdetail.h"

VDetail::VDetail():nodes(QVector<VNodeDetail>()),name(QString()), mx(0), my(0){
}

VDetail::VDetail(const QString &name, const QVector<VNodeDetail> &nodes):nodes(QVector<VNodeDetail>()),
    name(name), mx(0), my(0){
    this->nodes = nodes;
}

void VDetail::append(const VNodeDetail &node){
    nodes.append(node);
}

void VDetail::Clear(){
    nodes.clear();
    name.clear();
    mx = 0;
    my = 0;
}

qint32 VDetail::CountNode() const{
    return nodes.size();
}

bool VDetail::Containes(const qint64 &id) const{
    for(qint32 i = 0; i < nodes.size(); ++i){
        VNodeDetail node = nodes[i];
        if(node.getId() == id){
            return true;
        }
    }
    return false;
}

VNodeDetail &VDetail::operator [](int indx){
    return nodes[indx];
}

qreal VDetail::getMy() const{
    return my;
}

void VDetail::setMy(const qreal &value){
    my = value;
}

qreal VDetail::getMx() const{
    return mx;
}

void VDetail::setMx(const qreal &value){
    mx = value;
}

QString VDetail::getName() const{
    return name;
}

void VDetail::setName(const QString &value){
    name = value;
}
