/************************************************************************
 **
 **  @file   addtocalc.cpp
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

#include "addtocalc.h"
#include "../xml/vpattern.h"

//---------------------------------------------------------------------------------------------------------------------
AddToCalc::AddToCalc(const QDomElement &xml, VPattern *doc, QUndoCommand *parent)
    : QObject(), QUndoCommand(parent), xml(xml), doc(doc), nameActivDraw(doc->GetNameActivDraw()),
      cursor(doc->getCursor()), redoFlag(false)
{
    setText(tr("Add object"));
}

//---------------------------------------------------------------------------------------------------------------------
AddToCalc::~AddToCalc()
{}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::undo()
{
    doc->ChangeActivPP(nameActivDraw);
    doc->setCursor(cursor);

    QDomElement calcElement;
    if (doc->GetActivNodeElement(VPattern::TagCalculation, calcElement))
    {
        calcElement.removeChild(xml);
        if (cursor > 0)
        {
            doc->setCursor(0);
        }
    }
    else
    {
        qDebug()<<"Can't find tag Calculation"<< Q_FUNC_INFO;
        return;
    }
    emit NeedFullParsing();
}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::redo()
{
    doc->ChangeActivPP(nameActivDraw);
    doc->setCursor(cursor);

    QDomElement calcElement;
    if (doc->GetActivNodeElement(VPattern::TagCalculation, calcElement))
    {
        if (cursor <= 0)
        {
            calcElement.appendChild(xml);
        }
        else
        {
            QDomElement refElement = doc->elementById(QString().setNum(cursor));
            if (refElement.isElement())
            {
                calcElement.insertAfter(xml, refElement);
                doc->setCursor(0);
            }
            else
            {
                qDebug()<<"Can not find the element after which you want to insert."<< Q_FUNC_INFO;
                return;
            }
        }
    }
    else
    {
        qDebug()<<"Can't find tag Calculation"<< Q_FUNC_INFO;
        return;
    }
    if (redoFlag)
    {
        emit NeedFullParsing();
    }
    redoFlag = true;
}
