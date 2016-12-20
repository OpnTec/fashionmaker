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

#include "../ifc/xml/vabstractpattern.h"
#include "../tools/drawTools/toolcurve/vtoolsplinepath.h"
#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/def.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../vgeometry/vsplinepath.h"
#include "vundocommand.h"

class QUndoCommand;

//---------------------------------------------------------------------------------------------------------------------
MoveSplinePath::MoveSplinePath(VAbstractPattern *doc, const VSplinePath &oldSplPath, const VSplinePath &newSplPath,
                               const quint32 &id, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      oldSplinePath(oldSplPath),
      newSplinePath(newSplPath),
      scene(qApp->getCurrentScene())
{
    setText(tr("move spline path"));
    nodeId = id;

    SCASSERT(scene != nullptr)
}

//---------------------------------------------------------------------------------------------------------------------
MoveSplinePath::~MoveSplinePath()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveSplinePath::undo()
{
    qCDebug(vUndo, "Undo.");

    Do(oldSplinePath);
    VMainGraphicsView::NewSceneRect(scene, qApp->getSceneView());
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSplinePath::redo()
{
    qCDebug(vUndo, "Redo.");

    Do(newSplinePath);
    VMainGraphicsView::NewSceneRect(scene, qApp->getSceneView());
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveSplinePath::mergeWith(const QUndoCommand *command)
{
    const MoveSplinePath *moveCommand = static_cast<const MoveSplinePath *>(command);
    SCASSERT(moveCommand != nullptr)
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
        VToolSplinePath::UpdatePathPoints(doc, domElement, splPath);

        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qCDebug(vUndo, "Can't find spline path with id = %u.", nodeId);
        return;
    }
}
