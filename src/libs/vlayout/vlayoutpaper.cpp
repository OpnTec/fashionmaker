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
#include "vbestsquare.h"

#include <climits>
#include <QPointF>
#include <QtMath>
#include <QImage>
#include <QDir>
#include <QPainter>
#include <QGraphicsItem>
#include <QCoreApplication>

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
    return d->globalContour.GetHeight();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetHeight(int height)
{
    d->globalContour.SetHeight(height);
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::GetWidth() const
{
    return d->globalContour.GetWidth();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetWidth(int width)
{
    d->globalContour.SetWidth(width);
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
    return d->globalContour.GetShift();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetShift(unsigned int shift)
{
    d->globalContour.SetShift(shift);
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
    if (d->globalContour.GetHeight() <= 0 || d->globalContour.GetWidth() <= 0)
    {
        return false;
    }

    if (detail.EdgesCount() < 3)
    {
        return false;//Not enough edges
    }

    d->frame = 0;

    return AddToSheet(detail, stop);
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::Count() const
{
    return d->details.count();
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::AddToSheet(const VLayoutDetail &detail, bool &stop)
{
    VBestSquare bestResult;

    for (int j=1; j <= d->globalContour.EdgesCount(); ++j)
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

            int dEdge = i;// For mirror detail edge will be different
            if (CheckCombineEdges(workDetail, j, dEdge))
            {
                #ifdef LAYOUT_DEBUG
                #   ifdef SHOW_CANDIDATE_BEST
                        DrawDebug(d->globalContour, workDetail, d->frame+2, d->paperIndex, d->details.count(),
                                  d->details);
                #   endif
                #endif

                SaveCandidate(bestResult, workDetail, j, dEdge, BestFrom::Combine);
            }
            d->frame = d->frame + 3;

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
                    #ifdef LAYOUT_DEBUG
                    #   ifdef SHOW_CANDIDATE_BEST
                            ++d->frame;
                            DrawDebug(d->globalContour, workDetail, d->frame, d->paperIndex, d->details.count(),
                                      d->details);
                    #   endif
                    #endif

                    SaveCandidate(bestResult, workDetail, j, i, BestFrom::Rotation);
                }
                ++d->frame;
            }
        }
    }

    return SaveResult(bestResult, detail);
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::CheckCombineEdges(VLayoutDetail &detail, int j, int &dEdge) const
{
    const QLineF globalEdge = d->globalContour.GlobalEdge(j);
    bool flagMirror = false;
    bool flagSquare = false;

    CombineEdges(detail, globalEdge, dEdge);

#ifdef LAYOUT_DEBUG
#   ifdef SHOW_COMBINE
        DrawDebug(d->globalContour, detail, d->frame, d->paperIndex, d->details.count(), d->details);
#   endif
#endif

    CrossingType type = CrossingType::Intersection;
    if (SheetContains(detail.BoundingRect()))
    {
        type = Crossing(detail, j, dEdge);
    }

    switch (type)
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
                DrawDebug(d->globalContour, detail, d->frame+1, d->paperIndex, d->details.count(), d->details);
            #endif
        #endif

        dEdge = detail.EdgeByPoint(globalEdge.p2());
        if (dEdge <= 0)
        {
            return false;
        }

        CrossingType type = CrossingType::Intersection;
        if (SheetContains(detail.BoundingRect()))
        {
            type = Crossing(detail, j, dEdge);
        }

        switch (type)
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
    const QLineF globalEdge = d->globalContour.GlobalEdge(j);
    bool flagSquare = false;

    RotateEdges(detail, globalEdge, dEdge, angle);

#ifdef LAYOUT_DEBUG
    #ifdef SHOW_ROTATION
        DrawDebug(d->globalContour, detail, d->frame, d->paperIndex, d->details.count(), d->details);
    #endif
#endif

    CrossingType type = CrossingType::Intersection;
    if (SheetContains(detail.BoundingRect()))
    {
        type = Crossing(detail, j, dEdge);
    }

    switch (type)
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
    int globalEdgesCount = d->globalContour.EdgesCount();
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
        const QLineF globalEdge = d->globalContour.GlobalEdge(i);
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
                if (TrueIntersection(d->globalContour.GlobalEdge(globalI), detail.Edge(detailI), xPoint))
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
        const QLineF globalEdge = d->globalContour.GlobalEdge(1);
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
        const int polyCorners = d->globalContour.EdgesCount();
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
    const QRectF bRect(0, 0, d->globalContour.GetWidth(), d->globalContour.GetHeight());
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
bool VLayoutPaper::SaveResult(const VBestSquare &bestResult, const VLayoutDetail &detail)
{
    if (bestResult.ValideResult())
    {
        VLayoutDetail workDetail = detail;
        workDetail.SetMatrix(bestResult.Matrix());// Don't forget set matrix
        workDetail.SetMirror(bestResult.Mirror());
        const QVector<QPointF> newGContour = d->globalContour.UniteWithContour(workDetail, bestResult.GContourEdge(),
                                                                               bestResult.DetailEdge(),
                                                                               bestResult.Type());
        if (newGContour.isEmpty())
        {
            return false;
        }
        d->details.append(workDetail);
        d->globalContour.SetContour(newGContour);

#ifdef LAYOUT_DEBUG
#   ifdef SHOW_BEST
        DrawDebug(d->globalContour, workDetail, UINT_MAX, d->paperIndex, d->details.count(), d->details);
#   endif
#endif
    }

    return bestResult.ValideResult(); // Do we have the best result?
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SaveCandidate(VBestSquare &bestResult, const VLayoutDetail &detail, int globalI, int detJ,
                                 BestFrom type)
{
    QVector<QPointF> newGContour = d->globalContour.UniteWithContour(detail, globalI, detJ, type);
    newGContour.append(newGContour.first());
    const QRectF rec = QPolygonF(newGContour).boundingRect();
    bestResult.NewResult(static_cast<qint64>(rec.width()*rec.height()), globalI, detJ,
                         detail.GetMatrix(), detail.IsMirror(), type);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::DrawDebug(const VContour &contour, const VLayoutDetail &detail, int frame, quint32 paperIndex,
                             int detailsCount, const QVector<VLayoutDetail> &details)
{
    QImage frameImage(contour.GetWidth()*2, contour.GetHeight()*2, QImage::Format_RGB32);
    frameImage.fill(Qt::white);
    QPainter paint;
    paint.begin(&frameImage);

    paint.setPen(QPen(Qt::darkRed, 10, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    paint.drawRect(QRectF(contour.GetWidth()/2, contour.GetHeight()/2, contour.GetWidth(), contour.GetHeight()));

    paint.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    QPainterPath p;
    if (contour.GetContour().isEmpty())
    {
        p = DrawContour(contour.CutEdge(QLineF(0, 0, contour.GetWidth(), 0)));
        p.translate(contour.GetWidth()/2, contour.GetHeight()/2);
        paint.drawPath(p);
    }
    else
    {
        p = DrawContour(contour.GetContour());
        p.translate(contour.GetWidth()/2, contour.GetHeight()/2);
        paint.drawPath(p);
    }

#ifdef SHOW_CANDIDATE
    paint.setPen(QPen(Qt::darkGreen, 3, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p = DrawContour(detail.GetLayoutAllowencePoints());
    p.translate(contour.GetWidth()/2, contour.GetHeight()/2);
    paint.drawPath(p);
#endif

#ifdef ARRANGED_DETAILS
    paint.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p = DrawDetails(details);
    p.translate(contour.GetWidth()/2, contour.GetHeight()/2);
    paint.drawPath(p);
#else
    Q_UNUSED(detail)
#endif

    paint.end();
    const QString path = QDir::homePath()+QStringLiteral("/LayoutDebug/")+QString("%1_%2_%3.png").arg(paperIndex)
            .arg(detailsCount).arg(frame);
    frameImage.save (path);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPaper::ShowDirection(const QLineF &edge)
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
QPainterPath VLayoutPaper::DrawContour(const QVector<QPointF> &points)
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
QPainterPath VLayoutPaper::DrawDetails(const QVector<VLayoutDetail> &details)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    if (details.count() > 0)
    {
        for (int i = 0; i < details.size(); ++i)
        {
            path.addPath(details.at(i).ContourPath());
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
    QGraphicsRectItem *paper = new QGraphicsRectItem(QRectF(0, 0, d->globalContour.GetWidth(),
                                                            d->globalContour.GetHeight()));
    paper->setPen(QPen(Qt::black, 1));
    paper->setBrush(QBrush(Qt::white));
    for (int i=0; i < d->details.count(); ++i)
    {
        QGraphicsItem *item = d->details.at(i).GetItem();
        item->setParentItem(paper);
    }
    return paper;
}
