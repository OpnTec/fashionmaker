/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 6, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../vmisc/vabstractapplication.h"
#include "../vgeometry/vpointf.h"
#include "global.h"
#include "vgraphicssimpletextitem.h"
#include "scalesceneitems.h"

#include <QBrush>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

//---------------------------------------------------------------------------------------------------------------------
VScenePoint::VScenePoint(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),
      m_namePoint(new VGraphicsSimpleTextItem(this)),
      m_lineName(new VScaledLine(this)),
      m_onlyPoint(false),
      m_isHovered(false),
      m_showLabel(true),
      m_baseColor(Qt::black),
      m_selectedFromChild(false)
{
    m_lineName->SetBoldLine(false);
    m_lineName->setLine(QLineF(0, 0, 1, 0));
    m_lineName->setVisible(false);

    this->setBrush(QBrush(Qt::NoBrush));
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    if (m_namePoint->BaseFontSize()*scale < minVisibleFontSize)
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
            m_namePoint->setVisible(m_showLabel);

            QPen lPen = m_lineName->pen();
            QColor color = CorrectColor(m_lineName, Qt::black);
            color.setAlpha(50);
            lPen.setColor(color);
            m_lineName->setPen(lPen);

            RefreshLine();
        }
    }

    PaintWithFixItemHighlightSelected<QGraphicsEllipseItem>(this, painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VScenePoint::RefreshPointGeometry(const VPointF &point)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    setPos(static_cast<QPointF>(point));
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    m_showLabel = point.IsShowLabel();

    m_namePoint->blockSignals(true);
    m_namePoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    m_namePoint->SetRealPos(QPointF(point.mx(), point.my()));
    m_namePoint->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    m_namePoint->blockSignals(false);

    m_namePoint->setText(point.name());
    m_namePoint->setVisible(m_showLabel);

    RefreshLine();
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
        QPointF p1;
        QPointF p2;
        VGObject::LineIntersectCircle(QPointF(), ScaledRadius(SceneScale(scene())),
                                      QLineF(QPointF(), nameRec.center() - scenePos()), p1, p2);
        const QPointF pRec = VGObject::LineIntersectRect(nameRec, QLineF(scenePos(), nameRec.center()));

        if (QLineF(p1, pRec - scenePos()).length() <= ToPixel(4/qMax(1.0, SceneScale(scene())), Unit::Mm))
        {
            m_lineName->setVisible(false);
        }
        else
        {
            m_lineName->setLine(QLineF(p1, pRec - scenePos()));
            m_lineName->setVisible(m_showLabel);
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
    const qreal width = ScaleWidth(m_isHovered ? qApp->Settings()->WidthMainLine() : qApp->Settings()->WidthHairLine(),
                                   scale);

    setPen(QPen(CorrectColor(this, m_baseColor), width));
}
