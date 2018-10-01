/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 9, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "showdoublelabel.h"

#include <QDomElement>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/logging.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "../vmisc/vabstractapplication.h"
#include "../vtools/tools/drawTools/vdrawtool.h"

//---------------------------------------------------------------------------------------------------------------------
ShowDoubleLabel::ShowDoubleLabel(VAbstractPattern *doc, quint32 toolId, quint32 pointId, bool visible,
                                 ShowDoublePoint type, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      m_visible(visible),
      m_oldVisible(not visible),
      m_scene(qApp->getCurrentScene()),
      m_type(type),
      m_idTool(toolId)
{
    nodeId = pointId;
    qCDebug(vUndo, "Point id %u", nodeId);

    if (type == ShowDoublePoint::FirstPoint)
    {
        setText(tr("toggle the first dart label"));
    }
    else
    {
        setText(tr("togggle the second dart label"));
    }

    const QDomElement domElement = doc->elementById(m_idTool, VAbstractPattern::TagPoint);
    if (domElement.isElement())
    {
        if (type == ShowDoublePoint::FirstPoint)
        {
            m_oldVisible = doc->GetParametrBool(domElement, AttrShowLabel1, trueStr);
        }
        else
        {
            m_oldVisible = doc->GetParametrBool(domElement, AttrShowLabel2, trueStr);
        }
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", m_idTool);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ShowDoubleLabel::undo()
{
    qCDebug(vUndo, "Undo.");

    Do(m_oldVisible);
}

//---------------------------------------------------------------------------------------------------------------------
void ShowDoubleLabel::redo()
{
    qCDebug(vUndo, "Redo.");

    Do(m_visible);
}

//---------------------------------------------------------------------------------------------------------------------
void ShowDoubleLabel::Do(bool visible)
{
    QDomElement domElement = doc->elementById(m_idTool, VAbstractPattern::TagPoint);
    if (domElement.isElement())
    {
        if (m_type == ShowDoublePoint::FirstPoint)
        {
            doc->SetAttribute<bool>(domElement, AttrShowLabel1, visible);
        }
        else
        {
            doc->SetAttribute<bool>(domElement, AttrShowLabel2, visible);
        }

        if (VDrawTool *tool = qobject_cast<VDrawTool *>(VAbstractPattern::getTool(m_idTool)))
        {
            tool->SetLabelVisible(nodeId, visible);
        }
        VMainGraphicsView::NewSceneRect(m_scene, qApp->getSceneView());
    }
    else
    {
        qCDebug(vUndo, "Can't find point with id = %u.", m_idTool);
    }
}

