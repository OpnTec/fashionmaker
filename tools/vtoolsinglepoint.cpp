#include "vtoolsinglepoint.h"
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QGraphicsItem>
#include <cmath>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

#include "../options.h"
#include "../container/vpointf.h"

VToolSinglePoint::VToolSinglePoint (VDomDocument *doc, VContainer *data, qint64 id, Tool::Enum typeCreation,
                                    QGraphicsItem * parent ):VToolPoint(doc, data, id, parent){
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolSinglePoint::AddToFile(){
    VPointF point = VAbstractTool::data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "single");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "x", point.x()/PrintDPI*25.4);
    AddAttribute(domElement, "y", point.y()/PrintDPI*25.4);
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddToCalculation(domElement);
}

void VToolSinglePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogSinglePoint = QSharedPointer<DialogSinglePoint>(new DialogSinglePoint(VAbstractTool::data));
            VPointF p = VAbstractTool::data->GetPoint(id);
            dialogSinglePoint->setData(p.name(), p.toQPointF());
            connect(dialogSinglePoint.data(), &DialogSinglePoint::DialogClosed, this,
                    &VToolSinglePoint::FullUpdateFromGui);
            dialogSinglePoint->exec();
        }
    }
}

void  VToolSinglePoint::FullUpdateFromFile(){
    RefreshGeometry();
}

void VToolSinglePoint::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QPointF p = dialogSinglePoint->getPoint();
        QString name = dialogSinglePoint->getName();
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", name);
            domElement.setAttribute("x", QString().setNum(p.x()/PrintDPI*25.4));
            domElement.setAttribute("y", QString().setNum(p.y()/PrintDPI*25.4));
            emit FullUpdateTree();
        }
    }
    dialogSinglePoint.clear();
}
