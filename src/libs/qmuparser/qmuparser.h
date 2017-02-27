/***************************************************************************************************
 **
 **  Copyright (C) 2013 Ingo Berg
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

#include <qcompilerdetection.h>
#include <QString>
#include <QtGlobal>
#include <locale>

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
        virtual void InitCharSets() Q_DECL_OVERRIDE;
        virtual void InitFun() Q_DECL_OVERRIDE;
        virtual void InitConst() Q_DECL_OVERRIDE;
        virtual void InitOprt() Q_DECL_OVERRIDE;
        virtual void OnDetectVar(const QString &pExpr, int &nStart, int &nEnd) Q_DECL_OVERRIDE;
        qreal        Diff(qreal *a_Var, qreal a_fPos, qreal a_fEpsilon = 0) const;
    protected:
        static int   IsVal(const QString &a_szExpr, int *a_iPos, qreal *a_fVal, const QLocale &locale,
                           const QChar &decimal, const QChar &thousand);
        // hyperbolic functions
        static qreal Sinh(qreal);
        static qreal Cosh(qreal);
        static qreal Tanh(qreal);
        // arcus hyperbolic functions
        static qreal ASinh(qreal);
        static qreal ACosh(qreal);
        static qreal ATanh(qreal);
        // functions working with degrees
        static qreal DegreeToRadian(qreal);
        static qreal RadianToDegree(qreal);
        static qreal SinD(qreal);
        static qreal CosD(qreal);
        static qreal TanD(qreal);
        static qreal ASinD(qreal);
        static qreal ACosD(qreal);
        static qreal ATanD(qreal);
        static qreal SinhD(qreal);
        static qreal CoshD(qreal);
        static qreal TanhD(qreal);
        static qreal ASinhD(qreal);
        static qreal ACoshD(qreal);
        static qreal ATanhD(qreal);

        // Logarithm functions
        static qreal Log2(qreal);  // Logarithm Base 2
        static qreal Log10(qreal); // Logarithm Base 10
        // misc
        static qreal Abs(qreal);
        static qreal Rint(qreal);
        static qreal Sign(qreal);
        static qreal FMod(qreal, qreal);
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
