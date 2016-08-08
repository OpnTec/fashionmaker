/************************************************************************
 **
 **  @file   vundocommand.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 7, 2014
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

#include "vundocommand.h"

#include <QDomNode>

#include "../vgeometry/../ifc/ifcdef.h"
#include "../vwidgets/../vmisc/def.h"
#include "vnodedetail.h"

class QDomElement;
class QDomNode;
class QUndoCommand;

Q_LOGGING_CATEGORY(vUndo, "v.undo")

//---------------------------------------------------------------------------------------------------------------------
VUndoCommand::VUndoCommand(const QDomElement &xml, VAbstractPattern *doc, QUndoCommand *parent)
    :QObject(), QUndoCommand(parent), xml(xml), doc(doc), nodeId(NULL_ID), redoFlag(false)
{
    SCASSERT(doc != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
VUndoCommand::~VUndoCommand()
{}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::RedoFullParsing()
{
    if (redoFlag)
    {
        emit NeedFullParsing();
    }
    redoFlag = true;
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::UndoDeleteAfterSibling(QDomNode &parentNode, const quint32 &siblingId) const
{
    if (siblingId == NULL_ID)
    {
        parentNode.appendChild(xml);
    }
    else
    {
        const QDomElement refElement = doc->NodeById(siblingId);
        parentNode.insertAfter(xml, refElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::IncrementReferences(const QVector<VNodeDetail> &nodes) const
{
    for (qint32 i = 0; i < nodes.size(); ++i)
    {
        doc->IncrementReferens(nodes.at(i).getId());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::DecrementReferences(const QVector<VNodeDetail> &nodes) const
{
    for (qint32 i = 0; i < nodes.size(); ++i)
    {
        doc->DecrementReferens(nodes.at(i).getId());
    }
}
