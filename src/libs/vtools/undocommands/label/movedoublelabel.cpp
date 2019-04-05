/************************************************************************
 **
 **  @file   movedoublelabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "movedoublelabel.h"

#include <QDomElement>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/def.h"
#include "../vundocommand.h"
#include "moveabstractlabel.h"
#include "../vtools/tools/drawTools/vdrawtool.h"

//---------------------------------------------------------------------------------------------------------------------
MoveDoubleLabel::MoveDoubleLabel(VAbstractPattern *doc, const QPointF &pos, MoveDoublePoint type,
                                 quint32 toolId, quint32 pointId, QUndoCommand *parent)
    : MoveAbstractLabel(doc, pointId, pos, parent),
      m_type(type),
      m_idTool(toolId),
      m_scene(qApp->getCurrentScene())
{
    if (type == MoveDoublePoint::FirstPoint)
    {
        setText(tr("move the first dart label"));
    }
    else
    {
        setText(tr("move the second dart label"));
    }

    const QDomElement domElement = doc->elementById(m_idTool, VAbstractPattern::TagPoint);
    if (domElement.isElement())
    {
        if (type == MoveDoublePoint::FirstPoint)
        {
            m_oldPos.rx() = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMx1, "0.0"));
            m_oldPos.ry() = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMy1, "0.0"));

            qCDebug(vUndo, "Label old Mx1 %f", m_oldPos.x());
            qCDebug(vUndo, "Label old My1 %f", m_oldPos.y());
        }
        else
        {
            m_oldPos.rx() = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMx2, "0.0"));
            m_oldPos.ry() = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMy2, "0.0"));

            qCDebug(vUndo, "Label old Mx2 %f", m_oldPos.x());
            qCDebug(vUndo, "Label old My2 %f", m_oldPos.y());
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", nodeId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveDoubleLabel::mergeWith(const QUndoCommand *command)
{
    const MoveDoubleLabel *moveCommand = static_cast<const MoveDoubleLabel *>(command);
    SCASSERT(moveCommand != nullptr)

    if (moveCommand->GetPointId() != nodeId ||
        moveCommand->GetPointType() != m_type ||
        moveCommand->GetToolId() != m_idTool)
    {
        return false;
    }

    m_newPos = moveCommand->GetNewPos();

    if (m_type == MoveDoublePoint::FirstPoint)
    {
        qCDebug(vUndo, "Label new Mx1 %f", m_newPos.x());
        qCDebug(vUndo, "Label new My1 %f", m_newPos.y());
    }
    else
    {
        qCDebug(vUndo, "Label new Mx2 %f", m_newPos.x());
        qCDebug(vUndo, "Label new My2 %f", m_newPos.y());
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int MoveDoubleLabel::id() const
{
    return static_cast<int>(UndoCommand::MoveDoubleLabel);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveDoubleLabel::Do(const QPointF &pos)
{
    if (m_type == MoveDoublePoint::FirstPoint)
    {
        qCDebug(vUndo, "New mx1 %f", pos.x());
        qCDebug(vUndo, "New my1 %f", pos.y());
    }
    else
    {
        qCDebug(vUndo, "New mx2 %f", pos.x());
        qCDebug(vUndo, "New my2 %f", pos.y());
    }

    QDomElement domElement = doc->elementById(m_idTool, VAbstractPattern::TagPoint);
    if (domElement.isElement())
    {
        if (m_type == MoveDoublePoint::FirstPoint)
        {
            doc->SetAttribute(domElement, AttrMx1, QString().setNum(qApp->fromPixel(pos.x())));
            doc->SetAttribute(domElement, AttrMy1, QString().setNum(qApp->fromPixel(pos.y())));
        }
        else
        {
            doc->SetAttribute(domElement, AttrMx2, QString().setNum(qApp->fromPixel(pos.x())));
            doc->SetAttribute(domElement, AttrMy2, QString().setNum(qApp->fromPixel(pos.y())));
        }

        if (VDrawTool *tool = qobject_cast<VDrawTool *>(VAbstractPattern::getTool(m_idTool)))
        {
            tool->ChangeLabelPosition(nodeId, pos);
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", nodeId);
    }
}
