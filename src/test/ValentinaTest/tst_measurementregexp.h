/************************************************************************
 **
 **  @file   tst_measurementregexp.h
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

#ifndef TST_MEASUREMENTREGEXP_H
#define TST_MEASUREMENTREGEXP_H

#include "abstracttest.h"

#include <QPointer>

class QTranslator;
class VTranslateMeasurements;

class TST_MeasurementRegExp : public AbstractTest
{
    Q_OBJECT
public:
    explicit TST_MeasurementRegExp(QObject *parent = 0);
    virtual ~TST_MeasurementRegExp() Q_DECL_OVERRIDE;

private slots:
    void TestOriginalMeasurementNamesRegExp();
    void TestVariableStrings_data();
    void TestVariableStrings();
    void TestCorrectOrderMeasurement_data();
    void TestCorrectOrderMeasurement();

private:
    Q_DISABLE_COPY(TST_MeasurementRegExp)

    QPointer<QTranslator>   pmsTranslator;
    VTranslateMeasurements *trMs;

    void    PrepareMeasurementData();
    int     LoadTranslation(const QString &checkedSystem, const QString &checkedLocale);
    void    InitTrMs();
    void    CheckRegExpNames() const;
    void    CheckIsNamesUnique() const;
};

#endif // TST_MEASUREMENTREGEXP_H
