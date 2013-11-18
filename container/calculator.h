/************************************************************************
 **
 **  @file   calculator.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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
    /**
     * @brief Calculator class constructor.
     * @param data pointer to a variable container.
     */
    explicit Calculator(const VContainer *data):errorMsg(0), token(QString()), tok(0), token_type(0), prog(QString()),
        index(0), data(data), debugFormula(QString()){}
    /**
     * @brief eval calculate formula.
     * @param prog string of formula.
     * @param errorMsg keep error message.
     * @return value of formula.
     */
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
     * @brief data container of all variables.
     */
    const VContainer *data;
    /**
     * @brief debugFormula decoded string of formula.
     */
    QString     debugFormula;
    /**
     * @brief get_exp calculate formula.
     * @return value of formula.
     */
    qreal       get_exp();
    /**
     * @brief get_token return next token.
     */
    void        get_token();
    /**
     * @brief StrChr checks whether the character belongs to the line.
     * @param string string with formula
     * @param c character.
     * @return true - belongs to the line, false - don't belongs to the line.
     */
    static bool StrChr(QString string, QChar c);
    /**
     * @brief putback returns the readout token back into the flow.
     */
    void        putback();
    /**
     * @brief level2 method of addition and subtraction of two terms.
     * @param result result of operation.
     */
    void        level2(qreal *result);
    /**
     * @brief level3 method of multiplication, division, finding percent.
     * @param result result of operation.
     */
    void        level3(qreal *result);
    /**
     * @brief level4 method of degree two numbers.
     * @param result result of operation.
     */
    void        level4(qreal *result);
    /**
     * @brief level5 method for finding unary plus or minus.
     * @param result result of operation.
     */
    void        level5(qreal *result);
    /**
     * @brief level6 processing method of the expression in brackets.
     * @param result result of operation.
     */
    void        level6(qreal *result);
    /**
     * @brief primitive method of determining the value of a variable by its name.
     * @param result result of operation.
     */
    void        primitive(qreal *result);
    /**
     * @brief arith perform the specified arithmetic. The result is written to the first element.
     * @param o sign of operation.
     * @param r first element.
     * @param h second element.
     */
    static void arith(QChar o, qreal *r, qreal *h);
    /**
     * @brief unary method changes the sign.
     * @param o sign of symbol.
     * @param r element.
     */
    static void unary(QChar o, qreal *r);
    /**
     * @brief find_var method is finding variable by name.
     * @param s name of variable.
     * @return value of variable.
     */
    qreal       find_var(QString s);
    /**
     * @brief serror report an error
     * @param error error code
     */
    void        serror(qint32 error);
    /**
     * @brief look_up finding the internal format for the current token in the token table.
     * @param s name of token.
     * @return internal number of token.
     */
    static char look_up(QString s);
    /**
     * @brief isdelim return true if c delimiter.
     * @param c character.
     * @return true - delimiter, false - do not delimiter.
     */
    static bool isdelim(QChar c);
    /**
     * @brief isdelim return true if c delimiter.
     * @param c character.
     * @return true - delimiter, false - do not delimiter.
     */
    static bool iswhite(QChar c);
    /**
     * @brief iswhite checks whether c space or tab.
     * @param c character.
     * @return true - space or tab, false - don't space and don't tab.
     */
};

#endif // CALCULATOR_H
