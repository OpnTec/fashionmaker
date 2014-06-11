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

#include "movesplinepath.h"
#include <QDomElement>
#include "../tools/drawTools/vtoolsplinepath.h"

//---------------------------------------------------------------------------------------------------------------------
MoveSplinePath::MoveSplinePath(VPattern *doc, const VSplinePath &oldSplPath, const VSplinePath &newSplPath,
                               const quint32 &id, QGraphicsScene *scene, QUndoCommand *parent)
    : QObject(), QUndoCommand(parent), doc(doc), oldSplinePath(oldSplPath), newSplinePath(newSplPath), splinePathId(id),
      scene(scene)
{
    setText(tr("Move spline path"));

    SCASSERT(scene != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
MoveSplinePath::~MoveSplinePath()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveSplinePath::undo()
{
    QDomElement domElement = doc->elementById(QString().setNum(splinePathId));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, VToolSplinePath::AttrKCurve, QString().setNum(oldSplinePath.getKCurve()));
        VToolSplinePath::UpdatePathPoint(doc, domElement, oldSplinePath);

        emit NeedLiteParsing();

        QList<QGraphicsView*> list = scene->views();
        VAbstractTool::NewSceneRect(scene, list[0]);
    }
    else
    {
        qDebug()<<"Can't find spline path with id ="<< splinePathId << Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSplinePath::redo()
{
    QDomElement domElement = doc->elementById(QString().setNum(splinePathId));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, VToolSplinePath::AttrKCurve, QString().setNum(newSplinePath.getKCurve()));
        VToolSplinePath::UpdatePathPoint(doc, domElement, newSplinePath);

        emit NeedLiteParsing();

        QList<QGraphicsView*> list = scene->views();
        VAbstractTool::NewSceneRect(scene, list[0]);
    }
    else
    {
        qDebug()<<"Can't find spline path with id ="<< splinePathId << Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveSplinePath::mergeWith(const QUndoCommand *command)
{
    const MoveSplinePath *moveCommand = static_cast<const MoveSplinePath *>(command);
    SCASSERT(moveCommand != nullptr);
    const quint32 id = moveCommand->getSplinePathId();

    if (id != splinePathId)
    {
        return false;
    }

    newSplinePath = moveCommand->getNewSplinePath();
    return true;
}
