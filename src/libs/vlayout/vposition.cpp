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
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#include <QtConcurrent>
#include <QFutureWatcher>
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
#include <QThreadPool>
#include <Qt>
#include <functional>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"
#include "../ifc/exception/vexception.h"
#include "../vpatterndb/floatItemData/floatitemdef.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
#include "../vmisc/backport/qscopeguard.h"
#else
#include <QScopeGuard>
#endif

//---------------------------------------------------------------------------------------------------------------------
VPosition::VPosition()
{}

//---------------------------------------------------------------------------------------------------------------------
VPosition::VPosition(const VPositionData &data, std::atomic_bool *stop, bool saveLength)
    : m_isValid(true),
      m_bestResult(VBestSquare(data.gContour.GetSize(), saveLength, data.isOriginPaperOrientationPortrait)),
      m_data(data),
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
    if (not m_isValid)
    {
        return;
    }

    if (stop->load())
    {
        return;
    }

    try
    {
        for (int i=1; i<= m_data.detail.LayoutEdgesCount(); ++i)
        {
            if (stop->load())
            {
                return;
            }

            m_data.i = i;
            FindBestPosition();

            if (stop->load())
            {
                return;
            }
        }
    }
    catch (const VException &e)
    {
        m_bestResult.TerminatedByException(QStringLiteral("%1\n\n%2").arg(e.ErrorMessage(), e.DetailedInformation()));
        return;
    }
    catch (std::exception& e)
    {
        m_bestResult.TerminatedByException(QString::fromLatin1(e.what()));
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
VBestSquare VPosition::getBestResult() const
{
    return m_bestResult;
}

//---------------------------------------------------------------------------------------------------------------------
VBestSquare VPosition::ArrangeDetail(const VPositionData &data, std::atomic_bool *stop, bool saveLength)
{
    VBestSquare bestResult(data.gContour.GetSize(), saveLength, data.isOriginPaperOrientationPortrait);

    if (stop->load())
    {
        return bestResult;
    }

    // First need set size of paper
    if (data.gContour.GetHeight() <= 0 || data.gContour.GetWidth() <= 0)
    {
        return bestResult;
    }

    const VLayoutPiece detail = data.detail;
    const int detailEdgesCount = detail.LayoutEdgesCount();
    if (detailEdgesCount < 3 || detail.DetailEdgesCount() < 3)
    {
        return bestResult;//Not enough edges
    }

    QFutureWatcher<VBestSquare> watcher;
    QVector<VPosition> jobs;
    jobs.reserve(data.gContour.GlobalEdgesCount());

    for (int j=1; j <= data.gContour.GlobalEdgesCount(); ++j)
    {
        VPositionData linkedData = data;
        linkedData.j = j;

        jobs.append(VPosition(linkedData, stop, saveLength));
    }

    Q_ASSERT(not jobs.isEmpty());

    std::function<VBestSquare (VPosition position)> Nest = [](VPosition position)
    {
        position.run();
        return position.getBestResult();
    };

    watcher.setFuture(QtConcurrent::mapped(jobs, Nest));

    while(not watcher.isStarted())
    {
        QCoreApplication::processEvents();
        QThread::msleep(250);
    }

    // Wait for done
    do
    {
        QCoreApplication::processEvents();
        QThread::msleep(250);
    }
    while(watcher.isRunning() && not stop->load());

    if (stop->load())
    {
        do
        {
            QCoreApplication::processEvents();
            QThread::msleep(250);
        }
        while(watcher.isRunning());

        return bestResult;
    }

    QList<VBestSquare> results = watcher.future().results();
    for (auto &result : results)
    {
        bestResult.NewResult(result);
    }

    return bestResult;
}

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
    const QRectF boundingRect = detail.DetailBoundingRect();
    const qreal depthPosition = m_data.isOriginPaperOrientationPortrait ? boundingRect.y() : boundingRect.x();
    const qreal sidePosition = m_data.isOriginPaperOrientationPortrait ? boundingRect.x() : boundingRect.y();

    VBestSquareResData data;
    data.bestSize = size;
    data.globalI = globalI; // Edge of global contour
    data.detJ = detJ; // Edge of detail
    data.resMatrix = detail.GetMatrix(); // Matrix for rotation and translation detail
    data.resMirror = detail.IsMirror();
    data.type = type;
    data.depthPosition = depthPosition;
    data.sidePosition = sidePosition;

    bestResult.NewResult(data);
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::CheckCombineEdges(VLayoutPiece &detail, int j, int &dEdge)
{
    const QLineF globalEdge = m_data.gContour.GlobalEdge(j);
    bool flagMirror = false;
    bool flagSquare = false;

    CombineEdges(detail, globalEdge, dEdge);

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
        dEdge = detail.LayoutEdgeByPoint(globalEdge.p2());

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
        SaveCandidate(m_bestResult, workDetail, m_data.j, m_data.i, BestFrom::Rotation);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPosition::CrossingType VPosition::Crossing(const VLayoutPiece &detail) const
{
    if (m_data.positionsCache.isEmpty())
    {
        return CrossingType::NoIntersection;
    }

    const QVector<QPointF> layoutPoints = detail.GetLayoutAllowancePoints();
    const QRectF layoutBoundingRect = VLayoutPiece::BoundingRect(layoutPoints);
    const QPainterPath layoutAllowancePath = VLayoutPiece::PainterPath(layoutPoints);

    const QVector<QPointF> contourPoints = detail.IsSeamAllowance() && not detail.IsSeamAllowanceBuiltIn() ?
                detail.GetMappedSeamAllowancePoints() : detail.GetMappedContourPoints();
    const QRectF detailBoundingRect = VLayoutPiece::BoundingRect(contourPoints);
    const QPainterPath contourPath = VLayoutPiece::PainterPath(contourPoints);

    for(auto &position : m_data.positionsCache)
    {
        if (position.boundingRect.intersects(layoutBoundingRect) || position.boundingRect.contains(detailBoundingRect))
        {
            if (position.layoutAllowancePath.contains(contourPath) ||
                    position.layoutAllowancePath.intersects(layoutAllowancePath))
            {
                return CrossingType::Intersection;
            }
        }
    }

    return CrossingType::NoIntersection;
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
    QLineF detailEdge = detail.LayoutEdge(dEdge);

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
void VPosition::RotateEdges(VLayoutPiece &detail, const QLineF &globalEdge, int dEdge, qreal angle)
{
    QLineF detailEdge = detail.LayoutEdge(dEdge);
    // Now we move detail to position near to global contour edge.
    detail.Translate(globalEdge.x2() - detailEdge.x2(), globalEdge.y2() - detailEdge.y2());
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

    if (stop->load())
    {
        return;
    }

    const qreal angle = detailGrainline.angleTo(FabricGrainline());

    if (m_data.detail.GrainlineArrowType() == GrainlineArrowDirection::atBoth ||
            m_data.detail.GrainlineArrowType() == GrainlineArrowDirection::atFront)
    {
        RotateOnAngle(angle);
    }

    if (stop->load())
    {
        return;
    }

    if (m_data.detail.GrainlineArrowType() == GrainlineArrowDirection::atBoth ||
            m_data.detail.GrainlineArrowType() == GrainlineArrowDirection::atRear)
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
            SaveCandidate(m_bestResult, workDetail, m_data.j, dEdge, BestFrom::Combine);
        }

        if (stop->load())
        {
            return;
        }

        if (m_data.rotate)
        {
            Rotate(m_data.rotationNumber);
        }
    }
    else
    {
        FollowGrainline();
    }
}
