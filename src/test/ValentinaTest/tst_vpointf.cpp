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

#include "tst_vpointf.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/logging.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VPointF::TST_VPointF(QObject *parent)
    : QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPointF::TestFlip_data()
{
    QTest::addColumn<VPointF>("originPoint");
    QTest::addColumn<QLineF>("axis");
    QTest::addColumn<QPointF>("flipped");
    QTest::addColumn<QString>("prefix");

    VPointF originPoint;
    QLineF axis = QLineF(QPointF(5, 0), QPointF(5, 10));
    QPointF flipped = QPointF(10, 0);

    QTest::newRow("Vertical axis") << originPoint << axis << flipped << "a2";

    axis = QLineF(QPointF(0, 5), QPointF(10, 5));
    flipped = QPointF(0, 10);

    QTest::newRow("Horizontal axis") << originPoint << axis << flipped << "a2";

    QLineF l = QLineF(QPointF(), QPointF(10, 0));
    l.setAngle(315);
    flipped = l.p2();
    l.setLength(l.length()/2.0);

    axis = QLineF(l.p2(), l.p1());
    axis.setAngle(axis.angle()+90);

    QTest::newRow("Diagonal axis") << originPoint << axis << flipped << "a2";
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPointF::TestFlip()
{
    QFETCH(VPointF, originPoint);
    QFETCH(QLineF, axis);
    QFETCH(QPointF, flipped);
    QFETCH(QString, prefix);

    const VPointF res = originPoint.Flip(axis, prefix);

    const QString errorMsg = QString("The name doesn't contain the prefix '%1'.").arg(prefix);
    QVERIFY2(res.name().endsWith(prefix), qUtf8Printable(errorMsg));

    QCOMPARE(flipped.toPoint(), res.toQPointF().toPoint());
}

