/************************************************************************
 **
 **  @file   movedetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 6, 2014
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

#include "movedetail.h"
#include <QGraphicsScene>
#include <QDomElement>
#include "../xml/vpattern.h"
#include "../tools/vabstracttool.h"
#include "../widgets/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
MoveDetail::MoveDetail(VPattern *doc, const double &x, const double &y, const quint32 &id, QGraphicsScene *scene,
                       QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent), oldX(0.0), oldY(0.0), newX(x), newY(y), scene(scene)
{
    setText(QObject::tr("Move detail"));
    nodeId = id;

    SCASSERT(scene != nullptr);
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        oldX = qApp->toPixel(doc->GetParametrDouble(domElement, VAbstractTool::AttrMx, "0.0"));
        oldY = qApp->toPixel(doc->GetParametrDouble(domElement, VAbstractTool::AttrMy, "0.0"));
    }
    else
    {
        qDebug()<<"Can't find detail with id ="<< nodeId << Q_FUNC_INFO;
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
MoveDetail::~MoveDetail()
{}

//---------------------------------------------------------------------------------------------------------------------
void MoveDetail::undo()
{
    QDomElement domElement = doc->elementById(QString().setNum(nodeId));
    if (domElement.isElement())
    {
        SaveCoordinates(domElement, oldX, oldY);

        emit NeedLiteParsing(Document::LiteParse);

        QList<QGraphicsView*> list = scene->views();
        VAbstractTool::NewSceneRect(scene, list[0]);
    }
    else
    {
        qDebug()<<"Can't find detail with id ="<< nodeId << Q_FUNC_INFO;
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MoveDetail::redo()
{
    QDomElement domElement = doc->elementById(QString().setNum(nodeId));
    if (domElement.isElement())
    {
        SaveCoordinates(domElement, newX, newY);

        if (redoFlag)
        {
            emit NeedLiteParsing(Document::LiteParse);
        }
        redoFlag = true;

        QList<QGraphicsView*> list = scene->views();
        VAbstractTool::NewSceneRect(scene, list[0]);
    }
    else
    {
        qDebug()<<"Can't find detail with id ="<< nodeId << Q_FUNC_INFO;
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
bool MoveDetail::mergeWith(const QUndoCommand *command)
{
    const MoveDetail *moveCommand = static_cast<const MoveDetail *>(command);
    SCASSERT(moveCommand != nullptr);
    const quint32 id = moveCommand->getDetId();

    if (id != nodeId)
    {
        return false;
    }

    newX = moveCommand->getNewX();
    newY = moveCommand->getNewY();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int MoveDetail::id() const
{
    return static_cast<int>(UndoCommand::MoveDetail);
}

//---------------------------------------------------------------------------------------------------------------------
void MoveDetail::SaveCoordinates(QDomElement &domElement, double x, double y)
{
    doc->SetAttribute(domElement, VAbstractTool::AttrMx, QString().setNum(qApp->fromPixel(x)));
    doc->SetAttribute(domElement, VAbstractTool::AttrMy, QString().setNum(qApp->fromPixel(y)));
}
