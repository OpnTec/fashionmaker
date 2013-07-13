#include "vtoolsimplepoint.h"
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QGraphicsItem>
#include <cmath>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

#include "../options.h"
#include "../container/vpointf.h"
#include "../dialogs/dialogsinglepoint.h"

VToolSimplePoint::VToolSimplePoint (VDomDocument *doc, VContainer *data, qint64 id, Tool::Enum typeCreation,
                                    QGraphicsItem * parent ):QGraphicsEllipseItem(parent){
    InitializeSimplePoint(doc, data, id);
    if(typeCreation == Tool::FromGui){
        AddSimplePointToFile();
    }
}

void VToolSimplePoint::InitializeSimplePoint(VDomDocument *doc, VContainer *data, qint64 id){
    ignoreContextMenuEvent = false;//don't ignore context menu events;
    this->doc = doc;
    this->data = data;
    radius = 1.5*PrintDPI/25.4;
    this->id = id;
    nameActivDraw = doc->GetNameActivDraw();
    //create circle
    VPointF point = data->GetPoint(id);
    QRectF rec = QRectF(point.x(), point.y(), radius*2, radius*2);
    rec.translate(point.x()-rec.center().x(), point.y()-rec.center().y());
    this->setRect(rec);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    //Тексто мітка точки
    namePoint = new VGraphicsSimpleTextItem(point.name(), this);
    rec = this->rect();
    namePoint->setPos(QPointF(rec.center().x()+point.mx(), rec.center().y()+point.my()));
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VToolSimplePoint::NameChangePosition);

    //Лінія, що з'єднує точку і мітку
    QRectF nameRec = namePoint->sceneBoundingRect();
    QPointF p1, p2;
    LineIntersectCircle(rec.center(), radius, QLineF(rec.center(), nameRec.center()), p1, p2);
    QPointF pRec = LineIntersectRect(nameRec, QLineF(rec.center(), nameRec.center()));
    line = new QGraphicsLineItem(QLineF(p1, pRec), this);
    line->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    if(QLineF(p1, pRec).length() <= 4*PrintDPI/25.4){
        line->setVisible(false);
    } else {
        line->setVisible(true);
    }

    connect(this->doc, &VDomDocument::ChangedActivDraw, this, &VToolSimplePoint::ChangedActivDraw);
    connect(this->doc, &VDomDocument::ChangedNameDraw, this, &VToolSimplePoint::ChangedNameDraw);
    connect(this, &VToolSimplePoint::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
    connect(this, &VToolSimplePoint::haveLiteChange, this->doc, &VDomDocument::haveLiteChange);
    connect(this->doc, &VDomDocument::FullUpdateFromFile, this, &VToolSimplePoint::FullUpdateFromFile);
}

void VToolSimplePoint::NameChangePosition(const QPointF pos){
    VPointF point = data->GetPoint(id);
    QRectF rec = this->rect();
    point.setMx(pos.x() - rec.center().x());
    point.setMy(pos.y() - rec.center().y());
    RefreshLine();
    LiteUpdateFromGui(point.name(), point.mx(), point.my());
    data->UpdatePoint(id, point);
}

/*
 * Взято з сайту http://hardfire.ru/cross_line_circle
 */
