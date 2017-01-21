/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef VPIECENODE_P_H
#define VPIECENODE_P_H

#include <QSharedData>
#include "../ifc/ifcdef.h"
#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VPieceNodeData : public QSharedData
{
public:
    VPieceNodeData()
        : m_id(NULL_ID),
          m_typeTool(Tool::NodePoint),
          m_reverse(false),
          m_saBefore(-1),
          m_saAfter(-1),
          m_formulaWidthBefore("-1"),
          m_formulaWidthAfter("-1"),
          m_angleType(PieceNodeAngle::ByLength)
    {}

    VPieceNodeData(quint32 id, Tool typeTool, bool reverse)
        : m_id(id),
          m_typeTool(typeTool),
          m_reverse(reverse),
          m_saBefore(-1),
          m_saAfter(-1),
          m_formulaWidthBefore("-1"),
          m_formulaWidthAfter("-1"),
          m_angleType(PieceNodeAngle::ByLength)
    {
        if (m_typeTool == Tool::NodePoint)
        {
            m_reverse = false;
        }
    }

    VPieceNodeData (const VPieceNodeData& node)
        : QSharedData(node),
          m_id(node.m_id),
          m_typeTool(node.m_typeTool),
          m_reverse(node.m_reverse),
          m_saBefore(node.m_saBefore),
          m_saAfter(node.m_saAfter),
          m_formulaWidthBefore(node.m_formulaWidthBefore),
          m_formulaWidthAfter(node.m_formulaWidthAfter),
          m_angleType(node.m_angleType)
    {}

    ~VPieceNodeData();

    /** @brief id object id. */
    quint32 m_id;

    /** @brief typeTool type of tool */
    Tool m_typeTool;

    /** @brief reverse true if need reverse points list for node. */
    bool m_reverse;

    qreal m_saBefore;
    qreal m_saAfter;

    QString m_formulaWidthBefore;
    QString m_formulaWidthAfter;

    PieceNodeAngle m_angleType;

private:
    VPieceNodeData &operator=(const VPieceNodeData &) Q_DECL_EQ_DELETE;
};

VPieceNodeData::~VPieceNodeData()
{}

QT_WARNING_POP

#endif // VPIECENODE_P_H

