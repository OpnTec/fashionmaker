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

#include "deletepatternpiece.h"
#include "../xml/vpattern.h"
#include "addpatternpiece.h"

//---------------------------------------------------------------------------------------------------------------------
DeletePatternPiece::DeletePatternPiece(VPattern *doc, const QString &namePP, QUndoCommand *parent)
    : QObject(), QUndoCommand(parent), doc(doc), namePP(namePP), patternPiece(QDomElement()), mPath(QString()),
      previousNode(QDomNode())
{
    setText(tr("Delete pattern piece %1").arg(namePP));

    QDomElement patternP= doc->GetPPElement(namePP);
    patternPiece = patternP.cloneNode().toElement();
    mPath = doc->MPath();
    previousNode = patternP.previousSibling();//find previous pattern piece
}

//---------------------------------------------------------------------------------------------------------------------
DeletePatternPiece::~DeletePatternPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
void DeletePatternPiece::undo()
{
    if (CountPP() == 0)
    {
        doc->CreateEmptyFile(mPath);
    }

    QDomElement rootElement = doc->documentElement();
    rootElement.insertAfter(patternPiece, previousNode);

    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
void DeletePatternPiece::redo()
{
    if (CountPP() <= 1)
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
int DeletePatternPiece::CountPP()
{
    QDomElement rootElement = doc->documentElement();
    if (rootElement.isNull())
    {
        return 0;
    }

    const QDomNodeList elements = rootElement.elementsByTagName( VPattern::TagDraw );
    return elements.count();
}
