#include "vtoolpoint.h"
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QGraphicsItem>
#include <cmath>

#include "../container/vpointf.h"
#include "../widgets/vmaingraphicsscene.h"

VToolPoint::VToolPoint(VDomDocument *doc, VContainer *data, qint64 id,
                       QGraphicsItem *parent):VAbstractTool(doc, data, id), QGraphicsEllipseItem(parent){
    radius = 1.5*PrintDPI/25.4;
    //create circle
    VPointF point = data->GetPoint(id);
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
    this->setPos(point.toQPointF());

    //Тексто мітка точки
    namePoint = new VGraphicsSimpleTextItem(point.name(), this);
    rec = this->rect();
    namePoint->setPos(QPointF(point.mx(), point.my()));
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VToolPoint::NameChangePosition);

    //Лінія, що з'єднує точку і мітку
    lineName = new QGraphicsLineItem(this);
    RefreshLine();
}

void VToolPoint::NameChangePosition(const QPointF pos){
    VPointF point = VAbstractTool::data.GetPoint(id);
    QPointF p = pos - this->pos();
    point.setMx(p.x());
    point.setMy(p.y());
    RefreshLine();
    LiteUpdateFromGui(point.mx(), point.my());
    VAbstractTool::data.UpdatePoint(id, point);
}

QPointF VToolPoint::LineIntersectRect(QRectF rec, QLineF line) const{
    qreal x1, y1, x2, y2;
    rec.getCoords(&x1, &y1, &x2, &y2);
    QPointF point;
    QLineF::IntersectType type = line.intersect(QLineF(QPointF(x1,y1), QPointF(x1,y2)),&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    }
    type = line.intersect(QLineF(QPointF(x1,y1), QPointF(x2,y1)),&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    }
    type = line.intersect(QLineF(QPointF(x1,y2), QPointF(x2,y2)),&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    }
    type = line.intersect(QLineF(QPointF(x2,y1), QPointF(x2,y2)),&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    }
    Q_ASSERT_X(type != QLineF::BoundedIntersection, Q_FUNC_INFO, "Немає точки перетину.");
    return point;
}

void VToolPoint::RefreshLine(){
    QRectF nameRec = namePoint->sceneBoundingRect();
    QPointF p1, p2;
    LineIntersectCircle(QPointF(), radius, QLineF(QPointF(), nameRec.center()- pos()), p1, p2);
    QPointF pRec = LineIntersectRect(nameRec, QLineF(pos(), nameRec.center()));
    lineName->setLine(QLineF(p1, pRec - pos()));
    if(QLineF(p1, pRec - pos()).length() <= 4*PrintDPI/25.4){
        lineName->setVisible(false);
    } else {
        lineName->setVisible(true);
    }
}

qint32 VToolPoint::LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
                                       QPointF &p2) const{
    const qreal eps = 1e-8;
    //коефіцієнти для рівняння відрізку
    qreal a = line.p2().y() - line.p1().y();
    qreal b = line.p1().x() - line.p2().x();
    // В даному випадку не використовується.
    //qreal c = - a * line.p1().x() - b * line.p1().y();
    // проекция центра окружности на прямую
    QPointF p = ClosestPoint (line, center);
    // сколько всего решений?
    qint32 flag = 0;
    qreal d = QLineF (center, p).length();
    if (qAbs (d - radius) <= eps){
        flag = 1;
    } else {
        if (radius > d){
            flag = 2;
        } else {
            return 0;
        }
    }
    // находим расстояние от проекции до точек пересечения
    qreal k = sqrt (radius * radius - d * d);
    qreal t = QLineF (QPointF (0, 0), QPointF (b, - a)).length();
    // добавляем к проекции векторы направленные к точкам пеерсечения
    p1 = addVector (p, QPointF (0, 0), QPointF (- b, a), k / t);
    p2 = addVector (p, QPointF (0, 0), QPointF (b, - a), k / t);
    return flag;
}

QPointF VToolPoint::ClosestPoint(QLineF line, QPointF p) const{
    QLineF lineP2pointFrom = QLineF(line.p2(), p);
    qreal angle = 180-line.angleTo(lineP2pointFrom)-90;
    QLineF pointFromlineP2 = QLineF(p, line.p2());
    pointFromlineP2.setAngle(pointFromlineP2.angle()+angle);
    QPointF point;
    QLineF::IntersectType type = pointFromlineP2.intersect(line,&point);
    if ( type == QLineF::BoundedIntersection ){
        return point;
    } else{
        if ( type == QLineF::NoIntersection || type == QLineF::UnboundedIntersection ){
            Q_ASSERT_X(type != QLineF::BoundedIntersection, Q_FUNC_INFO, "Немає точки перетину.");
            return point;
        }
    }
    return point;
}

QPointF VToolPoint::addVector(QPointF p, QPointF p1, QPointF p2, qreal k) const{
    return QPointF (p.x() + (p2.x() - p1.x()) * k, p.y() + (p2.y() - p1.y()) * k);
}

void VToolPoint::LiteUpdateFromGui(qreal mx, qreal my){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        domElement.setAttribute("mx", QString().setNum(mx/PrintDPI*25.4));
        domElement.setAttribute("my", QString().setNum(my/PrintDPI*25.4));
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
        VAbstractTool::ChangedActivDraw(newName);
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
        VAbstractTool::ChangedActivDraw(newName);
    }
}

void VToolPoint::RefreshGeometry(){
    VPointF point = VAbstractTool::data.GetPoint(id);
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

void VToolPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Point);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolPoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthMainLine));
}

void VToolPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthHairLine));
}

VToolPoint::~VToolPoint(){

}
