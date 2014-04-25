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

#include "qmuparser.h"
#include "qmuparsertemplatemagic.h"

//--- Standard includes ------------------------------------------------------------------------
#include <cmath>
#include <algorithm>
#include <numeric>

/** \brief Pi (what else?). */
#define QmuParser_CONST_PI  3.141592653589793238462643

/** \brief The eulerian number. */
#define QmuParser_CONST_E   2.718281828459045235360287

using namespace std;

/** \file
    \brief Implementation of the standard floating point QmuParser.
*/



/** \brief Namespace for mathematical applications. */
namespace qmu
{


  //---------------------------------------------------------------------------
  // Trigonometric function
  value_type QmuParser::Sin(value_type v)   { return MathImpl<value_type>::Sin(v);  }
  value_type QmuParser::Cos(value_type v)   { return MathImpl<value_type>::Cos(v);  }
  value_type QmuParser::Tan(value_type v)   { return MathImpl<value_type>::Tan(v);  }
  value_type QmuParser::ASin(value_type v)  { return MathImpl<value_type>::ASin(v); }
  value_type QmuParser::ACos(value_type v)  { return MathImpl<value_type>::ACos(v); }
  value_type QmuParser::ATan(value_type v)  { return MathImpl<value_type>::ATan(v); }
  value_type QmuParser::ATan2(value_type v1, value_type v2) { return MathImpl<value_type>::ATan2(v1, v2); }
  value_type QmuParser::Sinh(value_type v)  { return MathImpl<value_type>::Sinh(v); }
  value_type QmuParser::Cosh(value_type v)  { return MathImpl<value_type>::Cosh(v); }
  value_type QmuParser::Tanh(value_type v)  { return MathImpl<value_type>::Tanh(v); }
  value_type QmuParser::ASinh(value_type v) { return MathImpl<value_type>::ASinh(v); }
  value_type QmuParser::ACosh(value_type v) { return MathImpl<value_type>::ACosh(v); }
  value_type QmuParser::ATanh(value_type v) { return MathImpl<value_type>::ATanh(v); }

  //---------------------------------------------------------------------------
  // Logarithm functions

  // Logarithm base 2
  value_type QmuParser::Log2(value_type v)
  {
    #ifdef MUP_MATH_EXCEPTIONS
        if (v<=0)
          throw QmuParserError(ecDOMAIN_ERROR, _T("Log2"));
    #endif

    return MathImpl<value_type>::Log2(v);
  }

  // Logarithm base 10
  value_type QmuParser::Log10(value_type v)
  {
    #ifdef MUP_MATH_EXCEPTIONS
        if (v<=0)
          throw QmuParserError(ecDOMAIN_ERROR, _T("Log10"));
    #endif

    return MathImpl<value_type>::Log10(v);
  }

// Logarithm base e (natural logarithm)
  value_type QmuParser::Ln(value_type v)
  {
    #ifdef MUP_MATH_EXCEPTIONS
        if (v<=0)
          throw QmuParserError(ecDOMAIN_ERROR, _T("Ln"));
    #endif

    return MathImpl<value_type>::Log(v);
  }

  //---------------------------------------------------------------------------
  //  misc
  value_type QmuParser::Exp(value_type v)  { return MathImpl<value_type>::Exp(v);  }
  value_type QmuParser::Abs(value_type v)  { return MathImpl<value_type>::Abs(v);  }
  value_type QmuParser::Sqrt(value_type v)
  {
    #ifdef MUP_MATH_EXCEPTIONS
        if (v<0)
          throw QmuParserError(ecDOMAIN_ERROR, _T("sqrt"));
    #endif

    return MathImpl<value_type>::Sqrt(v);
  }
  value_type QmuParser::Rint(value_type v) { return MathImpl<value_type>::Rint(v); }
  value_type QmuParser::Sign(value_type v) { return MathImpl<value_type>::Sign(v); }

  //---------------------------------------------------------------------------
  /** \brief Callback for the unary minus operator.
      \param v The value to negate
      \return -v
  */
  value_type QmuParser::UnaryMinus(value_type v)
  {
    return -v;
  }

  //---------------------------------------------------------------------------
  /** \brief Callback for adding multiple values.
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */
  value_type QmuParser::Sum(const value_type *a_afArg, int a_iArgc)
  {
    if (!a_iArgc)
      throw exception_type(_T("too few arguments for function sum."));

    value_type fRes=0;
    for (int i=0; i<a_iArgc; ++i) fRes += a_afArg[i];
    return fRes;
  }

