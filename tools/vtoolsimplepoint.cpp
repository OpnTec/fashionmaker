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

void VToolSimplePoint::AddToFile(){
    VPointF point = VAbstractTool::data->GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "simple");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "x", point.x()/PrintDPI*25.4);
    AddAttribute(domElement, "y", point.y()/PrintDPI*25.4);
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

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
            VPointF p = VAbstractTool::data->GetPoint(id);
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
    RefreshBaseGeometry(name, x, y, mx, my);
}
