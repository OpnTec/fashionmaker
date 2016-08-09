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

#include "adddetnode.h"

#include <QByteArray>
#include <QDomNode>

#include "../vmisc/logging.h"
#include "../ifc/xml/vabstractpattern.h"
#include "vundocommand.h"

class QDomElement;
class QUndoCommand;

//---------------------------------------------------------------------------------------------------------------------
AddDetNode::AddDetNode(const QDomElement &xml, VAbstractPattern *doc, const QString &drawName, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), m_drawName(drawName)
{
    setText(QObject::tr("add node"));
    nodeId = doc->GetParametrId(xml);
}

//---------------------------------------------------------------------------------------------------------------------
AddDetNode::~AddDetNode()
{}

//---------------------------------------------------------------------------------------------------------------------
void AddDetNode::undo()
{
    qCDebug(vUndo, "Undo.");

    QDomElement modeling = GetModelingSection();
    if (not modeling.isNull())
    {
        QDomElement domElement = doc->elementById(nodeId);
        if (domElement.isElement())
        {
            if (modeling.removeChild(domElement).isNull())
            {
                qCDebug(vUndo, "Can't delete node.");
                return;
            }
        }
        else
        {
            qCDebug(vUndo, "Can't get node by id = %u.", nodeId);
            return;
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find tag %s.", qUtf8Printable(VAbstractPattern::TagModeling));
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AddDetNode::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement modeling = GetModelingSection();
    if (not modeling.isNull())
    {
        modeling.appendChild(xml);
    }
    else
    {
        qCDebug(vUndo, "Can't find tag %s.", qUtf8Printable(VAbstractPattern::TagModeling));
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement AddDetNode::GetModelingSection() const
{
    QDomElement modeling;
    if (m_drawName.isEmpty())
    {
        doc->GetActivNodeElement(VAbstractPattern::TagModeling, modeling);
    }
    else
    {
        modeling = doc->GetDraw(m_drawName).firstChildElement(VAbstractPattern::TagModeling);
    }
    return modeling;
}
