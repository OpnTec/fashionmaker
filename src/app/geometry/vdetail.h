/************************************************************************
 **
 **  @file   vdetail.h
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

#ifndef VDETAIL_H
#define VDETAIL_H

#include "vnodedetail.h"
#include <QString>
#include <QVector>

class QString;
class VDetailData;

/**
 * @brief The VDetail class for path of object (points, arcs, splines).
 */
class VDetail
{
public:
    /**
     * @brief VDetail default contructor. Create empty detail.
     */
    VDetail();
    /**
     * @brief VDetail constructor.
     * @param name detail name.
     * @param nodes list of nodes.
     */
    VDetail(const QString &name, const QVector<VNodeDetail> &nodes);
    /**
     * @brief VDetail copy constructor.
     * @param detail detail.
     */
    VDetail(const VDetail &detail);
    /**
     * @brief operator = assignment operator.
     * @param detail detail.
     * @return new detail.
     */
    VDetail &operator=(const VDetail &detail);
    ~VDetail();
    /**
     * @brief append append in the end of list node.
     * @param node new node.
     */
    void           append(const VNodeDetail &node);
    /**
     * @brief Clear detail full clear.
     */
    void           Clear();
    /**
     * @brief ClearNodes clear list of nodes.
     */
    void           ClearNodes();
    /**
     * @brief CountNode return count nodes.
     * @return count.
     */
    qint32         CountNode() const;
    /**
     * @brief Containes check if detail containe this id.
     * @param id object id.
     * @return true if containe.
     */
    bool           Containes(const quint32 &id)const;
    /**
     * @brief operator [] find node by index in list.
     * @param indx index node in list.
     * @return node
     */
    VNodeDetail &  operator[](ptrdiff_t indx);
    /**
     * @brief at find node by index in list.
     * @param indx index node in list.
     * @return const node.
     */
    const VNodeDetail & at ( ptrdiff_t indx ) const;
    /**
     * @brief getName return detail name.
     * @return name.
     */
    QString        getName() const;
    /**
     * @brief setName set detail name.
     * @param value new name.
     */
    void           setName(const QString &value);
    /**
     * @brief getMx return bias for X axis.
     * @return x bias.
     */
    qreal          getMx() const;
    /**
     * @brief setMx set bias for X axis.
     * @param value new x bias.
     */
    void           setMx(const qreal &value);
    /**
     * @brief getMy get bias for y axis.
     * @return y axis.
     */
    qreal          getMy() const;
    /**
     * @brief setMy set bias for y axis.
     * @param value new y bias.
     */
    void           setMy(const qreal &value);
    /**
     * @brief getSeamAllowance keep status for seam allowance detail.
     * @return true - need seam allowance, false - no need seam allowance.
     */
    bool           getSeamAllowance() const;
    /**
     * @brief setSeamAllowance set status for seam allowance detail.
     * @param value true - need seam allowance, false - no need seam allowance.
     */
    void           setSeamAllowance(bool value);
    /**
     * @brief getClosed keep close status for detail equdistant.
     * @return true - close equdistant, false - don't close equdistant.
     */
    bool           getClosed() const;
    /**
     * @brief setClosed set close status for detail equdistant.
     * @param value true - close equdistant, false - don't close equdistant.
     */
    void           setClosed(bool value);
    /**
     * @brief getWidth return value detail seam allowance.
     * @return value in mm.
     */
    qreal          getWidth() const;
    /**
     * @brief setWidth set value detail seam allowance.
     * @param value width in mm.
     */
    void           setWidth(const qreal &value);
    /**
     * @brief getNodes return list of nodes.
     * @return list of nodes.
     */
    QVector<VNodeDetail> getNodes() const;
    /**
     * @brief setNodes set list of nodes
     * @param value list of nodes
     */
    void           setNodes(const QVector<VNodeDetail> &value);
    /**
     * @brief indexOfNode return index in list node using id object.
     * @param id object (arc, point, spline, splinePath) id.
     * @return index in list or -1 id can't find.
     */
    ptrdiff_t      indexOfNode(const quint32 &id) const;
    /**
     * @brief id return id detail in list data.
     * @return id.
     */
    quint32         id() const;
    /**
     * @brief setId set id detail in list data.
     * @param id detail id.
     */
    void           setId(const quint32 &id);
    /**
     * @brief OnEdge checks if two poins located on the edge. Edge is line between two points. If between two points
     * located arcs or splines ignore this.
     * @param p1 id first point.
     * @param p2 id second point.
     * @return true - on edge, false - no.
     */
    bool           OnEdge(const quint32 &p1, const quint32 &p2)const;
    /**
     * @brief Edge return edge index in detail. Edge is line between two points. If between two points
     * located arcs or splines ignore this.
     * @param p1 id first point.
     * @param p2 id second point.
     * @return edge index or -1 if points don't located on edge
     */
    ptrdiff_t      Edge(const quint32 &p1, const quint32 &p2)const;
    /**
     * @brief NodeOnEdge return nodes located on edge with index.
     * @param index index of edge.
     * @param p1 first node.
     * @param p2 second node.
     */
    void           NodeOnEdge(const quint32 &index, VNodeDetail &p1, VNodeDetail &p2)const;
    /**
     * @brief RemoveEdge return detail without edge with index.
     * @param index idex of edge.
     * @return detail without edge with index.
     */
    VDetail        RemoveEdge(const quint32 &index) const;
    /**
     * @brief Missing find missing ids in detail. When we deleted object in detail and return this detail need
     * understand, what nodes need make invisible.
     * @param det changed detail.
     * @return  list with missing detail.
     */
    QList<quint32> Missing(const VDetail &det) const;
private:
    QSharedDataPointer<VDetailData> d;
    /**
     * @brief listNodePoint return list nodes only with points.
     * @return list points node.
     */
    QVector<VNodeDetail> listNodePoint()const;
    /**
     * @brief indexOfNode return index in list node using id object.
     * @param list list nodes detail.
     * @param id object (arc, point, spline, splinePath) id.
     * @return index in list or -1 id can't find.
     */
    static int     indexOfNode(const QVector<VNodeDetail> &list, const quint32 &id);
};

#endif // VDETAIL_H
