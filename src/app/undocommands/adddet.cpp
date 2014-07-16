/************************************************************************
 **
 **  @file   adddet.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 6, 2014
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

#include "adddet.h"
#include "../xml/vpattern.h"

//---------------------------------------------------------------------------------------------------------------------
AddDet::AddDet(const QDomElement &xml, VPattern *doc, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent)
{
    setText(tr("Add detail"));
    nodeId = doc->GetParametrId(xml);
}

//---------------------------------------------------------------------------------------------------------------------
AddDet::~AddDet()
{}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void AddDet::undo()
{
    QDomElement element;
    if (doc->GetActivNodeElement(VPattern::TagDetails, element))
    {
        QDomElement domElement = doc->elementById(QString().setNum(nodeId));
        if (domElement.isElement())
        {
            if (element.removeChild(domElement).isNull())
            {
                qDebug()<<"Can't delete node";
                return;
            }
        }
        else
        {
            qDebug()<<"Can't get node by id = "<<nodeId<<Q_FUNC_INFO;
            return;
        }
    }
    else
    {
        qDebug()<<"Can't find tag"<<VPattern::TagDetails<< Q_FUNC_INFO;
        return;
    }
    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void AddDet::redo()
{
    QDomElement element;
    if (doc->GetActivNodeElement(VPattern::TagDetails, element))
    {
        element.appendChild(xml);
    }
    else
    {
        qDebug()<<"Can't find tag"<<VPattern::TagDetails<< Q_FUNC_INFO;
        return;
    }
    RedoFullParsing();
}
