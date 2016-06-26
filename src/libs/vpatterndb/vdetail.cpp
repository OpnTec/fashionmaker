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

#include "vdetail.h"
#include "vdetail_p.h"
#include "vcontainer.h"
#include "../vgeometry/vpointf.h"

#include <QDebug>
#include <QPainterPath>
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
bool VDetail::IsInLayout() const
{
    return d->inLayout;
}

//---------------------------------------------------------------------------------------------------------------------
void VDetail::SetInLayout(bool inLayout)
{
    d->inLayout = inLayout;
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
    if (list.size() < 2)
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

    // Edge can be only segment. We ignore all curves inside segments.
    const quint32 edges = static_cast<quint32>(listNodePoint().size());
    quint32 k = 0;
    for (quint32 i=0; i<edges; ++i)
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
            const int j1 = this->indexOfNode(p1.getId());
            int j2 = this->indexOfNode(p2.getId());
            if (j2 == 0)
            {
                j2 = this->CountNode();
            }
            for (int j=j1; j<j2; ++j)
            {// Add "segment" except last point. Inside can be curves too.
                det.append(this->at(j));
                ++k;
            }
        }
    }
    return det;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Missing find missing nodes in detail. When we deleted object in detail and return this detail need
 * understand, what nodes need make invisible.
 * @param det changed detail.
 * @return  list with missing nodes.
 */
