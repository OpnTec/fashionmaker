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

#ifndef VSPLINEPOINT_H
#define VSPLINEPOINT_H

#include "vpointf.h"
#include <QMetaType>

class VSplinePointData;

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
    ~VSplinePoint();

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
    QSharedDataPointer<VSplinePointData> d;
};

Q_DECLARE_METATYPE(VSplinePoint)
Q_DECLARE_TYPEINFO(VSplinePoint, Q_MOVABLE_TYPE);

#endif // VSPLINEPOINT_H
