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
  qreal QmuParser::Sin(qreal v)   { return MathImpl<qreal>::Sin(v);  }
  qreal QmuParser::Cos(qreal v)   { return MathImpl<qreal>::Cos(v);  }
  qreal QmuParser::Tan(qreal v)   { return MathImpl<qreal>::Tan(v);  }
  qreal QmuParser::ASin(qreal v)  { return MathImpl<qreal>::ASin(v); }
  qreal QmuParser::ACos(qreal v)  { return MathImpl<qreal>::ACos(v); }
  qreal QmuParser::ATan(qreal v)  { return MathImpl<qreal>::ATan(v); }
  qreal QmuParser::ATan2(qreal v1, qreal v2) { return MathImpl<qreal>::ATan2(v1, v2); }
  qreal QmuParser::Sinh(qreal v)  { return MathImpl<qreal>::Sinh(v); }
  qreal QmuParser::Cosh(qreal v)  { return MathImpl<qreal>::Cosh(v); }
  qreal QmuParser::Tanh(qreal v)  { return MathImpl<qreal>::Tanh(v); }
  qreal QmuParser::ASinh(qreal v) { return MathImpl<qreal>::ASinh(v); }
  qreal QmuParser::ACosh(qreal v) { return MathImpl<qreal>::ACosh(v); }
  qreal QmuParser::ATanh(qreal v) { return MathImpl<qreal>::ATanh(v); }

  //---------------------------------------------------------------------------
  // Logarithm functions

  // Logarithm base 2
  qreal QmuParser::Log2(qreal v)
  {
    #ifdef MUP_MATH_EXCEPTIONS
        if (v<=0)
          throw QmuParserError(ecDOMAIN_ERROR, "Log2");
    #endif

    return MathImpl<qreal>::Log2(v);
  }

  // Logarithm base 10
  qreal QmuParser::Log10(qreal v)
  {
    #ifdef MUP_MATH_EXCEPTIONS
        if (v<=0)
          throw QmuParserError(ecDOMAIN_ERROR, "Log10");
    #endif

    return MathImpl<qreal>::Log10(v);
  }

