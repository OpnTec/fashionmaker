/************************************************************************
 **
 **  @file   vcontour.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 1, 2015
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

#include "vcontour.h"
#include "vcontour_p.h"
#include "vlayoutdetail.h"

#include <QPointF>
#include <QLineF>
#include <QtMath>

//---------------------------------------------------------------------------------------------------------------------
VContour::VContour()
    :d(new VContourData())
{}

//---------------------------------------------------------------------------------------------------------------------
VContour::VContour(int height, int width)
    :d(new VContourData(height, width))
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
void VContour::SetContour(const QVector<QPointF> &contour)
{
    d->globalContour = contour;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VContour::GetContour() const
{
    return d->globalContour;
}

//---------------------------------------------------------------------------------------------------------------------
unsigned int VContour::GetShift() const
{
    return d->shift;
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::SetShift(unsigned int shift)
{
    d->shift = shift;
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
QVector<QPointF> VContour::UniteWithContour(const VLayoutDetail &detail, int globalI, int detJ, BestFrom type) const
{
    QVector<QPointF> newContour;
    if (d->globalContour.isEmpty())
    {
        AppendWhole(newContour, detail, detJ);
    }
    else
    {
        if (globalI <= 0 || globalI > EdgesCount())
        {
            return QVector<QPointF>();
        }

        if (detJ <= 0 || detJ > detail.EdgesCount())
        {
            return QVector<QPointF>();
        }

        int i2 = 0;
        if (globalI == d->globalContour.count())
        {
            i2 = 0;
        }
        else
        {
            i2 = globalI;
        }

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
                    int processedEdges = 0;
                    const int nD = detail.EdgesCount();
                    int j = detJ+1;
                    do
                    {
                        if (j > nD)
                        {
                            j=1;
                        }
                        if (j != detJ)
                        {
                            const QVector<QPointF> points = CutEdge(detail.Edge(j));
                            for (int i = 0; i < points.size()-1; ++i)
                            {
                                newContour.append(points.at(i));
                            }
                        }
                        ++processedEdges;
                        ++j;
                    }while (processedEdges < nD);
                }
            }

            if (newContour.isEmpty() == false)
            {
                if (newContour.last() != d->globalContour.at(i))
                {
                    newContour.append(d->globalContour.at(i));
                }
            }
            else
            {
                newContour.append(d->globalContour.at(i));
            }
            ++i;
        }
    }
    return newContour;
}

//---------------------------------------------------------------------------------------------------------------------
int VContour::EdgesCount() const
{
    if (d->globalContour.isEmpty())
    {
        if (d->shift == 0)
        {
            return 1;
        }

        const int n = qFloor(EmptySheetEdge().length()/d->shift);
        if (n <= 0)
        {
            return 1;
        }
        else
        {
            return n;
        }
    }
    else
    {
        return d->globalContour.count();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VContour::GlobalEdge(int i) const
{
    if (d->globalContour.isEmpty())
    {
        // Because sheet is blank we have one global edge for all cases - Ox axis.
        const QLineF axis = EmptySheetEdge();
        if (d->shift == 0)
        {
            return axis;
        }

        const int n = qFloor(axis.length()/d->shift);

        if (i < 1 || i > n)
        { // Doesn't exist such edge
            return QLineF();
        }

        const qreal nShift = axis.length()/n;
        return QLineF(nShift*(i-1), 0, nShift*i, 0);
    }
    else
    {
        if (i < 1 || i > EdgesCount())
        { // Doesn't exist such edge
            return QLineF();
        }
        QLineF edge;
        if (i < EdgesCount())
        {
            edge = QLineF(d->globalContour.at(i-1), d->globalContour.at(i));
        }
        else
        { // Closed countour
            edge = QLineF(d->globalContour.at(EdgesCount()-1), d->globalContour.at(0));
        }
        return edge;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VContour::CutEdge(const QLineF &edge) const
{
    QVector<QPointF> points;
    if (d->shift == 0)
    {
        points.append(edge.p1());
        points.append(edge.p2());
    }

    const int n = qFloor(edge.length()/d->shift);

    if (n <= 0)
    {
        points.append(edge.p1());
        points.append(edge.p2());
    }
    else
    {
        const qreal nShift = edge.length()/n;
        for (int i = 1; i <= n+1; ++i)
        {
            QLineF l1 = edge;
            l1.setLength(nShift*(i-1));
            points.append(l1.p2());
        }
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
const QPointF &VContour::at(int i) const
{
    return d->globalContour.at(i);
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::AppendWhole(QVector<QPointF> &contour, const VLayoutDetail &detail, int detJ) const
{
    int processedEdges = 0;
    const int nD = detail.EdgesCount();
    int j = detJ+1;
    do
    {
        if (j > nD)
        {
            j=1;
        }
        const QVector<QPointF> points = CutEdge(detail.Edge(j));
        for (int i = 0; i < points.size()-1; ++i)
        {
            contour.append(points.at(i));
        }
        ++processedEdges;
        ++j;
    }while (processedEdges < nD);
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VContour::EmptySheetEdge() const
{
    return QLineF(0, 0, d->paperWidth - 5, 0);
}
