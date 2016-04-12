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
 **  Copyright (C) 2013-2015 Valentina project
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
#include <QVector>

#include "../vlayout/vabstractdetail.h"

class VDetailData;
class VContainer;
class QPainterPath;

/**
 * @brief The VDetail class for path of object (points, arcs, splines).
 */
class VDetail :public VAbstractDetail
{
public:
    VDetail();
    VDetail(const QString &name, const QVector<VNodeDetail> &nodes);
    VDetail(const VDetail &detail);
    VDetail &operator=(const VDetail &detail);
    virtual ~VDetail() Q_DECL_OVERRIDE;

    void           append(const VNodeDetail &node);
    void           Clear();
    void           ClearNodes();
    qint32         CountNode() const;
    bool           Containes(const quint32 &id)const;
    VNodeDetail &  operator[](int indx);
    const VNodeDetail & at ( int indx ) const;

    qreal getMx() const;
    void  setMx(const qreal &value);

    qreal getMy() const;
    void  setMy(const qreal &value);

    quint32 id() const;
    void    setId(const quint32 &id);

    QVector<VNodeDetail> getNodes() const;
    void setNodes(const QVector<VNodeDetail> &value);

    int  indexOfNode(const quint32 &id) const;
    bool OnEdge(const quint32 &p1, const quint32 &p2)const;
    int  Edge(const quint32 &p1, const quint32 &p2)const;
    void NodeOnEdge(const quint32 &index, VNodeDetail &p1, VNodeDetail &p2)const;
    VDetail RemoveEdge(const quint32 &index) const;

    QVector<VNodeDetail> Missing(const VDetail &det) const;

    QVector<QPointF> ContourPoints(const VContainer *data) const;
    QVector<QPointF> SeamAllowancePoints(const VContainer *data) const;

    QPainterPath ContourPath(const VContainer *data) const;
    QPainterPath SeamAllowancePath(const VContainer *data) const;
    QVector<VNodeDetail> listNodePoint()const;
private:
    QSharedDataPointer<VDetailData> d;

    static int indexOfNode(const QVector<VNodeDetail> &list, const quint32 &id);

    QPointF StartSegment(const VContainer *data, const int &i, bool reverse) const;
    QPointF EndSegment(const VContainer *data, const int &i, bool reverse) const;

    static QVector<QPointF> biasPoints(const QVector<QPointF> &points, const qreal &mx, const qreal &my);
};

Q_DECLARE_TYPEINFO(VDetail, Q_MOVABLE_TYPE);

#endif // VDETAIL_H
