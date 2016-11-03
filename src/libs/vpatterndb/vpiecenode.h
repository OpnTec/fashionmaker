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

#ifndef VPIECENODE_H
#define VPIECENODE_H

#include <QtGlobal>
#include <QSharedDataPointer>
#include <QMetaType>

#include "../vmisc/def.h"

class VPieceNodeData;

class VPieceNode
{
public:
    VPieceNode();
    VPieceNode(quint32 id, Tool typeTool, bool reverse = false);
    VPieceNode(const VPieceNode &node);
    VPieceNode &operator=(const VPieceNode &node);
    ~VPieceNode();

    quint32 GetId() const;
    void    SetId(quint32 id);

    Tool GetTypeTool() const;
    void SetTypeTool(Tool value);

    bool GetReverse() const;
    void SetReverse(bool reverse);
private:
    QSharedDataPointer<VPieceNodeData> d;
};

Q_DECLARE_METATYPE(VPieceNode)
Q_DECLARE_TYPEINFO(VPieceNode, Q_MOVABLE_TYPE);

#endif // VPIECENODE_H
