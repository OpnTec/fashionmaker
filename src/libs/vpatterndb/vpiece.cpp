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

#include "vpiece.h"
#include "vpiece_p.h"

//---------------------------------------------------------------------------------------------------------------------
VPiece::VPiece()
    : d(new VPieceData)
{}

//---------------------------------------------------------------------------------------------------------------------
VPiece::VPiece(const VPiece &piece)
    : d (piece.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPiece &VPiece::operator=(const VPiece &piece)
{
    if ( &piece == this )
    {
        return *this;
    }
    d = piece.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPiece::~VPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief append append in the end of list node.
 * @param node new node.
 */
void VPiece::Append(const VPieceNode &node)
{
    d->nodes.append(node);
}

//---------------------------------------------------------------------------------------------------------------------
/** @brief Clear detail full clear. */
void VPiece::Clear()
{
    ClearNodes();
}

//---------------------------------------------------------------------------------------------------------------------
/** @brief ClearNodes clear list of nodes. */
void VPiece::ClearNodes()
{
    d->nodes.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CountNode return count nodes.
 * @return count.
 */
qint32 VPiece::CountNode() const
{
    return d->nodes.size();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator [] find node by index in list.
 * @param indx index node in list.
 * @return node
 */
VPieceNode &VPiece::operator [](int indx)
{
    return d->nodes[indx];
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief at find node by index in list.
 * @param indx index node in list.
 * @return const node.
 */
const VPieceNode &VPiece::at(int indx) const
{
    return d->nodes.at(indx);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getNodes return list of nodes.
 * @return list of nodes.
 */
QVector<VPieceNode> VPiece::GetNodes() const
{
    return d->nodes;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setNodes set list of nodes
 * @param value list of nodes
 */
// cppcheck-suppress unusedFunction
void VPiece::SetNodes(const QVector<VPieceNode> &nodes)
{
    d->nodes = nodes;
}
