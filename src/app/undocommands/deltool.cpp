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
    : VUndoCommand(QDomElement(), doc, parent), parentNode(QDomNode()), siblingId(NULL_ID)
{
    setText(tr("Delete tool"));
    nodeId = id;

    QVector<VToolRecord> history = doc->getLocalHistory();
    for (qint32 i = 0; i< history.size(); ++i)
    {
        const VToolRecord tool = history.at(i);
        if (nodeId == tool.getId())
        {
            if (i == 0)
            {
                siblingId = NULL_ID;
            }
            else
            {
                const VToolRecord tool = history.at(i-1);
                siblingId = tool.getId();
            }
        }
    }

    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        xml = domElement.cloneNode().toElement();
        parentNode = domElement.parentNode();
    }
    else
    {
        qDebug()<<"Can't get tool by id = "<<nodeId<<Q_FUNC_INFO;
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
DelTool::~DelTool()
{}

//---------------------------------------------------------------------------------------------------------------------
void DelTool::undo()
{
    UndoDeleteAfterSibling(parentNode, siblingId);
    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
void DelTool::redo()
{
    QDomElement domElement = doc->elementById(QString().setNum(nodeId));
    if (domElement.isElement())
    {
        parentNode.removeChild(domElement);
        emit NeedFullParsing();
    }
    else
    {
        qDebug()<<"Can't get tool by id = "<<nodeId<<Q_FUNC_INFO;
        return;
    }
}
