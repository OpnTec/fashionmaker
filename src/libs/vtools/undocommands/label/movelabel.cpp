/************************************************************************
 **
 **  @file   movelabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 12, 2014
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

#include "movelabel.h"

#include <QDomElement>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vundocommand.h"
#include "moveabstractlabel.h"

class QUndoCommand;

//---------------------------------------------------------------------------------------------------------------------
MoveLabel::MoveLabel(VAbstractPattern *doc, const double &x, const double &y, const quint32 &id, QUndoCommand *parent)
    : MoveAbstractLabel(doc, id, x, y, parent)
{
    setText(tr("move point label"));

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        m_oldMx = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMx, "0.0"));
        m_oldMy = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMy, "0.0"));

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
MoveLabel::~MoveLabel()
{}

//---------------------------------------------------------------------------------------------------------------------
bool MoveLabel::mergeWith(const QUndoCommand *command)
{
    const MoveLabel *moveCommand = static_cast<const MoveLabel *>(command);
    SCASSERT(moveCommand != nullptr);

    if (moveCommand->GetPointId() != nodeId)
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
int MoveLabel::id() const
{
    return static_cast<int>(UndoCommand::MoveLabel);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveLabel::Do(double mx, double my)
{
    qCDebug(vUndo, "New mx %f", mx);
    qCDebug(vUndo, "New my %f", my);

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
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