  //---------------------------------------------------------------------------
  /** \brief Callback for averaging multiple values.
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */
  value_type QmuParser::Avg(const value_type *a_afArg, int a_iArgc)
  {
    if (!a_iArgc)
      throw exception_type(_T("too few arguments for function sum."));

    value_type fRes=0;
    for (int i=0; i<a_iArgc; ++i) fRes += a_afArg[i];
    return fRes/(value_type)a_iArgc;
  }


  //---------------------------------------------------------------------------
  /** \brief Callback for determining the minimum value out of a vector.
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */
  value_type QmuParser::Min(const value_type *a_afArg, int a_iArgc)
  {
    if (!a_iArgc)
      throw exception_type(_T("too few arguments for function min."));

    value_type fRes=a_afArg[0];
    for (int i=0; i<a_iArgc; ++i)
      fRes = std::min(fRes, a_afArg[i]);

    return fRes;
  }


  //---------------------------------------------------------------------------
  /** \brief Callback for determining the maximum value out of a vector.
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */
  value_type QmuParser::Max(const value_type *a_afArg, int a_iArgc)
  {
    if (!a_iArgc)
      throw exception_type(_T("too few arguments for function min."));

    value_type fRes=a_afArg[0];
    for (int i=0; i<a_iArgc; ++i) fRes = std::max(fRes, a_afArg[i]);

    return fRes;
  }


  //---------------------------------------------------------------------------
  /** \brief Default value recognition callback.
      \param [in] a_szExpr Pointer to the expression
      \param [in, out] a_iPos Pointer to an index storing the current position within the expression
      \param [out] a_fVal Pointer where the value should be stored in case one is found.
      \return 1 if a value was found 0 otherwise.
  */
  int QmuParser::IsVal(const char_type* a_szExpr, int *a_iPos, value_type *a_fVal)
  {
    value_type fVal(0);

    stringstream_type stream(a_szExpr);
    stream.seekg(0);        // todo:  check if this really is necessary
    stream.imbue(QmuParser::s_locale);
    stream >> fVal;
    stringstream_type::pos_type iEnd = stream.tellg(); // Position after reading

    if (iEnd==(stringstream_type::pos_type)-1)
      return 0;

    *a_iPos += (int)iEnd;
    *a_fVal = fVal;
    return 1;
  }


  //---------------------------------------------------------------------------
  /** \brief Constructor.

    Call QmuParserBase class constructor and trigger Function, Operator and Constant initialization.
  */
  QmuParser::QmuParser()
    :QmuParserBase()
  {
    AddValIdent(IsVal);

    InitCharSets();
    InitFun();
    InitConst();
    InitOprt();
  }

