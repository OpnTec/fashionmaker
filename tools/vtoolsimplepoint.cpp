#include "vtoolsimplepoint.h"
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QGraphicsItem>
#include <cmath>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

#include "../options.h"
#include "../container/vpointf.h"
#include "../dialogs/dialogsinglepoint.h"

VToolSimplePoint::VToolSimplePoint (VDomDocument *doc, VContainer *data, qint64 id, Tool::Enum typeCreation,
                                    QGraphicsItem * parent ):VToolPoint(doc, data, id, parent){
    ignoreContextMenuEvent = false;//don't ignore context menu events;
    connect(this, &VToolSimplePoint::FullUpdateTree, this->doc, &VDomDocument::FullUpdateTree);
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolSimplePoint::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        VToolPoint::ChangedActivDraw(newName);
        ignoreContextMenuEvent = false;
    } else {
        VToolPoint::ChangedActivDraw(newName);
        ignoreContextMenuEvent = true;
    }
}

void VToolSimplePoint::AddToFile(){
    VPointF point = data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    QDomAttr domAttr = doc->createAttribute("id");
    domAttr.setValue(QString().setNum(id));
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("type");
    domAttr.setValue("simple");
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("name");
    domAttr.setValue(point.name());
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("x");
    domAttr.setValue(QString().setNum(point.x()/PrintDPI*25.4));
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("y");
    domAttr.setValue(QString().setNum(point.y()/PrintDPI*25.4));
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("mx");
    domAttr.setValue(QString().setNum(point.mx()/PrintDPI*25.4));
    domElement.setAttributeNode(domAttr);

    domAttr = doc->createAttribute("my");
    domAttr.setValue(QString().setNum(point.my()/PrintDPI*25.4));
    domElement.setAttributeNode(domAttr);

    QDomElement calcElement;
    bool ok = doc->GetActivCalculationElement(calcElement);
    if(ok){
        calcElement.appendChild(domElement);
    } else {
        qCritical("Не можу знайти тег калькуляції. VToolSimplePoint::AddSimplePoint");
    }
}

void VToolSimplePoint::FullUpdateFromGui(const QString &name, qreal x, qreal y){
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        domElement.setAttribute("name", name);
        domElement.setAttribute("x", QString().setNum(x/PrintDPI*25.4));
        domElement.setAttribute("y", QString().setNum(y/PrintDPI*25.4));
        emit FullUpdateTree();
    }
}

void VToolSimplePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ){
    if(!ignoreContextMenuEvent){
        QMenu menu;
        QAction *actionOption = menu.addAction("Властивості");
        QAction *selectedAction = menu.exec(event->screenPos());
        if(selectedAction == actionOption){
            DialogSinglePoint *dialogSinglePoint = new DialogSinglePoint;
            VPointF p = data->GetPoint(id);
            dialogSinglePoint->setData(p.name(), p.toQPointF());
            qint32 result = dialogSinglePoint->exec();
            if(result == QDialog::Accepted){
                QPointF p = dialogSinglePoint->getPoint();
                FullUpdateFromGui(dialogSinglePoint->getName(), p.x(), p.y());
            }
            delete dialogSinglePoint;
        }
    }
}

void  VToolSimplePoint::FullUpdateFromFile(){
    QString name;
    qreal x, y, mx, my;
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if(domElement.isElement()){
        name = domElement.attribute("name", "");
        x = domElement.attribute("x", "").toDouble()*PrintDPI/25.4;
        y = domElement.attribute("y", "").toDouble()*PrintDPI/25.4;
        mx = domElement.attribute("mx", "").toDouble()*PrintDPI/25.4;
        my = domElement.attribute("my", "").toDouble()*PrintDPI/25.4;
    }
    QRectF rec = QRectF(x, y, radius*2, radius*2);
    rec.translate(x-rec.center().x(), y-rec.center().y());
    this->setRect(rec);

    rec = this->rect();
    namePoint->setText(name);
    namePoint->setPos(QPointF(rec.center().x()+mx, rec.center().y()+my));

    RefreshLine();
}
