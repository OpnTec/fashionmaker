#include "vgraphicssimpletextitem.h"
#include <QGraphicsScene>
#include <QEvent>

VGraphicsSimpleTextItem::VGraphicsSimpleTextItem(QGraphicsItem * parent):QGraphicsSimpleTextItem(parent){
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

VGraphicsSimpleTextItem::VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent )
    :QGraphicsSimpleTextItem(text, parent){
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

QVariant VGraphicsSimpleTextItem::itemChange(GraphicsItemChange change, const QVariant &value){
     if (change == ItemPositionChange && scene()) {
         QPointF newPos = value.toPointF() + this->parentItem()->pos();
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
