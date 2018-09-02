/************************************************************************
 **
 **  @file   vnodepoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vnodepoint.h"

#include <QBrush>
#include <QDomElement>
#include <QFlags>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QIcon>
#include <QMenu>
#include <QPen>
#include <QPoint>
#include <QRectF>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <Qt>
#include <new>

#include "../../../vgeometry/vpointf.h"
#include "../../../vwidgets/vgraphicssimpletextitem.h"
#include "../ifc/xml/vdomdocument.h"
#include "../ifc/ifcdef.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../vabstracttool.h"
#include "../vdatatool.h"
#include "vabstractnode.h"
#include "../../undocommands/label/movelabel.h"
#include "../../undocommands/label/showlabel.h"
#include "../vtoolseamallowance.h"

const QString VNodePoint::ToolType = QStringLiteral("modeling");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VNodePoint constructor.
 * @param initData init data.
 * @param parent parent object.
 */
VNodePoint::VNodePoint(const VAbstractNodeInitData &initData, QObject *qoParent, QGraphicsItem *parent)
    : VAbstractNode(initData.doc, initData.data, initData.id, initData.idObject, initData.drawName, initData.idTool,
                    qoParent),
      VScenePoint(parent)
{
    connect(m_namePoint, &VGraphicsSimpleTextItem::PointChoosed, this, &VNodePoint::PointChoosed);
    connect(m_namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this, &VNodePoint::NameChangePosition);
    connect(m_namePoint, &VGraphicsSimpleTextItem::ShowContextMenu,
            this, [this](QGraphicsSceneContextMenuEvent *event)
    {
        contextMenuEvent(event);
    });
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(initData.id));
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param initData init data.
 */
