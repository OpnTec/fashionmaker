/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 10, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "tst_abstractregexp.h"
#include "../qmuparser/qmudef.h"

#include "../vmisc/logging.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../ifc/ifcdef.h"

#include <QtTest>
#include <QTranslator>

//---------------------------------------------------------------------------------------------------------------------
TST_AbstractRegExp::TST_AbstractRegExp(const QString &locale, QObject *parent)
    : AbstractTest(parent),
    m_locale(locale),
    m_vTranslator(nullptr),
    m_trMs(nullptr)
{
}

//---------------------------------------------------------------------------------------------------------------------
TST_AbstractRegExp::~TST_AbstractRegExp()
{
    delete m_vTranslator;
    delete m_trMs;
}

//---------------------------------------------------------------------------------------------------------------------
int TST_AbstractRegExp::LoadVariables(const QString &checkedLocale)
{
    const QString path = TranslationsPath();
    const QString file = QString("valentina_%1.qm").arg(checkedLocale);

    if (QFileInfo(path+QLatin1String("/")+file).size() <= 34)
    {
        const QString message = QString("Translation variables for locale = %1 is empty. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        return ErrorSize;
    }

    m_vTranslator = new QTranslator(this);

    if (not m_vTranslator->load(file, path))
    {
        const QString message = QString("Can't load translation variables for locale = %1. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete m_vTranslator;

        return ErrorLoad;
    }

    if (not QCoreApplication::installTranslator(m_vTranslator))
    {
        const QString message = QString("Can't install translation variables for locale = %1. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete m_vTranslator;

        return ErrorInstall;
    }

    return NoError;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_AbstractRegExp::RemoveTrVariables(const QString &checkedLocale)
{
    if (not m_vTranslator.isNull())
    {
        const bool result = QCoreApplication::removeTranslator(m_vTranslator);

        if (result == false)
        {
            const QString message = QString("Can't remove translation variables for locale = %1")
                    .arg(checkedLocale);
            QWARN(qUtf8Printable(message));
        }
        delete m_vTranslator;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_AbstractRegExp::InitTrMs()
{
    if (m_trMs != nullptr)
    {
        m_trMs->Retranslate();
    }
    else
    {
        m_trMs = new VTranslateVars();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_AbstractRegExp::CallTestCheckNoEndLine()
{
    QFETCH(QString, originalName);

    const QString translated = m_trMs->VarToUser(originalName);
    if (translated.endsWith(QLatin1String("\n")))
    {
        const QString message = QString("Translated string '%1' shouldn't contain new line character.")
                .arg(translated);
        QFAIL(qUtf8Printable(message));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_AbstractRegExp::CallTestCheckRegExpNames()
{
    QFETCH(QString, originalName);

    static const QRegularExpression re(NameRegExp());
    const QString translated = m_trMs->VarToUser(originalName);
    if (not re.match(translated).hasMatch())
    {
        const QString message = QString("Original name:'%1', translated name:'%2'").arg(originalName).arg(translated);
        QFAIL(qUtf8Printable(message));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_AbstractRegExp::CallTestCheckIsNamesUnique()
{
    QFETCH(QString, originalName);

    QSet<QString> names;

    const QString translated = m_trMs->VarToUser(originalName);
    if (names.contains(translated))
    {
        const QString message = QString("Name is not unique. Original name:'%1', translated name:'%2'")
                .arg(originalName).arg(translated);
        QFAIL(qUtf8Printable(message));
    }
    names.insert(translated);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_AbstractRegExp::CallTestCheckNoOriginalNamesInTranslation()
{
    QFETCH(QString, originalName);

    static const QStringList originalNames = AllNames();
    static const QSet<QString> names = QSet<QString>::fromList(originalNames);

    const QString translated = m_trMs->VarToUser(originalName);
    if (names.contains(translated))
    {
        if (originalName != translated)
        {
            const QString message = QString("Translation repeat original name from other place. "
                                            "Original name:'%1', translated name:'%2'")
                    .arg(originalName).arg(translated);
            QFAIL(qUtf8Printable(message));
        }
    }
}

