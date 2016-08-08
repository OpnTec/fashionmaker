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
 **  Copyright (C) 2013-2015 Valentina project
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

#include <QByteArray>
#include <QDomNode>

#include "../vlayout/../ifc/xml/vabstractpattern.h"
#include "../vmisc/logging.h"
#include "undocommands/vundocommand.h"

class QDomElement;
class QUndoCommand;

//---------------------------------------------------------------------------------------------------------------------
AddDet::AddDet(const QDomElement &xml, VAbstractPattern *doc, const VDetail &detail, const QString &drawName,
               QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), detail(detail), drawName(drawName)
{
    setText(tr("add detail"));
    nodeId = doc->GetParametrId(xml);
}

//---------------------------------------------------------------------------------------------------------------------
AddDet::~AddDet()
{}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void AddDet::undo()
{
    qCDebug(vUndo, "Undo.");

    QDomElement details = GetDetailsSection();
    if (not details.isNull())
    {
        QDomElement domElement = doc->elementById(nodeId);
        if (domElement.isElement())
        {
            if (details.removeChild(domElement).isNull())
            {
                qCDebug(vUndo, "Can't delete node");
                return;
            }

            DecrementReferences(detail.getNodes());
        }
        else
        {
            qCDebug(vUndo, "Can't get node by id = %u.", nodeId);
            return;
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find tag %s.", qUtf8Printable(VAbstractPattern::TagDetails));
        return;
    }
    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void AddDet::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement details = GetDetailsSection();
    if (not details.isNull())
    {
        details.appendChild(xml);
    }
    else
    {
        qCDebug(vUndo, "Can't find tag %s.", qUtf8Printable(VAbstractPattern::TagDetails));
        return;
    }
    RedoFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement AddDet::GetDetailsSection() const
{
    QDomElement details;
    if (drawName.isEmpty())
    {
        doc->GetActivNodeElement(VAbstractPattern::TagDetails, details);
    }
    else
    {
        details = doc->GetDraw(drawName).firstChildElement(VAbstractPattern::TagDetails);
    }
    return details;
}
