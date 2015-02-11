/************************************************************************
 **
 **  @file   deletedetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "deletedetail.h"
#include "../xml/vpattern.h"
#include "../tools/vtooldetail.h"

//---------------------------------------------------------------------------------------------------------------------
DeleteDetail::DeleteDetail(VPattern *doc, quint32 id, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), parentNode(QDomNode()), siblingId(NULL_ID)
{
    setText(tr("Delete tool"));
    nodeId = id;
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        xml = domElement.cloneNode().toElement();
        parentNode = domElement.parentNode();
        QDomNode previousDetail = domElement.previousSibling();
        if (previousDetail.isNull())
        {
            siblingId = NULL_ID;
        }
        else
        {
            // Better save id of previous detail instead of reference to node.
            siblingId = doc->GetParametrUInt(previousDetail.toElement(), VPattern::AttrId, NULL_ID_STR);
        }
    }
    else
    {
        qCDebug(vUndo)<<"Can't get detail by id ="<<nodeId<<".";
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
DeleteDetail::~DeleteDetail()
{}

//---------------------------------------------------------------------------------------------------------------------
void DeleteDetail::undo()
{
    qCDebug(vUndo)<<"Undo.";

    UndoDeleteAfterSibling(parentNode, siblingId);
    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
void DeleteDetail::redo()
{
    qCDebug(vUndo)<<"Redo.";

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        parentNode.removeChild(domElement);

        // UnionDetails delete two old details and create one new.
        // So when UnionDetail delete detail we can't use FullParsing. So we hide detail on scene directly.
        QHash<quint32, VDataTool*>* tools = doc->getTools();
        SCASSERT(tools != nullptr);
        VToolDetail *toolDet = qobject_cast<VToolDetail*>(tools->value(nodeId));
        SCASSERT(toolDet != nullptr);
        toolDet->hide();

        emit NeedFullParsing(); // Doesn't work when UnionDetail delete detail.
    }
    else
    {
        qCDebug(vUndo)<<"Can't get detail by id = "<<nodeId<<".";
        return;
    }
}
