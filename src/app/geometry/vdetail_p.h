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
 **  Copyright (C) 2014 Valentina project
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
#include "../options.h"
#include "vnodedetail.h"

class VDetailData : public QSharedData
{
public:
    VDetailData()
        :_id(NULL_ID), nodes(QVector<VNodeDetail>()), name(QString()), mx(0), my(0), seamAllowance(true), closed(true),
          width(0)
    {}

    VDetailData(const QString &name, const QVector<VNodeDetail> &nodes)
        :_id(NULL_ID), nodes(nodes), name(name), mx(0), my(0), seamAllowance(true), closed(true), width(0)
    {}

    VDetailData(const VDetailData &detail)
        :QSharedData(detail), _id(NULL_ID), nodes(detail.nodes), name(detail.name), mx(detail.mx), my(detail.my),
          seamAllowance(detail.seamAllowance), closed(detail.closed), width(detail.width)
    {}

    ~VDetailData() {}

    /**
    * @brief _id id detail.
    */
    quint32        _id;
    /**
    * @brief nodes list detail nodes.
    */
    QVector<VNodeDetail> nodes;
    /**
    * @brief name detail name.
    */
    QString        name;
    /**
    * @brief mx bias x axis.
    */
    qreal          mx;
    /**
    * @brief my bias y axis.
    */
    qreal          my;
    /**
    * @brief seamAllowance status seamAllowance detail.
    */
    bool           seamAllowance;
    /**
    * @brief closed status equdistant detail.
    */
    bool           closed;
    /**
    * @brief width value seamAllowance in mm.
    */
    qreal          width;
};

#endif // VDETAIL_P_H
