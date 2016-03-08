/************************************************************************
 **
 **  @file   vcubicbezier_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 3, 2016
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

#ifndef VCUBICBEZIER_P_H
#define VCUBICBEZIER_P_H

#include <QSharedData>
#include <QLineF>
#include <QtCore/qmath.h>

#include "vpointf.h"
#include "../vmisc/vabstractapplication.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VCubicBezierData : public QSharedData
{
public:
    VCubicBezierData();
    VCubicBezierData(const VCubicBezierData &spline);
    VCubicBezierData(const VPointF &p1, const VPointF &p2, const VPointF &p3, const VPointF &p4);
    virtual ~VCubicBezierData();

    /** @brief p1 first spline point. */
    VPointF p1;

    /** @brief p2 fourth spline point. */
    VPointF p2;

    /** @brief p3 fourth spline point. */
    VPointF p3;

    /** @brief p4 fourth spline point. */
    VPointF p4;

private:
    VCubicBezierData &operator=(const VCubicBezierData &) Q_DECL_EQ_DELETE;
};

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierData::VCubicBezierData()
    : p1(),
      p2(),
      p3(),
      p4()
{
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierData::VCubicBezierData(const VCubicBezierData &curve)
    : QSharedData(curve),
      p1(curve.p1),
      p2(curve.p2),
      p3(curve.p3),
      p4(curve.p4)
{
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierData::VCubicBezierData(const VPointF &p1, const VPointF &p2, const VPointF &p3, const VPointF &p4)
    : p1(p1),
      p2(p2),
      p3(p3),
      p4(p4)
{
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierData::~VCubicBezierData()
{
}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VCUBICBEZIER_P_H
