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
#include <QImage>
#include <QDir>
#include <QPainter>
#include <QGraphicsItem>
#include <QCoreApplication>

class BestResult
{
public:
    BestResult();

    void NewResult(qint64 square, int i, int j, const QTransform &matrix, bool mirror);

    qint64  BestSquare() const;
    int     GContourEdge() const;
    int     DetailEdge() const;
    QTransform Matrix() const;
    bool    ValideResult() const;
    bool    Mirror() const;

private:
    // All nedded information about best result
    int resI; // Edge of global contour
    int resJ; // Edge of detail
    QTransform resMatrix; // Matrix for rotation and translation detail
    qint64 resSquare; // Best square size (least). For begin set max value.
    bool valideResult;
    bool resMirror;
};

//===================================================BestResult========================================================
//---------------------------------------------------------------------------------------------------------------------
BestResult::BestResult()
    :resI(0), resJ(0), resMatrix(QMatrix()), resSquare(LLONG_MAX), valideResult(false), resMirror(false)
{}

//---------------------------------------------------------------------------------------------------------------------
void BestResult::NewResult(qint64 square, int i, int j, const QTransform &matrix, bool mirror)
{
    if (square <= resSquare && square > 0)
    {
        resI = i;
        resJ = j;
        resMatrix = matrix;
        resSquare = square;
        valideResult = true;
        resMirror = mirror;
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
QTransform BestResult::Matrix() const
{
    return resMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
bool BestResult::ValideResult() const
{
    return valideResult;
}

//---------------------------------------------------------------------------------------------------------------------
bool BestResult::Mirror() const
{
    return resMirror;
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
qreal VLayoutPaper::GetLayoutWidth() const
{
    return d->layoutWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetLayoutWidth(qreal width)
{
    if (width >= 0)
    {
        d->layoutWidth = width;
    }
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
void VLayoutPaper::SetPaperIndex(quint32 index)
{
    d->paperIndex = index;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::ArrangeDetail(const VLayoutDetail &detail, bool &stop)
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

    d->frame = 0;

    if (Count() == 0)
    {
        return AddToBlankSheet(detail, stop);
    }
    else
    {
        return AddToSheet(detail, stop);
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::Count() const
{
    return d->details.count();
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::AddToBlankSheet(const VLayoutDetail &detail, bool &stop)
{
    BestResult bestResult;

    for (int j=1; j <= EdgesCount(); ++j)
    {
        for (int i=1; i<= detail.EdgesCount(); i++)
        {
            QCoreApplication::processEvents();

            if (stop)
            {
                return false;
            }

            // We should use copy of the detail.
            VLayoutDetail workDetail = detail;

            int dEdge = i;// For mirrored detail edge will be different
            if (CheckCombineEdges(workDetail, j, dEdge))
            {
                const QRectF rec = workDetail.BoundingRect();
                if (SheetContains(rec))
                {
                    bestResult.NewResult(static_cast<qint64>(rec.width()*rec.height()), j, dEdge,
                                         workDetail.GetMatrix(), workDetail.IsMirror());
                }
            }
            d->frame = d->frame + 2;

            for (int angle = 0; angle <= 360; angle = angle+20)
            {
                QCoreApplication::processEvents();

                if (stop)
                {
                    return false;
                }

                // We should use copy of the detail.
                VLayoutDetail workDetail = detail;

                if (CheckRotationEdges(workDetail, j, i, angle))
                {
                    const QRectF rec = workDetail.BoundingRect();
                    if (SheetContains(rec))
                    {
                        bestResult.NewResult(static_cast<qint64>(rec.width()*rec.height()), j, i,
                                             workDetail.GetMatrix(), workDetail.IsMirror());
                    }
                }
                ++d->frame;
            }
        }
    }

    return SaveResult(bestResult, detail);
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::AddToSheet(const VLayoutDetail &detail, bool &stop)
{
    BestResult bestResult;

    for (int j=1; j <= EdgesCount(); ++j)
    {
        // We should use copy of the detail.
        VLayoutDetail workDetail = detail;

        for (int i=1; i<= workDetail.EdgesCount(); i++)
        {
            QCoreApplication::processEvents();

            if (stop)
            {
                return false;
            }

            int dEdge = i;// For mirror detail edge will be different
            if (CheckCombineEdges(workDetail, j, dEdge))
            {
                if (SheetContains(workDetail.BoundingRect()))
                {
                    QVector<QPointF> newGContour = UniteWithContour(workDetail, j, dEdge);
                    newGContour.append(newGContour.first());
                    const QRectF rec = QPolygonF(newGContour).boundingRect();
                    bestResult.NewResult(static_cast<qint64>(rec.width()*rec.height()), j, dEdge,
                                         workDetail.GetMatrix(), workDetail.IsMirror());
                }
            }
            d->frame = d->frame + 2;
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

#ifdef LAYOUT_DEBUG
#   ifdef SHOW_COMBINE
        DrawDebug(detail, d->frame);
#   endif
#endif

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
        #ifdef LAYOUT_DEBUG
            #ifdef SHOW_MIRROR
                DrawDebug(detail, d->frame+1);
            #endif
        #endif

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

#ifdef LAYOUT_DEBUG
    #ifdef SHOW_ROTATION
        DrawDebug(detail, d->frame);
    #endif
#endif

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
VLayoutPaper::CrossingType VLayoutPaper::Crossing(const VLayoutDetail &detail, const int &globalI,
                                                  const int &detailI) const
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
                if (TrueIntersection(globalEdge, detailEdge, xPoint))
                {
                    return CrossingType::Intersection;
                }
            }
        }
    }

    return CrossingType::NoIntersection;
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper::InsideType VLayoutPaper::InsideContour(const VLayoutDetail &detail, const int &detailI) const
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
        const int polyCorners = EdgesCount();
        int j = polyCorners-1;

        QVector<qreal> constant;
        QVector<qreal> multiple;

        for(int i=0; i<polyCorners; i++)
        {
            const qreal xi = d->globalContour.at(i).x();
            const qreal xj = d->globalContour.at(j).x();
            const qreal yi = d->globalContour.at(i).y();
            const qreal yj = d->globalContour.at(j).y();
            if(qFuzzyCompare(yj, yi))
            {
                constant.insert(i, xi);
                multiple.insert(i, 0);
            }
            else
            {
                constant.insert(i, xi - (yi*xj)/(yj-yi) + (yi*xi)/(yj-yi));
                multiple.insert(i, (xj-xi)/(yj-yi));
            }

            j=i;
        }

        for(int m = 1; m <= detail.EdgesCount(); ++m)
        {
            if (m == detailI)
            {
                continue;
            }

            const QLineF detailEdge = detail.Edge(m);
            if (detailEdge.isNull()) // Got null edge
            {
                return InsideType::EdgeError;
            }

            const QVector<QPointF> p = Triplet(detailEdge);
            for (int n=0; n<p.size(); ++n )
            {
                if (p.at(n) != detailEdge.p1() && p.at(n) != detailEdge.p2())
                {
                    int j = polyCorners-1;
                    bool oddNodes = false;

                    for (int i=0; i<polyCorners; i++)
                    {
                        const qreal yi = d->globalContour.at(i).y();
                        const qreal yj = d->globalContour.at(j).y();

                        if (((yi < p.at(n).y() && yj >= p.at(n).y()) || (yj < p.at(n).y() && yi >= p.at(n).y())))
                        {
                            oddNodes ^= (p.at(n).y() * multiple.at(i) + constant.at(i) < p.at(n).x());
                        }

                        j=i;
                    }

                    if (oddNodes)
                    {
                        return InsideType::Inside;
                    }
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
void VLayoutPaper::CombineEdges(VLayoutDetail &detail, const QLineF &globalEdge, const int &dEdge) const
{
    QLineF detailEdge = detail.Edge(dEdge);

    // Find distance between two edges for two begin vertex.
    const qreal dx = globalEdge.x2() - detailEdge.x2();
    const qreal dy = globalEdge.y2() - detailEdge.y2();

    detailEdge.translate(dx, dy); // Use values for translate detail edge.

    const qreal angle_between = globalEdge.angleTo(detailEdge); // Seek angle between two edges.

    // Now we move detail to position near to global contour edge.
    detail.Translate(dx, dy);
    detail.Rotate(detailEdge.p2(), -angle_between);
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
                j=1;
            }
            const QVector<QPointF> points = CutEdge(detail.Edge(j));
            for (int i = 0; i < points.size()-1; ++i)
            {
                newContour.append(points.at(i));
            }
            ++processedEdges;
            ++j;
        }while (processedEdges <= nD);
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
        while(i < d->globalContour.count())
        {
            if (i == i2)
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
QLineF VLayoutPaper::GlobalEdge(int i) const
{
    if (d->details.isEmpty())
    {
        // Because sheet is blank we have one global edge for all cases - Ox axis.
        const QLineF axis = QLineF(0, 0, d->paperWidth - d->layoutWidth/2, 0);
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
        workDetail.SetMirror(bestResult.Mirror());
        const QVector<QPointF> newGContour = UniteWithContour(workDetail, bestResult.GContourEdge(),
                                                              bestResult.DetailEdge());
        if (newGContour.isEmpty())
        {
            return false;
        }
        d->details.append(workDetail);
        d->globalContour = newGContour;

#ifdef LAYOUT_DEBUG
#   ifdef SHOW_BEST
        DrawDebug(workDetail, UINT_MAX);
#   endif
#endif
    }

    return bestResult.ValideResult(); // Do we have the best result?
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::DrawDebug(const VLayoutDetail &detail, int frame) const
{
    QImage frameImage(d->paperWidth*2, d->paperHeight*2, QImage::Format_RGB32);
    frameImage.fill(Qt::white);
    QPainter paint;
    paint.begin(&frameImage);

    paint.setPen(QPen(Qt::darkRed, 3, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    paint.drawRect(QRectF(d->paperWidth/2, d->paperHeight/2, d->paperWidth, d->paperHeight));

    paint.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    QPainterPath p;
    if (d->globalContour.isEmpty())
    {
        p = DrawContour(CutEdge(QLineF(0, 0, d->paperWidth, 0)));
        p.translate(d->paperWidth/2, d->paperHeight/2);
        paint.drawPath(p);
    }
    else
    {
        p = DrawContour(d->globalContour);
        p.translate(d->paperWidth/2, d->paperHeight/2);
        paint.drawPath(p);
    }

#ifdef SHOW_CANDIDATE
    paint.setPen(QPen(Qt::darkGreen, 3, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p = DrawContour(detail.GetLayoutAllowencePoints());
    p.translate(d->paperWidth/2, d->paperHeight/2);
    paint.drawPath(p);
#endif

#ifdef ARRANGED_DETAILS
    paint.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p = DrawDetails();
    p.translate(d->paperWidth/2, d->paperHeight/2);
    paint.drawPath(p);
#endif

    paint.end();
    const QString path = QDir::homePath()+QStringLiteral("/LayoutDebug/")+QString("%1_%2_%3.png").arg(d->paperIndex)
            .arg(d->details.count()).arg(frame);
    frameImage.save (path);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPaper::ShowDirection(const QLineF &edge) const
{
    QLineF arrow = edge;
    arrow.setLength(edge.length()/2.0);

    //Reverse line because we want start arrow from this point
    arrow = QLineF(arrow.p2(), arrow.p1());
    const qreal angle = arrow.angle();//we each time change line angle, better save original angle value
    arrow.setLength(14);//arrow length in pixels

    QPainterPath path;
    arrow.setAngle(angle-35);
    path.moveTo(arrow.p1());
    path.lineTo(arrow.p2());

    arrow.setAngle(angle+35);
    path.moveTo(arrow.p1());
    path.lineTo(arrow.p2());
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPaper::DrawContour(const QVector<QPointF> &points) const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    if (points.count() >= 2)
    {
        for (qint32 i = 0; i < points.count()-1; ++i)
        {
            path.moveTo(points.at(i));
            path.lineTo(points.at(i+1));
        }
        path.lineTo(points.at(0));

#ifdef SHOW_DIRECTION
        for (qint32 i = 0; i < points.count()-1; ++i)
        {
            path.addPath(ShowDirection(QLineF(points.at(i), points.at(i+1))));
        }
#endif

#ifdef SHOW_VERTICES
        for (qint32 i = 0; i < points.count(); ++i)
        {
            path.addRect(points.at(i).x()-3, points.at(i).y()-3, 6, 6);
        }
#endif
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPaper::TranslateContour(const QVector<QPointF> &points, qreal dx, qreal dy) const
{
    QVector<QPointF> p;
    for (qint32 i = 0; i < points.count(); ++i)
    {
        p.append(QPointF(points.at(i).x()+dx, points.at(i).y()+dy));
    }
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPaper::DrawDetails() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    if (Count() > 0)
    {
        for (int i = 0; i < d->details.size(); ++i)
        {
            path.addPath(d->details.at(i).ContourPath());
        }
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::TrueIntersection(const QLineF &gEdge, const QLineF &dEdge, const QPointF &p) const
{
    const QPointF pX = RoundedPoint(p);
    const QPointF gP1 = RoundedPoint(gEdge.p1());
    const QPointF gP2 = RoundedPoint(gEdge.p2());
    const QPointF dP1 = RoundedPoint(dEdge.p1());
    const QPointF dP2 = RoundedPoint(dEdge.p2());
    return !(pX == gP1 || pX == gP2 || pX == dP1 || pX == dP2);
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VLayoutPaper::RoundedPoint(const QPointF &p) const
{
    return QPointF(qRound(p.x()), qRound(p.y()));
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPaper::Triplet(const QLineF &edge) const
{
    QVector<QPointF> p;
    QLineF line = edge;
    line.setLength(edge.length()/2);

    p.append(edge.p1());
    p.append(line.p2());
    p.append(edge.p2());
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsItem *VLayoutPaper::GetItem() const
{
    QGraphicsRectItem *paper = new QGraphicsRectItem(QRectF(0, 0, d->paperWidth, d->paperHeight));
    paper->setPen(QPen(Qt::black, 1));
    paper->setBrush(QBrush(Qt::white));
    for (int i=0; i < d->details.count(); ++i)
    {
        QGraphicsItem *item = d->details.at(i).GetItem();
        item->setParentItem(paper);
    }
    return paper;
}
