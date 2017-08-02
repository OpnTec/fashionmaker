/************************************************************************
 **
 **  @file   vnodesplinepath.cpp
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

#include "vnodesplinepath.h"

#include <QDomElement>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>

#include "../ifc/xml/vdomdocument.h"
#include "../ifc/ifcdef.h"
#include "../vabstracttool.h"
#include "../vdatatool.h"
#include "vabstractnode.h"

const QString VNodeSplinePath::ToolType = QStringLiteral("modelingPath");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VNodeSplinePath constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idSpline object id in containerSpline.
 * @param typeCreation way we create this tool.
 * @param idTool tool id.
 * @param qoParent QObject parent.
 */
VNodeSplinePath::VNodeSplinePath(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 idSpline,
                                 const Source &typeCreation, const QString &drawName, const quint32 &idTool,
                                 QObject *qoParent)
    :VAbstractNode(doc, data, id, idSpline, drawName, idTool, qoParent)
{
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idSpline object id in containerSpline.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 * @param idTool tool id.
 */
void VNodeSplinePath::Create(VAbstractPattern *doc, VContainer *data, quint32 id,
                             quint32 idSpline, const Document &parse, const Source &typeCreation,
                             const QString &drawName, const quint32 &idTool)
{
    if (parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(id, Tool::NodeSplinePath, doc);
        VNodeSplinePath *splPath = new VNodeSplinePath(doc, data, id, idSpline, typeCreation, drawName, idTool, doc);

        VAbstractPattern::AddTool(id, splPath);
        if (idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = VAbstractPattern::getTool(idTool);
            SCASSERT(tool != nullptr)
            splPath->setParent(tool);// Adopted by a tool
        }
        else
        {
            // Help to delete the node before each FullParse
            doc->AddToolOnRemove(splPath);
        }
    }
    else
    {
        doc->UpdateToolData(id, data);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VNodeSplinePath::getTagName() const
{
    return VAbstractPattern::TagSpline;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeSplinePath::AllowHover(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeSplinePath::AllowSelecting(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VNodeSplinePath::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
    if (idTool != 0)
    {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);
}
