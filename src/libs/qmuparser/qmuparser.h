/***************************************************************************************************
 **
 **  Original work Copyright (C) 2013 Ingo Berg
 **  Modified work Copyright 2014 Roman Telezhynskyi <dismine(at)gmail.com>
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

#ifndef QMUPARSER_H
#define QMUPARSER_H

#include "qmuparser_global.h"
#include "qmuparserbase.h"

/**
 * @file
 * @brief Definition of the standard floating point parser.
 */

namespace qmu
{
    /** @brief Mathematical expressions parser.
    *
    * Standard implementation of the mathematical expressions parser.
    * Can be used as a reference implementation for subclassing the parser.
    *
    * <small>
    * (C) 2011 Ingo Berg<br>
    * muparser(at)gmx.de
    * </small>
    */
    /* final */ class QMUPARSERSHARED_EXPORT QmuParser : public QmuParserBase
    {
    public:
        QmuParser();
        virtual void InitCharSets();
        virtual void InitFun();
        virtual void InitConst();
        virtual void InitOprt();
        virtual void OnDetectVar(const QString &pExpr, int &nStart, int &nEnd);
        qreal        Diff(qreal *a_Var, qreal a_fPos, qreal a_fEpsilon = 0) const;
    protected:
        static int   IsVal(const QString &a_szExpr, int *a_iPos, qreal *a_fVal);
        // Trigonometric functions
        static qreal Tan2(qreal, qreal);
        // hyperbolic functions
        static qreal Sinh(qreal);
        static qreal Cosh(qreal);
        static qreal Tanh(qreal);
        // arcus hyperbolic functions
        static qreal ASinh(qreal);
        static qreal ACosh(qreal);
        static qreal ATanh(qreal);
        // Logarithm functions
        static qreal Log2(qreal);  // Logarithm Base 2
        static qreal Log10(qreal); // Logarithm Base 10
        // misc
        static qreal Abs(qreal);
        static qreal Rint(qreal);
        static qreal Sign(qreal);
        // Prefix operators
        // !!! Unary Minus is a MUST if you want to use negative signs !!!
        static qreal UnaryMinus(qreal v);
        // Functions with variable number of arguments
        static qreal Sum(const qreal*, int);  // sum
        static qreal Avg(const qreal*, int);  // mean value
        static qreal Min(const qreal*, int);  // minimum
        static qreal Max(const qreal*, int);  // maximum
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Callback for the unary minus operator.
 * @param v The value to negate
 * @return -v
 */
inline qreal QmuParser::UnaryMinus(qreal v)
{
    return -v;
}

} // namespace qmu

#endif // QMUPARSER_H
