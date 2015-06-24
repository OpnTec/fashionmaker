/************************************************************************
 **
 **  @file   movesplinepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 6, 2014
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

#include "movesplinepath.h"
#include <QDomElement>
#include "../tools/drawTools/toolcurve/vtoolsplinepath.h"

//---------------------------------------------------------------------------------------------------------------------
MoveSplinePath::MoveSplinePath(VAbstractPattern *doc, const VSplinePath &oldSplPath, const VSplinePath &newSplPath,
                               const quint32 &id, QGraphicsScene *scene, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), oldSplinePath(oldSplPath), newSplinePath(newSplPath), scene(scene)
{
    setText(tr("move spline path"));
    nodeId = id;

    SCASSERT(scene != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
MoveSplinePath::~MoveSplinePath()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveSplinePath::undo()
{
    qCDebug(vUndo, "Undo.");

    Do(oldSplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSplinePath::redo()
{
    qCDebug(vUndo, "Redo.");

    Do(newSplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveSplinePath::mergeWith(const QUndoCommand *command)
{
    const MoveSplinePath *moveCommand = static_cast<const MoveSplinePath *>(command);
    SCASSERT(moveCommand != nullptr);
    const quint32 id = moveCommand->getSplinePathId();

    if (id != nodeId)
    {
        return false;
    }

    newSplinePath = moveCommand->getNewSplinePath();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int MoveSplinePath::id() const
{
    return static_cast<int>(UndoCommand::MoveSplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSplinePath::Do(const VSplinePath &splPath)
{
    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, VToolSplinePath::AttrKCurve, QString().setNum(splPath.GetKCurve()));
        VToolSplinePath::UpdatePathPoint(doc, domElement, splPath);

        emit NeedLiteParsing(Document::LiteParse);

        QList<QGraphicsView*> list = scene->views();
        VMainGraphicsView::NewSceneRect(scene, list[0]);
    }
    else
    {
        qCDebug(vUndo, "Can't find spline path with id = %u.", nodeId);
        return;
    }
}
