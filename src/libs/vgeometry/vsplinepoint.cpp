/************************************************************************
 **
 **  @file   vsplinepoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "vsplinepoint.h"
#include "vsplinepoint_p.h"
#include <QDebug>
#include <QLineF>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSplinePoint default constructor.
 */
VSplinePoint::VSplinePoint()
    :d(new VSplinePointData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSplinePoint constructor.
 * @param pSpline spline point.
 * @param kAsm1 coefficient of length first control line.
 * @param angle1 first angle control line.
 * @param kAsm2 coefficient of length second control line.
 * @param angle2 second angle control line.
 */
VSplinePoint::VSplinePoint(VPointF pSpline, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2)
    :d(new VSplinePointData(pSpline, kAsm1, angle1, kAsm2, angle2))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VSplinePoint copy constructor
 * @param point point
 */
VSplinePoint::VSplinePoint(const VSplinePoint &point)
    :d(point.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VSplinePoint &VSplinePoint::operator=(const VSplinePoint &point)
{
    if ( &point == this )
    {
        return *this;
    }
    d = point.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePoint::~VSplinePoint()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetAngle1 set first angle of spline.
 * @param value angle.
 */
void VSplinePoint::SetAngle1(const qreal &value)
{
    QLineF line(0, 0, 100, 0);
    line.setAngle(value);
    d->angle1 = line.angle();

    line.setAngle(d->angle1+180);
    d->angle2 = line.angle();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetAngle2 set second angle of spline.
 * @param value angle.
 */
void VSplinePoint::SetAngle2(const qreal &value)
{
    QLineF line(0, 0, 100, 0);
    line.setAngle(value);
    d->angle2 = line.angle();

    line.setAngle(d->angle2+180);
    d->angle1 = line.angle();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief P return point.
 * @return point.
 */
VPointF VSplinePoint::P() const
{
    return d->pSpline;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP set point.
 * @param value point.
 */
void VSplinePoint::SetP(const VPointF &value)
{
    d->pSpline = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Angle1 return first angle of spline.
 * @return angle.
 */
qreal VSplinePoint::Angle1() const
{
    return d->angle1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Angle2 return second angle of spline.
 * @return angle.
 */
qreal VSplinePoint::Angle2() const
{
    return d->angle2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief KAsm1 return coefficient of length first control line.
 * @return coefficient.
 */
qreal VSplinePoint::KAsm1() const
{
    return d->kAsm1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetKAsm1 set coefficient of length first control line.
 * @param value coefficient.
 */
void VSplinePoint::SetKAsm1(const qreal &value)
{
    d->kAsm1 = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief KAsm2 return coefficient of length second control line.
 * @return coefficient.
 */
qreal VSplinePoint::KAsm2() const
{
    return d->kAsm2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetKAsm2 set coefficient of length second control line.
 * @param value coefficient.
 */
void VSplinePoint::SetKAsm2(const qreal &value)
{
    d->kAsm2 = value;
}
