/************************************************************************
 **
 **  @file   delgroup.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 4, 2016
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

#include "delgroup.h"

#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>

#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/def.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../ifc/xml/vabstractpattern.h"
#include "vundocommand.h"

//---------------------------------------------------------------------------------------------------------------------
DelGroup::DelGroup(VAbstractPattern *doc, quint32 id, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), nameActivDraw(doc->GetNameActivPP())
{
    setText(tr("delete group"));
    nodeId = id;
    xml = doc->CloneNodeById(nodeId);
}

//---------------------------------------------------------------------------------------------------------------------
DelGroup::~DelGroup()
{
}

//---------------------------------------------------------------------------------------------------------------------
void DelGroup::undo()
{
    qCDebug(vUndo, "Undo.");

    doc->SetCurrentPP(nameActivDraw);//Without this user will not see this change

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

    //Keep first!
    doc->SetCurrentPP(nameActivDraw);//Without this user will not see this change
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
