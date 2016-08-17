/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 8, 2016
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

#include "tst_vgobject.h"

#include <QtTest>

#include "../vgeometry/vgobject.h"
#include "../vmisc/def.h"

//---------------------------------------------------------------------------------------------------------------------
TST_VGObject::TST_VGObject(QObject *parent)
    :QObject(parent)
{}

//---------------------------------------------------------------------------------------------------------------------
void TST_VGObject::TestIsPointOnLineviaPDP_data()
{
    QTest::addColumn<QPointF>("p1");
    QTest::addColumn<QPointF>("p2");
    QTest::addColumn<QPointF>("t");
    QTest::addColumn<bool>("excpect");

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(483.54330708661416, 1920.763653543307);
    QTest::newRow("Point is on line, but not on segment.") << p1 << p2 << t << true;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(483.54330708661416, 2874.763653543307);
    QTest::newRow("Point is on segment. On middle.") << p1 << p2 << t << true;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(483.54330708661416, 1929.763653543307);
    QTest::newRow("Point is on segment. The end of segment.") << p1 << p2 << t << true;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(483.54330708661416 + VGObject::accuracyPointOnLine, 2874.763653543307);
    QTest::newRow("Min accuracy gap. On middle.") << p1 << p2 << t << false;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(483.54330708661416 + VGObject::accuracyPointOnLine, 1929.763653543307);
    QTest::newRow("Min accuracy gap.") << p1 << p2 << t << false;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(483.54330708661416 + VGObject::accuracyPointOnLine/2., 2874.763653543307);
    QTest::newRow("Less than min accuracy gap. On middle.") << p1 << p2 << t << true;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(483.54330708661416 + VGObject::accuracyPointOnLine/2., 1929.763653543307);
    QTest::newRow("Less than min accuracy gap.") << p1 << p2 << t << true;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(370.1574803149606, 2874.763653543307);
    QTest::newRow("Issue 534 - 3 cm gap. On middle.") << p1 << p2 << t << false;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(370.1574803149606, 1929.763653543307);
    QTest::newRow("Issue 534 - 3 cm gap.") << p1 << p2 << t << false;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(407.9527559055118, 2874.763653543307);
    QTest::newRow("Issue 534 - 2 cm gap. On middle.") << p1 << p2 << t << false;
    }

    {
    const QPointF p1(483.54330708661416, 3819.527433070866);
    const QPointF p2(483.54330708661416, 1929.763653543307);
    const QPointF t(407.9527559055118, 1929.763653543307);
    QTest::newRow("Issue 534 - 2 cm gap.") << p1 << p2 << t << false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VGObject::TestIsPointOnLineviaPDP() const
{
    QFETCH(QPointF, p1);
    QFETCH(QPointF, p2);
    QFETCH(QPointF, t);
    QFETCH(bool, excpect);

    const bool res = VGObject::IsPointOnLineviaPDP(t, p1, p2);
    QCOMPARE(res, excpect);
}

