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

#include "vnodepoint.h"

#include "../../widgets/vapplication.h"
#include "../../geometry/vpointf.h"
#include "../../visualization/vgraphicssimpletextitem.h"
#include <QPen>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>

const QString VNodePoint::TagName = QStringLiteral("point");
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
VNodePoint::VNodePoint(VPattern *doc, VContainer *data, quint32 id, quint32 idPoint, const Source &typeCreation,
                       const quint32 &idTool, QObject *qoParent, QGraphicsItem *parent)
    :VAbstractNode(doc, data, id, idPoint, idTool, qoParent), QGraphicsEllipseItem(parent), radius(0),
      namePoint(nullptr), lineName(nullptr)
{
    radius = (1.5/*mm*/ / 25.4) * VApplication::PrintDPI;
    namePoint = new VGraphicsSimpleTextItem(this);
    lineName = new QGraphicsLineItem(this);
    connect(namePoint, &VGraphicsSimpleTextItem::NameChangePosition, this,
            &VNodePoint::NameChangePosition);
    this->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine())));
    this->setBrush(QBrush(Qt::NoBrush));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
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
 * @param parent QObject parent
 */
void VNodePoint::Create(VPattern *doc, VContainer *data, quint32 id, quint32 idPoint, const Document &parse,
                        const Source &typeCreation, const quint32 &idTool, QObject *parent)
{
    VAbstractTool::AddRecord(id, Tool::NodePoint, doc);
    if (parse == Document::FullParse)
    {
        //TODO Need create garbage collector and remove all nodes, what we don't use.
        //Better check garbage before each saving file. Check only modeling tags.
        VNodePoint *point = new VNodePoint(doc, data, id, idPoint, typeCreation, idTool, parent);
        doc->AddTool(id, point);
        if (idTool != 0)
        {
            doc->IncrementReferens(idTool);
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = doc->getTool(idTool);
            SCASSERT(tool != nullptr);
            point->setParent(tool);
        }
        else
        {
            doc->IncrementReferens(idPoint);
        }
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DeleteNode delete node from detail.
 */
void VNodePoint::DeleteNode()
{
    VAbstractNode::DeleteNode();
    this->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VNodePoint::RestoreNode()
{
    if (this->isVisible() == false)
    {
        VAbstractNode::RestoreNode();
        this->setVisible(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VNodePoint::FullUpdateFromFile()
{
    RefreshPointGeometry(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VNodePoint::AddToFile()
{
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
    doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
    if (idTool != 0)
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
    const VPointF *point = VAbstractTool::data.GeometricObject<const VPointF *>(id);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrIdObject, idNode);
        doc->SetAttribute(domElement, AttrMx, qApp->fromPixel(point->mx()));
        doc->SetAttribute(domElement, AttrMy, qApp->fromPixel(point->my()));
        if (idTool != 0)
        {
            doc->SetAttribute(domElement, AttrIdTool, idTool);
        }
    }
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
    QGraphicsItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverMoveEvent handle hover move events.
 * @param event hover move event.
 */
void VNodePoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthMainLine())));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VNodePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NameChangePosition label change position.
 * @param pos new position.
 */
void VNodePoint::NameChangePosition(const QPointF &pos)
{
    VPointF *point = new VPointF(*VAbstractTool::data.GeometricObject<const VPointF *>(id));
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
    QDomElement domElement = doc->elementById(QString().setNum(id));
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
    this->setPos(point.toQPointF());

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
    QRectF nameRec = namePoint->sceneBoundingRect();
    QPointF p1, p2;
    LineIntersectCircle(QPointF(), radius, QLineF(QPointF(), nameRec.center()- scenePos()), p1, p2);
    QPointF pRec = LineIntersectRect(nameRec, QLineF(scenePos(), nameRec.center()));
    lineName->setLine(QLineF(p1, pRec - scenePos()));
    if (QLineF(p1, pRec - scenePos()).length() <= qApp->toPixel(4))
    {
        lineName->setVisible(false);
    }
    else
    {
        lineName->setVisible(true);
    }
}
