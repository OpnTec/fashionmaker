#include "vnodearc.h"

VNodeArc::VNodeArc(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc, Draw::Mode typeobject,
                           Tool::Enum typeCreation, QGraphicsItem * parent) :
    VAbstractNode(doc, data, id, idArc, typeobject), QGraphicsPathItem(parent){
    RefreshGeometry();
    this->setPen(QPen(baseColor, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

VNodeArc *VNodeArc::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc,
                                   Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation){
    VNodeArc *arc = 0;
    if(parse == Document::FullParse){
        arc = new VNodeArc(doc, data, id, idArc, typeobject, typeCreation);
        doc->AddTool(id, arc);
    } else {
        doc->UpdateToolData(id, data);
    }
    return arc;
}

void VNodeArc::FullUpdateFromFile(){
    RefreshGeometry();
}

void VNodeArc::AddToFile(){
    QDomElement domElement = doc->createElement("arc");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "modeling");
    AddAttribute(domElement, "idObject", idNode);
    if(typeobject == Draw::Calculation){
        AddAttribute(domElement, "typeObject", "Calculation");
    } else {
        AddAttribute(domElement, "typeObject", "Modeling");
    }

    AddToModeling(domElement);
}

void VNodeArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Arc);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VNodeArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VNodeArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VNodeArc::RefreshGeometry(){
    VArc arc = VAbstractTool::data.GetModelingArc(id);
    QPainterPath path;
    path.addPath(arc.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
