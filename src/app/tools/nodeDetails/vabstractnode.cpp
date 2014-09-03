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

#include "vabstractnode.h"
#include <QDebug>
#include "../../xml/vpattern.h"
#include "../../undocommands/adddetnode.h"
#include "../../widgets/vapplication.h"

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
VAbstractNode::VAbstractNode(VPattern *doc, VContainer *data, const quint32 &id, const quint32 &idNode,
                             const quint32 &idTool, QObject *parent)
    : VAbstractTool(doc, data, id, parent), idNode(idNode), idTool(idTool)
{
    _referens = 0;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::DeleteNode()
{
    if (_referens <= 1)
    {
        RemoveReferens();//deincrement referens
    }
}

void VAbstractNode::RestoreNode()
{
    if (_referens <= 1)
    {
        RestoreReferens();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::ShowVisualization(bool show)
{
    Q_UNUSED(show)
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
    if (_referens <= 0)
    {
        doc->DecrementReferens(idNode);
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            QDomNode element = domElement.parentNode();
            if (element.isNull() == false)
            {
                element.removeChild(domElement);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::RemoveReferens()
{
    if (idTool != 0)
    {
        doc->DecrementReferens(idTool);
    }
    else
    {
        doc->DecrementReferens(idNode);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::RestoreReferens()
{
    if (idTool != 0)
    {
        doc->IncrementReferens(idTool);
    }
    else
    {
        doc->IncrementReferens(idNode);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{

}
