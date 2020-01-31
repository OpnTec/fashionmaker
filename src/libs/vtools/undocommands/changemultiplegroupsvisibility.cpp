/************************************************************************
 **
 **  @file   changemultiplegroupsVisibility.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#include "changemultiplegroupsvisibility.h"
#include "../vmisc/vabstractapplication.h"
#include "../vwidgets/vmaingraphicsview.h"

//---------------------------------------------------------------------------------------------------------------------
ChangeMultipleGroupsVisibility::ChangeMultipleGroupsVisibility(VAbstractPattern *doc, const QVector<vidtype> &groups,
                                                               bool visible, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_groups(groups),
      m_newVisibility(visible),
      m_nameActivDraw(doc->GetNameActivPP())
{
    setText(tr("change multiple groups visibility"));

    for(auto & groupId : m_groups)
    {
        QDomElement group = doc->elementById(groupId, VAbstractPattern::TagGroup);
        if (group.isElement())
        {
            m_oldVisibility.insert(groupId, doc->GetParametrBool(group, VAbstractPattern::AttrVisible, trueStr));
        }
        else
        {
            qDebug("Can't get group by id = %u.", groupId);
        }
    }

}

//---------------------------------------------------------------------------------------------------------------------
void ChangeMultipleGroupsVisibility::undo()
{
    qCDebug(vUndo, "Undo.");

    doc->ChangeActivPP(m_nameActivDraw);//Without this user will not see this change

    QMap<vidtype, bool> groupsState;

    QMap<vidtype, bool>::const_iterator i = m_oldVisibility.constBegin();
    while (i != m_oldVisibility.constEnd())
    {
        QDomElement group = doc->elementById(i.key(), VAbstractPattern::TagGroup);
        if (group.isElement())
        {
            doc->SetAttribute(group, VAbstractPattern::AttrVisible, i.value());
            groupsState.insert(i.key(), i.value());
        }
        else
        {
            qDebug("Can't get group by id = %u.", i.key());
        }
        ++i;
    }

    if (not groupsState.isEmpty())
    {
        QDomElement groups = doc->CreateGroups();
        if (not groups.isNull())
        {
            doc->ParseGroups(groups);
        }

        VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());

        emit UpdateMultipleGroups(groupsState);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ChangeMultipleGroupsVisibility::redo()
{
    qCDebug(vUndo, "ChangeMultipleGroupsVisibility::redo");

    doc->ChangeActivPP(m_nameActivDraw);//Without this user will not see this change

    QMap<vidtype, bool> groupsState;

    for (auto& groupId : m_groups)
    {
        QDomElement group = doc->elementById(groupId, VAbstractPattern::TagGroup);
        if (group.isElement())
        {
            doc->SetAttribute(group, VAbstractPattern::AttrVisible, m_newVisibility);
            groupsState.insert(groupId, m_newVisibility);
        }
        else
        {
            qDebug("Can't get group by id = %u.", groupId);
        }
    }

    if (not groupsState.isEmpty())
    {
        QDomElement groups = doc->CreateGroups();
        if (not groups.isNull())
        {
            doc->ParseGroups(groups);
        }

        VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());

        emit UpdateMultipleGroups(groupsState);
    }
}
