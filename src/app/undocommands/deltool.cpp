/************************************************************************
 **
 **  @file   deltool.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "deltool.h"
#include "../xml/vpattern.h"
#include <QGraphicsItem>
#include "../tools/vtooldetail.h"

//---------------------------------------------------------------------------------------------------------------------
DelTool::DelTool(VPattern *doc, quint32 id, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), parentNode(QDomNode()), siblingId(NULL_ID),
      nameActivDraw(doc->GetNameActivPP())
{
    setText(tr("Delete tool"));
    nodeId = id;
    siblingId = doc->SiblingNodeId(nodeId);
    parentNode = doc->ParentNodeById(nodeId);
    xml = doc->CloneNodeById(nodeId);
}

//---------------------------------------------------------------------------------------------------------------------
DelTool::~DelTool()
{}

//---------------------------------------------------------------------------------------------------------------------
void DelTool::undo()
{
    qCDebug(vUndo)<<"Undo.";

    UndoDeleteAfterSibling(parentNode, siblingId);
    emit NeedFullParsing();
    doc->SetCurrentPP(nameActivDraw);
}

//---------------------------------------------------------------------------------------------------------------------
void DelTool::redo()
{
    qCDebug(vUndo)<<"Redo.";

    QDomElement domElement = doc->NodeById(nodeId);
    parentNode.removeChild(domElement);
    emit NeedFullParsing();
    doc->SetCurrentPP(nameActivDraw);
}
