/************************************************************************
 **
 **  @file   vposition.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 1, 2015
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

#include "vposition.h"

#include <QDir>
#include <QImage>
#include <QLineF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPicture>
#include <QPointF>
#include <QPolygonF>
#include <QRect>
#include <QRectF>
#include <QSizeF>
#include <QStaticStringData>
#include <QString>
#include <QStringData>
#include <QStringDataPtr>
#include <Qt>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"

namespace
{
//---------------------------------------------------------------------------------------------------------------------
#ifdef SHOW_DIRECTION
QPainterPath ShowDirection(const QLineF &edge)
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
#endif

#ifdef LAYOUT_DEBUG
//---------------------------------------------------------------------------------------------------------------------
QPainterPath DrawContour(const QVector<QPointF> &points)
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
#endif

//---------------------------------------------------------------------------------------------------------------------
#ifdef ARRANGED_DETAILS
QPainterPath DrawDetails(const QVector<VLayoutPiece> &details)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    if (details.count() > 0)
    {
        for (auto &detail : details)
        {
            path.addPath(detail.ContourPath());
        }
    }
    return path;
}
#endif

} //anonymous namespace

//---------------------------------------------------------------------------------------------------------------------
VPosition::VPosition(const VPositionData &data, std::atomic_bool *stop, bool saveLength)
    : QRunnable(),
      m_bestResult(VBestSquare(data.gContour.GetSize(), saveLength)),
      m_data(data),
#ifdef LAYOUT_DEBUG
      paperIndex(0),
      frame(0),
      detailsCount(0),
      details(),
#endif
      stop(stop),
      angle_between(0)
{
    if (m_data.rotationNumber > 360 || m_data.rotationNumber < 1)
    {
        m_data.rotationNumber = 2;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::run()
{
    if (stop->load())
    {
        return;
    }

    FindBestPosition();
}

#ifdef LAYOUT_DEBUG
//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VPosition::getPaperIndex() const
{
    return paperIndex;
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setPaperIndex(quint32 value)
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
void VPosition::setFrame(quint32 value)
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
void VPosition::setDetailsCount(quint32 value)
{
    detailsCount = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setDetails(const QVector<VLayoutPiece> &details)
{
    this->details = details;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
VBestSquare VPosition::getBestResult() const
{
    return m_bestResult;
}

#ifdef LAYOUT_DEBUG
//---------------------------------------------------------------------------------------------------------------------
void VPosition::DrawDebug(const VContour &contour, const VLayoutPiece &detail, int frame, quint32 paperIndex,
                          int detailsCount, const QVector<VLayoutPiece> &details)
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
    p = DrawContour(detail.GetLayoutAllowancePoints());
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

    const QString path = QDir::homePath()+QStringLiteral("/LayoutDebug/%1_%2_%3.png").arg(paperIndex)
            .arg(detailsCount).arg(frame);
    frameImage.save (path);
}

//---------------------------------------------------------------------------------------------------------------------
int VPosition::Bias(int length, int maxLength)
{
    return length < maxLength && length*2 < maxLength ? length : maxLength-length;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
void VPosition::SaveCandidate(VBestSquare &bestResult, const VLayoutPiece &detail, int globalI, int detJ,
                              BestFrom type)
{
    if (bestResult.IsSaveLength())
    {
        m_data.gContour.CeateEmptySheetContour();
    }

    QVector<QPointF> newGContour = m_data.gContour.UniteWithContour(detail, globalI, detJ, type);
    newGContour.append(newGContour.first());
    const QSizeF size = QPolygonF(newGContour).boundingRect().size();
    const qreal depthPosition = m_data.gContour.IsPortrait() ? detail.DetailBoundingRect().y() :
                                                               detail.DetailBoundingRect().x();

    VBestSquareResData data;
    data.bestSize = size;
    data.globalI = globalI; // Edge of global contour
    data.detJ = detJ; // Edge of detail
    data.resMatrix = detail.GetMatrix(); // Matrix for rotation and translation detail
    data.resMirror = detail.IsMirror();
    data.type = type;
    data.depthPosition = depthPosition;

    bestResult.NewResult(data);
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::CheckCombineEdges(VLayoutPiece &detail, int j, int &dEdge)
{
    const QLineF globalEdge = m_data.gContour.GlobalEdge(j);
    bool flagMirror = false;
    bool flagSquare = false;

    CombineEdges(detail, globalEdge, dEdge);

#ifdef LAYOUT_DEBUG
#   ifdef SHOW_COMBINE
        DrawDebug(gContour, detail, frame, paperIndex, detailsCount, details);
#   endif
#endif

    CrossingType type = CrossingType::Intersection;
    if (not detail.IsForceFlipping() && SheetContains(detail.DetailBoundingRect()))
    {
        if (not m_data.gContour.GetContour().isEmpty())
        {
            type = Crossing(detail);
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
            flagSquare = true;
            break;
        default:
            break;
    }

    if (flagMirror && not detail.IsForbidFlipping())
    {
        #ifdef LAYOUT_DEBUG
            #ifdef SHOW_MIRROR
                DrawDebug(gContour, detail, frame+1, paperIndex, detailsCount, details);
            #endif
        #endif

        if (m_data.gContour.GetContour().isEmpty())
        {
            dEdge = detail.DetailEdgeByPoint(globalEdge.p2());
        }
        else
        {
            dEdge = detail.LayoutEdgeByPoint(globalEdge.p2());
        }

        if (dEdge <= 0)
        {
            return false;
        }

        CrossingType type = CrossingType::Intersection;
        if (SheetContains(detail.DetailBoundingRect()))
        {
            type = Crossing(detail);
        }

        switch (type)
        {
            case CrossingType::EdgeError:
                return false;
            case CrossingType::Intersection:
                flagSquare = false;
                break;
            case CrossingType::NoIntersection:
                flagSquare = true;
                break;
            default:
                break;
        }
    }
    return flagSquare;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::CheckRotationEdges(VLayoutPiece &detail, int j, int dEdge, qreal angle) const
{
    const QLineF globalEdge = m_data.gContour.GlobalEdge(j);
    bool flagSquare = false;

    if (detail.IsForceFlipping())
    {
        detail.Mirror(not m_data.followGrainline ? globalEdge : QLineF(10, 10, 10, 100));
    }

    RotateEdges(detail, globalEdge, dEdge, angle);

#ifdef LAYOUT_DEBUG
    #ifdef SHOW_ROTATION
        DrawDebug(gContour, detail, frame, paperIndex, detailsCount, details);
    #endif
#endif

    CrossingType type = CrossingType::Intersection;
    if (SheetContains(detail.DetailBoundingRect()))
    {
        type = Crossing(detail);
    }

    switch (type)
    {
        case CrossingType::EdgeError:
            return false;
        case CrossingType::Intersection:
            flagSquare = false;
            break;
        case CrossingType::NoIntersection:
            flagSquare = true;
            break;
        default:
            break;
    }
    return flagSquare;
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::RotateOnAngle(qreal angle)
{
    // We should use copy of the detail.
    VLayoutPiece workDetail = m_data.detail;

    if (CheckRotationEdges(workDetail, m_data.j, m_data.i, angle))
    {
        #ifdef LAYOUT_DEBUG
        #   ifdef SHOW_CANDIDATE_BEST
                ++frame;
                DrawDebug(gContour, workDetail, frame, paperIndex, detailsCount, details);
        #   endif
        #endif

        SaveCandidate(m_bestResult, workDetail, m_data.j, m_data.i, BestFrom::Rotation);
    }
#ifdef LAYOUT_DEBUG
    ++frame;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
VPosition::CrossingType VPosition::Crossing(const VLayoutPiece &detail) const
{
    const QRectF gRect = m_data.gContour.BoundingRect();
    if (not gRect.intersects(detail.LayoutBoundingRect()) && not gRect.contains(detail.DetailBoundingRect()))
    {
        // This we can determine efficiently.
        return CrossingType::NoIntersection;
    }

    const QPainterPath gPath = m_data.gContour.ContourPath();
    CrossingType crossing = CrossingType::EdgeError;
    if (not gPath.intersects(detail.LayoutAllowancePath()) && not gPath.contains(detail.ContourPath()))
    {
        crossing = CrossingType::NoIntersection;
    }
    else
    {
        crossing = CrossingType::Intersection;
    }
    return crossing;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::SheetContains(const QRectF &rect) const
{
    const QRectF bRect(-accuracyPointOnLine, -accuracyPointOnLine, m_data.gContour.GetWidth()+accuracyPointOnLine,
                       m_data.gContour.GetHeight()+accuracyPointOnLine);
    return bRect.contains(rect);
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::CombineEdges(VLayoutPiece &detail, const QLineF &globalEdge, int dEdge)
{
    QLineF detailEdge;
    if (m_data.gContour.GetContour().isEmpty())
    {
        detailEdge = detail.DetailEdge(dEdge);
    }
    else
    {
        detailEdge = detail.LayoutEdge(dEdge);
    }

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
void VPosition::RotateEdges(VLayoutPiece &detail, const QLineF &globalEdge, int dEdge, qreal angle) const
{
    QLineF detailEdge;
    if (m_data.gContour.GetContour().isEmpty())
    {
        detailEdge = detail.DetailEdge(dEdge);
    }
    else
    {
        detailEdge = detail.LayoutEdge(dEdge);
    }

    // Find distance between two edges for two begin vertex.
    const qreal dx = globalEdge.x2() - detailEdge.x2();
    const qreal dy = globalEdge.y2() - detailEdge.y2();

    detailEdge.translate(dx, dy); // Use values for translate detail edge.

    // Now we move detail to position near to global contour edge.
    detail.Translate(dx, dy);
    detail.Rotate(globalEdge.p2(), angle);
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::Rotate(int number)
{
    const qreal step = 360/number;
    qreal startAngle = 0;
    if (VFuzzyComparePossibleNulls(angle_between, 360))
    {
        startAngle = step;
    }
    for (qreal angle = startAngle; angle < 360; angle = angle+step)
    {
        if (stop->load())
        {
            return;
        }

        RotateOnAngle(angle);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::FollowGrainline()
{
    if (stop->load())
    {
        return;
    }

    QLineF detailGrainline(10, 10, 100, 10);
    detailGrainline.setAngle(m_data.detail.GrainlineAngle());

    if (m_data.detail.IsForceFlipping())
    {
        VLayoutPiece workDetail = m_data.detail; // We need copy for temp change
        workDetail.Mirror(not m_data.followGrainline ? m_data.gContour.GlobalEdge(m_data.j) : QLineF(10, 10, 10, 100));
        detailGrainline = workDetail.GetMatrix().map(detailGrainline);
    }

    const qreal angle = detailGrainline.angleTo(FabricGrainline());

    if (m_data.detail.GrainlineArrowType() == ArrowType::atBoth ||
            m_data.detail.GrainlineArrowType() == ArrowType::atFront)
    {
        RotateOnAngle(angle);
    }

    if (m_data.detail.GrainlineArrowType() == ArrowType::atBoth ||
            m_data.detail.GrainlineArrowType() == ArrowType::atRear)
    {
        RotateOnAngle(angle+180);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::FindBestPosition()
{
    if (not m_data.followGrainline || not m_data.detail.IsGrainlineEnabled())
    {
        // We should use copy of the detail.
        VLayoutPiece workDetail = m_data.detail;

        int dEdge = m_data.i;// For mirror detail edge will be different
        if (CheckCombineEdges(workDetail, m_data.j, dEdge))
        {
            #ifdef LAYOUT_DEBUG
            #   ifdef SHOW_CANDIDATE_BEST
                    DrawDebug(gContour, workDetail, frame+2, paperIndex, detailsCount, details);
            #   endif
            #endif

            SaveCandidate(m_bestResult, workDetail, m_data.j, dEdge, BestFrom::Combine);
        }
#ifdef LAYOUT_DEBUG
        frame = frame + 3;
#endif

        if (m_data.rotate)
        {
            Rotate(m_data.rotationNumber);
        }
        else
        {
            if (m_data.gContour.GetContour().isEmpty())
            {
                Rotate(m_data.rotationNumber);
            }
        }
    }
    else
    {
        FollowGrainline();
    }
}
