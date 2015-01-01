/************************************************************************
 **
 **  @file   vnodedetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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
#include "vnodedetail_p.h"

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::VNodeDetail()
    :d(new VNodeDetailData)
{}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::VNodeDetail(quint32 id, Tool typeTool, NodeDetail typeNode, qreal mx, qreal my, bool reverse)
    :d(new VNodeDetailData(id, typeTool, typeNode, mx, my, reverse))
{}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::VNodeDetail(const VNodeDetail &node)
    :d (node.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail &VNodeDetail::operator =(const VNodeDetail &node)
{
    if ( &node == this )
    {
        return *this;
    }
    d = node.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::~VNodeDetail()
{}

//---------------------------------------------------------------------------------------------------------------------
quint32 VNodeDetail::getId() const
{
    return d->id;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setId(const quint32 &value)
{
    d->id = value;
}

//---------------------------------------------------------------------------------------------------------------------
Tool VNodeDetail::getTypeTool() const
{
    return d->typeTool;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setTypeTool(const Tool &value)
{
    d->typeTool = value;
}

//---------------------------------------------------------------------------------------------------------------------
NodeDetail VNodeDetail::getTypeNode() const
{
    return d->typeNode;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setTypeNode(const NodeDetail &value)
{
    d->typeNode = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VNodeDetail::getMx() const
{
    return d->mx;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setMx(const qreal &value)
{
    d->mx = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VNodeDetail::getMy() const
{
    return d->my;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setMy(const qreal &value)
{
    d->my = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VNodeDetail::getReverse() const
{
    if (getTypeTool() == Tool::NodePoint)
    {
        return false;
    }
    else
    {
        return d->reverse;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setReverse(bool reverse)
{
    if (getTypeTool() == Tool::NodePoint)
    {
        d->reverse = false;
    }
    else
    {
        d->reverse = reverse;
    }
}
