/************************************************************************
 **
 **  @file   tst_measurementregexp.h
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

#ifndef TST_MEASUREMENTREGEXP_H
#define TST_MEASUREMENTREGEXP_H

#include "tst_abstractregexp.h"

#include <QPointer>

class QTranslator;
class VTranslateVars;

class TST_MeasurementRegExp : public TST_AbstractRegExp
{
    Q_OBJECT
public:
    TST_MeasurementRegExp(quint32 systemCode, const QString &locale, QObject *parent = nullptr);
    virtual ~TST_MeasurementRegExp();

    static const quint32 systemCounts;

protected:
    virtual void        PrepareData() override;
    virtual QStringList AllNames() override;

private slots:
    void initTestCase();
    void TestCheckNoEndLine_data();
    void TestCheckNoEndLine();
    void TestCheckRegExpNames_data();
    void TestCheckRegExpNames();
    void TestCheckIsNamesUnique_data();
    void TestCheckIsNamesUnique();
    void TestCheckNoOriginalNamesInTranslation_data();
    void TestCheckNoOriginalNamesInTranslation();
    void TestForValidChars_data();
    void TestForValidChars();
    void cleanupTestCase();

private:
    Q_DISABLE_COPY(TST_MeasurementRegExp)

    quint32               m_systemCode;
    QString               m_system;
    QPointer<QTranslator> m_pmsTranslator;

    void    TestCombinations(int systemCounts, const QStringList &locales) const;

    int     LoadMeasurements(const QString &checkedSystem, const QString &checkedLocale);
    void    RemoveTrMeasurements(const QString &checkedSystem, const QString &checkedLocale);
};

#endif // TST_MEASUREMENTREGEXP_H
