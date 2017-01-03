/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   30 12, 2016
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

#include "tst_readval.h"
#include "../qmuparser/qmudef.h"
#include "../vmisc/logging.h"

#include <QtTest>
#include <limits>

//---------------------------------------------------------------------------------------------------------------------
TST_ReadVal::TST_ReadVal(QObject *parent)
    : QObject(parent),
      m_systemLocale(QLocale::system())
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ReadVal::TestReadVal_data()
{
    QTest::addColumn<QString>("formula");
    QTest::addColumn<int>("expCount");
    QTest::addColumn<qreal>("expVal");
    QTest::addColumn<QLocale>("locale");

    const QList<QLocale> allLocales =
            QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);
    for(int i = 0; i < allLocales.size(); ++i)
    {
        const QLocale locale = allLocales.at(i);
        PrepareVal(1., locale);
        PrepareVal(1.0, locale);
        PrepareVal(-1.0, locale);
        PrepareVal(1.5, locale);
        PrepareVal(-1.5, locale);
        PrepareVal(1000.0, locale);
        PrepareVal(-1000.0, locale);
        PrepareVal(1000.5, locale);
        PrepareVal(-1000.5, locale);
        PrepareVal(std::numeric_limits<double>::max(), locale);
        PrepareVal(-std::numeric_limits<double>::max(), locale);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ReadVal::TestReadVal()
{
    TestVal();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ReadVal::TestInvalidData_data()
{
    QTest::addColumn<QString>("formula");
    QTest::addColumn<int>("expCount");
    QTest::addColumn<qreal>("expVal");
    QTest::addColumn<QLocale>("locale");

    // Test invalid values
    const QLocale locale = QLocale::c();
    PrepareString(QString(), locale);
    PrepareString(QString("-1.000.5"), locale);
    PrepareString(QString("1.000.5"), locale);
    PrepareString(QString("-1.000,5"), locale);
    PrepareString(QString("1.000,5"), locale);
    PrepareString(QString("-1.0.00,5"), locale);
    PrepareString(QString("1.0.00,5"), locale);
    PrepareString(QString("7,5"), locale);
    PrepareString(QString("-7,5"), locale);
    PrepareString(QString("- 7,5"), locale);
    PrepareString(QString("- 7.5"), locale);
    PrepareString(QString("1,0,00.5"), locale);
    PrepareString(QString("1,,000.5"), locale);
    PrepareString(QString(",5"), locale);
    PrepareString(QString(","), locale);
    PrepareString(QString("."), locale);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ReadVal::TestInvalidData()
{
    TestVal();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ReadVal::cleanupTestCase()
{
    QLocale::setDefault(m_systemLocale);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ReadVal::PrepareVal(qreal val, const QLocale &locale)
{
    const QString string = locale.toString(val);
    bool ok = false;
    const double d = locale.toDouble(string, &ok);
    PrepareString(string, locale, d, string.size());
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ReadVal::PrepareString(const QString &str, const QLocale &locale, qreal val,  int count)
{
    const QString tag = QString("%1. String '%2'").arg(locale.name()).arg(str);
    QTest::newRow(qUtf8Printable(tag)) << str << count << val << locale;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_ReadVal::TestVal()
{
    QFETCH(QString, formula);
    QFETCH(int, expCount);
    QFETCH(qreal, expVal);
    QFETCH(QLocale, locale);

    qreal resVal = 0;
    QLocale::setDefault(locale);

    const int resCount = ReadVal(formula, resVal, locale, locale.decimalPoint(), locale.groupSeparator());

    QString errorMsg = QString("Conversion failed. Locale: '%1'.").arg(locale.name());
    QVERIFY2(resCount == expCount, qUtf8Printable(errorMsg));

    if (resCount != -1)
    {
        QString errorMsg = QString("Unexpected result. Locale: '%1'.").arg(locale.name());
        QVERIFY2(QmuFuzzyComparePossibleNulls(resVal, expVal), qUtf8Printable(errorMsg));
    }
}
