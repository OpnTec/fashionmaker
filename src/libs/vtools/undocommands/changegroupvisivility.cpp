/************************************************************************
 **
 **  @file   changegroupvisivility.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 7, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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
#include "changegroupvisivility.h"
#include "../vmisc/vabstractapplication.h"
#include "../vwidgets/vmaingraphicsview.h"

//---------------------------------------------------------------------------------------------------------------------
ChangeGroupVisivility::ChangeGroupVisivility(VAbstractPattern *doc, vidtype id, bool visible, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_newVisibility(visible),
      m_nameActivDraw(doc->GetNameActivPP())
{
    setText(tr("change group visibility"));
    nodeId = id;
    QDomElement group = doc->elementById(nodeId, VAbstractPattern::TagGroup);
    if (group.isElement())
    {
        m_oldVisibility = doc->GetParametrBool(group, VAbstractPattern::AttrVisible, trueStr);
    }
    else
    {
        qDebug("Can't get group by id = %u.", id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
ChangeGroupVisivility::~ChangeGroupVisivility()
{}

//---------------------------------------------------------------------------------------------------------------------
void ChangeGroupVisivility::undo()
{
    qCDebug(vUndo, "Undo.");

    Do(m_oldVisibility);
}

//---------------------------------------------------------------------------------------------------------------------
void ChangeGroupVisivility::redo()
{
    qCDebug(vUndo, "Redo.");

    Do(m_newVisibility);
}

//---------------------------------------------------------------------------------------------------------------------
void ChangeGroupVisivility::Do(bool visible)
{
    doc->ChangeActivPP(m_nameActivDraw);//Without this user will not see this change

    QDomElement group = doc->elementById(nodeId, VAbstractPattern::TagGroup);
    if (group.isElement())
    {
        doc->SetAttribute(group, VAbstractPattern::AttrVisible, visible);

        QDomElement groups = doc->CreateGroups();
        if (not groups.isNull())
        {
            doc->ParseGroups(groups);
        }

        emit UpdateGroup(nodeId, visible);

        VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
    }
    else
    {
        qDebug("Can't get group by id = %u.", nodeId);
    }
}
