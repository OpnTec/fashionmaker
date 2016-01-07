/************************************************************************
 **
 **  @file   vabstractnode.cpp
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

#include "vabstractnode.h"
#include "../../undocommands/adddetnode.h"

#include <QDebug>

const QString VAbstractNode::AttrIdObject = QStringLiteral("idObject");
const QString VAbstractNode::AttrIdTool = QStringLiteral("idTool");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractNode constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idNode object id in containerNode.
 * @param idTool id tool.
 * @param parent parent object.
 */
VAbstractNode::VAbstractNode(VAbstractPattern *doc, VContainer *data, const quint32 &id, const quint32 &idNode,
                             const quint32 &idTool, QObject *parent)
    : VAbstractTool(doc, data, id, parent), parentType(ParentType::Item), idNode(idNode), idTool(idTool),
      currentColor(Qt::black)
{
    _referens = 0;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::ShowVisualization(bool show)
{
    Q_UNUSED(show)
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::incrementReferens()
{
    ++_referens;
    if (_referens == 1)
    {
        idTool != NULL_ID ? doc->IncrementReferens(idTool) : doc->IncrementReferens(idNode);
        ShowNode();
        QDomElement domElement = doc->elementById(id);
        if (domElement.isElement())
        {
            doc->SetParametrUsage(domElement, AttrInUse, NodeUsage::InUse);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief decrementReferens decrement reference for all parent objects.
 */
void VAbstractNode::decrementReferens()
{
    if (_referens > 0)
    {
        --_referens;
    }
    if (_referens == 0)
    {
        idTool != NULL_ID ? doc->DecrementReferens(idTool) : doc->DecrementReferens(idNode);
        HideNode();
        QDomElement domElement = doc->elementById(id);
        if (domElement.isElement())
        {
            doc->SetParametrUsage(domElement, AttrInUse, NodeUsage::NotInUse);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
ParentType VAbstractNode::GetParentType() const
{
    return parentType;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::SetParentType(const ParentType &value)
{
    parentType = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToModeling add tag to modeling tag current pattern peace.
 * @param domElement tag.
 */
void VAbstractNode::AddToModeling(const QDomElement &domElement)
{
    AddDetNode *addNode = new AddDetNode(domElement, doc);
    qApp->getUndoStack()->push(addNode);
}
