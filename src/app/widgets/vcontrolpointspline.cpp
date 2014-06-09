/************************************************************************
 **
 **  @file   vcontrolpointspline.cpp
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

#include "vcontrolpointspline.h"
#include "../tools/vabstracttool.h"

#include <QPen>
#include "../widgets/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
VControlPointSpline::VControlPointSpline(const qint32 &indexSpline, SplinePoint::Position position,
                                         const QPointF &controlPoint, const QPointF &splinePoint,
                                         QGraphicsItem *parent)
    :QGraphicsEllipseItem(parent), radius(0), controlLine(nullptr), indexSpline(indexSpline),
      position(position)
{
    //create circle
    radius = (1.5/*mm*/ / 25.4) * VApplication::PrintDPI;
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
    this->setPos(controlPoint);

    QPointF p1, p2;
    VAbstractTool::LineIntersectCircle(QPointF(), radius, QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
    controlLine = new QGraphicsLineItem(QLineF(splinePoint-controlPoint, p1), this);
    controlLine->setPen(QPen(Qt::red, qApp->toPixel(qApp->widthHairLine())));
    controlLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine())));
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())));
}

//---------------------------------------------------------------------------------------------------------------------
QVariant VControlPointSpline::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        // value - new position.
        QPointF newPos = value.toPointF();
        emit ControlPointChangePosition(indexSpline, position, newPos);
    }
    return QGraphicsItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos,
                                      const QPointF &controlPoint, const QPointF &splinePoint)
{
    if (this->indexSpline == indexSpline && this->position == pos)
    {
        QPointF p1, p2;
        VAbstractTool::LineIntersectCircle(QPointF(), radius, QLineF( QPointF(), splinePoint-controlPoint), p1, p2);
        controlLine->setLine(QLineF(splinePoint-controlPoint, p1));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::setEnabledPoint(bool enable)
{
    if (enable == true)
    {
        this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())));
        this->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->setAcceptHoverEvents(true);
    }
    else
    {
        this->setPen(QPen(Qt::gray, qApp->toPixel(qApp->widthHairLine())));
        this->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->setFlag(QGraphicsItem::ItemIsMovable, false);
        this->setAcceptHoverEvents(false);
    }
}
