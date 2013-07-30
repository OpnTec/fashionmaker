#include "vtoolnormal.h"
#include <QMenu>

VToolNormal::VToolNormal(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                         const QString &formula, const qint32 &angle, const qint64 &firstPointId,
                         const qint64 &secondPointId, Tool::Enum typeCreation, QGraphicsItem *parent):
    VToolPoint(doc, data, id, parent){
    this->typeLine = typeLine;
    this->formula = formula;
    this->angle = angle;
    this->firstPointId = firstPointId;
    this->secondPointId = secondPointId;

    //Лінія, що з'єднує дві точки
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF point = data->GetPoint(id);
    mainLine = new QGraphicsLineItem(QLineF(firstPoint.toQPointF(), point.toQPointF()), this);
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

QPointF VToolNormal::FindPoint(const QPointF &firstPoint, const QPointF &secondPoint, const qreal &length,
                               const qint32 &angle){
    QLineF line(firstPoint, secondPoint);
    QLineF normal = line.normalVector();
    normal.setAngle(normal.angle()+angle);
    normal.setLength(length);
    return normal.p2();
}

void VToolNormal::FullUpdateFromFile(){
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
        angle = domElement.attribute("angle", "").toInt();
    }
    VPointF point = VAbstractTool::data->GetPoint(id);
    RefreshBaseGeometry(name, point.x(), point.y(), mx, my);
    VPointF firstPoint = VAbstractTool::data->GetPoint(firstPointId);
    mainLine->setLine(QLineF(firstPoint.toQPointF(), point.toQPointF()));
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}

void VToolNormal::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogNormal->getPointName());
            domElement.setAttribute("typeLine", dialogNormal->getTypeLine());
            domElement.setAttribute("length", dialogNormal->getFormula());
            domElement.setAttribute("angle", QString().setNum(dialogNormal->getAngle()));
            domElement.setAttribute("firstPoint", QString().setNum(dialogNormal->getFirstPointId()));
            domElement.setAttribute("secondPoint", QString().setNum(dialogNormal->getSecondPointId()));
            emit FullUpdateTree();
        }
    }
    dialogNormal.clear();
}

void VToolNormal::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        mainLine->setPen(QPen(Qt::black, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    } else {
        mainLine->setPen(QPen(Qt::gray, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    }
}

void VToolNormal::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogNormal = QSharedPointer<DialogNormal>(new DialogNormal(VAbstractTool::data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogNormal.data(), &DialogNormal::ChoosedPoint);
            connect(dialogNormal.data(), &DialogNormal::DialogClosed, this,
                    &VToolNormal::FullUpdateFromGui);
            connect(doc, &VDomDocument::FullUpdateFromFile, dialogNormal.data(), &DialogNormal::UpdateList);

            VPointF p = VAbstractTool::data->GetPoint(id);

            dialogNormal->setTypeLine(typeLine);
            dialogNormal->setFormula(formula);
            dialogNormal->setAngle(angle);
            dialogNormal->setFirstPointId(firstPointId);
            dialogNormal->setSecondPointId(secondPointId);
            dialogNormal->setPointName(p.name());

            dialogNormal->show();
        }
    }
}

void VToolNormal::AddToFile(){
    VPointF point = VAbstractTool::data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "normal");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "angle", angle);
    AddAttribute(domElement, "firstPoint", firstPointId);
    AddAttribute(domElement, "secondPoint", firstPointId);

    AddToCalculation(domElement);
}
