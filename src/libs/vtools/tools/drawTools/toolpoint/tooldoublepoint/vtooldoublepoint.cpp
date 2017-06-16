/************************************************************************
 **
 **  @file   vtooldoublepoint.cpp
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
#include "../ifc/exception/vexception.h"
#include "../ifc/exception/vexceptionbadid.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/logging.h"
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
    connect(firstPoint, &VSimplePoint::Choosed, this, &VToolDoublePoint::Point1Choosed);
    connect(firstPoint, &VSimplePoint::Selected, this, &VToolDoublePoint::Point1Selected);
    connect(firstPoint, &VSimplePoint::ShowContextMenu, this, &VToolDoublePoint::contextMenuEvent);
    connect(firstPoint, &VSimplePoint::Delete, this, &VToolDoublePoint::DeleteFromLabel);
    connect(firstPoint, &VSimplePoint::NameChangedPosition, this, &VToolDoublePoint::Label1ChangePosition);
    firstPoint->RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p1id));

    secondPoint = new VSimplePoint(p2id, QColor(Qt::black));
    secondPoint->setParentItem(this);
    connect(secondPoint, &VSimplePoint::Choosed, this, &VToolDoublePoint::Point2Choosed);
    connect(secondPoint, &VSimplePoint::Selected, this, &VToolDoublePoint::Point2Selected);
    connect(secondPoint, &VSimplePoint::ShowContextMenu, this, &VToolDoublePoint::contextMenuEvent);
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
void VToolDoublePoint::Label1ChangePosition(const QPointF &pos)
{
    ChangePosition(firstPoint, p1id, pos);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Label2ChangePosition(const QPointF &pos)
{
    ChangePosition(secondPoint, p2id, pos);
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
    emit ChangedToolSelection(selected, p1id, id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Point2Selected(bool selected)
{
    emit ChangedToolSelection(selected, p2id, id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::FullUpdateFromFile()
{
    ReadAttributes();
    firstPoint->RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p1id));
    secondPoint->RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p2id));
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::DoChangePosition(quint32 id, qreal mx, qreal my)
{
    if (id == p1id)
    {
        VPointF *point = new VPointF(*VAbstractTool::data.GeometricObject<VPointF>(p1id));
        point->setMx(mx);
        point->setMy(my);
        VAbstractTool::data.UpdateGObject(p1id, point);
        firstPoint->RefreshPointGeometry(*point);
    }
    else if (id == p2id)
    {
        VPointF *point = new VPointF(*VAbstractTool::data.GeometricObject<VPointF>(p2id));
        point->setMx(mx);
        point->setMy(my);
        VAbstractTool::data.UpdateGObject(p2id, point);
        secondPoint->RefreshPointGeometry(*point);
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
void VToolDoublePoint::UpdateNamePosition(quint32 id)
{
    if (id == p1id)
    {
        const VPointF *p1 = VAbstractTool::data.GeometricObject<VPointF>(p1id).data();

        auto moveLabel = new MoveDoubleLabel(doc, p1->mx(), p1->my(), DoublePoint::FirstPoint, this->id, p1id);
        connect(moveLabel, &MoveDoubleLabel::ChangePosition, this, &VToolDoublePoint::DoChangePosition);
        qApp->getUndoStack()->push(moveLabel);
    }
    else if (id == p2id)
    {
        const VPointF *p2 = VAbstractTool::data.GeometricObject<VPointF>(p2id).data();

        auto moveLabel = new MoveDoubleLabel(doc, p2->mx(), p2->my(), DoublePoint::SecondPoint, this->id, p2id);
        connect(moveLabel, &MoveDoubleLabel::ChangePosition, this, &VToolDoublePoint::DoChangePosition);
        qApp->getUndoStack()->push(moveLabel);
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
    if (change == QGraphicsItem::ItemSelectedChange)
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
                DeleteTool();
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
    Q_UNUSED(event)
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    VDrawTool::SaveOptions(tag, obj);

    if (obj->id() == p1id)
    {
        QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
        SCASSERT(point.isNull() == false)

        doc->SetAttribute(tag, AttrName1, point->name());
        doc->SetAttribute(tag, AttrMx1, qApp->fromPixel(point->mx()));
        doc->SetAttribute(tag, AttrMy1, qApp->fromPixel(point->my()));
    }
    else if (obj->id() == p2id)
    {
        QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
        SCASSERT(point.isNull() == false)

        doc->SetAttribute(tag, AttrName2, point->name());
        doc->SetAttribute(tag, AttrMx2, qApp->fromPixel(point->mx()));
        doc->SetAttribute(tag, AttrMy2, qApp->fromPixel(point->my()));
    }
    else
    {
        VPointF *p1 = VAbstractTool::data.GeometricObject<VPointF>(p1id).data();
        VPointF *p2 = VAbstractTool::data.GeometricObject<VPointF>(p2id).data();

        doc->SetAttribute(tag, AttrPoint1, p1id);
        doc->SetAttribute(tag, AttrName1, p1->name());
        doc->SetAttribute(tag, AttrMx1, qApp->fromPixel(p1->mx()));
        doc->SetAttribute(tag, AttrMy1, qApp->fromPixel(p1->my()));

        doc->SetAttribute(tag, AttrPoint2, p2id);
        doc->SetAttribute(tag, AttrName2, p2->name());
        doc->SetAttribute(tag, AttrMx2, qApp->fromPixel(p2->mx()));
        doc->SetAttribute(tag, AttrMy2, qApp->fromPixel(p2->my()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());
    QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(id);
    SaveOptions(domElement, obj);
    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        QSharedPointer<VGObject> obj = VContainer::GetFakeGObject(id);
        SaveOptions(domElement, obj);
    }
    else
    {
        qCDebug(vTool, "Can't find tool with id = %u", id);
    }
}
