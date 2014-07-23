/************************************************************************
 **
 **  @file   visline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 7, 2014
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

#include "visline.h"
#include "../container/vcontainer.h"
#include "../widgets/vapplication.h"
#include "../tools/drawTools/vdrawtool.h"

//---------------------------------------------------------------------------------------------------------------------
VisLine::VisLine(const VContainer *data, QGraphicsItem *parent)
    :QObject(), QGraphicsLineItem(parent), data(data), factor(VDrawTool::factor), scenePos(QPointF()),
      color(Qt::red), lineStyle(Qt::SolidLine), point1Id(0), toolTip(QString())
{}

//---------------------------------------------------------------------------------------------------------------------
VisLine::~VisLine()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::SetFactor(qreal factor)
{
    VApplication::CheckFactor(this->factor, factor);
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::setLineStyle(const Qt::PenStyle &value)
{
    lineStyle = value;
    this->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor, lineStyle));
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::MousePos(const QPointF &scenePos)
{
    this->scenePos = scenePos;
    RefreshGeometry();
    emit ToolTip(toolTip);
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::setColor(const QColor &value)
{
    color = value;
    this->setPen(QPen(color, qApp->toPixel(qApp->widthHairLine())/factor, lineStyle));
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::setScenePos(const QPointF &value)
{
    scenePos = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::VisualMode(const quint32 &pointId, const QPointF &scenePos)
{
    this->color = Qt::black;
    this->point1Id = pointId;
    this->scenePos = scenePos;
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void VisLine::setPoint1Id(const quint32 &value)
{
    point1Id = value;
}
