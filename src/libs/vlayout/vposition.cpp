/************************************************************************
 **
 **  @file   vposition.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 1, 2015
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

#include "vposition.h"
#include "../vmisc/def.h"

#include <QPointF>
#include <QRectF>
#include <QLineF>
#include <QPolygonF>
#include <QPainterPath>
#include <QImage>
#include <QPainter>
#include <QCoreApplication>
#include <QDir>
#include <QtWidgets>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

//---------------------------------------------------------------------------------------------------------------------
VPosition::VPosition(const VContour &gContour, int j, const VLayoutDetail &detail, int i, volatile bool *stop,
                     bool rotate, int rotationIncrease, bool saveLength)
    :QRunnable(), bestResult(VBestSquare(gContour.GetSize(), saveLength)), gContour(gContour), detail(detail), i(i),
      j(j), paperIndex(0), frame(0), detailsCount(0), details(QVector<VLayoutDetail>()), stop(stop), rotate(rotate),
      rotationIncrease(rotationIncrease), angle_between(0)
{
    if ((rotationIncrease >= 1 && rotationIncrease <= 180 && 360 % rotationIncrease == 0) == false)
    {
        this->rotationIncrease = 180;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::run()
{
    if (*stop)
    {
        return;
    }

    // We should use copy of the detail.
    VLayoutDetail workDetail = detail;

    int dEdge = i;// For mirror detail edge will be different
    if (CheckCombineEdges(workDetail, j, dEdge))
    {
        #ifdef LAYOUT_DEBUG
        #   ifdef SHOW_CANDIDATE_BEST
                DrawDebug(gContour, workDetail, frame+2, paperIndex, detailsCount, details);
        #   endif
        #endif

        SaveCandidate(bestResult, workDetail, j, dEdge, BestFrom::Combine);
    }
    frame = frame + 3;

    if (rotate)
    {
        Rotate(rotationIncrease);
    }
    else
    {
        if (gContour.GetContour().isEmpty())
        {
            Rotate(rotationIncrease);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VPosition::getPaperIndex() const
{
    return paperIndex;
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setPaperIndex(const quint32 &value)
{
    paperIndex = value;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VPosition::getFrame() const
{
    return frame;
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setFrame(const quint32 &value)
{
    frame = value;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VPosition::getDetailsCount() const
{
    return detailsCount;
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setDetailsCount(const quint32 &value)
{
    detailsCount = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setDetails(const QVector<VLayoutDetail> &details)
{
    this->details = details;
}

//---------------------------------------------------------------------------------------------------------------------
VBestSquare VPosition::getBestResult() const
{
    return bestResult;
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::DrawDebug(const VContour &contour, const VLayoutDetail &detail, int frame, quint32 paperIndex,
                          int detailsCount, const QVector<VLayoutDetail> &details)
{
    const int biasWidth = Bias(contour.GetWidth(), QIMAGE_MAX);
    const int biasHeight = Bias(contour.GetHeight(), QIMAGE_MAX);

    QPicture picture;
    QPainter paint;
    paint.begin(&picture);

    paint.setPen(QPen(Qt::black, 6, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    QPainterPath p;
    if (contour.GetContour().isEmpty())
    {
        p = DrawContour(contour.CutEmptySheetEdge());
        p.translate(biasWidth/2, biasHeight/2);
        paint.drawPath(p);
    }
    else
    {
        p = DrawContour(contour.GetContour());
        p.translate(biasWidth/2, biasHeight/2);
        paint.drawPath(p);
    }

#ifdef SHOW_CANDIDATE
    paint.setPen(QPen(Qt::darkGreen, 6, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p = DrawContour(detail.GetLayoutAllowencePoints());
    p.translate(biasWidth/2, biasHeight/2);
    paint.drawPath(p);
#else
    Q_UNUSED(detail)
    Q_UNUSED(details)
#endif

#ifdef ARRANGED_DETAILS
    paint.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p = DrawDetails(details);
    p.translate(biasWidth/2, biasHeight/2);
    paint.drawPath(p);
#else
    Q_UNUSED(details)
#endif

    // Calculate bounding rect before draw sheet rect
    const QRect pictureRect = picture.boundingRect();

    // Sheet
#ifdef SHOW_SHEET
    paint.setPen(QPen(Qt::darkRed, 15, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    paint.drawRect(QRectF(biasWidth/2, biasHeight/2, contour.GetWidth(), contour.GetHeight()));
#endif

    paint.end();

    // Dump frame to image
    // Note. If program was build with Address Sanitizer possible crashes. Address Sanitizer doesn't support big
    // allocations. See page https://bitbucket.org/dismine/valentina/wiki/developers/Address_Sanitizer
    QImage frameImage(pictureRect.width()+biasWidth, pictureRect.height()+biasHeight, QImage::Format_RGB32);

    if (frameImage.isNull())
    {
        return;
    }

    frameImage.fill(Qt::white);

    QPainter paintFrameImage;
    paintFrameImage.begin(&frameImage);
    paintFrameImage.drawPicture(0, 0, picture);
    paintFrameImage.end();

    const QString path = QDir::homePath()+QStringLiteral("/LayoutDebug/")+QString("%1_%2_%3.png").arg(paperIndex)
            .arg(detailsCount).arg(frame);
    frameImage.save (path);
}

//---------------------------------------------------------------------------------------------------------------------
int VPosition::Bias(int length, int maxLength)
{
    if (length < maxLength && length*2 < maxLength)
    {
        return length;
    }
    else
    {
        return maxLength-length;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::SaveCandidate(VBestSquare &bestResult, const VLayoutDetail &detail, int globalI, int detJ,
                              BestFrom type)
{
    QVector<QPointF> newGContour = gContour.UniteWithContour(detail, globalI, detJ, type);
    newGContour.append(newGContour.first());
    const QSizeF size = QPolygonF(newGContour).boundingRect().size();
    bestResult.NewResult(size, globalI, detJ, detail.GetMatrix(), detail.IsMirror(), type);
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::CheckCombineEdges(VLayoutDetail &detail, int j, int &dEdge)
{
    const QLineF globalEdge = gContour.GlobalEdge(j);
    bool flagMirror = false;
    bool flagSquare = false;

    CombineEdges(detail, globalEdge, dEdge);

#ifdef LAYOUT_DEBUG
#   ifdef SHOW_COMBINE
        DrawDebug(gContour, detail, frame, paperIndex, detailsCount, details);
#   endif
#endif

    CrossingType type = CrossingType::Intersection;
    if (SheetContains(detail.BoundingRect()))
    {
        if (not gContour.GetContour().isEmpty())
        {
            type = Crossing(detail, j, dEdge);
        }
        else
        {
            type = CrossingType::NoIntersection;
        }
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
            break;
        }
        default:
            break;
    }

    if (flagMirror)
    {
        #ifdef LAYOUT_DEBUG
            #ifdef SHOW_MIRROR
                DrawDebug(gContour, detail, frame+1, paperIndex, detailsCount, details);
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
                break;
            }
            default:
                break;
        }
    }
    return flagSquare;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::CheckRotationEdges(VLayoutDetail &detail, int j, int dEdge, int angle) const
{
    const QLineF globalEdge = gContour.GlobalEdge(j);
    bool flagSquare = false;

    RotateEdges(detail, globalEdge, dEdge, angle);

#ifdef LAYOUT_DEBUG
    #ifdef SHOW_ROTATION
        DrawDebug(gContour, detail, frame, paperIndex, detailsCount, details);
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
            break;
        }
        default:
            break;
    }
    return flagSquare;
}

//---------------------------------------------------------------------------------------------------------------------
VPosition::CrossingType VPosition::Crossing(const VLayoutDetail &detail, const int &globalI, const int &detailI) const
{
    int globalEdgesCount = gContour.GlobalEdgesCount();
    if (globalEdgesCount == 0)
    {
        globalEdgesCount = 1;// For blank sheet
    }

    const int detailEdgesCount = detail.EdgesCount();
    if (detailEdgesCount < 3)
    {
        return CrossingType::EdgeError;
    }

    const QLineF gEdge = gContour.GlobalEdge(globalI);
    const QLineF dEdge = detail.Edge(detailI);

    for (int i = 1; i <= globalEdgesCount; i++)
    {
        const QLineF globalEdge = gContour.GlobalEdge(i);
        if (globalEdge.isNull()) // Got null edge
        {
            return CrossingType::EdgeError;
        }

        for (int j = 1; j <= detailEdgesCount; j++)
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
            if (globalEdge.intersect(detailEdge, &xPoint) == QLineF::BoundedIntersection)
            {
                if (TrueIntersection(gEdge, dEdge, xPoint))
                {
                    return CrossingType::Intersection;
                }
            }
        }
    }

    return CrossingType::NoIntersection;
}

//---------------------------------------------------------------------------------------------------------------------
VPosition::InsideType VPosition::InsideContour(const VLayoutDetail &detail, const int &detailI) const
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

    if (gContour.GetContour().isEmpty())
    {
        const QLineF globalEdge = gContour.GlobalEdge(1);
        for (int i = 0; i < lPoints.count(); i++)
        {
            if (CheckSide(globalEdge, lPoints.at(i)) < 0)
            {
                return InsideType::Inside;
            }
        }
    }
    else
    {
        const int polyCorners = gContour.GlobalEdgesCount();
        int j = polyCorners-1;

        QVector<qreal> constant;
        QVector<qreal> multiple;

        for (int i=0; i<polyCorners; i++)
        {
            const qreal xi = gContour.at(i).x(); //-V807
            const qreal xj = gContour.at(j).x(); //-V807
            const qreal yi = gContour.at(i).y();
            const qreal yj = gContour.at(j).y();
            if (VFuzzyComparePossibleNulls(yj, yi))
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

        for (int m = 1; m <= detail.EdgesCount(); ++m)
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
                    const qreal yi = gContour.at(i).y();
                    const qreal yj = gContour.at(j).y();

                    const QPointF &pn = p.at(n);
                    if (((yi < pn.y() && yj >= pn.y()) || (yj < pn.y() && yi >= pn.y())))
                    {
                        oddNodes ^= (pn.y() * multiple.at(i) + constant.at(i) < pn.x());
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
qreal VPosition::CheckSide(const QLineF &edge, const QPointF &p) const
{
    return (edge.x2() - edge.x1()) * (p.y() - edge.y1()) - (edge.y2() - edge.y1()) * (p.x() - edge.x1());
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::SheetContains(const QRectF &rect) const
{
    const QRectF bRect(0, 0, gContour.GetWidth(), gContour.GetHeight());
    return bRect.contains(rect);
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::CombineEdges(VLayoutDetail &detail, const QLineF &globalEdge, const int &dEdge)
{
    QLineF detailEdge = detail.Edge(dEdge);

    // Find distance between two edges for two begin vertex.
    const qreal dx = globalEdge.x2() - detailEdge.x2();
    const qreal dy = globalEdge.y2() - detailEdge.y2();

    detailEdge.translate(dx, dy); // Use values for translate detail edge.

    angle_between = globalEdge.angleTo(detailEdge); // Seek angle between two edges.

    // Now we move detail to position near to global contour edge.
    detail.Translate(dx, dy);
    if (not qFuzzyIsNull(angle_between) || not qFuzzyCompare(angle_between, 360))
    {
        detail.Rotate(detailEdge.p2(), -angle_between);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::RotateEdges(VLayoutDetail &detail, const QLineF &globalEdge, int dEdge, int angle) const
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
void VPosition::AppendWhole(QVector<QPointF> &contour, const VLayoutDetail &detail, int detJ, quint32 shift)
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
        const QVector<QPointF> points = CutEdge(detail.Edge(j), shift);
        for (int i = 0; i < points.size()-1; ++i)
        {
            contour.append(points.at(i));
        }
        ++processedEdges;
        ++j;
    }while (processedEdges < nD);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QPolygonF VPosition::GlobalPolygon() const
{
    QVector<QPointF> points = gContour.GetContour();
    points.append(points.first());
    return QPolygonF(points);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VPosition::CutEdge(const QLineF &edge, quint32 shift)
{
    QVector<QPointF> points;
    if (shift == 0)
    {
        points.append(edge.p1());
        points.append(edge.p2());
    }
    else
    {
        const int n = qFloor(edge.length()/shift);

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
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::Rotate(int increase)
{
    int startAngle = 0;
    if (VFuzzyComparePossibleNulls(angle_between, 360))
    {
        startAngle = increase;
    }
    for (int angle = startAngle; angle < 360; angle = angle+increase)
    {
        if (*stop)
        {
            return;
        }

        // We should use copy of the detail.
        VLayoutDetail workDetail = detail;

        if (CheckRotationEdges(workDetail, j, i, angle))
        {
            #ifdef LAYOUT_DEBUG
            #   ifdef SHOW_CANDIDATE_BEST
                    ++frame;
                    DrawDebug(gContour, workDetail, frame, paperIndex, detailsCount, details);
            #   endif
            #endif

            SaveCandidate(bestResult, workDetail, j, i, BestFrom::Rotation);
        }
        ++frame;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::TrueIntersection(const QLineF &gEdge, const QLineF &dEdge, const QPointF &p) const
{
    const QPointF pX = RoundedPoint(p);

    QPointF xPoint;
    if (gEdge.intersect(dEdge, &xPoint) == QLineF::NoIntersection)
    {
        const QPointF gP1 = RoundedPoint(gEdge.p1());
        const QPointF gP2 = RoundedPoint(gEdge.p2());
        const QPointF dP1 = RoundedPoint(dEdge.p1());
        const QPointF dP2 = RoundedPoint(dEdge.p2());

        // If two edges are same line ignorring all intersection point that are equal to the start or the end point
        return !(pX == gP1 || pX == gP2 || pX == dP1 || pX == dP2);
    }
    else
    {
        // Rotation case. Ignore intersection only in the point of edges intersection.
        return pX != RoundedPoint(xPoint);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VPosition::RoundedPoint(const QPointF &p) const
{
    return QPointF(qRound(p.x()), qRound(p.y()));
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VPosition::Triplet(const QLineF &edge) const
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
QPainterPath VPosition::ShowDirection(const QLineF &edge)
{
    const int arrowLength = 14;
    QPainterPath path;
    if (edge.length()/arrowLength < 5)
    {
        return  path;
    }

    QLineF arrow = edge;
    arrow.setLength(edge.length()/2.0);

    //Reverse line because we want start arrow from this point
    arrow = QLineF(arrow.p2(), arrow.p1());
    const qreal angle = arrow.angle();//we each time change line angle, better save original angle value
    arrow.setLength(arrowLength);//arrow length in pixels

    arrow.setAngle(angle-35);
    path.moveTo(arrow.p1());
    path.lineTo(arrow.p2());

    arrow.setAngle(angle+35);
    path.moveTo(arrow.p1());
    path.lineTo(arrow.p2());
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPosition::DrawContour(const QVector<QPointF> &points)
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
QPainterPath VPosition::DrawDetails(const QVector<VLayoutDetail> &details)
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
