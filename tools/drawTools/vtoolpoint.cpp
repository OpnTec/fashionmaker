#include "vtoolpoint.h"
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QGraphicsItem>
#include "container/vpointf.h"


VToolPoint::VToolPoint(VDomDocument *doc, VContainer *data, qint64 id,
                       QGraphicsItem *parent):VDrawTool(doc, data, id),
    QGraphicsEllipseItem(parent), radius(toPixel(1.5)), namePoint(0), lineName(0){
    namePoint = new VGraphicsSimpleTextItem(this);
    lineName = new QGraphicsLineItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VToolPoint::NameChangePosition);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
    RefreshPointGeometry(VAbstractTool::data.GetPoint(id));
}

void VToolPoint::NameChangePosition(const QPointF pos){
    VPointF point = VAbstractTool::data.GetPoint(id);
    QPointF p = pos - this->pos();
    point.setMx(p.x());
    point.setMy(p.y());
    RefreshLine();
    UpdateNamePosition(point.mx(), point.my());
    VAbstractTool::data.UpdatePoint(id, point);
}

void VToolPoint::UpdateNamePosition(qreal mx, qreal my){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        domElement.setAttribute("mx", QString().setNum(toMM(mx)));
        domElement.setAttribute("my", QString().setNum(toMM(my)));
        emit toolhaveChange();
    }
}

void VToolPoint::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        this->setPen(QPen(Qt::black, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->setAcceptHoverEvents(true);
        namePoint->setFlag(QGraphicsItem::ItemIsMovable, true);
        namePoint->setFlag(QGraphicsItem::ItemIsSelectable, true);
        namePoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
        namePoint->setBrush(QBrush(Qt::black));
        namePoint->setAcceptHoverEvents(true);
        lineName->setPen(QPen(Qt::black, widthHairLine));
        VDrawTool::ChangedActivDraw(newName);
    } else {
        this->setPen(QPen(Qt::gray, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->setAcceptHoverEvents (false);
        namePoint->setFlag(QGraphicsItem::ItemIsMovable, false);
        namePoint->setFlag(QGraphicsItem::ItemIsSelectable, false);
        namePoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        namePoint->setBrush(QBrush(Qt::gray));
        namePoint->setAcceptHoverEvents(false);
        lineName->setPen(QPen(Qt::gray, widthHairLine));
        VDrawTool::ChangedActivDraw(newName);
    }
}

void VToolPoint::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
    if(id == this->id){
        if(enable == false){
            this->setPen(QPen(baseColor, widthHairLine));
            currentColor = baseColor;
        } else {
            this->setPen(QPen(color, widthHairLine));
            currentColor = color;
        }
    }
}

void VToolPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Point);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolPoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VToolPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VToolPoint::RefreshPointGeometry(const VPointF &point){
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPos(point.toQPointF());
    disconnect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
               &VToolPoint::NameChangePosition);
    namePoint->setText(point.name());
    namePoint->setPos(QPointF(point.mx(), point.my()));
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VToolPoint::NameChangePosition);
    RefreshLine();
}

void VToolPoint::RefreshLine(){
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

VToolPoint::~VToolPoint(){
}
