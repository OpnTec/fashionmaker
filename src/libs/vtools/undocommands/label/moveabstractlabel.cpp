/************************************************************************
 **
 **  @file   moveabstractlabel.cpp
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

#include "moveabstractlabel.h"

#include <QDomElement>

#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../vundocommand.h"

//---------------------------------------------------------------------------------------------------------------------
MoveAbstractLabel::MoveAbstractLabel(VAbstractPattern *doc, quint32 pointId, const QPointF &pos, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_oldPos(),
      m_newPos(pos)
{
    nodeId = pointId;
    qCDebug(vUndo, "Point id %u", nodeId);

    qCDebug(vUndo, "Label new position (%f;%f)", m_newPos.x(), m_newPos.y());
}

//---------------------------------------------------------------------------------------------------------------------
void MoveAbstractLabel::undo()
{
    qCDebug(vUndo, "Undo.");

    Do(m_oldPos);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveAbstractLabel::redo()
{
    qCDebug(vUndo, "Redo.");

    Do(m_newPos);
}
