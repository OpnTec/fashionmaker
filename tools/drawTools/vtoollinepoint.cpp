#include "vtoollinepoint.h"

VToolLinePoint::VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                               const QString &typeLine, const QString &formula, const qint64 &basePointId,
                               const qint32 &angle, QGraphicsItem *parent):VToolPoint(doc, data, id, parent),
    typeLine(typeLine), formula(formula), angle(angle), basePointId(basePointId), mainLine(0){
    Q_ASSERT_X(basePointId > 0, Q_FUNC_INFO, "basePointId <= 0");
    //Лінія, що з'єднує дві точки
    QPointF point1 = data->GetPoint(basePointId).toQPointF();
    QPointF point2 = data->GetPoint(id).toQPointF();
    mainLine = new QGraphicsLineItem(QLineF(point1 - point2, point2 - point2), this);
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
    VToolPoint::RefreshPointGeometry(VDrawTool::data.GetPoint(id));
    QPointF point = VDrawTool::data.GetPoint(id).toQPointF();
    QPointF basePoint = VDrawTool::data.GetPoint(basePointId).toQPointF();
    mainLine->setLine(QLineF(basePoint - point, point - point));
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}
