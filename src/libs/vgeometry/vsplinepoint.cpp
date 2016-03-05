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
#include "../qmuparser/qmutokenparser.h"

#include <QDebug>
#include <QLineF>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VFSplinePoint default constructor.
 */
VFSplinePoint::VFSplinePoint()
    :d(new VFSplinePointData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VFSplinePoint constructor.
 * @param pSpline spline point.
 * @param kAsm1 coefficient of length first control line.
 * @param angle1 first angle control line.
 * @param kAsm2 coefficient of length second control line.
 * @param angle2 second angle control line.
 */
VFSplinePoint::VFSplinePoint(VPointF pSpline, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2)
    :d(new VFSplinePointData(pSpline, kAsm1, angle1, kAsm2, angle2))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VFSplinePoint copy constructor
 * @param point point
 */
VFSplinePoint::VFSplinePoint(const VFSplinePoint &point)
    :d(point.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VFSplinePoint &VFSplinePoint::operator=(const VFSplinePoint &point)
{
    if ( &point == this )
    {
        return *this;
    }
    d = point.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VFSplinePoint::~VFSplinePoint()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetAngle1 set first angle of spline.
 * @param value angle.
 */
void VFSplinePoint::SetAngle1(const qreal &value)
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
void VFSplinePoint::SetAngle2(const qreal &value)
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
VPointF VFSplinePoint::P() const
{
    return d->pSpline;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP set point.
 * @param value point.
 */
void VFSplinePoint::SetP(const VPointF &value)
{
    d->pSpline = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Angle1 return first angle of spline.
 * @return angle.
 */
qreal VFSplinePoint::Angle1() const
{
    return d->angle1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Angle2 return second angle of spline.
 * @return angle.
 */
qreal VFSplinePoint::Angle2() const
{
    return d->angle2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief KAsm1 return coefficient of length first control line.
 * @return coefficient.
 */
qreal VFSplinePoint::KAsm1() const
{
    return d->kAsm1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetKAsm1 set coefficient of length first control line.
 * @param value coefficient.
 */
void VFSplinePoint::SetKAsm1(const qreal &value)
{
    d->kAsm1 = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief KAsm2 return coefficient of length second control line.
 * @return coefficient.
 */
qreal VFSplinePoint::KAsm2() const
{
    return d->kAsm2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetKAsm2 set coefficient of length second control line.
 * @param value coefficient.
 */
void VFSplinePoint::SetKAsm2(const qreal &value)
{
    d->kAsm2 = value;
}

//------------------------------------------VSplinePoint---------------------------------------------------------------
VSplinePoint::VSplinePoint()
    : d(new VSplinePointData)
{
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePoint::VSplinePoint(VPointF pSpline, qreal angle1, const QString &angle1F, qreal angle2, const QString &angle2F,
                           qreal length1, const QString &length1F, qreal length2, const QString &length2F)
    : d(new VSplinePointData(pSpline, angle1, angle1F, angle2, angle2F, length1, length1F, length2, length2F))
{
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePoint::VSplinePoint(const VSplinePoint &point)
    : d(point.d)
{
}

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
{
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VSplinePoint::P() const
{
    return d->pSpline;
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePoint::SetP(const VPointF &value)
{
    d->pSpline = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSplinePoint::Angle1() const
{
    return d->angle1;
}

//---------------------------------------------------------------------------------------------------------------------
QString VSplinePoint::Angle1Formula() const
{
    return d->angle1F;
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePoint::SetAngle1(const qreal &value, const QString &angle1F)
{
    d->angle1F = angle1F;

    QLineF line(0, 0, 100, 0);
    line.setAngle(value);
    d->angle1 = line.angle();

    line.setAngle(d->angle1+180);
    d->angle2 = line.angle();
    d->angle2F = QString().number(d->angle2);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSplinePoint::Angle2() const
{
    return d->angle2;
}

//---------------------------------------------------------------------------------------------------------------------
QString VSplinePoint::Angle2Formula() const
{
    return d->angle2F;
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePoint::SetAngle2(const qreal &value, const QString &angle2F)
{
    d->angle2F = angle2F;

    QLineF line(0, 0, 100, 0);
    line.setAngle(value);
    d->angle2 = line.angle();

    line.setAngle(d->angle2+180);
    d->angle1 = line.angle();
    d->angle1F = QString().number(d->angle1);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSplinePoint::Length1() const
{
    return d->length1;
}

//---------------------------------------------------------------------------------------------------------------------
QString VSplinePoint::Length1Formula() const
{
    return d->length1F;
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePoint::SetLength1(const qreal &value, const QString &length1F)
{
    d->length1 = value;
    d->length1F = length1F;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSplinePoint::Length2() const
{
    return d->length2;
}

//---------------------------------------------------------------------------------------------------------------------
QString VSplinePoint::Length2Formula() const
{
    return d->length2F;
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePoint::SetLength2(const qreal &value, const QString &length2F)
{
    d->length2 = value;
    d->length2F = length2F;
}

//---------------------------------------------------------------------------------------------------------------------
bool VSplinePoint::IsMovable() const
{
    return qmu::QmuTokenParser::IsSingle(d->angle1F) && qmu::QmuTokenParser::IsSingle(d->angle2F) &&
           qmu::QmuTokenParser::IsSingle(d->length1F) && qmu::QmuTokenParser::IsSingle(d->length2F);
}
