/************************************************************************
 **
 **  @file   vtooldoublepoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include "vtooldoublepoint.h"

#include <QColor>
#include <QDomElement>
#include <QKeyEvent>
#include <QPoint>
#include <QSharedPointer>
#include <QUndoStack>
#include <Qt>
#include <new>

#include "../../../../undocommands/label/movedoublelabel.h"
#include "../../../../undocommands/label/showdoublelabel.h"
#include "../ifc/exception/vexception.h"
#include "../ifc/exception/vexceptionbadid.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/../ifc/ifcdef.h"
#include "../vwidgets/vsimplepoint.h"
#include "../../../vabstracttool.h"
#include "../../../vdatatool.h"
#include "../../vdrawtool.h"
#include "../vabstractpoint.h"

//---------------------------------------------------------------------------------------------------------------------
VToolDoublePoint::VToolDoublePoint(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 p1id, quint32 p2id,
                                   QGraphicsItem *parent)
    :VAbstractPoint(doc, data, id),
      QGraphicsPathItem(parent),
      firstPoint(nullptr),
      secondPoint(nullptr),
      p1id(p1id),
      p2id(p2id)
{
    firstPoint = new VSimplePoint(p1id, QColor(Qt::black));
    firstPoint->setParentItem(this);
    firstPoint->setToolTip(ComplexToolTip(p1id));
    connect(firstPoint, &VSimplePoint::Choosed, this, &VToolDoublePoint::Point1Choosed);
    connect(firstPoint, &VSimplePoint::Selected, this, &VToolDoublePoint::Point1Selected);
    connect(firstPoint, &VSimplePoint::ShowContextMenu, this, &VToolDoublePoint::ShowContextMenu);
    connect(firstPoint, &VSimplePoint::Delete, this, &VToolDoublePoint::DeleteFromLabel);
    connect(firstPoint, &VSimplePoint::NameChangedPosition, this, &VToolDoublePoint::Label1ChangePosition);
    firstPoint->RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p1id));

    secondPoint = new VSimplePoint(p2id, QColor(Qt::black));
    secondPoint->setParentItem(this);
    secondPoint->setToolTip(ComplexToolTip(p2id));
    connect(secondPoint, &VSimplePoint::Choosed, this, &VToolDoublePoint::Point2Choosed);
    connect(secondPoint, &VSimplePoint::Selected, this, &VToolDoublePoint::Point2Selected);
    connect(secondPoint, &VSimplePoint::ShowContextMenu, this, &VToolDoublePoint::ShowContextMenu);
    connect(secondPoint, &VSimplePoint::Delete, this, &VToolDoublePoint::DeleteFromLabel);
    connect(secondPoint, &VSimplePoint::NameChangedPosition, this, &VToolDoublePoint::Label2ChangePosition);
    secondPoint->RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p2id));
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolDoublePoint::nameP1() const
{
    return ObjectName<VPointF>(p1id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::setNameP1(const QString &name)
{
    SetPointName(p1id, name);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolDoublePoint::nameP2() const
{
    return ObjectName<VPointF>(p2id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::setNameP2(const QString &name)
{
    SetPointName(p2id, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::GroupVisibility(quint32 object, bool visible)
{
    if (object == p1id)
    {
        firstPoint->setVisible(visible);
    }
    else if (object == p2id)
    {
        secondPoint->setVisible(visible);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VToolDoublePoint::IsLabelVisible(quint32 id) const
{
    if (p1id == id)
    {
        return VAbstractTool::data.GeometricObject<VPointF>(p1id)->IsShowLabel();
    }
    else if (p2id == id)
    {
        return VAbstractTool::data.GeometricObject<VPointF>(p2id)->IsShowLabel();
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::SetLabelVisible(quint32 id, bool visible)
{
    if (p1id == id)
    {
        const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(p1id);
        point->SetShowLabel(visible);
        firstPoint->RefreshPointGeometry(*point);
        if (QGraphicsScene *sc = firstPoint->scene())
        {
            VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), firstPoint);
        }
    }
    else if (p2id == id)
    {
        const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(p2id);
        point->SetShowLabel(visible);
        secondPoint->RefreshPointGeometry(*point);
        if (QGraphicsScene *sc = secondPoint->scene())
        {
            VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), secondPoint);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Label1ChangePosition(const QPointF &pos)
{
    UpdateNamePosition(p1id, pos - firstPoint->pos());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Label2ChangePosition(const QPointF &pos)
{
    UpdateNamePosition(p2id, pos - secondPoint->pos());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Disable(bool disable, const QString &namePP)
{
    const bool enabled = !CorrectDisable(disable, namePP);
    this->setEnabled(enabled);
    firstPoint->SetEnabled(enabled);
    secondPoint->SetEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::EnableToolMove(bool move)
{
    firstPoint->EnableToolMove(move);
    secondPoint->EnableToolMove(move);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Point1Choosed()
{
    emit ChoosedTool(p1id, SceneObject::Point);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Point2Choosed()
{
    emit ChoosedTool(p2id, SceneObject::Point);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Point1Selected(bool selected)
{
    emit ChangedToolSelection(selected, p1id, m_id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Point2Selected(bool selected)
{
    emit ChangedToolSelection(selected, p2id, m_id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::FullUpdateFromFile()
{
    ReadAttributes();
    firstPoint->setToolTip(ComplexToolTip(p1id));
    firstPoint->RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p1id));
    secondPoint->setToolTip(ComplexToolTip(p2id));
    secondPoint->RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p2id));
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::ChangeLabelPosition(quint32 id, const QPointF &pos)
{
    if (id == p1id)
    {
        QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(p1id);
        point->setMx(pos.x());
        point->setMy(pos.y());
        firstPoint->RefreshPointGeometry(*(point.data()));

        if (QGraphicsScene *sc = firstPoint->scene())
        {
            VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), firstPoint);
        }
    }
    else if (id == p2id)
    {
        QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(p2id);
        point->setMx(pos.x());
        point->setMy(pos.y());
        secondPoint->RefreshPointGeometry(*(point.data()));

        if (QGraphicsScene *sc = secondPoint->scene())
        {
            VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), secondPoint);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::AllowHover(bool enabled)
{
    firstPoint->setAcceptHoverEvents(enabled);
    secondPoint->setAcceptHoverEvents(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::AllowSelecting(bool enabled)
{
    firstPoint->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
    secondPoint->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::AllowLabelHover(bool enabled)
{
    firstPoint->AllowLabelHover(enabled);
    secondPoint->AllowLabelHover(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::AllowLabelSelecting(bool enabled)
{
    firstPoint->AllowLabelSelecting(enabled);
    secondPoint->AllowLabelSelecting(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::ToolSelectionType(const SelectionType &type)
{
    VAbstractTool::ToolSelectionType(type);
    firstPoint->ToolSelectionType(type);
    secondPoint->ToolSelectionType(type);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::UpdateNamePosition(quint32 id, const QPointF &pos)
{    
    if (id == p1id)
    {
        qApp->getUndoStack()->push(new MoveDoubleLabel(doc, pos, MoveDoublePoint::FirstPoint, m_id, p1id));
    }
    else if (id == p2id)
    {
        qApp->getUndoStack()->push(new MoveDoubleLabel(doc, pos, MoveDoublePoint::SecondPoint, m_id, p2id));
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange hadle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant VToolDoublePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        if (value == true)
        {
            // do stuff if selected
            this->setFocus();
        }
        else
        {
            // do stuff if not selected
        }
    }

    return QGraphicsPathItem::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyReleaseEvent handle key release events.
 * @param event key release event.
 */
