#include "vmodelingpoint.h"

VModelingPoint::VModelingPoint(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint,
                               Draw::Mode typeobject, Tool::Enum typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, Draw::Modeling, parent), idPoint(idPoint), typeobject(typeobject){
    disconnect(this->doc, &VDomDocument::ChangedActivDraw, this, &VModelingPoint::ChangedActivDraw);
    disconnect(this->doc, &VDomDocument::ChangedNameDraw, this, &VModelingPoint::ChangedNameDraw);
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

VModelingPoint *VModelingPoint::Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint,
                                       Draw::Mode typeobject, Document::Enum parse, Tool::Enum typeCreation){
    VModelingPoint *point;
    if(parse == Document::FullParse){
        point = new VModelingPoint(doc, data, id, idPoint, typeobject, typeCreation);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id, point);
    } else {
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        VDataTool *tool = tools->value(id);
        if(tool != 0){
            tool->VDataTool::setData(data);
            tools->insert(id, tool);
        }
    }
    return point;
}

void VModelingPoint::FullUpdateFromFile(){
    RefreshGeometry();
}

void VModelingPoint::FullUpdateFromGui(int result)
{
}

void VModelingPoint::AddToFile(){
    VPointF point = VAbstractTool::data.GetModelingPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "modeling");
    AddAttribute(domElement, "idObject", idPoint);
    if(typeobject == Draw::Calculation){
        AddAttribute(domElement, "typeObject", "Calculation");
    } else {
        AddAttribute(domElement, "typeObject", "Modeling");
    }
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddToDraw(domElement);
}
