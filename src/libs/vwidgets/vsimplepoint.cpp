/************************************************************************
 **
 **  @file   vsimplepoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vsimplepoint.h"

#include <QBrush>
#include <QFlags>
#include <QFont>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QLineF>
#include <QPen>
#include <QPoint>
#include <QRectF>
#include <Qt>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "vgraphicssimpletextitem.h"

class QGraphicsSceneContextMenuEvent;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;
class QKeyEvent;

//---------------------------------------------------------------------------------------------------------------------
VSimplePoint::VSimplePoint(quint32 id, const QColor &currentColor, Unit patternUnit, qreal *factor, QObject *parent)
    : VAbstractSimple(id, currentColor, patternUnit, factor, parent),
      QGraphicsEllipseItem(),
      radius(ToPixel(DefPointRadius/*mm*/, Unit::Mm)),
      namePoint(nullptr),
      lineName(nullptr),
      m_onlyPoint(false),
      m_isHighlight(false)
{
    namePoint = new VGraphicsSimpleTextItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::ShowContextMenu, this, &VSimplePoint::ContextMenu);
    connect(namePoint, &VGraphicsSimpleTextItem::DeleteTool, this, &VSimplePoint::DeleteFromLabel);
    connect(namePoint, &VGraphicsSimpleTextItem::PointChoosed, this, &VSimplePoint::PointChoosed);
    connect(namePoint, &VGraphicsSimpleTextItem::PointSelected, this, &VSimplePoint::PointSelected);
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this, &VSimplePoint::ChangedPosition);
    lineName = new QGraphicsLineItem(this);
    this->setBrush(QBrush(Qt::NoBrush));
    SetPen(this, currentColor, m_isHighlight ? WidthMainLine(patternUnit) : WidthHairLine(patternUnit));
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);// For keyboard input focus
}

//---------------------------------------------------------------------------------------------------------------------
VSimplePoint::~VSimplePoint()
{}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::SetOnlyPoint(bool value)
{
    m_onlyPoint = value;
    namePoint->setVisible(not m_onlyPoint);
    lineName->setVisible(not m_onlyPoint);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSimplePoint::IsOnlyPoint() const
{
    return m_onlyPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::SetPointHighlight(bool value)
{
    m_isHighlight = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::RefreshLine()
{
    QRectF nRec = namePoint->sceneBoundingRect();
    nRec.translate(- scenePos());
    if (this->rect().intersects(nRec) == false)
    {
        const QRectF nameRec = namePoint->sceneBoundingRect();
        QPointF p1, p2;
        VGObject::LineIntersectCircle(QPointF(), radius, QLineF(QPointF(), nameRec.center() - scenePos()), p1, p2);
        const QPointF pRec = VGObject::LineIntersectRect(nameRec, QLineF(scenePos(), nameRec.center()));
        lineName->setLine(QLineF(p1, pRec - scenePos()));
        SetPen(lineName, Qt::black, WidthHairLine(patternUnit));

        if (QLineF(p1, pRec - scenePos()).length() <= ToPixel(4, Unit::Mm))
        {
            lineName->setVisible(false);
        }
        else
        {
            lineName->setVisible(true);
        }
    }
    else
    {
        lineName->setVisible(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::RefreshGeometry(const VPointF &point)
{
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    SetPen(this, currentColor, m_isHighlight ? WidthMainLine(patternUnit) : WidthHairLine(patternUnit));
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPos(point);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    namePoint->blockSignals(true);
    QFont font = namePoint->font();
    font.setPointSize(static_cast<qint32>(namePoint->FontSize()/ *factor));
    namePoint->setFont(font);
    namePoint->setText(point.name());
    namePoint->setPos(QPointF(point.mx(), point.my()));
    namePoint->blockSignals(false);
    RefreshLine();
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::SetEnabled(bool enabled)
{
    VAbstractSimple::SetEnabled(enabled);
    SetPen(this, currentColor, m_isHighlight ? WidthMainLine(patternUnit) : WidthHairLine(patternUnit));
    SetPen(lineName, Qt::black, WidthHairLine(patternUnit));
    namePoint->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::EnableToolMove(bool move)
{
    namePoint->setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::AllowLabelHover(bool enabled)
{
    namePoint->setAcceptHoverEvents(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::AllowLabelSelecting(bool enabled)
{
    namePoint->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::ToolSelectionType(const SelectionType &type)
{
    VAbstractSimple::ToolSelectionType(type);
    namePoint->LabelSelectionType(type);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::DeleteFromLabel()
{
    emit Delete();
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::PointChoosed()
{
    emit Choosed(id);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::PointSelected(bool selected)
{
    setSelected(selected);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::ChangedPosition(const QPointF &pos)
{
    emit NameChangedPosition(pos, id);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Special for not selectable item first need to call standard mousePressEvent then accept event
    QGraphicsEllipseItem::mousePressEvent(event);

    // Somehow clicking on notselectable object do not clean previous selections.
    if (not (flags() & ItemIsSelectable) && scene())
    {
        scene()->clearSelection();
    }

    if (selectionType == SelectionType::ByMouseRelease)
    {
        event->accept();// Special for not selectable item first need to call standard mousePressEvent then accept event
    }
    else
    {
        if (event->button() == Qt::LeftButton)
        {
            emit Choosed(id);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (selectionType == SelectionType::ByMouseRelease)
    {
        if (event->button() == Qt::LeftButton)
        {
            emit Choosed(id);
        }
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    SetPen(this, currentColor, WidthMainLine(patternUnit));
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    SetPen(this, currentColor, WidthHairLine(patternUnit));
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            emit Delete();
            return; //Leave this method immediately after call!!!
        default:
            break;
    }
    QGraphicsEllipseItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
QVariant VSimplePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        namePoint->blockSignals(true);
        namePoint->setSelected(value.toBool());
        namePoint->blockSignals(false);
        emit Selected(value.toBool(), id);
    }

    return QGraphicsEllipseItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    emit ShowContextMenu(event);
}
