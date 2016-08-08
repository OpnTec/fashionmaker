/************************************************************************
 **
 **  @file   deletepatternpiece.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 6, 2014
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

#include "deletepatternpiece.h"

#include <QDomNode>
#include <QDomNodeList>

#include "../vmisc/logging.h"
#include "undocommands/../../ifc/xml/vabstractpattern.h"
#include "undocommands/vundocommand.h"

class QUndoCommand;

//---------------------------------------------------------------------------------------------------------------------
DeletePatternPiece::DeletePatternPiece(VAbstractPattern *doc, const QString &namePP, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), namePP(namePP), patternPiece(QDomElement()),
      previousPPName(QString())
{
    setText(tr("delete pattern piece %1").arg(namePP));

    const QDomElement patternP = doc->GetPPElement(namePP);
    patternPiece = patternP.cloneNode().toElement();
    const QDomElement previousPP = patternP.previousSibling().toElement();//find previous pattern piece
    if (not previousPP.isNull() && previousPP.tagName() == VAbstractPattern::TagDraw)
    {
        previousPPName = doc->GetParametrString(previousPP, VAbstractPattern::AttrName, "");
    }
}

//---------------------------------------------------------------------------------------------------------------------
DeletePatternPiece::~DeletePatternPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
void DeletePatternPiece::undo()
{
    qCDebug(vUndo, "Undo.");

    QDomElement rootElement = doc->documentElement();

    if (not previousPPName.isEmpty())
    { // not first in the list, add after tag draw
        const QDomNode previousPP = doc->GetPPElement(previousPPName);
        rootElement.insertAfter(patternPiece, previousPP);
    }
    else
    { // first in the list, add before tag draw
        const QDomNodeList list = rootElement.elementsByTagName(VAbstractPattern::TagDraw);
        QDomElement draw;

        if (not list.isEmpty())
        {
            draw = list.at(0).toElement();
        }

        Q_ASSERT_X(not draw.isNull(), Q_FUNC_INFO, "Couldn't' find tag draw");
        rootElement.insertBefore(patternPiece, draw);
    }

    emit NeedFullParsing();
    doc->ChangeActivPP(namePP);
}

//---------------------------------------------------------------------------------------------------------------------
void DeletePatternPiece::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement rootElement = doc->documentElement();
    const QDomElement patternPiece = doc->GetPPElement(namePP);
    rootElement.removeChild(patternPiece);
    emit NeedFullParsing();
}
