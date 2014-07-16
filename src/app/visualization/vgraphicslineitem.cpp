/************************************************************************
 **
 **  @file   vgraphicslineitem.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vgraphicslineitem.h"
#include "../container/vcontainer.h"
#include "../geometry/vpointf.h"
#include "../widgets/vapplication.h"
#include "../tools/drawTools/vdrawtool.h"

//---------------------------------------------------------------------------------------------------------------------
VGraphicsLineItem::VGraphicsLineItem(const VContainer *data, QGraphicsItem *parent)
    :QObject(), QGraphicsLineItem(parent), data(data), point1Id(0), point2Id(0), factor(VDrawTool::factor),
      scenePos(QPointF()), color(Qt::red), lineStyle(Qt::SolidLine)
{}

//---------------------------------------------------------------------------------------------------------------------
VGraphicsLineItem::VGraphicsLineItem(const VContainer *data, const quint32 &pointId, const QPointF &scenePos,
                                     QGraphicsItem *parent)
    :QObject(), QGraphicsLineItem(parent), data(data), point1Id(pointId), point2Id(0), factor(VDrawTool::factor),
      scenePos(scenePos), color(Qt::black), lineStyle(Qt::SolidLine)
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
VGraphicsLineItem::~VGraphicsLineItem()
{}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsLineItem::SetFactor(qreal factor)
{
    VApplication::CheckFactor(this->factor, factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsLineItem::MousePos(const QPointF &scenePos)
{
    this->scenePos = scenePos;
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsLineItem::mouseMoveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsLineItem::setPoint2Id(const quint32 &value)
{
    point2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsLineItem::setPoint1Id(const quint32 &value)
{
    point1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsLineItem::setLineStyle(const Qt::PenStyle &value)
{
    lineStyle = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VGraphicsLineItem::RefreshGeometry()
{
    const VPointF *first = data->GeometricObject<const VPointF *>(point1Id);
    if (point2Id == 0)
    {
        this->setLine(QLineF(first->toQPointF(), scenePos));
    }
    else
    {
        const VPointF *second = data->GeometricObject<const VPointF *>(point2Id);
        this->setLine(QLineF(first->toQPointF(), second->toQPointF()));
    }
    this->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor, lineStyle));
}
