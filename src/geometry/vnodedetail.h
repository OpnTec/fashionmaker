/************************************************************************
 **
 **  @file   vnodedetail.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#include <QMetaType>
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
                  VNodeDetail(quint32 id, Tool::Tools typeTool, NodeDetail::NodeDetails typeNode, qreal mx = 0,
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
    inline quint32 getId() const {return id;}
    /**
     * @brief setId set object id.
     * @param value object id.
     */
    inline void   setId(const quint32 &value) {id = value;}
    /**
     * @brief getTypeTool return tool type.
     * @return tool type.
     */
    inline Tool::Tools getTypeTool() const {return typeTool;}
    /**
     * @brief setTypeTool set tool type.
     * @param value tool type.
     */
    inline void   setTypeTool(const Tool::Tools &value) {typeTool = value;}
    /**
     * @brief getTypeNode return node type.
     * @return node type.
     */
    inline NodeDetail::NodeDetails getTypeNode() const {return typeNode;}
    /**
     * @brief setTypeNode set node type.
     * @param value node type.
     */
    inline void   setTypeNode(const NodeDetail::NodeDetails &value) {typeNode = value;}
    /**
     * @brief getMx return object bias x axis.
     * @return bias x axis.
     */
    inline qreal  getMx() const {return mx;}
    /**
     * @brief setMx set object bias x axis.
     * @param value bias x axis.
     */
    inline void   setMx(const qreal &value) {mx = value;}
    /**
     * @brief getMy return object bias y axis.
     * @return bias y axis.
     */
    inline qreal  getMy() const {return my;}
    /**
     * @brief setMy set object bias y axis.
     * @param value bias y axis.
     */
    inline void   setMy(const qreal &value) {my = value;}
private:
    /**
     * @brief id object id.
     */
    quint32        id;
    /**
     * @brief typeTool type of tool
     */
    Tool::Tools   typeTool;
    /**
     * @brief typeNode node type.
     */
    NodeDetail::NodeDetails typeNode;
    /**
     * @brief mx bias x axis.
     */
    qreal         mx;
    /**
     * @brief my bias y axis.
     */
    qreal         my;
};

Q_DECLARE_METATYPE(VNodeDetail)

#endif // VNODEDETAIL_H
