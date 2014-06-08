/************************************************************************
 **
 **  @file   vnodedetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VNODEDETAIL_H
#define VNODEDETAIL_H

#include "../options.h"

namespace NodeDetail
{
    /**
     * @brief The NodeDetail enum node can be node of contour or node modeling.
     */
    enum NodeDetail { Contour, Modeling };
    Q_DECLARE_FLAGS(NodeDetails, NodeDetail)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(NodeDetail::NodeDetails)

/**
 * @brief The VNodeDetail class keep information about detail node.
 */
class VNodeDetail
{
public:
    /**
     * @brief VNodeDetail default constructor.
     */
    VNodeDetail();
    /**
     * @brief VNodeDetail constructor.
     * @param id object id
     * @param typeTool type tool
     * @param typeNode type node
     * @param mx object bias x axis
     * @param my object bias y axis
     */
    VNodeDetail(quint32 id, Valentina::Tools typeTool, NodeDetail::NodeDetails typeNode, qreal mx = 0,
              qreal my = 0);
    /**
     * @brief VNodeDetail copy constructor
     * @param node node
     */
    VNodeDetail(const VNodeDetail &node);
    /**
     * @brief operator = assignment operator
     * @param node node
     * @return node
     */
    VNodeDetail &operator=(const VNodeDetail &node);
    /**
     * @brief getId return object id.
     * @return id.
     */
    quint32     getId() const;
    /**
     * @brief setId set object id.
     * @param value object id.
     */
    void        setId(const quint32 &value);
    /**
     * @brief getTypeTool return tool type.
     * @return tool type.
     */
    Valentina::Tools getTypeTool() const;
    /**
     * @brief setTypeTool set tool type.
     * @param value tool type.
     */
    void        setTypeTool(const Valentina::Tools &value);
    /**
     * @brief getTypeNode return node type.
     * @return node type.
     */
    NodeDetail::NodeDetails getTypeNode() const;
    /**
     * @brief setTypeNode set node type.
     * @param value node type.
     */
    void        setTypeNode(const NodeDetail::NodeDetails &value);
    /**
     * @brief getMx return object bias x axis.
     * @return bias x axis.
     */
    qreal       getMx() const;
    /**
     * @brief setMx set object bias x axis.
     * @param value bias x axis.
     */
    void        setMx(const qreal &value);
    /**
     * @brief getMy return object bias y axis.
     * @return bias y axis.
     */
    qreal       getMy() const;
    /**
     * @brief setMy set object bias y axis.
     * @param value bias y axis.
     */
    void        setMy(const qreal &value);
private:
    /**
     * @brief id object id.
     */
    quint32     id;
    /**
     * @brief typeTool type of tool
     */
    Valentina::Tools typeTool;
    /**
     * @brief typeNode node type.
     */
    NodeDetail::NodeDetails typeNode;
    /**
     * @brief mx bias x axis.
     */
    qreal       mx;
    /**
     * @brief my bias y axis.
     */
    qreal       my;
};

inline quint32 VNodeDetail::getId() const
{
    return id;
}

inline void VNodeDetail::setId(const quint32 &value)
{
    id = value;
}

inline Valentina::Tools VNodeDetail::getTypeTool() const
{
    return typeTool;
}

inline void VNodeDetail::setTypeTool(const Valentina::Tools &value)
{
    typeTool = value;
}

inline NodeDetail::NodeDetails VNodeDetail::getTypeNode() const
{
    return typeNode;
}

inline void VNodeDetail::setTypeNode(const NodeDetail::NodeDetails &value)
{
    typeNode = value;
}

inline qreal VNodeDetail::getMx() const
{
    return mx;
}

inline void VNodeDetail::setMx(const qreal &value)
{
    mx = value;
}

inline qreal VNodeDetail::getMy() const
{
    return my;
}

inline void VNodeDetail::setMy(const qreal &value)
{
    my = value;
}

Q_DECLARE_METATYPE(VNodeDetail)

#endif // VNODEDETAIL_H
