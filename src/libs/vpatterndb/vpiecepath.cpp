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
QVector<QPointF> VPiecePath::PathNodePoints(const VContainer *data) const
{
    QVector<QPointF> points;
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
VSAPoint VPiecePath::StartSegment(const VContainer *data, int i, bool reverse) const
{
    if (i < 0 && i > CountNodes()-1)
    {
        return VSAPoint();
    }

    const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(at(i).GetId());

    QVector<QPointF> points = curve->GetPoints();
    if (reverse)
    {
        points = VGObject::GetReversePoints(points);
    }

    VSAPoint begin = VSAPoint(points.first());
    if (CountNodes() > 1)
    {
        if (i == 0)
        {
            if (at(CountNodes()-1).GetTypeTool() == Tool::NodePoint)
            {
                const VPieceNode &node = at(CountNodes()-1);
                const QPointF p = *data->GeometricObject<VPointF>(node.GetId());
                if (curve->IsPointOnCurve(p))
                {
                    begin = VSAPoint(p);
                    begin.SetSAAfter(node.GetSAAfter(*data->GetPatternUnit()));
                    begin.SetSABefore(node.GetSABefore(*data->GetPatternUnit()));
                    begin.SetAngleType(node.GetAngleType());
                }
            }
        }
        else
        {
            if (at(i-1).GetTypeTool() == Tool::NodePoint)
            {
                const VPieceNode &node = at(i-1);
                const QPointF p = *data->GeometricObject<VPointF>(node.GetId());
                if (curve->IsPointOnCurve(p))
                {
                    begin = VSAPoint(p);
                    begin.SetSAAfter(node.GetSAAfter(*data->GetPatternUnit()));
                    begin.SetSABefore(node.GetSABefore(*data->GetPatternUnit()));
                    begin.SetAngleType(node.GetAngleType());
                }
            }
        }
    }
    return begin;
}

//---------------------------------------------------------------------------------------------------------------------
VSAPoint VPiecePath::EndSegment(const VContainer *data, int i, bool reverse) const
{
    if (i < 0 && i > CountNodes()-1)
    {
        return VSAPoint();
    }

    const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(at(i).GetId());

    QVector<QPointF> points = curve->GetPoints();
    if (reverse)
    {
        points = VGObject::GetReversePoints(points);
    }

    VSAPoint end = VSAPoint(points.last());
    if (CountNodes() > 2)
    {
        if (i == CountNodes() - 1)
        {
            if (at(0).GetTypeTool() == Tool::NodePoint)
            {
                const VPieceNode &node = at(0);
                const QPointF p = *data->GeometricObject<VPointF>(node.GetId());
                if (curve->IsPointOnCurve(p))
                {
                    end = VSAPoint(p);
                    end.SetSAAfter(node.GetSAAfter(*data->GetPatternUnit()));
                    end.SetSABefore(node.GetSABefore(*data->GetPatternUnit()));
                    end.SetAngleType(node.GetAngleType());
                }
            }
        }
        else
        {
            if (at(i+1).GetTypeTool() == Tool::NodePoint)
            {
                const VPieceNode &node = at(i+1);
                const QPointF p = *data->GeometricObject<VPointF>(node.GetId());
                if (curve->IsPointOnCurve(p))
                {
                    end = VSAPoint(p);
                    end.SetSAAfter(node.GetSAAfter(*data->GetPatternUnit()));
                    end.SetSABefore(node.GetSABefore(*data->GetPatternUnit()));
                    end.SetAngleType(node.GetAngleType());
                }
            }
        }
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
