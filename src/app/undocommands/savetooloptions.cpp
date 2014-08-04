/************************************************************************
 **
 **  @file   savetooloptions.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 6, 2014
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

#include "savetooloptions.h"
#include "../options.h"
#include "../xml/vpattern.h"

//---------------------------------------------------------------------------------------------------------------------
SaveToolOptions::SaveToolOptions(const QDomElement &oldXml, const QDomElement &newXml, VPattern *doc, const quint32 &id,
                                 QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), oldXml(oldXml), newXml(newXml)
{
    setText(tr("Save tool option"));
    nodeId = id;
}

//---------------------------------------------------------------------------------------------------------------------
SaveToolOptions::~SaveToolOptions()
{}

//---------------------------------------------------------------------------------------------------------------------
void SaveToolOptions::undo()
{
    QDomElement domElement = doc->elementById(QString().setNum(nodeId));
    if (domElement.isElement())
    {
        domElement.parentNode().replaceChild(oldXml, domElement);

        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qDebug()<<"Can't find tool with id ="<< nodeId << Q_FUNC_INFO;
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void SaveToolOptions::redo()
{
    QDomElement domElement = doc->elementById(QString().setNum(nodeId));
    if (domElement.isElement())
    {
        domElement.parentNode().replaceChild(newXml, domElement);

        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qDebug()<<"Can't find tool with id ="<< nodeId << Q_FUNC_INFO;
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool SaveToolOptions::mergeWith(const QUndoCommand *command)
{
    const SaveToolOptions *saveCommand = static_cast<const SaveToolOptions *>(command);
    SCASSERT(saveCommand != nullptr);
    const quint32 id = saveCommand->getToolId();

    if (id != nodeId)
    {
        return false;
    }

    newXml = saveCommand->getNewXml();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int SaveToolOptions::id() const
{
    return static_cast<int>(UndoCommand::SaveToolOptions);
}
