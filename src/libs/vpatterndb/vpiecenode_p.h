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
#include <QDataStream>
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
          m_excluded(false),
          m_isPassmark(false),
          m_isMainPathNode(true),
          m_formulaWidthBefore(currentSeamAllowance),
          m_formulaWidthAfter(currentSeamAllowance),
          m_angleType(PieceNodeAngle::ByLength),
          m_passmarkLineType(PassmarkLineType::OneLine),
          m_passmarkAngleType(PassmarkAngleType::Straightforward),
          m_isShowSecondPassmark(true)
    {}

    VPieceNodeData(quint32 id, Tool typeTool, bool reverse)
        : m_id(id),
          m_typeTool(typeTool),
          m_reverse(reverse),
          m_excluded(false),
          m_isPassmark(false),
          m_isMainPathNode(true),
          m_formulaWidthBefore(currentSeamAllowance),
          m_formulaWidthAfter(currentSeamAllowance),
          m_angleType(PieceNodeAngle::ByLength),
          m_passmarkLineType(PassmarkLineType::OneLine),
          m_passmarkAngleType(PassmarkAngleType::Straightforward),
          m_isShowSecondPassmark(true)
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
          m_excluded(node.m_excluded),
          m_isPassmark(node.m_isPassmark),
          m_isMainPathNode(node.m_isMainPathNode),
          m_formulaWidthBefore(node.m_formulaWidthBefore),
          m_formulaWidthAfter(node.m_formulaWidthAfter),
          m_angleType(node.m_angleType),
          m_passmarkLineType(node.m_passmarkLineType),
          m_passmarkAngleType(node.m_passmarkAngleType),
          m_isShowSecondPassmark(node.m_isShowSecondPassmark)
    {}

    ~VPieceNodeData() Q_DECL_EQ_DEFAULT;

    friend QDataStream& operator<<(QDataStream& out, const VPieceNodeData& p);
    friend QDataStream& operator>>(QDataStream& in, VPieceNodeData& p);

    /** @brief id object id. */
    quint32 m_id;

    /** @brief typeTool type of tool */
    Tool m_typeTool;

    /** @brief reverse true if need reverse points list for node. */
    bool m_reverse;

    /** @brief m_excluded true if item excluded from main path. Excluded item is not visible and also will not has
     * affect on main path. Also include to exist path items automatically setted excluded. */
    bool m_excluded;

    /** @brief m_isPassmark has sense only for points. If true to seam allowance should be added a passmark. */
    bool m_isPassmark;

    /** @brief m_isMainPathNode need fin know if allowed for this passmakr to be double. */
    bool m_isMainPathNode;

    QString m_formulaWidthBefore;
    QString m_formulaWidthAfter;

    PieceNodeAngle m_angleType;

    PassmarkLineType  m_passmarkLineType;
    PassmarkAngleType m_passmarkAngleType;

    bool m_isShowSecondPassmark;

private:
    VPieceNodeData &operator=(const VPieceNodeData &) Q_DECL_EQ_DELETE;
};

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const VPieceNodeData &p)
{
    out << p.m_id
        << static_cast<int>(p.m_typeTool)
        << p.m_reverse
        << p.m_excluded
        << p.m_isPassmark
        << p.m_formulaWidthBefore
        << p.m_formulaWidthAfter
        << static_cast<int>(p.m_angleType)
        << static_cast<int>(p.m_passmarkLineType)
        << static_cast<int>(p.m_passmarkAngleType)
        << p.m_isShowSecondPassmark;
    return out;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, VPieceNodeData &p)
{
    int typeTool = 0;
    int angleType = 0;
    int passmarkLineType = 0;
    int passmarkAngleType = 0;

    in >> p.m_id
       >> typeTool
       >> p.m_reverse
       >> p.m_excluded
       >> p.m_isPassmark
       >> p.m_formulaWidthBefore
       >> p.m_formulaWidthAfter
       >> angleType
       >> passmarkLineType
       >> passmarkAngleType
       >> p.m_isShowSecondPassmark;

    p.m_typeTool = static_cast<Tool>(typeTool);
    p.m_angleType = static_cast<PieceNodeAngle>(angleType);
    p.m_passmarkLineType = static_cast<PassmarkLineType>(passmarkLineType);
    p.m_passmarkAngleType = static_cast<PassmarkAngleType>(passmarkAngleType);

    return in;
}

QT_WARNING_POP

#endif // VPIECENODE_P_H

