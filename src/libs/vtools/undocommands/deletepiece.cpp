/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "deletepiece.h"

#include <QDomElement>
#include <QHash>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vmisc/logging.h"
#include "../vmisc/def.h"
#include "../tools/vdatatool.h"
#include "vundocommand.h"
#include "../vpatterndb/vpiecenode.h"

class QUndoCommand;

//---------------------------------------------------------------------------------------------------------------------
DeletePiece::DeletePiece(VAbstractPattern *doc, quint32 id, const VPiece &detail, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_parentNode(),
      m_siblingId(NULL_ID),
      m_detail(detail)
{
    setText(tr("delete tool"));
    nodeId = id;
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        xml = domElement.cloneNode().toElement();
        m_parentNode = domElement.parentNode();
        QDomNode previousDetail = domElement.previousSibling();
        if (previousDetail.isNull())
        {
            m_siblingId = NULL_ID;
        }
        else
        {
            // Better save id of previous detail instead of reference to node.
            m_siblingId = doc->GetParametrUInt(previousDetail.toElement(), VAbstractPattern::AttrId, NULL_ID_STR);
        }
    }
    else
    {
        qCDebug(vUndo, "Can't get detail by id = %u.", nodeId);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
DeletePiece::~DeletePiece()
{}

//---------------------------------------------------------------------------------------------------------------------
void DeletePiece::undo()
{
    qCDebug(vUndo, "Undo.");

    UndoDeleteAfterSibling(m_parentNode, m_siblingId);
    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
void DeletePiece::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        m_parentNode.removeChild(domElement);

        // UnionDetails delete two old details and create one new.
        // So when UnionDetail delete detail we can't use FullParsing. So we hide detail on scene directly.
        QHash<quint32, VDataTool*>* tools = doc->getTools();
        SCASSERT(tools != nullptr);
        VToolSeamAllowance *toolDet = qobject_cast<VToolSeamAllowance*>(tools->value(nodeId));
        SCASSERT(toolDet != nullptr);
        toolDet->hide();

        DecrementReferences(m_detail.GetNodes());
        emit NeedFullParsing(); // Doesn't work when UnionDetail delete detail.
    }
    else
    {
        qCDebug(vUndo, "Can't get detail by id = %u.", nodeId);
        return;
    }
}
