#include "vtoolshoulderpoint.h"
#include <QDebug>
#include <QMenu>

VToolShoulderPoint::VToolShoulderPoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &typeLine, const QString &formula, const qint64 &p1Line,
                                       const qint64 &p2Line, const qint64 &pShoulder, Tool::Enum typeCreation,
                                       QGraphicsItem * parent):VToolPoint(doc, data, id, parent){
    this->typeLine = typeLine;
    this->formula = formula;
    this->p1Line = p1Line;
    this->p2Line = p2Line;
    this->pShoulder = pShoulder;

    //Лінія, що з'єднує дві точки
    VPointF p1L = data->GetPoint(p1Line);
    VPointF point = data->GetPoint(id);
    mainLine = new QGraphicsLineItem(QLineF(p1L.toQPointF(), point.toQPointF()), this);
    mainLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }

    AddLine(p1Line, id);
    AddLine(p2Line, id);

    if(typeCreation == Tool::FromGui){
        AddToFile();
    }

}

QPointF VToolShoulderPoint::FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                                      const qreal &length){
    QLineF line = QLineF(p1Line, p2Line);
        qreal dist = line.length();
        if(dist>length){
            qDebug()<<"A3П2="<<length/PrintDPI*25.4<<"А30П ="<<dist/PrintDPI*25.4;
            throw"Не можу знайти точку плеча. Довжина А3П2 < А3П.";
        }
        if(dist==length){
            return line.p2();
        }
        qreal step = 0.01;
        while(1){
            line.setLength(line.length()+step);
            QLineF line2 = QLineF(pShoulder, line.p2());
            if(line2.length()>=length){
                return line.p2();
            }
        }
}

void VToolShoulderPoint::FullUpdateFromFile(){
    QString name;
    qreal mx, my;
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        name = domElement.attribute("name", "");
        mx = domElement.attribute("mx", "").toDouble()*PrintDPI/25.4;
        my = domElement.attribute("my", "").toDouble()*PrintDPI/25.4;
        typeLine = domElement.attribute("typeLine", "");
        formula = domElement.attribute("length", "");
        p1Line = domElement.attribute("p1Line", "").toLongLong();
        p2Line = domElement.attribute("p2Line", "").toLongLong();
        pShoulder = domElement.attribute("pShoulder", "").toLongLong();
    }
    VPointF point = VAbstractTool::data->GetPoint(id);
    RefreshBaseGeometry(name, point.x(), point.y(), mx, my);
    VPointF p1L = VAbstractTool::data->GetPoint(p1Line);
    mainLine->setLine(QLineF(p1L.toQPointF(), point.toQPointF()));
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }

    AddLine(p1Line, id);
    AddLine(p2Line, id);
}

void VToolShoulderPoint::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", dialogShoulderPoint->getPointName());
            domElement.setAttribute("typeLine", dialogShoulderPoint->getTypeLine());
            domElement.setAttribute("length", dialogShoulderPoint->getFormula());
            domElement.setAttribute("p1Line", QString().setNum(dialogShoulderPoint->getP1Line()));
            domElement.setAttribute("p2Line", QString().setNum(dialogShoulderPoint->getP2Line()));
            domElement.setAttribute("pShoulder", QString().setNum(dialogShoulderPoint->getPShoulder()));
            emit FullUpdateTree();
        }
    }
    dialogShoulderPoint.clear();
}

void VToolShoulderPoint::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        mainLine->setPen(QPen(Qt::black, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    } else {
        mainLine->setPen(QPen(Qt::gray, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    }
}

void VToolShoulderPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            dialogShoulderPoint =
                    QSharedPointer<DialogShoulderPoint>(new DialogShoulderPoint(VAbstractTool::data));

            connect(qobject_cast< VMainGraphicsScene * >(this->scene()), &VMainGraphicsScene::ChoosedObject,
                    dialogShoulderPoint.data(), &DialogShoulderPoint::ChoosedPoint);
            connect(dialogShoulderPoint.data(), &DialogShoulderPoint::DialogClosed, this,
                    &VToolShoulderPoint::FullUpdateFromGui);
            connect(doc, &VDomDocument::FullUpdateFromFile, dialogShoulderPoint.data(), &DialogShoulderPoint::UpdateList);

            VPointF p = VAbstractTool::data->GetPoint(id);

            dialogShoulderPoint->setTypeLine(typeLine);
            dialogShoulderPoint->setFormula(formula);
            dialogShoulderPoint->setP1Line(p1Line);
            dialogShoulderPoint->setP2Line(p2Line);
            dialogShoulderPoint->setPShoulder(pShoulder);
            dialogShoulderPoint->setPointName(p.name());

            dialogShoulderPoint->show();
        }
    }
}

void VToolShoulderPoint::AddToFile(){
    VPointF point = VAbstractTool::data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "shoulder");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddAttribute(domElement, "typeLine", typeLine);
    AddAttribute(domElement, "length", formula);
    AddAttribute(domElement, "p1Line", p1Line);
    AddAttribute(domElement, "p2Line", p2Line);
    AddAttribute(domElement, "pShoulder", pShoulder);

    AddToCalculation(domElement);
}
