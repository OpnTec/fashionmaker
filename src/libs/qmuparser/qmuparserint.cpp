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

#include "qmuparserint.h"

#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

/** \file
    \brief Implementation of a parser using integer value.
*/

/** \brief Namespace for mathematical applications. */
namespace qmu
{
qreal QmuParserInt::Abs(qreal v)  { return (qreal)Round(fabs((double)v)); }
qreal QmuParserInt::Sign(qreal v) { return (Round(v)<0) ? -1 : (Round(v)>0) ? 1 : 0; }
qreal QmuParserInt::Ite(qreal v1,
                          qreal v2,
                          qreal v3) { return (Round(v1)==1) ? Round(v2) : Round(v3); }
qreal QmuParserInt::Add(qreal v1, qreal v2) { return Round(v1)  + Round(v2); }
qreal QmuParserInt::Sub(qreal v1, qreal v2) { return Round(v1)  - Round(v2); }
qreal QmuParserInt::Mul(qreal v1, qreal v2) { return Round(v1)  * Round(v2); }
qreal QmuParserInt::Div(qreal v1, qreal v2) { return Round(v1)  / Round(v2); }
qreal QmuParserInt::Mod(qreal v1, qreal v2) { return Round(v1)  % Round(v2); }
qreal QmuParserInt::Shr(qreal v1, qreal v2) { return Round(v1) >> Round(v2); }
qreal QmuParserInt::Shl(qreal v1, qreal v2) { return Round(v1) << Round(v2); }
qreal QmuParserInt::LogAnd(qreal v1, qreal v2) { return Round(v1) & Round(v2); }
qreal QmuParserInt::LogOr(qreal v1, qreal v2)  { return Round(v1) | Round(v2); }
qreal QmuParserInt::And(qreal v1, qreal v2) { return Round(v1) && Round(v2); }
qreal QmuParserInt::Or(qreal v1, qreal v2)  { return Round(v1) || Round(v2); }
qreal QmuParserInt::Less(qreal v1, qreal v2)      { return Round(v1)  < Round(v2); }
qreal QmuParserInt::Greater(qreal v1, qreal v2)   { return Round(v1)  > Round(v2); }
qreal QmuParserInt::LessEq(qreal v1, qreal v2)    { return Round(v1) <= Round(v2); }
qreal QmuParserInt::GreaterEq(qreal v1, qreal v2) { return Round(v1) >= Round(v2); }
qreal QmuParserInt::Equal(qreal v1, qreal v2)     { return Round(v1) == Round(v2); }
qreal QmuParserInt::NotEqual(qreal v1, qreal v2)  { return Round(v1) != Round(v2); }
qreal QmuParserInt::Not(qreal v) { return !Round(v); }

qreal QmuParserInt::Pow(qreal v1, qreal v2)
{ 
  return std::pow((double)Round(v1), (double)Round(v2)); 
}

//---------------------------------------------------------------------------
// Unary operator Callbacks: Infix operators
qreal QmuParserInt::UnaryMinus(qreal v)
{ 
  return -Round(v); 
}

//---------------------------------------------------------------------------
qreal QmuParserInt::Sum(const qreal* a_afArg, int a_iArgc)
{ 
  if (!a_iArgc)	
    throw QmuParserError("too few arguments for function sum.");

  qreal fRes=0;
  for (int i=0; i<a_iArgc; ++i) 
    fRes += a_afArg[i];

  return fRes;
}

//---------------------------------------------------------------------------
qreal QmuParserInt::Min(const qreal* a_afArg, int a_iArgc)
{ 
  if (!a_iArgc)	
    throw QmuParserError( "too few arguments for function min." );

  qreal fRes=a_afArg[0];
  for (int i=0; i<a_iArgc; ++i) 
    fRes = std::min(fRes, a_afArg[i]);

  return fRes;
}

//---------------------------------------------------------------------------
qreal QmuParserInt::Max(const qreal* a_afArg, int a_iArgc)
{ 
  if (!a_iArgc)	
    throw QmuParserError("too few arguments for function min.");

  qreal fRes=a_afArg[0];
  for (int i=0; i<a_iArgc; ++i) 
    fRes = std::max(fRes, a_afArg[i]);

  return fRes;
}

//---------------------------------------------------------------------------
// Default value recognition callback
int QmuParserInt::IsVal(const char_type *a_szExpr, int *a_iPos, qreal *a_fVal)
{
  string_type buf(a_szExpr);
  std::size_t pos = buf.find_first_not_of("0123456789");

  if (pos==std::string::npos)
    return 0;

  stringstream_type stream( buf.substr(0, pos ) );
  int iVal(0);

  stream >> iVal;
  if (stream.fail())
    return 0;
      
  stringstream_type::pos_type iEnd = stream.tellg();   // Position after reading
  if (stream.fail())
    iEnd = stream.str().length();  

  if (iEnd==(stringstream_type::pos_type)-1)
    return 0;

  *a_iPos += (int)iEnd;
  *a_fVal = (qreal)iVal;
  return 1;
}

//---------------------------------------------------------------------------
/** \brief Check a given position in the expression for the presence of 
           a hex value. 
    \param a_szExpr Pointer to the expression string
    \param [in/out] a_iPos Pointer to an interger value holding the current parsing 
           position in the expression.
    \param [out] a_fVal Pointer to the position where the detected value shall be stored.

  Hey values must be prefixed with "0x" in order to be detected properly.
*/
int QmuParserInt::IsHexVal(const char_type *a_szExpr, int *a_iPos, qreal *a_fVal)
{
  if (a_szExpr[1]==0 || (a_szExpr[0]!='0' || a_szExpr[1]!='x') ) 
    return 0;

  unsigned iVal(0);

  // New code based on streams for UNICODE compliance:
  stringstream_type::pos_type nPos(0);
  stringstream_type ss(a_szExpr + 2);
  ss >> std::hex >> iVal;
  nPos = ss.tellg();

  if (nPos==(stringstream_type::pos_type)0)
    return 1;

  *a_iPos += (int)(2 + nPos);
  *a_fVal = (qreal)iVal;
  return 1;
}

//---------------------------------------------------------------------------
int QmuParserInt::IsBinVal(const char_type *a_szExpr, int *a_iPos, qreal *a_fVal)
{
  if (a_szExpr[0]!='#') 
    return 0;

  unsigned iVal(0), 
           iBits(sizeof(iVal)*8),
           i(0);

  for (i=0; (a_szExpr[i+1]=='0' || a_szExpr[i+1]=='1') && i<iBits; ++i)
    iVal |= (int)(a_szExpr[i+1]=='1') << ((iBits-1)-i);

  if (i==0) 
    return 0;

  if (i==iBits)
    throw exception_type("Binary to integer conversion error (overflow).");

  *a_fVal = (unsigned)(iVal >> (iBits-i) );
  *a_iPos += i+1;

  return 1;
}

//---------------------------------------------------------------------------
/** \brief Constructor. 

  Call ParserBase class constructor and trigger Function, Operator and Constant initialization.
*/
QmuParserInt::QmuParserInt()
  :QmuParserBase()
{
  AddValIdent(IsVal);    // lowest priority
  AddValIdent(IsBinVal);
  AddValIdent(IsHexVal); // highest priority

  InitCharSets();
  InitFun();
  InitOprt();
}

//---------------------------------------------------------------------------
void QmuParserInt::InitConst()
{
}

//---------------------------------------------------------------------------
void QmuParserInt::InitCharSets()
{
  DefineNameChars( "0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" );
  DefineOprtChars( "+-*^/?<>=!%&|~'_" );
  DefineInfixOprtChars( "/+-*^?<>=!%&|~'_" );
}

//---------------------------------------------------------------------------
/** \brief Initialize the default functions. */
void QmuParserInt::InitFun()
{
  DefineFun( "sign", Sign);
  DefineFun( "abs", Abs);
  DefineFun( "if", Ite);
  DefineFun( "sum", Sum);
  DefineFun( "min", Min);
  DefineFun( "max", Max);
}

//---------------------------------------------------------------------------
/** \brief Initialize operators. */
void QmuParserInt::InitOprt()
{
  // disable all built in operators, not all of them usefull for integer numbers
  // (they don't do rounding of values)
  EnableBuiltInOprt(false);

  // Disable all built in operators, they wont work with integer numbers
  // since they are designed for floating point numbers
  DefineInfixOprt( "-", UnaryMinus);
  DefineInfixOprt( "!", Not);

  DefineOprt( "&", LogAnd, prLOGIC);
  DefineOprt( "|", LogOr, prLOGIC);
  DefineOprt( "&&", And, prLOGIC);
  DefineOprt( "||", Or, prLOGIC);

  DefineOprt( "<", Less, prCMP);
  DefineOprt( ">", Greater, prCMP);
  DefineOprt( "<=", LessEq, prCMP);
  DefineOprt( ">=", GreaterEq, prCMP);
  DefineOprt( "==", Equal, prCMP);
  DefineOprt( "!=", NotEqual, prCMP);

  DefineOprt( "+", Add, prADD_SUB);
  DefineOprt( "-", Sub, prADD_SUB);

  DefineOprt( "*", Mul, prMUL_DIV);
  DefineOprt( "/", Div, prMUL_DIV);
  DefineOprt( "%", Mod, prMUL_DIV);

  DefineOprt( "^", Pow, prPOW, oaRIGHT);
  DefineOprt( ">>", Shr, prMUL_DIV+1);
  DefineOprt( "<<", Shl, prMUL_DIV+1);
}

} // namespace qmu
