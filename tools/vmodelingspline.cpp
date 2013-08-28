#include "vmodelingspline.h"

VModelingSpline::VModelingSpline(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                 Draw::Mode typeobject, Tool::Enum typeCreation, QGraphicsItem * parent)
    :VAbstractTool(doc, data, id, Draw::Modeling), QGraphicsPathItem(parent), idSpline(idSpline),
      typeobject(typeobject){
    VSpline spl = data->GetModelingSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
    this->setPen(QPen(Qt::black, widthHairLine));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

VModelingSpline *VModelingSpline::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                         Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation){
    VModelingSpline *spl;
    if(parse == Document::FullParse){
        spl = new VModelingSpline(doc, data, id, idSpline, typeobject, typeCreation);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id, spl);
    } else {
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        VDataTool *tool = tools->value(id);
        if(tool != 0){
            tool->VDataTool::setData(data);
            tools->insert(id, tool);
        }
    }
    return spl;
}

void VModelingSpline::FullUpdateFromFile(){
    RefreshGeometry();
}

void VModelingSpline::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
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

void VModelingSpline::FullUpdateFromGui(int result)
{
}

void VModelingSpline::AddToFile(){
    QDomElement domElement = doc->createElement("spline");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "modelingSpline");
    AddAttribute(domElement, "idObject", idSpline);
    if(typeobject == Draw::Calculation){
        AddAttribute(domElement, "typeObject", "Calculation");
    } else {
        AddAttribute(domElement, "typeObject", "Modeling");
    }

    AddToDraw(domElement);
}

void VModelingSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Spline);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VModelingSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VModelingSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VModelingSpline::RefreshGeometry(){
    VSpline spl = VAbstractTool::data.GetModelingSpline(id);
    QPainterPath path;
    path.addPath(spl.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
