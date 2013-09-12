#include "vmodelinglinepoint.h"

VModelingLinePoint::VModelingLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                               const QString &typeLine, const QString &formula, const qint64 &basePointId,
                               const qint32 &angle, QGraphicsItem *parent):
    VModelingPoint(doc, data, id, parent), typeLine(typeLine), formula(formula), angle(angle),
    basePointId(basePointId), mainLine(0){
    //Лінія, що з'єднує дві точки
    QPointF point1 = data->GetModelingPoint(basePointId).toQPointF();
    QPointF point2 = data->GetModelingPoint(id).toQPointF();
    mainLine = new QGraphicsLineItem(QLineF(point1 - point2, 0), this);
    mainLine->setPen(QPen(Qt::black, widthHairLine));
    mainLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}

void VModelingLinePoint::RefreshGeometry(){
    VModelingPoint::RefreshPointGeometry(VModelingTool::data.GetModelingPoint(id));
    QPointF point = VModelingTool::data.GetModelingPoint(id).toQPointF();
    QPointF basePoint = VModelingTool::data.GetModelingPoint(basePointId).toQPointF();
    mainLine->setLine(QLineF(basePoint - point, 0));
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}
