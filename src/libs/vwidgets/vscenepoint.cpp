/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 6, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "vscenepoint.h"
#include "../vmisc/def.h"
#include "../vgeometry/vpointf.h"
#include "global.h"
#include "vgraphicssimpletextitem.h"

#include <QBrush>
#include <QFont>
#include <QPen>

//---------------------------------------------------------------------------------------------------------------------
VScenePoint::VScenePoint(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),
      m_namePoint(nullptr),
      m_lineName(nullptr),
      m_onlyPoint(false),
      m_isHovered(false),
      m_baseColor(Qt::black)
{
    m_namePoint = new VGraphicsSimpleTextItem(this);
    m_lineName = new QGraphicsLineItem(this);
    this->setBrush(QBrush(Qt::NoBrush));
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    if (m_namePoint->BaseFontSize()*scale < 1)
    {
        m_namePoint->setVisible(false);
        m_lineName->setVisible(false);
    }
    else
    {
        ScaleMainPenWidth(scale);
        ScaleCircleSize(this, scale);

        if (not m_onlyPoint)
        {
            m_namePoint->setVisible(true);
            m_lineName->setVisible(true);

            ScaleLinePenWidth(m_lineName, scale);

            RefreshLine();
        }
    }

    QGraphicsEllipseItem::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::RefreshPointGeometry(const VPointF &point)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    setPos(static_cast<QPointF>(point));
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    m_namePoint->blockSignals(true);
    m_namePoint->setText(point.name());
    m_namePoint->setPos(QPointF(point.mx(), point.my()));
    m_namePoint->blockSignals(false);

    RefreshLine();
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VScenePoint::boundingRect() const
{
    QRectF recTool = QGraphicsEllipseItem::boundingRect();
    recTool = recTool.united(childrenBoundingRect());
    return recTool;
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::SetOnlyPoint(bool value)
{
    m_onlyPoint = value;
    m_namePoint->setVisible(not m_onlyPoint);
    m_lineName->setVisible(not m_onlyPoint);
}

//---------------------------------------------------------------------------------------------------------------------
bool VScenePoint::IsOnlyPoint() const
{
    return m_onlyPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_isHovered = true;
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_isHovered = false;
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::RefreshLine()
{
    QRectF nRec = m_namePoint->sceneBoundingRect();
    nRec.translate(- scenePos());
    if (not rect().intersects(nRec))
    {
        const QRectF nameRec = m_namePoint->sceneBoundingRect();
        QPointF p1, p2;
        VGObject::LineIntersectCircle(QPointF(), ScaledRadius(SceneScale(scene())),
                                      QLineF(QPointF(), nameRec.center() - scenePos()), p1, p2);
        const QPointF pRec = VGObject::LineIntersectRect(nameRec, QLineF(scenePos(), nameRec.center()));
        m_lineName->setLine(QLineF(p1, pRec - scenePos()));

        if (QLineF(p1, pRec - scenePos()).length() <= ToPixel(4, Unit::Mm))
        {
            m_lineName->setVisible(false);
        }
        else
        {
            m_lineName->setVisible(true);
        }
    }
    else
    {
        m_lineName->setVisible(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::ScaleMainPenWidth(qreal scale)
{
    const qreal width = ScaleWidth(m_isHovered ? widthMainLine : widthHairLine, scale);

    setPen(QPen(CorrectColor(this, m_baseColor), width));
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::ScaleLinePenWidth(QGraphicsLineItem *line, qreal scale)
{
    SCASSERT(line != nullptr)
    const qreal width = ScaleWidth(widthHairLine, scale);

    line->setPen(QPen(CorrectColor(line, Qt::black), width));
}

