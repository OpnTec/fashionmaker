/************************************************************************
 **
 **  @file   vsplinepath_p.h
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

#ifndef VSPLINEPATH_P_H
#define VSPLINEPATH_P_H

#include <QSharedData>
#include "vsplinepoint.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VSplinePathData : public QSharedData
{
public:

    VSplinePathData()
        : path(QVector<VSplinePoint>()), kCurve(1)
    {}

    explicit VSplinePathData(qreal kCurve)
        : path(QVector<VSplinePoint>()), kCurve(kCurve)
    {}

    VSplinePathData(const VSplinePathData &splPath)
        : QSharedData(splPath), path(splPath.path), kCurve(splPath.kCurve)
    {}

    virtual ~VSplinePathData();

    /**
     * @brief path list spline point.
     */
    QVector<VSplinePoint> path;
    /**
     * @brief kCurve coefficient of curvature spline.
     */
    qreal         kCurve;
};

VSplinePathData::~VSplinePathData()
{}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VSPLINEPATH_P_H
