/************************************************************************
 **
 **  @file   vtoollinepoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vtoollinepoint.h"

//---------------------------------------------------------------------------------------------------------------------
VToolLinePoint::VToolLinePoint(VPattern *doc, VContainer *data, const quint32 &id,
                               const QString &typeLine, const QString &formula, const quint32 &basePointId,
                               const qreal &angle, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent),  formula(formula), angle(angle), basePointId(basePointId),
      mainLine(nullptr)
{
    this->typeLine = typeLine;
    Q_ASSERT_X(basePointId > 0, Q_FUNC_INFO, "basePointId <= 0");
    QPointF point1 = data->GeometricObject<const VPointF *>(basePointId)->toQPointF();
    QPointF point2 = data->GeometricObject<const VPointF *>(id)->toQPointF();
    mainLine = new QGraphicsLineItem(QLineF(point1 - point2, QPointF()), this);
    mainLine->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle()));
    mainLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        currentColor = Qt::black;
    }
    else
    {
        currentColor = Qt::gray;
    }
    mainLine->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle()));
    VToolPoint::ChangedActivDraw(newName);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::RefreshGeometry()
{
    mainLine->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor, LineStyle()));
    VToolPoint::RefreshPointGeometry(*VDrawTool::data.GeometricObject<const VPointF *>(id));
    QPointF point = VDrawTool::data.GeometricObject<const VPointF *>(id)->toQPointF();
    QPointF basePoint = VDrawTool::data.GeometricObject<const VPointF *>(basePointId)->toQPointF();
    mainLine->setLine(QLineF(basePoint - point, QPointF()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolLinePoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    RefreshGeometry();
}
