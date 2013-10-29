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
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vmaingraphicsscene.h"

VMainGraphicsScene::VMainGraphicsScene():QGraphicsScene(), horScrollBar(0), verScrollBar(0), scaleFactor(1){
}

VMainGraphicsScene::VMainGraphicsScene(const QRectF & sceneRect, QObject * parent):
    QGraphicsScene ( sceneRect, parent ), horScrollBar(0), verScrollBar(0), scaleFactor(1){
}

void VMainGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    emit mouseMove(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

void VMainGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit mousePress(event->scenePos());
    QGraphicsScene::mousePressEvent(event);
}

void VMainGraphicsScene::ChoosedItem(qint64 id, Scene::Scenes type){
    emit ChoosedObject(id, type);
}

void VMainGraphicsScene::SetFactor(qreal factor){
    scaleFactor=scaleFactor*factor;
    emit NewFactor(scaleFactor);
}
