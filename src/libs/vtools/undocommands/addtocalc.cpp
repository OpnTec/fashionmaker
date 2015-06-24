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

#include "addtocalc.h"
#include "../tools/vabstracttool.h"
#include "../../vwidgets/vmaingraphicsscene.h"
#include "../../vwidgets/vmaingraphicsview.h"

//---------------------------------------------------------------------------------------------------------------------
AddToCalc::AddToCalc(const QDomElement &xml, VAbstractPattern *doc, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), nameActivDraw(doc->GetNameActivPP()), cursor(doc->getCursor())
{
    setText(tr("add object"));
    nodeId = doc->GetParametrId(xml);
}

//---------------------------------------------------------------------------------------------------------------------
AddToCalc::~AddToCalc()
{}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::undo()
{
    qCDebug(vUndo, "Undo.");

    doc->ChangeActivPP(nameActivDraw);//Without this user will not see this change
    doc->setCursor(cursor);

    QDomElement calcElement;
    if (doc->GetActivNodeElement(VAbstractPattern::TagCalculation, calcElement))
    {
        QDomElement domElement = doc->elementById(nodeId);
        if (domElement.isElement())
        {
            if (calcElement.removeChild(domElement).isNull())
            {
                qCDebug(vUndo, "Can't delete node.");
                return;
            }
        }
        else
        {
            qCDebug(vUndo, "Can't get tool by id = %u.", nodeId);
            return;
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find tag Calculation.");
        return;
    }
    if (cursor > 0)
    {
        doc->setCursor(0);
    }
    emit NeedFullParsing();
    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
    doc->SetCurrentPP(nameActivDraw);//Return current pattern piece after undo
}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::redo()
{
    qCDebug(vUndo, "Redo.");

    doc->ChangeActivPP(nameActivDraw);//Without this user will not see this change
    doc->setCursor(cursor);

    QDomElement calcElement;
    if (doc->GetActivNodeElement(VAbstractPattern::TagCalculation, calcElement))
    {
        if (cursor <= 0)
        {
            calcElement.appendChild(xml);
        }
        else
        {
            QDomElement refElement = doc->elementById(cursor);
            if (refElement.isElement())
            {
                calcElement.insertAfter(xml, refElement);
                doc->setCursor(0);
            }
            else
            {
                qCDebug(vUndo, "Can not find the element after which you want to insert.");
                return;
            }
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find tag Calculation.");
        return;
    }
    RedoFullParsing();
    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::RedoFullParsing()
{
    if (redoFlag)
    {
        emit NeedFullParsing();
        doc->SetCurrentPP(nameActivDraw);//Return current pattern piece after undo
    }
    redoFlag = true;
}
