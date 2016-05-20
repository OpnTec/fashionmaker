/************************************************************************
 **
 **  @file   movespline.cpp
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

#include "movespline.h"
#include "../tools/vabstracttool.h"
#include "../vwidgets/vmaingraphicsview.h"

#include <QDomElement>
#include <QGraphicsView>

//---------------------------------------------------------------------------------------------------------------------
MoveSpline::MoveSpline(VAbstractPattern *doc, const VSpline *oldSpl, const VSpline &newSpl, const quint32 &id,
                       QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      oldSpline(*oldSpl),
      newSpline(newSpl),
      scene(qApp->getCurrentScene())
{
    setText(tr("move spline"));
    nodeId = id;

    SCASSERT(scene != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
MoveSpline::~MoveSpline()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveSpline::undo()
{
    qCDebug(vUndo, "Undo.");

    Do(oldSpline);
    VMainGraphicsView::NewSceneRect(scene, qApp->getSceneView());
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSpline::redo()
{
    qCDebug(vUndo, "Redo.");

    Do(newSpline);
    VMainGraphicsView::NewSceneRect(scene, qApp->getSceneView());
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveSpline::mergeWith(const QUndoCommand *command)
{
    const MoveSpline *moveCommand = static_cast<const MoveSpline *>(command);
    SCASSERT(moveCommand != nullptr);
    const quint32 id = moveCommand->getSplineId();

    if (id != nodeId)
    {
        return false;
    }

    newSpline = moveCommand->getNewSpline();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int MoveSpline::id() const
{
    return static_cast<int>(UndoCommand::MoveSpline);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSpline::Do(const VSpline &spl)
{
    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrPoint1,  spl.GetP1().id());
        doc->SetAttribute(domElement, AttrPoint4,  spl.GetP4().id());
        doc->SetAttribute(domElement, AttrAngle1,  spl.GetStartAngleFormula());
        doc->SetAttribute(domElement, AttrAngle2,  spl.GetEndAngleFormula());
        doc->SetAttribute(domElement, AttrLength1, spl.GetC1LengthFormula());
        doc->SetAttribute(domElement, AttrLength2, spl.GetC2LengthFormula());

        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qCDebug(vUndo, "Can't find spline with id = %u.", nodeId);
        return;
    }
}
