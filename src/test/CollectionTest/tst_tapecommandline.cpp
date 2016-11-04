/************************************************************************
 **
 **  @file   tst_tapecommandline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 9, 2015
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

#include "tst_tapecommandline.h"
#include "../vmisc/vsysexits.h"

#include <QtTest>

const QString tmpTestFolder = QStringLiteral("tst_tape_tmp");

//---------------------------------------------------------------------------------------------------------------------
TST_TapeCommandLine::TST_TapeCommandLine(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TapeCommandLine::initTestCase()
{
    QDir tmpDir(tmpTestFolder);
    if (not tmpDir.removeRecursively())
    {
        QFAIL("Fail to remove temp directory.");
    }

    if (not CopyRecursively(QApplication::applicationDirPath() + QDir::separator() + QStringLiteral("tst_tape"),
                            QApplication::applicationDirPath() + QDir::separator() + tmpTestFolder))
    {
        QFAIL("Fail to prepare files for testing.");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TapeCommandLine::OpenMeasurements_data() const
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<int>("exitCode");

    // The file doesn't exist!
    QTest::newRow("Send wrong path to a file")                                     << "wrongPath.vit"
                                                                                   << V_EX_NOINPUT;

    QTest::newRow("Old individual format to new version")                          << "keiko.vit"
                                                                                   << V_EX_OK;

    QTest::newRow("Open empty file")                                               << "empty.vit"
                                                                                   << V_EX_OK;

    QTest::newRow("Open the VIT file with all know measurements (v0.3.0)")         << "all_measurements_v0.3.0.vit"
                                                                                   << V_EX_OK;

    QTest::newRow("Open the VST file with all know measurements (v0.4.0)")         << "all_measurements_v0.4.0.vst"
                                                                                   << V_EX_OK;

    QTest::newRow("Open the VST file for man ru GOST (v0.3.0).")                   << "GOST_man_ru_v0.3.0.vst"
                                                                                   << V_EX_OK;

    QTest::newRow("Open the VIT file with all know measurements (v0.3.3)")         << "all_measurements_v0.3.3.vit"
                                                                                   << V_EX_OK;

    QTest::newRow("Open the VST file with all know measurements (v0.4.2)")         << "all_measurements_v0.4.2.vst"
                                                                                   << V_EX_OK;

    QTest::newRow("Open the VST file for man ru GOST (v0.4.2).")                   << "GOST_man_ru_v0.4.2.vst"
                                                                                   << V_EX_OK;

    QTest::newRow("Broken file. Not unique name.")                                 << "broken1.vit"
                                                                                   << V_EX_NOINPUT;

    QTest::newRow("Broken file. Measurement name can't be empty.")                 << "broken1.vit"
                                                                                   << V_EX_NOINPUT;

    QTest::newRow("Broken file. An empty value shouldn't break a file.")           << "broken3.vit"
                                                                                   << V_EX_OK;

    QTest::newRow("Broken file. Invalid measurement name.")                        << "broken4.vit"
                                                                                   << V_EX_NOINPUT;

    QTest::newRow("Empty text VIT file.")                                          << "text.vit"
                                                                                   << V_EX_NOINPUT;

    QTest::newRow("Empty text VST file.")                                          << "text.vst"
                                                                                   << V_EX_NOINPUT;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TapeCommandLine::OpenMeasurements()
{
    QFETCH(QString, file);
    QFETCH(int, exitCode);

    const int exit = Run(exitCode, TapePath(), QStringList() << "--test"
                         << QApplication::applicationDirPath() + QDir::separator() + tmpTestFolder + QDir::separator() +
                         file);

    QCOMPARE(exit, exitCode);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TapeCommandLine::cleanupTestCase()
{
    QDir tmpDir(tmpTestFolder);
    if (not tmpDir.removeRecursively())
    {
        QWARN("Fail to remove temp directory.");
    }
}
