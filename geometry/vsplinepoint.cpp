/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
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
 ****************************************************************************/

#include "vsplinepoint.h"

VSplinePoint::VSplinePoint()
    :pSpline(0), angle(0), kAsm1(1), kAsm2(1){}

VSplinePoint::VSplinePoint(qint64 pSpline, qreal kAsm1, qreal angle, qreal kAsm2)
    :pSpline(pSpline), angle(angle), kAsm1(kAsm1), kAsm2(kAsm2){}

VSplinePoint::VSplinePoint(const VSplinePoint &point)
    :pSpline(point.P()), angle(point.Angle2()), kAsm1(point.KAsm1()), kAsm2(point.KAsm2()){}
