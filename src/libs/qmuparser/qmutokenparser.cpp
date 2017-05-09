/***************************************************************************************************
 **
 **  Copyright (C) 2015 Roman Telezhynskyi
 **
 **  Permission is hereby granted, free of charge, to any person obtaining a copy of this
 **  software and associated documentation files (the "Software"), to deal in the Software
 **  without restriction, including without limitation the rights to use, copy, modify,
 **  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 **  permit persons to whom the Software is furnished to do so, subject to the following conditions:
 **
 **  The above copyright notice and this permission notice shall be included in all copies or
 **  substantial portions of the Software.
 **
 **  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 **  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 **  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 **  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 **  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **
 ******************************************************************************************************/

#include "qmutokenparser.h"

#include <QMap>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>

#include "qmuparsererror.h"

namespace qmu
{

//---------------------------------------------------------------------------------------------------------------------
QmuTokenParser::QmuTokenParser()
{
    InitCharSets();
    setAllowSubexpressions(false);//Only one expression per time
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief QmuTokenParser class constructor. Make easy initialization math parser.
 *
 * This constructor hide initialization variables, operators, character sets.
 * Use this constuctor to get tokens from formula. All formulas must be converted to external look.
 * Example:
 *
 * QmuTokenParser *cal = new QmuTokenParser(formula, osSeparator, false);
 * tokens = cal->GetTokens();
 * numbers = cal->GetNumbers();
 * delete cal;
 *
 * @param formula string with formula.
 * @param fromUser true if we parse formula from user
 */
QmuTokenParser::QmuTokenParser(const QString &formula, bool osSeparator, bool fromUser)
    :QmuFormulaBase()
{
    InitCharSets();
    setAllowSubexpressions(false);//Only one expression per time
    SetVarFactory(AddVariable, this);

    SetSepForTr(osSeparator, fromUser);

    SetExpr(formula);
    //Need run for making tokens. Don't catch exception here, because we want know if formula has error.
    Eval();
}

//---------------------------------------------------------------------------------------------------------------------
QmuTokenParser::~QmuTokenParser()
{
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IsSingle test formula and return true if it contain only one number.
 *
 * Work only with expressions in internal (C) locale.
 * @param formula expression for test
 * @return true if fomula has single number
 */
bool QmuTokenParser::IsSingle(const QString &formula)
{
    if (formula.isEmpty())
    {
        return false;// if don't know say no
    }

    QScopedPointer<QmuTokenParser> cal(new QmuTokenParser());

    // Parser doesn't know any variable on this stage. So, we just use variable factory that for each unknown
    // variable set value to 0.
    cal->SetVarFactory(AddVariable, cal.data());
    cal->SetSepForEval();//Reset separators options

    try
    {
        cal->SetExpr(formula);
        cal->Eval();// We don't need save result, only parse formula
    }
    catch (const qmu::QmuParserError &e)
    {
        Q_UNUSED(e)
        return false;// something wrong with formula, say no
    }

    QMap<int, QString> tokens = cal->GetTokens();// Tokens (variables, measurements)
    const QMap<int, QString> numbers = cal->GetNumbers();// All numbers in expression
    delete cal.take();

    // Remove "-" from tokens list if exist. If don't do that unary minus operation will broken.
    RemoveAll(tokens, QStringLiteral("-"));

    if (tokens.isEmpty() && numbers.size() == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

}// namespace qmu
