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

#include "global.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "vgraphicssimpletextitem.h"
#include "../vmisc/vabstractapplication.h"

//---------------------------------------------------------------------------------------------------------------------
VSimplePoint::VSimplePoint(quint32 id, const QColor &currentColor, QObject *parent)
    : VAbstractSimple(id, parent),
      VScenePoint(),
      m_visualizationMode(false),
      m_alwaysHovered(false)
{
    m_baseColor = currentColor;
    connect(m_namePoint, &VGraphicsSimpleTextItem::ShowContextMenu, this, &VSimplePoint::ContextMenu);
    connect(m_namePoint, &VGraphicsSimpleTextItem::DeleteTool, this, &VSimplePoint::DeleteFromLabel);
    connect(m_namePoint, &VGraphicsSimpleTextItem::PointChoosed, this, &VSimplePoint::PointChoosed);
    connect(m_namePoint, &VGraphicsSimpleTextItem::PointSelected, this, &VSimplePoint::PointSelected);
    connect(m_namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this, &VSimplePoint::ChangedPosition);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::SetVisualizationMode(bool value)
{
    m_visualizationMode = value;
    setFlag(QGraphicsItem::ItemIsFocusable, not m_visualizationMode);
}

//---------------------------------------------------------------------------------------------------------------------
bool VSimplePoint::IsVisualizationMode() const
{
    return m_visualizationMode;
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::SetPointHighlight(bool value)
{
    m_alwaysHovered = value;
    m_isHovered = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::SetEnabled(bool enabled)
{
    setEnabled(enabled);
    m_namePoint->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::EnableToolMove(bool move)
{
    m_namePoint->setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::AllowLabelHover(bool enabled)
{
    m_namePoint->setAcceptHoverEvents(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::AllowLabelSelecting(bool enabled)
{
    m_namePoint->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::ToolSelectionType(const SelectionType &type)
{
    VAbstractSimple::ToolSelectionType(type);
    m_namePoint->LabelSelectionType(type);
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
    if (m_visualizationMode)
    {
        event->ignore();
    }
    else
    {
        // Special for not selectable item first need to call standard mousePressEvent then accept event
        QGraphicsEllipseItem::mousePressEvent(event);

        // Somehow clicking on notselectable object do not clean previous selections.
        if (not (flags() & ItemIsSelectable) && scene())
        {
            scene()->clearSelection();
        }

        if (selectionType == SelectionType::ByMouseRelease)
        {// Special for not selectable item first need to call standard mousePressEvent then accept event
            event->accept();
        }
        else
        {
            if (event->button() == Qt::LeftButton)
            {
                emit Choosed(id);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (not m_visualizationMode)
    {
        if (selectionType == SelectionType::ByMouseRelease)
        {
            if (event->button() == Qt::LeftButton)
            {
                emit Choosed(id);
            }
        }
        VScenePoint::mouseReleaseEvent(event);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_isHovered = true;
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (not m_alwaysHovered)
    {
        m_isHovered = false;
    }
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
    VScenePoint::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
QVariant VSimplePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        m_namePoint->blockSignals(true);
        m_namePoint->setSelected(value.toBool());
        m_namePoint->blockSignals(false);
        emit Selected(value.toBool(), id);
    }

    return VScenePoint::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VSimplePoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    emit ShowContextMenu(event);
}
