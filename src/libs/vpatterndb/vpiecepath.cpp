/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vpiecepath.h"
#include "vpiecepath_p.h"
#include "vcontainer.h"
#include "../vgeometry/vpointf.h"
#include "../vlayout/vabstractpiece.h"

#include <QPainterPath>

namespace
{
//---------------------------------------------------------------------------------------------------------------------
VSAPoint CurvePoint(VSAPoint candidate, const VContainer *data, const VPieceNode &node,
                    const QVector<QPointF> &curvePoints)
{
    if (node.GetTypeTool() == Tool::NodePoint)
    {
        const QPointF p = *data->GeometricObject<VPointF>(node.GetId());
        if (VAbstractCurve::IsPointOnCurve(curvePoints, p))
        {
            candidate = VSAPoint(p);
            candidate.SetSAAfter(node.GetSAAfter(data, *data->GetPatternUnit()));
            candidate.SetSABefore(node.GetSABefore(data, *data->GetPatternUnit()));
            candidate.SetAngleType(node.GetAngleType());
        }
    }
    return candidate;
}

//---------------------------------------------------------------------------------------------------------------------
VSAPoint CurveStartPoint(VSAPoint candidate, const VContainer *data, const VPieceNode &node,
                         const QVector<QPointF> &curvePoints)
{
    if (node.GetTypeTool() == Tool::NodePoint)
    {
        return CurvePoint(candidate, data, node, curvePoints);
    }
    else
    {
        // See issue #620. Detail path not correct. Previous curve also should cut segment.
        const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(node.GetId());

        const QVector<QPointF> points = curve->GetPoints();
        if (not points.isEmpty())
        {
            QPointF end; // Last point for this curve show start of next segment
            node.GetReverse() ? end = points.first() : end = points.last();
            if (VAbstractCurve::IsPointOnCurve(curvePoints, end))
            {
                candidate = VSAPoint(end);
            }
        }
    }
    return candidate;
}

//---------------------------------------------------------------------------------------------------------------------
VSAPoint CurveEndPoint(VSAPoint candidate, const VContainer *data, const VPieceNode &node,
                       const QVector<QPointF> &curvePoints)
{
    if (node.GetTypeTool() == Tool::NodePoint)
    {
        return CurvePoint(candidate, data, node, curvePoints);
    }
    else
    {
        // See issue #620. Detail path not correct. Previous curve also should cut segment.
        const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(node.GetId());

        const QVector<QPointF> points = curve->GetPoints();
        if (not points.isEmpty())
        {
            QPointF begin;// First point for this curve show finish of previous segment
            node.GetReverse() ? begin = points.last() : begin = points.first();
            if (VAbstractCurve::IsPointOnCurve(curvePoints, begin))
            {
                candidate = VSAPoint(begin);
            }
        }
    }
    return candidate;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief indexOfNode return index in list node using id object.
 * @param list list nodes detail.
 * @param id object (arc, point, spline, splinePath) id.
 * @return index in list or -1 id can't find.
 */
int IndexOfNode(const QVector<VPieceNode> &list, quint32 id)
{
    for (int i = 0; i < list.size(); ++i)
    {
        if (list.at(i).GetId() == id)
        {
            return i;
        }
    }
    qDebug()<<"Can't find node.";
    return -1;
}
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath::VPiecePath()
    : d(new VPiecePathData)
{}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath::VPiecePath(PiecePathType type)
    : d(new VPiecePathData(type))
{}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath::VPiecePath(const VPiecePath &path)
    : d (path.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath &VPiecePath::operator=(const VPiecePath &path)
{
    if ( &path == this )
    {
        return *this;
    }
    d = path.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath::~VPiecePath()
{}

//---------------------------------------------------------------------------------------------------------------------
void VPiecePath::Append(const VPieceNode &node)
{
    d->m_nodes.append(node);
}

//---------------------------------------------------------------------------------------------------------------------
void VPiecePath::Clear()
{
    d->m_nodes.clear();
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VPiecePath::CountNodes() const
{
    return d->m_nodes.size();
}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode &VPiecePath::operator[](int indx)
{
    return d->m_nodes[indx];
}

//---------------------------------------------------------------------------------------------------------------------
const VPieceNode &VPiecePath::at(int indx) const
{
    return d->m_nodes.at(indx);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPieceNode> VPiecePath::GetNodes() const
{
    return d->m_nodes;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiecePath::SetNodes(const QVector<VPieceNode> &nodes)
{
    d->m_nodes = nodes;
}

//---------------------------------------------------------------------------------------------------------------------
PiecePathType VPiecePath::GetType() const
{
    return d->m_type;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiecePath::SetType(PiecePathType type)
{
    d->m_type = type;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPiecePath::GetName() const
{
    return d->m_name;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiecePath::SetName(const QString &name)
{
    d->m_name = name;
}

//---------------------------------------------------------------------------------------------------------------------
Qt::PenStyle VPiecePath::GetPenType() const
{
    return d->m_penType;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiecePath::SetPenType(const Qt::PenStyle &type)
{
    d->m_penType = type;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VPiecePath::PathPoints(const VContainer *data) const
{
    QVector<QPointF> points;
    for (int i = 0; i < CountNodes(); ++i)
    {
        switch (at(i).GetTypeTool())
        {
            case (Tool::NodePoint):
            {
                const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(at(i).GetId());
                points.append(*point);
            }
            break;
            case (Tool::NodeArc):
            case (Tool::NodeElArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(at(i).GetId());

                const QPointF begin = StartSegment(data, i, at(i).GetReverse());
                const QPointF end = EndSegment(data, i, at(i).GetReverse());

                points << curve->GetSegmentPoints(begin, end, at(i).GetReverse());
            }
            break;
            default:
                qDebug()<<"Get wrong tool type. Ignore."<< static_cast<char>(at(i).GetTypeTool());
                break;
        }
    }

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPointF> VPiecePath::PathNodePoints(const VContainer *data) const
{
    QVector<VPointF> points;
    for (int i = 0; i < CountNodes(); ++i)
    {
        switch (at(i).GetTypeTool())
        {
            case Tool::NodePoint:
            {
                const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(at(i).GetId());
                points.append(*point);
            }
            break;
            case Tool::NodeArc:
            case Tool::NodeElArc:
            case Tool::NodeSpline:
            case Tool::NodeSplinePath:
            default:
                break;
        }
    }

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> VPiecePath::SeamAllowancePoints(const VContainer *data, qreal width, bool reverse) const
{
    SCASSERT(data != nullptr);

    QVector<VSAPoint> pointsEkv;
    for (int i = 0; i< d->m_nodes.size(); ++i)
    {
        const VPieceNode &node = d->m_nodes.at(i);
        switch (node.GetTypeTool())
        {
            case (Tool::NodePoint):
            {
                pointsEkv.append(PreparePointEkv(node, data));
            }
            break;
            case (Tool::NodeArc):
            case (Tool::NodeElArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(node.GetId());
                pointsEkv += CurveSeamAllowanceSegment(data, d->m_nodes, curve, i, node.GetReverse(), width);
            }
            break;
            default:
                qDebug()<<"Get wrong tool type. Ignore."<< static_cast<char>(node.GetTypeTool());
                break;
        }
    }

    if (reverse)
    {
        pointsEkv = VGObject::GetReversePoints(pointsEkv);
    }

    return pointsEkv;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPiecePath::PainterPath(const VContainer *data) const
{
    const QVector<QPointF> points = PathPoints(data);
    QPainterPath path;

    if (not points.isEmpty())
    {
        path.moveTo(points[0]);
        for (qint32 i = 1; i < points.count(); ++i)
        {
            path.lineTo(points.at(i));
        }
        path.setFillRule(Qt::WindingFill);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
VSAPoint VPiecePath::StartSegment(const VContainer *data, const QVector<VPieceNode> &nodes, int i, bool reverse)
{
    if (i < 0 || i > nodes.size()-1)
    {
        return VSAPoint();
    }

    const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(nodes.at(i).GetId());

    const QVector<QPointF> points = curve->GetPoints();
    if (points.isEmpty())
    {
        return VSAPoint();
    }

    VSAPoint begin;
    reverse ? begin = VSAPoint(points.last()) : begin = VSAPoint(points.first());

    if (nodes.size() > 1)
    {
        int index = 0;
        i == 0 ? index = nodes.size()-1 : index = i-1;

        begin = CurveStartPoint(begin, data, nodes.at(index), points);
    }
    return begin;
}

//---------------------------------------------------------------------------------------------------------------------
VSAPoint VPiecePath::EndSegment(const VContainer *data, const QVector<VPieceNode> &nodes, int i, bool reverse)
{
    if (i < 0 || i > nodes.size()-1)
    {
        return VSAPoint();
    }

    const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(nodes.at(i).GetId());

    const QVector<QPointF> points = curve->GetPoints();
    if (points.isEmpty())
    {
        return VSAPoint();
    }

    VSAPoint end;
    reverse ? end = VSAPoint(points.first()) : end = VSAPoint(points.last());

    if (nodes.size() > 2)
    {
        int index = 0;
        i == nodes.size() - 1 ? index = 0 : index = i+1;

        end = CurveEndPoint(end, data, nodes.at(index), points);
    }
    return end;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPiecePath::MissingNodes(const VPiecePath &path) const
{
    if (d->m_nodes.size() == path.CountNodes()) //-V807
    {
        return QVector<quint32>();
    }

    QSet<quint32> set1;
    for (qint32 i = 0; i < d->m_nodes.size(); ++i)
    {
        set1.insert(d->m_nodes.at(i).GetId());
    }

    QSet<quint32> set2;
    for (qint32 j = 0; j < path.CountNodes(); ++j)
    {
        set2.insert(path.at(j).GetId());
    }

    const QList<quint32> set3 = set1.subtract(set2).toList();
    QVector<quint32> nodes;
    for (qint32 i = 0; i < set3.size(); ++i)
    {
        const int index = indexOfNode(set3.at(i));
        if (index != -1)
        {
            nodes.append(d->m_nodes.at(index).GetId());
        }
    }

    return nodes;
}

//---------------------------------------------------------------------------------------------------------------------
int VPiecePath::indexOfNode(quint32 id) const
{
    for (int i = 0; i < d->m_nodes.size(); ++i)
    {
        if (d->m_nodes.at(i).GetId() == id)
        {
            return i;
        }
    }
    qDebug()<<"Can't find node.";
    return -1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NodeOnEdge return nodes located on edge with index.
 * @param index index of edge.
 * @param p1 first node.
 * @param p2 second node.
 */
void VPiecePath::NodeOnEdge(quint32 index, VPieceNode &p1, VPieceNode &p2) const
{
    const QVector<VPieceNode> list = ListNodePoint();
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
bool VPiecePath::Contains(quint32 id) const
{
    for (int i = 0; i < d->m_nodes.size(); ++i)
    {
        if (d->m_nodes.at(i).GetId() == id)
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief OnEdge checks if two poins located on the edge. Edge is line between two points. If between two points
 * located arcs or splines ignore this.
 * @param p1 id first point.
 * @param p2 id second point.
 * @return true - on edge, false - no.
 */
bool VPiecePath::OnEdge(quint32 p1, quint32 p2) const
{
    const QVector<VPieceNode> list = ListNodePoint();
    if (list.size() < 2)
    {
        qDebug()<<"Not enough points.";
        return false;
    }
    int i = IndexOfNode(list, p1);
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

    if (list.at(j1).GetId() == p2 || list.at(j2).GetId() == p2)
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
int VPiecePath::Edge(quint32 p1, quint32 p2) const
{
    if (OnEdge(p1, p2) == false)
    {
        qDebug()<<"Points don't on edge.";
        return -1;
    }

    const QVector<VPieceNode> list = ListNodePoint();
    int i = IndexOfNode(list, p1);
    int j = IndexOfNode(list, p2);

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
 * @brief listNodePoint return list nodes only with points.
 * @return list points node.
 */
QVector<VPieceNode> VPiecePath::ListNodePoint() const
{
    QVector<VPieceNode> list;
    for (int i = 0; i < d->m_nodes.size(); ++i) //-V807
    {
        if (d->m_nodes.at(i).GetTypeTool() == Tool::NodePoint)
        {
            list.append(d->m_nodes.at(i));
        }
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveEdge return path without edge with index.
 * @param index idex of edge.
 * @return path without edge with index.
 */
VPiecePath VPiecePath::RemoveEdge(quint32 index) const
{
    VPiecePath path(*this);
    path.Clear();

    // Edge can be only segment. We ignore all curves inside segments.
    const quint32 edges = static_cast<quint32>(ListNodePoint().size());
    quint32 k = 0;
    for (quint32 i=0; i<edges; ++i)
    {
        if (i == index)
        {
            path.Append(this->at(static_cast<int>(k)));
            ++k;
        }
        else
        {
            VPieceNode p1;
            VPieceNode p2;
            this->NodeOnEdge(i, p1, p2);
            const int j1 = this->indexOfNode(p1.GetId());
            int j2 = this->indexOfNode(p2.GetId());
            if (j2 == 0)
            {
                j2 = this->CountNodes();
            }
            for (int j=j1; j<j2; ++j)
            {// Add "segment" except last point. Inside can be curves too.
                path.Append(this->at(j));
                ++k;
            }
        }
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
VSAPoint VPiecePath::StartSegment(const VContainer *data, int i, bool reverse) const
{
    return StartSegment(data, d->m_nodes, i, reverse);
}

//---------------------------------------------------------------------------------------------------------------------
VSAPoint VPiecePath::EndSegment(const VContainer *data, int i, bool reverse) const
{
    return EndSegment(data, d->m_nodes, i, reverse);
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VPiecePath::NodePreviousPoint(const VContainer *data, int i) const
{
    if (i < 0 || i > d->m_nodes.size()-1)
    {
        return QPointF();
    }

    if (d->m_nodes.size() > 1)
    {
        int index = 0;
        if (i == 0)
        {
            index = d->m_nodes.size()-1;
        }
        else
        {
            index = i-1;
        }

        const VPieceNode &node = d->m_nodes.at(index);
        switch (node.GetTypeTool())
        {
            case (Tool::NodePoint):
                return *data->GeometricObject<VPointF>(node.GetId());
            case (Tool::NodeArc):
            case (Tool::NodeElArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(node.GetId());

                const VSAPoint begin = StartSegment(data, d->m_nodes, index, node.GetReverse());
                const VSAPoint end = EndSegment(data, d->m_nodes, index, node.GetReverse());

                const QVector<QPointF> points = curve->GetSegmentPoints(begin, end, node.GetReverse());
                if (points.size() > 1)
                {
                    return points.at(points.size()-2);
                }
            }
                break;
            default:
                qDebug()<<"Get wrong tool type. Ignore."<< static_cast<char>(node.GetTypeTool());
                break;
        }
    }

    return QPointF();
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VPiecePath::NodeNextPoint(const VContainer *data, int i) const
{
    QPointF point;
    if (i < 0 || i > d->m_nodes.size()-1)
    {
        return point;
    }

    if (d->m_nodes.size() > 1)
    {
        int index = 0;
        if (i == d->m_nodes.size() - 1)
        {
            index = 0;
        }
        else
        {
            index = i+1;
        }

        const VPieceNode &node = d->m_nodes.at(index);
        switch (node.GetTypeTool())
        {
            case (Tool::NodePoint):
                return *data->GeometricObject<VPointF>(node.GetId());
            case (Tool::NodeArc):
            case (Tool::NodeElArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(node.GetId());

                const VSAPoint begin = StartSegment(data, d->m_nodes, index, node.GetReverse());
                const VSAPoint end = EndSegment(data, d->m_nodes, index, node.GetReverse());

                const QVector<QPointF> points = curve->GetSegmentPoints(begin, end, node.GetReverse());
                if (points.size() > 1)
                {
                    return points.at(1);
                }
            }
                break;
            default:
                qDebug()<<"Get wrong tool type. Ignore."<< static_cast<char>(node.GetTypeTool());
                break;
        }
    }

    return point;
}

//---------------------------------------------------------------------------------------------------------------------
VSAPoint VPiecePath::PreparePointEkv(const VPieceNode &node, const VContainer *data)
{
    const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(node.GetId());
    VSAPoint p(point->toQPointF());

    p.SetSAAfter(node.GetSAAfter(data, *data->GetPatternUnit()));
    p.SetSABefore(node.GetSABefore(data, *data->GetPatternUnit()));
    p.SetAngleType(node.GetAngleType());

    return p;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> VPiecePath::CurveSeamAllowanceSegment(const VContainer *data, const QVector<VPieceNode> &nodes,
                                                        const QSharedPointer<VAbstractCurve> &curve, int i,
                                                        bool reverse, qreal width)
{
    QVector<VSAPoint> pointsEkv;

    const VSAPoint begin = StartSegment(data, nodes, i, reverse);
    const VSAPoint end = EndSegment(data, nodes, i, reverse);

    const QVector<QPointF> points = curve->GetSegmentPoints(begin, end, reverse);
    if (points.isEmpty())
    {
        return pointsEkv;
    }

    qreal w1 = begin.GetSAAfter();
    qreal w2 = end.GetSABefore();
    if (w1 < 0 && w2 < 0)
    {// no local widths
        for(int i = 0; i < points.size(); ++i)
        {
            VSAPoint p(points.at(i));
            if (i == 0)
            { // first point
                p.SetSAAfter(begin.GetSAAfter());
                p.SetSABefore(begin.GetSABefore());
                p.SetAngleType(begin.GetAngleType());
            }
            else if (i == points.size() - 1)
            { // last point
                p.SetSAAfter(end.GetSAAfter());
                p.SetSABefore(end.GetSABefore());
                p.SetAngleType(end.GetAngleType());
            }
            pointsEkv.append(p);
        }
    }
    else
    {
        if (w1 < 0)
        {
            w1 = width;
        }

        if (w2 < 0)
        {
            w2 = width;
        }

        const qreal wDiff = w2 - w1;// Difference between to local widths
        const qreal fullLength = VAbstractCurve::PathLength(points);

        VSAPoint p(points.at(0));//First point in the list
        p.SetSAAfter(begin.GetSAAfter());
        p.SetSABefore(begin.GetSABefore());
        p.SetAngleType(begin.GetAngleType());
        pointsEkv.append(p);

        qreal length = 0; // how much we handle

        for(int i = 1; i < points.size(); ++i)
        {
            p = VSAPoint(points.at(i));

            if (i == points.size() - 1)
            {// last point
                p.SetSAAfter(end.GetSAAfter());
                p.SetSABefore(end.GetSABefore());
                p.SetAngleType(end.GetAngleType());
            }
            else
            {
                length += QLineF(points.at(i-1), points.at(i)).length();
                const qreal localWidth = w1 + wDiff*(length/fullLength);

                p.SetSAAfter(localWidth);
                p.SetSABefore(localWidth);
                // curve points have angle type by default
            }

            pointsEkv.append(p);
        }
    }

    return pointsEkv;
}
