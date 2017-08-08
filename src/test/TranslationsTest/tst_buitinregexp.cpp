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

#include "tst_buitinregexp.h"
#include "../qmuparser/qmudef.h"

#include "../vmisc/logging.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../ifc/ifcdef.h"

#include <QtTest>
#include <QTranslator>

//---------------------------------------------------------------------------------------------------------------------
TST_BuitInRegExp::TST_BuitInRegExp(const QString &locale, QObject *parent)
    : TST_AbstractRegExp(locale, parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::initTestCase()
{
    if (m_locale.isEmpty())
    {
        QFAIL("Empty locale code.");
    }

    const QStringList locales = SupportedLocales();

    if (not locales.contains(m_locale))
    {
        QFAIL("Unsupported locale code.");
    }

    if (LoadVariables(m_locale) != NoError)
    {
        const QString message = QString("Couldn't load variables. Locale = %1").arg(m_locale);
        QSKIP(qUtf8Printable(message));
    }

    InitTrMs();//Very important do this after loading QM files.
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckNoEndLine_data()
{
    PrepareData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckNoEndLine()
{
    CallTestCheckNoEndLine();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckRegExpNames_data()
{
    PrepareData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckRegExpNames()
{
    CallTestCheckRegExpNames();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckIsNamesUnique_data()
{
    PrepareData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckIsNamesUnique()
{
    CallTestCheckIsNamesUnique();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckNoOriginalNamesInTranslation_data()
{
    PrepareData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckNoOriginalNamesInTranslation()
{
    CallTestCheckNoOriginalNamesInTranslation();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckUnderlineExists_data()
{
    QMap<QString, bool> data;

    data.insert(measurement_, true);
    data.insert(increment_, true);
    data.insert(line_, true);
    data.insert(angleLine_, true);
    data.insert(arc_, true);
    data.insert(elarc_, true);
    data.insert(spl_, true);
    data.insert(splPath, false);
    data.insert(radiusArc_, true);
    data.insert(radius1ElArc_, true);
    data.insert(radius2ElArc_, true);
    data.insert(angle1Arc_, true);
    data.insert(angle2Arc_, true);
    data.insert(angle1ElArc_, true);
    data.insert(angle2ElArc_, true);
    data.insert(angle1Spl_, true);
    data.insert(angle2Spl_, true);
    data.insert(angle1SplPath, false);
    data.insert(angle2SplPath, false);
    data.insert(seg_, true);
    data.insert(currentLength, false);
    data.insert(currentSeamAllowance, false);
    data.insert(c1LengthSpl_, true);
    data.insert(c2LengthSpl_, true);
    data.insert(c1LengthSplPath, false);
    data.insert(c2LengthSplPath, false);

    //Catch case when new internal variable appears.
    QCOMPARE(data.size(), builInVariables.size());

    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("exists");

    auto i = data.constBegin();
    while (i != data.constEnd())
    {
        const QString tag = QString("Locale: '%1'. Name '%2'").arg(m_locale).arg(i.key());
        QTest::newRow(qUtf8Printable(tag)) << i.key() << i.value();
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckUnderlineExists()
{
    QFETCH(QString, name);
    QFETCH(bool, exists);

    const QString translated = m_trMs->InternalVarToUser(name);
    if ((translated.right(1) == QLatin1String("_")) != exists)
    {
        const QString message = QString("String '%1' doesn't contain underline. Original string is '%2'")
                .arg(translated).arg(name);
        QFAIL(qUtf8Printable(message));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckInternalVaribleRegExp_data()
{
    QTest::addColumn<QString>("var");
    QTest::addColumn<QString>("originalName");

    foreach(const QString &var, builInVariables)
    {
        const QString tag = QString("Locale: '%1'. Var '%2'").arg(m_locale).arg(var);
        const QStringList originalNames = AllNames();
        foreach(const QString &str, originalNames)
        {
            QTest::newRow(qUtf8Printable(tag)) << var << str;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::TestCheckInternalVaribleRegExp()
{
    QFETCH(QString, var);
    QFETCH(QString, originalName);

    static const QString regex = QStringLiteral("(.){1,}_(.){1,}$");

    const QString sourceRegex = QLatin1String("^") + var + regex;
    const QRegularExpression sourceRe(sourceRegex);

    const QString translated = m_trMs->InternalVarToUser(var);
    const QString translationRegex = QLatin1String("^") + translated + regex;
    const QRegularExpression translationRe(translationRegex);

    {
        if (sourceRe.match(originalName).hasMatch() || translationRe.match(originalName).hasMatch())
        {
            const QString message = QString("Invalid original string '%1'").arg(originalName);
            QFAIL(qUtf8Printable(message));
        }

        const QString translated = m_trMs->VarToUser(originalName);
        if (sourceRe.match(translated).hasMatch() || translationRe.match(translated).hasMatch())
        {
            const QString message = QString("Invalid translation string '%1'").arg(translated);
            QFAIL(qUtf8Printable(message));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::cleanupTestCase()
{
    RemoveTrVariables(m_locale);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_BuitInRegExp::PrepareData()
{
    static const QStringList originalNames = AllNames();

    QTest::addColumn<QString>("originalName");

    foreach(const QString &str, originalNames)
    {
        const QString tag = QString("Locale: '%1'. Name '%2'").arg(m_locale).arg(str);
        QTest::newRow(qUtf8Printable(tag)) << str;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList TST_BuitInRegExp::AllNames()
{
    return builInFunctions + builInVariables;
}
