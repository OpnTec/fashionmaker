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
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vdetail.h"

VDetail::VDetail():nodes(QVector<VNodeDetail>()),name(QString()), mx(0), my(0), supplement(true), closed(true),
    width(10){
}

VDetail::VDetail(const QString &name, const QVector<VNodeDetail> &nodes):nodes(QVector<VNodeDetail>()),
    name(name), mx(0), my(0), supplement(true), closed(true), width(10){
    this->nodes = nodes;
}

VDetail::VDetail(const VDetail &detail):nodes(detail.getNodes()), name(detail.getName()), mx(detail.getMx()),
    my(detail.getMy()), supplement(detail.getSupplement()), closed(detail.getClosed()), width(detail.getWidth()){
}

VDetail &VDetail::operator =(const VDetail &detail){
    nodes = detail.getNodes();
    name = detail.getName();
    mx = detail.getMx();
    my = detail.getMy();
    supplement = detail.getSupplement();
    closed = detail.getClosed();
    width = detail.getWidth();
    return *this;
}

void VDetail::Clear(){
    nodes.clear();
    name.clear();
    mx = 0;
    my = 0;
    supplement = true;
    closed = true;
    width = 10;
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