// Logarithm base e (natural logarithm)
  qreal QmuParser::Ln(qreal v)
  {
    #ifdef MUP_MATH_EXCEPTIONS
        if (v<=0)
          throw QmuParserError(ecDOMAIN_ERROR, "Ln");
    #endif

    return MathImpl<qreal>::Log(v);
  }

  //---------------------------------------------------------------------------
  //  misc
  qreal QmuParser::Exp(qreal v)  { return MathImpl<qreal>::Exp(v);  }
  qreal QmuParser::Abs(qreal v)  { return MathImpl<qreal>::Abs(v);  }
  qreal QmuParser::Sqrt(qreal v)
  {
    #ifdef MUP_MATH_EXCEPTIONS
        if (v<0)
          throw QmuParserError(ecDOMAIN_ERROR, "sqrt");
    #endif

    return MathImpl<qreal>::Sqrt(v);
  }
  qreal QmuParser::Rint(qreal v) { return MathImpl<qreal>::Rint(v); }
  qreal QmuParser::Sign(qreal v) { return MathImpl<qreal>::Sign(v); }

  //---------------------------------------------------------------------------
  /** \brief Callback for the unary minus operator.
      \param v The value to negate
      \return -v
  */
  qreal QmuParser::UnaryMinus(qreal v)
  {
    return -v;
  }

  //---------------------------------------------------------------------------
  /** \brief Callback for adding multiple values.
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */
  qreal QmuParser::Sum(const qreal *a_afArg, int a_iArgc)
  {
    if (!a_iArgc)
      throw exception_type("too few arguments for function sum.");

    qreal fRes=0;
    for (int i=0; i<a_iArgc; ++i) fRes += a_afArg[i];
    return fRes;
  }

  //---------------------------------------------------------------------------
  /** \brief Callback for averaging multiple values.
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */
  qreal QmuParser::Avg(const qreal *a_afArg, int a_iArgc)
  {
    if (!a_iArgc)
      throw exception_type("too few arguments for function sum.");

    qreal fRes=0;
    for (int i=0; i<a_iArgc; ++i) fRes += a_afArg[i];
    return fRes/(qreal)a_iArgc;
  }


  //---------------------------------------------------------------------------
  /** \brief Callback for determining the minimum value out of a vector.
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */
  qreal QmuParser::Min(const qreal *a_afArg, int a_iArgc)
  {
    if (!a_iArgc)
      throw exception_type("too few arguments for function min.");

    qreal fRes=a_afArg[0];
    for (int i=0; i<a_iArgc; ++i)
      fRes = std::min(fRes, a_afArg[i]);

    return fRes;
  }


  //---------------------------------------------------------------------------
  /** \brief Callback for determining the maximum value out of a vector.
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */
  qreal QmuParser::Max(const qreal *a_afArg, int a_iArgc)
  {
    if (!a_iArgc)
      throw exception_type("too few arguments for function min.");

    qreal fRes=a_afArg[0];
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
  int QmuParser::IsVal(const char_type* a_szExpr, int *a_iPos, qreal *a_fVal)
  {
    qreal fVal(0);

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
    DefineNameChars( "0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    DefineOprtChars( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-*^/?<>=#!$%&|~'_{}" );
    DefineInfixOprtChars( "/+-*^?<>=#!$%&|~'_" );
  }

  //---------------------------------------------------------------------------
  /** \brief Initialize the default functions. */
  void QmuParser::InitFun()
  {
    if (qmu::TypeInfo<qreal>::IsInteger())
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
      DefineFun("sin", Sin);
      DefineFun("cos", Cos);
      DefineFun("tan", Tan);
      // arcus functions
      DefineFun("asin", ASin);
      DefineFun("acos", ACos);
      DefineFun("atan", ATan);
      DefineFun("atan2", ATan2);
      // hyperbolic functions
      DefineFun("sinh", Sinh);
      DefineFun("cosh", Cosh);
      DefineFun("tanh", Tanh);
      // arcus hyperbolic functions
      DefineFun("asinh", ASinh);
      DefineFun("acosh", ACosh);
      DefineFun("atanh", ATanh);
      // Logarithm functions
      DefineFun("log2", Log2);
      DefineFun("log10", Log10);
      DefineFun("log", Log10);
      DefineFun("ln", Ln);
      // misc
      DefineFun("exp", Exp);
      DefineFun("sqrt", Sqrt);
      DefineFun("sign", Sign);
      DefineFun("rint", Rint);
      DefineFun("abs", Abs);
      // Functions with variable number of arguments
      DefineFun("sum", Sum);
      DefineFun("avg", Avg);
      DefineFun("min", Min);
      DefineFun("max", Max);
    }
  }

  //---------------------------------------------------------------------------
  /** \brief Initialize constants.

    By default the QmuParser recognizes two constants. Pi ("pi") and the eulerian
    number ("_e").
  */
  void QmuParser::InitConst()
  {
    DefineConst("_pi", (qreal)QmuParser_CONST_PI);
    DefineConst("_e", (qreal)QmuParser_CONST_E);
  }

  //---------------------------------------------------------------------------
  /** \brief Initialize operators.

    By default only the unary minus operator is added.
  */
  void QmuParser::InitOprt()
  {
    DefineInfixOprt("-", UnaryMinus);
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
  qreal QmuParser::Diff(qreal *a_Var,
                          qreal  a_fPos,
                          qreal  a_fEpsilon) const
  {
    qreal fRes(0),
               fBuf(*a_Var),
               f[4] = {0,0,0,0},
               fEpsilon(a_fEpsilon);

    // Backwards compatible calculation of epsilon inc case the user doesnt provide
    // his own epsilon
    if (fEpsilon==0)
      fEpsilon = (a_fPos==0) ? (qreal)1e-10 : (qreal)1e-7 * a_fPos;

    *a_Var = a_fPos+2 * fEpsilon;  f[0] = Eval();
    *a_Var = a_fPos+1 * fEpsilon;  f[1] = Eval();
    *a_Var = a_fPos-1 * fEpsilon;  f[2] = Eval();
    *a_Var = a_fPos-2 * fEpsilon;  f[3] = Eval();
    *a_Var = fBuf; // restore variable

    fRes = (-f[0] + 8*f[1] - 8*f[2] + f[3]) / (12*fEpsilon);
    return fRes;
  }
} // namespace qmu
