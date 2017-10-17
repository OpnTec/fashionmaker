/************************************************************************
 **
 **  @file   saveplacelabeloptions.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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
#include "saveplacelabeloptions.h"
#include "../tools/nodeDetails/vtoolplacelabel.h"

//---------------------------------------------------------------------------------------------------------------------
SavePlaceLabelOptions::SavePlaceLabelOptions(const VPlaceLabelItem &oldLabel, const VPlaceLabelItem &newLabel,
                                             VAbstractPattern *doc, VContainer *data, quint32 id, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_oldLabel(oldLabel),
      m_newLabel(newLabel),
      m_data(data)
{
    setText(tr("save place label options"));
    nodeId = id;
}

//---------------------------------------------------------------------------------------------------------------------
void SavePlaceLabelOptions::undo()
{
    qCDebug(vUndo, "Undo.");

    QDomElement domElement = doc->elementById(nodeId, VAbstractPattern::TagPoint);
    if (domElement.isElement())
    {
        VToolPlaceLabel::AddAttributes(doc, domElement, nodeId, m_oldLabel);

        if (m_oldLabel.GetCenterPoint() != m_newLabel.GetCenterPoint())
        {
            doc->IncrementReferens(m_oldLabel.GetCenterPoint());
            doc->DecrementReferens(m_newLabel.GetCenterPoint());
        }

        SCASSERT(m_data);
        m_data->UpdateGObject(nodeId, new VPlaceLabelItem(m_oldLabel));
    }
    else
    {
        qCDebug(vUndo, "Can't find place label with id = %u.", nodeId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void SavePlaceLabelOptions::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement domElement = doc->elementById(nodeId, VAbstractPattern::TagPoint);
    if (domElement.isElement())
    {
        VToolPlaceLabel::AddAttributes(doc, domElement, nodeId, m_newLabel);

        if (m_oldLabel.GetCenterPoint() != m_newLabel.GetCenterPoint())
        {
            doc->IncrementReferens(m_newLabel.GetCenterPoint());
            doc->DecrementReferens(m_oldLabel.GetCenterPoint());
        }

        SCASSERT(m_data);
        m_data->UpdateGObject(nodeId, new VPlaceLabelItem(m_newLabel));
    }
    else
    {
        qCDebug(vUndo, "Can't find path with id = %u.", nodeId);
    }
}
