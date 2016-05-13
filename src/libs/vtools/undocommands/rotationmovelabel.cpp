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

#include "rotationmovelabel.h"
#include "../tools/drawTools/operation/vtoolrotation.h"

//---------------------------------------------------------------------------------------------------------------------
RotationMoveLabel::RotationMoveLabel(quint32 idTool, VAbstractPattern *doc, double x, double y, quint32 idPoint,
                                     QGraphicsScene *scene, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_oldMx(0.0),
      m_oldMy(0.0),
      m_newMx(x),
      m_newMy(y),
      m_isRedo(false),
      m_scene(scene),
      m_idTool(idTool)
{
    setText(tr("move point label"));
    nodeId = idPoint;
    qCDebug(vUndo, "Tool id %u", m_idTool);
    qCDebug(vUndo, "Point id %u", nodeId);

    qCDebug(vUndo, "Label new Mx %f", m_newMx);
    qCDebug(vUndo, "Label new My %f", m_newMy);

    SCASSERT(scene != nullptr);
    QDomElement element = GetDestinationObject(m_idTool, nodeId);
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
RotationMoveLabel::~RotationMoveLabel()
{
}

//---------------------------------------------------------------------------------------------------------------------
void RotationMoveLabel::undo()
{
    qCDebug(vUndo, "Undo.");

    Do(m_oldMx, m_oldMy);
    m_isRedo = true;
    emit ChangePosition(nodeId, m_oldMx, m_oldMy);
}

//---------------------------------------------------------------------------------------------------------------------
void RotationMoveLabel::redo()
{
    qCDebug(vUndo, "Redo.");

    Do(m_newMx, m_newMy);
    if (m_isRedo)
    {
        emit ChangePosition(nodeId, m_newMx, m_newMy);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool RotationMoveLabel::mergeWith(const QUndoCommand *command)
{
    const RotationMoveLabel *moveCommand = static_cast<const RotationMoveLabel *>(command);
    SCASSERT(moveCommand != nullptr);

    if (moveCommand->GetToolId() != m_idTool && moveCommand->GetPointId() != nodeId)
    {
        return false;
    }

    qCDebug(vUndo, "Mergin undo.");
    m_newMx = moveCommand->getNewMx();
    m_newMy = moveCommand->getNewMy();
    qCDebug(vUndo, "Label new Mx %f", m_newMx);
    qCDebug(vUndo, "Label new My %f", m_newMy);
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int RotationMoveLabel::id() const
{
    return static_cast<int>(UndoCommand::RotationMoveLabel);
}

//---------------------------------------------------------------------------------------------------------------------
void RotationMoveLabel::Do(double mx, double my)
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
QDomElement RotationMoveLabel::GetDestinationObject(quint32 idTool, quint32 idPoint) const
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
