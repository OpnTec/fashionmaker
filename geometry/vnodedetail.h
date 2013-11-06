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

#ifndef VNODEDETAIL_H
#define VNODEDETAIL_H

#include <QMetaType>
#include "../options.h"

namespace NodeDetail
{
    enum NodeDetail { Contour, Modeling };
    Q_DECLARE_FLAGS(NodeDetails, NodeDetail)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(NodeDetail::NodeDetails)

class VNodeDetail
{
public:
                  VNodeDetail();
                  VNodeDetail(qint64 id, Tool::Tools typeTool, Draw::Draws mode, NodeDetail::NodeDetails typeNode,
                              qreal mx = 0, qreal my = 0);
                  VNodeDetail(const VNodeDetail &node);
                  VNodeDetail &operator=(const VNodeDetail &node);
    inline qint64 getId() const {return id;}
    inline void   setId(const qint64 &value) {id = value;}
    inline Tool::Tools getTypeTool() const {return typeTool;}
    inline void   setTypeTool(const Tool::Tools &value) {typeTool = value;}
    inline Draw::Draws getMode() const {return mode;}
    inline void   setMode(const Draw::Draws &value) {mode = value;}
    inline NodeDetail::NodeDetails getTypeNode() const {return typeNode;}
    inline void   setTypeNode(const NodeDetail::NodeDetails &value) {typeNode = value;}
    inline qreal  getMx() const {return mx;}
    inline void   setMx(const qreal &value) {mx = value;}
    inline qreal  getMy() const {return my;}
    inline void   setMy(const qreal &value) {my = value;}
private:
    qint64        id;
    Tool::Tools   typeTool;
    Draw::Draws   mode;
    NodeDetail::NodeDetails typeNode;
    qreal         mx;
    qreal         my;
};

Q_DECLARE_METATYPE(VNodeDetail)

#endif // VNODEDETAIL_H
