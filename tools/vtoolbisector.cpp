#include "vtoolbisector.h"
#include <QMenu>

VToolBisector::VToolBisector(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                             const QString &formula, const qint64 &firstPointId, const qint64 &secondPointId,
                             const qint64 &thirdPointId, Tool::Enum typeCreation, QGraphicsItem *parent):
    VToolLinePoint(doc, data, id, typeLine, formula, secondPointId, 0, parent){
    this->firstPointId = firstPointId;
    this->thirdPointId = thirdPointId;

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

QPointF VToolBisector::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                 const QPointF &thirdPoint, const qreal &length){
    QLineF line1(secondPoint, firstPoint);
    QLineF line2(secondPoint, thirdPoint);
    qreal angle = line1.angleTo(line2);
    if(angle>180){
        angle = 360 - angle;
    }
    line1.setAngle(line1.angle()-angle/2);
    line1.setLength(length);
    return line1.p2();
}

void VToolBisector::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        firstPointId = domElement.attribute("firstPoint", "").toLongLong();
        basePointId = domElement.attribute("secondPoint", "").toLongLong();
        thirdPointId = domElement.attribute("thirdPoint", "").toLongLong();
    }
    RefreshGeometry();
}

void VToolBisector::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogBisector->getPointName());
            domElement.setAttribute("typeLine", dialogBisector->getTypeLine());
            domElement.setAttribute("length", dialogBisector->getFormula());
            domElement.setAttribute("firstPoint", QString().setNum(dialogBisector->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogBisector->getSecondPointId()));
            domElement.setAttribute("thirdPoint", QString().setNum(dialogBisector->getThirdPointId()));
            emit FullUpdateTree();
        }
    }
    dialogBisector.clear();
}

void VToolBisector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogBisector = QSharedPointer<DialogBisector>(new DialogBisector(VAbstractTool::data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogBisector.data(), &DialogBisector::ChoosedPoint);
            connect(dialogBisector.data(), &DialogBisector::DialogClosed, this,
                    &VToolBisector::FullUpdateFromGui);
            connect(doc, &VDomDocument::FullUpdateFromFile, dialogBisector.data(),
                    &DialogBisector::UpdateList);

            VPointF p = VAbstractTool::data->GetPoint(id);

            dialogBisector->setTypeLine(typeLine);
            dialogBisector->setFormula(formula);
            dialogBisector->setFirstPointId(firstPointId);
            dialogBisector->setSecondPointId(basePointId);
            dialogBisector->setThirdPointId(thirdPointId);
            dialogBisector->setPointName(p.name());

            dialogBisector->show();
        }
    }
}

void VToolBisector::AddToFile(){
    VPointF point = VAbstractTool::data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "bisector");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "firstPoint", firstPointId);
    AddAttribute(domElement, "secondPoint", basePointId);
    AddAttribute(domElement, "thirdPoint", thirdPointId);

    AddToCalculation(domElement);
}
