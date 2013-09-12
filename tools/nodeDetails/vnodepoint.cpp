#include "vnodepoint.h"

VNodePoint::VNodePoint(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint,
                       Draw::Mode typeobject, Tool::Enum typeCreation, QGraphicsItem *parent)
    :VAbstractNode(doc, data, id, idPoint, typeobject), QGraphicsEllipseItem(parent),
     radius(toPixel(1.5)), namePoint(0), lineName(0){
    namePoint = new VGraphicsSimpleTextItem(this);
    lineName = new QGraphicsLineItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VNodePoint::NameChangePosition);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
    RefreshPointGeometry(VAbstractTool::data.GetModelingPoint(id));
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

VNodePoint *VNodePoint::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint,
                                       Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation){
    VNodePoint *point = 0;
    if(parse == Document::FullParse){
        point = new VNodePoint(doc, data, id, idPoint, typeobject, typeCreation);
        doc->AddTool(id, point);
    } else {
        doc->UpdateToolData(id, data);
    }
    return point;
}

void VNodePoint::FullUpdateFromFile(){
    RefreshPointGeometry(VAbstractTool::data.GetModelingPoint(id));
}

void VNodePoint::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "modeling");
    AddAttribute(domElement, "idObject", idNode);
    if(typeobject == Draw::Calculation){
        AddAttribute(domElement, "typeObject", "Calculation");
    } else {
        AddAttribute(domElement, "typeObject", "Modeling");
    }
    AddAttribute(domElement, "mx", toMM(point.mx()));
    AddAttribute(domElement, "my", toMM(point.my()));

    AddToModeling(domElement);
}

void VNodePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Point);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VNodePoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VNodePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}


void VNodePoint::NameChangePosition(const QPointF pos){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QPointF p = pos - this->pos();
    point.setMx(p.x());
    point.setMy(p.y());
    RefreshLine();
    UpdateNamePosition(point.mx(), point.my());
    VAbstractTool::data.UpdatePoint(id, point);
}

void VNodePoint::UpdateNamePosition(qreal mx, qreal my){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        domElement.setAttribute("mx", QString().setNum(toMM(mx)));
        domElement.setAttribute("my", QString().setNum(toMM(my)));
        emit toolhaveChange();
    }
}

void VNodePoint::RefreshPointGeometry(const VPointF &point){
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPos(point.toQPointF());
    disconnect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
               &VNodePoint::NameChangePosition);
    namePoint->setText(point.name());
    namePoint->setPos(QPointF(point.mx(), point.my()));
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VNodePoint::NameChangePosition);

    RefreshLine();
}

void VNodePoint::RefreshLine(){
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
