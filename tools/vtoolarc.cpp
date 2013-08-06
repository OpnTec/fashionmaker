#include "vtoolarc.h"
#include <QMenu>

VToolArc::VToolArc(VDomDocument *doc, VContainer *data, qint64 id, Tool::Enum typeCreation,
                   QGraphicsItem *parent):VAbstractTool(doc, data, id), QGraphicsPathItem(parent){
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

void VToolArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogArc = QSharedPointer<DialogArc>(new DialogArc(VAbstractTool::data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogArc.data(), &DialogArc::ChoosedObject);
            connect(dialogArc.data(), &DialogArc::DialogClosed, this,
                    &VToolArc::FullUpdateFromGui);

            VArc arc = VAbstractTool::data->GetArc(id);

            dialogArc->GetCenter(arc.GetCenter());
            dialogArc->GetRadius(arc.GetFormulaRadius());
            dialogArc->GetF1(arc.GetFormulaF1());
            dialogArc->GetF2(arc.GetFormulaF2());

            dialogArc->show();
        }
    }
}

void VToolArc::AddToFile(){
    VArc arc = VAbstractTool::data->GetArc(id);
    QDomElement domElement = doc->createElement("arc");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "simple");
    AddAttribute(domElement, "center", arc.GetCenter());
    AddAttribute(domElement, "radius", arc.GetFormulaRadius());
    AddAttribute(domElement, "angle1", arc.GetFormulaF1());
    AddAttribute(domElement, "angle2", arc.GetFormulaF2());

    AddToCalculation(domElement);
}

void VToolArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit ChoosedTool(id, Scene::Arc);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void VToolArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthMainLine));
}

void VToolArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, widthHairLine));
}

void VToolArc::RefreshGeometry(){
    VArc arc = VAbstractTool::data->GetArc(id);
    QPainterPath path;
    path.addPath(arc.GetPath());
    path.setFillRule( Qt::WindingFill );
    this->setPath(path);
}
