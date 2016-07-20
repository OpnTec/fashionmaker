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

const QString TST_TSTranslation::TagName           = QStringLiteral("name");
const QString TST_TSTranslation::TagMessage        = QStringLiteral("message");
const QString TST_TSTranslation::TagSource         = QStringLiteral("source");
const QString TST_TSTranslation::TagTranslation    = QStringLiteral("translation");

const QString TST_TSTranslation::AttrType          = QStringLiteral("type");
const QString TST_TSTranslation::AttrValVanished   = QStringLiteral("vanished");
const QString TST_TSTranslation::AttrValUnfinished = QStringLiteral("unfinished");
const QString TST_TSTranslation::AttrValObsolete   = QStringLiteral("obsolete");

//---------------------------------------------------------------------------------------------------------------------
TST_TSTranslation::TST_TSTranslation(QObject *parent) :
    QObject(parent),
    tsFile(),
    tsXML()
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::CheckEnglishLocalization()
{
    const QDomNodeList messages = LoadTSFile(QStringLiteral("valentina_en_US.ts"));
    if (messages.isEmpty())
    {
        QSKIP("Can't begin test.");
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

            QCOMPARE(source, translation);
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
    const QDomNodeList messages = LoadTSFile(QStringLiteral("valentina.ts"));
    if (messages.isEmpty())
    {
        QSKIP("Can't begin test.");
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

            if (source == QLatin1String("..."))
            {
                const QDomElement translationTag = message.firstChildElement(TagTranslation);
                if (translationTag.hasAttribute(AttrType))
                {
                    const QString attrVal = translationTag.attribute(AttrType);
                    if (attrVal == AttrValVanished || attrVal == AttrValObsolete)
                    {
                        continue;
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
        else
        {
            const QString message = QString("Message %1 is null.").arg(i);
            QFAIL(qUtf8Printable(message));
        }
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

    QTest::addColumn<QString>("filename");

    for(int i = 0; i < locales.size(); ++i)
    {
        const QString filename = QString("valentina_%1.ts").arg(locales.at(i));
        const QString tag = QString("Check localization strings valentina_%1.ts").arg(locales.at(i));
        QTest::newRow(qUtf8Printable(tag)) << filename;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::CheckPlaceMarkerExist()
{
    QFETCH(QString, filename);

    const QDomNodeList messages = LoadTSFile(filename);
    if (messages.isEmpty())
    {
        QSKIP("Can't begin test.");
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
        else
        {
            const QString message = QString("Message %1 is null.").arg(i);
            QFAIL(qUtf8Printable(message));
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
        const QString message = QString("Parsing error file valentina_en_US.ts in line %1 column %2.")
                .arg(errorLine).arg(errorColumn);
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
