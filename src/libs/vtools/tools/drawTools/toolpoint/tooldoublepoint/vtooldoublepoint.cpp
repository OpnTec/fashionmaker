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
#include "../vwidgets/vsimplepoint.h"
#include "../vgeometry/vpointf.h"
#include "../../../../undocommands/movelabel.h"

//---------------------------------------------------------------------------------------------------------------------
VToolDoublePoint::VToolDoublePoint(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 p1id, quint32 p2id,
                                   QGraphicsItem *parent)
    :VAbstractPoint(doc, data, id), QGraphicsPathItem(parent), firstPoint(nullptr), secondPoint(nullptr), p1id(p1id),
      p2id(p2id)
{
    firstPoint = new VSimplePoint(p1id, QColor(baseColor), *data->GetPatternUnit(), &factor);
    firstPoint->setParentItem(this);
    connect(firstPoint, &VSimplePoint::Choosed, this, &VToolDoublePoint::Point1Choosed);
    connect(firstPoint, &VSimplePoint::ShowContextMenu, this, &VToolDoublePoint::contextMenuEvent);
    connect(firstPoint, &VSimplePoint::Delete, this, &VToolDoublePoint::DeleteFromLabel);
    connect(firstPoint, &VSimplePoint::NameChangedPosition, this, &VToolDoublePoint::Label1ChangePosition);

    secondPoint = new VSimplePoint(p2id, QColor(baseColor), *data->GetPatternUnit(), &factor);
    secondPoint->setParentItem(this);
    connect(secondPoint, &VSimplePoint::Choosed, this, &VToolDoublePoint::Point2Choosed);
    connect(firstPoint, &VSimplePoint::ShowContextMenu, this, &VToolDoublePoint::contextMenuEvent);
    connect(firstPoint, &VSimplePoint::Delete, this, &VToolDoublePoint::DeleteFromLabel);
    connect(firstPoint, &VSimplePoint::NameChangedPosition, this, &VToolDoublePoint::Label2ChangePosition);
}

//---------------------------------------------------------------------------------------------------------------------
VToolDoublePoint::~VToolDoublePoint()
{}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /* From question on StackOverflow
     * https://stackoverflow.com/questions/10985028/how-to-remove-border-around-qgraphicsitem-when-selected
     *
     * There's no interface to disable the drawing of the selection border for the build-in QGraphicsItems. The only way
     * I can think of is derive your own items from the build-in ones and override the paint() function:*/
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected;
    QGraphicsPathItem::paint(painter, &myOption, widget);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolDoublePoint::nameP1() const
{
    return PointName(p1id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::setNameP1(const QString &name)
{
    SetPointName(p1id, name);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolDoublePoint::nameP2() const
{
    return PointName(p2id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::setNameP2(const QString &name)
{
    SetPointName(p2id, name);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::SetEnabled(bool enabled)
{
    SetToolEnabled(this, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Label1ChangePosition(const QPointF &pos)
{
    ChangePosition(this, p1id, pos);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Label2ChangePosition(const QPointF &pos)
{
    ChangePosition(this, p2id, pos);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::SetFactor(qreal factor)
{
    VDrawTool::SetFactor(factor);
    firstPoint->RefreshGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p1id));
    secondPoint->RefreshGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p2id));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::Disable(bool disable, const QString &namePP)
{
    enabled = !CorrectDisable(disable, namePP);
    this->SetEnabled(enabled);
    firstPoint->setEnabled(enabled);
    secondPoint->setEnabled(enabled);
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
void VToolDoublePoint::FullUpdateFromFile()
{
    ReadAttributes();
    firstPoint->RefreshGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p1id));
    secondPoint->RefreshGeometry(*VAbstractTool::data.GeometricObject<VPointF>(p2id));
    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::UpdateNamePosition()
{
    qApp->getUndoStack()->beginMacro("move labels");

    VPointF *p1 = VAbstractTool::data.GeometricObject<VPointF>(p1id).data();
    MoveLabel *moveLabel1 = new MoveLabel(doc, p1->mx(), p1->my(), p1id, this->scene());
    connect(moveLabel1, &MoveLabel::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveLabel1);

    VPointF *p2 = VAbstractTool::data.GeometricObject<VPointF>(p2id).data();
    MoveLabel *moveLabel2 = new MoveLabel(doc, p2->mx(), p2->my(), p2id, this->scene());
    connect(moveLabel2, &MoveLabel::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
    qApp->getUndoStack()->push(moveLabel2);

    qApp->getUndoStack()->endMacro();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::RefreshLine()
{
    firstPoint->RefreshLine();
    secondPoint->RefreshLine();
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

    return QGraphicsItem::itemChange(change, value);
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
            DeleteTool();
            return; //Leave this method immediately after call!!!
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
        SCASSERT(point.isNull() == false);

        doc->SetAttribute(tag, AttrName1, point->name());
        doc->SetAttribute(tag, AttrMx1, qApp->fromPixel(point->mx()));
        doc->SetAttribute(tag, AttrMy1, qApp->fromPixel(point->my()));

        VPointF *p = VAbstractTool::data.GeometricObject<VPointF>(p2id).data();

        doc->SetAttribute(tag, AttrName2, p->name());
        doc->SetAttribute(tag, AttrMx2, qApp->fromPixel(p->mx()));
        doc->SetAttribute(tag, AttrMy2, qApp->fromPixel(p->my()));
    }
    else if (obj->id() == p2id)
    {
        QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
        SCASSERT(point.isNull() == false);

        doc->SetAttribute(tag, AttrName2, point->name());
        doc->SetAttribute(tag, AttrMx2, qApp->fromPixel(point->mx()));
        doc->SetAttribute(tag, AttrMy2, qApp->fromPixel(point->my()));

        VPointF *p = VAbstractTool::data.GeometricObject<VPointF>(p1id).data();

        doc->SetAttribute(tag, AttrName1, p->name());
        doc->SetAttribute(tag, AttrMx1, qApp->fromPixel(p->mx()));
        doc->SetAttribute(tag, AttrMy1, qApp->fromPixel(p->my()));
    }
    else
    {
        VPointF *p1 = VAbstractTool::data.GeometricObject<VPointF>(p1id).data();
        VPointF *p2 = VAbstractTool::data.GeometricObject<VPointF>(p1id).data();

        doc->SetAttribute(tag, AttrName1, p1->name());
        doc->SetAttribute(tag, AttrMx1, qApp->fromPixel(p1->mx()));
        doc->SetAttribute(tag, AttrMy1, qApp->fromPixel(p1->my()));

        doc->SetAttribute(tag, AttrName2, p2->name());
        doc->SetAttribute(tag, AttrMx2, qApp->fromPixel(p2->mx()));
        doc->SetAttribute(tag, AttrMy2, qApp->fromPixel(p2->my()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetFakeGObject(id);
    SaveOptions(domElement, obj);
    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolDoublePoint::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        QSharedPointer<VGObject> obj = VAbstractTool::data.GetFakeGObject(id);
        SaveOptions(domElement, obj);
    }
    else
    {
        qCDebug(vTool, "Can't find tool with id = %u", id);
    }
}
