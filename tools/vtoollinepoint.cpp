#include "vtoollinepoint.h"

VToolLinePoint::VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                               const QString &formula, const qint64 &basePointId, const qint32 &angle,
                               QGraphicsItem *parent):VToolPoint(doc, data, id, parent){
    this->typeLine = typeLine;
    this->formula = formula;
    this->angle = angle;
    this->basePointId = basePointId;

    //Лінія, що з'єднує дві точки
    VPointF point1 = data->GetPoint(basePointId);
    VPointF point2 = data->GetPoint(id);
    mainLine = new QGraphicsLineItem(QLineF(point1.toQPointF(), point2.toQPointF()), this);
    mainLine->setPen(QPen(Qt::black, widthHairLine));
    mainLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}

void VToolLinePoint::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        mainLine->setPen(QPen(Qt::black, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    } else {
        mainLine->setPen(QPen(Qt::gray, widthHairLine));
        VToolPoint::ChangedActivDraw(newName);
    }
}

void VToolLinePoint::RefreshGeometry(){
    VToolPoint::RefreshGeometry();
    VPointF point = VAbstractTool::data->GetPoint(id);
    VPointF basePoint = VAbstractTool::data->GetPoint(basePointId);
    mainLine->setLine(QLineF(basePoint.toQPointF(), point.toQPointF()));
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}
