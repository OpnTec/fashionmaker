#ifndef VMAINGRAPHICSSCENE_H
#define VMAINGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include "options.h"

class VMainGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    VMainGraphicsScene();
    VMainGraphicsScene(const QRectF & sceneRect, QObject * parent = 0);
    qint32 getHorScrollBar() const;
    void setHorScrollBar(const qint32 &value);

    qint32 getVerScrollBar() const;
    void setVerScrollBar(const qint32 &value);

public slots:
    void ChoosedItem(qint64 id, Scene::Type type);
    void RemoveTool(QGraphicsItem *tool);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void mouseMove(QPointF scenePos);
    void mousePress(QPointF scenePos);
    void ChoosedObject(qint64 id, Scene::Type type);
private:
    qint32 horScrollBar;
    qint32 verScrollBar;
};

#endif // VMAINGRAPHICSSCENE_H
