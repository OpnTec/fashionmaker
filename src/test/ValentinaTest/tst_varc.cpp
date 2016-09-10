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
#include "../vgeometry/varc.h"
#include "../vlayout/vabstractdetail.h"
#include "../vmisc/logging.h"

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

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_VArc::TestGetPoints_data()
{
    QTest::addColumn<qreal>("radius");
    QTest::addColumn<qreal>("startAngle");
    QTest::addColumn<qreal>("endAngle");

    QTest::newRow("Full circle: radius 10") << 10.0 << 0.0 << 360.0;
    QTest::newRow("Full circle: radius 150") << 150.0 << 0.0 << 360.0;
    QTest::newRow("Full circle: radius 1500") << 1500.0 << 0.0 << 360.0;
    QTest::newRow("Full circle: radius 50000") << 50000.0 << 0.0 << 360.0;
    QTest::newRow("Full circle: radius 90000") << 90000.0 << 0.0 << 360.0;

    QTest::newRow("Arc less than 45 degree, radius 100") << 100.0 << 0.0 << 10.5;
    QTest::newRow("Arc less than 45 degree, radius 150") << 150.0 << 0.0 << 10.5;
    QTest::newRow("Arc less than 45 degree, radius 1500") << 1500.0 << 0.0 << 10.5;
    QTest::newRow("Arc less than 45 degree, radius 50000") << 50000.0 << 0.0 << 10.5;
    QTest::newRow("Arc less than 45 degree, radius 90000") << 90000.0 << 0.0 << 10.5;

    QTest::newRow("Arc 45 degree, radius 100") << 100.0 << 0.0 << 45.0;
    QTest::newRow("Arc 45 degree, radius 150") << 150.0 << 0.0 << 45.0;
    QTest::newRow("Arc 45 degree, radius 1500") << 1500.0 << 0.0 << 45.0;
    QTest::newRow("Arc 45 degree, radius 50000") << 50000.0 << 0.0 << 45.0;
    QTest::newRow("Arc 45 degree, radius 90000") << 90000.0 << 0.0 << 45.0;

    QTest::newRow("Arc less than 90 degree, radius 100") << 100.0 << 0.0 << 75.0;
    QTest::newRow("Arc less than 90 degree, radius 150") << 150.0 << 0.0 << 75.0;
    QTest::newRow("Arc less than 90 degree, radius 1500") << 1500.0 << 0.0 << 75.0;
    QTest::newRow("Arc less than 90 degree, radius 50000") << 50000.0 << 0.0 << 75.0;
    QTest::newRow("Arc less than 90 degree, radius 90000") << 90000.0 << 0.0 << 75.0;

    QTest::newRow("Arc 90 degree, radius 100") << 100.0 << 0.0 << 90.0;
    QTest::newRow("Arc 90 degree, radius 150") << 150.0 << 0.0 << 90.0;
    QTest::newRow("Arc 90 degree, radius 1500") << 1500.0 << 0.0 << 90.0;
    QTest::newRow("Arc 90 degree, radius 50000") << 50000.0 << 0.0 << 90.0;
    QTest::newRow("Arc 90 degree, radius 90000") << 90000.0 << 0.0 << 90.0;

    QTest::newRow("Arc less than 135 degree, radius 100") << 100.0 << 0.0 << 110.6;
    QTest::newRow("Arc less than 135 degree, radius 150") << 150.0 << 0.0 << 110.6;
    QTest::newRow("Arc less than 135 degree, radius 1500") << 1500.0 << 0.0 << 110.6;
    QTest::newRow("Arc less than 135 degree, radius 50000") << 50000.0 << 0.0 << 110.6;
    QTest::newRow("Arc less than 135 degree, radius 90000") << 90000.0 << 0.0 << 110.6;

    QTest::newRow("Arc 135 degree, radius 100") << 100.0 << 0.0 << 135.0;
    QTest::newRow("Arc 135 degree, radius 150") << 150.0 << 0.0 << 135.0;
    QTest::newRow("Arc 135 degree, radius 1500") << 1500.0 << 0.0 << 135.0;
    QTest::newRow("Arc 135 degree, radius 50000") << 50000.0 << 0.0 << 135.0;
    QTest::newRow("Arc 135 degree, radius 90000") << 90000.0 << 0.0 << 135.0;

    QTest::newRow("Arc less than 180 degree, radius 100") << 100.0 << 0.0 << 160.7;
    QTest::newRow("Arc less than 180 degree, radius 150") << 150.0 << 0.0 << 160.7;
    QTest::newRow("Arc less than 180 degree, radius 1500") << 1500.0 << 0.0 << 160.7;
    QTest::newRow("Arc less than 180 degree, radius 50000") << 50000.0 << 0.0 << 160.7;
    QTest::newRow("Arc less than 180 degree, radius 90000") << 90000.0 << 0.0 << 160.7;

    QTest::newRow("Arc 180 degree, radius 100") << 100.0 << 0.0 << 180.0;
    QTest::newRow("Arc 180 degree, radius 150") << 150.0 << 0.0 << 180.0;
    QTest::newRow("Arc 180 degree, radius 1500") << 1500.0 << 0.0 << 180.0;
    QTest::newRow("Arc 180 degree, radius 50000") << 50000.0 << 0.0 << 180.0;
    QTest::newRow("Arc 180 degree, radius 90000") << 90000.0 << 0.0 << 180.0;

    QTest::newRow("Arc less than 270 degree, radius 100") << 100.0 << 0.0 << 150.3;
    QTest::newRow("Arc less than 270 degree, radius 150") << 150.0 << 0.0 << 150.3;
    QTest::newRow("Arc less than 270 degree, radius 1500") << 1500.0 << 0.0 << 150.3;
    QTest::newRow("Arc less than 270 degree, radius 50000") << 50000.0 << 0.0 << 150.3;
    QTest::newRow("Arc less than 270 degree, radius 90000") << 90000.0 << 0.0 << 150.3;

    QTest::newRow("Arc 270 degree, radius 100") << 100.0 << 0.0 << 270.0;
    QTest::newRow("Arc 270 degree, radius 150") << 150.0 << 0.0 << 270.0;
    QTest::newRow("Arc 270 degree, radius 1500") << 1500.0 << 0.0 << 270.0;
    QTest::newRow("Arc 270 degree, radius 50000") << 50000.0 << 0.0 << 270.0;
    QTest::newRow("Arc 270 degree, radius 90000") << 90000.0 << 0.0 << 270.0;

    QTest::newRow("Arc less than 360 degree, radius 100") << 100.0 << 0.0 << 340.0;
    QTest::newRow("Arc less than 360 degree, radius 150") << 150.0 << 0.0 << 340.0;
    QTest::newRow("Arc less than 360 degree, radius 1500") << 1500.0 << 0.0 << 340.0;
    QTest::newRow("Arc less than 360 degree, radius 50000") << 50000.0 << 0.0 << 340.0;
    QTest::newRow("Arc less than 360 degree, radius 90000") << 90000.0 << 0.0 << 340.0;

    QTest::newRow("Arc start 90 degree, angle 45 degree, radius 100") << 100.0 << 90.0 << 135.0;
    QTest::newRow("Arc start 90 degree, angle 45 degree, radius 150") << 150.0 << 90.0 << 135.0;
    QTest::newRow("Arc start 90 degree, angle 45 degree, radius 1500") << 1500.0 << 90.0 << 135.0;
    QTest::newRow("Arc start 90 degree, angle 45 degree, radius 50000") << 50000.0 << 90.0 << 135.0;
    QTest::newRow("Arc start 90 degree, angle 45 degree, radius 90000") << 90000.0 << 90.0 << 135.0;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_VArc::TestGetPoints()
{
    QFETCH(qreal, radius);
    QFETCH(qreal, startAngle);
    QFETCH(qreal, endAngle);

    const VPointF center;
    VArc arc(center, radius, startAngle, endAngle);

    QVector<QPointF> points = arc.GetPoints();

    {
        const qreal epsRadius = 1.5; // computing error

        for (int i=0; i < points.size(); ++i)
        {
            QLineF rLine(center, points.at(i));
            const qreal value = qAbs(rLine.length() - radius);
            const QString errorMsg = QString("Broken the first rule. All points should be on the same distance from "
                                             "the center. Error ='%1'.").arg(value);
            QVERIFY2(value <= epsRadius, qUtf8Printable(errorMsg));
        }
    }

    {
        qreal gSquere = 0.0;// geometry square

        if (VFuzzyComparePossibleNulls(arc.AngleArc(), 360.0))
        {// circle square
            gSquere = M_PI * radius * radius;
        }
        else
        {// sector square
            gSquere = (M_PI * radius * radius) / 360.0 * arc.AngleArc();
            points.append(center);
        }

        // calculated square
        const qreal cSquare = qAbs(VAbstractDetail::SumTrapezoids(points)/2.0);
        const qreal value = qAbs(gSquere - cSquare);
        const QString errorMsg =
                QString("Broken the second rule. Interpolation has too big computing error. Error ='%1'.").arg(value);
        const qreal epsSquere = gSquere * 0.24 / 100; // computing error 0.24 % from origin squere
        QVERIFY2(value <= epsSquere, qUtf8Printable(errorMsg));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VArc::TestRotation_data()
{
    QTest::addColumn<QPointF>("center");
    QTest::addColumn<qreal>("radius");
    QTest::addColumn<qreal>("startAngle");
    QTest::addColumn<qreal>("endAngle");
    QTest::addColumn<QPointF>("rotatePoint");
    QTest::addColumn<qreal>("degrees");
    QTest::addColumn<QString>("prefix");

    QTest::newRow("Test arc 1") << QPointF(10, 10) << 10. << 0. << 90. << QPointF() << 90. << "_r";
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VArc::TestRotation()
{
    QFETCH(QPointF, center);
    QFETCH(qreal, radius);
    QFETCH(qreal, startAngle);
    QFETCH(qreal, endAngle);
    QFETCH(QPointF, rotatePoint);
    QFETCH(qreal, degrees);
    QFETCH(QString, prefix);

    const VArc arcOrigin(VPointF(center), radius, startAngle, endAngle);
    const VArc rotatedArc = arcOrigin.Rotate(rotatePoint, degrees, prefix);

    QCOMPARE(arcOrigin.GetLength(), rotatedArc.GetLength());
    QCOMPARE(arcOrigin.AngleArc(), rotatedArc.AngleArc());
    QCOMPARE(arcOrigin.GetRadius(), rotatedArc.GetRadius());
    const QString errorMsg = QString("The name doesn't contain the prefix '%1'.").arg(prefix);
    QVERIFY2(rotatedArc.name().endsWith(prefix), qUtf8Printable(errorMsg));
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VArc::TestFlip_data()
{
    QTest::addColumn<QPointF>("center");
    QTest::addColumn<qreal>("radius");
    QTest::addColumn<qreal>("startAngle");
    QTest::addColumn<qreal>("endAngle");
    QTest::addColumn<QLineF>("axis");
    QTest::addColumn<QString>("prefix");

    const qreal radius = 5;
    QPointF center(10, 5);

    QPointF p1(10, 0);
    QPointF p2(5, 5);

    QLineF axis(QPointF(4, 6), QPointF(12, 6));

    QTest::newRow("Vertical axis") << center << radius << QLineF(center, p1).angle() << QLineF(center, p2).angle()
                                   << axis << "a2";

    p1 = QPointF(15, 5);
    p2 = QPointF(10, 0);

    axis = QLineF(QPointF(9, -1), QPointF(9, 6));

    QTest::newRow("Horizontal axis") << center << radius << QLineF(center, p1).angle() << QLineF(center, p2).angle()
                                     << axis << "a2";

    QLineF l(center.x(), center.y(), center.x() + radius, center.y());

    l.setAngle(45);
    p2 = l.p2();

    l.setAngle(225);
    p1 = l.p2();

    l.setAngle(45+90);
    l.setLength(5);

    const QPointF p1Axis = l.p2();
    axis = QLineF(p1Axis.x(), p1Axis.y(), p1Axis.x() + radius, p1Axis.y());
    axis.setAngle(45);

    QTest::newRow("Diagonal axis") << center << radius << QLineF(center, p1).angle() << QLineF(center, p2).angle()
                                   << axis << "a2";
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VArc::TestFlip()
{
    QFETCH(QPointF, center);
    QFETCH(qreal, radius);
    QFETCH(qreal, startAngle);
    QFETCH(qreal, endAngle);
    QFETCH(QLineF, axis);
    QFETCH(QString, prefix);

    VArc originArc(center, radius, startAngle, endAngle);
    const VArc res = originArc.Flip(axis, prefix);

    const QString errorMsg = QString("The name doesn't contain the prefix '%1'.").arg(prefix);
    QVERIFY2(res.name().endsWith(prefix), qUtf8Printable(errorMsg));

    QVERIFY2(res.IsFlipped(), qUtf8Printable("The arc is not flipped"));

    QCOMPARE(originArc.GetLength()*-1, res.GetLength());
    QCOMPARE(originArc.GetRadius(), res.GetRadius());
    QCOMPARE(originArc.AngleArc(), res.AngleArc());
}
