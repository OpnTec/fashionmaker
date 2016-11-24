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

#include "savepieceoptions.h"

#include <QDomElement>
#include <QPointF>
#include <QUndoCommand>
#include <QDebug>

#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/ifcdef.h"
#include "../vmisc/logging.h"
#include "../vmisc/def.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpatterninfogeometry.h"
#include "../vpatterndb/vpatternpiecedata.h"
#include "../vpatterndb/vgrainlinegeometry.h"
#include "../tools/vtoolseamallowance.h"
#include "vundocommand.h"

class QDomElement;
class QUndoCommand;

//---------------------------------------------------------------------------------------------------------------------
SavePieceOptions::SavePieceOptions(const VPiece &oldDet, const VPiece &newDet, VAbstractPattern *doc, quint32 id, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_oldDet(oldDet),
      m_newDet(newDet)
{
    setText(tr("save detail option"));
    nodeId = id;
}

//---------------------------------------------------------------------------------------------------------------------
SavePieceOptions::~SavePieceOptions()
{}

//---------------------------------------------------------------------------------------------------------------------
void SavePieceOptions::undo()
{
    qCDebug(vUndo, "Undo.");

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        VToolSeamAllowance::AddAttributes(doc, domElement, nodeId, m_oldDet);
        doc->RemoveAllChildren(domElement);//Very important to clear before rewrite
        VToolSeamAllowance::AddPatternPieceData(doc, domElement, m_oldDet);
        VToolSeamAllowance::AddPatternInfo(doc, domElement, m_oldDet);
        VToolSeamAllowance::AddGrainline(doc, domElement, m_oldDet);
        VToolSeamAllowance::AddNodes(doc, domElement, m_oldDet);

        IncrementReferences(m_oldDet.Missing(m_newDet));
        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qCDebug(vUndo, "Can't find detail with id = %u.", nodeId);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void SavePieceOptions::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        VToolSeamAllowance::AddAttributes(doc, domElement, nodeId, m_newDet);
        doc->RemoveAllChildren(domElement);//Very important to clear before rewrite
        VToolSeamAllowance::AddPatternPieceData(doc, domElement, m_newDet);
        VToolSeamAllowance::AddPatternInfo(doc, domElement, m_newDet);
        VToolSeamAllowance::AddGrainline(doc, domElement, m_newDet);
        VToolSeamAllowance::AddNodes(doc, domElement, m_newDet);

        DecrementReferences(m_oldDet.Missing(m_newDet));
        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qCDebug(vUndo, "Can't find detail with id = %u.", nodeId);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool SavePieceOptions::mergeWith(const QUndoCommand *command)
{
    const SavePieceOptions *saveCommand = static_cast<const SavePieceOptions *>(command);
    SCASSERT(saveCommand != nullptr);
    const quint32 id = saveCommand->DetId();

    if (id != nodeId)
    {
        return false;
    }

    m_newDet = saveCommand->NewDet();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int SavePieceOptions::id() const
{
    return static_cast<int>(UndoCommand::SavePieceOptions);
}
