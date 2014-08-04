/************************************************************************
 **
 **  @file   movespoint.cpp
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

#include "movespoint.h"
#include <QGraphicsScene>
#include <QDomElement>
#include "../xml/vpattern.h"
#include "../tools/vabstracttool.h"
#include "../widgets/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
MoveSPoint::MoveSPoint(VPattern *doc, const double &x, const double &y, const quint32 &id, QGraphicsScene *scene,
                       QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), oldX(0.0), oldY(0.0), newX(x), newY(y), scene(scene)
{
    setText(tr("Move single point"));
    nodeId = id;

    SCASSERT(scene != nullptr);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        oldX = qApp->toPixel(doc->GetParametrDouble(domElement, VAbstractTool::AttrX, "0.0"));
        oldY = qApp->toPixel(doc->GetParametrDouble(domElement, VAbstractTool::AttrY, "0.0"));
    }
    else
    {
        qDebug()<<"Can't find spoint with id ="<< nodeId << Q_FUNC_INFO;
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
MoveSPoint::~MoveSPoint()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveSPoint::undo()
{
    Do(oldX, oldY);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSPoint::redo()
{
    Do(newX, newY);
}

//---------------------------------------------------------------------------------------------------------------------
bool MoveSPoint::mergeWith(const QUndoCommand *command)
{
    const MoveSPoint *moveCommand = static_cast<const MoveSPoint *>(command);
    SCASSERT(moveCommand != nullptr);
    const quint32 id = moveCommand->getSPointId();

    if (id != nodeId)
    {
        return false;
    }

    newX = moveCommand->getNewX();
    newY = moveCommand->getNewY();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int MoveSPoint::id() const
{
    return static_cast<int>(UndoCommand::MoveSPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveSPoint::Do(double x, double y)
{
    QDomElement domElement = doc->elementById(QString().setNum(nodeId));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, VAbstractTool::AttrX, QString().setNum(qApp->fromPixel(x)));
        doc->SetAttribute(domElement, VAbstractTool::AttrY, QString().setNum(qApp->fromPixel(y)));

        emit NeedLiteParsing(Document::LitePPParse);

        QList<QGraphicsView*> list = scene->views();
        VAbstractTool::NewSceneRect(scene, list[0]);
    }
    else
    {
        qDebug()<<"Can't find spoint with id ="<< nodeId << Q_FUNC_INFO;
        return;
    }
}
