#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "vmaingraphicsscene.h"
#include <QDebug>
#include <QApplication>
#include <QScrollBar>
#include <QGraphicsItem>
#pragma GCC diagnostic pop

VMainGraphicsScene::VMainGraphicsScene():QGraphicsScene(){
}


VMainGraphicsScene::VMainGraphicsScene(const QRectF & sceneRect, QObject * parent):QGraphicsScene ( sceneRect, parent ){
}

void VMainGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    emit mouseMove(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

void VMainGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit mousePress(event->scenePos());
    QGraphicsScene::mousePressEvent(event);
}

void VMainGraphicsScene::ChoosedItem(qint64 id, Scene::Type type){
    emit ChoosedObject(id, type);
}

void VMainGraphicsScene::RemoveTool(QGraphicsItem *tool){
    this->removeItem(tool);
}
