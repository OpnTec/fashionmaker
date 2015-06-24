/************************************************************************
 **
 **  @file   addpatternpiece.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2014
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

#include "addpatternpiece.h"

//---------------------------------------------------------------------------------------------------------------------
AddPatternPiece::AddPatternPiece(const QDomElement &xml, VAbstractPattern *doc, const QString &namePP,
                                 const QString &mPath, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), namePP(namePP), mPath(mPath)
{
    SCASSERT(namePP.isEmpty() == false);
    SCASSERT(mPath.isEmpty() == false);
    setText(tr("add pattern piece %1").arg(namePP));
}

//---------------------------------------------------------------------------------------------------------------------
AddPatternPiece::~AddPatternPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
void AddPatternPiece::undo()
{
    qCDebug(vUndo, "Undo.");

    if (doc->CountPP() <= 1)
    {
        emit ClearScene();
    }
    else
    {
        QDomElement rootElement = doc->documentElement();
        QDomElement patternPiece = doc->GetPPElement(namePP);
        rootElement.removeChild(patternPiece);
        emit NeedFullParsing();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AddPatternPiece::redo()
{
    qCDebug(vUndo, "Redo.");

    if (doc->CountPP() == 0 && mPath.isEmpty() == false)
    {
        doc->CreateEmptyFile(mPath);
    }

    QDomElement rootElement = doc->documentElement();

    rootElement.appendChild(xml);

    RedoFullParsing();
}
