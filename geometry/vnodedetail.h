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

#ifndef VNODEDETAIL_H
#define VNODEDETAIL_H

#include <QtGlobal>
#include <QMetaType>
#include "options.h"

namespace NodeDetail {
    enum NodeDetail { Contour, Modeling };
    Q_DECLARE_FLAGS(NodeDetails, NodeDetail)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(NodeDetail::NodeDetails)

class VNodeDetail
{
public:
    VNodeDetail();
    VNodeDetail(qint64 id, Tool::Tools typeTool, Draw::Draws mode, NodeDetail::NodeDetails typeNode);
    VNodeDetail(const VNodeDetail &node);
    qint64 getId() const;
    void setId(const qint64 &value);
    Tool::Tools getTypeTool() const;
    void setTypeTool(const Tool::Tools &value);
    Draw::Draws getMode() const;
    void setMode(const Draw::Draws &value);
    NodeDetail::NodeDetails getTypeNode() const;
    void setTypeNode(const NodeDetail::NodeDetails &value);

private:
    qint64 id;
    Tool::Tools typeTool;
    Draw::Draws mode;
    NodeDetail::NodeDetails typeNode;
};

Q_DECLARE_METATYPE(VNodeDetail)

#endif // VNODEDETAIL_H
