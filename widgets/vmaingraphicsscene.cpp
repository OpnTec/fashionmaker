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

void VMainGraphicsScene::ChoosedItem(qint64 id, Scene::Type type){
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
