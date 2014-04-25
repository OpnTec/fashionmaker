/***************************************************************************************************
 **
 **  Original work Copyright (C) 2013 Ingo Berg
 **  Modified work Copyright 2014 Roman Telezhinsky <dismine@gmail.com>
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

#ifndef QMUPARSERINT_H
#define QMUPARSERINT_H

#include "qmuparserbase.h"
#include <vector>


/** \file
    \brief Definition of a parser using integer value.
*/


namespace qmu
{

/** \brief Mathematical expressions parser.
  
  This version of the parser handles only integer numbers. It disables the built in operators thus it is 
  slower than muParser. Integer values are stored in the double value_type and converted if needed.
*/
class QmuParserInt : public QmuParserBase
{
private:
    static int  Round(qreal v) { return (int)(v + ((v>=0) ? 0.5 : -0.5) ); };
  
    static qreal  Abs(qreal);
    static qreal  Sign(qreal);
    static qreal  Ite(qreal, qreal, qreal);
    // !! The unary Minus is a MUST, otherwise you cant use negative signs !!
    static qreal  UnaryMinus(qreal);
    // Functions with variable number of arguments
    static qreal  Sum(const qreal* a_afArg, int a_iArgc);  // sum
    static qreal  Min(const qreal* a_afArg, int a_iArgc);  // minimum
    static qreal  Max(const qreal* a_afArg, int a_iArgc);  // maximum
    // binary operator callbacks
    static qreal  Add(qreal v1, qreal v2);
    static qreal  Sub(qreal v1, qreal v2);
    static qreal  Mul(qreal v1, qreal v2);
    static qreal  Div(qreal v1, qreal v2);
    static qreal  Mod(qreal v1, qreal v2);
    static qreal  Pow(qreal v1, qreal v2);
    static qreal  Shr(qreal v1, qreal v2);
    static qreal  Shl(qreal v1, qreal v2);
    static qreal  LogAnd(qreal v1, qreal v2);
    static qreal  LogOr(qreal v1, qreal v2);
    static qreal  And(qreal v1, qreal v2);
    static qreal  Or(qreal v1, qreal v2);
    static qreal  Xor(qreal v1, qreal v2);
    static qreal  Less(qreal v1, qreal v2);
    static qreal  Greater(qreal v1, qreal v2);
    static qreal  LessEq(qreal v1, qreal v2);
    static qreal  GreaterEq(qreal v1, qreal v2);
    static qreal  Equal(qreal v1, qreal v2);
    static qreal  NotEqual(qreal v1, qreal v2);
    static qreal  Not(qreal v1);

    static int IsHexVal(const char_type* a_szExpr, int *a_iPos, qreal *a_iVal);
    static int IsBinVal(const char_type* a_szExpr, int *a_iPos, qreal *a_iVal);
    static int IsVal   (const char_type* a_szExpr, int *a_iPos, qreal *a_iVal);

    /** \brief A facet class used to change decimal and thousands separator. */
    template<class TChar>
    class change_dec_sep : public std::numpunct<TChar>
    {
    public:
      
      explicit change_dec_sep(char_type cDecSep, char_type cThousandsSep = 0, int nGroup = 3)
        :std::numpunct<TChar>()
        ,m_cDecPoint(cDecSep)
        ,m_cThousandsSep(cThousandsSep)
        ,m_nGroup(nGroup)
      {}
      
    protected:
      
      virtual char_type do_decimal_point() const
      {
        return m_cDecPoint;
      }

      virtual char_type do_thousands_sep() const
      {
        return m_cThousandsSep;
      }

      virtual std::string do_grouping() const 
      { 
        return std::string(1, m_nGroup); 
      }

    private:

      int m_nGroup;
      char_type m_cDecPoint;  
      char_type m_cThousandsSep;
    };

public:
    QmuParserInt();

    virtual void InitFun();
    virtual void InitOprt();
    virtual void InitConst();
    virtual void InitCharSets();
};

} // namespace qmu

#endif

