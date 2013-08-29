#include "vmodelingarc.h"

VModelingArc::VModelingArc(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc, Draw::Mode typeobject,
                           Tool::Enum typeCreation, QGraphicsItem * parent):VAbstractTool(doc, data, id, Draw::Modeling),
    QGraphicsPathItem(parent), idArc(idArc), typeobject(typeobject){
    VArc arc = data->GetModelingArc(id);
    QPainterPath path;
    path.addPath(arc.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

VModelingArc *VModelingArc::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc,
                                   Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation){
    VModelingArc *arc;
    if(parse == Document::FullParse){
        arc = new VModelingArc(doc, data, id, idArc, typeobject, typeCreation);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id, arc);
    } else {
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        VDataTool *tool = tools->value(id);
        if(tool != 0){
            tool->VDataTool::setData(data);
            tools->insert(id, tool);
        }
    }
    return arc;
}

void VModelingArc::FullUpdateFromFile(){
    RefreshGeometry();
}

void VModelingArc::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
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

void VModelingArc::FullUpdateFromGui(int result){
    Q_UNUSED(result);
}

void VModelingArc::AddToFile(){
    QDomElement domElement = doc->createElement("arc");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "modeling");
    AddAttribute(domElement, "idObject", idArc);
    if(typeobject == Draw::Calculation){
        AddAttribute(domElement, "typeObject", "Calculation");
    } else {
        AddAttribute(domElement, "typeObject", "Modeling");
    }

    AddToDraw(domElement);
}

void VModelingArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Arc);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VModelingArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VModelingArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VModelingArc::RefreshGeometry(){
    VArc arc = VAbstractTool::data.GetModelingArc(id);
    QPainterPath path;
    path.addPath(arc.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
