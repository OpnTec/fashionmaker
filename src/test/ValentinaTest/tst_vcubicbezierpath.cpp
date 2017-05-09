/************************************************************************
 **
 **  @file   tst_vcubicbezierpath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 3, 2016
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

#include "tst_vcubicbezierpath.h"
#include "../vgeometry/vcubicbezierpath.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VCubicBezierPath::TST_VCubicBezierPath(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VCubicBezierPath::TestCountSubSpl_data() const
{
    QTest::addColumn<qint32>("points");
    QTest::addColumn<qint32>("countSubSpl");

    QTest::newRow("Empty")     << 0  << 0;
    QTest::newRow("1 point")   << 1  << 0;
    QTest::newRow("2 points")  << 2  << 0;
    QTest::newRow("3 points")  << 3  << 0;
    QTest::newRow("4 points")  << 4  << 1;
    QTest::newRow("5 points")  << 5  << 1;
    QTest::newRow("6 points")  << 6  << 1;
    QTest::newRow("7 points")  << 7  << 2;
    QTest::newRow("8 points")  << 8  << 2;
    QTest::newRow("9 points")  << 9  << 2;
    QTest::newRow("10 points") << 10 << 3;
    QTest::newRow("11 points") << 11 << 3;
    QTest::newRow("12 points") << 12 << 3;
    QTest::newRow("13 points") << 13 << 4;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VCubicBezierPath::TestCountSubSpl() const
{
    QFETCH(qint32, points);
    QFETCH(qint32, countSubSpl);

    const qint32 res = VCubicBezierPath::CountSubSpl(points);
    QCOMPARE(res, countSubSpl);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VCubicBezierPath::TestSubSplOffset_data() const
{
    QTest::addColumn<qint32>("subSplIndex");
    QTest::addColumn<qint32>("offset");

    QTest::newRow("Wrong index") << -1 << -1;
    QTest::newRow("Wrong index") << 0  << -1;
    QTest::newRow("1 subSpl")    << 1  << 0;
    QTest::newRow("2 subSpls")   << 2  << 3;
    QTest::newRow("3 subSpls")   << 3  << 6;
    QTest::newRow("4 subSpls")   << 4  << 9;
    QTest::newRow("5 subSpls")   << 5  << 12;
    QTest::newRow("6 subSpls")   << 6  << 15;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VCubicBezierPath::TestSubSplOffset() const
{
    QFETCH(qint32, subSplIndex);
    QFETCH(qint32, offset);

    const qint32 res = VCubicBezierPath::SubSplOffset(subSplIndex);
    QCOMPARE(res, offset);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VCubicBezierPath::TestSubSplPointsCount_data() const
{
    QTest::addColumn<qint32>("countSubSpl");
    QTest::addColumn<qint32>("points");

    QTest::newRow("Wrong count") << -1 << 0;
    QTest::newRow("Wrong count") << 0  << 0;
    QTest::newRow("1 subSpl")    << 1  << 4;
    QTest::newRow("2 subSpls")   << 2  << 7;
    QTest::newRow("3 subSpls")   << 3  << 10;
    QTest::newRow("4 subSpls")   << 4  << 13;
    QTest::newRow("5 subSpls")   << 5  << 16;
    QTest::newRow("6 subSpls")   << 6  << 19;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VCubicBezierPath::TestSubSplPointsCount() const
{
    QFETCH(qint32, countSubSpl);
    QFETCH(qint32, points);

    const qint32 res = VCubicBezierPath::SubSplPointsCount(countSubSpl);
    QCOMPARE(res, points);
}
