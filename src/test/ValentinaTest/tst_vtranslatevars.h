/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef TST_VTRANSLATEVARS_H
#define TST_VTRANSLATEVARS_H

#include <QtCore/qglobal.h>
#include <QLocale>
#include <QObject>

class VTranslateVars;

class TST_VTranslateVars : public QObject
{
    Q_OBJECT
public:
    explicit TST_VTranslateVars(QObject *parent = nullptr);
private slots:
    void initTestCase();
    void TestFormulaFromUser_data();
    void TestFormulaFromUser();
    void TestFormulaToUser_data();
    void TestFormulaToUser();
    void cleanupTestCase();
private:
    Q_DISABLE_COPY(TST_VTranslateVars)
    VTranslateVars *m_trMs;
    QLocale m_systemLocale;

    void PrepareValFromUser(double d, const QLocale &locale);
    void PrepareValToUser(double d, const QLocale &locale);
    void PrepareVal(const QString &inputFormula, const QString &outputFormula, const QLocale &locale);
};

#endif // TST_VTRANSLATEVARS_H
