/************************************************************************
 **
 **  @file   vsplinepoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VSPLINEPOINT_H
#define VSPLINEPOINT_H

#include "vpointf.h"

/**
 * @brief The VSplinePoint class keep information about point in spline path. Each point have two angles and two
 * coefficient. Point represent at the same time first and last point of spline.
 */
class VSplinePoint
{
public:
    VSplinePoint();
    VSplinePoint(VPointF pSpline, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2);
    VSplinePoint(const VSplinePoint &point);
    VSplinePoint &operator=(const VSplinePoint &point);
    ~VSplinePoint() {}
    VPointF P() const;
    void    SetP(const VPointF &value);
    qreal   Angle1() const;
    void    SetAngle1(const qreal &value);
    void    SetAngle2(const qreal &value);
    qreal   Angle2() const;
    qreal   KAsm1() const;
    void    SetKAsm1(const qreal &value);
    qreal   KAsm2() const;
    void    SetKAsm2(const qreal &value);
protected:
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
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief P return point.
 * @return point.
 */
inline VPointF VSplinePoint::P() const
{
    return pSpline;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetP set point.
 * @param value point.
 */
inline void VSplinePoint::SetP(const VPointF &value)
{
    pSpline = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Angle1 return first angle of spline.
 * @return angle.
 */
inline qreal VSplinePoint::Angle1() const
{
    return angle1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Angle2 return second angle of spline.
 * @return angle.
 */
inline qreal VSplinePoint::Angle2() const
{
    return angle2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief KAsm1 return coefficient of length first control line.
 * @return coefficient.
 */
inline qreal VSplinePoint::KAsm1() const
{
    return kAsm1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetKAsm1 set coefficient of length first control line.
 * @param value coefficient.
 */
inline void VSplinePoint::SetKAsm1(const qreal &value)
{
    kAsm1 = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief KAsm2 return coefficient of length second control line.
 * @return coefficient.
 */
inline qreal VSplinePoint::KAsm2() const
{
    return kAsm2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetKAsm2 set coefficient of length second control line.
 * @param value coefficient.
 */
inline void VSplinePoint::SetKAsm2(const qreal &value)
{
    kAsm2 = value;
}

Q_DECLARE_METATYPE(VSplinePoint)

#endif // VSPLINEPOINT_H
