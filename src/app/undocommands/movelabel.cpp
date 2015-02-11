/************************************************************************
 **
 **  @file   movelabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 12, 2014
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

#include "movelabel.h"
#include <QGraphicsScene>
#include <QDomElement>
#include "../xml/vpattern.h"
#include "../tools/vabstracttool.h"
#include "../core/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
MoveLabel::MoveLabel(VPattern *doc, const double &x, const double &y, const quint32 &id, QGraphicsScene *scene,
                     QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), oldMx(0.0), oldMy(0.0), newMx(x), newMy(y), scene(scene)
{
    setText(tr("Move point label"));
    nodeId = id;
    qCDebug(vUndo)<<"Point id"<<nodeId;

    qCDebug(vUndo)<<"Label new Mx"<<newMx;
    qCDebug(vUndo)<<"Label new My"<<newMy;

    SCASSERT(scene != nullptr);
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        oldMx = qApp->toPixel(doc->GetParametrDouble(domElement, VAbstractTool::AttrMx, "0.0"));
        oldMy = qApp->toPixel(doc->GetParametrDouble(domElement, VAbstractTool::AttrMy, "0.0"));

        qCDebug(vUndo)<<"Label old Mx"<<oldMx;
        qCDebug(vUndo)<<"Label old My"<<oldMy;
    }
    else
    {
        qCDebug(vUndo)<<"Can't find point with id ="<<nodeId<<".";
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
MoveLabel::~MoveLabel()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveLabel::undo()
{
    qCDebug(vUndo)<<"Undo.";

    Do(oldMx, oldMy);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveLabel::redo()
{
    qCDebug(vUndo)<<"Redo.";

    Do(newMx, newMy);
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveLabel::mergeWith(const QUndoCommand *command)
{
    const MoveLabel *moveCommand = static_cast<const MoveLabel *>(command);
    SCASSERT(moveCommand != nullptr);
    const quint32 id = moveCommand->getPointId();

    if (id != nodeId)
    {
        return false;
    }

    qCDebug(vUndo)<<"Mergin undo.";
    newMx = moveCommand->getNewMx();
    newMy = moveCommand->getNewMy();
    qCDebug(vUndo)<<"Label new Mx"<<newMx;
    qCDebug(vUndo)<<"Label new My"<<newMy;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int MoveLabel::id() const
{
    return static_cast<int>(UndoCommand::MoveLabel);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveLabel::Do(double mx, double my)
{
    qCDebug(vUndo)<<"New mx"<<mx;
    qCDebug(vUndo)<<"New my"<<my;

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, VAbstractTool::AttrMx, QString().setNum(qApp->fromPixel(mx)));
        doc->SetAttribute(domElement, VAbstractTool::AttrMy, QString().setNum(qApp->fromPixel(my)));

        emit NeedLiteParsing(Document::LitePPParse);

        QList<QGraphicsView*> list = scene->views();
        VAbstractTool::NewSceneRect(scene, list[0]);
    }
    else
    {
        qCDebug(vUndo)<<"Can't find point with id ="<<nodeId<<".";
        return;
    }
}
