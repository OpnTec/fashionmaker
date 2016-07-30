/************************************************************************
 **
 **  @file   vdetail_p.h
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

#ifndef VDETAIL_P_H
#define VDETAIL_P_H

#include <QSharedData>
#include "vnodedetail.h"
#include "vpatternpiecedata.h"
#include "vpatterninfogeometry.h"
#include "../ifc/ifcdef.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VDetailData : public QSharedData
{
public:
    VDetailData()
        :_id(NULL_ID), nodes(QVector<VNodeDetail>()), mx(0), my(0), inLayout(true)
    {}

    explicit VDetailData(const QVector<VNodeDetail> &nodes)
        :_id(NULL_ID), nodes(nodes), mx(0), my(0), inLayout(true)
    {}

    VDetailData(const VDetailData &detail)
        :QSharedData(detail), _id(NULL_ID), nodes(detail.nodes), mx(detail.mx), my(detail.my),
          m_ppData(detail.m_ppData), m_piPatternInfo(detail.m_piPatternInfo), inLayout(detail.inLayout)
    {}

    ~VDetailData() {}

    /** @brief _id id detail. */
    quint32        _id;

    /** @brief nodes list detail nodes. */
    QVector<VNodeDetail> nodes;

    /** @brief mx bias x axis. */
    qreal          mx;

    /** @brief my bias y axis. */
    qreal          my;

    /** @brief Pattern piece data */
    VPatternPieceData m_ppData;
    /** @brief Pattern info coordinates */
    VPatternInfoGeometry m_piPatternInfo;

    bool           inLayout;

private:
    VDetailData &operator=(const VDetailData &) Q_DECL_EQ_DELETE;
};

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

#endif // VDETAIL_P_H
