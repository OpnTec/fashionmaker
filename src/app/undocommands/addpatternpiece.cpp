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

#include "addpatternpiece.h"
#include "../xml/vpattern.h"

int AddPatternPiece::countPP = 0;

//---------------------------------------------------------------------------------------------------------------------
AddPatternPiece::AddPatternPiece(const QDomElement &xml, VPattern *doc, const QString &namePP, const QString &mPath,
                                 QUndoCommand *parent)
    : QObject(), QUndoCommand(parent), xml(xml), doc(doc), namePP(namePP), redoFlag(false), mPath(mPath)
{
    setText(tr("Add pattern piece %1").arg(namePP));
}

//---------------------------------------------------------------------------------------------------------------------
AddPatternPiece::~AddPatternPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
void AddPatternPiece::undo()
{
    if (countPP <= 1)
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
    --countPP;
}

//---------------------------------------------------------------------------------------------------------------------
void AddPatternPiece::redo()
{
    ++countPP;
    if (countPP == 1 && mPath.isEmpty() == false)
    {
        doc->CreateEmptyFile(mPath);
    }

    QDomElement rootElement = doc->documentElement();

    rootElement.appendChild(xml);
    doc->haveLiteChange();

    if (redoFlag)
    {
        emit NeedFullParsing();
    }
    redoFlag = true;
}
