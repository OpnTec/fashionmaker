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

#include "vmodelinglinepoint.h"

VModelingLinePoint::VModelingLinePoint(VDomDocument *doc, VContainer *data, const qint64 &id,
                               const QString &typeLine, const QString &formula, const qint64 &basePointId,
                               const qreal &angle, QGraphicsItem *parent):
    VModelingPoint(doc, data, id, parent), typeLine(typeLine), formula(formula), angle(angle),
    basePointId(basePointId), mainLine(0){
    //Лінія, що з'єднує дві точки
    QPointF point1 = data->GetModelingPoint(basePointId).toQPointF();
    QPointF point2 = data->GetModelingPoint(id).toQPointF();
    mainLine = new QGraphicsLineItem(QLineF(point1 - point2, QPointF()), this);
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
    mainLine->setLine(QLineF(basePoint - point, QPointF()));
    if(typeLine == "none"){
        mainLine->setVisible(false);
    } else {
        mainLine->setVisible(true);
    }
}
