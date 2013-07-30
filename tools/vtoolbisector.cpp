#include "vtoolbisector.h"
#include <QMenu>

VToolBisector::VToolBisector(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                             const QString &formula, const qint64 &firstPointId, const qint64 &secondPointId,
                             const qint64 &thirdPointId, Tool::Enum typeCreation, QGraphicsItem *parent):
    VToolPoint(doc, data, id, parent){
    this->typeLine = typeLine;
    this->formula = formula;
    this->firstPointId = firstPointId;
    this->secondPointId = secondPointId;
    this->thirdPointId = thirdPointId;

    //Лінія, що з'єднує дві точки
    VPointF basePoint = data->GetPoint(secondPointId);
    VPointF point = data->GetPoint(id);
    mainLine = new QGraphicsLineItem(QLineF(basePoint.toQPointF(), point.toQPointF()), this);
    mainLine->setPen(QPen(Qt::black, widthHairLine));
    mainLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }

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
    QString name;
    qreal mx, my;
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        name = domElement.attribute("name", "");
        mx = domElement.attribute("mx", "").toDouble()*PrintDPI/25.4;
        my = domElement.attribute("my", "").toDouble()*PrintDPI/25.4;
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        firstPointId = domElement.attribute("firstPoint", "").toLongLong();
        secondPointId = domElement.attribute("secondPoint", "").toLongLong();
        thirdPointId = domElement.attribute("thirdPoint", "").toLongLong();
    }
    VPointF point = VAbstractTool::data->GetPoint(id);
    RefreshGeometry(name, point.x(), point.y(), mx, my);
    VPointF basePoint = VAbstractTool::data->GetPoint(secondPointId);
    mainLine->setLine(QLineF(basePoint.toQPointF(), point.toQPointF()));
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
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

void VToolBisector::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        mainLine->setPen(QPen(Qt::black, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    } else {
        mainLine->setPen(QPen(Qt::gray, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    }
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
            dialogBisector->setSecondPointId(secondPointId);
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
    AddAttribute(domElement, "secondPoint", secondPointId);
    AddAttribute(domElement, "thirdPoint", thirdPointId);

    AddToCalculation(domElement);
}
