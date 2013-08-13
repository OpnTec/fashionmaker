#include "vtoolsinglepoint.h"
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QGraphicsItem>
#include <cmath>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

#include "../options.h"
#include "../container/vpointf.h"

VToolSinglePoint::VToolSinglePoint (VDomDocument *doc, VContainer *data, qint64 id, Tool::Enum typeCreation,
                                    QGraphicsItem * parent ):VToolPoint(doc, data, id, parent){
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VToolSinglePoint::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "single");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "x", point.x()/PrintDPI*25.4);
    AddAttribute(domElement, "y", point.y()/PrintDPI*25.4);
    AddAttribute(domElement, "mx", point.mx()/PrintDPI*25.4);
    AddAttribute(domElement, "my", point.my()/PrintDPI*25.4);

    AddToCalculation(domElement);
    emit toolhaveChange();
}

QVariant VToolSinglePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value){
    if (change == ItemPositionChange && scene()) {
        // value - это новое положение.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        if (!rect.contains(newPos)) {
            // Сохраняем элемент внутри прямоугольника сцены.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
    }
    if (change == ItemPositionHasChanged && scene()) {
        // value - это новое положение.
        QPointF newPos = value.toPointF();
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("x", QString().setNum(newPos.x()/PrintDPI*25.4));
            domElement.setAttribute("y", QString().setNum(newPos.y()/PrintDPI*25.4));
            //I don't now why but signal does not work.
            doc->FullUpdateTree();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void VToolSinglePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ){
    ContextMenu(dialogSinglePoint, this, event);
}

void  VToolSinglePoint::FullUpdateFromFile(){
    RefreshGeometry();
}

void VToolSinglePoint::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QPointF p = dialogSinglePoint->getPoint();
        QString name = dialogSinglePoint->getName();
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", name);
            domElement.setAttribute("x", QString().setNum(p.x()/PrintDPI*25.4));
            domElement.setAttribute("y", QString().setNum(p.y()/PrintDPI*25.4));
            //I don't now why but signal does not work.
            doc->FullUpdateTree();
        }
    }
    dialogSinglePoint.clear();
}

void VToolSinglePoint::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        VToolPoint::ChangedActivDraw(newName);
    } else {
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        VToolPoint::ChangedActivDraw(newName);
    }
}
