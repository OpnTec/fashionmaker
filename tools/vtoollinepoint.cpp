#include "vtoollinepoint.h"

VToolLinePoint::VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                               const QString &formula, const qint64 &pointId, const qint32 &angle,
                               QGraphicsItem *parent):VToolPoint(doc, data, id, parent){
    this->typeLine = typeLine;
    this->formula = formula;
    this->angle = angle;

    //Лінія, що з'єднує дві точки
    VPointF firstPoint = data->GetPoint(pointId);
    VPointF point = data->GetPoint(id);
    mainLine = new QGraphicsLineItem(QLineF(firstPoint.toQPointF(), point.toQPointF()), this);
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
