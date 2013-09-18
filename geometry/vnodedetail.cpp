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

#include "vnodedetail.h"

VNodeDetail::VNodeDetail():id(0), typeTool(Tools::NodePoint), mode(Draw::Calculation),
    typeNode(NodeDetail::Contour){
}

VNodeDetail::VNodeDetail(qint64 id, Tools::Enum typeTool, Draw::Mode mode, NodeDetail::Type typeNode):id(id),
    typeTool(typeTool), mode(mode), typeNode(typeNode){
}

VNodeDetail::VNodeDetail(const VNodeDetail &node):id(node.getId()), typeTool(node.getTypeTool()),
    mode(node.getMode()), typeNode(node.getTypeNode()){
}

Tools::Enum VNodeDetail::getTypeTool() const{
    return typeTool;
}

void VNodeDetail::setTypeTool(const Tools::Enum &value){
    typeTool = value;
}

qint64 VNodeDetail::getId() const{
    return id;
}

void VNodeDetail::setId(const qint64 &value){
    id = value;
}

Draw::Mode VNodeDetail::getMode() const{
    return mode;
}

void VNodeDetail::setMode(const Draw::Mode &value){
    mode = value;
}

NodeDetail::Type VNodeDetail::getTypeNode() const{
    return typeNode;
}

void VNodeDetail::setTypeNode(const NodeDetail::Type &value){
    typeNode = value;
}
