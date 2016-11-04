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
    : id(NULL_ID),
      typeTool(Tool::NodePoint),
      reverse(false)
  {}

  VPieceNodeData(quint32 id, Tool typeTool, bool reverse)
      : id(id),
        typeTool(typeTool),
        reverse(reverse)
  {
      if (typeTool == Tool::NodePoint)
      {
          reverse = false;
      }
  }

  VPieceNodeData (const VPieceNodeData& node)
      : QSharedData(node),
        id(node.id),
        typeTool(node.typeTool),
        reverse(node.reverse)
  {}

  ~VPieceNodeData();

  /** @brief id object id. */
  quint32 id;

  /** @brief typeTool type of tool */
  Tool typeTool;

  /** @brief reverse true if need reverse points list for node. */
  bool reverse;

private:
    VPieceNodeData &operator=(const VPieceNodeData &) Q_DECL_EQ_DELETE;
};

VPieceNodeData::~VPieceNodeData()
{}

QT_WARNING_POP

#endif // VPIECENODE_P_H

