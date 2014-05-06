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

#include "vsplinepoint.h"
#include <QDebug>

//---------------------------------------------------------------------------------------------------------------------
VSplinePoint::VSplinePoint()
    :pSpline(VPointF()), angle1(0), angle2(180), kAsm1(1), kAsm2(1)
{}

//---------------------------------------------------------------------------------------------------------------------
VSplinePoint::VSplinePoint(VPointF pSpline, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2)
    :pSpline(pSpline), angle1(0), angle2(180), kAsm1(kAsm1), kAsm2(kAsm2)
{
    if (qFuzzyCompare(qAbs(angle1-angle2), 180) == false)
    {
        qDebug()<<"angle1 and angle2 are not equal.";
    }
    SetAngle2(angle2);
}

//---------------------------------------------------------------------------------------------------------------------
VSplinePoint::VSplinePoint(const VSplinePoint &point)
    :pSpline(point.P()), angle1(point.Angle1()), angle2(point.Angle2()), kAsm1(point.KAsm1()), kAsm2(point.KAsm2())
{}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePoint::SetAngle1(const qreal &value)
{
    QLineF line(0, 0, 100, 0);
    line.setAngle(value);
    angle1 = line.angle();

    line.setAngle(value+180);
    angle2 = line.angle();
}

//---------------------------------------------------------------------------------------------------------------------
void VSplinePoint::SetAngle2(const qreal &value)
{
    QLineF line(0, 0, 100, 0);
    line.setAngle(value);
    angle2 = line.angle();

    line.setAngle(value-180);
    angle1 = line.angle();
}
