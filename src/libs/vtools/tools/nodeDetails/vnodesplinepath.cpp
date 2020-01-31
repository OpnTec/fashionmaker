/************************************************************************
 **
 **  @file   vnodesplinepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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
 * @param initData init data.
 * @param qoParent QObject parent.
 */
VNodeSplinePath::VNodeSplinePath(const VAbstractNodeInitData &initData, QObject *qoParent)
    :VAbstractNode(initData.doc, initData.data, initData.id, initData.idObject, initData.drawName, initData.idTool,
                   qoParent)
{
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param initData init data.
 */
void VNodeSplinePath::Create(const VAbstractNodeInitData &initData)
{
    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::NodeSplinePath, initData.doc);
        VNodeSplinePath *splPath = new VNodeSplinePath(initData);

        VAbstractPattern::AddTool(initData.id, splPath);
        if (initData.idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = VAbstractPattern::getTool(initData.idTool);
            SCASSERT(tool != nullptr)
            splPath->setParent(tool);// Adopted by a tool
        }
        else
        {
            // Help to delete the node before each FullParse
            initData.doc->AddToolOnRemove(splPath);
        }
    }
    else
    {
        initData.doc->UpdateToolData(initData.id, initData.data);
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

    doc->SetAttribute(domElement, VDomDocument::AttrId, m_id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
    if (idTool != 0)
    {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);
}
