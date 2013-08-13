#ifndef VMAINGRAPHICSSCENE_H
#define VMAINGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

#include "../options.h"

class VMainGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    VMainGraphicsScene();
    VMainGraphicsScene(const QRectF & sceneRect, QObject * parent = 0);
public slots:
    void ChoosedItem(qint64 id, Scene::Type type);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void mouseMove(QPointF scenePos);
    void mousePress(QPointF scenePos);
    void ChoosedObject(qint64 id, Scene::Type type);
};

#endif // VMAINGRAPHICSSCENE_H
