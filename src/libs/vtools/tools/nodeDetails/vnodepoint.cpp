/************************************************************************
 **
 **  @file   vnodepoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vabstracttool.h"
#include "../vdatatool.h"
#include "vabstractnode.h"

const QString VNodePoint::ToolType = QStringLiteral("modeling");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VNodePoint constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idPoint object id in containerPoint.
 * @param typeCreation way we create this tool.
 * @param idTool tool id.
 * @param qoParent QObject parent
 * @param parent parent object.
 */
VNodePoint::VNodePoint(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 idPoint, const Source &typeCreation,
                       const QString &drawName, const quint32 &idTool, QObject *qoParent, QGraphicsItem *parent)
    :VAbstractNode(doc, data, id, idPoint, drawName, idTool, qoParent), QGraphicsEllipseItem(parent), radius(0),
      namePoint(nullptr), lineName(nullptr)
{
    radius = ToPixel(DefPointRadius/*mm*/, Unit::Mm);
    namePoint = new VGraphicsSimpleTextItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::PointChoosed, this, &VNodePoint::PointChoosed);
    lineName = new QGraphicsLineItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VNodePoint::NameChangePosition);
    connect(namePoint, &VGraphicsSimpleTextItem::ShowContextMenu,
            RECEIVER(this)[this](QGraphicsSceneContextMenuEvent *event)
    {
        emit ShowContextMenu(event);
    });
    this->setPen(QPen(Qt::black, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idPoint object id in containerPoint.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @param idTool tool id.
 */
void VNodePoint::Create(VAbstractPattern *doc, VContainer *data, VMainGraphicsScene *scene,
                        quint32 id, quint32 idPoint, const Document &parse,
                        const Source &typeCreation, const QString &drawName, const quint32 &idTool)
{
    if (parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(id, Tool::NodePoint, doc);
        //TODO Need create garbage collector and remove all nodes, what we don't use.
        //Better check garbage before each saving file. Check only modeling tags.
        VNodePoint *point = new VNodePoint(doc, data, id, idPoint, typeCreation, drawName, idTool, doc);

        connect(scene, &VMainGraphicsScene::EnableToolMove, point, &VNodePoint::EnableToolMove);
        connect(scene, &VMainGraphicsScene::EnablePointItemHover, point, &VNodePoint::AllowHover);
        connect(scene, &VMainGraphicsScene::EnablePointItemSelection, point, &VNodePoint::AllowSelecting);
        connect(scene, &VMainGraphicsScene::EnableLabelItemHover, point, &VNodePoint::AllowLabelHover);
        connect(scene, &VMainGraphicsScene::EnableLabelItemSelection, point, &VNodePoint::AllowLabelSelecting);
        doc->AddTool(id, point);
        if (idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = doc->getTool(idTool);
            SCASSERT(tool != nullptr)
            point->setParent(tool);// Adopted by a tool
        }
        else
        {
            // Try to prevent memory leak
            scene->addItem(point);// First adopted by scene
            point->hide();// If no one will use node, it will stay hidden
            point->SetParentType(ParentType::Scene);
        }
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VNodePoint::getTagName() const
{
    return VAbstractPattern::TagPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::PointChoosed()
{
    emit ChoosedTool(id, SceneObject::Point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VNodePoint::FullUpdateFromFile()
{
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<VPointF>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VNodePoint::AddToFile()
{
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    QDomElement domElement = doc->createElement(getTagName());

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
    if (idTool != NULL_ID)
    {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VNodePoint::RefreshDataInFile()
{
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrIdObject, idNode);
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        if (idTool != NULL_ID)
        {
            doc->SetAttribute(domElement, AttrIdTool, idTool);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Special for not selectable item first need to call standard mousePressEvent then accept event
    QGraphicsEllipseItem::mousePressEvent(event);

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
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, SceneObject::Point);
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverMoveEvent handle hover move events.
 * @param event hover move event.
 */
void VNodePoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    this->setPen(QPen(currentColor, qApp->toPixel(WidthMainLine(*VAbstractTool::data.GetPatternUnit()))));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VNodePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    this->setPen(QPen(currentColor, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NameChangePosition label change position.
 * @param pos new position.
 */
void VNodePoint::NameChangePosition(const QPointF &pos)
{
    VPointF *point = new VPointF(*VAbstractTool::data.GeometricObject<VPointF>(id));
    QPointF p = pos - this->pos();
    point->setMx(p.x());
    point->setMy(p.y());
    RefreshLine();
    UpdateNamePosition(point->mx(), point->my());
    VAbstractTool::data.UpdateGObject(id, point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateNamePosition update label position in file.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 */
void VNodePoint::UpdateNamePosition(qreal mx, qreal my)
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrMx, QString().setNum(qApp->fromPixel(mx)));
        doc->SetAttribute(domElement, AttrMy, QString().setNum(qApp->fromPixel(my)));
        emit toolhaveChange();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshPointGeometry refresh point on scene.
 * @param point point position.
 */
void VNodePoint::RefreshPointGeometry(const VPointF &point)
{
    QRectF rec = QRectF(0, 0, radius*2, radius*2);
    rec.translate(-rec.center().x(), -rec.center().y());
    this->setRect(rec);
    this->setPos(point);

    namePoint->blockSignals(true);
    namePoint->setText(point.name());
    namePoint->setPos(QPointF(point.mx(), point.my()));
    namePoint->blockSignals(false);

    RefreshLine();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshLine refresh label line on scene.
 */
void VNodePoint::RefreshLine()
{
    VAbstractTool::RefreshLine(this, namePoint, lineName, radius);
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
void VNodePoint::EnableToolMove(bool move)
{
    namePoint->setFlag(QGraphicsItem::ItemIsMovable, move);
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
    namePoint->setAcceptHoverEvents(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::AllowLabelSelecting(bool enabled)
{
    namePoint->setFlag(QGraphicsItem::ItemIsSelectable, enabled);
}
