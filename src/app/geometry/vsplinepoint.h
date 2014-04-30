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

#include <QMetaType>
#include "vpointf.h"

/**
 * @brief The VSplinePoint class keep information about point in spline path. Each point have two angles and two
 * coefficient. Point represent at the same time first and last point of spline.
 */
class VSplinePoint
{
public:
    /**
     * @brief VSplinePoint default constructor.
     */
          VSplinePoint();
    /**
     * @brief VSplinePoint constructor.
     * @param pSpline spline point.
     * @param angle second angle control line.
     * @param factor coefficient of length second control line.
     */
    VSplinePoint(VPointF pSpline, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2);
    /**
     * @brief VSplinePoint copy constructor
     * @param point point
     */
    VSplinePoint(const VSplinePoint &point);
    ~VSplinePoint() {}
    /**
     * @brief P return point.
     * @return point.
     */
    VPointF P() const;
    /**
     * @brief SetP set point.
     * @param value point.
     */
    void    SetP(const VPointF &value);
    /**
     * @brief Angle1 return first angle of spline.
     * @return angle.
     */
    qreal   Angle1() const;
    /**
     * @brief SetAngle1 set first angle of spline.
     * @param value angle.
     */
    void    SetAngle1(const qreal &value);
    /**
     * @brief SetAngle2 set second angle of spline.
     * @param value angle.
     */
    void    SetAngle2(const qreal &value);
    /**
     * @brief Angle2 return second angle of spline.
     * @return angle.
     */
    qreal   Angle2() const;
    /**
     * @brief KAsm1 return coefficient of length first control line.
     * @return coefficient.
     */
    qreal   KAsm1() const;
    /**
     * @brief SetKAsm1 set coefficient of length first control line.
     * @param value coefficient.
     */
    void    SetKAsm1(const qreal &value);
    /**
     * @brief KAsm2 return coefficient of length second control line.
     * @return coefficient.
     */
    qreal   KAsm2() const;
    /**
     * @brief SetKAsm2 set coefficient of length second control line.
     * @param value coefficient.
     */
    void    SetKAsm2(const qreal &value);
protected:
    /**
     * @brief pSpline point.
     */
    VPointF        pSpline;
    /**
     * @brief angle1 first angle spline.
     */
    qreal          angle1;
    /**
     * @brief angle2 second angle spline.
     */
    qreal          angle2;
    /**
     * @brief kAsm1 coefficient of length first control line.
     */
    qreal          kAsm1;
    /**
     * @brief kAsm2 coefficient of length second control line.
     */
    qreal          kAsm2;
};

inline VPointF VSplinePoint::P() const
{
    return pSpline;
}

inline void VSplinePoint::SetP(const VPointF &value)
{
    pSpline = value;
}

inline qreal VSplinePoint::Angle1() const
{
    return angle1;
}

inline qreal VSplinePoint::Angle2() const
{
    return angle2;
}

inline qreal VSplinePoint::KAsm1() const
{
    return kAsm1;
}

inline void VSplinePoint::SetKAsm1(const qreal &value)
{
    kAsm1 = value;
}

inline qreal VSplinePoint::KAsm2() const
{
    return kAsm2;
}

inline void VSplinePoint::SetKAsm2(const qreal &value)
{
    kAsm2 = value;
}

Q_DECLARE_METATYPE(VSplinePoint)

#endif // VSPLINEPOINT_H
