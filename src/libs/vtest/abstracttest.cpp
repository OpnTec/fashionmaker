/************************************************************************
 **
 **  @file   abstracttest.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <qtestcase.h>
#include <QApplication>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFlags>
#include <QIODevice>
#include <QPointF>
#include <QProcess>
#include <QScopedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QStringList>
#include <QVector>
#include <QtGlobal>
#include <QLineF>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "vsysexits.h"
#include "../vgeometry/vgobject.h"
#include "../vlayout/vabstractpiece.h"

//---------------------------------------------------------------------------------------------------------------------
AbstractTest::AbstractTest(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::VectorFromJson(const QString &json, QVector<QPointF>& vector)
{
    QFile loadFile(json);
    if (not loadFile.open(QIODevice::ReadOnly))
    {
        const QString error = QStringLiteral("Couldn't open json file. %1").arg(loadFile.errorString());
        QFAIL(qUtf8Printable(error));
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    const QString vectorKey = QStringLiteral("vector");
    const QString typeKey = QStringLiteral("type");
    const QString xKey = QStringLiteral("x");
    const QString yKey = QStringLiteral("y");

    QJsonObject vectorObject = loadDoc.object();
    if (not vectorObject.contains(vectorKey))
    {
        const QString error = QStringLiteral("Invalid json file '%1'. File doesn't contain root object.").arg(json);
        QFAIL(qUtf8Printable(error));
    }

    QJsonArray vectorArray = loadDoc.object()[vectorKey].toArray();
    for (int i = 0; i < vectorArray.size(); ++i)
    {
        QJsonObject pointObject = vectorArray[i].toObject();

        if (not pointObject.contains(typeKey))
        {
            const QString error = QStringLiteral("Invalid json file '%1'. Json object doesn't provide class type.")
                    .arg(json);
            QFAIL(qUtf8Printable(error));
        }

        if (pointObject[typeKey].toString() != QLatin1String("QPointF"))
        {
            const QString error = QStringLiteral("Invalid json file '%1'. Unexpected class '%2'.")
                    .arg(json, pointObject[typeKey].toString());
            QFAIL(qUtf8Printable(error));
        }

        QPointF point;

        if (pointObject.contains(xKey))
        {
            QJsonValue xValue = pointObject[xKey];
            if (xValue.isDouble())
            {
                point.setX(xValue.toDouble());
            }
            else
            {
                const QString error = QStringLiteral("X coordinate is not double '%1'.").arg(xValue.toString());
                QFAIL(qUtf8Printable(error));
            }
        }
        else
        {
            const QString error = QStringLiteral("Json object does not contain X coordinate.");
            QFAIL(qUtf8Printable(error));
        }

        if (pointObject.contains(yKey))
        {
            QJsonValue yValue = pointObject[yKey];
            if (yValue.isDouble())
            {
                point.setY(yValue.toDouble());
            }
            else
            {
                const QString error = QStringLiteral("Y coordinate is not double '%1'.").arg(yValue.toString());
                QFAIL(qUtf8Printable(error));
            }
        }
        else
        {
            const QString error = QStringLiteral("Json object does not contain Y coordinate.");
            QFAIL(qUtf8Printable(error));
        }

        vector.append(point);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::VectorFromJson(const QString &json, QVector<VSAPoint> &vector)
{
    QFile loadFile(json);
    if (not loadFile.open(QIODevice::ReadOnly))
    {
        const QString error = QStringLiteral("Couldn't open json file. %1").arg(loadFile.errorString());
        QFAIL(qUtf8Printable(error));
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    const int defaultAngle = static_cast<int>(PieceNodeAngle::ByLength);

    const QString vectorKey = QStringLiteral("vector");
    const QString typeKey = QStringLiteral("type");
    const QString xKey = QStringLiteral("x");
    const QString yKey = QStringLiteral("y");
    const QString saBeforeKey = QStringLiteral("saBefore");
    const QString saAfterKey = QStringLiteral("saAfter");
    const QString angleKey = QStringLiteral("angle");

    QJsonObject vectorObject = loadDoc.object();
    if (not vectorObject.contains(vectorKey))
    {
        const QString error = QStringLiteral("Invalid json file '%1'. File doesn't contain root object.").arg(json);
        QFAIL(qUtf8Printable(error));
    }

    QJsonArray vectorArray = loadDoc.object()[vectorKey].toArray();
    for (int i = 0; i < vectorArray.size(); ++i)
    {
        QJsonObject pointObject = vectorArray[i].toObject();

        if (not pointObject.contains(typeKey))
        {
            const QString error = QStringLiteral("Invalid json file '%1'. Json object doesn't provide class type.")
                    .arg(json);
            QFAIL(qUtf8Printable(error));
        }

        if (pointObject[typeKey].toString() != QLatin1String("VSAPoint"))
        {
            const QString error = QStringLiteral("Invalid json file '%1'. Unexpected class '%2'.")
                    .arg(json, pointObject[typeKey].toString());
            QFAIL(qUtf8Printable(error));
        }

        VSAPoint point;

        if (pointObject.contains(xKey))
        {
            QJsonValue xValue = pointObject[xKey];
            if (xValue.isDouble())
            {
                point.setX(xValue.toDouble());
            }
            else
            {
                const QString error = QStringLiteral("X coordinate is not double '%1'.").arg(xValue.toString());
                QFAIL(qUtf8Printable(error));
            }
        }
        else
        {
            const QString error = QStringLiteral("Json object does not contain X coordinate.");
            QFAIL(qUtf8Printable(error));
        }

        if (pointObject.contains(yKey))
        {
            QJsonValue yValue = pointObject[yKey];
            if (yValue.isDouble())
            {
                point.setY(yValue.toDouble());
            }
            else
            {
                const QString error = QStringLiteral("Y coordinate is not double '%1'.").arg(yValue.toString());
                QFAIL(qUtf8Printable(error));
            }
        }
        else
        {
            const QString error = QStringLiteral("Json object does not contain Y coordinate.");
            QFAIL(qUtf8Printable(error));
        }

        if (pointObject.contains(saBeforeKey))
        {
            QJsonValue saBeforeValue = pointObject[saBeforeKey];
            if (saBeforeValue.isDouble())
            {
                point.SetSABefore(saBeforeValue.toDouble(-1));
            }
            else
            {
                const QString error = QStringLiteral("SABefore is not double '%1'.").arg(saBeforeValue.toString());
                QFAIL(qUtf8Printable(error));
            }
        }

        if (pointObject.contains(saAfterKey))
        {
            QJsonValue saAfterValue = pointObject[saAfterKey];
            if (saAfterValue.isDouble())
            {
                point.SetSABefore(saAfterValue.toDouble(-1));
            }
            else
            {
                const QString error = QStringLiteral("SAAfter is not double '%1'.").arg(saAfterValue.toString());
                QFAIL(qUtf8Printable(error));
            }
        }

        if (pointObject.contains(angleKey))
        {
            QJsonValue angleValue = pointObject[angleKey];
            if (angleValue.isDouble())
            {
                const int angle = static_cast<int>(angleValue.toDouble(defaultAngle));
                point.SetAngleType(static_cast<PieceNodeAngle>(angle));
            }
            else
            {
                const QString error = QStringLiteral("Angle type is not double '%1'.").arg(angleValue.toString());
                QFAIL(qUtf8Printable(error));
            }
        }

        vector.append(point);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::Comparison(const QVector<QPointF> &ekv, const QVector<QPointF> &ekvOrig) const
{
    // Begin comparison
    QCOMPARE(ekv.size(), ekvOrig.size());// First check if sizes equal

    for (int i=0; i < ekv.size(); i++)
    {
        const QPointF p1 = ekv.at(i);
        const QPointF p2 = ekvOrig.at(i);
        const QString msg = QString("Index: %1. Got '%2;%3', Expected '%4;%5'.")
                .arg(i).arg(p1.x()).arg(p1.y()).arg(p2.x()).arg(p2.y());
        // Check each point. Don't use comparison float values
        QVERIFY2(VFuzzyComparePoints(p1, p2), qUtf8Printable(msg));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::Comparison(const QPointF &result, const QPointF &expected) const
{
    const QString msg = QStringLiteral("Got '%2;%3', Expected '%4;%5'.")
            .arg(result.x()).arg(result.y()).arg(expected.x()).arg(expected.y());
    // Check each point. Don't use comparison float values
    QVERIFY2(VFuzzyComparePoints(result, expected), qUtf8Printable(msg));
}


//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::Comparison(const QVector<QLineF> &result, const QVector<QLineF> &expected) const
{
    // Begin comparison
    QCOMPARE(result.size(), expected.size());// First check if sizes equal

    for (int i=0; i < result.size(); i++)
    {
        const QLineF &line1 = result.at(i);
        const QLineF &line2 = expected.at(i);
        // Check each point. Don't use comparison float values
        QVERIFY2(VFuzzyComparePoints(line1.p1(), line2.p1()) && VFuzzyComparePoints(line1.p2(), line2.p2()),
                 qUtf8Printable(
                     QStringLiteral("Index: %1. Got line '(%2;%3):(%4;%5)', Expected line '(%6;%7):(%8;%9)'.")
                     .arg(i)
                     .arg(line1.p1().x())
                     .arg(line1.p1().y())
                     .arg(line1.p2().x())
                     .arg(line1.p2().y())
                     .arg(line2.p1().x())
                     .arg(line2.p1().y())
                     .arg(line2.p2().x())
                     .arg(line2.p2().y())
                     )
                 );
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString AbstractTest::ValentinaPath() const
{
    const QString path = QStringLiteral("/../../../app/valentina/bin/valentina");
#ifdef Q_OS_WIN
    return QCoreApplication::applicationDirPath() + path + QLatin1String(".exe");
#else
    return QCoreApplication::applicationDirPath() + path;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString AbstractTest::TapePath() const
{
    const QString path = QStringLiteral("/../../../app/tape/bin/tape");
#ifdef Q_OS_WIN
    return QCoreApplication::applicationDirPath() + path + QLatin1String(".exe");
#else
    return QCoreApplication::applicationDirPath() + path;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
QString AbstractTest::TranslationsPath() const
{
    return QCoreApplication::applicationDirPath() + QStringLiteral("/../../../app/valentina/bin/translations");
}

//---------------------------------------------------------------------------------------------------------------------
int AbstractTest::Run(int exit, const QString &program, const QStringList &arguments, QString &error, int msecs)
{
    const QString parameters = QString("Program: %1 \nArguments: %2.").arg(program, arguments.join(", "));

    QFileInfo info(program);
    if (not info.exists())
    {
        error = QString("Can't find binary.\n%1").arg(parameters);
        return TST_EX_BIN;
    }

    QScopedPointer<QProcess> process(new QProcess());
    process->setWorkingDirectory(info.absoluteDir().absolutePath());
    process->start(program, arguments);

    if (not process->waitForStarted(msecs))
    {
        error = QStringLiteral("The start operation timed out or an error occurred.\n%1\n%2")
                .arg(parameters, QString(process->readAllStandardError()));
        process->kill();
        return TST_EX_START_TIME_OUT;
    }

    if (not process->waitForFinished(msecs))
    {
        error = QStringLiteral("The finish operation timed out or an error occurred.\n%1\n%2")
                .arg(parameters, QString(process->readAllStandardError()));
        process->kill();
        return TST_EX_FINISH_TIME_OUT;
    }

    if (process->exitStatus() == QProcess::CrashExit)
    {
        error = QStringLiteral("Program crashed.\n%1\n%2").arg(parameters, QString(process->readAllStandardError()));
        return TST_EX_CRASH;
    }

    if (process->exitCode() != exit)
    {
        error = QStringLiteral("Unexpected finish. Exit code: %1\n%2").arg(process->exitCode())
                .arg(QString(process->readAllStandardError()));
        return process->exitCode();
    }

    return process->exitCode();
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
        const QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot |
                                                          QDir::Hidden | QDir::System);
        for (auto &fileName : fileNames)
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
        if (QFileInfo::exists(tgtFilePath))
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
            const QString msg = QString("Can't copy file '%1'. Error: %2").arg(srcFilePath, srcFile.errorString());
            QWARN(qUtf8Printable(msg));
            return false;
        }
        srcFile.close();

        if (not srcFile.copy(tgtFilePath))
        {
            const QString msg = QString("Can't copy file '%1' to '%2'. Error: %3")
                    .arg(srcFilePath, tgtFilePath, srcFile.errorString());
            QWARN(qUtf8Printable(msg));
            return false;
        }
    }
    return true;
}
