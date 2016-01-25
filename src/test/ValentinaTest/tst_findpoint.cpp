/************************************************************************
 **
 **  @file   tst_findpoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 1, 2016
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

#include "tst_findpoint.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectioncurves.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_FindPoint::TST_FindPoint(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestPointOfIntersectionCurves_data()
{
    QTest::addColumn<QVector<QPointF>>("curve1Points");
    QTest::addColumn<QVector<QPointF>>("curve2Points");
    QTest::addColumn<int>("vCross");
    QTest::addColumn<int>("hCross");
    QTest::addColumn<QPointF>("expect");

    QVector<QPointF> points1;
    QVector<QPointF> points2;

    VCrossCurvesPoint v = VCrossCurvesPoint::HighestPoint;
    HCrossCurvesPoint h = HCrossCurvesPoint::LeftmostPoint;
    QPointF p;

    QTest::newRow("Empty lists of points") << points1 << points2 << static_cast<int>(v) << static_cast<int>(h) << p;

    points1.clear();
    points1.append(QPointF(10, 10));
    points1.append(QPointF(15, 20));
    points1.append(QPointF(10, 30));

    points2.clear();
    points2.append(QPointF(30, 10));
    points2.append(QPointF(25, 20));
    points2.append(QPointF(30, 30));
    QTest::newRow("No intersections") << points1 << points2 << static_cast<int>(v) << static_cast<int>(h) << p;

    points1.clear();
    points1.append(QPointF(10, 10));
    points1.append(QPointF(20, 20));
    points1.append(QPointF(10, 30));

    points2.clear();
    points2.append(QPointF(30, 10));
    points2.append(QPointF(20, 20));
    points2.append(QPointF(30, 30));

    p = QPointF(20, 20);
    QTest::newRow("One intersection point") << points1 << points2 << static_cast<int>(v) << static_cast<int>(h) << p;

    points1.clear();
    points1.append(QPointF(10, 10));
    points1.append(QPointF(25, 20));
    points1.append(QPointF(10, 30));

    points2.clear();
    points2.append(QPointF(25, 10));
    points2.append(QPointF(15, 20));
    points2.append(QPointF(25, 30));

    p = QPointF(19, 16);

    h = HCrossCurvesPoint::LeftmostPoint;
    QTest::newRow("Two intersection points (highest point, leftmost point)") << points1 << points2
                                                                             << static_cast<int>(v)
                                                                             << static_cast<int>(h) << p;

    h = HCrossCurvesPoint::RightmostPoint;
    QTest::newRow("Two intersection points (highest point, rightmost point)") << points1 << points2
                                                                              << static_cast<int>(v)
                                                                              << static_cast<int>(h) << p;

    v = VCrossCurvesPoint::LowestPoint;
    p = QPointF(19, 24);

    h = HCrossCurvesPoint::LeftmostPoint;
    QTest::newRow("Two intersection points (lowest point, leftmost point)") << points1 << points2
                                                                            << static_cast<int>(v)
                                                                            << static_cast<int>(h) << p;

    h = HCrossCurvesPoint::RightmostPoint;
    QTest::newRow("Two intersection points (lowest point, rightmost point)") << points1 << points2
                                                                             << static_cast<int>(v)
                                                                             << static_cast<int>(h) << p;

    points1.clear();
    points1.append(QPointF(10, 10));
    points1.append(QPointF(20, 25));
    points1.append(QPointF(30, 10));

    points2.clear();
    points2.append(QPointF(10, 30));
    points2.append(QPointF(20, 15));
    points2.append(QPointF(30, 30));

    h = HCrossCurvesPoint::LeftmostPoint;
    p = QPointF(16.6667, 20);

    v = VCrossCurvesPoint::HighestPoint;
    QTest::newRow("Two intersection points (leftmost point, highest point)") << points1 << points2
                                                                             << static_cast<int>(v)
                                                                             << static_cast<int>(h) << p;

    v = VCrossCurvesPoint::LowestPoint;
    QTest::newRow("Two intersection points (leftmost point, lowest point)") << points1 << points2
                                                                            << static_cast<int>(v)
                                                                            << static_cast<int>(h) << p;

    h = HCrossCurvesPoint::RightmostPoint;
    p = QPointF(23.3333, 20);

    v = VCrossCurvesPoint::HighestPoint;
    QTest::newRow("Two intersection points (rightmost point, highest point)") << points1 << points2
                                                                              << static_cast<int>(v)
                                                                              << static_cast<int>(h) << p;

    v = VCrossCurvesPoint::LowestPoint;
    QTest::newRow("Two intersection points (rightmost point, lowest point)") << points1 << points2
                                                                             << static_cast<int>(v)
                                                                             << static_cast<int>(h) << p;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestPointOfIntersectionCurves()
{
    QFETCH(QVector<QPointF>, curve1Points);
    QFETCH(QVector<QPointF>, curve2Points);
    QFETCH(int, vCross);
    QFETCH(int, hCross);
    QFETCH(QPointF, expect);

    const QPointF result = VToolPointOfIntersectionCurves::FindPoint(curve1Points, curve2Points,
                                                                     static_cast<VCrossCurvesPoint>(vCross),
                                                                     static_cast<HCrossCurvesPoint>(hCross));
    QCOMPARE(result.toPoint(), expect.toPoint());
}