  //---------------------------------------------------------------------------
  /** \brief Define the character sets.
      \sa DefineNameChars, DefineOprtChars, DefineInfixOprtChars

    This function is used for initializing the default character sets that define
    the characters to be useable in function and variable names and operators.
  */
  void QmuParser::InitCharSets()
  {
    DefineNameChars( _T("0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") );
    DefineOprtChars( _T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-*^/?<>=#!$%&|~'_{}") );
    DefineInfixOprtChars( _T("/+-*^?<>=#!$%&|~'_") );
  }

  //---------------------------------------------------------------------------
  /** \brief Initialize the default functions. */
  void QmuParser::InitFun()
  {
    if (qmu::TypeInfo<qmu::value_type>::IsInteger())
    {
      // When setting MUP_BASETYPE to an integer type
      // Place functions for dealing with integer values here
      // ...
      // ...
      // ...
    }
    else
    {
      // trigonometric functions
      DefineFun(_T("sin"), Sin);
      DefineFun(_T("cos"), Cos);
      DefineFun(_T("tan"), Tan);
      // arcus functions
      DefineFun(_T("asin"), ASin);
      DefineFun(_T("acos"), ACos);
      DefineFun(_T("atan"), ATan);
      DefineFun(_T("atan2"), ATan2);
      // hyperbolic functions
      DefineFun(_T("sinh"), Sinh);
      DefineFun(_T("cosh"), Cosh);
      DefineFun(_T("tanh"), Tanh);
      // arcus hyperbolic functions
      DefineFun(_T("asinh"), ASinh);
      DefineFun(_T("acosh"), ACosh);
      DefineFun(_T("atanh"), ATanh);
      // Logarithm functions
      DefineFun(_T("log2"), Log2);
      DefineFun(_T("log10"), Log10);
      DefineFun(_T("log"), Log10);
      DefineFun(_T("ln"), Ln);
      // misc
      DefineFun(_T("exp"), Exp);
      DefineFun(_T("sqrt"), Sqrt);
      DefineFun(_T("sign"), Sign);
      DefineFun(_T("rint"), Rint);
      DefineFun(_T("abs"), Abs);
      // Functions with variable number of arguments
      DefineFun(_T("sum"), Sum);
      DefineFun(_T("avg"), Avg);
      DefineFun(_T("min"), Min);
      DefineFun(_T("max"), Max);
    }
  }

  //---------------------------------------------------------------------------
  /** \brief Initialize constants.

    By default the QmuParser recognizes two constants. Pi ("pi") and the eulerian
    number ("_e").
  */
  void QmuParser::InitConst()
  {
    DefineConst(_T("_pi"), (value_type)QmuParser_CONST_PI);
    DefineConst(_T("_e"), (value_type)QmuParser_CONST_E);
  }

  //---------------------------------------------------------------------------
  /** \brief Initialize operators.

    By default only the unary minus operator is added.
  */
  void QmuParser::InitOprt()
  {
    DefineInfixOprt(_T("-"), UnaryMinus);
  }

  //---------------------------------------------------------------------------
  void QmuParser::OnDetectVar(string_type * /*pExpr*/, int & /*nStart*/, int & /*nEnd*/)
  {
    // this is just sample code to illustrate modifying variable names on the fly.
    // I'm not sure anyone really needs such a feature...
    /*


    string sVar(pExpr->begin()+nStart, pExpr->begin()+nEnd);
    string sRepl = std::string("_") + sVar + "_";

    int nOrigVarEnd = nEnd;
    cout << "variable detected!\n";
    cout << "  Expr: " << *pExpr << "\n";
    cout << "  Start: " << nStart << "\n";
    cout << "  End: " << nEnd << "\n";
    cout << "  Var: \"" << sVar << "\"\n";
    cout << "  Repl: \"" << sRepl << "\"\n";
    nEnd = nStart + sRepl.length();
    cout << "  End: " << nEnd << "\n";
    pExpr->replace(pExpr->begin()+nStart, pExpr->begin()+nOrigVarEnd, sRepl);
    cout << "  New expr: " << *pExpr << "\n";
    */
  }

  //---------------------------------------------------------------------------
  /** \brief Numerically differentiate with regard to a variable.
      \param [in] a_Var Pointer to the differentiation variable.
      \param [in] a_fPos Position at which the differentiation should take place.
      \param [in] a_fEpsilon Epsilon used for the numerical differentiation.

    Numerical differentiation uses a 5 point operator yielding a 4th order
    formula. The default value for epsilon is 0.00074 which is
    numeric_limits<double>::epsilon() ^ (1/5) as suggested in the muQmuParser
    forum:

    http://sourceforge.net/forum/forum.php?thread_id=1994611&forum_id=462843
  */
  value_type QmuParser::Diff(value_type *a_Var,
                          value_type  a_fPos,
                          value_type  a_fEpsilon) const
  {
    value_type fRes(0),
               fBuf(*a_Var),
               f[4] = {0,0,0,0},
               fEpsilon(a_fEpsilon);

    // Backwards compatible calculation of epsilon inc case the user doesnt provide
    // his own epsilon
    if (fEpsilon==0)
      fEpsilon = (a_fPos==0) ? (value_type)1e-10 : (value_type)1e-7 * a_fPos;

    *a_Var = a_fPos+2 * fEpsilon;  f[0] = Eval();
    *a_Var = a_fPos+1 * fEpsilon;  f[1] = Eval();
    *a_Var = a_fPos-1 * fEpsilon;  f[2] = Eval();
    *a_Var = a_fPos-2 * fEpsilon;  f[3] = Eval();
    *a_Var = fBuf; // restore variable

    fRes = (-f[0] + 8*f[1] - 8*f[2] + f[3]) / (12*fEpsilon);
    return fRes;
  }
} // namespace qmu
