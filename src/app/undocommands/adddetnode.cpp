/************************************************************************
 **
 **  @file   adddetnode.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 6, 2014
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

#include "adddetnode.h"
#include "../xml/vpattern.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(vUndoAddDetailNode, "v.undo.add.detail.node")

//---------------------------------------------------------------------------------------------------------------------
AddDetNode::AddDetNode(const QDomElement &xml, VPattern *doc, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent)
{
    setText(QObject::tr("Add node"));
    nodeId = doc->GetParametrId(xml);
}

//---------------------------------------------------------------------------------------------------------------------
AddDetNode::~AddDetNode()
{}

//---------------------------------------------------------------------------------------------------------------------
void AddDetNode::undo()
{
    qCDebug(vUndoAddDetailNode)<<"Undo.";

    QDomElement modelingElement;
    if (doc->GetActivNodeElement(VPattern::TagModeling, modelingElement))
    {
        QDomElement domElement = doc->elementById(QString().setNum(nodeId));
        if (domElement.isElement())
        {
            if (modelingElement.removeChild(domElement).isNull())
            {
               qCDebug(vUndoAddDetailNode)<<"Can't delete node.";
                return;
            }
        }
        else
        {
            qCDebug(vUndoAddDetailNode)<<"Can't get node by id = "<<nodeId<<".";
            return;
        }
    }
    else
    {
        qCDebug(vUndoAddDetailNode)<<"Can't find tag"<<VPattern::TagModeling<<".";
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AddDetNode::redo()
{
    qCDebug(vUndoAddDetailNode)<<"Redo.";

    QDomElement modelingElement;
    if (doc->GetActivNodeElement(VPattern::TagModeling, modelingElement))
    {
        modelingElement.appendChild(xml);
    }
    else
    {
        qCDebug(vUndoAddDetailNode)<<"Can't find tag"<<VPattern::TagModeling<<".";
        return;
    }
}