void VToolDoublePoint::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
            try
            {
                DeleteToolWithConfirm();
            }
            catch(const VExceptionToolWasDeleted &e)
            {
                Q_UNUSED(e)
                return;//Leave this method immediately!!!
            }
            break;
        default:
            break;
    }
    QGraphicsPathItem::keyReleaseEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ShowContextMenu(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    auto SavePoint1 = [this](QDomElement &tag, const QSharedPointer<VPointF> &point)
    {
        doc->SetAttribute(tag, AttrName1, point->name());
        doc->SetAttribute(tag, AttrMx1, qApp->fromPixel(point->mx()));
        doc->SetAttribute(tag, AttrMy1, qApp->fromPixel(point->my()));
        doc->SetAttribute<bool>(tag, AttrShowLabel1, point->IsShowLabel());
    };

    auto SavePoint2 = [this](QDomElement &tag, const QSharedPointer<VPointF> &point)
    {
        doc->SetAttribute(tag, AttrName2, point->name());
        doc->SetAttribute(tag, AttrMx2, qApp->fromPixel(point->mx()));
        doc->SetAttribute(tag, AttrMy2, qApp->fromPixel(point->my()));
        doc->SetAttribute<bool>(tag, AttrShowLabel2, point->IsShowLabel());
    };

    if (obj->id() == p1id)
    {
        QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
        SCASSERT(point.isNull() == false)

        SavePoint1(tag, point);
    }
    else if (obj->id() == p2id)
    {
        QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
        SCASSERT(point.isNull() == false)

        SavePoint2(tag, point);
    }
    else
    {
        doc->SetAttribute(tag, AttrPoint1, p1id);
        SavePoint1(tag, VAbstractTool::data.GeometricObject<VPointF>(p1id));

        doc->SetAttribute(tag, AttrPoint2, p2id);
        SavePoint2(tag, VAbstractTool::data.GeometricObject<VPointF>(p2id));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());
    QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(m_id);
    SaveOptions(domElement, obj);
    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::ChangeLabelVisibility(quint32 id, bool visible)
{
    if (id == p1id)
    {
        qApp->getUndoStack()->push(new ShowDoubleLabel(doc, m_id, p1id, visible, ShowDoublePoint::FirstPoint));
    }
    else if (id == p2id)
    {
        qApp->getUndoStack()->push(new ShowDoubleLabel(doc, m_id, p2id, visible, ShowDoublePoint::SecondPoint));
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolDoublePoint::ComplexToolTip(quint32 itemId) const
{
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(itemId);

    const QString toolTip = QString("<table>"
                                    "<tr> <td><b>%1:</b> %2</td> </tr>"
                                    "%3"
                                    "</table>")
            .arg(tr("Label"), point->name(), MakeToolTip());
    return toolTip;
}
