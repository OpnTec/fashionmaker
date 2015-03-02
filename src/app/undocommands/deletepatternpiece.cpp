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
#include "../xml/vpattern.h"
#include "addpatternpiece.h"

//---------------------------------------------------------------------------------------------------------------------
DeletePatternPiece::DeletePatternPiece(VPattern *doc, const QString &namePP, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), namePP(namePP), patternPiece(QDomElement()), mPath(QString()),
      previousPPName(QString())
{
    setText(tr("Delete pattern piece %1").arg(namePP));

    QDomElement patternP = doc->GetPPElement(namePP);
    patternPiece = patternP.cloneNode().toElement();
    mPath = doc->MPath();
    QDomNode previousPP = patternP.previousSibling();//find previous pattern piece
    previousPPName = doc->GetParametrString(previousPP.toElement(), VPattern::AttrName, "");
}

//---------------------------------------------------------------------------------------------------------------------
DeletePatternPiece::~DeletePatternPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
void DeletePatternPiece::undo()
{
    qCDebug(vUndo)<<"Undo.";

    QDomElement rootElement = doc->documentElement();
    QDomNode previousPP = doc->GetPPElement(previousPPName);
    rootElement.insertAfter(patternPiece, previousPP);

    emit NeedFullParsing();
    doc->ChangedActivPP(namePP);
}

//---------------------------------------------------------------------------------------------------------------------
void DeletePatternPiece::redo()
{
    qCDebug(vUndo)<<"Redo.";

    QDomElement rootElement = doc->documentElement();
    QDomElement patternPiece = doc->GetPPElement(namePP);
    rootElement.removeChild(patternPiece);
    emit NeedFullParsing();
}
