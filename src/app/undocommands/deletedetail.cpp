/************************************************************************
 **
 **  @file   deletedetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 6, 2014
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

#include "deletedetail.h"
#include "../xml/vpattern.h"
#include "../tools/vtooldetail.h"

//---------------------------------------------------------------------------------------------------------------------
DeleteDetail::DeleteDetail(VPattern *doc, quint32 id, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), parentNode(QDomNode())
{
    setText(tr("Delete tool"));
    nodeId = id;
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        xml = domElement.cloneNode().toElement();
        parentNode = domElement.parentNode();
    }
    else
    {
        qDebug()<<"Can't get detail by id = "<<nodeId<<Q_FUNC_INFO;
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
DeleteDetail::~DeleteDetail()
{}

//---------------------------------------------------------------------------------------------------------------------
void DeleteDetail::undo()
{
    parentNode.appendChild(xml);
    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
void DeleteDetail::redo()
{
    QDomElement domElement = doc->elementById(QString().setNum(nodeId));
    if (domElement.isElement())
    {
        parentNode.removeChild(domElement);

        //When UnionDetail delete detail we can't use FullParsing. So we hide detail on scene directly.
        QHash<quint32, VDataTool*>* tools = doc->getTools();
        SCASSERT(tools != nullptr);
        VToolDetail *toolDet = qobject_cast<VToolDetail*>(tools->value(nodeId));
        SCASSERT(toolDet != nullptr);
        toolDet->hide();

        emit NeedFullParsing();
    }
    else
    {
        qDebug()<<"Can't get detail by id = "<<nodeId<<Q_FUNC_INFO;
        return;
    }
}
