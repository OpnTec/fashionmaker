/************************************************************************
 **
 **  @file   addgroup.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 3, 2018
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

#include "additemtogroup.h"

#include <QDomNode>
#include <QDomNodeList>

#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/def.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../ifc/xml/vabstractpattern.h"
#include "vundocommand.h"

//---------------------------------------------------------------------------------------------------------------------
AddItemToGroup::AddItemToGroup(const QDomElement &xml, VAbstractPattern *doc, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), nameActivDraw(doc->GetNameActivPP()), toolId(0), objectId(0)
{
    setText(tr("add item to group"));

    objectId = doc->GetParametrUInt(xml,QString("object"),NULL_ID_STR);
    toolId = doc->GetParametrUInt(xml,QString("tool"),NULL_ID_STR);
    QDomNode parentNode = xml.parentNode();

    if (parentNode.isElement())
    {
        nodeId = doc->GetParametrId(parentNode.toElement()); // nodeId is the groupId
    }
}

//---------------------------------------------------------------------------------------------------------------------
AddItemToGroup::~AddItemToGroup()
{
}

//---------------------------------------------------------------------------------------------------------------------
void AddItemToGroup::undo()
{
    qCDebug(vUndo, "Undo.");

    doc->ChangeActivPP(nameActivDraw);//Without this user will not see this change

    QDomElement groups = doc->CreateGroups();
    if (not groups.isNull())
    {
        QDomElement group = doc->elementById(nodeId, VAbstractPattern::TagGroup);
        if (group.isElement())
        {
            if (group.removeChild(xml).isNull())
            {
                qCDebug(vUndo, "Can't delete item.");
                return;
            }

            doc->SetModified(true);
            emit qApp->getCurrentDocument()->patternChanged(false);

            doc->ParseGroups(groups);

            emit UpdateGroups();
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
    emit doc->SetCurrentPP(nameActivDraw);//Return current pattern piece after undo
}

void AddItemToGroup::redo()
{

}
