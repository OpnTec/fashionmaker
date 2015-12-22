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
const QString tmpTestCollectionFolder = QStringLiteral("tst_valentina_collection_tmp");

TST_ValentinaCommandLine::TST_ValentinaCommandLine(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_ValentinaCommandLine::init()
{
    {// Test files
        QDir tmpDir(tmpTestFolder);
        if (not tmpDir.removeRecursively())
        {
            QFAIL("Fail to remove test temp directory.");
        }

        if (not CopyRecursively(QApplication::applicationDirPath() + QDir::separator() +
                                QLatin1Literal("tst_valentina"),
                                QApplication::applicationDirPath() + QDir::separator() + tmpTestFolder))
        {
            QFAIL("Fail to prepare test files for testing.");
        }
    }

    {// Collection
        QDir tmpDir(tmpTestCollectionFolder);
        if (not tmpDir.removeRecursively())
        {
            QFAIL("Fail to remove collection temp directory.");
        }

        if (not CopyRecursively(QApplication::applicationDirPath() + QDir::separator() +
                                QLatin1Literal("tst_valentina_collection"),
                                QApplication::applicationDirPath() + QDir::separator() + tmpTestCollectionFolder))
        {
            QFAIL("Fail to prepare collection files for testing.");
        }
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

    QTest::newRow("File with invalid object type")                                 << "wrong_obj_type.val"
                                                                                   << false
                                                                                   << V_EX_NOINPUT;

    QTest::newRow("Empty text VAL file")                                           << "txt.val"
                                                                                   << false
                                                                                   << V_EX_NOINPUT;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_ValentinaCommandLine::OpenPatterns()
{
    QFETCH(QString, file);
    QFETCH(bool, result);
    QFETCH(int, exitCode);

    int exit;
    const QString tmp = QApplication::applicationDirPath() + QDir::separator() + tmpTestFolder;
    const bool res = Run(result, exitCode, exit, ValentinaPath(), QStringList() << "--test"
                    << tmp + QDir::separator() + file);

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

    const QString tmp = QApplication::applicationDirPath() + QDir::separator() + tmpTestFolder;

    QTest::newRow("Issue #372")<< "issue_372.val"
                               << QString("-p;;0;;-d;;%1;;-b;;output").arg(tmp)
                               << true
                               << V_EX_OK;

    QTest::newRow("A file with limited gradation. Standard measurements. Wrong data.")
            << "glimited_vst.val"
            << QString("-p;;0;;-d;;%1;;--gsize;;46;;--gheight;;164;;-b;;output").arg(tmp)
            << false
            << V_EX_DATAERR;

    QTest::newRow("A file with limited gradation. Standard measurements. Correct data.")
            << "glimited_vst.val"
            << QString("-p;;0;;-d;;%1;;--gsize;;40;;--gheight;;134;;-b;;output").arg(tmp)
            << true
            << V_EX_OK;

    QTest::newRow("A file with limited gradation. Individual measurements.")
            << "glimited_vit.val"
            << QString("-p;;0;;-d;;%1;;--gsize;;40;;--gheight;;134;;-b;;output").arg(tmp)
            << false
            << V_EX_DATAERR;

    QTest::newRow("A file with limited gradation. No measurements.")
            << "glimited_no_m.val"
            << QString("-p;;0;;-d;;%1;;--gsize;;40;;--gheight;;134;;-b;;output").arg(tmp)
            << false
            << V_EX_DATAERR;
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
    const QString tmp = QApplication::applicationDirPath() + QDir::separator() + tmpTestFolder;
    const QStringList arg = QStringList() << tmp + QDir::separator() + file
                                          << arguments.split(";;");
    const bool res = Run(result, exitCode, exit, ValentinaPath(), arg);

    QCOMPARE(res, result);
    QCOMPARE(exit, exitCode);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ValentinaCommandLine::TestMode_data() const
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("arguments");
    QTest::addColumn<bool>("result");
    QTest::addColumn<int>("exitCode");

    const QString tmp = QApplication::applicationDirPath() + QDir::separator() + tmpTestFolder;

    QTest::newRow("Issue #256. Correct path.")<< "issue_256.val"
                               << QString("--test")
                               << true
                               << V_EX_OK;

    QTest::newRow("Issue #256. Wrong path.")<< "issue_256_wrong_path.vit"
                               << QString("--test")
                               << false
                               << V_EX_NOINPUT;

    QTest::newRow("Issue #256. Correct individual measurements.")<< "issue_256.val"
                               << QString("--test;;-m;;%1").arg(tmp + QDir::separator() +
                                                                QLatin1Literal("issue_256_correct.vit"))
                               << true
                               << V_EX_OK;

    QTest::newRow("Issue #256. Wrong individual measurements.")<< "issue_256.val"
                               << QString("--test;;-m;;%1").arg(tmp + QDir::separator() +
                                                                QLatin1Literal("issue_256_wrong.vit"))
                               << false
                               << V_EX_NOINPUT;

    QTest::newRow("Issue #256. Correct standard measurements.")<< "issue_256.val"
                               << QString("--test;;-m;;%1").arg(tmp + QDir::separator() +
                                                                QLatin1Literal("issue_256_correct.vst"))
                               << true
                               << V_EX_OK;

    QTest::newRow("Issue #256. Wrong standard measurements.")<< "issue_256.val"
                               << QString("--test;;-m;;%1").arg(tmp + QDir::separator() +
                                                                QLatin1Literal("issue_256_wrong.vst"))
                               << false
                               << V_EX_NOINPUT;

    QTest::newRow("Wrong formula.")<< "wrong_formula.val"
                               << QString("--test")
                               << false
                               << V_EX_DATAERR;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ValentinaCommandLine::TestMode()
{
    QFETCH(QString, file);
    QFETCH(QString, arguments);
    QFETCH(bool, result);
    QFETCH(int, exitCode);

    int exit;
    const QString tmp = QApplication::applicationDirPath() + QDir::separator() + tmpTestFolder;
    const QStringList arg = QStringList() << tmp + QDir::separator() + file
                                          << arguments.split(";;");
    const bool res = Run(result, exitCode, exit, ValentinaPath(), arg);

    QCOMPARE(res, result);
    QCOMPARE(exit, exitCode);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ValentinaCommandLine::TestOpenCollection_data() const
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("arguments");
    QTest::addColumn<bool>("result");
    QTest::addColumn<int>("exitCode");

    const QString tmp = QApplication::applicationDirPath() + QDir::separator() + tmpTestCollectionFolder;

    QTest::newRow("jacketМ6_30-110")<< "jacketМ6_30-110.val"
                               << QString("--test;;-m;;%1").arg(tmp + QDir::separator() +
                                                                QLatin1Literal("GOST_man_ru.vst"))
                               << true
                               << V_EX_OK;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ValentinaCommandLine::TestOpenCollection()
{
    QFETCH(QString, file);
    QFETCH(QString, arguments);
    QFETCH(bool, result);
    QFETCH(int, exitCode);

    int exit;
    const QString tmp = QApplication::applicationDirPath() + QDir::separator() + tmpTestCollectionFolder;
    const QStringList arg = QStringList() << tmp + QDir::separator() + file
                                          << arguments.split(";;");
    const bool res = Run(result, exitCode, exit, ValentinaPath(), arg);

    QCOMPARE(res, result);
    QCOMPARE(exit, exitCode);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_ValentinaCommandLine::cleanup()
{
    {
        QDir tmpDir(tmpTestFolder);
        if (not tmpDir.removeRecursively())
        {
            QWARN("Fail to remove test temp directory.");
        }
    }

    {
        QDir tmpDir(tmpTestCollectionFolder);
        if (not tmpDir.removeRecursively())
        {
            QWARN("Fail to remove collection temp directory.");
        }
    }
}
