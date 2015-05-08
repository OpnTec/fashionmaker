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
#include "vlayoutpaper_p.h"
#include "vbestsquare.h"
#include "vposition.h"

#include <QGraphicsItem>
#include <QCoreApplication>
#include <QThreadPool>
#include <QPen>
#include <QGraphicsScene>

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
bool VLayoutPaper::GetRotate() const
{
    return d->rotate;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetRotate(bool value)
{
    d->rotate = value;
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::GetRotationIncrease() const
{
    return d->rotationIncrease;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetRotationIncrease(int value)
{
    d->rotationIncrease = value;

    if ((d->rotationIncrease >= 1 && d->rotationIncrease <= 180 && 360 % d->rotationIncrease == 0) == false)
    {
        d->rotationIncrease = 180;
    }
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
bool VLayoutPaper::ArrangeDetail(const VLayoutDetail &detail, volatile bool &stop)
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
bool VLayoutPaper::AddToSheet(const VLayoutDetail &detail, volatile bool &stop)
{
    VBestSquare bestResult(d->globalContour.GetSize(), d->saveLength);
    QThreadPool *thread_pool = QThreadPool::globalInstance();
    thread_pool->setExpiryTimeout(1000);
    QVector<VPosition *> threads;

    for (int j=1; j <= d->globalContour.EdgesCount(); ++j)
    {
        for (int i=1; i<= detail.EdgesCount(); i++)
        {
            VPosition *thread = new VPosition(d->globalContour, j, detail, i, &stop, d->rotate, d->rotationIncrease,
                                              d->saveLength);
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

            d->frame = d->frame + 3 + static_cast<unsigned int>(360/d->rotationIncrease*2);
        }
    }

    // Wait for done
    do
    {
        QCoreApplication::processEvents();
        QThread::msleep(250);
    }
    while(thread_pool->activeThreadCount() > 0 && not stop);

    if (stop)
    {
        qDeleteAll(threads.begin(), threads.end());
        threads.clear();
        return false;
    }

    for (int i=0; i < threads.size(); ++i)
    {
        bestResult.NewResult(threads.at(i)->getBestResult());
    }

    qDeleteAll(threads.begin(), threads.end());
    threads.clear();

    return SaveResult(bestResult, detail);
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::SaveResult(const VBestSquare &bestResult, const VLayoutDetail &detail)
{
    if (bestResult.ValidResult())
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
        VPosition::DrawDebug(d->globalContour, workDetail, UINT_MAX, d->paperIndex, d->details.count(), d->details);
#   endif
#endif
    }

    return bestResult.ValidResult(); // Do we have the best result?
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsRectItem *VLayoutPaper::GetPaperItem(bool autoCrop) const
{
    QGraphicsRectItem *paper;
    if (autoCrop)
    {
        QGraphicsScene *scene = new QGraphicsScene();
        QList<QGraphicsItem *> list = GetDetails();
        for (int i=0; i < list.size(); ++i)
        {
            scene->addItem(list.at(i));
        }
        const int height = scene->itemsBoundingRect().toRect().height() + static_cast<int>(d->layoutWidth)*2;
        delete scene;
        if (d->globalContour.GetHeight() > height)
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
        paper = new QGraphicsRectItem(QRectF(0, 0, d->globalContour.GetWidth(), d->globalContour.GetHeight()));
    }
    paper->setPen(QPen(Qt::black, 1));
    paper->setBrush(QBrush(Qt::white));
    return paper;
}

//---------------------------------------------------------------------------------------------------------------------
QList<QGraphicsItem *> VLayoutPaper::GetDetails() const
{
    QList<QGraphicsItem *> list;
    for (int i=0; i < d->details.count(); ++i)
    {
        list.append(d->details.at(i).GetItem());
    }
    return list;
}
