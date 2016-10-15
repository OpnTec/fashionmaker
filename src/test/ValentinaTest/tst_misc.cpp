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

    #ifdef Q_OS_WIN
    QTest::newRow("Measurements one level above")
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/pattern.val")
            << "../measurements/m.vit"
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/measurements/m.vit");

    QTest::newRow("Measurements one level above")
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/pattern.val")
            << "../measurements/m.vit"
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/measurements/m.vit");

    QTest::newRow("Measurements one level under")
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/pattern.val")
            << "measurements/m.vit"
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/measurements/m.vit");

    QTest::newRow("Measurements in the same folder")
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/pattern.val")
            << "m.vit"
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/m.vit");

    QTest::newRow("Path to measurements is empty")
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/pattern.val")
            << "" << "";

    QTest::newRow("Path to a pattern file is empty. Ablosute measurements path.")
            << ""
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/m.vit")
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/m.vit");

    QTest::newRow("Path to a pattern file is empty. Relative measurements path.")
            << ""
            << "measurements/m.vit"
            << "measurements/m.vit";

    QTest::newRow("Relative measurements path.")
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/pattern.val")
            << "../measurements/m.vit"
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/measurements/m.vit");

    QTest::newRow("Both paths are empty") << "" << "" << "";

    QTest::newRow("Path to measurements is relative")
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/pattern.val")
            << "m.vit"
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns/m.vit");

    QTest::newRow("Absolute pattern path.")
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/patterns")
            << "m.vit"
            << QApplication::applicationDirPath() + QStringLiteral("/home/user/m.vit");
    #else
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
    #endif
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
