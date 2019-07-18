/************************************************************************
 **
 **  @file   vcontour.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "vcontour.h"

#include <QLineF>
#include <QPainterPath>
#include <QPoint>
#include <QPointF>
#include <QPolygonF>
#include <QRectF>
#include <Qt>

#include "vcontour_p.h"
#include "vlayoutpiece.h"
#include "../vmisc/vmath.h"

namespace
{
//---------------------------------------------------------------------------------------------------------------------
void AppendToContour(QVector<QPointF> &contour, QPointF point)
{
    if (not contour.isEmpty())
    {
        if (not VFuzzyComparePoints(contour.last(), point))
        {
            contour.append(point);
        }
    }
    else
    {
        contour.append(point);
    }
}
}

//---------------------------------------------------------------------------------------------------------------------
VContour::VContour()
    :d(new VContourData())
{}

//---------------------------------------------------------------------------------------------------------------------
VContour::VContour(int height, int width, qreal layoutWidth)
    :d(new VContourData(height, width, layoutWidth))
{}

//---------------------------------------------------------------------------------------------------------------------
VContour::VContour(const VContour &contour)
    :d (contour.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VContour &VContour::operator=(const VContour &contour)
{
    if ( &contour == this )
    {
        return *this;
    }
    d = contour.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VContour::~VContour()
{}

//---------------------------------------------------------------------------------------------------------------------
void VContour::CeateEmptySheetContour()
{
    if (d->globalContour.isEmpty())
    {
        d->globalContour = CutEmptySheetEdge();
        d->globalContour.append(d->globalContour.first()); // Close path

        ResetAttributes();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::SetContour(const QVector<QPointF> &contour)
{
    d->globalContour = contour;

    ResetAttributes();
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VContour::GetContour() const
{
    return d->globalContour;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VContour::GetShift() const
{
    return d->shift;
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::SetShift(qreal shift)
{
    d->shift = shift;

    ResetAttributes();
}

//---------------------------------------------------------------------------------------------------------------------
int VContour::GetHeight() const
{
    return d->paperHeight;
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::SetHeight(int height)
{
    d->paperHeight = height;
}

//---------------------------------------------------------------------------------------------------------------------
int VContour::GetWidth() const
{
    return d->paperWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::SetWidth(int width)
{
    d->paperWidth = width;
}

//---------------------------------------------------------------------------------------------------------------------
QSizeF VContour::GetSize() const
{
    return QSizeF(d->paperWidth, d->paperHeight);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VContour::UniteWithContour(const VLayoutPiece &detail, int globalI, int detJ, BestFrom type) const
{
    QVector<QPointF> newContour;
    if (d->globalContour.isEmpty()) //-V807
    {
        AppendWhole(newContour, detail, 0);
    }
    else
    {
        if (globalI <= 0 || globalI > GlobalEdgesCount())
        {
            return QVector<QPointF>();
        }

        if (detJ <= 0 || detJ > detail.LayoutEdgesCount())
        {
            return QVector<QPointF>();
        }

        const int i2 = globalI == d->globalContour.count() ? 0 : globalI;

        int i=0;
        while (i < d->globalContour.count())
        {
            if (i == i2)
            {
                if (type == BestFrom::Rotation)
                {
                    AppendWhole(newContour, detail, detJ);
                }
                else
                {
                    InsertDetail(newContour, detail, detJ);
                }
            }

            AppendToContour(newContour, d->globalContour.at(i));
            ++i;
        }
    }
    return newContour;
}

//---------------------------------------------------------------------------------------------------------------------
int VContour::GlobalEdgesCount() const
{
    return d->m_emptySheetEdgesCount;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VContour::GlobalEdge(int i) const
{
    QLineF edge;
    if (d->globalContour.isEmpty()) //-V807
    {
        // Because sheet is blank we have one global edge for all cases.
        if (i < 1 || i > GlobalEdgesCount())
        { // Doesn't exist such edge
            return EmptySheetEdge();
        }

        const qreal nShift = EmptySheetEdge().length()/GlobalEdgesCount();
        edge = IsPortrait() ? QLineF(nShift*(i-1), 0, nShift*i, 0) : QLineF(0, nShift*(i-1), 0, nShift*i);
    }
    else
    {
        if (i < 1 || i > GlobalEdgesCount())
        { // Doesn't exist such edge
            return QLineF();
        }

        if (i < GlobalEdgesCount())
        {
            edge = QLineF(d->globalContour.at(i-1), d->globalContour.at(i));
        }
        else
        { // Closed countour
            edge = QLineF(d->globalContour.at(GlobalEdgesCount()-1), d->globalContour.at(0));
        }
    }
    return edge;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VContour::CutEdge(const QLineF &edge) const
{
    QVector<QPointF> points;
    if (qFuzzyIsNull(d->shift))
    {
        points.append(edge.p1());
        points.append(edge.p2());
    }
    else
    {
        const int n = qFloor(edge.length()/d->shift);

        if (n <= 0)
        {
            points.append(edge.p1());
            points.append(edge.p2());
        }
        else
        {
            points.reserve(n);
            const qreal nShift = edge.length()/n;
            for (int i = 1; i <= n+1; ++i)
            {
                QLineF l1 = edge;
                l1.setLength(nShift*(i-1));
                points.append(l1.p2());
            }
        }
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VContour::CutEmptySheetEdge() const
{
    QVector<QPointF> points;
    const qreal nShift = EmptySheetEdge().length()/GlobalEdgesCount();
    for (int i = 1; i <= GlobalEdgesCount()+1; ++i)
    {
        QLineF l1 = EmptySheetEdge();
        l1.setLength(nShift*(i-1));
        points.append(l1.p2());
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
const QPointF &VContour::at(int i) const
{
    return d->globalContour.at(i);
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::AppendWhole(QVector<QPointF> &contour, const VLayoutPiece &detail, int detJ) const
{
    int processedEdges = 0;
    const int nD = detail.LayoutEdgesCount();
    int j = detJ+1;
    do
    {
        if (j > nD)
        {
            j=1;
        }
        const QVector<QPointF> points = CutEdge(detail.LayoutEdge(j));
        for (int i = 0; i < points.size()-1; ++i)
        {
            contour.append(points.at(i));
        }
        ++processedEdges;
        ++j;
    }while (processedEdges < nD);
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::InsertDetail(QVector<QPointF> &contour, const VLayoutPiece &detail, int detJ) const
{
    int processedEdges = 0;
    const int nD = detail.LayoutEdgesCount();
    int j = detJ+1;
    do
    {
        if (j > nD)
        {
            j=1;
        }
        if (j != detJ)
        {
            for(auto &point : CutEdge(detail.LayoutEdge(j)))
            {
                AppendToContour(contour, point);
            }
        }
        ++processedEdges;
        ++j;
    }
    while (processedEdges <= nD);
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::ResetAttributes()
{
    if (not d->globalContour.isEmpty())
    {
        d->m_emptySheetEdgesCount = d->globalContour.count(); // Edges count
    }
    else
    {
        d->m_emptySheetEdgesCount = EmptySheetEdgesCount(); // Edges count
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VContour::EmptySheetEdgesCount() const
{
    qreal defaultShift = ToPixel(0.5, Unit::Cm);
    const qreal emptyEdgeLength = EmptySheetEdge().length();
    if (emptyEdgeLength < defaultShift)
    {
        defaultShift = emptyEdgeLength / 2.0;
    }

    const qreal shift = (qFuzzyIsNull(d->shift) || d->shift > defaultShift) ? defaultShift : d->shift;
    return qFloor(emptyEdgeLength/shift);
}

//---------------------------------------------------------------------------------------------------------------------
bool VContour::IsPortrait() const
{
    return d->paperHeight >= d->paperWidth;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VContour::EmptySheetEdge() const
{
    static const int offset = 2;
    const int layoutOffset = qCeil(d->layoutWidth + accuracyPointOnLine);
    return IsPortrait() ? QLineF(offset, -layoutOffset, d->paperWidth-offset, -layoutOffset) :
                          QLineF(-layoutOffset, offset, -layoutOffset, d->paperHeight-offset);
}
