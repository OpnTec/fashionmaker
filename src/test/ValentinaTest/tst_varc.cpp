/************************************************************************
 **
 **  @file   tst_varc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "tst_varc.h"
#include "../../libs/vgeometry/varc.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VArc::TST_VArc(QObject *parent)
    :QObject(parent)
{}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_VArc::CompareTwoWays()
{
    const VPointF center;
    const qreal radius = 100;
    const qreal f1 = 1;
    const qreal f2 = 46;
    const qreal length = M_PI*radius/180*(f2-f1);

    VArc arc1(center, radius, f1, f2);
    VArc arc2(length, center, radius, f1);

    QCOMPARE(arc1.GetLength(), length);
    QCOMPARE(arc2.GetLength(), length);

    QCOMPARE(arc1.GetLength(), arc2.GetLength());
    QCOMPARE(arc1.GetEndAngle(), arc2.GetEndAngle());

    QCOMPARE(arc1.GetEndAngle(), f2);
    QCOMPARE(arc2.GetEndAngle(), f2);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_VArc::NegativeArc()
{
    const VPointF center;
    const qreal radius = 100;
    const qreal f1 = 1;
    const qreal f2 = 316;
    const qreal length = M_PI*radius/180*45;
    VArc arc(-length, center, radius, f1);

    QCOMPARE(arc.GetLength(), -length);
    QCOMPARE(arc.GetEndAngle(), f2);
}
