#include "vnodesplinepath.h"

VNodeSplinePath::VNodeSplinePath(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                         Draw::Mode typeobject, Tool::Enum typeCreation,
                                         QGraphicsItem * parent) :
    VAbstractNode(doc, data, id, idSpline, typeobject), QGraphicsPathItem(parent){
    RefreshGeometry();
    this->setPen(QPen(baseColor, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

VNodeSplinePath *VNodeSplinePath::Create(VDomDocument *doc, VContainer *data, qint64 id,
                                                 qint64 idSpline, Draw::Mode typeobject, Document::Enum parse,
                                                 Tool::Enum typeCreation){
    VNodeSplinePath *splPath = 0;
    if(parse == Document::FullParse){
        splPath = new VNodeSplinePath(doc, data, id, idSpline, typeobject, typeCreation);
        doc->AddTool(id, splPath);
    } else {
        doc->UpdateToolData(id, data);
    }
    return splPath;
}

void VNodeSplinePath::FullUpdateFromFile(){
    RefreshGeometry();
}

void VNodeSplinePath::AddToFile(){
    QDomElement domElement = doc->createElement("spline");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "modelingPath");
    AddAttribute(domElement, "idObject", idNode);
    if(typeobject == Draw::Calculation){
        AddAttribute(domElement, "typeObject", "Calculation");
    } else {
        AddAttribute(domElement, "typeObject", "Modeling");
    }

    AddToModeling(domElement);
}

void VNodeSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::SplinePath);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VNodeSplinePath::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VNodeSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VNodeSplinePath::RefreshGeometry(){
    VSplinePath splPath = VAbstractTool::data.GetModelingSplinePath(id);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
