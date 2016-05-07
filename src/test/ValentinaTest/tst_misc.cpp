/************************************************************************
 **
 **  @file   tst_misc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 10, 2015
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

#include "tst_misc.h"
#include "../vmisc/def.h"
#include "../vgeometry/vgobject.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_Misc::TST_Misc(QObject *parent)
    :QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_Misc::TestRelativeFilePath_data()
{
    QTest::addColumn<QString>("patternPath");
    QTest::addColumn<QString>("absoluteMPath");
    QTest::addColumn<QString>("output");

    QTest::newRow("Measurements one level above")
            << "/home/user/patterns/pattern.val" << "/home/user/measurements/m.vit" << "../measurements/m.vit";

    QTest::newRow("Measurements one level under")
            << "/home/user/patterns/pattern.val" << "/home/user/patterns/measurements/m.vit" << "measurements/m.vit";

    QTest::newRow("Measurements in the same folder")
            << "/home/user/patterns/pattern.val" << "/home/user/patterns/m.vit" << "m.vit";

    QTest::newRow("Path to measurements is empty")
            << "/home/user/patterns/pattern.val" << "" << "";

    QTest::newRow("Path to a pattern file is empty. Ablosute measurements path.")
            << "" << "/home/user/patterns/m.vit" << "/home/user/patterns/m.vit";

    QTest::newRow("Path to a pattern file is empty. Relative measurements path.")
            << "" << "measurements/m.vit" << "measurements/m.vit";

    QTest::newRow("Relative measurements path.")
            << "/home/user/patterns/pattern.val" << "../measurements/m.vit" << "../measurements/m.vit";

    QTest::newRow("Both paths are empty") << "" << "" << "";

    QTest::newRow("Path to measurements is relative")
            << "/home/user/patterns/pattern.val" << "m.vit" << "m.vit";

    QTest::newRow("Absolute pattern path.") << "/home/user/patterns" << "m.vit" << "m.vit";
}

//---------------------------------------------------------------------------------------------------------------------
void TST_Misc::TestRelativeFilePath()
{
    QFETCH(QString, patternPath);
    QFETCH(QString, absoluteMPath);
    QFETCH(QString, output);

    const QString result = RelativeMPath(patternPath, absoluteMPath);
    QCOMPARE(output, result);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_Misc::TestAbsoluteFilePath_data()
{
    QTest::addColumn<QString>("patternPath");
    QTest::addColumn<QString>("relativeMPath");
    QTest::addColumn<QString>("output");

    QTest::newRow("Measurements one level above")
            << "/home/user/patterns/pattern.val" << "../measurements/m.vit" << "/home/user/measurements/m.vit";

    QTest::newRow("Measurements one level under")
            << "/home/user/patterns/pattern.val" << "measurements/m.vit" << "/home/user/patterns/measurements/m.vit";

    QTest::newRow("Measurements in the same folder")
            << "/home/user/patterns/pattern.val" << "m.vit" << "/home/user/patterns/m.vit";

    QTest::newRow("Path to measurements is empty")
            << "/home/user/patterns/pattern.val" << "" << "";

    QTest::newRow("Path to a pattern file is empty. Ablosute measurements path.")
            << "" << "/home/user/patterns/m.vit" << "/home/user/patterns/m.vit";

    QTest::newRow("Path to a pattern file is empty. Relative measurements path.")
            << "" << "measurements/m.vit" << "measurements/m.vit";

    QTest::newRow("Relative measurements path.")
            << "/home/user/patterns/pattern.val" << "../measurements/m.vit" << "/home/user/measurements/m.vit";

    QTest::newRow("Both paths are empty") << "" << "" << "";

    QTest::newRow("Path to measurements is relative")
            << "/home/user/patterns/pattern.val" << "m.vit" << "/home/user/patterns/m.vit";

    QTest::newRow("Absolute pattern path.") << "/home/user/patterns" << "m.vit" << "/home/user/m.vit";
}

//---------------------------------------------------------------------------------------------------------------------
void TST_Misc::TestAbsoluteFilePath()
{
    QFETCH(QString, patternPath);
    QFETCH(QString, relativeMPath);
    QFETCH(QString, output);

    const QString result = AbsoluteMPath(patternPath, relativeMPath);
    QCOMPARE(output, result);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_Misc::TestCLocale_data()
{
    QTest::addColumn<qreal>("number");
    QTest::addColumn<QString>("expected");

    QTest::newRow("10000") << 10000.0 << "10000";
    QTest::newRow("10000.5") << 10000.5 << "10000.5";
}

//---------------------------------------------------------------------------------------------------------------------
// Need for testing thousand separator in the C locale.
// Better be sure that the C locale have not thousand separator
void TST_Misc::TestCLocale()
{
    QFETCH(qreal, number);
    QFETCH(QString, expected);

    const QString localized = QString().number(number);

    QCOMPARE(localized, expected);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_Misc::TestIssue485()
{
    const qreal radius = 5.6692913385826778;
    const QPointF cPoint(407.9527559055118, 39.999874015748034);
    const QPointF sPoint(407.9527559055118, 39.999874015748034);

    QPointF p1, p2;
    const int res = VGObject::LineIntersectCircle(QPointF(), radius, QLineF(QPointF(), sPoint-cPoint), p1, p2);
    QCOMPARE(res, 0);
}
