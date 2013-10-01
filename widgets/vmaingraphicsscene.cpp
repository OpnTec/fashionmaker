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

#include "vmaingraphicsscene.h"
#include <QDebug>
#include <QApplication>
#include <QScrollBar>
#include <QGraphicsItem>

VMainGraphicsScene::VMainGraphicsScene():QGraphicsScene(), horScrollBar(0), verScrollBar(0){
}

VMainGraphicsScene::VMainGraphicsScene(const QRectF & sceneRect, QObject * parent):
    QGraphicsScene ( sceneRect, parent ), horScrollBar(0), verScrollBar(0){
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

void VMainGraphicsScene::RemoveTool(QGraphicsItem *tool){
    this->removeItem(tool);
}

qint32 VMainGraphicsScene::getVerScrollBar() const{
    return verScrollBar;
}

void VMainGraphicsScene::setVerScrollBar(const qint32 &value){
    verScrollBar = value;
}

qint32 VMainGraphicsScene::getHorScrollBar() const{
    return horScrollBar;
}

void VMainGraphicsScene::setHorScrollBar(const qint32 &value){
    horScrollBar = value;
}