qint32 VToolSimplePoint::LineIntersectCircle(QPointF center, qreal radius, QLineF line, QPointF &p1,
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
QPointF VToolSimplePoint::add_vector (QPointF p, QPointF p1, QPointF p2, qreal k) const{
    return QPointF (p.x() + (p2.x() - p1.x()) * k, p.y() + (p2.y() - p1.y()) * k);
}

QPointF VToolSimplePoint::ClosestPoint(QLineF line, QPointF p) const{
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

QPointF VToolSimplePoint::LineIntersectRect(QRectF rec, QLineF line) const{
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

void VToolSimplePoint::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        this->setPen(QPen(Qt::black, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        namePoint->setFlag(QGraphicsItem::ItemIsMovable, true);
        namePoint->setFlag(QGraphicsItem::ItemIsSelectable, true);
        namePoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
        namePoint->setBrush(QBrush(Qt::black));
        line->setPen(QPen(Qt::black, widthHairLine));
        ignoreContextMenuEvent = false;
    } else {
        this->setPen(QPen(Qt::gray, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        namePoint->setFlag(QGraphicsItem::ItemIsMovable, false);
        namePoint->setFlag(QGraphicsItem::ItemIsSelectable, false);
        namePoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        namePoint->setBrush(QBrush(Qt::gray));
        line->setPen(QPen(Qt::gray, widthHairLine));
        ignoreContextMenuEvent = true;
    }
}

void VToolSimplePoint::AddSimplePointToFile() const{
    VPointF point = data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    QDomAttr domAttr = doc->createAttribute("id");
    domAttr.setValue(QString().setNum(id));
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("type");
    domAttr.setValue("simple");
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("name");
    domAttr.setValue(point.name());
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("x");
    domAttr.setValue(QString().setNum(point.x()/PrintDPI*25.4));
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("y");
    domAttr.setValue(QString().setNum(point.y()/PrintDPI*25.4));
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("mx");
    domAttr.setValue(QString().setNum(point.mx()/PrintDPI*25.4));
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("my");
    domAttr.setValue(QString().setNum(point.my()/PrintDPI*25.4));
    domElement.setAttributeNode(domAttr);

    QDomElement calcElement;
    bool ok = doc->GetActivCalculationElement(calcElement);
    if(ok){
        calcElement.appendChild(domElement);
    } else {
        qCritical("Не можу знайти тег калькуляції. VToolSimplePoint::AddSimplePoint");
    }
}

void VToolSimplePoint::LiteUpdateFromGui(const QString& name, qreal mx, qreal my){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        domElement.setAttribute("name", name);
        domElement.setAttribute("mx", QString().setNum(mx/PrintDPI*25.4));
        domElement.setAttribute("my", QString().setNum(my/PrintDPI*25.4));
        emit haveLiteChange();
    }
}

void VToolSimplePoint::FullUpdateFromGui(const QString &name, qreal x, qreal y){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        domElement.setAttribute("name", name);
        domElement.setAttribute("x", QString().setNum(x/PrintDPI*25.4));
        domElement.setAttribute("y", QString().setNum(y/PrintDPI*25.4));
        emit FullUpdateTree();
    }
}

void  VToolSimplePoint::ChangedNameDraw(const QString oldName, const QString newName){
    if(nameActivDraw == oldName){
        nameActivDraw = newName;
    }
}

void VToolSimplePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            DialogSinglePoint *dialogSinglePoint = new DialogSinglePoint;
            VPointF p = data->GetPoint(id);
            dialogSinglePoint->setData(p.name(), p.toQPointF());
            qint32 result = dialogSinglePoint->exec();
            if(result == QDialog::Accepted){
                QPointF p = dialogSinglePoint->getPoint();
                FullUpdateFromGui(dialogSinglePoint->getName(), p.x(), p.y());
            }
            delete dialogSinglePoint;
        }
    }
}

void  VToolSimplePoint::FullUpdateFromFile(){
    QString name;
    qreal x, y, mx, my;
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        name = domElement.attribute("name", "");
        x = domElement.attribute("x", "").toDouble()*PrintDPI/25.4;
        y = domElement.attribute("y", "").toDouble()*PrintDPI/25.4;
        mx = domElement.attribute("mx", "").toDouble()*PrintDPI/25.4;
        my = domElement.attribute("my", "").toDouble()*PrintDPI/25.4;
    }
    QRectF rec = QRectF(x, y, radius*2, radius*2);
    rec.translate(x-rec.center().x(), y-rec.center().y());
    this->setRect(rec);

    rec = this->rect();
    namePoint->setText(name);
    namePoint->setPos(QPointF(rec.center().x()+mx, rec.center().y()+my));

    RefreshLine();
}

void VToolSimplePoint::RefreshLine(){
    QRectF nameRec = namePoint->sceneBoundingRect();
    QPointF p1, p2;
    QRectF rec = this->rect();
    LineIntersectCircle(rec.center(), radius, QLineF(rec.center(), nameRec.center()), p1, p2);
    QPointF pRec = LineIntersectRect(nameRec, QLineF(rec.center(), nameRec.center()));
    line->setLine(QLineF(p1, pRec));
    if(QLineF(p1, pRec).length() <= 4*PrintDPI/25.4){
        line->setVisible(false);
    } else {
        line->setVisible(true);
    }
}
