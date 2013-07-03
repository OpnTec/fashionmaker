#ifndef VMAINGRAPHICSSCENE_H
#define VMAINGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class VMainGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    VMainGraphicsScene();
    VMainGraphicsScene(const QRectF & sceneRect, QObject * parent = 0);
private:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void mouseMove(QPointF scenePos);
    void mousePress(QPointF scenePos);
};

#endif // VMAINGRAPHICSSCENE_H
