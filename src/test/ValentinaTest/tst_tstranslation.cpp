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

#include <QDomDocument>
#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_TSTranslation::TST_TSTranslation(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_TSTranslation::CheckEnglishLocalization()
{
    const QString path = QString("%1/valentina_en_US.ts").arg(TS_DIR);
    QFile tsFile(path);
    if (not tsFile.exists())
    {
        QSKIP("Can't find valentina_en_US.ts");
    }

    if (tsFile.open(QIODevice::ReadOnly) == false)
    {
        const QString message = QString("Can't open file valentina_en_US.ts.\n%1").arg(tsFile.errorString());
        QSKIP(qUtf8Printable(message));
    }

    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;
    QDomDocument ts;
    if (ts.setContent(&tsFile, &errorMsg, &errorLine, &errorColumn) == false)
    {
        tsFile.close();
        const QString message = QString("Parsing error file valentina_en_US.ts in line %1 column %2")
                .arg(errorLine).arg(errorColumn);
        QFAIL(qUtf8Printable(message));
    }

    const QDomNodeList messages = ts.elementsByTagName(QStringLiteral("message"));
    if (messages.isEmpty())
    {
        tsFile.close();
        QFAIL("File doesn't contain any messages");
    }

    const QString attrType = QStringLiteral("type");
    const qint32 num = messages.size();
    for (qint32 i = 0; i < num; ++i)
    {
        const QDomElement message = messages.at(i).toElement();
        if (message.isNull() == false)
        {
            const QString source = message.firstChildElement(QStringLiteral("source")).text();
            if (source.isEmpty())
            {
                continue;
            }

            if (message.hasAttribute(attrType))
            {
                const QString attrVal = message.attribute(attrType);
                if (attrVal == QLatin1Literal("vanished") || attrVal == QLatin1Literal("unfinished"))
                {
                    continue;
                }
            }
            const QString translation = message.firstChildElement(QStringLiteral("translation")).text();
            if (translation.isEmpty())
            {
                continue;
            }

            QCOMPARE(source, translation);
        }
        else
        {
            tsFile.close();
            const QString message = QString("Message %1 is null.").arg(i);
            QFAIL(qUtf8Printable(message));
        }
    }
}
