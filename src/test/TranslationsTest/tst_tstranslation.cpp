/************************************************************************
 **
 **  @file   tst_tstranslation.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 12, 2015
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

#include "tst_tstranslation.h"
#include "../vmisc/logging.h"
#include "../vmisc/def.h"

#include <QDomDocument>
#include <QtTest>

Q_DECLARE_METATYPE(QDomElement) // Need for testing

const QString TST_TSTranslation::TagName           = QStringLiteral("name");
const QString TST_TSTranslation::TagMessage        = QStringLiteral("message");
const QString TST_TSTranslation::TagSource         = QStringLiteral("source");
const QString TST_TSTranslation::TagTranslation    = QStringLiteral("translation");

const QString TST_TSTranslation::AttrType          = QStringLiteral("type");
const QString TST_TSTranslation::AttrValVanished   = QStringLiteral("vanished");
const QString TST_TSTranslation::AttrValUnfinished = QStringLiteral("unfinished");
const QString TST_TSTranslation::AttrValObsolete   = QStringLiteral("obsolete");

//---------------------------------------------------------------------------------------------------------------------
TST_TSTranslation::TST_TSTranslation(QObject *parent)
    : QObject(parent),
      tsFile(),
      tsXML()
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::CheckEnglishLocalization_data()
{
    QTest::addColumn<QString>("source");
    QTest::addColumn<QString>("translation");

    const QString fileName = QStringLiteral("valentina_en_US.ts");
    const QDomNodeList messages = LoadTSFile(fileName);
    if (messages.isEmpty())
    {
        QFAIL("Can't begin test.");
    }

    for (qint32 i = 0, num = messages.size(); i < num; ++i)
    {
        const QDomElement message = messages.at(i).toElement();
        if (message.isNull() == false)
        {
            const QString source = message.firstChildElement(TagSource).text();
            if (source.isEmpty())
            {
                continue;
            }

            const QDomElement translationTag = message.firstChildElement(TagTranslation);
            if (translationTag.hasAttribute(AttrType))
            {
                const QString attrVal = translationTag.attribute(AttrType);
                if (attrVal == AttrValVanished || attrVal == AttrValUnfinished || attrVal == AttrValObsolete)
                {
                    continue;
                }
            }
            const QString translation = translationTag.text();
            if (translation.isEmpty())
            {
                continue;
            }

            const QString message = QString("File '%1'. Check modification source message '%2'.").arg(fileName)
                    .arg(source);
            QTest::newRow(qUtf8Printable(message)) << source << translation;
        }
        else
        {
            const QString message = QString("Message %1 is null.").arg(i);
            QFAIL(qUtf8Printable(message));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::CheckEnglishLocalization()
{
    QFETCH(QString, source);
    QFETCH(QString, translation);

    QCOMPARE(source, translation);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::CheckEmptyToolButton_data()
{
    QTest::addColumn<QString>("source");
    QTest::addColumn<QDomElement>("message");

    const QString fileName = QStringLiteral("valentina.ts");
    const QDomNodeList messages = LoadTSFile(fileName);
    if (messages.isEmpty())
    {
        QFAIL("Can't begin test.");
    }

    for (qint32 i = 0, num = messages.size(); i < num; ++i)
    {
        const QDomElement message = messages.at(i).toElement();
        if (message.isNull() == false)
        {
            const QString source = message.firstChildElement(TagSource).text();
            if (source.isEmpty())
            {
                continue;
            }

            const QString tag = QString("File '%1'. Check modification source message '%2'.").arg(fileName)
                    .arg(source);
            QTest::newRow(qUtf8Printable(tag)) << source << message;
        }
        else
        {
            const QString message = QString("Message %1 is null.").arg(i);
            QFAIL(qUtf8Printable(message));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::CheckEmptyToolButton()
{
    QFETCH(QString, source);
    QFETCH(QDomElement, message);

    if (source == QLatin1String("..."))
    {
        const QDomElement translationTag = message.firstChildElement(TagTranslation);
        if (translationTag.hasAttribute(AttrType))
        {
            const QString attrVal = translationTag.attribute(AttrType);
            if (attrVal == AttrValVanished || attrVal == AttrValObsolete)
            {
                return;
            }
        }

        const QDomNode context = message.parentNode();
        if (context.isNull())
        {
            QFAIL("Can't get context.");
        }

        const QString contextName = context.firstChildElement(TagName).text();
        const QString error = QString("Found '...' in context '%1'").arg(contextName);
        QFAIL(qUtf8Printable(error));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::CheckPlaceMarkerExist_data()
{
    const QStringList locales = SupportedLocales();

    {
        QDir dir(TS_DIR);
        const QStringList fileNames = dir.entryList(QStringList("valentina*.ts"));
        QVERIFY2(locales.size() == fileNames.size()-1, "Unexpected count of files.");
    }

    QTest::addColumn<QString>("source");
    QTest::addColumn<QString>("translation");

    for(int j = 0; j < locales.size(); ++j)
    {
        const QString filename = QString("valentina_%1.ts").arg(locales.at(j));

        const QDomNodeList messages = LoadTSFile(filename);
        if (messages.isEmpty())
        {
            QFAIL("Can't begin test.");
        }

        for (qint32 i = 0, num = messages.size(); i < num; ++i)
        {
            const QDomElement message = messages.at(i).toElement();
            if (message.isNull() == false)
            {
                const QString source = message.firstChildElement(TagSource).text();
                if (source.isEmpty())
                {
                    continue;
                }

                const QDomElement translationTag = message.firstChildElement(TagTranslation);
                if (translationTag.hasAttribute(AttrType))
                {
                    const QString attrVal = translationTag.attribute(AttrType);
                    if (attrVal == AttrValVanished || attrVal == AttrValUnfinished || attrVal == AttrValObsolete)
                    {
                        continue;
                    }
                }
                const QString translation = translationTag.text();
                if (translation.isEmpty())
                {
                    continue;
                }

                const QString message = QString("File '%1'. Check place holder source message '%2'").arg(filename)
                        .arg(source);
                QTest::newRow(qUtf8Printable(message)) << source << translation;
            }
            else
            {
                const QString message = QString("File '%2'. Message %1 is null.").arg(i).arg(filename);
                QFAIL(qUtf8Printable(message));
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::CheckPlaceMarkerExist()
{
    QFETCH(QString, source);
    QFETCH(QString, translation);

    int sourceMarkCount = 0;
    int translationMarkCount = 0;

    for (int i = 1; i <= 99; ++i)
    {
        const QString marker = QLatin1String("%") + QString().setNum(i);
        const bool sourceMark = source.indexOf(marker) != -1;
        if (sourceMark)
        {
            ++sourceMarkCount;
            if (sourceMarkCount != i)
            {
                const QString message = QString("In source string '%1' was missed place marker ")
                        .arg(source) + QLatin1String("'%") + QString().setNum(sourceMarkCount) +
                        QLatin1String("'.");
                QFAIL(qUtf8Printable(message));
            }
        }

        const bool translationMark = translation.indexOf(marker) != -1;
        if (translationMark)
        {
            ++translationMarkCount;
            if (translationMarkCount != i)
            {
                const QString message = QString("In translation string '%1' was missed place marker ")
                        .arg(translation) + QLatin1String("'%") + QString().setNum(translationMarkCount) +
                        QLatin1String("'.");
                QFAIL(qUtf8Printable(message));
            }
        }

        if (sourceMark != translationMark)
        {
            const QString message =
                QString("Compare to source string in the translation string '%1' was missed place marker ")
                    .arg(translation) + QLatin1String("'%") + QString().setNum(sourceMarkCount) +
                    QLatin1String("'.");
            QFAIL(qUtf8Printable(message));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::TestPunctuation_data()
{
    const QStringList locales = SupportedLocales();

    {
        QDir dir(TS_DIR);
        const QStringList fileNames = dir.entryList(QStringList("valentina*.ts"));
        QVERIFY2(locales.size() == fileNames.size()-1, "Unexpected count of files.");
    }

    QTest::addColumn<QString>("source");
    QTest::addColumn<QString>("translation");

    for(int j = 0; j < locales.size(); ++j)
    {
        const QString filename = QString("valentina_%1.ts").arg(locales.at(j));

        const QDomNodeList messages = LoadTSFile(filename);
        if (messages.isEmpty())
        {
            QFAIL("Can't begin test.");
        }

        for (qint32 i = 0, num = messages.size(); i < num; ++i)
        {
            const QDomElement message = messages.at(i).toElement();
            if (message.isNull() == false)
            {
                const QString source = message.firstChildElement(TagSource).text();
                if (source.isEmpty())
                {
                    continue;
                }

                const QDomElement translationTag = message.firstChildElement(TagTranslation);
                if (translationTag.hasAttribute(AttrType))
                {
                    const QString attrVal = translationTag.attribute(AttrType);
                    if (attrVal == AttrValVanished || attrVal == AttrValUnfinished || attrVal == AttrValObsolete)
                    {
                        continue;
                    }
                }
                const QString translation = translationTag.text();
                if (translation.isEmpty())
                {
                    continue;
                }

                const QString message = QString("File '%1'.").arg(filename);
                QTest::newRow(qUtf8Printable(message)) << source << translation;
            }
            else
            {
                const QString message = QString("File '%2'. Message %1 is null.").arg(i).arg(filename);
                QFAIL(qUtf8Printable(message));
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::TestPunctuation()
{
    QFETCH(QString, source);
    QFETCH(QString, translation);

    static const QStringList punctuation = QStringList() << QLatin1String(".")
                                                         << QLatin1String(":")
                                                         << QLatin1String(" ")
                                                         << QLatin1String("\n")
                                                         << QLatin1String("!")
                                                         << QLatin1String("?");
    bool testFail = false;
    QChar c = source.at(source.length()-1);
    if (punctuation.contains(c))
    {
        if (not (source.endsWith(c) && translation.endsWith(c)))
        {
            testFail = true;
        }
    }
    else
    {
        c = translation.at(translation.length()-1);
        if (punctuation.contains(c))
        {
            testFail = true;
        }
    }

    if (testFail)
    {
        const QString message = QString("Translation string does not end with the same punctuation character '%1' or "
                                        "vice versa. ").arg(c) + QString("Original name:'%1'").arg(source) +
                QString(", translated name:'%1'").arg(translation);
        QFAIL(qUtf8Printable(message));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::TestHTMLTags_data()
{
    const QStringList locales = SupportedLocales();

    {
        QDir dir(TS_DIR);
        const QStringList fileNames = dir.entryList(QStringList("valentina*.ts"));
        QVERIFY2(locales.size() == fileNames.size()-1, "Unexpected count of files.");
    }

    QTest::addColumn<QString>("source");
    QTest::addColumn<QString>("translation");

    for(int j = 0; j < locales.size(); ++j)
    {
        const QString filename = QString("valentina_%1.ts").arg(locales.at(j));

        const QDomNodeList messages = LoadTSFile(filename);
        if (messages.isEmpty())
        {
            QFAIL("Can't begin test.");
        }

        for (qint32 i = 0, num = messages.size(); i < num; ++i)
        {
            const QDomElement message = messages.at(i).toElement();
            if (message.isNull() == false)
            {
                const QString source = message.firstChildElement(TagSource).text();
                if (source.isEmpty())
                {
                    continue;
                }

                const QDomElement translationTag = message.firstChildElement(TagTranslation);
                if (translationTag.hasAttribute(AttrType))
                {
                    const QString attrVal = translationTag.attribute(AttrType);
                    if (attrVal == AttrValVanished || attrVal == AttrValUnfinished || attrVal == AttrValObsolete)
                    {
                        continue;
                    }
                }
                const QString translation = translationTag.text();
                if (translation.isEmpty())
                {
                    continue;
                }

                const QString message = QString("File '%1'.").arg(filename);
                QTest::newRow(qUtf8Printable(message)) << source << translation;
            }
            else
            {
                const QString message = QString("File '%2'. Message %1 is null.").arg(i).arg(filename);
                QFAIL(qUtf8Printable(message));
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::TestHTMLTags()
{
    QFETCH(QString, source);
    QFETCH(QString, translation);

    static const QStringList tags = QStringList() << QLatin1String("p")
                                                  << QLatin1String("html")
                                                  << QLatin1String("body");
    static const QString pattern("{1}.*>");
    for (int i=0; i < tags.size(); ++i)
    {
        const QRegularExpression openRegex(QLatin1String("<") + tags.at(i) + pattern,
                                           QRegularExpression::DotMatchesEverythingOption);
        if (source.contains(openRegex))
        {
            const int countOpenTag = source.count(openRegex);
            const QRegularExpression closeRegex(QLatin1String("</") + tags.at(i) + pattern,
                                                QRegularExpression::DotMatchesEverythingOption);
            const int countCloseTag = translation.count(closeRegex);
            if (not translation.contains(closeRegex) || countCloseTag != countOpenTag)
            {
                const QString message = QString("Tag mismatch. Tag: '<%1>'. ").arg(tags.at(i)) +
                        QString("Original name:'%1'").arg(source) + QString(", translated name:'%1'").arg(translation);
                QFAIL(qUtf8Printable(message));
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QDomNodeList TST_TSTranslation::LoadTSFile(const QString &filename)
{
    tsFile.reset();
    tsFile = QSharedPointer<QFile>(new QFile(QString("%1/%2").arg(TS_DIR).arg(filename)));
    if (not tsFile->exists())
    {
        const QString message = QString("Can't find '%1'.\n%2.").arg(filename).arg(tsFile->errorString());
        QWARN(qUtf8Printable(message));
        return QDomNodeList();
    }

    if (tsFile->open(QIODevice::ReadOnly) == false)
    {
        const QString message = QString("Can't open file '%1'.\n%2.").arg(filename).arg(tsFile->errorString());
        QWARN(qUtf8Printable(message));
        return QDomNodeList();
    }

    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;
    tsXML.reset();
    tsXML = QSharedPointer<QDomDocument>(new QDomDocument());
    if (tsXML->setContent(tsFile.data(), &errorMsg, &errorLine, &errorColumn) == false)
    {
        const QString message = QString("Parsing error file %1 in line %2 column %3.")
                .arg(filename).arg(errorLine).arg(errorColumn);
        QWARN(qUtf8Printable(message));
        return QDomNodeList();
    }

    const QDomNodeList messages = tsXML->elementsByTagName(TagMessage);
    if (messages.isEmpty())
    {
        QWARN("File doesn't contain any messages.");
        return QDomNodeList();
    }

    return messages;
}
