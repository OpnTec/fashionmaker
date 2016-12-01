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

    QVector<QPointF> points = curve->GetPoints();
    if (reverse)
    {
        points = VGObject::GetReversePoints(points);
    }

    VSAPoint begin = VSAPoint(points.first());
    if (nodes.size() > 1)
    {
        int index = 0;
        i == 0 ? index = nodes.size()-1 : index = i-1;

        begin = CurvePoint(begin, data, nodes.at(index), curve);
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

    QVector<QPointF> points = curve->GetPoints();
    if (reverse)
    {
        points = VGObject::GetReversePoints(points);
    }

    VSAPoint end = VSAPoint(points.last());
    if (nodes.size() > 2)
    {
        int index = 0;
        i == nodes.size() - 1 ? index = 0 : index = i+1;

        end = CurvePoint(end, data, nodes.at(index), curve);
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
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(node.GetId());

                const VSAPoint begin = StartSegment(data, d->m_nodes, i, node.GetReverse());
                const VSAPoint end = EndSegment(data, d->m_nodes, i, node.GetReverse());

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
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(node.GetId());

                const VSAPoint begin = StartSegment(data, d->m_nodes, i, node.GetReverse());
                const VSAPoint end = EndSegment(data, d->m_nodes, i, node.GetReverse());

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

    p.SetSAAfter(node.GetSAAfter(*data->GetPatternUnit()));
    p.SetSABefore(node.GetSABefore(*data->GetPatternUnit()));
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

//---------------------------------------------------------------------------------------------------------------------
VSAPoint VPiecePath::CurvePoint(const VSAPoint &candidate, const VContainer *data, const VPieceNode &node,
                                const QSharedPointer<VAbstractCurve> &curve)
{
    VSAPoint point = candidate;
    if (node.GetTypeTool() == Tool::NodePoint)
    {
        const QPointF p = *data->GeometricObject<VPointF>(node.GetId());
        if (curve->IsPointOnCurve(p))
        {
            point = VSAPoint(p);
            point.SetSAAfter(node.GetSAAfter(*data->GetPatternUnit()));
            point.SetSABefore(node.GetSABefore(*data->GetPatternUnit()));
            point.SetAngleType(node.GetAngleType());
        }
    }
    return point;
}
