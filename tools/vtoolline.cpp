#include "vtoolline.h"
#include <QMenu>
#include <QDebug>

VToolLine::VToolLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint, qint64 secondPoint,
                     Tool::Enum typeCreation, QGraphicsItem *parent):VAbstractTool(doc, data, id),
    QGraphicsLineItem(parent){
    this->firstPoint = firstPoint;
    this->secondPoint = secondPoint;

    //Лінія
    VPointF first = data->GetPoint(firstPoint);
    VPointF second = data->GetPoint(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolLine::setDialog(){
    dialogLine->setFirstPoint(firstPoint);
    dialogLine->setSecondPoint(secondPoint);
}

void VToolLine::Create(QSharedPointer<DialogLine> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                       VContainer *data){
    qint64 firstPoint = dialog->getFirstPoint();
    qint64 secondPoint = dialog->getSecondPoint();
    Create(0, firstPoint, secondPoint, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolLine::Create(const qint64 &id, const qint64 &firstPoint, const qint64 &secondPoint,
                       VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data, Document::Enum parse,
                       Tool::Enum typeCreation){
    data->AddLine(firstPoint, secondPoint);
    if(parse != Document::FullParse){
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        VDataTool *tool = tools->value(id);
        tool->VDataTool::setData(data);
        tools->insert(id, tool);
    }
    VAbstractTool::AddRecord(id, Tools::LineTool, doc);
    if(parse == Document::FullParse){
        qint64 id = data->getNextId();
        VToolLine *line = new VToolLine(doc, data, id, firstPoint, secondPoint, typeCreation);
        scene->addItem(line);
        connect(line, &VToolLine::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id,line);
    }
}

void VToolLine::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        firstPoint = domElement.attribute("firstPoint", "").toLongLong();
        secondPoint = domElement.attribute("secondPoint", "").toLongLong();
    }
    VPointF first = VAbstractTool::data.GetPoint(firstPoint);
    VPointF second = VAbstractTool::data.GetPoint(secondPoint);
    this->setLine(QLineF(first.toQPointF(), second.toQPointF()));
}

void VToolLine::FullUpdateFromGui(int result){
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

void VToolLine::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
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

void VToolLine::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        this->setPen(QPen(Qt::black, widthHairLine));
        this->setAcceptHoverEvents (true);
        VAbstractTool::ChangedActivDraw(newName);
    } else {
        this->setPen(QPen(Qt::gray, widthHairLine));
        this->setAcceptHoverEvents (false);
        VAbstractTool::ChangedActivDraw(newName);
    }
}

void VToolLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogLine, this, event);
}


void VToolLine::AddToFile(){
    QDomElement domElement = doc->createElement("line");
    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "firstPoint", firstPoint);
    AddAttribute(domElement, "secondPoint", secondPoint);

    AddToCalculation(domElement);
}

void VToolLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VToolLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

