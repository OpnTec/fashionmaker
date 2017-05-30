/************************************************************************
 **
 **  @file   vnodedetail_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
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

#ifndef VNODEDETAIL_P_H
#define VNODEDETAIL_P_H

#include <QSharedData>
#include "../ifc/ifcdef.h"
#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VNodeDetailData : public QSharedData
{
public:
  VNodeDetailData()
    :id(NULL_ID), typeTool(Tool::NodePoint), typeNode(NodeDetail::Contour), mx(0), my(0), reverse(false)
  {}

  VNodeDetailData(quint32 id, Tool typeTool, NodeDetail typeNode, qreal mx, qreal my, bool reverse)
      :id(id), typeTool(typeTool), typeNode(typeNode), mx(mx), my(my), reverse(reverse)
  {}

  VNodeDetailData (const VNodeDetailData& node)
      :QSharedData(node), id(node.id), typeTool(node.typeTool), typeNode(node.typeNode), mx(node.mx), my(node.my),
        reverse(node.reverse)
  {}

  ~VNodeDetailData() {}

  /**
   * @brief id object id.
   */
  quint32     id;
  /**
   * @brief typeTool type of tool
   */
  Tool typeTool;
  /**
   * @brief typeNode node type.
   */
  NodeDetail typeNode;
  /**
   * @brief mx bias x axis.
   */
  qreal       mx;
  /**
   * @brief my bias y axis.
   */
  qreal       my;
  /**
   * @brief reverse true if need reverse points list for node.
   */
  bool        reverse;

private:
    VNodeDetailData &operator=(const VNodeDetailData &) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

#endif // VNODEDETAIL_P_H
