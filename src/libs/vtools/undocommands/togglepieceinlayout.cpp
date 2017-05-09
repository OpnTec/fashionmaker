/************************************************************************
 **
 **  @file   toggledetailinlayout.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2016
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

#include "togglepieceinlayout.h"

#include <QDomElement>
#include <QHash>
#include <QMessageLogger>
#include <QtDebug>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "../vmisc/logging.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiece.h"
#include "vundocommand.h"

//---------------------------------------------------------------------------------------------------------------------
TogglePieceInLayout::TogglePieceInLayout(quint32 id, bool state, VContainer *data, VAbstractPattern *doc,
                                           QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_id(id),
      m_data(data),
      m_oldState(m_data->DataPieces()->value(m_id).IsInLayout()),
      m_newState(state)
{
    setText(tr("detail in layout list"));
}

//---------------------------------------------------------------------------------------------------------------------
TogglePieceInLayout::~TogglePieceInLayout()
{
}

//---------------------------------------------------------------------------------------------------------------------
void TogglePieceInLayout::undo()
{
    qCDebug(vUndo, "ToggleDetailInLayout::undo().");

    if (m_newState != m_oldState)
    {
        Do(m_oldState);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TogglePieceInLayout::redo()
{
    qCDebug(vUndo, "ToggleDetailInLayout::redo().");

    if (m_newState != m_oldState)
    {
        Do(m_newState);
    }
}

//---------------------------------------------------------------------------------------------------------------------
int TogglePieceInLayout::id() const
{
    return static_cast<int>(UndoCommand::TogglePieceInLayout);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 TogglePieceInLayout::getDetId() const
{
    return m_id;
}

//---------------------------------------------------------------------------------------------------------------------
bool TogglePieceInLayout::getNewState() const
{
    return m_newState;
}

//---------------------------------------------------------------------------------------------------------------------
void TogglePieceInLayout::Do(bool state)
{
    QDomElement detail = doc->elementById(m_id);
    if (detail.isElement())
    {
        if (state == false)
        {
            doc->SetAttribute(detail, AttrInLayout, state);
        }
        else
        {
            detail.removeAttribute(AttrInLayout);
        }

        VPiece det = m_data->DataPieces()->value(m_id);
        det.SetInLayout(state);
        m_data->UpdatePiece(m_id, det);
        emit UpdateList();
    }
    else
    {
        qDebug("Can't get detail by id = %u.", m_id);
        return;
    }
}
