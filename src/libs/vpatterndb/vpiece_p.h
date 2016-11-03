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

#ifndef VPIECE_P_H
#define VPIECE_P_H

#include <QSharedData>
#include <QVector>

#include "../vmisc/diagnostic.h"
#include "vpiecenode.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VPieceData : public QSharedData
{
public:
    VPieceData()
        : nodes()
    {}

    VPieceData(const VPieceData &detail)
        : QSharedData(detail),
          nodes(detail.nodes)
    {}

    ~VPieceData();

    /** @brief nodes list detail nodes. */
    QVector<VPieceNode> nodes;

private:
    VPieceData &operator=(const VPieceData &) Q_DECL_EQ_DELETE;
};

VPieceData::~VPieceData()
{}

QT_WARNING_POP

#endif // VPIECE_P_H

