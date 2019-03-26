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
quint32 VLayoutPaper::GetShift() const
{
    return d->globalContour.GetShift();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetShift(quint32 shift)
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
int VLayoutPaper::GetRotationIncrease() const
{
    return d->globalRotationIncrease;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetRotationIncrease(int value)
{
    d->globalRotationIncrease = value;

    if (not (d->globalRotationIncrease >= 1
         && d->globalRotationIncrease <= 180
         && 360 % d->globalRotationIncrease == 0))
    {
        d->globalRotationIncrease = 180;
    }

    d->localRotationIncrease = d->globalRotationIncrease;
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
        d->localRotationIncrease = 180;
    }
    else
    { // Return to global values if was changed
        d->localRotate = d->globalRotate;
        d->localRotationIncrease = d->globalRotationIncrease;
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
bool VLayoutPaper::AddToSheet(const VLayoutPiece &detail, std::atomic_bool &stop)
{
    VBestSquare bestResult(d->globalContour.GetSize(), d->saveLength);
    QThreadPool *thread_pool = QThreadPool::globalInstance();
    thread_pool->setExpiryTimeout(1000);
    QVector<VPosition *> threads;

    int detailEdgesCount = d->globalContour.GetContour().isEmpty() ? detail.DetailEdgesCount() :
                                                                     detail.LayoutEdgesCount();

    for (int j=1; j <= d->globalContour.GlobalEdgesCount(); ++j)
    {
        for (int i=1; i<= detailEdgesCount; ++i)
        {
            VPositionData data;
            data.gContour = d->globalContour;
            data.detail = detail;
            data.i = i;
            data.j = j;
            data.rotate = d->localRotate;
            data.rotationIncrease = d->localRotationIncrease;
            data.followGrainline = d->followGrainline;

            auto *thread = new VPosition(data, &stop, d->saveLength);
            //Info for debug
            #ifdef LAYOUT_DEBUG
                thread->setPaperIndex(d->paperIndex);
                thread->setFrame(d->frame);
                thread->setDetailsCount(d->details.count());
                thread->setDetails(d->details);
            #endif

            thread->setAutoDelete(false);
            threads.append(thread);
            thread_pool->start(thread);

#ifdef LAYOUT_DEBUG
            d->frame = d->frame + 3 + static_cast<quint32>(360/d->localRotationIncrease*2);
#endif
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
    if (bestResult.IsValidResult())
    {
        VLayoutPiece workDetail = detail;
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
        VPosition::DrawDebug(d->globalContour, workDetail, UINT_MAX, d->paperIndex, d->details.count(), d->details);
#   endif
#endif
    }

    return bestResult.IsValidResult(); // Do we have the best result?
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

        if (d->globalContour.IsPortrait())
        {
            const int height = scene->itemsBoundingRect().toRect().height();
            if (d->globalContour.GetHeight() > height) //-V807
            {
                paper = new QGraphicsRectItem(QRectF(0, 0, d->globalContour.GetWidth(), height));
            }
            else
            {
                paper = new QGraphicsRectItem(QRectF(0, 0, d->globalContour.GetWidth(), d->globalContour.GetHeight()));
            }
        }
        else
        {
            const int width = scene->itemsBoundingRect().toRect().width();
            if (d->globalContour.GetWidth() > width) //-V807
            {
                paper = new QGraphicsRectItem(QRectF(0, 0, width, d->globalContour.GetHeight()));
            }
            else
            {
                paper = new QGraphicsRectItem(QRectF(0, 0, d->globalContour.GetWidth(), d->globalContour.GetHeight()));
            }
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
