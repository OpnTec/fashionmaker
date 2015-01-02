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
#include "vdetail_p.h"
#include <QDebug>
#include <QString>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDetail default contructor. Create empty detail.
 */
VDetail::VDetail()
    :VAbstractDetail(), d(new VDetailData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDetail constructor.
 * @param name detail name.
 * @param nodes list of nodes.
 */
VDetail::VDetail(const QString &name, const QVector<VNodeDetail> &nodes)
    :VAbstractDetail(name), d(new VDetailData(nodes))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDetail copy constructor.
 * @param detail detail.
 */
VDetail::VDetail(const VDetail &detail)
    :VAbstractDetail(detail), d (detail.d)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator.
 * @param detail detail.
 * @return new detail.
 */
VDetail &VDetail::operator =(const VDetail &detail)
{
    if ( &detail == this )
    {
        return *this;
    }
    VAbstractDetail::operator=(detail);
    d = detail.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VDetail::~VDetail()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear detail full clear.
 */
void VDetail::Clear()
{
    d->nodes.clear();
    d->mx = 0;
    d->my = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearNodes clear list of nodes.
 */
void VDetail::ClearNodes()
{
    d->nodes.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Containes check if detail containe this id.
 * @param id object id.
 * @return true if containe.
 */
bool VDetail::Containes(const quint32 &id) const
{
    for (int i = 0; i < d->nodes.size(); ++i)
    {
        VNodeDetail node = d->nodes.at(i);
        if (node.getId() == id)
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator [] find node by index in list.
 * @param indx index node in list.
 * @return node
 */
VNodeDetail &VDetail::operator [](int indx)
{
    return d->nodes[indx];
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief at find node by index in list.
 * @param indx index node in list.
 * @return const node.
 */
const VNodeDetail &VDetail::at(int indx) const
{
    return d->nodes.at(indx);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief indexOfNode return index in list node using id object.
 * @param id object (arc, point, spline, splinePath) id.
 * @return index in list or -1 id can't find.
 */
int VDetail::indexOfNode(const quint32 &id) const
{
    return indexOfNode(d->nodes, id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief id return id detail in list data.
 * @return id.
 */
quint32 VDetail::id() const
{
    return d->_id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setId set id detail in list data.
 * @param id detail id.
 */
void VDetail::setId(const quint32 &id)
{
    d->_id = id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief OnEdge checks if two poins located on the edge. Edge is line between two points. If between two points
 * located arcs or splines ignore this.
 * @param p1 id first point.
 * @param p2 id second point.
 * @return true - on edge, false - no.
 */
bool VDetail::OnEdge(const quint32 &p1, const quint32 &p2) const
{
    QVector<VNodeDetail> list = listNodePoint();
    if (list.size() < 3)
    {
        qDebug()<<"Not enough points.";
        return false;
    }
    int i = indexOfNode(list, p1);
    int j1 = 0, j2 = 0;

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
/**
 * @brief Edge return edge index in detail. Edge is line between two points. If between two points
 * located arcs or splines ignore this.
 * @param p1 id first point.
 * @param p2 id second point.
 * @return edge index or -1 if points don't located on edge
 */
int VDetail::Edge(const quint32 &p1, const quint32 &p2) const
{
    if (OnEdge(p1, p2) == false)
    {
        qDebug()<<"Points don't on edge.";
        return -1;
    }

    QVector<VNodeDetail> list = listNodePoint();
    int i = indexOfNode(list, p1);
    int j = indexOfNode(list, p2);

    int min = qMin(i, j);

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
/**
 * @brief NodeOnEdge return nodes located on edge with index.
 * @param index index of edge.
 * @param p1 first node.
 * @param p2 second node.
 */
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
/**
 * @brief RemoveEdge return detail without edge with index.
 * @param index idex of edge.
 * @return detail without edge with index.
 */
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
            int j1 = this->indexOfNode(p1.getId());
            int j2 = this->indexOfNode(p2.getId());
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
            for (int j=j1; j<j2; ++j)
            {
                det.append(this->at(j));
                ++k;
            }
        }
    }
    return det;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Missing find missing ids in detail. When we deleted object in detail and return this detail need
 * understand, what nodes need make invisible.
 * @param det changed detail.
 * @return  list with missing detail.
 */
QList<quint32> VDetail::Missing(const VDetail &det) const
{
    if (d->nodes.size() == det.CountNode())
    {
        return QList<quint32>();
    }

    QSet<quint32> set1;
    for (qint32 i = 0; i < d->nodes.size(); ++i)
    {
        set1.insert(d->nodes.at(i).getId());
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
/**
 * @brief listNodePoint return list nodes only with points.
 * @return list points node.
 */
QVector<VNodeDetail> VDetail::listNodePoint() const
{
    QVector<VNodeDetail> list;
    for (int i = 0; i < d->nodes.size(); ++i)
    {
        if (d->nodes.at(i).getTypeTool() == Tool::NodePoint)
        {
            list.append(d->nodes.at(i));
        }
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief indexOfNode return index in list node using id object.
 * @param list list nodes detail.
 * @param id object (arc, point, spline, splinePath) id.
 * @return index in list or -1 id can't find.
 */
int VDetail::indexOfNode(const QVector<VNodeDetail> &list, const quint32 &id)
{
    for (int i = 0; i < list.size(); ++i)
    {
        if (list.at(i).getId() == id)
        {
            return i;
        }
    }
    qDebug()<<"Can't find node.";
    return -1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief append append in the end of list node.
 * @param node new node.
 */
void VDetail::append(const VNodeDetail &node)
{
    d->nodes.append(node);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CountNode return count nodes.
 * @return count.
 */
qint32 VDetail::CountNode() const
{
    return d->nodes.size();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getMx return bias for X axis.
 * @return x bias.
 */
qreal VDetail::getMx() const
{
    return d->mx;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMx set bias for X axis.
 * @param value new x bias.
 */
void VDetail::setMx(const qreal &value)
{
    d->mx = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getMy get bias for y axis.
 * @return y axis.
 */
qreal VDetail::getMy() const
{
    return d->my;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMy set bias for y axis.
 * @param value new y bias.
 */
void VDetail::setMy(const qreal &value)
{
    d->my = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getNodes return list of nodes.
 * @return list of nodes.
 */
QVector<VNodeDetail> VDetail::getNodes() const
{
    return d->nodes;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setNodes set list of nodes
 * @param value list of nodes
 */
void VDetail::setNodes(const QVector<VNodeDetail> &value)
{
    d->nodes = value;
}