QVector<VNodeDetail> VDetail::Missing(const VDetail &det) const
{
    if (d->nodes.size() == det.CountNode()) //-V807
    {
        return QVector<VNodeDetail>();
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

    const QList<quint32> set3 = set1.subtract(set2).toList();
    QVector<VNodeDetail> nodes;
    for (qint32 i = 0; i < set3.size(); ++i)
    {
        const int index = indexOfNode(d->nodes, set3.at(i));
        if (index != -1)
        {
            nodes.append(d->nodes.at(index));
        }
    }

    return nodes;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VDetail::ContourPoints(const VContainer *data) const
{
    QVector<QPointF> points;
    for (int i = 0; i< CountNode(); ++i)
    {
        switch (at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(at(i).getId());
                points.append(*point);
            }
            break;
            case (Tool::NodeArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(at(i).getId());

                const QPointF begin = StartSegment(data, i, at(i).getReverse());
                const QPointF end = EndSegment(data, i, at(i).getReverse());

                points << curve->GetSegmentPoints(begin, end, at(i).getReverse());
            }
            break;
            default:
                qDebug()<<"Get wrong tool type. Ignore."<< static_cast<char>(at(i).getTypeTool());
                break;
        }
    }

    points = CheckLoops(CorrectEquidistantPoints(points));//A path can contains loops
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VDetail::SeamAllowancePoints(const VContainer *data) const
{
    QVector<QPointF> pointsEkv;
    if (getSeamAllowance() == false)
    {
        return pointsEkv;
    }

    for (int i = 0; i< CountNode(); ++i)
    {
        switch (at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(at(i).getId());
                QPointF pEkv = *point;
                pEkv.setX(pEkv.x()+at(i).getMx());
                pEkv.setY(pEkv.y()+at(i).getMy());
                pointsEkv.append(pEkv);
            }
            break;
            case (Tool::NodeArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(at(i).getId());

                const QPointF begin = StartSegment(data, i, at(i).getReverse());
                const QPointF end = EndSegment(data, i, at(i).getReverse());

                const QVector<QPointF> nodePoints = curve->GetSegmentPoints(begin, end, at(i).getReverse());
                pointsEkv << biasPoints(nodePoints, at(i).getMx(), at(i).getMy());
            }
            break;
            default:
                qDebug()<<"Get wrong tool type. Ignore."<< static_cast<char>(at(i).getTypeTool());
                break;
        }
    }

    pointsEkv = CheckLoops(CorrectEquidistantPoints(pointsEkv));//A path can contains loops

    if (getClosed() == true)
    {
        pointsEkv = Equidistant(pointsEkv, EquidistantType::CloseEquidistant, ToPixel(getWidth(),
                                                                                      *data->GetPatternUnit()));
    }
    else
    {
        pointsEkv = Equidistant(pointsEkv, EquidistantType::OpenEquidistant, ToPixel(getWidth(),
                                                                                     *data->GetPatternUnit()));
    }
    return pointsEkv;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VDetail::ContourPath(const VContainer *data) const
{
    const QVector<QPointF> points = ContourPoints(data);
    QPainterPath path;

    // contour
    path.moveTo(points[0]);
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points.at(i));
    }
    path.lineTo(points.at(0));
    path.setFillRule(Qt::WindingFill);

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VDetail::SeamAllowancePath(const VContainer *data) const
{
    const QVector<QPointF> pointsEkv = SeamAllowancePoints(data);
    QPainterPath ekv;

    // seam allowence
    if (getSeamAllowance())
    {
        if (not pointsEkv.isEmpty())
        {
            ekv.moveTo(pointsEkv.at(0));
            for (qint32 i = 1; i < pointsEkv.count(); ++i)
            {
                ekv.lineTo(pointsEkv.at(i));
            }

            ekv.setFillRule(Qt::WindingFill);
        }
    }

    return ekv;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief listNodePoint return list nodes only with points.
 * @return list points node.
 */
QVector<VNodeDetail> VDetail::listNodePoint() const
{
    QVector<VNodeDetail> list;
    for (int i = 0; i < d->nodes.size(); ++i) //-V807
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
QPointF VDetail::StartSegment(const VContainer *data, const int &i, bool reverse) const
{
    if (i < 0 && i > CountNode()-1)
    {
        return QPointF();
    }

    const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(at(i).getId());

    QVector<QPointF> points = curve->GetPoints();
    if (reverse)
    {
        points = VGObject::GetReversePoints(points);
    }

    QPointF begin = points.first();
    if (CountNode() > 1)
    {
        if (i == 0)
        {
            if (at(CountNode()-1).getTypeTool() == Tool::NodePoint)
            {
                begin = *data->GeometricObject<VPointF>(at(CountNode()-1).getId());
            }
        }
        else
        {
            if (at(i-1).getTypeTool() == Tool::NodePoint)
            {
                begin = *data->GeometricObject<VPointF>(at(i-1).getId());
            }
        }
    }
    return begin;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VDetail::EndSegment(const VContainer *data, const int &i, bool reverse) const
{
    if (i < 0 && i > CountNode()-1)
    {
        return QPointF();
    }

    const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(at(i).getId());

    QVector<QPointF> points = curve->GetPoints();
    if (reverse)
    {
        points = VGObject::GetReversePoints(points);
    }

    QPointF end = points.last();
    if (CountNode() > 2)
    {
        if (i == CountNode() - 1)
        {
            if (at(0).getTypeTool() == Tool::NodePoint)
            {
                end = *data->GeometricObject<VPointF>(at(0).getId());
            }
        }
        else
        {
            if (at(i+1).getTypeTool() == Tool::NodePoint)
            {
                end = *data->GeometricObject<VPointF>(at(i+1).getId());
            }
        }
    }
    return end;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief biasPoints bias point.
 * @param points vector of points.
 * @param mx offset respect to x.
 * @param my offset respect to y.
 * @return new vector biased points.
 */
QVector<QPointF> VDetail::biasPoints(const QVector<QPointF> &points, const qreal &mx, const qreal &my)
{
    QVector<QPointF> p;
    for (qint32 i = 0; i < points.size(); ++i)
    {
        QPointF point = points.at(i);
        point.setX(point.x() + mx);
        point.setY(point.y() + my);
        p.append(point);
    }
    return p;
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
// cppcheck-suppress unusedFunction
void VDetail::setNodes(const QVector<VNodeDetail> &value)
{
    d->nodes = value;
}
