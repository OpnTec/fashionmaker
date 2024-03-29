/************************************************************************
 **
 **  @file   tst_measurementregexp.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include "../vpatterndb/vtranslatevars.h"
#include "../vpatterndb/measurements.h"
#include "../ifc/ifcdef.h"

#include <QtTest>
#include <QTranslator>

const quint32 TST_MeasurementRegExp::systemCounts = 56; // count of pattern making systems

//---------------------------------------------------------------------------------------------------------------------
TST_MeasurementRegExp::TST_MeasurementRegExp(quint32 systemCode, const QString &locale, QObject *parent)
    : TST_AbstractRegExp(locale, parent),
      m_systemCode(systemCode),
      m_system(),
      m_pmsTranslator(nullptr)
{
}

//---------------------------------------------------------------------------------------------------------------------
TST_MeasurementRegExp::~TST_MeasurementRegExp()
{
    delete m_pmsTranslator;
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
                .arg(m_system, m_locale);
        QSKIP(qUtf8Printable(message));
    }

    if (LoadVariables(m_locale) != NoError)
    {
        const QString message = QString("Couldn't load variables. System = %1, locale = %2")
                .arg(m_system, m_locale);
        QSKIP(qUtf8Printable(message));
    }

    QLocale::setDefault(QLocale(m_locale));

    InitTrMs();//Very important do this after loading QM files.
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckNoEndLine_data()
{
    PrepareData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckNoEndLine()
{
    CallTestCheckNoEndLine();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckRegExpNames_data()
{
    PrepareData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckRegExpNames()
{
    CallTestCheckRegExpNames();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckIsNamesUnique_data()
{
    PrepareData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckIsNamesUnique()
{
    CallTestCheckIsNamesUnique();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckNoOriginalNamesInTranslation_data()
{
    PrepareData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestCheckNoOriginalNamesInTranslation()
{
    CallTestCheckNoOriginalNamesInTranslation();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestForValidChars_data()
{
    PrepareData();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::TestForValidChars()
{
    CallTestForValidCharacters();
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

    const QString error = QString("Unexpected count of files. Excpected %1, got %2.")
            .arg(combinations).arg(fileNames.size());
    QVERIFY2(combinations == fileNames.size(), qUtf8Printable(error));
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::PrepareData()
{
    static const QStringList originalNames = AllNames();

    QTest::addColumn<QString>("originalName");

    for (auto &str : originalNames)
    {
        const QString tag = QString("System: '%1', locale: '%2'. Name '%3'").arg(m_system, m_locale, str);
        QTest::newRow(qUtf8Printable(tag)) << str;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList TST_MeasurementRegExp::AllNames()
{
    return AllGroupNames();
}

//---------------------------------------------------------------------------------------------------------------------
int TST_MeasurementRegExp::LoadMeasurements(const QString &checkedSystem, const QString &checkedLocale)
{
    const QString path = TranslationsPath();
    const QString file = QString("measurements_%1_%2.qm").arg(checkedSystem, checkedLocale);

    if (QFileInfo(path+QLatin1String("/")+file).size() <= 34)
    {
        const QString message = QString("Translation for system = %1 and locale = %2 is empty. \nFull path: %3/%4")
                .arg(checkedSystem, checkedLocale, path, file);
        QWARN(qUtf8Printable(message));

        return ErrorSize;
    }

    delete m_pmsTranslator;
    m_pmsTranslator = new QTranslator(this);

    if (not m_pmsTranslator->load(file, path))
    {
        const QString message = QString("Can't load translation for system = %1 and locale = %2. \nFull path: %3/%4")
                .arg(checkedSystem, checkedLocale, path, file);
        QWARN(qUtf8Printable(message));

        delete m_pmsTranslator;

        return ErrorLoad;
    }

    if (not QCoreApplication::installTranslator(m_pmsTranslator))
    {
        const QString message = QString("Can't install translation for system = %1 and locale = %2. \nFull path: %3/%4")
                .arg(checkedSystem, checkedLocale, path, file);
        QWARN(qUtf8Printable(message));

        delete m_pmsTranslator;

        return ErrorInstall;
    }

    return NoError;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::RemoveTrMeasurements(const QString &checkedSystem, const QString &checkedLocale)
{
    if (not m_pmsTranslator.isNull())
    {
        const bool result = QCoreApplication::removeTranslator(m_pmsTranslator);

        if (result == false)
        {
            const QString message = QString("Can't remove translation for system = %1 and locale = %2")
                    .arg(checkedSystem, checkedLocale);
            QWARN(qUtf8Printable(message));
        }
        delete m_pmsTranslator;
    }
}
