/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 11, 2016
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

#include "addpiece.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vwidgets/vmaingraphicsview.h"

//---------------------------------------------------------------------------------------------------------------------
AddPiece::AddPiece(const QDomElement &xml, VAbstractPattern *doc, VContainer data, VMainGraphicsScene *scene,
                   const QString &drawName, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent),
      m_detail(),
      m_drawName(drawName),
      m_tool(),
      m_record(),
      m_scene(scene),
      m_data(data)
{
    setText(tr("add detail"));
    nodeId = doc->GetParametrId(xml);
    m_detail = data.GetPiece(nodeId);
    m_record = VAbstractTool::GetRecord(nodeId, Tool::Piece, doc);
}

//---------------------------------------------------------------------------------------------------------------------
AddPiece::~AddPiece()
{
    if (not m_tool.isNull())
    {
        delete m_tool;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AddPiece::undo()
{
    qCDebug(vUndo, "Undo.");

    QDomElement details = GetDetailsSection();
    if (not details.isNull())
    {
        QDomElement domElement = doc->elementById(nodeId, VAbstractPattern::TagDetail);
        if (domElement.isElement())
        {
            if (details.removeChild(domElement).isNull())
            {
                qCDebug(vUndo, "Can't delete node");
                return;
            }

            m_tool = qobject_cast<VToolSeamAllowance*>(VAbstractPattern::getTool(nodeId));
            SCASSERT(not m_tool.isNull());
            m_tool->DisconnectOutsideSignals();
            m_tool->hide();

            m_scene->removeItem(m_tool);

            VAbstractPattern::RemoveTool(nodeId);
            m_data.RemovePiece(nodeId);
            VAbstractTool::RemoveRecord(m_record, doc);

            DecrementReferences(m_detail.GetPath().GetNodes());
            DecrementReferences(m_detail.GetCustomSARecords());
            DecrementReferences(m_detail.GetInternalPaths());
            DecrementReferences(m_detail.GetPins());
        }
        else
        {
            qCDebug(vUndo, "Can't get node by id = %u.", nodeId);
            return;
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find tag %s.", qUtf8Printable(VAbstractPattern::TagDetails));
        return;
    }
    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
void AddPiece::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement details = GetDetailsSection();
    if (not details.isNull())
    {
        details.appendChild(xml);

        if (not m_tool.isNull())
        {
            VAbstractPattern::AddTool(nodeId, m_tool);
            m_data.UpdatePiece(nodeId, m_detail);

            m_tool->ReinitInternals(m_detail, m_scene);

            VAbstractTool::AddRecord(m_record, doc);
            m_scene->addItem(m_tool);
            m_tool->ConnectOutsideSignals();
            m_tool->show();
            VMainGraphicsView::NewSceneRect(m_scene, qApp->getSceneView(), m_tool);
            m_tool.clear();
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find tag %s.", qUtf8Printable(VAbstractPattern::TagDetails));
    }
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement AddPiece::GetDetailsSection() const
{
    QDomElement details;
    if (m_drawName.isEmpty())
    {
        doc->GetActivNodeElement(VAbstractPattern::TagDetails, details);
    }
    else
    {
        details = doc->GetDraw(m_drawName).firstChildElement(VAbstractPattern::TagDetails);
    }
    return details;
}
