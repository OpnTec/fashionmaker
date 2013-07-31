#include "vtoollineintersect.h"
#include <QMenu>

VToolLineIntersect::VToolLineIntersect(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const qint64 &p1Line1, const qint64 &p2Line1, const qint64 &p1Line2,
                                       const qint64 &p2Line2, Tool::Enum typeCreation, QGraphicsItem *parent):
    VToolPoint(doc, data, id, parent){
    this->p1Line1 = p1Line1;
    this->p2Line1 = p2Line1;
    this->p1Line2 = p1Line2;
    this->p2Line2 = p2Line2;
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolLineIntersect::FullUpdateFromFile(){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        p1Line1 = domElement.attribute("p1Line1", "").toLongLong();
        p2Line1 = domElement.attribute("p2Line1", "").toLongLong();
        p1Line2 = domElement.attribute("p1Line2", "").toLongLong();
        p2Line2 = domElement.attribute("p2Line2", "").toLongLong();
    }
    RefreshGeometry();
}

void VToolLineIntersect::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogLineIntersect->getPointName());
            domElement.setAttribute("p1Line1", QString().setNum(dialogLineIntersect->getP1Line1()));
            domElement.setAttribute("p2Line1", QString().setNum(dialogLineIntersect->getP2Line1()));
            domElement.setAttribute("p1Line2", QString().setNum(dialogLineIntersect->getP1Line2()));
            domElement.setAttribute("p2Line2", QString().setNum(dialogLineIntersect->getP2Line2()));
            emit FullUpdateTree();
        }
    }
    dialogLineIntersect.clear();
}

void VToolLineIntersect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogLineIntersect = QSharedPointer<DialogLineIntersect>(new DialogLineIntersect(VAbstractTool::data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogLineIntersect.data(), &DialogLineIntersect::ChoosedPoint);
            connect(dialogLineIntersect.data(), &DialogLineIntersect::DialogClosed, this,
                    &VToolLineIntersect::FullUpdateFromGui);

            VPointF p = VAbstractTool::data->GetPoint(id);

            dialogLineIntersect->setP1Line1(p1Line1);
            dialogLineIntersect->setP2Line1(p2Line1);
            dialogLineIntersect->setP1Line2(p1Line2);
            dialogLineIntersect->setP2Line2(p2Line2);
            dialogLineIntersect->setPointName(p.name());

            dialogLineIntersect->show();
        }
    }
}

void VToolLineIntersect::AddToFile(){
    VPointF point = VAbstractTool::data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "lineIntersect");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "p1Line1", p1Line1);
    AddAttribute(domElement, "p2Line1", p2Line1);
    AddAttribute(domElement, "p1Line2", p1Line2);
    AddAttribute(domElement, "p2Line2", p2Line2);

    AddToCalculation(domElement);
}
