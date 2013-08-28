#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "vtoolarc.h"
#include <QMenu>
#pragma GCC diagnostic pop
#include "../container/calculator.h"

VToolArc::VToolArc(VDomDocument *doc, VContainer *data, qint64 id, Draw::Mode mode, Tool::Enum typeCreation,
                   QGraphicsItem *parent):VAbstractTool(doc, data, id, mode), QGraphicsPathItem(parent),
    dialogArc(QSharedPointer<DialogArc>()){
    VArc arc = data->GetArc(id);
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

void VToolArc::setDialog(){
    Q_ASSERT(!dialogArc.isNull());
    if(!dialogArc.isNull()){
        VArc arc = VAbstractTool::data.GetArc(id);
        dialogArc->SetCenter(arc.GetCenter());
        dialogArc->SetRadius(arc.GetFormulaRadius());
        dialogArc->SetF1(arc.GetFormulaF1());
        dialogArc->SetF2(arc.GetFormulaF2());
    }
}

void VToolArc::Create(QSharedPointer<DialogArc> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                      VContainer *data, Draw::Mode mode){
    qint64 center = dialog->GetCenter();
    QString radius = dialog->GetRadius();
    QString f1 = dialog->GetF1();
    QString f2 = dialog->GetF2();
    Create(0, center, radius, f1, f2, scene, doc, data, Document::FullParse, Tool::FromGui, mode);
}

void VToolArc::Create(const qint64 _id, const qint64 &center, const QString &radius, const QString &f1,
                      const QString &f2, VMainGraphicsScene *scene, VDomDocument *doc,
                      VContainer *data, Document::Enum parse, Tool::Enum typeCreation, Draw::Mode mode){
    qreal calcRadius = 0, calcF1 = 0, calcF2 = 0;

    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(radius, &errorMsg);
    if(errorMsg.isEmpty()){
        calcRadius = result*PrintDPI/25.4;
    }

    errorMsg.clear();
    result = cal.eval(f1, &errorMsg);
    if(errorMsg.isEmpty()){
        calcF1 = result;
    }

    errorMsg.clear();
    result = cal.eval(f2, &errorMsg);
    if(errorMsg.isEmpty()){
        calcF2 = result;
    }

    VArc arc = VArc(data->DataPoints(), center, calcRadius, radius, calcF1, f1, calcF2, f2 );
    qint64 id = _id;
    if(typeCreation == Tool::FromGui){
        id = data->AddArc(arc);
    } else {
        data->UpdateArc(id, arc);
        if(parse != Document::FullParse){
            QMap<qint64, VDataTool*>* tools = doc->getTools();
            VDataTool *tool = tools->value(id);
            if(tool != 0){
                tool->VDataTool::setData(data);
                tools->insert(id, tool);
                data->IncrementReferens(id, Scene::Arc);
            }
        }
    }
    data->AddLengthArc(data->GetNameArc(center,id), arc.GetLength());
    VAbstractTool::AddRecord(id, Tools::ArcTool, doc);
    if(mode == Draw::Modeling){
        data->IncrementReferens(center, Scene::Point);
    }
    if(parse == Document::FullParse){
        VToolArc *toolArc = new VToolArc(doc, data, id, mode, typeCreation);
        scene->addItem(toolArc);
        connect(toolArc, &VToolArc::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(toolArc, &VToolArc::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
        QMap<qint64, VDataTool*>* tools = doc->getTools();
        tools->insert(id,toolArc);
    }
}

void VToolArc::FullUpdateFromFile(){
    RefreshGeometry();
}

void VToolArc::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("center", QString().setNum(dialogArc->GetCenter()));
            domElement.setAttribute("radius", dialogArc->GetRadius());
            domElement.setAttribute("angle1", dialogArc->GetF1());
            domElement.setAttribute("angle2", dialogArc->GetF2());
            emit FullUpdateTree();
        }
    }
    dialogArc.clear();
}

void VToolArc::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        this->setPen(QPen(Qt::black, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->setAcceptHoverEvents(true);
        VAbstractTool::ChangedActivDraw(newName);
    } else {
        this->setPen(QPen(Qt::gray, widthHairLine));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->setAcceptHoverEvents (false);
        VAbstractTool::ChangedActivDraw(newName);
    }
}

void VToolArc::ShowTool(qint64 id, Qt::GlobalColor color, bool enable){
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

void VToolArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ContextMenu(dialogArc, this, event);
}

void VToolArc::AddToFile(){
    VArc arc;
    if(mode == Draw::Calculation){
        arc = VAbstractTool::data.GetArc(id);
    } else {
        arc = VAbstractTool::data.GetModelingArc(id);
    }
    QDomElement domElement = doc->createElement("arc");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "simple");
    AddAttribute(domElement, "center", arc.GetCenter());
    AddAttribute(domElement, "radius", arc.GetFormulaRadius());
    AddAttribute(domElement, "angle1", arc.GetFormulaF1());
    AddAttribute(domElement, "angle2", arc.GetFormulaF2());

    AddToDraw(domElement);
}

void VToolArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Arc);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthMainLine));
}

void VToolArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, widthHairLine));
}

void VToolArc::RefreshGeometry(){
    VArc arc = VAbstractTool::data.GetArc(id);
    QPainterPath path;
    path.addPath(arc.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
