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

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VSplinePathData : public QSharedData
{
public:

    VSplinePathData()
        : path(QVector<VSplinePoint>())
    {}

    VSplinePathData(const VSplinePathData &splPath)
        : QSharedData(splPath),
          path(splPath.path)
    {}

    virtual ~VSplinePathData();

    /**
     * @brief path list spline point.
     */
    QVector<VSplinePoint> path;

private:
    VSplinePathData &operator=(const VSplinePathData &) Q_DECL_EQ_DELETE;
};

VSplinePathData::~VSplinePathData()
{}

QT_WARNING_POP

#endif // VSPLINEPATH_P_H
