/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vpiecenode.h"
#include "vpiecenode_p.h"

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode()
    : d(new VPieceNodeData)
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode(quint32 id, Tool typeTool, bool reverse)
    : d(new VPieceNodeData(id, typeTool, reverse))
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode(const VPieceNode &node)
    : d (node.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode &VPieceNode::operator=(const VPieceNode &node)
{
    if ( &node == this )
    {
        return *this;
    }
    d = node.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::~VPieceNode()
{}

//---------------------------------------------------------------------------------------------------------------------
quint32 VPieceNode::GetId() const
{
    return d->id;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetId(quint32 id)
{
    d->id = id;
}

//---------------------------------------------------------------------------------------------------------------------
Tool VPieceNode::GetTypeTool() const
{
    return d->typeTool;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetTypeTool(Tool value)
{
    d->typeTool = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::GetReverse() const
{
    return d->reverse;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetReverse(bool reverse)
{
    if (d->typeTool != Tool::NodePoint)
    {
        d->reverse = reverse;
    }
}
