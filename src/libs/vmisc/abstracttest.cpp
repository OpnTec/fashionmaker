/************************************************************************
 **
 **  @file   abstracttest.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2015
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

#include "abstracttest.h"
#include "logging.h"
#include "vsysexits.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
AbstractTest::AbstractTest(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::Comparison(const QVector<QPointF> &ekv, const QVector<QPointF> &ekvOrig) const
{
    // Begin comparison
    QCOMPARE(ekv.size(), ekvOrig.size());// First check if sizes equal
    for (int i=0; i < ekv.size(); i++)
    {
        // Check each point
        QCOMPARE(ekv.at(i).toPoint(), ekvOrig.at(i).toPoint()); // Don't use comparison float values
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString AbstractTest::ValentinaPath() const
{
    const QString path = QStringLiteral("/../../../app/valentina/bin/valentina");
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + path + QLatin1String(".exe");
#else
    return QApplication::applicationDirPath() + path;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString AbstractTest::TapePath() const
{
    const QString path = QStringLiteral("/../../../app/tape/bin/tape");
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + path + QLatin1String(".exe");
#else
    return QApplication::applicationDirPath() + path;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString AbstractTest::TranslationsPath() const
{
    return QApplication::applicationDirPath() + QStringLiteral("/../../../app/valentina/bin/translations");
}

//---------------------------------------------------------------------------------------------------------------------
bool AbstractTest::Run(bool showWarn, int exit, int &exitCode, const QString &program, const QStringList &arguments,
                       int msecs)
{
    const QString parameters = QString("Program: %1 \nArguments: %2.").arg(program).arg(arguments.join(", "));

    QFileInfo info(program);
    if (not info.exists())
    {
        const QString msg = QString("Can't find binary.\n%1").arg(parameters);
        QWARN(qUtf8Printable(msg));
        exitCode = TST_EX_BIN;
        return false;
    }

    QScopedPointer<QProcess> process(new QProcess());
    process->setWorkingDirectory(info.absoluteDir().absolutePath());
    process->start(program, arguments);

    if (not process->waitForFinished(msecs) && process->state() != QProcess::NotRunning)
    {
        const QString msg = QString("The operation timed out or an error occurred.\n%1").arg(parameters);
        QWARN(qUtf8Printable(msg));
        exitCode = TST_EX_TIME_OUT;
        process->kill();
        return false;
    }

    if (process->exitStatus() == QProcess::CrashExit)
    {
        const QString msg = QString("Program crashed.\n%1\n%2").arg(parameters)
                                                               .arg(QString(process->readAllStandardError()));
        QWARN(qUtf8Printable(msg));
        exitCode = TST_EX_CRASH;
        return false;
    }

    if (process->exitCode() != V_EX_OK)
    {
        if (showWarn || process->exitCode() != exit)
        {
            const QString msg = QString("\n%1").arg(QString(process->readAllStandardError()));
            QWARN(qUtf8Printable(msg));
        }
        exitCode = process->exitCode();
        return false;
    }

    exitCode = process->exitCode();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool AbstractTest::CopyRecursively(const QString &srcFilePath, const QString &tgtFilePath) const
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir())
    {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        const QString dirName = QFileInfo(tgtFilePath).fileName();
        if (not targetDir.mkdir(dirName))
        {
            const QString msg = QString("Can't create dir '%1'.").arg(dirName);
            QWARN(qUtf8Printable(msg));
            return false;
        }
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden |
                                                    QDir::System);
        foreach (const QString &fileName, fileNames)
        {
            const QString newSrcFilePath = srcFilePath + QDir::separator() + fileName;
            const QString newTgtFilePath = tgtFilePath + QDir::separator() + fileName;
            if (not CopyRecursively(newSrcFilePath, newTgtFilePath))
            {
                return false;
            }
        }
    }
    else
    {
        if (QFileInfo(tgtFilePath).exists())
        {
            const QString msg = QString("File '%1' exists.").arg(srcFilePath);
            QWARN(qUtf8Printable(msg));

            if (QFile::remove(tgtFilePath))
            {
                QWARN("File successfully removed.");
            }
            else
            {
                QWARN("Can't remove file.");
                return false;
            }
        }

        // Check error: Cannot open %file for input
        QFile srcFile(srcFilePath);
        if (not srcFile.open(QFile::ReadOnly))
        {
            const QString msg = QString("Can't copy file '%1'. Error: %2").arg(srcFilePath).arg(srcFile.errorString());
            QWARN(qUtf8Printable(msg));
            return false;
        }
        srcFile.close();

        if (not srcFile.copy(tgtFilePath))
        {
            const QString msg = QString("Can't copy file '%1' to '%2'. Error: %3").arg(srcFilePath).arg(tgtFilePath)
                    .arg(srcFile.errorString());
            QWARN(qUtf8Printable(msg));
            return false;
        }
    }
    return true;
}
