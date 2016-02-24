/************************************************************************
 **
 **  @file   tst_vellipticalarc.cpp
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   12 2, 2016
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

#include "tst_vellipticalarc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vlayout/vabstractdetail.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VEllipticalArc::TST_VEllipticalArc(QObject *parent) : QObject(parent)
{}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_VEllipticalArc::CompareTwoWays()
{
    const VPointF center;
    const qreal radius1 = 100;
    const qreal radius2 = 200;
    const qreal f1 = 0;
    const qreal f2 = 90;
    const qreal rotationAngle = 0;

    const qreal h = ((radius1-radius2)*(radius1-radius2))/((radius1+radius2)*(radius1+radius2));
    const qreal length =  M_PI*(radius1+radius2)*(1+3*h/(10+qSqrt(4-3*h)))/4;

    VEllipticalArc arc1(center, radius1, radius2, f1, f2, rotationAngle);
    VEllipticalArc arc2(length, center, radius1, radius2, f1);

    const qreal eps = length*0.5/100; // computing error
    const QString errorMsg =
            QString("Difference between real and computing lengthes bigger than eps = %1.").arg(eps);
    QVERIFY2(qAbs(arc1.GetLength() - length) <= eps, qUtf8Printable(errorMsg));
    QVERIFY2(qAbs(arc2.GetLength() - length) <= eps, qUtf8Printable(errorMsg));
    QVERIFY2(qAbs(arc1.GetLength() - arc2.GetLength()) <= eps, qUtf8Printable(errorMsg));

    // compare angles
    QVERIFY2(qAbs(arc1.GetEndAngle() - arc2.GetEndAngle()) <= eps, qUtf8Printable(errorMsg));
    QVERIFY2(qAbs(arc1.GetEndAngle() - f2) <= eps, qUtf8Printable(errorMsg));
    QVERIFY2(qAbs(arc1.GetEndAngle() - f2) <= eps, qUtf8Printable(errorMsg));
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_VEllipticalArc::NegativeArc()
{
    const VPointF center;
    const qreal radius1 = 100;
    const qreal radius2 = 200;
    const qreal f1 = 1;
    const qreal f2 = 181;

    const qreal h = ((radius1-radius2)*(radius1-radius2))/((radius1+radius2)*(radius1+radius2));
    const qreal length = M_PI*(radius1+radius2)*(1+3*h/(10+qSqrt(4-3*h)))/2;
    qreal l = -length;
    VEllipticalArc arc(l, center, radius1, radius2, f1);

    const qreal eps = 1; // computing error
    const QString errorMsg =
            QString("Difference between real and computing lengthes bigger than eps = %1.").arg(eps);

    QVERIFY2(qAbs(arc.GetLength() + length) <= eps, qUtf8Printable(errorMsg));
    QVERIFY2(arc.GetEndAngle() - f2 <= eps, qUtf8Printable(errorMsg));
}

// cppcheck-suppress unusedFunction
//---------------------------------------------------------------------------------------------------------------------
void TST_VEllipticalArc::TestGetPoints_data()
{
    QTest::addColumn<qreal>("radius1");
    QTest::addColumn<qreal>("radius2");
    QTest::addColumn<qreal>("startAngle");
    QTest::addColumn<qreal>("endAngle");
    QTest::addColumn<qreal>("rotationAngle");

    QTest::newRow("Full circle: radiuses 10, 20") << 10.0 << 20.0 << 0.0 << 360.0 << 0.0;
    QTest::newRow("Full circle: radiuses 150, 200") << 150.0 << 200.0 << 0.0 << 360.0 << 0.0;
    QTest::newRow("Full circle: radiuses 1500, 1000") << 1500.0 << 1000.0 << 0.0 << 360.0 << 0.0;
    QTest::newRow("Full circle: radiuses 50000, 10000") << 50000.0 << 10000.0 << 0.0 << 360.0 << 0.0;
    QTest::newRow("Full circle: radiuses 90000, 80000") << 90000.0 << 80000.0 << 0.0 << 360.0 << 0.0;

    QTest::newRow("Arc less than 45 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 10.5 << 0.0;
    QTest::newRow("Arc less than 45 degree, radiuses 150, 50") << 150.0 << 50.0 << 0.0 << 10.5 << 0.0;
    QTest::newRow("Arc less than 45 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 0.0 << 10.5 << 0.0;
    QTest::newRow("Arc less than 45 degree, radiuses 50000, 10000") << 50000.0 << 10000.0 << 0.0 << 10.5 << 0.0;
    QTest::newRow("Arc less than 45 degree, radiuses 90000, 10000") << 90000.0 << 10000.0 << 0.0 << 10.5 << 0.0;

    QTest::newRow("Arc 45 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 45.0 << 0.0;
    QTest::newRow("Arc 45 degree, radiuses 150, 15") << 150.0 << 15.0 << 0.0 << 45.0 << 0.0;
    QTest::newRow("Arc 45 degree, radiuses 1500, 150") << 1500.0 << 150.0 << 0.0 << 45.0 << 0.0;
    QTest::newRow("Arc 45 degree, radiuses 50000, 50000") << 50000.0 << 50000.0 << 0.0 << 45.0 << 0.0;
    QTest::newRow("Arc 45 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 45.0 << 0.0;

    QTest::newRow("Arc less than 90 degree, radiuses 100, 400") << 100.0 << 400.0 << 0.0 << 75.0 << 0.0;
    QTest::newRow("Arc less than 90 degree, radiuses 150, 400") << 150.0 << 400.0 << 0.0 << 75.0 << 0.0;
    QTest::newRow("Arc less than 90 degree, radiuses 1500, 50000") << 1500.0 << 50000.0 << 0.0 << 75.0 << 0.0;
    QTest::newRow("Arc less than 90 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 0.0 << 75.0 << 0.0;
    QTest::newRow("Arc less than 90 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 75.0 << 0.0;

    QTest::newRow("Arc 90 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 90.0 << 0.0;
    QTest::newRow("Arc 90 degree, radiuses 150, 400") << 150.0 << 400.0 << 0.0 << 90.0 << 0.0;
    QTest::newRow("Arc 90 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 0.0 << 90.0 << 0.0;
    QTest::newRow("Arc 90 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 0.0 << 90.0 << 0.0;
    QTest::newRow("Arc 90 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 90.0 << 0.0;

    QTest::newRow("Arc less than 135 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 110.6 << 0.0;
    QTest::newRow("Arc less than 135 degree, radiuses 150, 400") << 150.0 << 400.0 << 0.0 << 110.6 << 0.0;
    QTest::newRow("Arc less than 135 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 0.0 << 110.6 << 0.0;
    QTest::newRow("Arc less than 135 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 0.0 << 110.6 << 0.0;
    QTest::newRow("Arc less than 135 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 110.6 << 0.0;

    QTest::newRow("Arc 135 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 135.0 << 0.0;
    QTest::newRow("Arc 135 degree, radiuses 150, 400") << 150.0 << 400.0 << 0.0 << 135.0 << 0.0;
    QTest::newRow("Arc 135 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 0.0 << 135.0 << 0.0;
    QTest::newRow("Arc 135 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 0.0 << 135.0 << 0.0;
    QTest::newRow("Arc 135 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 135.0 << 0.0;

    QTest::newRow("Arc less than 180 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 160.7 << 0.0;
    QTest::newRow("Arc less than 180 degree, radiuses 150, 400") << 150.0 << 400.0 << 0.0 << 160.7 << 0.0;
    QTest::newRow("Arc less than 180 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 0.0 << 160.7 << 0.0;
    QTest::newRow("Arc less than 180 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 0.0 << 160.7 << 0.0;
    QTest::newRow("Arc less than 180 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 160.7 << 0.0;

    QTest::newRow("Arc 180 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 180.0 << 0.0;
    QTest::newRow("Arc 180 degree, radiuses 150, 400") << 150.0 << 400.0 << 0.0 << 180.0 << 0.0;
    QTest::newRow("Arc 180 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 0.0 << 180.0 << 0.0;
    QTest::newRow("Arc 180 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 0.0 << 180.0 << 0.0;
    QTest::newRow("Arc 180 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 180.0 << 0.0;

    QTest::newRow("Arc less than 270 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 150.3 << 0.0;
    QTest::newRow("Arc less than 270 degree, radiuses 150, 400") << 150.0 << 400.0 << 0.0 << 150.3 << 0.0;
    QTest::newRow("Arc less than 270 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 0.0 << 150.3 << 0.0;
    QTest::newRow("Arc less than 270 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 0.0 << 150.3 << 0.0;
    QTest::newRow("Arc less than 270 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 150.3 << 0.0;

    QTest::newRow("Arc 270 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 270.0 << 0.0;
    QTest::newRow("Arc 270 degree, radiuses 150, 400") << 150.0 << 400.0 << 0.0 << 270.0 << 0.0;
    QTest::newRow("Arc 270 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 0.0 << 270.0 << 0.0;
    QTest::newRow("Arc 270 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 0.0 << 270.0 << 0.0;
    QTest::newRow("Arc 270 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 270.0 << 0.0;

    QTest::newRow("Arc less than 360 degree, radiuses 100, 50") << 100.0 << 50.0 << 0.0 << 340.0 << 0.0;
    QTest::newRow("Arc less than 360 degree, radiuses 150, 400") << 150.0 << 400.0 << 0.0 << 340.0 << 0.0;
    QTest::newRow("Arc less than 360 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 0.0 << 340.0 << 0.0;
    QTest::newRow("Arc less than 360 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 0.0 << 340.0 << 0.0;
    QTest::newRow("Arc less than 360 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 0.0 << 340.0 << 0.0;

    QTest::newRow("Arc start 90 degree, angle 45 degree, radiuses 100, 50") << 100.0 << 50.0 << 90.0 << 135.0 << 0.0;
    QTest::newRow("Arc start 90 degree, angle 45 degree, radiuses 150, 400") << 150.0 << 400.0 << 90.0 << 135.0 << 0.0;
    QTest::newRow("Arc start 90 degree, angle 45 degree, radiuses 1500, 800") << 1500.0 << 800.0 << 90.0 << 135.0 << 0.0;
    QTest::newRow("Arc start 90 degree, angle 45 degree, radiuses 50000, 5000") << 50000.0 << 5000.0 << 90.0 << 135.0 << 0.0;
    QTest::newRow("Arc start 90 degree, angle 45 degree, radiuses 90000, 50000") << 90000.0 << 50000.0 << 90.0 << 135.0 << 0.0;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_VEllipticalArc::TestGetPoints()
{
    QFETCH(qreal, radius1);
    QFETCH(qreal, radius2);
    QFETCH(qreal, startAngle);
    QFETCH(qreal, endAngle);
    QFETCH(qreal, rotationAngle);

    const VPointF center;
    VEllipticalArc arc(center, radius1, radius2, startAngle, endAngle, rotationAngle);

    QVector<QPointF> points = arc.GetPoints();
    {
        qreal eps = 0.5;
        const QString errorMsg = QString("Broken the first rule, part 1. Any point must satisfy the equation of ellipse.");
        for (int i=0; i < points.size(); ++i)
        {
            QPointF p = points.at(i);
            const qreal equationRes = p.rx()*p.rx()/(radius1*radius1) + p.ry()*p.ry()/(radius2*radius2);
            const qreal diff = qAbs(equationRes - 1);
            QVERIFY2( diff <= eps, qUtf8Printable(errorMsg));
        }
    }
    {
        const qreal c = qSqrt(qAbs(radius2*radius2 - radius1*radius1));
        // distance from the center to the focus

        QPointF focus1 = center.toQPointF();
        QPointF focus2 = center.toQPointF();

        if (radius1 < radius2)
        {
            focus1.setY(focus1.ry() + c);
            QLineF line(center.toQPointF(), focus1);
            line.setAngle(line.angle() + rotationAngle);
            focus1 = line.p2();

            focus2.setY(focus2.ry() - c);
            line.setP2(focus2);
            line.setAngle(line.angle() + rotationAngle);
            focus2 = line.p2();
        }
        else
        {
            focus1.setX(focus1.rx() + c);
            QLineF line(center.toQPointF(), focus1);
            line.setAngle(line.angle() + rotationAngle);
            focus1 = line.p2();

            focus2.setX(focus2.rx() - c);
            line.setP2(focus2);
            line.setAngle(line.angle() + rotationAngle);
            focus2 = line.p2();
        }

        QPointF ellipsePoint(center.x() + radius1, center.y());
        QLineF line(center.toQPointF(), ellipsePoint);
        line.setAngle(line.angle() + rotationAngle);
        ellipsePoint = line.p2();

        const QLineF distance1(focus1, ellipsePoint);
        const QLineF distance2(focus2, ellipsePoint);

        const qreal distance = distance1.length() + distance2.length();
        const qreal eps = distance * 0.5/ 100; // computing error 0.5 % from origin distance
        for (int i=0; i < points.size(); ++i)
        {
            const QLineF rLine1(focus1, points.at(i));
            const QLineF rLine2(focus2, points.at(i));
            const qreal resultingDistance = rLine1.length()+rLine2.length();
            const qreal diff = qAbs(resultingDistance - distance);
            const QString errorMsg = QString("Broken the first rule, part 2. Distance from the any point to the focus1 plus"
                                             " distance from this point to the focus2 should be the same. Problem with point '%1'."
                                             " The disired distance is '%2', but resulting distance is '%3'. Difference is '%4' and it"
                                             " biggest than eps ('%5')").arg(i).arg(distance).arg(resultingDistance).arg(diff).arg(eps);
            QVERIFY2( diff <= eps, qUtf8Printable(errorMsg));
        }
    }
    {
        if (qFuzzyCompare(arc.AngleArc(), 360.0))
        {// calculated full ellipse square
            const qreal ellipseSquare = M_PI * radius1 * radius2;
            const qreal epsSquare = ellipseSquare * 0.24 / 100; // computing error 0.24 % from origin squere
            const qreal arcSquare = qAbs(VAbstractDetail::SumTrapezoids(points)/2.0);
            const qreal diffSquare = qAbs(ellipseSquare - arcSquare);
            const QString errorMsg1 = QString("Broken the second rule. Interpolation has too big computing error. "
                                              "Difference ='%1' bigger than eps = '%2'.").arg(diffSquare).arg(epsSquare);
            QVERIFY2(diffSquare <= epsSquare, qUtf8Printable(errorMsg1));

            // calculated full ellipse length
            const qreal h = ((radius1-radius2)*(radius1-radius2))/((radius1+radius2)*(radius1+radius2));
            const qreal ellipseLength =  M_PI*(radius1+radius2)*(1+3*h/(10+qSqrt(4-3*h)));
            const qreal epsLength = ellipseLength*0.5/100; // computing error
            VEllipticalArc arc(center, radius1, radius2, 0, 360, 0);
            const qreal arcLength = arc.GetLength();
            const qreal diffLength = qAbs(arcLength - ellipseLength);
            const QString errorMsg2 = QString("Difference between real and computing lengthes "
                                              "(diff = '%1') bigger than eps = '%2'.").arg(diffLength).arg(epsLength);
            QVERIFY2(diffLength <= epsLength, qUtf8Printable(errorMsg2));
        }
    }
}
