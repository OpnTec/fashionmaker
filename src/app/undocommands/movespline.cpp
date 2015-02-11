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

#include "movespline.h"
#include "../tools/vabstracttool.h"
#include <QDomElement>
#include <QGraphicsView>
#include "../xml/vpattern.h"

//---------------------------------------------------------------------------------------------------------------------
MoveSpline::MoveSpline(VPattern *doc, const VSpline *oldSpl, const VSpline &newSpl, const quint32 &id,
                       QGraphicsScene *scene, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), oldSpline(*oldSpl), newSpline(newSpl), scene(scene)
{
    setText(tr("Move spline"));
    nodeId = id;

    SCASSERT(scene != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
MoveSpline::~MoveSpline()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveSpline::undo()
{
    qCDebug(vUndo)<<"Undo.";

    Do(oldSpline);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSpline::redo()
{
    qCDebug(vUndo)<<"Redo.";

    Do(newSpline);
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
        doc->SetAttribute(domElement, VAbstractTool::AttrAngle1, QString().setNum(spl.GetAngle1()));
        doc->SetAttribute(domElement, VAbstractTool::AttrAngle2, QString().setNum(spl.GetAngle2()));
        doc->SetAttribute(domElement, VAbstractTool::AttrKAsm1, QString().setNum(spl.GetKasm1()));
        doc->SetAttribute(domElement, VAbstractTool::AttrKAsm2, QString().setNum(spl.GetKasm2()));
        doc->SetAttribute(domElement, VAbstractTool::AttrKCurve, QString().setNum(spl.GetKcurve()));

        emit NeedLiteParsing(Document::LiteParse);

        QList<QGraphicsView*> list = scene->views();
        VAbstractTool::NewSceneRect(scene, list[0]);
    }
    else
    {
        qCDebug(vUndo)<<"Can't find spline with id ="<<nodeId<<".";
        return;
    }
}
