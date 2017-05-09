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

#include "movepiece.h"

#include <QDomElement>

#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/ifcdef.h"
#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/def.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "vundocommand.h"

//---------------------------------------------------------------------------------------------------------------------
MovePiece::MovePiece(VAbstractPattern *doc, const double &x, const double &y, const quint32 &id,
                     QGraphicsScene *scene, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_oldX(0.0),
      m_oldY(0.0),
      m_newX(x),
      m_newY(y),
      m_scene(scene)
{
    setText(QObject::tr("move detail"));
    nodeId = id;

    SCASSERT(scene != nullptr)
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        m_oldX = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMx, "0.0"));
        m_oldY = qApp->toPixel(doc->GetParametrDouble(domElement, AttrMy, "0.0"));
    }
    else
    {
        qCDebug(vUndo, "Can't find detail with id = %u.", nodeId);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
MovePiece::~MovePiece()
{}

//---------------------------------------------------------------------------------------------------------------------
void MovePiece::undo()
{
    qCDebug(vUndo, "Undo.");

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        SaveCoordinates(domElement, m_oldX, m_oldY);

        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qCDebug(vUndo, "Can't find detail with id = %u.", nodeId);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MovePiece::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        SaveCoordinates(domElement, m_newX, m_newY);

        if (redoFlag)
        {
            emit NeedLiteParsing(Document::LiteParse);
        }
        else
        {
            VMainGraphicsView::NewSceneRect(m_scene, qApp->getSceneView());
        }
        redoFlag = true;
    }
    else
    {
        qCDebug(vUndo, "Can't find detail with id = %u.", nodeId);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
bool MovePiece::mergeWith(const QUndoCommand *command)
{
    const MovePiece *moveCommand = static_cast<const MovePiece *>(command);
    SCASSERT(moveCommand != nullptr)
    const quint32 id = moveCommand->getDetId();

    if (id != nodeId)
    {
        return false;
    }

    m_newX = moveCommand->getNewX();
    m_newY = moveCommand->getNewY();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int MovePiece::id() const
{
    return static_cast<int>(UndoCommand::MovePiece);
}

//---------------------------------------------------------------------------------------------------------------------
void MovePiece::SaveCoordinates(QDomElement &domElement, double x, double y)
{
    doc->SetAttribute(domElement, AttrMx, QString().setNum(qApp->fromPixel(x)));
    doc->SetAttribute(domElement, AttrMy, QString().setNum(qApp->fromPixel(y)));
}
