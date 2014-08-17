/************************************************************************
 **
 **  @file   vdetail.cpp
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

#include "vdetail.h"
#include <QDebug>
#include <QString>

//---------------------------------------------------------------------------------------------------------------------
VDetail::VDetail()
    :_id(NULL_ID), nodes(QVector<VNodeDetail>()), name(QString()), mx(0), my(0), seamAllowance(true), closed(true),
      width(0)
{}

//---------------------------------------------------------------------------------------------------------------------
VDetail::VDetail(const QString &name, const QVector<VNodeDetail> &nodes)
    :_id(NULL_ID), nodes(QVector<VNodeDetail>()), name(name), mx(0), my(0), seamAllowance(true), closed(true),
      width(0)
{
    this->nodes = nodes;
}

//---------------------------------------------------------------------------------------------------------------------
VDetail::VDetail(const VDetail &detail)
    :_id(NULL_ID), nodes(detail.getNodes()), name(detail.getName()), mx(detail.getMx()), my(detail.getMy()),
      seamAllowance(detail.getSeamAllowance()), closed(detail.getClosed()), width(detail.getWidth())
{}

//---------------------------------------------------------------------------------------------------------------------
VDetail &VDetail::operator =(const VDetail &detail)
{
    if ( &detail == this )
    {
        return *this;
    }
    _id = detail.id();
    nodes = detail.getNodes();
    name = detail.getName();
    mx = detail.getMx();
    my = detail.getMy();
    seamAllowance = detail.getSeamAllowance();
    closed = detail.getClosed();
    width = detail.getWidth();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
void VDetail::Clear()
{
    nodes.clear();
    name.clear();
    mx = 0;
    my = 0;
    seamAllowance = true;
    closed = true;
    width = 0;
}

//---------------------------------------------------------------------------------------------------------------------
void VDetail::ClearNodes()
{
    nodes.clear();
}

//---------------------------------------------------------------------------------------------------------------------
bool VDetail::Containes(const quint32 &id) const
{
    for (ptrdiff_t i = 0; i < nodes.size(); ++i)
    {
        VNodeDetail node = nodes.at(i);
        if (node.getId() == id)
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail &VDetail::operator [](ptrdiff_t indx)
{
    return nodes[indx];
}

//---------------------------------------------------------------------------------------------------------------------
const VNodeDetail &VDetail::at(ptrdiff_t indx) const
{
    return nodes.at(indx);
}

//---------------------------------------------------------------------------------------------------------------------
ptrdiff_t VDetail::indexOfNode(const quint32 &id) const
{
    return indexOfNode(nodes, id);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VDetail::id() const
{
    return _id;
}

//---------------------------------------------------------------------------------------------------------------------
void VDetail::setId(const quint32 &id)
{
    _id = id;
}

//---------------------------------------------------------------------------------------------------------------------
bool VDetail::OnEdge(const quint32 &p1, const quint32 &p2) const
{
    QVector<VNodeDetail> list = listNodePoint();
    if (list.size() < 3)
    {
        qDebug()<<"Not enough points.";
        return false;
    }
    ptrdiff_t i = indexOfNode(list, p1);
    ptrdiff_t j1 = 0, j2 = 0;

    if (i == list.size() - 1)
    {
        j1 = i-1;
        j2 = 0;
    }
    else if (i == 0)
    {
        j1 = list.size() - 1;
        j2 = i + 1;
    }
    else
    {
        j1 = i - 1;
        j2 = i + 1;
    }

    if (list.at(j1).getId() == p2 || list.at(j2).getId() == p2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
ptrdiff_t VDetail::Edge(const quint32 &p1, const quint32 &p2) const
{
    if (OnEdge(p1, p2) == false)
    {
        qDebug()<<"Points don't on edge.";
        return -1;
    }

    QVector<VNodeDetail> list = listNodePoint();
    ptrdiff_t i = indexOfNode(list, p1);
    ptrdiff_t j = indexOfNode(list, p2);

    ptrdiff_t min = qMin(i, j);

    if (min == 0 && (i == list.size() - 1 || j == list.size() - 1))
    {
        return list.size() - 1;
    }
    else
    {
        return min;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDetail::NodeOnEdge(const quint32 &index, VNodeDetail &p1, VNodeDetail &p2) const
{
    QVector<VNodeDetail> list = listNodePoint();
    if (index > static_cast<quint32>(list.size()))
    {
        qDebug()<<"Wrong edge index index ="<<index;
        return;
    }
    p1 = list.at(static_cast<int>(index));
    if (index + 1 > static_cast<quint32>(list.size()) - 1)
    {
        p2 = list.at(0);
    }
    else
    {
        p2 = list.at(static_cast<int>(index+1));
    }
}

//---------------------------------------------------------------------------------------------------------------------
VDetail VDetail::RemoveEdge(const quint32 &index) const
{
    VDetail det(*this);
    det.ClearNodes();

    QVector<VNodeDetail> list = this->listNodePoint();
    quint32 edge = static_cast<quint32>(list.size());
    quint32 k = 0;
    for (quint32 i=0; i<edge; ++i)
    {
        if (i == index)
        {
            det.append(this->at(static_cast<int>(k)));
            ++k;
        }
        else
        {
            VNodeDetail p1;
            VNodeDetail p2;
            this->NodeOnEdge(i, p1, p2);
            ptrdiff_t j1 = this->indexOfNode(p1.getId());
            ptrdiff_t j2 = this->indexOfNode(p2.getId());
            if (j2 == 0)
            {
                j2 = this->CountNode()-1;
                if (j1 == j2)
                {
                    det.append(this->at(j1));
                    ++k;
                    continue;
                }
            }
            for (ptrdiff_t j=j1; j<j2; ++j)
            {
                det.append(this->at(j));
                ++k;
            }
        }
    }
    return det;
}

//---------------------------------------------------------------------------------------------------------------------
QList<quint32> VDetail::Missing(const VDetail &det) const
{
    if (nodes.size() == det.CountNode())
    {
        return QList<quint32>();
    }

    QSet<quint32> set1;
    for (qint32 i = 0; i < nodes.size(); ++i)
    {
        set1.insert(nodes.at(i).getId());
    }

    QSet<quint32> set2;
    for (qint32 j = 0; j < det.CountNode(); ++j)
    {
        set2.insert(det.at(j).getId());
    }

    QSet<quint32> set3 = set1.subtract(set2);

    return set3.toList();
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VNodeDetail> VDetail::listNodePoint() const
{
    QVector<VNodeDetail> list;
    for (ptrdiff_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes.at(i).getTypeTool() == Tool::NodePoint)
        {
            list.append(nodes.at(i));
        }
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
ptrdiff_t VDetail::indexOfNode(const QVector<VNodeDetail> &list, const quint32 &id)
{
    for (ptrdiff_t i = 0; i < list.size(); ++i)
    {
        if (list.at(i).getId() == id)
        {
            return i;
        }
    }
    qDebug()<<"Can't find node.";
    return -1;
}
