/************************************************************************
 **
 **  @file   tst_abstracttranslation.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 2, 2018
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2018 Valentina project
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
#include "tst_abstracttranslation.h"

#include <QtTest>

const QString TST_AbstractTranslation::TagName           = QStringLiteral("name");
const QString TST_AbstractTranslation::TagMessage        = QStringLiteral("message");
const QString TST_AbstractTranslation::TagSource         = QStringLiteral("source");
const QString TST_AbstractTranslation::TagTranslation    = QStringLiteral("translation");

const QString TST_AbstractTranslation::AttrType          = QStringLiteral("type");
const QString TST_AbstractTranslation::AttrValVanished   = QStringLiteral("vanished");
const QString TST_AbstractTranslation::AttrValUnfinished = QStringLiteral("unfinished");
const QString TST_AbstractTranslation::AttrValObsolete   = QStringLiteral("obsolete");

//---------------------------------------------------------------------------------------------------------------------
TST_AbstractTranslation::TST_AbstractTranslation(QObject *parent)
    : QObject(parent),
      tsFile(),
      tsXML()
{}

//---------------------------------------------------------------------------------------------------------------------
QDomNodeList TST_AbstractTranslation::LoadTSFile(const QString &filename)
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
