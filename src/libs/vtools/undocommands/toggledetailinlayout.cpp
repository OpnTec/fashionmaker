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

#include "toggledetailinlayout.h"
#include "../vpatterndb/vdetail.h"
#include "../vpatterndb/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
ToggleDetailInLayout::ToggleDetailInLayout(quint32 id, bool state, VContainer *data, VAbstractPattern *doc,
                                           QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_id(id),
      m_data(data),
      m_oldState(m_data->DataDetails()->value(m_id).IsInLayout()),
      m_newState(state)
{
    setText(tr("detail in layout list"));
}

//---------------------------------------------------------------------------------------------------------------------
ToggleDetailInLayout::~ToggleDetailInLayout()
{
}

//---------------------------------------------------------------------------------------------------------------------
void ToggleDetailInLayout::undo()
{
    qCDebug(vUndo, "ToggleDetailInLayout::undo().");

    if (m_newState != m_oldState)
    {
        Do(m_oldState);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ToggleDetailInLayout::redo()
{
    qCDebug(vUndo, "ToggleDetailInLayout::redo().");

    if (m_newState != m_oldState)
    {
        Do(m_newState);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool ToggleDetailInLayout::mergeWith(const QUndoCommand *command)
{
    const ToggleDetailInLayout *stateCommand = static_cast<const ToggleDetailInLayout *>(command);
    SCASSERT(stateCommand != nullptr);
    const quint32 id = stateCommand->getDetId();

    if (id != m_id)
    {
        return false;
    }

    m_newState = stateCommand->getNewState();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int ToggleDetailInLayout::id() const
{
    return static_cast<int>(UndoCommand::ToggleDetailInLayout);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 ToggleDetailInLayout::getDetId() const
{
    return m_id;
}

//---------------------------------------------------------------------------------------------------------------------
bool ToggleDetailInLayout::getNewState() const
{
    return m_newState;
}

//---------------------------------------------------------------------------------------------------------------------
void ToggleDetailInLayout::Do(bool state)
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

        VDetail det = m_data->DataDetails()->value(m_id);
        det.SetInLayout(state);
        m_data->UpdateDetail(m_id, det);

        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qDebug("Can't get detail by id = %u.", m_id);
        return;
    }
}
