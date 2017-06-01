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

#ifndef TST_ABSTRACTREGEXP_H
#define TST_ABSTRACTREGEXP_H

#include "../vtest/abstracttest.h"

#include <QPointer>

class QTranslator;
class VTranslateVars;

class TST_AbstractRegExp : public AbstractTest
{
    Q_OBJECT
public:
    TST_AbstractRegExp(const QString &locale, QObject *parent = nullptr);
    virtual ~TST_AbstractRegExp();

protected:
    QString m_locale;
    QPointer<QTranslator> m_vTranslator;
    VTranslateVars *m_trMs;

    virtual void        PrepareData()=0;
    virtual QStringList AllNames()=0;

    int  LoadVariables(const QString &checkedLocale);
    void RemoveTrVariables(const QString &checkedLocale);
    void InitTrMs();

    void CallTestCheckNoEndLine();
    void CallTestCheckRegExpNames();
    void CallTestCheckIsNamesUnique();
    void CallTestCheckNoOriginalNamesInTranslation();

private:
    Q_DISABLE_COPY(TST_AbstractRegExp)
};

#endif // TST_ABSTRACTREGEXP_H
