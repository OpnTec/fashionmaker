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

VMainGraphicsView::VMainGraphicsView(QWidget *parent) :
    QGraphicsView(parent){
    QGraphicsView::setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

void VMainGraphicsView::wheelEvent(QWheelEvent *event){
    if (QApplication::keyboardModifiers() == Qt::ControlModifier){
        // Если нажата клавиша CTRL этот код выполнится
        if ((event->delta())>0){
            ZoomIn();
        } else if ((event->delta())<0){
            ZoomOut();
        }
    } else {
       verticalScrollBar()->setValue(verticalScrollBar()->value() - event->delta());
    }
}

void VMainGraphicsView::ZoomIn(){
    scale(1.1,1.1);
}

void VMainGraphicsView::ZoomOut(){
    scale(1/1.1,1/1.1);
}
