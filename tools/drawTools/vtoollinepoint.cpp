/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vtoollinepoint.h"

VToolLinePoint::VToolLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                               const QString &typeLine, const QString &formula, const qint64 &basePointId,
                               const qreal &angle, QGraphicsItem *parent):VToolPoint(doc, data, id, parent),
    typeLine(typeLine), formula(formula), angle(angle), basePointId(basePointId), mainLine(0){
    Q_ASSERT_X(basePointId > 0, Q_FUNC_INFO, "basePointId <= 0");
    //Лінія, що з'єднує дві точки
    QPointF point1 = data->GetPoint(basePointId).toQPointF();
    QPointF point2 = data->GetPoint(id).toQPointF();
    mainLine = new QGraphicsLineItem(QLineF(point1 - point2, QPointF()), this);
    mainLine->setPen(QPen(Qt::black, widthHairLine/factor));
    mainLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}

void VToolLinePoint::ChangedActivDraw(const QString newName){
    if(nameActivDraw == newName){
        mainLine->setPen(QPen(Qt::black, widthHairLine/factor));
        currentColor = Qt::black;
        VToolPoint::ChangedActivDraw(newName);
    } else {
        mainLine->setPen(QPen(Qt::gray, widthHairLine/factor));
        currentColor = Qt::gray;
        VToolPoint::ChangedActivDraw(newName);
    }
}

void VToolLinePoint::RefreshGeometry(){
    mainLine->setPen(QPen(currentColor, widthHairLine/factor));
    VToolPoint::RefreshPointGeometry(VDrawTool::data.GetPoint(id));
    QPointF point = VDrawTool::data.GetPoint(id).toQPointF();
    QPointF basePoint = VDrawTool::data.GetPoint(basePointId).toQPointF();
    mainLine->setLine(QLineF(basePoint - point, QPointF()));
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}

void VToolLinePoint::SetFactor(qreal factor){
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}
