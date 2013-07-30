#include "vtoolalongline.h"
#include "../container/calculator.h"
#include <QDialog>
#include <QMenu>
#include <QDebug>

VToolAlongLine::VToolAlongLine(VDomDocument *doc, VContainer *data, qint64 id, const QString &formula,
                               const qint64 &firstPointId, const qint64 &secondPointId,
                               const QString &typeLine, Tool::Enum typeCreation,
                               QGraphicsItem *parent):VToolPoint(doc, data, id, parent){
    this->typeLine = typeLine;
    this->formula = formula;
    this->firstPointId = firstPointId;
    this->secondPointId = secondPointId;

    //Лінія, якщо потрібно.
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);
    QLineF line = QLineF(firstPoint.toQPointF(), secondPoint.toQPointF());
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if(errorMsg.isEmpty()){
        line.setLength(result*PrintDPI/25.4);
        mainLine = new QGraphicsLineItem(line, this);
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
}

void VToolAlongLine::FullUpdateFromFile(){
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
    }
    VPointF point = VAbstractTool::data->GetPoint(id);
    RefreshGeometry(name, point.x(), point.y(), mx, my);
    VPointF firstPoint = VAbstractTool::data->GetPoint(firstPointId);
    VPointF secondPoint = VAbstractTool::data->GetPoint(secondPointId);
    mainLine->setLine(QLineF(firstPoint.toQPointF(), secondPoint.toQPointF()));
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}

void VToolAlongLine::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        mainLine->setPen(QPen(Qt::black, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    } else {
        mainLine->setPen(QPen(Qt::gray, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    }
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
                    dialogAlongLine.data(), &DialogAlongLine::ChoosedPoint);
            connect(dialogAlongLine.data(), &DialogAlongLine::DialogClosed, this,
                    &VToolAlongLine::FullUpdateFromGui);
            connect(doc, &VDomDocument::FullUpdateFromFile, dialogAlongLine.data(),
                    &DialogAlongLine::UpdateList);

            VPointF p = VAbstractTool::data->GetPoint(id);

            dialogAlongLine->setTypeLine(typeLine);
            dialogAlongLine->setFormula(formula);
            dialogAlongLine->setFirstPointId(firstPointId);
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
    AddAttribute(domElement, "firstPoint", firstPointId);
    AddAttribute(domElement, "secondPoint", secondPointId);

    AddToCalculation(domElement);
}
