/************************************************************************
 **
 **  @file   vnodedetail.cpp
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

#include "vnodedetail.h"
#include "vnodedetail_p.h"
#include "vpiecenode.h"
#include "vpiecepath.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"

#include <QLineF>
#include <QVector>

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::VNodeDetail()
    :d(new VNodeDetailData)
{}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::VNodeDetail(quint32 id, Tool typeTool, NodeDetail typeNode, qreal mx, qreal my, bool reverse)
    :d(new VNodeDetailData(id, typeTool, typeNode, mx, my, reverse))
{}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::VNodeDetail(const VNodeDetail &node)
    :d (node.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail &VNodeDetail::operator =(const VNodeDetail &node)
{
    if ( &node == this )
    {
        return *this;
    }
    d = node.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::~VNodeDetail()
{}

//---------------------------------------------------------------------------------------------------------------------
quint32 VNodeDetail::getId() const
{
    return d->id;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setId(const quint32 &value)
{
    d->id = value;
}

//---------------------------------------------------------------------------------------------------------------------
Tool VNodeDetail::getTypeTool() const
{
    return d->typeTool;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VNodeDetail::setTypeTool(const Tool &value)
{
    d->typeTool = value;
}

//---------------------------------------------------------------------------------------------------------------------
NodeDetail VNodeDetail::getTypeNode() const
{
    return d->typeNode;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VNodeDetail::setTypeNode(const NodeDetail &value)
{
    d->typeNode = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VNodeDetail::getMx() const
{
    return d->mx;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setMx(const qreal &value)
{
    d->mx = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VNodeDetail::getMy() const
{
    return d->my;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setMy(const qreal &value)
{
    d->my = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VNodeDetail::getReverse() const
{
    if (getTypeTool() == Tool::NodePoint)
    {
        return false;
    }
    else
    {
        return d->reverse;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setReverse(bool reverse)
{
    if (getTypeTool() == Tool::NodePoint)
    {
        d->reverse = false;
    }
    else
    {
        d->reverse = reverse;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPieceNode> VNodeDetail::Convert(const VContainer *data, const QVector<VNodeDetail> &nodes, qreal width,
                                         bool closed)
{
    if (width < 0)
    {
        width = 0;
    }

    VPiecePath path;
    for (int i = 0; i < nodes.size(); ++i)
    {
        const VNodeDetail &node = nodes.at(i);
        path.Append(VPieceNode(node.getId(), node.getTypeTool(), node.getReverse()));
    }

    auto LocalWidth = [width](qreal move) noexcept
    {
        qreal value = width + move;
        if (value < 0)
        {
            value = 0;
        }
        return value;
    };

    if (path.PathPoints(data).size() > 2)
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            const VNodeDetail &node = nodes.at(i);
            if (node.getTypeTool() == Tool::NodePoint)
            {
                if (not qFuzzyIsNull(node.getMx()) || not qFuzzyIsNull(node.getMy()))
                {
                    const QPointF previosPoint = path.NodePreviousPoint(data, i);
                    const QPointF nextPoint = path.NodeNextPoint(data, i);

                    const QPointF point = data->GeometricObject<VPointF>(node.getId())->toQPointF();

                    const QPointF xPoint(point.x()+node.getMx(), point.y());
                    const QPointF yPoint(point.x(), point.y()+node.getMy());

                    if (IsSABefore(QLineF(point, previosPoint), QLineF(point, xPoint)))
                    {
                        path[i].SetSABefore(LocalWidth(node.getMx()));

                        if (IsSAAfter(QLineF(point, nextPoint), QLineF(point, yPoint)))
                        {
                            path[i].SetSAAfter(LocalWidth(node.getMy()));
                        }
                    }
                    else if (IsSABefore(QLineF(point, previosPoint), QLineF(point, yPoint)))
                    {
                        path[i].SetSABefore(LocalWidth(node.getMy()));

                        if (IsSAAfter(QLineF(point, nextPoint), QLineF(point, xPoint)))
                        {
                            path[i].SetSAAfter(LocalWidth(node.getMx()));
                        }
                    }
                    else if (IsSAAfter(QLineF(point, nextPoint), QLineF(point, xPoint)))
                    {
                        path[i].SetSAAfter(LocalWidth(node.getMx()));
                    }
                    else if (IsSAAfter(QLineF(point, nextPoint), QLineF(point, yPoint)))
                    {
                        path[i].SetSAAfter(LocalWidth(node.getMy()));
                    }
                }
            }
        }
    }

    if (not closed && path.CountNodes() > 1)
    {
        path[0].SetSABefore(0);
        path[path.CountNodes()-1].SetSAAfter(0);
    }

    return path.GetNodes();
}

//---------------------------------------------------------------------------------------------------------------------
bool VNodeDetail::IsSABefore(const QLineF &saBaseLine, const QLineF &mLine)
{
    if (qFuzzyIsNull(mLine.length()))
    {
        return false;
    }

    QLineF saLine = saBaseLine;
    saLine.setAngle(saLine.angle() - 90);

    int saAngle = qRound(saLine.angle());
    if (saAngle >= 360)
    {
        saAngle = 0;
    }

    int mTest1 = qRound(mLine.angle());
    if (mTest1 >= 360)
    {
        mTest1 = 0;
    }

    QLineF mrLine = mLine;
    mrLine.setAngle(mrLine.angle()+180);
    int mTest2 = qRound(mrLine.angle());
    if (mTest2 >=360)
    {
        mTest2 = 0;
    }

    return (saAngle == mTest1 || saAngle == mTest2);
}

//---------------------------------------------------------------------------------------------------------------------
bool VNodeDetail::IsSAAfter(const QLineF &saBaseLine, const QLineF &mLine)
{
    if (qFuzzyIsNull(mLine.length()))
    {
        return false;
    }

    QLineF saLine = saBaseLine;
    saLine.setAngle(saLine.angle() + 90);

    int saAngle = qRound(saLine.angle());
    if (saAngle >= 360)
    {
        saAngle = 0;
    }

    int mTest1 = qRound(mLine.angle());
    if (mTest1 >= 360)
    {
        mTest1 = 0;
    }

    QLineF mrLine = mLine;
    mrLine.setAngle(mrLine.angle()+180);
    int mTest2 = qRound(mrLine.angle());
    if (mTest2 >=360)
    {
        mTest2 = 0;
    }

    return (saAngle == mTest1 || saAngle == mTest2);
}
