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
#include "../vtools/tools/drawTools/toolpoint/tooldoublepoint/vtooltruedarts.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoollineintersectaxis.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/vtooltriangle.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolshoulderpoint.h"

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

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestTrueDarts_data()
{
    QTest::addColumn<QPointF>("baseLineP1");
    QTest::addColumn<QPointF>("baseLineP2");
    QTest::addColumn<QPointF>("dartP1");
    QTest::addColumn<QPointF>("dartP2");
    QTest::addColumn<QPointF>("dartP3");
    QTest::addColumn<QPointF>("expectP1");
    QTest::addColumn<QPointF>("expectP2");

    const QPointF baseLineP1(30.0, 3094.31433071);
    const QPointF baseLineP2(621.006962676, 3222.38611313);
    const QPointF dartP1(196.220708253, 3130.33451951);
    const QPointF dartP2(196.220708253, 3470.49199983);
    const QPointF dartP3(270.096578587, 3146.34349232);
    const QPointF p1(196.220708253, 3106.93562497);
    const QPointF p2(277.006407384, 3116.02492305);

    QTest::newRow("Real case") << baseLineP1 << baseLineP2 << dartP1 << dartP2 << dartP3 << p1 << p2;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestTrueDarts()
{
    QFETCH(QPointF, baseLineP1);
    QFETCH(QPointF, baseLineP2);
    QFETCH(QPointF, dartP1);
    QFETCH(QPointF, dartP2);
    QFETCH(QPointF, dartP3);
    QFETCH(QPointF, expectP1);
    QFETCH(QPointF, expectP2);

    QPointF p1;
    QPointF p2;

    VToolTrueDarts::FindPoint(baseLineP1, baseLineP2, dartP1, dartP2, dartP3, p1, p2);

    QCOMPARE(p1.toPoint(), expectP1.toPoint());
    QCOMPARE(p2.toPoint(), expectP2.toPoint());
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestLineIntersectAxis_data()
{
    QTest::addColumn<QLineF>("axis");
    QTest::addColumn<QLineF>("line");
    QTest::addColumn<QPointF>("point");

    const QLineF axis(0, 0, 1, 0);
    const QLineF line(0, 1, 1, 1);
    const QPointF point(0, 0);

    QTest::newRow("Parallel lines") << axis << line << point;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestLineIntersectAxis()
{
    QFETCH(QLineF, axis);
    QFETCH(QLineF, line);
    QFETCH(QPointF, point);

    QPointF resultPoint = VToolLineIntersectAxis::FindPoint(axis, line);
    QCOMPARE(point, resultPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestTriangle_data()
{
    QTest::addColumn<QPointF>("axisP1");
    QTest::addColumn<QPointF>("axisP2");
    QTest::addColumn<QPointF>("firstPoint");
    QTest::addColumn<QPointF>("secondPoint");
    QTest::addColumn<QPointF>("point");

    const QPointF axisP1(0, 0);
    const QPointF axisP2(0, 1);
    const QPointF firstPoint(1, 0);
    const QPointF secondPoint(1, 1);
    const QPointF point(0, 0);

    QTest::newRow("Parallel lines") << axisP1 << axisP2 << firstPoint << secondPoint << point;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestTriangle()
{
    QFETCH(QPointF, axisP1);
    QFETCH(QPointF, axisP2);
    QFETCH(QPointF, firstPoint);
    QFETCH(QPointF, secondPoint);
    QFETCH(QPointF, point);

    QPointF resultPoint = VToolTriangle::FindPoint(axisP1, axisP2, firstPoint, secondPoint);
    QCOMPARE(point, resultPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestShoulderPoint_data()
{
    QTest::addColumn<QPointF>("p1");
    QTest::addColumn<QPointF>("p2");
    QTest::addColumn<QPointF>("pShoulder");
    QTest::addColumn<qreal>("length");
    QTest::addColumn<QPointF>("point");

    // See file <root>/src/app/share/collection/bugs/Issue_#647.val
    QTest::newRow("Value found") << QPointF(-234.5669291338583, 39.999874015748034)
                                 << QPointF(-574.724409448819, 115.5904251968504)
                                 << QPointF(-234.5669291338583, -35.590677165354336)
                                 << 566.92913385826773
                                 << QPointF(-767.2805101289953, 158.3806697924456);

    // The same file <root>/src/app/share/collection/bugs/Issue_#647.val
    // The length changed to get default value
    QPointF p2(-574.724409448819, 115.5904251968504);
    QTest::newRow("Value not found") << QPointF(-234.5669291338583, 39.999874015748034)
                                     << p2
                                     << QPointF(-234.5669291338583, -35.590677165354336)
                                     << 75.59055118110237
                                     << p2;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestShoulderPoint()
{
    QFETCH(QPointF, p1);
    QFETCH(QPointF, p2);
    QFETCH(QPointF, pShoulder);
    QFETCH(qreal, length);
    QFETCH(QPointF, point);

    QPointF resultPoint = VToolShoulderPoint::FindPoint(p1, p2, pShoulder, length);

    QCOMPARE(point, resultPoint);
}
