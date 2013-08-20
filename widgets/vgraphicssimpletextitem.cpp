#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vgraphicssimpletextitem.h"
#include <QGraphicsScene>
#pragma GCC diagnostic pop

VGraphicsSimpleTextItem::VGraphicsSimpleTextItem(QGraphicsItem * parent):QGraphicsSimpleTextItem(parent){
}

VGraphicsSimpleTextItem::VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent ):QGraphicsSimpleTextItem(text, parent){
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

QVariant VGraphicsSimpleTextItem::itemChange(GraphicsItemChange change, const QVariant &value){
     if (change == ItemPositionChange && scene()) {
         // value - это новое положение.
         QPointF newPos = value.toPointF() + this->parentItem()->pos();
//         QRectF rect = scene()->sceneRect();
//         if (!rect.contains(newPos)) {
//             // Сохраняем элемент внутри прямоугольника сцены.
//             newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
//             newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
//             emit NameChangePosition(newPos - this->parentItem()->pos());
//             return newPos - this->parentItem()->pos();
//         }
         emit NameChangePosition(newPos);
     }
     return QGraphicsItem::itemChange(change, value);
}

void VGraphicsSimpleTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setBrush(Qt::green);
}

void VGraphicsSimpleTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setBrush(Qt::black);
}
