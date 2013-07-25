#include "vmaingraphicsscene.h"
#include <QDebug>


VMainGraphicsScene::VMainGraphicsScene():QGraphicsScene()
{
}


VMainGraphicsScene::VMainGraphicsScene(const QRectF & sceneRect, QObject * parent):QGraphicsScene ( sceneRect, parent )
{
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
