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

#include <QtTest>

const QString tmpTestFolder = QStringLiteral("tst_tape_tmp");

//---------------------------------------------------------------------------------------------------------------------
TST_TapeCommandLine::TST_TapeCommandLine(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TapeCommandLine::init()
{
    QDir tmpDir(tmpTestFolder);
    if (not tmpDir.removeRecursively())
    {
        QFAIL("Fail to remove temp directory.");
    }

    if (not CopyRecursively(QApplication::applicationDirPath() + QLatin1Char('/') + QStringLiteral("tst_tape"),
                            QApplication::applicationDirPath() + QLatin1Char('/') + tmpTestFolder))
    {
        QFAIL("Fail to prepare files for testing.");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TapeCommandLine::OpenMeasurements_data() const
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<bool>("result");

    QTest::newRow("Send wrong path to file") << "wrongPath.vit" << false; // The file doesn't exist!
    QTest::newRow("Old individual format to new version") << "keiko.vit" << true;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TapeCommandLine::OpenMeasurements()
{
    QFETCH(QString, file);
    QFETCH(bool, result);

    const bool res = Run(result, TapePath(), QStringList() << "--test"
                    << QApplication::applicationDirPath() + QLatin1Char('/') + tmpTestFolder + QLatin1Char('/') + file);

    QCOMPARE(res, result);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TapeCommandLine::cleanup()
{
    QDir tmpDir(tmpTestFolder);
    if (not tmpDir.removeRecursively())
    {
        QWARN("Fail to remove temp directory.");
    }
}
