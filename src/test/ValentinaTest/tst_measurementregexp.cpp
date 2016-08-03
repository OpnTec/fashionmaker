/************************************************************************
 **
 **  @file   tst_measurementregexp.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2015
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

#include "tst_measurementregexp.h"
#include "../qmuparser/qmudef.h"

#include "../vmisc/logging.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../ifc/ifcdef.h"

#include <QtTest>
#include <QTranslator>

const quint32 TST_MeasurementRegExp::systemCounts = 56; // count of pattern making systems

//---------------------------------------------------------------------------------------------------------------------
TST_MeasurementRegExp::TST_MeasurementRegExp(quint32 systemCode, const QString &locale, QObject *parent)
    : AbstractTest(parent),
      m_systemCode(systemCode),
      m_system(),
      m_locale(locale),
      pmsTranslator(nullptr),
      vTranslator(nullptr),
      trMs(nullptr)
{
}

//---------------------------------------------------------------------------------------------------------------------
TST_MeasurementRegExp::~TST_MeasurementRegExp()
{
    delete pmsTranslator;
    delete vTranslator;
    delete trMs;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::initTestCase()
{
    if (m_systemCode > systemCounts)
    {
        QFAIL("Unexpected system code.");
    }

    m_system = QString("p%1").arg(m_systemCode);

    if (m_locale.isEmpty())
    {
        QFAIL("Empty locale code.");
    }

    const QStringList locales = SupportedLocales();

    if (not locales.contains(m_locale))
    {
        QFAIL("Unsupported locale code.");
    }

    TestCombinations(systemCounts, locales);

    if (LoadMeasurements(m_system, m_locale) != NoError)
    {
        const QString message = QString("Couldn't load measurements. System = %1, locale = %2")
                .arg(m_system)
                .arg(m_locale);
        QSKIP(qUtf8Printable(message));
    }

    if (LoadVariables(m_locale) != NoError)
    {
        const QString message = QString("Couldn't load variables. System = %1, locale = %2")
                .arg(m_system)
                .arg(m_locale);
        QSKIP(qUtf8Printable(message));
    }

    InitTrMs();//Very important do this after loading QM files.
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckNoEndLine_data()
{
    PrepareMeasurementData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckNoEndLine()
{
    QFETCH(QString, originalName);

    const QString translated = trMs->VarToUser(originalName);
    if (translated.endsWith(QLatin1String("\n")))
    {
        const QString message = QString("Translated string '%1' shouldn't contain new line character.")
                .arg(translated);
        QFAIL(qUtf8Printable(message));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckRegExpNames_data()
{
    PrepareMeasurementData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckRegExpNames()
{
    QFETCH(QString, originalName);

    static const QRegularExpression re(NameRegExp());
    const QString translated = trMs->VarToUser(originalName);
    if (not re.match(translated).hasMatch())
    {
        const QString message = QString("Original name:'%1', translated name:'%2'").arg(originalName).arg(translated);
        QFAIL(qUtf8Printable(message));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckIsNamesUnique_data()
{
    PrepareMeasurementData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckIsNamesUnique()
{
    QFETCH(QString, originalName);

    QSet<QString> names;

    const QString translated = trMs->VarToUser(originalName);
    if (names.contains(translated))
    {
        const QString message = QString("Name is not unique. Original name:'%1', translated name:'%2'")
                .arg(originalName).arg(translated);
        QFAIL(qUtf8Printable(message));
    }
    names.insert(translated);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckNoOriginalNamesInTranslation_data()
{
    PrepareMeasurementData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckNoOriginalNamesInTranslation()
{
    QFETCH(QString, originalName);

    static const QStringList originalNames = AllGroupNames() + builInFunctions + builInVariables;
    static const QSet<QString> names = QSet<QString>::fromList(originalNames);

    const QString translated = trMs->VarToUser(originalName);
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

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckUnderlineExists_data()
{
    QMap<QString, bool> data;

    data.insert(line_, true);
    data.insert(angleLine_, true);
    data.insert(arc_, true);
    data.insert(spl_, true);
    data.insert(splPath, false);
    data.insert(radius_V, false);
    data.insert(radiusArc_, true);
    data.insert(angle1_V, false);
    data.insert(angle2_V, false);
    data.insert(angle1Arc_, true);
    data.insert(angle2Arc_, true);
    data.insert(angle1Spl_, true);
    data.insert(angle2Spl_, true);
    data.insert(angle1SplPath, false);
    data.insert(angle2SplPath, false);
    data.insert(seg_, true);
    data.insert(currentLength, false);

    //Catch case when new internal variable appears.
    QCOMPARE(data.size(), builInVariables.size());

    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("exists");

    auto i = data.constBegin();
    while (i != data.constEnd())
    {
        const QString tag = QString("System: '%1', locale: '%2'. Name '%3'").arg(m_system).arg(m_locale).arg(i.key());
        QTest::newRow(qUtf8Printable(tag)) << i.key() << i.value();
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckUnderlineExists()
{
    QFETCH(QString, name);
    QFETCH(bool, exists);

    const QString translated = trMs->InternalVarToUser(name);
    if ((translated.right(1) == QLatin1String("_")) != exists)
    {
        const QString message = QString("String '%1' doesn't contain underline. Original string is '%2'")
                .arg(translated).arg(name);
        QFAIL(qUtf8Printable(message));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckInternalVaribleRegExp_data()
{
    QTest::addColumn<QString>("var");
    QTest::addColumn<QString>("originalName");

    foreach(const QString &var, builInVariables)
    {
        const QString tag = QString("System: '%1', locale: '%2'. Var '%3'").arg(m_system).arg(m_locale).arg(var);
        const QStringList originalNames = AllGroupNames() + builInFunctions + builInVariables;
        foreach(const QString &str, originalNames)
        {
            QTest::newRow(qUtf8Printable(tag)) << var << str;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckInternalVaribleRegExp()
{
    QFETCH(QString, var);
    QFETCH(QString, originalName);

    static const QString regex = QStringLiteral("(.){1,}_(.){1,}$");

    const QString sourceRegex = QLatin1String("^") + var + regex;
    const QRegularExpression sourceRe(sourceRegex);

    const QString translated = trMs->InternalVarToUser(var);
    const QString translationRegex = QLatin1String("^") + translated + regex;
    const QRegularExpression translationRe(translationRegex);

    {
        if (sourceRe.match(originalName).hasMatch() || translationRe.match(originalName).hasMatch())
        {
            const QString message = QString("Invalid original string '%1'").arg(originalName);
            QFAIL(qUtf8Printable(message));
        }

        const QString translated = trMs->VarToUser(originalName);
        if (sourceRe.match(translated).hasMatch() || translationRe.match(translated).hasMatch())
        {
            const QString message = QString("Invalid translation string '%1'").arg(translated);
            QFAIL(qUtf8Printable(message));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::cleanupTestCase()
{
    RemoveTrMeasurements(m_system, m_locale);
    RemoveTrVariables(m_locale);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCombinations(int systemCounts, const QStringList &locales) const
{
    const int combinations = systemCounts * locales.size();

    QDir dir(TranslationsPath());
    const QStringList fileNames = dir.entryList(QStringList("measurements_p*_*.qm"));

    QVERIFY2(combinations == fileNames.size(), "Unexpected count of files.");
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::PrepareMeasurementData()
{
    static const QStringList originalNames = AllGroupNames() + builInFunctions + builInVariables;

    QTest::addColumn<QString>("originalName");

    foreach(const QString &str, originalNames)
    {
        const QString tag = QString("System: '%1', locale: '%2'. Name '%3'").arg(m_system).arg(m_locale).arg(str);
        QTest::newRow(qUtf8Printable(tag)) << str;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int TST_MeasurementRegExp::LoadMeasurements(const QString &checkedSystem, const QString &checkedLocale)
{
    const QString path = TranslationsPath();
    const QString file = QString("measurements_%1_%2.qm").arg(checkedSystem).arg(checkedLocale);

    if (QFileInfo(path+QLatin1String("/")+file).size() <= 34)
    {
        const QString message = QString("Translation for system = %1 and locale = %2 is empty. \nFull path: %3/%4")
                .arg(checkedSystem)
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        return ErrorSize;
    }

    delete pmsTranslator;
    pmsTranslator = new QTranslator(this);

    if (not pmsTranslator->load(file, path))
    {
        const QString message = QString("Can't load translation for system = %1 and locale = %2. \nFull path: %3/%4")
                .arg(checkedSystem)
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete pmsTranslator;

        return ErrorLoad;
    }

    if (not QCoreApplication::installTranslator(pmsTranslator))
    {
        const QString message = QString("Can't install translation for system = %1 and locale = %2. \nFull path: %3/%4")
                .arg(checkedSystem)
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete pmsTranslator;

        return ErrorInstall;
    }

    return NoError;
}

//---------------------------------------------------------------------------------------------------------------------
int TST_MeasurementRegExp::LoadVariables(const QString &checkedLocale)
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

    vTranslator = new QTranslator(this);

    if (not vTranslator->load(file, path))
    {
        const QString message = QString("Can't load translation variables for locale = %1. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete vTranslator;

        return ErrorLoad;
    }

    if (not QCoreApplication::installTranslator(vTranslator))
    {
        const QString message = QString("Can't install translation variables for locale = %1. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete vTranslator;

        return ErrorInstall;
    }

    return NoError;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::RemoveTrMeasurements(const QString &checkedSystem, const QString &checkedLocale)
{
    if (not pmsTranslator.isNull())
    {
        const bool result = QCoreApplication::removeTranslator(pmsTranslator);

        if (result == false)
        {
            const QString message = QString("Can't remove translation for system = %1 and locale = %2")
                    .arg(checkedSystem)
                    .arg(checkedLocale);
            QWARN(qUtf8Printable(message));
        }
        delete pmsTranslator;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::RemoveTrVariables(const QString &checkedLocale)
{
    if (not vTranslator.isNull())
    {
        const bool result = QCoreApplication::removeTranslator(vTranslator);

        if (result == false)
        {
            const QString message = QString("Can't remove translation variables for locale = %1")
                    .arg(checkedLocale);
            QWARN(qUtf8Printable(message));
        }
        delete vTranslator;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::InitTrMs()
{
    if (trMs != nullptr)
    {
        trMs->Retranslate();
    }
    else
    {
        trMs = new VTranslateVars();
    }
}
