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
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../vlayout/vabstractpiece.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiece.h"
#include "../vpatterndb/vpiecenode.h"

//---------------------------------------------------------------------------------------------------------------------
AbstractTest::AbstractTest(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::VectorFromJson(const QString &json, QVector<QPointF>& vector)
{
    QByteArray saveData;
    PrepareDocument(json, saveData);
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    const QString vectorKey = QStringLiteral("vector");
    const QString typeKey = QStringLiteral("type");

    QJsonObject vectorObject = loadDoc.object();
    TestRoot(vectorObject, vectorKey, json);

    QJsonArray vectorArray = vectorObject[vectorKey].toArray();
    for (int i = 0; i < vectorArray.size(); ++i)
    {
        QJsonObject pointObject = vectorArray[i].toObject();

        QString type;
        AbstractTest::ReadStringValue(pointObject, typeKey, type);

        if (type != QLatin1String("QPointF"))
        {
            const QString error = QStringLiteral("Invalid json file '%1'. Unexpected class '%2'.")
                    .arg(json, pointObject[typeKey].toString());
            QFAIL(qUtf8Printable(error));
        }

        QPointF point;

        qreal x = 0;
        AbstractTest::ReadDoubleValue(pointObject, QStringLiteral("x"), x);
        point.setX(x);

        qreal y = 0;
        AbstractTest::ReadDoubleValue(pointObject, QStringLiteral("y"), y);
        point.setY(y);

        vector.append(point);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::VectorFromJson(const QString &json, QVector<VSAPoint> &vector)
{
    QByteArray saveData;
    PrepareDocument(json, saveData);
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    const QString vectorKey = QStringLiteral("vector");

    QJsonObject vectorObject = loadDoc.object();
    TestRoot(vectorObject, vectorKey, json);

    QJsonArray vectorArray = vectorObject[vectorKey].toArray();
    for (int i = 0; i < vectorArray.size(); ++i)
    {
        QJsonObject pointObject = vectorArray[i].toObject();

        QString type;
        AbstractTest::ReadStringValue(pointObject, QStringLiteral("type"), type);

        if (type != QLatin1String("VSAPoint"))
        {
            const QString error = QStringLiteral("Invalid json file '%1'. Unexpected class '%2'.").arg(json, type);
            QFAIL(qUtf8Printable(error));
        }

        VSAPoint point;

        qreal x = 0;
        AbstractTest::ReadDoubleValue(pointObject, QStringLiteral("x"), x);
        point.setX(x);

        qreal y = 0;
        AbstractTest::ReadDoubleValue(pointObject, QStringLiteral("y"), y);
        point.setY(y);

        qreal saBefore;
        AbstractTest::ReadDoubleValue(pointObject, QStringLiteral("saBefore"), saBefore, QStringLiteral("-1"));
        point.SetSABefore(saBefore);

        qreal saAfter;
        AbstractTest::ReadDoubleValue(pointObject, QStringLiteral("saAfter"), saAfter, QStringLiteral("-1"));
        point.SetSAAfter(saAfter);

        PieceNodeAngle angleType;
        AbstractTest::ReadDoubleValue(pointObject, QStringLiteral("angle"), angleType,
                                      QString::number(static_cast<int>(PieceNodeAngle::ByLength)));
        point.SetAngleType(angleType);

        vector.append(point);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::PieceFromJson(const QString &json, VPiece &piece, QSharedPointer<VContainer> &data)
{
    QByteArray saveData;
    PrepareDocument(json, saveData);
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    const QString testCaseKey = QStringLiteral("testCase");
    const QString bdKey = QStringLiteral("bd");
    const QString pieceKey = QStringLiteral("piece");

    QJsonObject testCaseObject = loadDoc.object();
    TestRoot(testCaseObject, testCaseKey, json);

    QJsonObject testCase = testCaseObject[testCaseKey].toObject();

    if (testCase.contains(bdKey))
    {
        DBFromJson(testCase[bdKey].toObject(), data);
    }
    else
    {
        const QString error = QStringLiteral("Test case json object does not contain db data.");
        QFAIL(qUtf8Printable(error));
    }

    if (testCase.contains(pieceKey))
    {
        MainPathFromJson(testCase[pieceKey].toObject(), piece);
    }
    else
    {
        const QString error = QStringLiteral("Test case json object does not contain piece data.");
        QFAIL(qUtf8Printable(error));
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
        const QString msg = QStringLiteral("Index: %1. Got '%2;%3', Expected '%4;%5'.")
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
    const QString parameters = QStringLiteral("Program: %1 \nArguments: %2.")
            .arg(program, arguments.join(QStringLiteral(", ")));

    QFileInfo info(program);
    if (not info.exists())
    {
        error = QStringLiteral("Can't find binary.\n%1").arg(parameters);
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
            const QString msg = QStringLiteral("Can't create dir '%1'.").arg(dirName);
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
            const QString msg = QStringLiteral("File '%1' exists.").arg(srcFilePath);
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
            const QString msg = QStringLiteral("Can't copy file '%1'. Error: %2")
                    .arg(srcFilePath, srcFile.errorString());
            QWARN(qUtf8Printable(msg));
            return false;
        }
        srcFile.close();

        if (not srcFile.copy(tgtFilePath))
        {
            const QString msg = QStringLiteral("Can't copy file '%1' to '%2'. Error: %3")
                    .arg(srcFilePath, tgtFilePath, srcFile.errorString());
            QWARN(qUtf8Printable(msg));
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::PrepareDocument(const QString &json, QByteArray &data) const
{
    QFile loadFile(json);
    if (not loadFile.open(QIODevice::ReadOnly))
    {
        const QString error = QStringLiteral("Couldn't open json file. %1").arg(loadFile.errorString());
        QFAIL(qUtf8Printable(error));
    }

    data = loadFile.readAll();
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::TestRoot(const QJsonObject &root, const QString &attribute, const QString &file)
{
    if (not root.contains(attribute))
    {
        const QString error = QStringLiteral("Invalid json file '%1'. File doesn't contain root object.").arg(file);
        QFAIL(qUtf8Printable(error));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::ReadStringValue(const QJsonObject &itemObject, const QString &attribute, QString &value,
                                   const QString &defaultValue)
{
    if (itemObject.contains(attribute))
    {
        QJsonValue attributeValue = itemObject[attribute];
        if (attributeValue.isString())
        {
            value = attributeValue.toString();
        }
        else
        {
            const QString error = QStringLiteral("%1 is not string '%2'.").arg(attribute, attributeValue.toString());
            QFAIL(qUtf8Printable(error));
        }
    }
    else
    {
        if (not defaultValue.isEmpty())
        {
            value = defaultValue;
        }
        else
        {
            const QString error = QStringLiteral("Json object does not contain attribute '%1'.").arg(attribute);
            QFAIL(qUtf8Printable(error));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::ReadBooleanValue(const QJsonObject &itemObject, const QString &attribute, bool &value,
                                    const QString &defaultValue)
{
    if (itemObject.contains(attribute))
    {
        QJsonValue attributeValue = itemObject[attribute];
        if (attributeValue.isBool())
        {
            value = attributeValue.toBool();
        }
        else
        {
            const QString error = QStringLiteral("%1 is not boolean value '%2'.").arg(attribute,
                                                                                      attributeValue.toString());
            QFAIL(qUtf8Printable(error));
        }
    }
    else
    {
        if (not defaultValue.isEmpty())
        {
            bool ok = false;
            int defVal = defaultValue.toInt(&ok);

            if (not ok)
            {
                const QString error = QStringLiteral("Cannot convert default value '%1' to int.").arg(defaultValue);
                QFAIL(qUtf8Printable(error));
            }

            value = static_cast<bool>(defVal);
        }
        else
        {
            const QString error = QStringLiteral("Json object does not contain attribute '%1'.").arg(attribute);
            QFAIL(qUtf8Printable(error));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::ReadPointValue(const QJsonObject &itemObject, const QString &attribute, VPointF &value)
{
    if (itemObject.contains(attribute))
    {
        QJsonObject p1Object = itemObject[attribute].toObject();
        PointFromJson(p1Object, value);
    }
    else
    {
        const QString error = QStringLiteral("Json object does not contain attribute '%1'.").arg(attribute);
        QFAIL(qUtf8Printable(error));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::ReadSplinePointValues(const QJsonObject &itemObject, const QString &attribute,
                                         QVector<VSplinePoint> &points)
{
    points.clear();
    if (itemObject.contains(attribute))
    {
        QJsonArray nodes = itemObject[attribute].toArray();
        for (int i = 0; i < nodes.size(); ++i)
        {
            QJsonObject item = nodes[i].toObject();
            VSplinePoint point;
            ReadSplinePointValue(item, point);
            points.append(point);
        }
    }
    else
    {
        const QString error = QStringLiteral("Json object does not contain attribute '%1'.").arg(attribute);
        QFAIL(qUtf8Printable(error));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::ReadSplinePointValue(const QJsonObject &itemObject, VSplinePoint &point)
{
    qreal angle1 = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("angle1"), angle1);

    QString angle1Formula;
    AbstractTest::ReadStringValue(itemObject, QStringLiteral("angle1Formula"), angle1Formula);

    qreal angle2 = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("angle2"), angle2);

    QString angle2Formula;
    AbstractTest::ReadStringValue(itemObject, QStringLiteral("angle2Formula"), angle2Formula);

    qreal length1 = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("length1"), length1);

    QString length1Formula;
    AbstractTest::ReadStringValue(itemObject, QStringLiteral("length1Formula"), length1Formula);

    qreal length2 = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("length2"), length2);

    QString length2Formula;
    AbstractTest::ReadStringValue(itemObject, QStringLiteral("length2Formula"), length2Formula);

    VPointF pSpline;
    ReadPointValue(itemObject, QStringLiteral("point"), pSpline);

    point = VSplinePoint(pSpline, angle1, angle1Formula, angle2, angle2Formula, length1, length1Formula, length2,
                         length2Formula);
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::ReadPieceNodeValue(const QJsonObject &itemObject, VPieceNode &node)
{
    vidtype id = NULL_ID;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("id"), id);

    Tool typeTool = Tool::LAST_ONE_DO_NOT_USE;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("type"), typeTool);

    bool reverse = false;
    AbstractTest::ReadBooleanValue(itemObject, QStringLiteral("reverse"), reverse, QChar('0'));

    node = VPieceNode(id, typeTool, reverse);
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
void AbstractTest::ReadDoubleValue(const QJsonObject &itemObject, const QString &attribute, T &value,
                                   const QString &defaultValue)
{
    if (itemObject.contains(attribute))
    {
        QJsonValue attributeValue = itemObject[attribute];
        if (attributeValue.isDouble())
        {
            value = static_cast<T>(attributeValue.toDouble());
        }
        else
        {
            const QString error = QStringLiteral("%1 is not double '%2'.").arg(attribute, attributeValue.toString());
            QFAIL(qUtf8Printable(error));
        }
    }
    else
    {
        if (not defaultValue.isEmpty())
        {
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wunused-but-set-variable")
            bool ok = false;
            qreal defVal = defaultValue.toDouble(&ok);

            if (not ok)
            {
                const QString error = QStringLiteral("Cannot convert default value '%1' to double.").arg(defaultValue);
                QFAIL(qUtf8Printable(error));
            }

            value = static_cast<T>(defVal);
QT_WARNING_POP
        }
        else
        {
            const QString error = QStringLiteral("Json object does not contain attribute '%1'.").arg(attribute);
            QFAIL(qUtf8Printable(error));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::PointFromJson(const QJsonObject &itemObject, VPointF &value)
{
    vidtype id = NULL_ID;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("id"), id);

    qreal mx = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("mx"), mx);

    qreal my = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("my"), my);

    QString name;
    AbstractTest::ReadStringValue(itemObject, QStringLiteral("name"), name);

    qreal x = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("x"), x);

    qreal y = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("y"), y);

    value = VPointF(x, y, name, mx, my);
    value.setId(id);
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::SplineFromJson(const QJsonObject &itemObject, QSharedPointer<VContainer> &data)
{
    vidtype id = NULL_ID;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("id"), id);

    qreal aScale = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("aScale"), aScale);

    qreal angle1 = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("angle1"), angle1);

    QString angle1Formula;
    AbstractTest::ReadStringValue(itemObject, QStringLiteral("angle1Formula"), angle1Formula);

    qreal angle2 = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("angle2"), angle2);

    QString angle2Formula;
    AbstractTest::ReadStringValue(itemObject, QStringLiteral("angle2Formula"), angle2Formula);

    qreal c1Length = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("c1Length"), c1Length);

    QString c1LengthFormula;
    AbstractTest::ReadStringValue(itemObject, QStringLiteral("c1LengthFormula"), c1LengthFormula);

    qreal c2Length = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("c2Length"), c2Length);

    QString c2LengthFormula;
    AbstractTest::ReadStringValue(itemObject, QStringLiteral("c2LengthFormula"), c2LengthFormula);

    VPointF p1;
    ReadPointValue(itemObject, QStringLiteral("p1"), p1);
    data->UpdateGObject(p1.id(), new VPointF(p1));

    VPointF p4;
    ReadPointValue(itemObject, QStringLiteral("p4"), p4);
    data->UpdateGObject(p4.id(), new VPointF(p4));

    VSpline *spl = new VSpline(p1, p4, angle1, angle1Formula, angle2, angle2Formula, c1Length, c1LengthFormula,
                               c2Length, c2LengthFormula);
    spl->SetApproximationScale(aScale);
    data->UpdateGObject(id, spl);
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::SplinePathFromJson(const QJsonObject &itemObject, QSharedPointer<VContainer> &data)
{
    vidtype id = NULL_ID;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("id"), id);

    qreal aScale = 0;
    AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("aScale"), aScale);

    QVector<VSplinePoint> points;
    AbstractTest::ReadSplinePointValues(itemObject, QStringLiteral("nodes"), points);
    for (auto &point : points)
    {
        data->UpdateGObject(point.P().id(), new VPointF(point.P()));
    }

    VSplinePath *path = new VSplinePath(points);
    path->SetApproximationScale(aScale);
    data->UpdateGObject(id, path);
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::DBFromJson(const QJsonObject &dbObject, QSharedPointer<VContainer> &data)
{
    const QString itemsKey = QStringLiteral("items");

    if (dbObject.contains(itemsKey))
    {
        QJsonArray items = dbObject[itemsKey].toArray();
        for (int i = 0; i < items.size(); ++i)
        {
            QJsonObject itemObject = items[i].toObject();
            GOType objectType;
            AbstractTest::ReadDoubleValue(itemObject, QStringLiteral("type"), objectType);

            switch(objectType)
            {
                case GOType::Point:
                {
                    VPointF point;
                    PointFromJson(itemObject, point);
                    data->UpdateGObject(point.id(), new VPointF(point));
                    break;
                }
                case GOType::Spline:
                    SplineFromJson(itemObject, data);
                    break;
                case GOType::SplinePath:
                    SplinePathFromJson(itemObject, data);
                    break;
                default:
                {
                    const QString error = QStringLiteral("Not supported item type '%1'.")
                            .arg(static_cast<int>(objectType));
                    QFAIL(qUtf8Printable(error));
                }
            }
        }
    }
    else
    {
        const QString error = QStringLiteral("DB json object does not contain items.");
        QFAIL(qUtf8Printable(error));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::MainPathFromJson(const QJsonObject &pieceObject, VPiece &piece)
{
    qreal saWidth = 0;
    AbstractTest::ReadDoubleValue(pieceObject, QStringLiteral("saWidth"), saWidth);

    bool seamAllowance = false;
    AbstractTest::ReadBooleanValue(pieceObject, QStringLiteral("seamAllowance"), seamAllowance);

    piece.SetSeamAllowance(seamAllowance);
    piece.SetSAWidth(saWidth);

    piece.GetPath().Clear();

    const QString nodesKey = QStringLiteral("nodes");

    if (pieceObject.contains(nodesKey))
    {
        QJsonArray nodes = pieceObject[nodesKey].toArray();
        for (int i = 0; i < nodes.size(); ++i)
        {
            QJsonObject itemObject = nodes[i].toObject();

            VPieceNode node;
            ReadPieceNodeValue(itemObject, node);
            piece.GetPath().Append(node);
        }
    }
    else
    {
        const QString error = QStringLiteral("Piece json object does not contain nodes.");
        QFAIL(qUtf8Printable(error));
    }
}
