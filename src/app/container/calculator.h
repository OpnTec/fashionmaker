/************************************************************************
 **
 **  @file   calculator.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "vcontainer.h"

/**
 * @brief The Calculator class calculate formulas of pattern. Support operation +,-,/,* and braces.
 * Can replace name of variables her value.
 */
class Calculator
{
public:
    explicit Calculator(const VContainer *data);
    qreal       eval(QString prog, QString *errorMsg);
private:
    Q_DISABLE_COPY(Calculator)
    /**
     * @brief errorMsg keeps error message of calculation.
     */
    QString     *errorMsg;
    /**
     * @brief token теперішня лексема.
     */
    QString     token;
    /**
     * @brief tok internal representation of token.
     */
    qint32      tok;
    /**
     * @brief token_type type of token.
     */
    qint32      token_type;
    /**
     * @brief prog string where keeps formula.
     */
    QString     prog;
    /**
     * @brief index number character in string of formula.
     */
    qint32      index;
    /**
     * @brief data container with data container of all variables.
     */
    const VContainer *data;
    /**
     * @brief debugFormula decoded string of formula.
     */
    QString     debugFormula;
    qreal       get_exp();
    void        get_token();
    static bool StrChr(QString string, QChar c);
    void        putback();
    void        level2(qreal *result);
    void        level3(qreal *result);
    void        level4(qreal *result);
    void        level5(qreal *result);
    void        level6(qreal *result);
    void        primitive(qreal *result);
    static void arith(QChar o, qreal *r, qreal *h);
    static void unary(QChar o, qreal *r);
    qreal       find_var(QString s);
    // cppcheck-suppress functionStatic
    void        serror(qint32 error);
    static char look_up(QString s);
    static bool isdelim(QChar c);
    static bool iswhite(QChar c);
};

#endif // CALCULATOR_H
