#include "vmodelingsplinepath.h"

VModelingSplinePath::VModelingSplinePath(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                         Draw::Mode typeobject, Tool::Enum typeCreation,
                                         QGraphicsItem * parent):VAbstractTool(doc, data, id, Draw::Modeling),
    QGraphicsPathItem(parent), idSpline(idSpline), typeobject(typeobject){
    VSplinePath splPath = data->GetModelingSplinePath(id);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

VModelingSplinePath *VModelingSplinePath::Create(VDomDocument *doc, VContainer *data, qint64 id,
                                                 qint64 idSpline, Draw::Mode typeobject, Document::Enum parse,
                                                 Tool::Enum typeCreation){
    VModelingSplinePath *splPath;
    if(parse == Document::FullParse){
        splPath = new VModelingSplinePath(doc, data, id, idSpline, typeobject, typeCreation);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id, splPath);
    } else {
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        VDataTool *tool = tools->value(id);
        if(tool != 0){
            tool->VDataTool::setData(data);
            tools->insert(id, tool);
        }
    }
    return splPath;
}

void VModelingSplinePath::FullUpdateFromFile(){
    RefreshGeometry();
}

void VModelingSplinePath::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
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

void VModelingSplinePath::FullUpdateFromGui(int result){
    Q_UNUSED(result);
}

void VModelingSplinePath::AddToFile(){
    QDomElement domElement = doc->createElement("spline");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "modelingPath");
    AddAttribute(domElement, "idObject", idSpline);
    if(typeobject == Draw::Calculation){
        AddAttribute(domElement, "typeObject", "Calculation");
    } else {
        AddAttribute(domElement, "typeObject", "Modeling");
    }

    AddToDraw(domElement);
}

void VModelingSplinePath::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::SplinePath);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VModelingSplinePath::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VModelingSplinePath::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VModelingSplinePath::RefreshGeometry(){
    VSplinePath splPath = VAbstractTool::data.GetModelingSplinePath(id);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
