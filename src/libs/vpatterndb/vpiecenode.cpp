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

#include <QDataStream>

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
    return d->m_id;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetId(quint32 id)
{
    d->m_id = id;
}

//---------------------------------------------------------------------------------------------------------------------
Tool VPieceNode::GetTypeTool() const
{
    return d->m_typeTool;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetTypeTool(Tool value)
{
    d->m_typeTool = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::GetReverse() const
{
    return d->m_reverse;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetReverse(bool reverse)
{
    if (d->m_typeTool != Tool::NodePoint)
    {
        d->m_reverse = reverse;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSABefore() const
{
    return d->m_saBefore;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSABefore(Unit unit) const
{
    qreal value = d->m_saBefore;
    if (value >= 0)
    {
        value = ToPixel(value, unit);
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetSABefore(qreal value)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        value < 0 ? d->m_saBefore = -1: d->m_saBefore = value;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSAAfter() const
{
    return d->m_saAfter;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSAAfter(Unit unit) const
{
    qreal value = d->m_saAfter;
    if (value >= 0)
    {
        value = ToPixel(value, unit);
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetSAAfter(qreal value)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        value < 0 ? d->m_saAfter = -1: d->m_saAfter = value;
    }
}

//---------------------------------------------------------------------------------------------------------------------
PieceNodeAngle VPieceNode::GetAngleType() const
{
    return d->m_angleType;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetAngleType(PieceNodeAngle type)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        d->m_angleType = type;
    }
}

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator<<(QDataStream& out, const VPieceNode& p)
{
    out << p.d->m_id << static_cast<int>(p.d->m_typeTool) << p.d->m_reverse;
    return out;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator>>(QDataStream& in, VPieceNode& p)
{
    in >> p.d->m_id;

    int type = 0;
    in >> type;
    p.d->m_typeTool = static_cast<Tool>(type);

    in >> p.d->m_reverse;
    return in;
}
