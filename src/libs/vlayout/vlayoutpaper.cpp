/************************************************************************
 **
 **  @file   vlayoutpaper.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vlayoutpaper.h"
#include "vlayoutpaper_p.h"

#include <climits>
#include <QPointF>
#include <QtMath>

class BestResult
{
public:
    BestResult();

    void NewResult(qint64 square, int i, int j, const QMatrix &matrix);

    qint64  BestSquare() const;
    int     GContourEdge() const;
    int     DetailEdge() const;
    QMatrix Matrix() const;
    bool    ValideResult() const;

private:
    // All nedded information about best result
    int resI; // Edge of global contour
    int resJ; // Edge of detail
    QMatrix resMatrix; // Matrix for rotation and translation detail
    qint64 resSquare; // Best square size (least). For begin set max value.
    bool valideResult;
};

//===================================================BestResult========================================================
//---------------------------------------------------------------------------------------------------------------------
BestResult::BestResult()
    :resI(0), resJ(0), resMatrix(QMatrix()), resSquare(LLONG_MAX),valideResult(false)
{}

//---------------------------------------------------------------------------------------------------------------------
void BestResult::NewResult(qint64 square, int i, int j, const QMatrix &matrix)
{
    if (square <= resSquare && square > 0)
    {
        resI = i;
        resJ = j;
        resMatrix = matrix;
        resSquare = square;
        valideResult = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qint64 BestResult::BestSquare() const
{
    return resSquare;
}

//---------------------------------------------------------------------------------------------------------------------
int BestResult::GContourEdge() const
{
    return resI;
}

//---------------------------------------------------------------------------------------------------------------------
int BestResult::DetailEdge() const
{
    return resJ;
}

//---------------------------------------------------------------------------------------------------------------------
QMatrix BestResult::Matrix() const
{
    return resMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
bool BestResult::ValideResult() const
{
    return valideResult;
}

//===================================================VLayoutPaper======================================================
//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper::VLayoutPaper()
    :d(new VLayoutPaperData)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper::VLayoutPaper(int height, int width)
    :d(new VLayoutPaperData(height, width))
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper::VLayoutPaper(const VLayoutPaper &paper)
    :d (paper.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper &VLayoutPaper::operator=(const VLayoutPaper &paper)
{
    if ( &paper == this )
    {
        return *this;
    }
    d = paper.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper::~VLayoutPaper()
{}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::GetHeight() const
{
    return d->paperHeight;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetHeight(int height)
{
    d->paperHeight = height;
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::GetWidth() const
{
    return d->paperWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetWidth(int width)
{
    d->paperWidth = width;
}

//---------------------------------------------------------------------------------------------------------------------
unsigned int VLayoutPaper::GetShift() const
{
    return d->shift;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetShift(unsigned int shift)
{
    d->shift = shift;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::ArrangeDetail(const VLayoutDetail &detail)
{
    // First need set size of paper
    if (d->paperHeight <= 0 || d->paperWidth <= 0)
    {
        return false;
    }

    if (detail.EdgesCount() < 3)
    {
        return false;//Not enough edges
    }

    if (Count() == 0)
    {
        return AddToBlankSheet(detail);
    }
    else
    {
        return AddToSheet(detail);
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::Count() const
{
    return d->details.count();
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::AddToBlankSheet(const VLayoutDetail &detail)
{
    BestResult bestResult;

    for (int j=1; j <= EdgesCount(); ++j)
    {
        for (int i=1; i<= detail.EdgesCount(); i++)
        {
            // We should use copy of the detail.
            VLayoutDetail workDetail = detail;

            int dEdge = i;// For mirrored detail edge will be different
            if (CheckCombineEdges(workDetail, j, dEdge))
            {
                const QRectF rec = workDetail.BoundingRect();
                if (SheetContains(rec))
                {
                    bestResult.NewResult(static_cast<qint64>(rec.width()*rec.height()), j, dEdge,
                                         workDetail.GetMatrix());
                }
            }

            for (int angle = 0; angle < 360; ++angle)
            {
                // We should use copy of the detail.
                VLayoutDetail workDetail = detail;

                if (CheckRotationEdges(workDetail, j, i, angle))
                {
                    const QRectF rec = workDetail.BoundingRect();
                    if (SheetContains(rec))
                    {
                        bestResult.NewResult(static_cast<qint64>(rec.width()*rec.height()), j, i,
                                             workDetail.GetMatrix());
                    }
                }
            }
        }
    }

    return SaveResult(bestResult, detail);
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::AddToSheet(const VLayoutDetail &detail)
{
    BestResult bestResult;

    for (int j=1; j <= EdgesCount(); ++j)
    {
        // We should use copy of the detail.
        VLayoutDetail workDetail = detail;

        for (int i=1; i<= workDetail.EdgesCount(); i++)
        {
            int dEdge = i;// For mirror detail edge will be different
            if (CheckCombineEdges(workDetail, j, dEdge))
            {
                if (SheetContains(workDetail.BoundingRect()))
                {
                    QVector<QPointF> newGContour = UniteWithContour(workDetail, j, dEdge);
                    newGContour.append(newGContour.first());
                    const QRectF rec = QPolygonF(newGContour).boundingRect();
                    bestResult.NewResult(static_cast<qint64>(rec.width()*rec.height()), j, dEdge,
                                         workDetail.GetMatrix());
                }
                else
                {
                    continue; // Outside of sheet.
                }
            }
        }
    }

    return SaveResult(bestResult, detail);
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::CheckCombineEdges(VLayoutDetail &detail, int j, int &dEdge) const
{
    const QLineF globalEdge = GlobalEdge(j);
    bool flagMirror = false;
    bool flagSquare = false;

    CombineEdges(detail, globalEdge, dEdge);

    switch (Crossing(detail, j, dEdge))
    {
        case CrossingType::EdgeError:
            return false;
        case CrossingType::Intersection:
            detail.Mirror(globalEdge);
            flagMirror = true;
            break;
        case CrossingType::NoIntersection:
        {
            switch (InsideContour(detail, dEdge))
            {
                case InsideType::EdgeError:
                    return false;
                case InsideType::Inside:
                    detail.Mirror(globalEdge);
                    flagMirror = true;
                    break;
                case InsideType::Outside:
                    flagSquare = true;
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }

    if (flagMirror)
    {
        dEdge = detail.EdgeByPoint(globalEdge.p2());
        if (dEdge <= 0)
        {
            return false;
        }

        switch (Crossing(detail, j, dEdge))
        {
            case CrossingType::EdgeError:
                return false;
            case CrossingType::Intersection:
                flagSquare = false;
                break;
            case CrossingType::NoIntersection:
            {
                switch (InsideContour(detail, dEdge))
                {
                    case InsideType::EdgeError:
                        return false;
                    case InsideType::Inside:
                        flagSquare = false;
                        break;
                    case InsideType::Outside:
                        flagSquare = true;
                        break;
                    default:
                        break;
                }
            }
            default:
                break;
        }
    }
    return flagSquare;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::CheckRotationEdges(VLayoutDetail &detail, int j, int dEdge, int angle) const
{
    const QLineF globalEdge = GlobalEdge(j);
    bool flagSquare = false;

    RotateEdges(detail, globalEdge, dEdge, angle);

    switch (Crossing(detail, j, dEdge))
    {
        case CrossingType::EdgeError:
            return false;
        case CrossingType::Intersection:
            flagSquare = false;
            break;
        case CrossingType::NoIntersection:
        {
            switch (InsideContour(detail, dEdge))
            {
                case InsideType::EdgeError:
                    return false;
                case InsideType::Inside:
                    flagSquare = false;
                    break;
                case InsideType::Outside:
                    flagSquare = true;
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }
    return flagSquare;
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper::CrossingType VLayoutPaper::Crossing(const VLayoutDetail &detail, int globalI, int detailI) const
{
    int globalEdgesCount = EdgesCount();
    if (globalEdgesCount == 0)
    {
        globalEdgesCount = 1;// For blank sheet
    }

    const int detailEdgesCount = detail.EdgesCount();
    if (detailEdgesCount < 3)
    {
        return CrossingType::EdgeError;
    }

    for(int i = 1; i <= globalEdgesCount; i++)
    {
        const QLineF globalEdge = GlobalEdge(i);
        if (globalEdge.isNull()) // Got null edge
        {
            return CrossingType::EdgeError;
        }

        for(int j = 1; j <= detailEdgesCount; j++)
        {
            if (i == globalI && j == detailI)
            {
                continue;
            }

            const QLineF detailEdge = detail.Edge(j);
            if (detailEdge.isNull()) // Got null edge
            {
                return CrossingType::EdgeError;
            }

            QPointF xPoint;
            QLineF::IntersectType type = globalEdge.intersect(detailEdge, &xPoint);

            if (type == QLineF::BoundedIntersection)
            {
                return CrossingType::Intersection;
            }
        }
    }

    return CrossingType::NoIntersection;
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper::InsideType VLayoutPaper::InsideContour(const VLayoutDetail &detail, int detailI) const
{
    if (detail.EdgesCount() < 3)
    {
        return InsideType::EdgeError;
    }

    const QVector<QPointF> lPoints = detail.GetLayoutAllowencePoints();

    const QLineF detailEdge = detail.Edge(detailI);
    if (detailEdge.isNull()) // Got null edge
    {
        return InsideType::EdgeError;
    }

    if (d->details.isEmpty())
    {
        const QLineF globalEdge = GlobalEdge(1);
        for(int i = 0; i < lPoints.count(); i++)
        {
            if (CheckSide(globalEdge, lPoints.at(i)) < 0)
            {
                return InsideType::Inside;
            }
        }
    }
    else
    {
        const QPolygonF gPoly = GlobalPolygon();
        for(int i = 0; i < lPoints.count(); i++)
        {
            const QPointF p = lPoints.at(i);
            if (p.isNull())
            {
                return InsideType::EdgeError;
            }

            if (p != detailEdge.p1() && p != detailEdge.p2())
            {
                if (gPoly.containsPoint(p, Qt::OddEvenFill))
                {
                    return InsideType::Inside;
                }
            }
        }
    }
    return InsideType::Outside;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutPaper::CheckSide(const QLineF &edge, const QPointF &p) const
{
    return (edge.x2() - edge.x1()) * (p.y() - edge.y1()) - (edge.y2() - edge.y1()) * (p.x() - edge.x1());
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::SheetContains(const QRectF &rect) const
{
    const QRectF bRect(0, 0, d->paperWidth, d->paperHeight);
    return bRect.contains(rect);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::CombineEdges(VLayoutDetail &detail, const QLineF &globalEdge, int dEdge) const
{
    QLineF detailEdge = detail.Edge(dEdge);

    // Find distance between two edges for two begin vertex.
    const qreal dx = globalEdge.x2() - detailEdge.x2();
    const qreal dy = globalEdge.y2() - detailEdge.y2();

    detailEdge.translate(dx, dy); // Use values for translate detail edge.

    const qreal angle_between = globalEdge.angleTo(detailEdge); // Seek angle between two edges.

    // Now we move detail to position near to global contour edge.
    detail.Translate(dx, dy);
    detail.Rotate(globalEdge.p2(), angle_between);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::RotateEdges(VLayoutDetail &detail, const QLineF &globalEdge, int dEdge, int angle) const
{
    QLineF detailEdge = detail.Edge(dEdge);

    // Find distance between two edges for two begin vertex.
    const qreal dx = globalEdge.x2() - detailEdge.x2();
    const qreal dy = globalEdge.y2() - detailEdge.y2();

    detailEdge.translate(dx, dy); // Use values for translate detail edge.

    // Now we move detail to position near to global contour edge.
    detail.Translate(dx, dy);
    detail.Rotate(globalEdge.p2(), angle);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPaper::UniteWithContour(const VLayoutDetail &detail, int globalI, int detJ) const
{
    QVector<QPointF> newContour;
    if (d->globalContour.isEmpty())
    {
        int processedEdges = 0;
        const int nD = detail.EdgesCount();
        int j = detJ+1;
        do
        {
            if (j > nD)
            {
                j=0;
            }
            const QVector<QPointF> points = CutEdge(detail.Edge(j));
            for (int i = 0; i < points.size()-1; ++i)
            {
                newContour.append(points.at(i));
            }
            ++processedEdges;
            ++j;
        }while (processedEdges < nD);
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

        for(int i=0; i < d->globalContour.count(); ++i)
        {
            newContour.append(d->globalContour.at(i));
            ++i;
            if (i==globalI)
            {
                int processedEdges = 0;
                const int nD = detail.EdgesCount();
                int j = detJ+1;
                do
                {
                    if (j > nD)
                    {
                        j=0;
                    }
                    const QVector<QPointF> points = CutEdge(detail.Edge(j));
                    for (int i = 0; i < points.size()-1; ++i)
                    {
                        newContour.append(points.at(i));
                    }
                    ++processedEdges;
                    ++j;
                }while (processedEdges < nD);
            }
        }
    }
    return newContour;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VLayoutPaper::GlobalEdge(int i) const
{
    if (d->details.isEmpty())
    {
        // Because sheet is blank we have one global edge for all cases - Ox axis.
        const QLineF axis = QLineF(0, 0, d->paperWidth, 0);
        if (d->shift == 0)
        {
            return axis;
        }

        const int n = qFloor(axis.length()/d->shift);

        if (i < 1 || i > n)
        { // Doesn't exist such edge
            return QLineF();
        }

        if (n <= 0)
        {
            return axis;
        }
        else
        {
            const qreal nShift = axis.length()/n;
            return QLineF(nShift*(i-1), 0, nShift*i, 0);
        }
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
int VLayoutPaper::EdgesCount() const
{
    if (d->details.isEmpty())
    {
        if (d->shift == 0)
        {
            return 1;
        }

        const QLineF axis = QLineF(0, 0, d->paperWidth, 0);
        const int n = qFloor(axis.length()/d->shift);
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
QPolygonF VLayoutPaper::GlobalPolygon() const
{
    QVector<QPointF> points = d->globalContour;
    points.append(points.first());
    return QPolygonF(points);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPaper::CutEdge(const QLineF &edge) const
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
bool VLayoutPaper::SaveResult(const BestResult &bestResult, const VLayoutDetail &detail)
{
    if (bestResult.ValideResult())
    {
        VLayoutDetail workDetail = detail;
        workDetail.SetMatrix(bestResult.Matrix());// Don't forget set matrix
        const QVector<QPointF> newGContour = UniteWithContour(workDetail, bestResult.GContourEdge(),
                                                              bestResult.DetailEdge());
        if (newGContour.isEmpty())
        {
            return false;
        }
        d->details.append(workDetail);
        d->globalContour = newGContour;
    }

    return bestResult.ValideResult(); // Do we have the best result?
}
