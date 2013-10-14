/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vmaingraphicsview.h"
#include <QApplication>
#include <QWheelEvent>
#include <QScrollBar>
#include <QTimeLine>

VMainGraphicsView::VMainGraphicsView(QWidget *parent) :
    QGraphicsView(parent), _numScheduledScalings(0){
    QGraphicsView::setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

void VMainGraphicsView::wheelEvent(QWheelEvent *event){
    // If you press CTRL this code will execute
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;  // see QWheelEvent documentation
    _numScheduledScalings += numSteps;
    if (_numScheduledScalings * numSteps < 0){  // if user moved the wheel in another direction, we reset
        _numScheduledScalings = numSteps;       // previously scheduled scalings
    }

    QTimeLine *anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, &QTimeLine::valueChanged, this, &VMainGraphicsView::scalingTime);
    connect(anim, &QTimeLine::finished, this, &VMainGraphicsView::animFinished);
    anim->start();
}

void VMainGraphicsView::scalingTime(qreal x){
    Q_UNUSED(x);
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 300.0;
    if (QApplication::keyboardModifiers() == Qt::ControlModifier){
        scale(factor, factor);
    } else {
        if(_numScheduledScalings < 0){
            verticalScrollBar()->setValue(qRound(verticalScrollBar()->value() + factor*3.5));
        } else {
            verticalScrollBar()->setValue(qRound(verticalScrollBar()->value() - factor*3.5));
        }
    }
}

void VMainGraphicsView::animFinished(){
    if (_numScheduledScalings > 0){
        _numScheduledScalings--;
    } else {
        _numScheduledScalings++;
    }
    sender()->~QObject();
}
