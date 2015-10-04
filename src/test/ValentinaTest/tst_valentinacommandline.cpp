/************************************************************************
 **
 **  @file   tst_valentinacommandline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 10, 2015
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

#include "tst_valentinacommandline.h"
#include "../vmisc/vsysexits.h"

#include <QtTest>

const QString tmpTestFolder = QStringLiteral("tst_valentina_tmp");

TST_ValentinaCommandLine::TST_ValentinaCommandLine(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_ValentinaCommandLine::init()
{
    QDir tmpDir(tmpTestFolder);
    if (not tmpDir.removeRecursively())
    {
        QFAIL("Fail to remove temp directory.");
    }

    if (not CopyRecursively(QApplication::applicationDirPath() + QLatin1Char('/') + QLatin1Literal("tst_valentina"),
                            QApplication::applicationDirPath() + QLatin1Char('/') + tmpTestFolder))
    {
        QFAIL("Fail to prepare files for testing.");
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_ValentinaCommandLine::OpenPatterns_data() const
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<bool>("result");
    QTest::addColumn<int>("exitCode");

    // The file doesn't exist!
    QTest::newRow("Send wrong path to a file")                                     << "wrongPath.val"
                                                                                   << false
                                                                                   << V_EX_NOINPUT;

    QTest::newRow("Measurement independent empty file")                            << "empty.val"
                                                                                   << true
                                                                                   << V_EX_OK;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_ValentinaCommandLine::OpenPatterns()
{
    QFETCH(QString, file);
    QFETCH(bool, result);
    QFETCH(int, exitCode);

    int exit;
    const bool res = Run(result, exit, ValentinaPath(), QStringList() << "--test"
                    << QApplication::applicationDirPath() + QLatin1Char('/') + tmpTestFolder + QLatin1Char('/') + file);

    QCOMPARE(res, result);
    QCOMPARE(exit, exitCode);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_ValentinaCommandLine::ExportMode_data() const
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("arguments");
    QTest::addColumn<bool>("result");
    QTest::addColumn<int>("exitCode");

    QTest::newRow("Issue #372")<< "issue_372.val"
                               << "-p;;0;;-o;;output"
                               << true
                               << V_EX_OK;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_ValentinaCommandLine::ExportMode()
{
    QFETCH(QString, file);
    QFETCH(QString, arguments);
    QFETCH(bool, result);
    QFETCH(int, exitCode);

    int exit;
    const QStringList arg = QStringList() << QApplication::applicationDirPath() + QLatin1Char('/') + tmpTestFolder +
                                             QLatin1Char('/') + file
                                          << arguments.split(";;");
    const bool res = Run(result, exit, ValentinaPath(), arg);

    QCOMPARE(res, result);
    QCOMPARE(exit, exitCode);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_ValentinaCommandLine::cleanup()
{
    QDir tmpDir(tmpTestFolder);
    if (not tmpDir.removeRecursively())
    {
        QWARN("Fail to remove temp directory.");
    }
}
