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
                   const quint32 &idTool, QObject *qoParent)
    :VAbstractNode(doc, data, id, idArc, idTool, qoParent)
{
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
void VNodeArc::Create(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 idArc,
                      const Document &parse, const Source &typeCreation, const quint32 &idTool)
{
    VAbstractTool::AddRecord(id, Tool::NodeArc, doc);
    if (parse == Document::FullParse)
    {
        VNodeArc *arc = new VNodeArc(doc, data, id, idArc, typeCreation, idTool, doc);

        doc->AddTool(id, arc);
        if (idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = doc->getTool(idTool);
            SCASSERT(tool != nullptr);
            arc->setParent(tool);// Adopted by a tool
        }
        else
        {
            // Help to delete the node before each FullParse
            doc->AddToolOnRemove(arc);
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
