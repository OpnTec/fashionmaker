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

namespace NodeDetail{
    enum Type
    {
        Contour,
        Modeling
    };
}

class VNodeDetail
{
public:
    VNodeDetail();
    VNodeDetail(qint64 id, Tools::Enum typeTool, Draw::Mode mode, NodeDetail::Type typeNode);
    VNodeDetail(const VNodeDetail &node);
    qint64 getId() const;
    void setId(const qint64 &value);
    Tools::Enum getTypeTool() const;
    void setTypeTool(const Tools::Enum &value);
    Draw::Mode getMode() const;
    void setMode(const Draw::Mode &value);
    NodeDetail::Type getTypeNode() const;
    void setTypeNode(const NodeDetail::Type &value);

private:
    qint64 id;
    Tools::Enum typeTool;
    Draw::Mode mode;
    NodeDetail::Type typeNode;
};

Q_DECLARE_METATYPE(VNodeDetail)

#endif // VNODEDETAIL_H
