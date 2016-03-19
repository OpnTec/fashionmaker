/************************************************************************
 **
 **  @file   vcubicbezierpath_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 3, 2016
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

#ifndef VCUBICBEZIERPATH_P_H
#define VCUBICBEZIERPATH_P_H

#include <QSharedData>

#include "vpointf.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VCubicBezierPathData : public QSharedData
{
public:

    VCubicBezierPathData()
        : path()
    {}

    VCubicBezierPathData(const VCubicBezierPathData &splPath)
        : QSharedData(splPath),
          path(splPath.path)
    {}

    virtual ~VCubicBezierPathData();

    /** @brief path list of points. */
    QVector<VPointF> path;

private:
    VCubicBezierPathData &operator=(const VCubicBezierPathData &) Q_DECL_EQ_DELETE;
};

VCubicBezierPathData::~VCubicBezierPathData()
{}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VCUBICBEZIERPATH_P_H
