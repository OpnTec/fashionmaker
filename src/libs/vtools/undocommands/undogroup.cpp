/************************************************************************
 **
 **  @file   undogroup.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 3, 2020
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2020 Valentina project
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

#include "undogroup.h"

#include <QDomNode>
#include <QDomNodeList>

#include "../vmisc/vabstractapplication.h"
#include "../vmisc/def.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../ifc/xml/vabstractpattern.h"
#include "vundocommand.h"
#include "../vtools/tools/vdatatool.h"

//AddGroup
//---------------------------------------------------------------------------------------------------------------------
AddGroup::AddGroup(const QDomElement &xml, VAbstractPattern *doc, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), nameActivDraw(doc->GetNameActivPP())
{
    setText(tr("add group"));
    nodeId = doc->GetParametrId(xml);
}

//---------------------------------------------------------------------------------------------------------------------
void AddGroup::undo()
{
    qCDebug(vUndo, "Undo.");

    doc->ChangeActivPP(nameActivDraw);//Without this user will not see this change

    QDomElement groups = doc->CreateGroups();
    if (not groups.isNull())
    {
        QDomElement group = doc->elementById(nodeId, VAbstractPattern::TagGroup);
        if (group.isElement())
        {
            group.setAttribute(VAbstractPattern::AttrVisible, trueStr);
            doc->ParseGroups(groups);
            if (groups.removeChild(group).isNull())
            {
                qCDebug(vUndo, "Can't delete group.");
                return;
            }
            emit UpdateGroups();
        }
        else
        {
            if (groups.childNodes().isEmpty())
            {
                QDomNode parent = groups.parentNode();
                parent.removeChild(groups);
            }

            qCDebug(vUndo, "Can't get group by id = %u.", nodeId);
            return;
        }
    }
    else
    {
        qCDebug(vUndo, "Can't get tag Groups.");
        return;
    }

    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
    if (qApp->GetDrawMode() == Draw::Calculation)
    {
        emit doc->SetCurrentPP(nameActivDraw);//Return current pattern piece after undo
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AddGroup::redo()
{
    qCDebug(vUndo, "Redo.");

    doc->ChangeActivPP(nameActivDraw);//Without this user will not see this change

    QDomElement groups = doc->CreateGroups();
    if (not groups.isNull())
    {
        groups.appendChild(xml);
        doc->ParseGroups(groups);
        emit UpdateGroups();
    }
    else
    {
        qCDebug(vUndo, "Can't get tag Groups.");
        return;
    }

    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
}

//AddItemToGroup
//---------------------------------------------------------------------------------------------------------------------
AddItemToGroup::AddItemToGroup(const QDomElement &xml, VAbstractPattern *doc, quint32 groupId, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), nameActivDraw(doc->GetNameActivPP())
{
    setText(tr("Add item to group"));
    nodeId = groupId;
}

//---------------------------------------------------------------------------------------------------------------------
void AddItemToGroup::undo()
{
    qCDebug(vUndo, "Undo the add item to group");
    performUndoRedo(true);
}

//---------------------------------------------------------------------------------------------------------------------
void AddItemToGroup::redo()
{
    qCDebug(vUndo, "Redo the add item to group");
    performUndoRedo(false);
}

//---------------------------------------------------------------------------------------------------------------------
void AddItemToGroup::performUndoRedo(bool isUndo)
{
    doc->ChangeActivPP(nameActivDraw);//Without this user will not see this change

    QDomElement group = doc->elementById(nodeId, VAbstractPattern::TagGroup);
    if (group.isElement())
    {
        if(isUndo)
        {
            if (group.removeChild(xml).isNull())
            {
                qCDebug(vUndo, "Can't delete item.");
                return;
            }

            // set the item visible. Because if the undo is done when unvisible and it's not in any group after the
            // undo, it stays unvisible until the entire drawing is completly rerendered.
            quint32 objectId = doc->GetParametrUInt(xml, QStringLiteral("object"), NULL_ID_STR);
            quint32 toolId = doc->GetParametrUInt(xml, QStringLiteral("tool"), NULL_ID_STR);
            VDataTool* tool = VAbstractPattern::getTool(toolId);
            tool->GroupVisibility(objectId,true);
        }
        else // is redo
        {

            if (group.appendChild(xml).isNull())
            {
                qCDebug(vUndo, "Can't add item.");
                return;
            }
        }

        doc->SetModified(true);
        emit qApp->getCurrentDocument()->patternChanged(false);

        QDomElement groups = doc->CreateGroups();
        if (not groups.isNull())
        {
            doc->ParseGroups(groups);
        } else
        {
            qCDebug(vUndo, "Can't get tag Groups.");
            return;
        }

        emit UpdateGroups();
    }
    else
    {
        qCDebug(vUndo, "Can't get group by id = %u.", nodeId);
        return;
    }

    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
    if (qApp->GetDrawMode() == Draw::Calculation)
    {
        emit doc->SetCurrentPP(nameActivDraw);//Return current pattern piece after undo
    }
}

//RemoveItemFromGroup
//---------------------------------------------------------------------------------------------------------------------
RemoveItemFromGroup::RemoveItemFromGroup(const QDomElement &xml, VAbstractPattern *doc, quint32 groupId,
                                         QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), nameActivDraw(doc->GetNameActivPP())
{
    setText(tr("Remove item from group"));
    nodeId = groupId;
}

//---------------------------------------------------------------------------------------------------------------------
void RemoveItemFromGroup::undo()
{
    qCDebug(vUndo, "Undo the remove item from group");
    performUndoRedo(true);
}

//---------------------------------------------------------------------------------------------------------------------
void RemoveItemFromGroup::redo()
{
    qCDebug(vUndo, "Redo the add item to group");
    performUndoRedo(false);
}

//---------------------------------------------------------------------------------------------------------------------
void RemoveItemFromGroup::performUndoRedo(bool isUndo)
{
    doc->ChangeActivPP(nameActivDraw);//Without this user will not see this change

    QDomElement group = doc->elementById(nodeId, VAbstractPattern::TagGroup);
    if (group.isElement())
    {
        if(isUndo)
        {
            if (group.appendChild(xml).isNull())
            {
                qCDebug(vUndo, "Can't add the item.");
                return;
            }
        }
        else // is redo
        {
            if (group.removeChild(xml).isNull())
            {
                qCDebug(vUndo, "Can't delete item.");
                return;
            }

            // set the item visible. Because if the undo is done when unvisibile and it's not in any group after the
            // undo, it stays unvisible until the entire drawing is completly rerendered.
            quint32 objectId = doc->GetParametrUInt(xml, QStringLiteral("object"), NULL_ID_STR);
            quint32 toolId = doc->GetParametrUInt(xml, QStringLiteral("tool"), NULL_ID_STR);
            VDataTool* tool = VAbstractPattern::getTool(toolId);
            tool->GroupVisibility(objectId,true);
        }

        doc->SetModified(true);
        emit qApp->getCurrentDocument()->patternChanged(false);

        QDomElement groups = doc->CreateGroups();
        if (not groups.isNull())
        {
            doc->ParseGroups(groups);
        } else
        {
            qCDebug(vUndo, "Can't get tag Groups.");
            return;
        }

        emit UpdateGroups();
    }
    else
    {
        qCDebug(vUndo, "Can't get group by id = %u.", nodeId);
        return;
    }

    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());

    if (qApp->GetDrawMode() == Draw::Calculation)
    {
        emit doc->SetCurrentPP(nameActivDraw);//Return current pattern piece after undo
    }
}

//ChangeGroupVisibility
//---------------------------------------------------------------------------------------------------------------------
ChangeGroupVisibility::ChangeGroupVisibility(VAbstractPattern *doc, vidtype id, bool visible, QUndoCommand *parent)
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
void ChangeGroupVisibility::undo()
{
    qCDebug(vUndo, "Undo.");

    Do(m_oldVisibility);
}

//---------------------------------------------------------------------------------------------------------------------
void ChangeGroupVisibility::redo()
{
    qCDebug(vUndo, "Redo.");

    Do(m_newVisibility);
}

//---------------------------------------------------------------------------------------------------------------------
void ChangeGroupVisibility::Do(bool visible)
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

//ChangeMultipleGroupsVisibility
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

//DelGroup
//---------------------------------------------------------------------------------------------------------------------
DelGroup::DelGroup(VAbstractPattern *doc, quint32 id, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), nameActivDraw(doc->GetNameActivPP())
{
    setText(tr("delete group"));
    nodeId = id;
    xml = doc->CloneNodeById(nodeId);
}

//---------------------------------------------------------------------------------------------------------------------
void DelGroup::undo()
{
    qCDebug(vUndo, "Undo.");

    if (qApp->GetDrawMode() == Draw::Calculation)
    {
        emit doc->SetCurrentPP(nameActivDraw);//Without this user will not see this change
    }

    QDomElement groups = doc->CreateGroups();
    if (not groups.isNull())
    {
        groups.appendChild(xml);
        doc->ParseGroups(groups);
        emit UpdateGroups();
    }
    else
    {
        qCDebug(vUndo, "Can't get tag Groups.");
        return;
    }

    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
}

//---------------------------------------------------------------------------------------------------------------------
void DelGroup::redo()
{
    qCDebug(vUndo, "Redo.");

    if (qApp->GetDrawMode() == Draw::Calculation)
    {//Keep first!
        emit doc->SetCurrentPP(nameActivDraw);//Without this user will not see this change
    }
    QDomElement groups = doc->CreateGroups();
    if (not groups.isNull())
    {
        QDomElement group = doc->elementById(nodeId, VAbstractPattern::TagGroup);
        if (group.isElement())
        {
            group.setAttribute(VAbstractPattern::AttrVisible, trueStr);
            doc->ParseGroups(groups);
            if (groups.removeChild(group).isNull())
            {
                qCDebug(vUndo, "Can't delete group.");
                return;
            }
            emit UpdateGroups();

            if (groups.childNodes().isEmpty())
            {
                QDomNode parent = groups.parentNode();
                parent.removeChild(groups);
            }
        }
        else
        {
            qCDebug(vUndo, "Can't get group by id = %u.", nodeId);
            return;
        }
    }
    else
    {
        qCDebug(vUndo, "Can't get tag Groups.");
        return;
    }

    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
}