void VNodePoint::Create(const VAbstractNodeInitData &initData)
{
    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::NodePoint, initData.doc);
        //TODO Need create garbage collector and remove all nodes, what we don't use.
        //Better check garbage before each saving file. Check only modeling tags.
        VNodePoint *point = new VNodePoint(initData);

        connect(initData.scene, &VMainGraphicsScene::EnableToolMove, point, &VNodePoint::EnableToolMove);
        connect(initData.scene, &VMainGraphicsScene::EnablePointItemHover, point, &VNodePoint::AllowHover);
        connect(initData.scene, &VMainGraphicsScene::EnablePointItemSelection, point, &VNodePoint::AllowSelecting);
        connect(initData.scene, &VMainGraphicsScene::EnableLabelItemHover, point, &VNodePoint::AllowLabelHover);
        connect(initData.scene, &VMainGraphicsScene::EnableLabelItemSelection, point, &VNodePoint::AllowLabelSelecting);
        VAbstractPattern::AddTool(initData.id, point);
        if (initData.idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = VAbstractPattern::getTool(initData.idTool);
            SCASSERT(tool != nullptr)
            point->setParent(tool);// Adopted by a tool
        }
        else
        {
            // Try to prevent memory leak
            initData.scene->addItem(point);// First adopted by scene
            point->hide();// If no one will use node, it will stay hidden
            point->SetParentType(ParentType::Scene);
        }
    }
    else
    {
        initData.doc->UpdateToolData(initData.id, initData.data);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VNodePoint::getTagName() const
{
    return VAbstractPattern::TagPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::ChangeLabelPosition(quint32 id, const QPointF &pos)
{
    if (id == m_id)
    {
        QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
        point->setMx(pos.x());
        point->setMy(pos.y());
        m_namePoint->blockSignals(true);
        m_namePoint->setPos(pos);
        m_namePoint->blockSignals(false);
        RefreshLine();
        if (QGraphicsScene *sc = scene())
        {
            VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), this);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::SetLabelVisible(quint32 id, bool visible)
{
    if (m_id == id)
    {
        const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
        point->SetShowLabel(visible);
        RefreshPointGeometry(*point);
        if (QGraphicsScene *sc = scene())
        {
            VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), this);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::PointChoosed()
{
    emit ChoosedTool(m_id, SceneObject::Point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VNodePoint::FullUpdateFromFile()
{
    try
    {
        RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(m_id));
    }
    catch (const VExceptionBadId &)
    {
        // ignore
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VNodePoint::AddToFile()
{
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(m_id);
    QDomElement domElement = doc->createElement(getTagName());

    doc->SetAttribute(domElement, VDomDocument::AttrId, m_id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
    doc->SetAttribute<bool>(domElement, AttrShowLabel, point->IsShowLabel());
    if (idTool != NULL_ID)
    {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Special for not selectable item first need to call standard mousePressEvent then accept event
    VScenePoint::mousePressEvent(event);

    // Somehow clicking on notselectable object do not clean previous selections.
    if (not (flags() & ItemIsSelectable) && scene())
    {
        scene()->clearSelection();
    }

    event->accept();// Special for not selectable item first need to call standard mousePressEvent then accept event
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseReleaseEvent handle mouse release events.
 * @param event mouse release event.
 */
void VNodePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (IsSelectedByReleaseEvent(this, event))
    {
        emit ChoosedTool(m_id, SceneObject::Point);
    }
    VScenePoint::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NameChangePosition label change position.
 * @param pos new position.
 */
void VNodePoint::NameChangePosition(const QPointF &pos)
{
    qApp->getUndoStack()->push(new MoveLabel(doc, pos - this->pos(), m_id));
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::ShowNode()
{
    if (parentType != ParentType::Scene && not m_exluded)
    {
        show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::HideNode()
{
    hide();
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (m_suppressContextMenu)
    {
        return;
    }

    if (VToolSeamAllowance *piece = qgraphicsitem_cast<VToolSeamAllowance *>(parentItem()))
    {
        QMenu menu;
        QAction *actionShowLabel = menu.addAction(tr("Show label"));
        actionShowLabel->setCheckable(true);
        actionShowLabel->setChecked(VAbstractTool::data.GeometricObject<VPointF>(m_id)->IsShowLabel());

        QAction *actionExclude = menu.addAction(tr("Exclude"));

        QMenu *angleTypeMenu = menu.addMenu(tr("Angle"));
        PieceNodeAngle curType = PieceNodeAngle::ByLength;

        const VPiece detail = VAbstractTool::data.GetPiece(piece->getId());
        const int nodeIndex = detail.GetPath().indexOfNode(m_id);
        if (nodeIndex != -1)
        {
            const VPieceNode &node = detail.GetPath().at(nodeIndex);
            curType = node.GetAngleType();
        }
        else
        {
            angleTypeMenu->setVisible(false);
        }

        auto InitAngleAction = [angleTypeMenu, curType](const QString &name, PieceNodeAngle checkType)
        {
            QAction *action = angleTypeMenu->addAction(name);
            action->setCheckable(true);
            action->setChecked(curType == checkType);
            return action;
        };

        QAction *actionByLength = InitAngleAction(tr("by length"), PieceNodeAngle::ByLength);
        QAction *actionByPointsIntersection = InitAngleAction(tr("by points intersetions"),
                                                              PieceNodeAngle::ByPointsIntersection);
        QAction *actionByFirstEdgeSymmetry = InitAngleAction(tr("by first edge symmetry"),
                                                             PieceNodeAngle::ByFirstEdgeSymmetry);
        QAction *actionBySecondEdgeSymmetry = InitAngleAction(tr("by second edge symmetry"),
                                                              PieceNodeAngle::BySecondEdgeSymmetry);
        QAction *actionByFirstEdgeRightAngle = InitAngleAction(tr("by first edge right angle"),
                                                               PieceNodeAngle::ByFirstEdgeRightAngle);
        QAction *actionBySecondEdgeRightAngle = InitAngleAction(tr("by second edge right angle"),
                                                                PieceNodeAngle::BySecondEdgeRightAngle);

        QAction *separatorAct = new QAction(this);
        separatorAct->setSeparator(true);
        menu.addAction(separatorAct);

        QAction *actionOption = menu.addAction(QIcon::fromTheme("preferences-other"), tr("Options"));

        QAction *inLayoutOption = menu.addAction(tr("In layout"));
        inLayoutOption->setCheckable(true);
        inLayoutOption->setChecked(detail.IsInLayout());

        QAction *forbidFlippingOption = menu.addAction(tr("Forbid flipping"));
        forbidFlippingOption->setCheckable(true);
        forbidFlippingOption->setChecked(detail.IsForbidFlipping());

        QAction *forceFlippingOption = menu.addAction(tr("Force flipping"));
        forceFlippingOption->setCheckable(true);
        forceFlippingOption->setChecked(detail.IsForceFlipping());

        QAction *actionRemove = menu.addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));
        piece->referens() > 1 ? actionRemove->setEnabled(false) : actionRemove->setEnabled(true);

        QAction *selectedAction = menu.exec(event->screenPos());
        if (selectedAction == actionOption)
        {
            emit ShowOptions();
        }
        else if (selectedAction == inLayoutOption)
        {
            emit ToggleInLayout(selectedAction->isChecked());
        }
        else if (selectedAction == forbidFlippingOption)
        {
            emit ToggleForbidFlipping(selectedAction->isChecked());
        }
        else if (selectedAction == forceFlippingOption)
        {
            emit ToggleForceFlipping(selectedAction->isChecked());
        }
        else if (selectedAction == actionRemove)
        {
            try
            {
                emit Delete();
            }
            catch(const VExceptionToolWasDeleted &e)
            {
                Q_UNUSED(e);
                return;//Leave this method immediately!!!
            }
            //Leave this method immediately after call!!!
        }
        else if (selectedAction == actionShowLabel)
        {
            qApp->getUndoStack()->push(new ShowLabel(doc, m_id, selectedAction->isChecked()));
        }
        else if (selectedAction == actionExclude)
        {
            emit ToggleExcludeState(m_id);
        }
        else if (selectedAction == actionByLength)
        {
            if (curType != PieceNodeAngle::ByLength)
            {
                emit ToggleAngleType(m_id, PieceNodeAngle::ByLength);
            }
        }
        else if (selectedAction == actionByPointsIntersection)
        {
            if (curType != PieceNodeAngle::ByPointsIntersection)
            {
                emit ToggleAngleType(m_id, PieceNodeAngle::ByPointsIntersection);
            }
        }
        else if (selectedAction == actionByFirstEdgeSymmetry)
        {
            if (curType != PieceNodeAngle::ByFirstEdgeSymmetry)
            {
                emit ToggleAngleType(m_id, PieceNodeAngle::ByFirstEdgeSymmetry);
            }
        }
        else if (selectedAction == actionBySecondEdgeSymmetry)
        {
            if (curType != PieceNodeAngle::BySecondEdgeSymmetry)
            {
                emit ToggleAngleType(m_id, PieceNodeAngle::BySecondEdgeSymmetry);
            }
        }
        else if (selectedAction == actionByFirstEdgeRightAngle)
        {
            if (curType != PieceNodeAngle::ByFirstEdgeRightAngle)
            {
                emit ToggleAngleType(m_id, PieceNodeAngle::ByFirstEdgeRightAngle);
            }
        }
        else if (selectedAction == actionBySecondEdgeRightAngle)
        {
            if (curType != PieceNodeAngle::BySecondEdgeRightAngle)
            {
                emit ToggleAngleType(m_id, PieceNodeAngle::BySecondEdgeRightAngle);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::EnableToolMove(bool move)
{
    m_namePoint->setFlag(QGraphicsItem::ItemIsMovable, move);
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::AllowHover(bool enabled)
{
    setAcceptHoverEvents(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::AllowSelecting(bool enabled)
{
    setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::AllowLabelHover(bool enabled)
{
    m_namePoint->setAcceptHoverEvents(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::AllowLabelSelecting(bool enabled)
{
    m_namePoint->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}
