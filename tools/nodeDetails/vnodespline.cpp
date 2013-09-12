#include "vnodespline.h"

VNodeSpline::VNodeSpline(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                 Draw::Mode typeobject, Tool::Enum typeCreation, QGraphicsItem * parent) :
    VAbstractNode(doc, data, id, idSpline, typeobject), QGraphicsPathItem(parent){
    RefreshGeometry();
    this->setPen(QPen(baseColor, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

VNodeSpline *VNodeSpline::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                         Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation){
    VNodeSpline *spl = 0;
    if(parse == Document::FullParse){
        spl = new VNodeSpline(doc, data, id, idSpline, typeobject, typeCreation);
        doc->AddTool(id, spl);
    } else {
        doc->UpdateToolData(id, data);
    }
    return spl;
}

void VNodeSpline::FullUpdateFromFile(){
    RefreshGeometry();
}

void VNodeSpline::AddToFile(){
    QDomElement domElement = doc->createElement("spline");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "modelingSpline");
    AddAttribute(domElement, "idObject", idNode);
    if(typeobject == Draw::Calculation){
        AddAttribute(domElement, "typeObject", "Calculation");
    } else {
        AddAttribute(domElement, "typeObject", "Modeling");
    }

    AddToModeling(domElement);
}

void VNodeSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Spline);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VNodeSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VNodeSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VNodeSpline::RefreshGeometry(){
    VSpline spl = VAbstractTool::data.GetModelingSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
