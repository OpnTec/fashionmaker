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
#include "../tools/vabstracttool.h"
#include "../core/vapplication.h"
#include "../widgets/vmaingraphicsscene.h"
#include "../widgets/vmaingraphicsview.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(vUndoAddToCalc, "v.undo.add.to.calc")

//---------------------------------------------------------------------------------------------------------------------
AddToCalc::AddToCalc(const QDomElement &xml, VPattern *doc, QUndoCommand *parent)
    : VUndoCommand(xml, doc, parent), nameActivDraw(doc->GetNameActivPP()), cursor(doc->getCursor())
{
    setText(tr("Add object"));
    nodeId = doc->GetParametrId(xml);
}

//---------------------------------------------------------------------------------------------------------------------
AddToCalc::~AddToCalc()
{}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::undo()
{
    qCDebug(vUndoAddToCalc)<<"Undo.";

    doc->ChangeActivPP(nameActivDraw);//User will not see this change
    doc->setCursor(cursor);

    QDomElement calcElement;
    if (doc->GetActivNodeElement(VPattern::TagCalculation, calcElement))
    {
        QDomElement domElement = doc->elementById(QString().setNum(nodeId));
        if (domElement.isElement())
        {
            if (calcElement.removeChild(domElement).isNull())
            {
                qCDebug(vUndoAddToCalc)<<"Can't delete node.";
                return;
            }
        }
        else
        {
            qCDebug(vUndoAddToCalc)<<"Can't get tool by id = "<<nodeId<<".";
            return;
        }
    }
    else
    {
        qCDebug(vUndoAddToCalc)<<"Can't find tag Calculation.";
        return;
    }
    if (cursor > 0)
    {
        doc->setCursor(0);
    }
    emit NeedFullParsing();
    VAbstractTool::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
    doc->SetCurrentPP(nameActivDraw);//Return current pattern piece after undo
}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::redo()
{
    qCDebug(vUndoAddToCalc)<<"Redo.";

    doc->ChangeActivPP(nameActivDraw);//User will not see this change
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
                qCDebug(vUndoAddToCalc)<<"Can not find the element after which you want to insert.";
                return;
            }
        }
    }
    else
    {
        qCDebug(vUndoAddToCalc)<<"Can't find tag Calculation.";
        return;
    }
    RedoFullParsing();
    VAbstractTool::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
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
