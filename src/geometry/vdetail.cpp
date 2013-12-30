/************************************************************************
 **
 **  @file   vdetail.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include "vdetail.h"

VDetail::VDetail()
    :QObject(), _id(0), nodes(QVector<VNodeDetail>()), name(QString()), mx(0), my(0), supplement(true), closed(true),
      width(10){}

VDetail::VDetail(const QString &name, const QVector<VNodeDetail> &nodes)
    :QObject(), _id(0), nodes(QVector<VNodeDetail>()), name(name), mx(0), my(0), supplement(true), closed(true),
      width(10)
{
    this->nodes = nodes;
}

VDetail::VDetail(const VDetail &detail)
    :QObject(), _id(0), nodes(detail.getNodes()), name(detail.getName()), mx(detail.getMx()), my(detail.getMy()),
      supplement(detail.getSupplement()), closed(detail.getClosed()), width(detail.getWidth()){}

VDetail &VDetail::operator =(const VDetail &detail)
{
    _id = detail.id();
    nodes = detail.getNodes();
    name = detail.getName();
    mx = detail.getMx();
    my = detail.getMy();
    supplement = detail.getSupplement();
    closed = detail.getClosed();
    width = detail.getWidth();
    return *this;
}

void VDetail::Clear()
{
    nodes.clear();
    name.clear();
    mx = 0;
    my = 0;
    supplement = true;
    closed = true;
    width = 10;
}

bool VDetail::Containes(const qint64 &id) const
{
    for (ptrdiff_t i = 0; i < nodes.size(); ++i)
    {
        VNodeDetail node = nodes[i];
        if (node.getId() == id)
        {
            return true;
        }
    }
    return false;
}

VNodeDetail &VDetail::operator [](ptrdiff_t indx)
{
    return nodes[indx];
}

const VNodeDetail &VDetail::at(ptrdiff_t indx) const
{
    return nodes[indx];
}

ptrdiff_t VDetail::indexOfNode(const qint64 &id) const
{
    for (ptrdiff_t i = 0; i < nodes.size(); ++i)
    {
        VNodeDetail node = nodes[i];
        if (node.getId() == id)
        {
            return i;
        }
    }
    return -1;
}
qint64 VDetail::id() const
{
    return _id;
}

void VDetail::setId(const qint64 &id)
{
    _id = id;
}

