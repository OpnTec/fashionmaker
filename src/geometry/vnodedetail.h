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
     * @brief The NodeDetail enum
     */
    enum NodeDetail { Contour, Modeling };
    Q_DECLARE_FLAGS(NodeDetails, NodeDetail)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(NodeDetail::NodeDetails)

/**
 * @brief The VNodeDetail class
 */
class VNodeDetail
{
public:
                  /**
                   * @brief VNodeDetail
                   */
                  VNodeDetail();
                  /**
                   * @brief VNodeDetail
                   * @param id
                   * @param typeTool
                   * @param mode
                   * @param typeNode
                   * @param mx
                   * @param my
                   */
                  VNodeDetail(qint64 id, Tool::Tools typeTool, NodeDetail::NodeDetails typeNode, qreal mx = 0,
                              qreal my = 0);
                  /**
                   * @brief VNodeDetail
                   * @param node
                   */
                  VNodeDetail(const VNodeDetail &node);
                  /**
                   * @brief operator =
                   * @param node
                   * @return
                   */
                  VNodeDetail &operator=(const VNodeDetail &node);
    /**
     * @brief getId
     * @return
     */
    inline qint64 getId() const {return id;}
    /**
     * @brief setId
     * @param value
     */
    inline void   setId(const qint64 &value) {id = value;}
    /**
     * @brief getTypeTool
     * @return
     */
    inline Tool::Tools getTypeTool() const {return typeTool;}
    /**
     * @brief setTypeTool
     * @param value
     */
    inline void   setTypeTool(const Tool::Tools &value) {typeTool = value;}
    /**
     * @brief getTypeNode
     * @return
     */
    inline NodeDetail::NodeDetails getTypeNode() const {return typeNode;}
    /**
     * @brief setTypeNode
     * @param value
     */
    inline void   setTypeNode(const NodeDetail::NodeDetails &value) {typeNode = value;}
    /**
     * @brief getMx
     * @return
     */
    inline qreal  getMx() const {return mx;}
    /**
     * @brief setMx
     * @param value
     */
    inline void   setMx(const qreal &value) {mx = value;}
    /**
     * @brief getMy
     * @return
     */
    inline qreal  getMy() const {return my;}
    /**
     * @brief setMy
     * @param value
     */
    inline void   setMy(const qreal &value) {my = value;}
private:
    /**
     * @brief id
     */
    qint64        id;
    /**
     * @brief typeTool
     */
    Tool::Tools   typeTool;
    /**
     * @brief typeNode
     */
    NodeDetail::NodeDetails typeNode;
    /**
     * @brief mx
     */
    qreal         mx;
    /**
     * @brief my
     */
    qreal         my;
};

Q_DECLARE_METATYPE(VNodeDetail)

#endif // VNODEDETAIL_H
