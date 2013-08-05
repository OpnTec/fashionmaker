#include "vtoolalongline.h"
#include "../container/calculator.h"
#include <QDialog>
#include <QMenu>
#include <QDebug>

VToolAlongLine::VToolAlongLine(VDomDocument *doc, VContainer *data, qint64 id, const QString &formula,
                               const qint64 &firstPointId, const qint64 &secondPointId,
                               const QString &typeLine, Tool::Enum typeCreation,
                               QGraphicsItem *parent):
    VToolLinePoint(doc, data, id, typeLine, formula, firstPointId, 0, parent){
    this->secondPointId = secondPointId;

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolAlongLine::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
    }
    RefreshGeometry();
}

void VToolAlongLine::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogAlongLine->getPointName());
            domElement.setAttribute("typeLine", dialogAlongLine->getTypeLine());
            domElement.setAttribute("length", dialogAlongLine->getFormula());
            domElement.setAttribute("firstPoint", QString().setNum(dialogAlongLine->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogAlongLine->getSecondPointId()));
            emit FullUpdateTree();
        }

    }
    dialogAlongLine.clear();
}

void VToolAlongLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogAlongLine = QSharedPointer<DialogAlongLine>(new DialogAlongLine(VAbstractTool::data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogAlongLine.data(), &DialogAlongLine::ChoosedObject);
            connect(dialogAlongLine.data(), &DialogAlongLine::DialogClosed, this,
                    &VToolAlongLine::FullUpdateFromGui);
            connect(doc, &VDomDocument::FullUpdateFromFile, dialogAlongLine.data(),
                    &DialogAlongLine::UpdateList);

            VPointF p = VAbstractTool::data->GetPoint(id);

            dialogAlongLine->setTypeLine(typeLine);
            dialogAlongLine->setFormula(formula);
            dialogAlongLine->setFirstPointId(basePointId);
            dialogAlongLine->setSecondPointId(secondPointId);
            dialogAlongLine->setPointName(p.name());

            dialogAlongLine->show();
        }
    }
}

void VToolAlongLine::AddToFile(){
    VPointF point = VAbstractTool::data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "alongLine");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "firstPoint", basePointId);
    AddAttribute(domElement, "secondPoint", secondPointId);

    AddToCalculation(domElement);
}
