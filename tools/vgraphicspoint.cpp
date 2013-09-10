#include "vgraphicspoint.h"
#include "options.h"

VGraphicsPoint::VGraphicsPoint(QGraphicsItem *parent): QGraphicsEllipseItem(parent),
    radius(toPixel(1.5)), namePoint(0), lineName(0){
    //namePoint = new VGraphicsSimpleTextItem(this);
    lineName = new QGraphicsLineItem(this);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
}

VGraphicsPoint::~VGraphicsPoint(){
}


void VGraphicsPoint::RefreshLine(){
    QRectF nameRec = namePoint->sceneBoundingRect();
    QPointF p1, p2;
    LineIntersectCircle(QPointF(), radius, QLineF(QPointF(), nameRec.center()- scenePos()), p1, p2);
    QPointF pRec = LineIntersectRect(nameRec, QLineF(scenePos(), nameRec.center()));
    lineName->setLine(QLineF(p1, pRec - scenePos()));
    if(QLineF(p1, pRec - scenePos()).length() <= toPixel(4)){
        lineName->setVisible(false);
    } else {
        lineName->setVisible(true);
    }
}

void VGraphicsPoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthMainLine));
}

void VGraphicsPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthHairLine));
}
