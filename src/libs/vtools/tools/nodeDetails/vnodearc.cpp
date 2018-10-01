/************************************************************************
 **
 **  @file   vnodearc.cpp
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

#include "vnodearc.h"

#include <QDomElement>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>

#include "../ifc/xml/vdomdocument.h"
#include "../ifc/ifcdef.h"
#include "../vabstracttool.h"
#include "../vdatatool.h"
#include "vabstractnode.h"

const QString VNodeArc::ToolType = QStringLiteral("modeling");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VNodeArc constructor.
 * @param initData init data.
 * @param qoParent QObject parent
 */
VNodeArc::VNodeArc(const VAbstractNodeInitData &initData, QObject *qoParent)
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
void VNodeArc::Create(const VAbstractNodeInitData &initData)
{
    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(initData.id, Tool::NodeArc, initData.doc);
        VNodeArc *arc = new VNodeArc(initData);

        VAbstractPattern::AddTool(initData.id, arc);
        if (initData.idTool != NULL_ID)
        {
            //Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool *tool = VAbstractPattern::getTool(initData.idTool);
            SCASSERT(tool != nullptr)
            arc->setParent(tool);// Adopted by a tool
        }
        else
        {
            // Help to delete the node before each FullParse
            initData.doc->AddToolOnRemove(arc);
        }
    }
    else
    {
        initData.doc->UpdateToolData(initData.id, initData.data);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VNodeArc::getTagName() const
{
    return VAbstractPattern::TagArc;
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeArc::AllowHover(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeArc::AllowSelecting(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VNodeArc::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());

    doc->SetAttribute(domElement, VDomDocument::AttrId, m_id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
    if (idTool != NULL_ID)
    {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);
}
