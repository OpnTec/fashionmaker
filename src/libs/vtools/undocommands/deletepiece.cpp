/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../vmisc/def.h"
#include "../tools/vdatatool.h"
#include "vundocommand.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vwidgets/vmaingraphicsview.h"

//---------------------------------------------------------------------------------------------------------------------
DeletePiece::DeletePiece(VAbstractPattern *doc, quint32 id, const VContainer &data, VMainGraphicsScene *scene,
                         QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_parentNode(),
      m_siblingId(NULL_ID),
      m_detail(data.GetPiece(id)),
      m_data(data),
      m_scene(scene),
      m_tool(),
      m_record(VAbstractTool::GetRecord(id, Tool::Piece, doc))
{
    setText(tr("delete tool"));
    nodeId = id;
    QDomElement domElement = doc->elementById(id, VAbstractPattern::TagDetail);
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
            m_siblingId = doc->GetParametrUInt(previousDetail.toElement(), VDomDocument::AttrId, NULL_ID_STR);
        }
    }
    else
    {
        qCDebug(vUndo, "Can't get detail by id = %u.", nodeId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
DeletePiece::~DeletePiece()
{
    if (not m_tool.isNull())
    {
        delete m_tool;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DeletePiece::undo()
{
    qCDebug(vUndo, "Undo.");

    UndoDeleteAfterSibling(m_parentNode, m_siblingId, VAbstractPattern::TagDetail);

    VAbstractPattern::AddTool(nodeId, m_tool);
    m_data.UpdatePiece(nodeId, m_detail);

    m_tool->ReinitInternals(m_detail, m_scene);

    VAbstractTool::AddRecord(m_record, doc);
    m_scene->addItem(m_tool);
    m_tool->ConnectOutsideSignals();
    m_tool->show();
    VMainGraphicsView::NewSceneRect(m_scene, qApp->getSceneView(), m_tool);
    m_tool.clear();
    emit doc->UpdateInLayoutList();
}

//---------------------------------------------------------------------------------------------------------------------
void DeletePiece::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement domElement = doc->elementById(nodeId, VAbstractPattern::TagDetail);
    if (domElement.isElement())
    {
        m_parentNode.removeChild(domElement);

        m_tool = qobject_cast<VToolSeamAllowance*>(VAbstractPattern::getTool(nodeId));
        SCASSERT(not m_tool.isNull());
        m_tool->DisconnectOutsideSignals();
        m_tool->EnableToolMove(true);
        m_tool->hide();

        m_scene->removeItem(m_tool);

        VAbstractPattern::RemoveTool(nodeId);
        m_data.RemovePiece(nodeId);
        VAbstractTool::RemoveRecord(m_record, doc);

        DecrementReferences(m_detail.GetPath().GetNodes());
        DecrementReferences(m_detail.GetCustomSARecords());
        DecrementReferences(m_detail.GetInternalPaths());
        DecrementReferences(m_detail.GetPins());

        emit doc->UpdateInLayoutList();
    }
    else
    {
        qCDebug(vUndo, "Can't get detail by id = %u.", nodeId);
    }
}
