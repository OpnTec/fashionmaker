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

#ifndef TST_BUITINREGEXP_H
#define TST_BUITINREGEXP_H

#include "tst_abstractregexp.h"

#include <QPointer>

class QTranslator;
class VTranslateVars;

class TST_BuitInRegExp : public TST_AbstractRegExp
{
    Q_OBJECT
public:
    explicit TST_BuitInRegExp(const QString &locale, QObject *parent = nullptr);
    virtual ~TST_BuitInRegExp() Q_DECL_EQ_DEFAULT;

protected:
    virtual void        PrepareData() Q_DECL_OVERRIDE;
    virtual QStringList AllNames() Q_DECL_OVERRIDE;

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
    void TestCheckUnderlineExists_data();
    void TestCheckUnderlineExists();
    void TestCheckInternalVaribleRegExp_data();
    void TestCheckInternalVaribleRegExp();
    void TestTemplatePlaceholders();
    void cleanupTestCase();

private:
    Q_DISABLE_COPY(TST_BuitInRegExp)
};

#endif // TST_BUITINREGEXP_H
