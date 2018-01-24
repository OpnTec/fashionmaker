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
 * @param translatedFunctions relations between translated function names and original. Contains only translated unique
 * names.
 */
QmuTokenParser::QmuTokenParser(const QString &formula, bool osSeparator,
                               bool fromUser, const QMap<QString, QString> &translatedFunctions)
    :QmuFormulaBase()
{
    InitCharSets();
    setAllowSubexpressions(false);//Only one expression per time
    SetVarFactory(AddVariable, this);

    SetSepForTr(osSeparator, fromUser);

    // Fix for issue #776. Valentina cannot recognize translated functions.
    QMap<QString, QString>::const_iterator i = translatedFunctions.constBegin();
    while (i != translatedFunctions.constEnd())
    {
        if (i.key() != i.value())
        {
            auto search = m_FunDef.find(i.value());
            if(search != m_FunDef.end())
            {
                AddCallback(i.key(), search->second, m_FunDef, ValidNameChars());
            }
        }
        ++i;
    }

    SetExpr(formula);
    //Need run for making tokens. Don't catch exception here, because we want know if formula has error.
    m_pTokenReader->IgnoreUndefVar(true);
    Eval();
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
    QLocale c(QLocale::C);
    bool ok = false;
    c.toDouble(formula, &ok);
    return ok;
}

}// namespace qmu
