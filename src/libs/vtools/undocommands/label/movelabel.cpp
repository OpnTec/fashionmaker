/************************************************************************
 **
 **  @file   movelabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 12, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "movelabel.h"

#include <QDomElement>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vundocommand.h"
#include "moveabstractlabel.h"
#include "../vtools/tools/drawTools/vdrawtool.h"

//---------------------------------------------------------------------------------------------------------------------
MoveLabel::MoveLabel(VAbstractPattern *doc, const QPointF &pos, const quint32 &id, QUndoCommand *parent)
    : MoveAbstractLabel(doc, id, pos, parent),
      m_scene(qApp->getCurrentScene())
{
    setText(tr("move point label"));

    QDomElement domElement = doc->elementById(nodeId, VAbstractPattern::TagPoint);
    if (domElement.isElement())
    {
        m_oldPos.rx() = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMx, "0.0"));
        m_oldPos.ry() = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMy, "0.0"));

        qCDebug(vUndo, "Label old Mx %f", m_oldPos.x());
        qCDebug(vUndo, "Label old My %f", m_oldPos.y());
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", nodeId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveLabel::mergeWith(const QUndoCommand *command)
{
    const MoveLabel *moveCommand = static_cast<const MoveLabel *>(command);
    SCASSERT(moveCommand != nullptr)

    if (moveCommand->GetPointId() != nodeId)
    {
        return false;
    }

    qCDebug(vUndo, "Mergin undo.");
    m_newPos = moveCommand->GetNewPos();
    qCDebug(vUndo, "Label new Mx %f", m_newPos.x());
    qCDebug(vUndo, "Label new My %f", m_newPos.y());
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int MoveLabel::id() const
{
    return static_cast<int>(UndoCommand::MoveLabel);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveLabel::Do(const QPointF &pos)
{
    qCDebug(vUndo, "New mx %f", pos.x());
    qCDebug(vUndo, "New my %f", pos.y());

    QDomElement domElement = doc->elementById(nodeId, VAbstractPattern::TagPoint);
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrMx, QString().setNum(qApp->fromPixel(pos.x())));
        doc->SetAttribute(domElement, AttrMy, QString().setNum(qApp->fromPixel(pos.y())));

        if (VDrawTool *tool = qobject_cast<VDrawTool *>(VAbstractPattern::getTool(nodeId)))
        {
            tool->DoChangePosition(nodeId, pos);
        }
        VMainGraphicsView::NewSceneRect(m_scene, qApp->getSceneView());
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", nodeId);
    }
}
