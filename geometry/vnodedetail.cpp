/************************************************************************
 **
 **  @file vnodedetail.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
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
 *************************************************************************/

#include "vnodedetail.h"

VNodeDetail::VNodeDetail()
    :id(0), typeTool(Tool::NodePoint), mode(Draw::Modeling), typeNode(NodeDetail::Contour), mx(0), my(0){}

VNodeDetail::VNodeDetail(qint64 id, Tool::Tools typeTool, Draw::Draws mode, NodeDetail::NodeDetails typeNode,
                         qreal mx, qreal my)
    :id(id), typeTool(typeTool), mode(mode), typeNode(typeNode), mx(mx), my(my){}

VNodeDetail::VNodeDetail(const VNodeDetail &node)
    :id(node.getId()), typeTool(node.getTypeTool()), mode(node.getMode()), typeNode(node.getTypeNode()),
      mx(node.getMx()), my(node.getMy()){}

VNodeDetail &VNodeDetail::operator =(const VNodeDetail &node)
{
    id = node.getId();
    typeTool = node.getTypeTool();
    mode = node.getMode();
    typeNode = node.getTypeNode();
    mx = node.getMx();
    my = node.getMy();
    return *this;
}
