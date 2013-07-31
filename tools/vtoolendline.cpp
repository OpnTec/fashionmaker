#include "vtoolendline.h"
#include <QDebug>
#include <QMenu>

#include "../widgets/vmaingraphicsscene.h"

VToolEndLine::VToolEndLine(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                           const QString &formula, const qint32 &angle, const qint64 &basePointId,
                           Tool::Enum typeCreation, QGraphicsItem *parent):
    VToolLinePoint(doc, data, id, typeLine, formula, basePointId, angle, parent){

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolEndLine::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        basePointId = domElement.attribute("basePoint", "").toLongLong();
        angle = domElement.attribute("angle", "").toInt();
    }
    RefreshGeometry();
}

void VToolEndLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogEndLine = QSharedPointer<DialogEndLine>(new DialogEndLine(VAbstractTool::data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogEndLine.data(), &DialogEndLine::ChoosedPoint);
            connect(dialogEndLine.data(), &DialogEndLine::DialogClosed, this,
                    &VToolEndLine::FullUpdateFromGui);
            connect(doc, &VDomDocument::FullUpdateFromFile, dialogEndLine.data(), &DialogEndLine::UpdateList);

            VPointF p = VAbstractTool::data->GetPoint(id);

            dialogEndLine->setTypeLine(typeLine);
            dialogEndLine->setFormula(formula);
            dialogEndLine->setAngle(angle);
            dialogEndLine->setBasePointId(basePointId);
            dialogEndLine->setPointName(p.name());

            dialogEndLine->show();
        }
    }
}

void VToolEndLine::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogEndLine->getPointName());
            domElement.setAttribute("typeLine", dialogEndLine->getTypeLine());
            domElement.setAttribute("length", dialogEndLine->getFormula());
            domElement.setAttribute("angle", QString().setNum(dialogEndLine->getAngle()));
            domElement.setAttribute("basePoint", QString().setNum(dialogEndLine->getBasePointId()));
            emit FullUpdateTree();
        }
    }
    dialogEndLine.clear();
}

void VToolEndLine::AddToFile(){
    VPointF point = VAbstractTool::data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "endLine");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "angle", angle);
    AddAttribute(domElement, "basePoint", basePointId);

    AddToCalculation(domElement);
}
