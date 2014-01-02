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
    :_id(0), nodes(QVector<VNodeDetail>()), name(QString()), mx(0), my(0), supplement(true), closed(true),
      width(10){}

VDetail::VDetail(const QString &name, const QVector<VNodeDetail> &nodes)
    :_id(0), nodes(QVector<VNodeDetail>()), name(name), mx(0), my(0), supplement(true), closed(true),
      width(10)
{
    this->nodes = nodes;
}

VDetail::VDetail(const VDetail &detail)
    :_id(0), nodes(detail.getNodes()), name(detail.getName()), mx(detail.getMx()), my(detail.getMy()),
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

bool VDetail::OnEdge(const qint64 &p1, const qint64 &p2) const
{
    ptrdiff_t i = indexOfNode(p1);
    ptrdiff_t j1 = 0, j2 = 0;

    if (i == nodes.size() - 1)
    {
        j1 = i-1;
        j2 = 0;
    }
    else if (i == 0)
    {
        j1 = nodes.size() - 1;
        j2 = i + 1;
    }
    else
    {
        j1 = i - 1;
        j2 = i + 1;
    }

    if (nodes.at(j1).getId() == p2 || nodes.at(j2).getId() == p2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

ptrdiff_t VDetail::Edge(const qint64 &p1, const qint64 &p2) const
{
    if (OnEdge(p1, p2) == false)
    {
        qWarning()<<"Points don't on edge.";
        return -1;
    }

    ptrdiff_t i = indexOfNode(p1);
    ptrdiff_t j = indexOfNode(p2);

    ptrdiff_t min = qMin(i, j);

    if (min == 0 && (i == nodes.size() - 1 || j == nodes.size() - 1))
    {
        return nodes.size() - 1;
    }
    else
    {
        return min;
    }
}

void VDetail::NodeOnEdge(const ptrdiff_t &index, VNodeDetail &p1, VNodeDetail &p2) const
{
    if (index <= 0 || index > nodes.size())
    {
        qWarning()<<"Wrong edge index";
        return;
    }
    p1 = nodes.at(index);
    if (index + 1 > nodes.size() - 1)
    {
        p2 = nodes.at(0);
    }
    else
    {
        p2 = nodes.at(index+1);
    }
}
