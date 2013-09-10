#include "vmodelingline.h"
#include <QMenu>
#include <QDebug>

VModelingLine::VModelingLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint,
                             qint64 secondPoint, Tool::Enum typeCreation, QGraphicsItem *parent):
    VModelingTool(doc, data, id), QGraphicsLineItem(parent), firstPoint(firstPoint),
    secondPoint(secondPoint), dialogLine(QSharedPointer<DialogLine>()){
    //Лінія
    VPointF first = data->GetModelingPoint(firstPoint);
    VPointF second = data->GetModelingPoint(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VModelingLine::setDialog(){
    dialogLine->setFirstPoint(firstPoint);
    dialogLine->setSecondPoint(secondPoint);
}

VModelingLine *VModelingLine::Create(QSharedPointer<DialogLine> &dialog, VDomDocument *doc,
                                     VContainer *data){
    qint64 firstPoint = dialog->getFirstPoint();
    qint64 secondPoint = dialog->getSecondPoint();
    return Create(0, firstPoint, secondPoint, doc, data, Document::FullParse, Tool::FromGui);
}

VModelingLine *VModelingLine::Create(const qint64 &id, const qint64 &firstPoint, const qint64 &secondPoint,
                                     VDomDocument *doc, VContainer *data, Document::Enum parse,
                                     Tool::Enum typeCreation){
    VModelingLine *line = 0;
    Q_CHECK_PTR(doc);
    Q_CHECK_PTR(data);
    data->AddLine(firstPoint, secondPoint, Draw::Modeling);
    data->IncrementReferens(firstPoint, Scene::Point, Draw::Modeling);
    data->IncrementReferens(secondPoint, Scene::Point, Draw::Modeling);
    if(parse != Document::FullParse){
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        Q_CHECK_PTR(tools);
        VDataTool *tool = tools->value(id);
        Q_CHECK_PTR(tool);
        tool->VDataTool::setData(data);
    }
    if(parse == Document::FullParse){
        qint64 id = data->getNextId();
        line = new VModelingLine(doc, data, id, firstPoint, secondPoint, typeCreation);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id,line);
    }
    return line;
}

void VModelingLine::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        firstPoint = domElement.attribute("firstPoint", "").toLongLong();
        secondPoint = domElement.attribute("secondPoint", "").toLongLong();
    }
    VPointF first = VAbstractTool::data.GetModelingPoint(firstPoint);
    VPointF second = VAbstractTool::data.GetModelingPoint(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
}

void VModelingLine::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("firstPoint", QString().setNum(dialogLine->getFirstPoint()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogLine->getSecondPoint()));
            emit FullUpdateTree();
        }
    }
    dialogLine.clear();
}

void VModelingLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogLine, this, event);
}

void VModelingLine::AddToFile(){
    QDomElement domElement = doc->createElement("line");
    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "firstPoint", firstPoint);
    AddAttribute(domElement, "secondPoint", secondPoint);

    AddToModeling(domElement);
}

void VModelingLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VModelingLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}
