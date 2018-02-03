/************************************************************************
 **
 **  @file   toggledetailinlayout.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2016
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

#include "togglepiecestate.h"

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
TogglePieceState::TogglePieceState(quint32 id, bool state, VContainer *data, VAbstractPattern *doc,
                                   QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_id(id),
      m_data(data),
      m_oldState(not state),
      m_newState(state)
{}

//---------------------------------------------------------------------------------------------------------------------
void TogglePieceState::undo()
{
    qCDebug(vUndo, "TogglePieceState::undo().");

    if (m_newState != m_oldState)
    {
        Do(m_oldState);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TogglePieceState::redo()
{
    qCDebug(vUndo, "TogglePieceState::redo().");

    if (m_newState != m_oldState)
    {
        Do(m_newState);
    }
}

//---------------------------------------------------------------------------------------------------------------------
TogglePieceInLayout::TogglePieceInLayout(quint32 id, bool state, VContainer *data, VAbstractPattern *doc,
                                         QUndoCommand *parent)
    : TogglePieceState(id, state, data, doc, parent)
{
    setText(tr("detail in layout list"));
    m_oldState = m_data->DataPieces()->value(m_id).IsInLayout();
}

//---------------------------------------------------------------------------------------------------------------------
void TogglePieceInLayout::Do(bool state)
{
    QDomElement detail = doc->elementById(m_id, VAbstractPattern::TagDetail);
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
    }
}

//---------------------------------------------------------------------------------------------------------------------
TogglePieceForbidFlipping::TogglePieceForbidFlipping(quint32 id, bool state, VContainer *data, VAbstractPattern *doc,
                                                     QUndoCommand *parent)
    : TogglePieceState(id, state, data, doc, parent)
{
    setText(tr("forbid flipping"));
    m_oldState = m_data->DataPieces()->value(m_id).IsForbidFlipping();
}

//---------------------------------------------------------------------------------------------------------------------
void TogglePieceForbidFlipping::Do(bool state)
{
    QDomElement detail = doc->elementById(m_id, VAbstractPattern::TagDetail);
    if (detail.isElement())
    {
        doc->SetAttribute(detail, AttrForbidFlipping, state);

        VPiece det = m_data->DataPieces()->value(m_id);
        det.SetForbidFlipping(state);
        m_data->UpdatePiece(m_id, det);
    }
    else
    {
        qDebug("Can't get detail by id = %u.", m_id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
TogglePieceForceFlipping::TogglePieceForceFlipping(quint32 id, bool state, VContainer *data, VAbstractPattern *doc,
                                                   QUndoCommand *parent)
    : TogglePieceState(id, state, data, doc, parent)
{
    setText(tr("force flipping"));
    m_oldState = m_data->DataPieces()->value(m_id).IsForceFlipping();
}

//---------------------------------------------------------------------------------------------------------------------
void TogglePieceForceFlipping::Do(bool state)
{
    QDomElement detail = doc->elementById(m_id, VAbstractPattern::TagDetail);
    if (detail.isElement())
    {
        doc->SetAttribute(detail, AttrForceFlipping, state);

        VPiece det = m_data->DataPieces()->value(m_id);
        det.SetForceFlipping(state);
        m_data->UpdatePiece(m_id, det);
    }
    else
    {
        qDebug("Can't get detail by id = %u.", m_id);
    }
}
