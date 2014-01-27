/************************************************************************
 **
 **  @file   vmaingraphicsview.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vmaingraphicsview.h"

#include <QTimeLine>
#include <QWheelEvent>
#include <QApplication>
#include <QScrollBar>
#include "../tools/vabstracttool.h"

VMainGraphicsView::VMainGraphicsView(QWidget *parent)
    :QGraphicsView(parent), _numScheduledScalings(0)
{
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

//cppcheck-suppress unusedFunction
void VMainGraphicsView::wheelEvent(QWheelEvent *event)
{
    int numSteps = event->delta() / 8 / 15;  // see QWheelEvent documentation

    _numScheduledScalings += numSteps;
    if (_numScheduledScalings * numSteps < 0)
    {  // if user moved the wheel in another direction, we reset
        _numScheduledScalings = numSteps;       // previously scheduled scalings
    }

    QTimeLine *anim = new QTimeLine(300, this);
    Q_ASSERT(anim != 0);
    anim->setUpdateInterval(20);

    if (QApplication::keyboardModifiers() == Qt::ControlModifier)
    {// If you press CTRL this code will be executed
        connect(anim, &QTimeLine::valueChanged, this, &VMainGraphicsView::scalingTime);
    }
    else
    {
        connect(anim, &QTimeLine::valueChanged, this, &VMainGraphicsView::scrollingTime);
    }
    connect(anim, &QTimeLine::finished, this, &VMainGraphicsView::animFinished);
    anim->start();
}

void VMainGraphicsView::scalingTime(qreal x)
{
    Q_UNUSED(x);
    const QPointF p0scene = mapToScene(mapFromGlobal(QCursor::pos()));

    qreal factor = 1.0 + static_cast<qreal>(_numScheduledScalings) / 50.0;
    scale(factor, factor);

    const QPointF p1mouse = mapFromScene(p0scene);
    const QPointF move = p1mouse - this->mapFromGlobal(QCursor::pos()); // The move
    horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
    verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());

    VAbstractTool::NewSceneRect(this->scene(), this);

    emit NewFactor(factor);
}

void VMainGraphicsView::scrollingTime(qreal x)
{
    Q_UNUSED(x);
    qreal factor = 1.0;
    if (_numScheduledScalings < 0)
    {
        verticalScrollBar()->setValue(qRound(verticalScrollBar()->value() + factor*3.5));
        emit NewFactor(factor);
    }
    else
    {
//        if (verticalScrollBar()->value() > 0)
//        {
            verticalScrollBar()->setValue(qRound(verticalScrollBar()->value() - factor*3.5));
            emit NewFactor(factor);
//        }
    }
}

void VMainGraphicsView::animFinished()
{
    if (_numScheduledScalings > 0)
    {
        _numScheduledScalings--;
    }
    else
    {
        _numScheduledScalings++;
    }
    sender()->~QObject();
}

void VMainGraphicsView::mousePressEvent(QMouseEvent *mousePress)
{
    if (mousePress->button() & Qt::LeftButton)
    {
        switch (QGuiApplication::keyboardModifiers())
        {
            case Qt::ControlModifier:
                QGraphicsView::setDragMode(QGraphicsView::ScrollHandDrag);
                QGraphicsView::mousePressEvent(mousePress);
                break;
            default:
                QGraphicsView::mousePressEvent(mousePress);
                break;
        }
    }
}

void VMainGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent ( event );
    QGraphicsView::setDragMode( QGraphicsView::RubberBandDrag );
}
