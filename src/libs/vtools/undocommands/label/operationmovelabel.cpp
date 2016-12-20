/************************************************************************
 **
 **  @file   moverotationlabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 5, 2016
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

#include "operationmovelabel.h"

#include <QDomNode>
#include <QDomNodeList>

#include "../../tools/drawTools/operation/vtoolrotation.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/ifcdef.h"
#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/def.h"
#include "../vundocommand.h"
#include "moveabstractlabel.h"

class QUndoCommand;

//---------------------------------------------------------------------------------------------------------------------
OperationMoveLabel::OperationMoveLabel(quint32 idTool, VAbstractPattern *doc, double x, double y, quint32 idPoint,
                                       QUndoCommand *parent)
    : MoveAbstractLabel(doc, idPoint, x, y, parent),
      m_idTool(idTool)
{
    setText(tr("move point label"));

    qCDebug(vUndo, "Tool id %u", m_idTool);

    const QDomElement element = GetDestinationObject(m_idTool, nodeId);
    if (element.isElement())
    {
        m_oldMx = qApp->toPixel(doc->GetParametrDouble(element, AttrMx, "0.0"));
        m_oldMy = qApp->toPixel(doc->GetParametrDouble(element, AttrMy, "0.0"));

        qCDebug(vUndo, "Label old Mx %f", m_oldMx);
        qCDebug(vUndo, "Label old My %f", m_oldMy);
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", nodeId);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
OperationMoveLabel::~OperationMoveLabel()
{
}

//---------------------------------------------------------------------------------------------------------------------
bool OperationMoveLabel::mergeWith(const QUndoCommand *command)
{
    const OperationMoveLabel *moveCommand = static_cast<const OperationMoveLabel *>(command);
    SCASSERT(moveCommand != nullptr)

    if (moveCommand->GetToolId() != m_idTool && moveCommand->GetPointId() != nodeId)
    {
        return false;
    }

    qCDebug(vUndo, "Mergin undo.");
    m_newMx = moveCommand->GetNewMx();
    m_newMy = moveCommand->GetNewMy();
    qCDebug(vUndo, "Label new Mx %f", m_newMx);
    qCDebug(vUndo, "Label new My %f", m_newMy);
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int OperationMoveLabel::id() const
{
    return static_cast<int>(UndoCommand::RotationMoveLabel);
}

//---------------------------------------------------------------------------------------------------------------------
void OperationMoveLabel::Do(double mx, double my)
{
    qCDebug(vUndo, "New mx %f", mx);
    qCDebug(vUndo, "New my %f", my);

    QDomElement domElement = GetDestinationObject(m_idTool, nodeId);
    if (not domElement.isNull() && domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrMx, QString().setNum(qApp->fromPixel(mx)));
        doc->SetAttribute(domElement, AttrMy, QString().setNum(qApp->fromPixel(my)));
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", nodeId);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement OperationMoveLabel::GetDestinationObject(quint32 idTool, quint32 idPoint) const
{
    const QDomElement tool = doc->elementById(idTool);
    if (tool.isElement())
    {
        QDomElement correctDest;
        const QDomNodeList nodeList = tool.childNodes();
        for (qint32 i = 0; i < nodeList.size(); ++i)
        {
            const QDomElement dest = nodeList.at(i).toElement();
            if (not dest.isNull() && dest.isElement() && dest.tagName() == VToolRotation::TagDestination)
            {
                correctDest = dest;
                break;
            }
        }

        if (not correctDest.isNull())
        {
            const QDomNodeList destObjects = correctDest.childNodes();
            for (qint32 i = 0; i < destObjects.size(); ++i)
            {
                const QDomElement obj = destObjects.at(i).toElement();
                if (not obj.isNull() && obj.isElement())
                {
                    const quint32 id = doc->GetParametrUInt(obj, AttrIdObject, NULL_ID_STR);
                    if (idPoint == id)
                    {
                        return obj;
                    }
                }
            }
        }
    }

    return QDomElement();
}
