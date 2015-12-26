/************************************************************************
 **
 **  @file   vnodearc.cpp
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

#include "vnodearc.h"
#include "../../../vgeometry/varc.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>

const QString VNodeArc::TagName = QStringLiteral("arc");
const QString VNodeArc::ToolType = QStringLiteral("modeling");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VNodeArc constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idArc object id in containerArc.
 * @param typeCreation way we create this tool.
 * @param idTool tool id.
 * @param qoParent QObject parent
 * @param parent parent object.
 */
VNodeArc::VNodeArc(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 idArc, const Source &typeCreation,
                   const quint32 &idTool, QObject *qoParent, QGraphicsItem *parent)
    :VAbstractNode(doc, data, id, idArc, idTool, qoParent), QGraphicsPathItem(parent)
{
    RefreshGeometry();
    this->setPen(QPen(baseColor, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))));

    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idArc object id in containerArc.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @param idTool tool id.
 * @param parent QObject parent
 */
void VNodeArc::Create(VAbstractPattern *doc, VContainer *data, VMainGraphicsScene *scene, quint32 id, quint32 idArc,
                      const Document &parse, const Source &typeCreation, const quint32 &idTool, QObject *parent)
{
    VAbstractTool::AddRecord(id, Tool::NodeArc, doc);
    if (parse == Document::FullParse)
    {
        VNodeArc *arc = new VNodeArc(doc, data, id, idArc, typeCreation, idTool, parent);

        // Try to prevent memory leak
        arc->hide();// If no one will use node, it will stay hidden
        scene->addItem(arc);// First adopted by scene

        doc->AddTool(id, arc);
        if (idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = doc->getTool(idTool);
            SCASSERT(tool != nullptr);
            arc->setParent(tool);// Adopted by a tool
        }
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VNodeArc::getTagName() const
{
    return VNodeArc::TagName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VNodeArc::FullUpdateFromFile()
{
    RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VNodeArc::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
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
void VNodeArc::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrIdObject, idNode);
        if (idTool != NULL_ID)
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
void VNodeArc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit ChoosedTool(id, SceneObject::Arc);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverMoveEvent handle hover move events.
 * @param event hover move event.
 */
// cppcheck-suppress unusedFunction
void VNodeArc::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(WidthMainLine(*VAbstractTool::data.GetPatternUnit()))));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverLeaveEvent handle hover leave events.
 * @param event hover leave event.
 */
void VNodeArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    this->setPen(QPen(currentColor, qApp->toPixel(WidthHairLine(*VAbstractTool::data.GetPatternUnit()))));
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeArc::ShowNode()
{
    show();
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeArc::HideNode()
{
    hide();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshGeometry refresh item on scene.
 */
void VNodeArc::RefreshGeometry()
{
//    const QSharedPointer<VArc> arc = VAbstractTool::data.GeometricObject<VArc>(id);
//    QPainterPath path;
//    path.addPath(arc->GetPath());
//    path.setFillRule( Qt::WindingFill );
//    this->setPath(path);
}
