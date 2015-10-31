/************************************************************************
 **
 **  @file   vsplinepoint_p.h
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

#ifndef VSPLINEPOINT_P_H
#define VSPLINEPOINT_P_H

#include <QSharedData>
#include "vpointf.h"
#include <QDebug>

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VSplinePointData : public QSharedData
{
public:

    VSplinePointData()
        :pSpline(VPointF()), angle1(0), angle2(180), kAsm1(1), kAsm2(1)
    {}

    VSplinePointData(VPointF pSpline, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2)
        :pSpline(pSpline), angle1(angle1), angle2(angle2), kAsm1(kAsm1), kAsm2(kAsm2)
    {
        if (qFuzzyCompare(qAbs(angle1-angle2), 180) == false)
        {
            qWarning()<<"angle1 and angle2 are not equal.";
            this->angle1 = angle1;
            this->angle2 = angle1 + 180;
        }
    }

    VSplinePointData(const VSplinePointData &point)
        :QSharedData(point), pSpline(point.pSpline), angle1(point.angle1), angle2(point.angle2), kAsm1(point.kAsm1),
          kAsm2(point.kAsm2)
    {}

    virtual ~VSplinePointData();

    /** @brief pSpline point. */
    VPointF        pSpline;

    /** @brief angle1 first angle spline. */
    qreal          angle1;

    /** @brief angle2 second angle spline. */
    qreal          angle2;

    /** @brief kAsm1 coefficient of length first control line. */
    qreal          kAsm1;

    /** @brief kAsm2 coefficient of length second control line. */
    qreal          kAsm2;

private:
    VSplinePointData &operator=(const VSplinePointData &) Q_DECL_EQ_DELETE;
};

VSplinePointData::~VSplinePointData()
{}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VSPLINEPOINT_P_H
