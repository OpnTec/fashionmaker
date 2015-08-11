/************************************************************************
 **
 **  @file   movedoublelabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "movedoublelabel.h"
#include "../tools/vabstracttool.h"
#include "../../vwidgets/vmaingraphicsview.h"

#include <QGraphicsScene>
#include <QDomElement>

//---------------------------------------------------------------------------------------------------------------------
MoveDoubleLabel::MoveDoubleLabel(VAbstractPattern *doc, const double &x, const double &y, DoublePoint type,
                                 const quint32 &id, QGraphicsScene *scene, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      oldMx(0.0), oldMy(0.0),
      newMx(x), newMy(y),
      scene(scene), type(type)
{
    if (type == DoublePoint::FirstPoint)
    {
        setText(tr("move the first dart label"));
    }
    else
    {
        setText(tr("move the second dart label"));
    }
    nodeId = id;
    qCDebug(vUndo, "Point id %u", nodeId);

    if (type == DoublePoint::FirstPoint)
    {
        qCDebug(vUndo, "Label new Mx1 %f", newMx);
        qCDebug(vUndo, "Label new My1 %f", newMy);
    }
    else
    {
        qCDebug(vUndo, "Label new Mx2 %f", newMx);
        qCDebug(vUndo, "Label new My2 %f", newMy);
    }

    SCASSERT(scene != nullptr);
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        if (type == DoublePoint::FirstPoint)
        {
            oldMx = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMx1, "0.0"));
            oldMy = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMy1, "0.0"));
        }
        else
        {
            oldMx = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMx2, "0.0"));
            oldMy = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMy2, "0.0"));
        }

        if (type == DoublePoint::FirstPoint)
        {
            qCDebug(vUndo, "Label old Mx1 %f", oldMx);
            qCDebug(vUndo, "Label old My1 %f", oldMy);
        }
        else
        {
            qCDebug(vUndo, "Label old Mx2 %f", oldMx);
            qCDebug(vUndo, "Label old My2 %f", oldMy);
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", nodeId);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
MoveDoubleLabel::~MoveDoubleLabel()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveDoubleLabel::undo()
{
    qCDebug(vUndo, "Undo.");

    Do(oldMx, oldMy);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveDoubleLabel::redo()
{
    qCDebug(vUndo, "Redo.");

    Do(newMx, newMy);
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveDoubleLabel::mergeWith(const QUndoCommand *command)
{
    const MoveDoubleLabel *moveCommand = static_cast<const MoveDoubleLabel *>(command);
    SCASSERT(moveCommand != nullptr);

    if (moveCommand->getPointId() != nodeId || moveCommand->getPointType() != type)
    {
        return false;
    }

    newMx = moveCommand->getNewMx();
    newMy = moveCommand->getNewMy();

    if (type == DoublePoint::FirstPoint)
    {
        qCDebug(vUndo, "Label new Mx1 %f", newMx);
        qCDebug(vUndo, "Label new My1 %f", newMy);
    }
    else
    {
        qCDebug(vUndo, "Label new Mx2 %f", newMx);
        qCDebug(vUndo, "Label new My2 %f", newMy);
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int MoveDoubleLabel::id() const
{
    return static_cast<int>(UndoCommand::MoveDoubleLabel);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveDoubleLabel::Do(double mx, double my)
{
    if (type == DoublePoint::FirstPoint)
    {
        qCDebug(vUndo, "New mx1 %f", mx);
        qCDebug(vUndo, "New my1 %f", my);
    }
    else
    {
        qCDebug(vUndo, "New mx2 %f", mx);
        qCDebug(vUndo, "New my2 %f", my);
    }

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        if (type == DoublePoint::FirstPoint)
        {
            doc->SetAttribute(domElement, AttrMx1, QString().setNum(qApp->fromPixel(mx)));
            doc->SetAttribute(domElement, AttrMy1, QString().setNum(qApp->fromPixel(my)));
        }
        else
        {
            doc->SetAttribute(domElement, AttrMx2, QString().setNum(qApp->fromPixel(mx)));
            doc->SetAttribute(domElement, AttrMy2, QString().setNum(qApp->fromPixel(my)));
        }

        emit NeedLiteParsing(Document::LitePPParse);

        QList<QGraphicsView*> list = scene->views();
        VMainGraphicsView::NewSceneRect(scene, list[0]);
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", nodeId);
        return;
    }
}
