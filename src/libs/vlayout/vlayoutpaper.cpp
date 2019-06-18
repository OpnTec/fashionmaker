/************************************************************************
 **
 **  @file   vlayoutpaper.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2015
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

#include "vlayoutpaper.h"

#include <QBrush>
#include <QCoreApplication>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QList>
#include <QPen>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QThread>
#include <QThreadPool>
#include <QVector>
#include <Qt>
#include <QtAlgorithms>

#include "vbestsquare.h"
#include "vcontour.h"
#include "vlayoutpiece.h"
#include "vlayoutpaper_p.h"
#include "vposition.h"

//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper::VLayoutPaper()
    :d(new VLayoutPaperData)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPaper::VLayoutPaper(int height, int width, qreal layoutWidth)
    :d(new VLayoutPaperData(height, width, layoutWidth))
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
qreal VLayoutPaper::GetShift() const
{
    return d->globalContour.GetShift();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetShift(qreal shift)
{
    d->globalContour.SetShift(shift);
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::GetRotate() const
{
    return d->globalRotate;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetRotate(bool value)
{
    d->globalRotate = value;
    d->localRotate = d->globalRotate;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::GetFollowGrainline() const
{
    return d->followGrainline;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetFollowGrainline(bool value)
{
    d->followGrainline = value;
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::GetRotationNumber() const
{
    return d->globalRotationNumber;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetRotationNumber(int value)
{
    d->globalRotationNumber = value;

    if (d->globalRotationNumber > 360 || d->globalRotationNumber < 1)
    {
        d->globalRotationNumber = 2;
    }

    d->localRotationNumber = d->globalRotationNumber;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::IsSaveLength() const
{
    return d->saveLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetSaveLength(bool value)
{
    d->saveLength = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetPaperIndex(quint32 index)
{
    d->paperIndex = index;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::IsOriginPaperPortrait() const
{
    return d->originPaperOrientation;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetOriginPaperPortrait(bool portrait)
{
    d->originPaperOrientation = portrait;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::ArrangeDetail(const VLayoutPiece &detail, std::atomic_bool &stop)
{
    // First need set size of paper
    if (d->globalContour.GetHeight() <= 0 || d->globalContour.GetWidth() <= 0)
    {
        return false;
    }

    if (detail.LayoutEdgesCount() < 3 || detail.DetailEdgesCount() < 3)
    {
        return false;//Not enough edges
    }

    if ((detail.IsForceFlipping() || detail.IsForbidFlipping()) && not d->globalRotate)
    { // Compensate forbidden flipping by rotating. 180 degree will be enough.
        d->localRotate = true;
        d->localRotationNumber = 2;
    }
    else
    { // Return to global values if was changed
        d->localRotate = d->globalRotate;
        d->localRotationNumber = d->globalRotationNumber;
    }

    return AddToSheet(detail, stop);
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::Count() const
{
    return d->details.count();
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::AddToSheet(const VLayoutPiece &detail, std::atomic_bool &stop)
{
    VBestSquare bestResult(d->globalContour.GetSize(), d->saveLength, d->originPaperOrientation);
    QThreadPool *thread_pool = QThreadPool::globalInstance();
    thread_pool->setExpiryTimeout(1000);
    QVector<VPosition *> threads;

    int detailEdgesCount = detail.LayoutEdgesCount();

    for (int j=1; j <= d->globalContour.GlobalEdgesCount(); ++j)
    {
        QCoreApplication::processEvents();

        for (int i=1; i<= detailEdgesCount; ++i)
        {
            VPositionData data;
            data.gContour = d->globalContour;
            data.detail = detail;
            data.i = i;
            data.j = j;
            data.rotate = d->localRotate;
            data.rotationNumber = d->localRotationNumber;
            data.followGrainline = d->followGrainline;
            data.positionsCache = d->positionsCache;
            data.isOriginPaperOrientationPortrait = d->originPaperOrientation;

            auto *thread = new VPosition(data, &stop, d->saveLength);
            thread->setAutoDelete(false);
            threads.append(thread);
            thread_pool->start(thread);
        }
    }

    // Wait for done
    do
    {
        QCoreApplication::processEvents();
        QThread::msleep(250);
    }
    while(thread_pool->activeThreadCount() > 0 && not stop.load());

    if (stop.load())
    {
        qDeleteAll(threads.begin(), threads.end());
        threads.clear();
        return false;
    }

    for (auto thread : threads)
    {
        bestResult.NewResult(thread->getBestResult());
    }

    qDeleteAll(threads.begin(), threads.end());
    threads.clear();

    return SaveResult(bestResult, detail);
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::SaveResult(const VBestSquare &bestResult, const VLayoutPiece &detail)
{
    if (bestResult.HasValidResult())
    {
        VLayoutPiece workDetail = detail;
        workDetail.SetMatrix(bestResult.Matrix());// Don't forget set matrix
        workDetail.SetMirror(bestResult.Mirror());

        if (d->saveLength)
        {
            d->globalContour.CeateEmptySheetContour();
        }

        const QVector<QPointF> newGContour = d->globalContour.UniteWithContour(workDetail, bestResult.GContourEdge(),
                                                                               bestResult.DetailEdge(),
                                                                               bestResult.Type());
        if (newGContour.isEmpty())
        {
            return false;
        }
        d->details.append(workDetail);
        d->globalContour.SetContour(newGContour);

        VCachedPositions positionChache;
        QVector<QPointF> layoutPoints = workDetail.GetLayoutAllowancePoints();
        positionChache.boundingRect = VLayoutPiece::BoundingRect(layoutPoints);
        positionChache.layoutAllowancePath = VLayoutPiece::PainterPath(layoutPoints);
        d->positionsCache.append(positionChache);
    }

    return bestResult.HasValidResult(); // Do we have the best result?
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsRectItem *VLayoutPaper::GetPaperItem(bool autoCrop, bool textAsPaths) const
{
    QGraphicsRectItem *paper;
    if (autoCrop)
    {
        QScopedPointer<QGraphicsScene> scene(new QGraphicsScene());
        QList<QGraphicsItem *> list = GetItemDetails(textAsPaths);
        for (auto item : list)
        {
            scene->addItem(item);
        }

        const QRect boundingRect = scene->itemsBoundingRect().toRect();
        if (d->globalContour.IsPortrait())
        {
            const int height = boundingRect.height() + boundingRect.y() + 1;
            paper = new QGraphicsRectItem(QRectF(0, 0, d->globalContour.GetWidth(), height));
        }
        else
        {
            const int width = boundingRect.width() + boundingRect.x() + 1;
            paper = new QGraphicsRectItem(QRectF(0, 0, width, d->globalContour.GetHeight()));
        }
    }
    else
    {
        paper = new QGraphicsRectItem(QRectF(0, 0, d->globalContour.GetWidth(), d->globalContour.GetHeight()));
    }
    paper->setPen(QPen(Qt::black, 1));
    paper->setBrush(QBrush(Qt::white));
    return paper;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsPathItem *VLayoutPaper::GetGlobalContour() const
{
    // contour
    const QVector<QPointF> points = d->globalContour.GetContour();

    QPainterPath path;
    if (points.size() > 0)
    {
        path.moveTo(points.at(0));
        for (auto point : points)
        {
            path.lineTo(point);
        }
        path.lineTo(points.at(0));
    }

    const qreal radius = 1;
    for (auto point : points)
    {
        path.addEllipse(point.x()-radius, point.y()-radius, radius*2, radius*2);
    }

    return new QGraphicsPathItem(path);
}

//---------------------------------------------------------------------------------------------------------------------
QList<QGraphicsItem *> VLayoutPaper::GetItemDetails(bool textAsPaths) const
{
    QList<QGraphicsItem *> list;
    for (auto &detail : d->details)
    {
        list.append(detail.GetItem(textAsPaths));
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPiece> VLayoutPaper::GetDetails() const
{
    return d->details;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetDetails(const QList<VLayoutPiece> &details)
{
    d->details = details.toVector();
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VLayoutPaper::DetailsBoundingRect() const
{
    QRectF rec;
    for (auto &detail : d->details)
    {
        rec = rec.united(detail.DetailBoundingRect());
    }

    return rec;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutPaper::Efficiency() const
{
    qreal efficiency = 0;
    for(auto &detail : d->details)
    {
        efficiency += static_cast<qreal>(detail.Square());
    }

    const QRectF boundingRect = DetailsBoundingRect();

    return efficiency / (boundingRect.width() * boundingRect.height()) * 100.0;
}
