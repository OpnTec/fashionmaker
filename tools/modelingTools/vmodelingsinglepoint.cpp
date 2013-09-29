#include "vmodelingsinglepoint.h"
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QGraphicsItem>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <cmath>
#include "options.h"
#include "container/vpointf.h"

VModelingSinglePoint::VModelingSinglePoint (VDomDocument *doc, VContainer *data, qint64 id, Tool::Sources typeCreation,
                                    QGraphicsItem * parent ):VModelingPoint(doc, data, id, parent),
    dialogSinglePoint(QSharedPointer<DialogSinglePoint>()){
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    if(typeCreation == Tool::FromGui){
        AddToFile();
    }
}

void VModelingSinglePoint::setDialog(){
    Q_ASSERT(!dialogSinglePoint.isNull());
    if(!dialogSinglePoint.isNull()){
        VPointF p = VAbstractTool::data.GetPoint(id);
        dialogSinglePoint->setData(p.name(), p.toQPointF());
    }
}

void VModelingSinglePoint::AddToFile(){
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement("point");

    AddAttribute(domElement, "id", id);
    AddAttribute(domElement, "type", "single");
    AddAttribute(domElement, "name", point.name());
    AddAttribute(domElement, "x", toMM(point.x()));
    AddAttribute(domElement, "y", toMM(point.y()));
    AddAttribute(domElement, "mx", toMM(point.mx()));
    AddAttribute(domElement, "my", toMM(point.my()));

    AddToModeling(domElement);
}

QVariant VModelingSinglePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value){
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
            domElement.setAttribute("x", QString().setNum(toMM(newPos.x())));
            domElement.setAttribute("y", QString().setNum(toMM(newPos.y())));
            //I don't now why but signal does not work.
            doc->FullUpdateTree();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void VModelingSinglePoint::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ){
    ContextMenu(dialogSinglePoint, this, event, false);
}

void  VModelingSinglePoint::FullUpdateFromFile(){
    RefreshPointGeometry(VAbstractTool::data.GetPoint(id));
}

void VModelingSinglePoint::FullUpdateFromGui(int result){
    if(result == QDialog::Accepted){
        QPointF p = dialogSinglePoint->getPoint();
        QString name = dialogSinglePoint->getName();
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if(domElement.isElement()){
            domElement.setAttribute("name", name);
            domElement.setAttribute("x", QString().setNum(toMM(p.x())));
            domElement.setAttribute("y", QString().setNum(toMM(p.y())));
            //I don't now why but signal does not work.
            doc->FullUpdateTree();
        }
    }
    dialogSinglePoint.clear();
}
