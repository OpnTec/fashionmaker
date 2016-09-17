/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 9, 2016
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

#include "tst_vsplinepath.h"
#include "../vgeometry/vsplinepath.h"
#include "../vmisc/logging.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VSplinePath::TST_VSplinePath(QObject *parent)
    : QObject (parent)
{

}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSplinePath::TestRotation_data()
{
    QTest::addColumn<QVector<VSplinePoint>>("originPoints");
    QTest::addColumn<QPointF>("originPoint");
    QTest::addColumn<qreal>("degrees");
    QTest::addColumn<QString>("prefix");
    QTest::addColumn<QVector<VSplinePoint>>("rotatedPoints");

    QVector<VSplinePoint> originPoints;

    {
        VPointF pSpline(30, 39.999874015748034, "X", 5.0000125984251973, 9.9999874015748045);
        VSplinePoint p(pSpline, 89.208600000000004, "89.2086", 269.20859999999999, "269.209", 0, "0",
                       153.33618897637794, "4.05702");
        originPoints.append(p);
    }

    {
        VPointF pSpline(198.77104389529981, 249.18158602595835, "X", 5.0000125984251973, 9.9999874015748045);
        VSplinePoint p(pSpline, 146.43199999999999, "146.432", 326.43200000000002, "326.432",
                       36.387590551181106, "0.962755", 60.978897637795278, "1.6134");
        originPoints.append(p);
    }

    {
        VPointF pSpline(820.42771653543309, 417.95262992125987, "X", 5.0000125984251973, 9.9999874015748045);
        VSplinePoint p(pSpline, 173.39500000000001, "173.395", 353.39499999999998, "353.395",
                       381.23716535433073, "10.0869", 0, "0");
        originPoints.append(p);
    }

    QVector<VSplinePoint> rotatedPoints;

    {
        VPointF pSpline(-347.55140568256883, 400.53936590535034, "X", 5.0000125984251973, 9.9999874015748045);
        VSplinePoint p(pSpline, 176.56789999999998, "176.568", 356.56790000000001, "356.568", 0, "0",
                       153.33618897637794, "4.05702");
        rotatedPoints.append(p);
    }

    {
        VPointF pSpline(-130.81610509751462, 241.58508512833563, "X", 5.0000125984251973, 9.9999874015748045);
        VSplinePoint p(pSpline, 233.79130000000001, "233.791", 53.791300000000014, "53.7913",
                       36.387590551181141, "0.962755", 60.978897637795207, "1.6134");
        rotatedPoints.append(p);
    }

    {
        VPointF pSpline(66.417055567032421, -371.63572683892357, "X", 5.0000125984251973, 9.9999874015748045);
        VSplinePoint p(pSpline, 260.7543, "260.754", 80.754300000000015, "80.7543", 381.23716535433078, "10.0869", 0,
                       "0");
        rotatedPoints.append(p);
    }


    QTest::newRow("Test spline path 1") << originPoints << QPointF(30, 417.95262992125987) << 87.359300000000005 << "a2"
                                        << rotatedPoints;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSplinePath::TestRotation()
{
    QFETCH(QVector<VSplinePoint>, originPoints);
    QFETCH(QPointF, originPoint);
    QFETCH(qreal, degrees);
    QFETCH(QString, prefix);
    QFETCH(QVector<VSplinePoint>, rotatedPoints);

    const VSplinePath rotatedPath(rotatedPoints);

    const VSplinePath originPath(originPoints);
    const VSplinePath res = originPath.Rotate(originPoint, degrees, prefix);

    QCOMPARE(originPath.GetLength(), res.GetLength());
    QCOMPARE(rotatedPath.GetLength(), res.GetLength());
    QCOMPARE(rotatedPath.CountPoints(), res.CountPoints());

    const QString errorMsg = QString("The name doesn't contain the prefix '%1'.").arg(prefix);
    QVERIFY2(res.name().endsWith(prefix), qUtf8Printable(errorMsg));

    const QVector<VSplinePoint> resPoints = res.GetSplinePath();

    for (int i=0; i < resPoints.size(); ++i)
    {
        const VSplinePoint resPoint = resPoints.at(i);
        const VSplinePoint rotatedPoint = rotatedPoints.at(i);

        QCOMPARE(rotatedPoint.Angle1(), resPoint.Angle1());
        QCOMPARE(rotatedPoint.Angle2(), resPoint.Angle2());
        QCOMPARE(rotatedPoint.Length1(), resPoint.Length1());
        QCOMPARE(rotatedPoint.Length2(), resPoint.Length2());
        QCOMPARE(rotatedPoint.P().toQPointF().toPoint(), resPoint.P().toQPointF().toPoint());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSplinePath::TestFlip_data()
{
    QTest::addColumn<QVector<VSplinePoint>>("originPoints");
    QTest::addColumn<QLineF>("axis");
    QTest::addColumn<QString>("prefix");

    QVector<VSplinePoint> originPoints;

    {
        VPointF pSpline(30, 39.999874015748034, "X", 5.0000125984251973, 9.9999874015748045);
        VSplinePoint p(pSpline, 89.208600000000004, "89.2086", 269.20859999999999, "269.209", 0, "0",
                       153.33618897637794, "4.05702");
        originPoints.append(p);
    }

    {
        VPointF pSpline(198.77104389529981, 249.18158602595835, "X", 5.0000125984251973, 9.9999874015748045);
        VSplinePoint p(pSpline, 146.43199999999999, "146.432", 326.43200000000002, "326.432",
                       36.387590551181106, "0.962755", 60.978897637795278, "1.6134");
        originPoints.append(p);
    }

    {
        VPointF pSpline(820.42771653543309, 417.95262992125987, "X", 5.0000125984251973, 9.9999874015748045);
        VSplinePoint p(pSpline, 173.39500000000001, "173.395", 353.39499999999998, "353.395",
                       381.23716535433073, "10.0869", 0, "0");
        originPoints.append(p);
    }

    QLineF axis(QPointF(0, 0), QPointF(0, 10));

    QTest::newRow("Vertical axis") << originPoints << axis << "a2";

    axis = QLineF(QPointF(0, 0), QPointF(10, 0));

    QTest::newRow("Horizontal axis") << originPoints << axis << "a2";

    axis = QLineF(QPointF(0, 0), QPointF(0, 10));
    axis.setAngle(45);

    QTest::newRow("Diagonal axis") << originPoints << axis << "a2";
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSplinePath::TestFlip()
{
    QFETCH(QVector<VSplinePoint>, originPoints);
    QFETCH(QLineF, axis);
    QFETCH(QString, prefix);

    const VSplinePath splPath(originPoints);
    const VSplinePath res = splPath.Flip(axis, prefix);

    const QString errorMsg = QString("The name doesn't contain the prefix '%1'.").arg(prefix);
    QVERIFY2(res.name().endsWith(prefix), qUtf8Printable(errorMsg));

    QCOMPARE(splPath.GetLength(), res.GetLength());
    QCOMPARE(splPath.CountPoints(), res.CountPoints());
}

