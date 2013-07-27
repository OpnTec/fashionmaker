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
    QRectF rec = QRectF(point.x(), point.y(), radius*2, radius*2);
    rec.translate(point.x()-rec.center().x(), point.y()-rec.center().y());
    this->setRect(rec);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    //Тексто мітка точки
    namePoint = new VGraphicsSimpleTextItem(point.name(), this);
    rec = this->rect();
    namePoint->setPos(QPointF(rec.center().x()+point.mx(), rec.center().y()+point.my()));
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VToolPoint::NameChangePosition);

    //Лінія, що з'єднує точку і мітку
    QRectF nameRec = namePoint->sceneBoundingRect();
    QPointF p1, p2;
    LineIntersectCircle(rec.center(), radius, QLineF(rec.center(), nameRec.center()), p1, p2);
    QPointF pRec = LineIntersectRect(nameRec, QLineF(rec.center(), nameRec.center()));
    lineName = new QGraphicsLineItem(QLineF(p1, pRec), this);
    lineName->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    if(QLineF(p1, pRec).length() <= 4*PrintDPI/25.4){
        lineName->setVisible(false);
    } else {
        lineName->setVisible(true);
    }
}

void VToolPoint::NameChangePosition(const QPointF pos){
    VPointF point = VAbstractTool::data->GetPoint(id);
    QRectF rec = this->rect();
    point.setMx(pos.x() - rec.center().x());
    point.setMy(pos.y() - rec.center().y());
    RefreshLine();
    LiteUpdateFromGui(point.mx(), point.my());
    VAbstractTool::data->UpdatePoint(id, point);
}

/*
 * Взято з сайту http://hardfire.ru/cross_line_circle
 */
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
    p1 = add_vector (p, QPointF (0, 0), QPointF (- b, a), k / t);
    p2 = add_vector (p, QPointF (0, 0), QPointF (b, - a), k / t);
    return flag;
}

/*
 * Добавление вектора к точке
 * Взято з сайту http://hardfire.ru/add_vector
 */
QPointF VToolPoint::add_vector (QPointF p, QPointF p1, QPointF p2, qreal k) const{
    return QPointF (p.x() + (p2.x() - p1.x()) * k, p.y() + (p2.y() - p1.y()) * k);
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
    QRectF rec = this->rect();
    LineIntersectCircle(rec.center(), radius, QLineF(rec.center(), nameRec.center()), p1, p2);
    QPointF pRec = LineIntersectRect(nameRec, QLineF(rec.center(), nameRec.center()));
    lineName->setLine(QLineF(p1, pRec));
    if(QLineF(p1, pRec).length() <= 4*PrintDPI/25.4){
        lineName->setVisible(false);
    } else {
        lineName->setVisible(true);
    }
}

void VToolPoint::LiteUpdateFromGui(qreal mx, qreal my){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        domElement.setAttribute("mx", QString().setNum(mx/PrintDPI*25.4));
        domElement.setAttribute("my", QString().setNum(my/PrintDPI*25.4));
        emit haveLiteChange();
    }
}

void VToolPoint::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        this->setPen(QPen(Qt::black, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        namePoint->setFlag(QGraphicsItem::ItemIsMovable, true);
        namePoint->setFlag(QGraphicsItem::ItemIsSelectable, true);
        namePoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
        namePoint->setBrush(QBrush(Qt::black));
        namePoint->setAcceptHoverEvents(true);
        lineName->setPen(QPen(Qt::black, widthHairLine));
        this->setAcceptHoverEvents(true);
        VAbstractTool::ChangedActivDraw(newName);
    } else {
        this->setPen(QPen(Qt::gray, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        namePoint->setFlag(QGraphicsItem::ItemIsMovable, false);
        namePoint->setFlag(QGraphicsItem::ItemIsSelectable, false);
        namePoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        namePoint->setBrush(QBrush(Qt::gray));
        namePoint->setAcceptHoverEvents(false);
        lineName->setPen(QPen(Qt::gray, widthHairLine));
        this->setAcceptHoverEvents (false);
        VAbstractTool::ChangedActivDraw(newName);
    }
}

void VToolPoint::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ){
    if(event->button() == Qt::LeftButton){
        emit ChoosedPoint(id, Scene::Point);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolPoint::RefreshBaseGeometry(const QString &name, const qreal &x, const qreal &y, const qreal &mx,
                                     const qreal &my){
    QRectF rec = QRectF(x, y, radius*2, radius*2);
    rec.translate(x-rec.center().x(), y-rec.center().y());
    this->setRect(rec);

    rec = this->rect();
    namePoint->setText(name);
    namePoint->setPos(QPointF(rec.center().x()+mx, rec.center().y()+my));

    RefreshLine();
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
