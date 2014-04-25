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
value_type QmuParserInt::Abs(value_type v)  { return (value_type)Round(fabs((double)v)); }
value_type QmuParserInt::Sign(value_type v) { return (Round(v)<0) ? -1 : (Round(v)>0) ? 1 : 0; }
value_type QmuParserInt::Ite(value_type v1,
                          value_type v2, 
                          value_type v3) { return (Round(v1)==1) ? Round(v2) : Round(v3); }
value_type QmuParserInt::Add(value_type v1, value_type v2) { return Round(v1)  + Round(v2); }
value_type QmuParserInt::Sub(value_type v1, value_type v2) { return Round(v1)  - Round(v2); }
value_type QmuParserInt::Mul(value_type v1, value_type v2) { return Round(v1)  * Round(v2); }
value_type QmuParserInt::Div(value_type v1, value_type v2) { return Round(v1)  / Round(v2); }
value_type QmuParserInt::Mod(value_type v1, value_type v2) { return Round(v1)  % Round(v2); }
value_type QmuParserInt::Shr(value_type v1, value_type v2) { return Round(v1) >> Round(v2); }
value_type QmuParserInt::Shl(value_type v1, value_type v2) { return Round(v1) << Round(v2); }
value_type QmuParserInt::LogAnd(value_type v1, value_type v2) { return Round(v1) & Round(v2); }
value_type QmuParserInt::LogOr(value_type v1, value_type v2)  { return Round(v1) | Round(v2); }
value_type QmuParserInt::And(value_type v1, value_type v2) { return Round(v1) && Round(v2); }
value_type QmuParserInt::Or(value_type v1, value_type v2)  { return Round(v1) || Round(v2); }
value_type QmuParserInt::Less(value_type v1, value_type v2)      { return Round(v1)  < Round(v2); }
value_type QmuParserInt::Greater(value_type v1, value_type v2)   { return Round(v1)  > Round(v2); }
value_type QmuParserInt::LessEq(value_type v1, value_type v2)    { return Round(v1) <= Round(v2); }
value_type QmuParserInt::GreaterEq(value_type v1, value_type v2) { return Round(v1) >= Round(v2); }
value_type QmuParserInt::Equal(value_type v1, value_type v2)     { return Round(v1) == Round(v2); }
value_type QmuParserInt::NotEqual(value_type v1, value_type v2)  { return Round(v1) != Round(v2); }
value_type QmuParserInt::Not(value_type v) { return !Round(v); }

value_type QmuParserInt::Pow(value_type v1, value_type v2)
{ 
  return std::pow((double)Round(v1), (double)Round(v2)); 
}

//---------------------------------------------------------------------------
// Unary operator Callbacks: Infix operators
value_type QmuParserInt::UnaryMinus(value_type v)
{ 
  return -Round(v); 
}

//---------------------------------------------------------------------------
value_type QmuParserInt::Sum(const value_type* a_afArg, int a_iArgc)
{ 
  if (!a_iArgc)	
    throw QmuParserError(_T("too few arguments for function sum."));

  value_type fRes=0;
  for (int i=0; i<a_iArgc; ++i) 
    fRes += a_afArg[i];

  return fRes;
}

//---------------------------------------------------------------------------
value_type QmuParserInt::Min(const value_type* a_afArg, int a_iArgc)
{ 
  if (!a_iArgc)	
    throw QmuParserError( _T("too few arguments for function min.") );

  value_type fRes=a_afArg[0];
  for (int i=0; i<a_iArgc; ++i) 
    fRes = std::min(fRes, a_afArg[i]);

  return fRes;
}

//---------------------------------------------------------------------------
value_type QmuParserInt::Max(const value_type* a_afArg, int a_iArgc)
{ 
  if (!a_iArgc)	
    throw QmuParserError(_T("too few arguments for function min."));

  value_type fRes=a_afArg[0];
  for (int i=0; i<a_iArgc; ++i) 
    fRes = std::max(fRes, a_afArg[i]);

  return fRes;
}

//---------------------------------------------------------------------------
// Default value recognition callback
int QmuParserInt::IsVal(const char_type *a_szExpr, int *a_iPos, value_type *a_fVal)
{
  string_type buf(a_szExpr);
  std::size_t pos = buf.find_first_not_of(_T("0123456789"));

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
  *a_fVal = (value_type)iVal;
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
int QmuParserInt::IsHexVal(const char_type *a_szExpr, int *a_iPos, value_type *a_fVal)
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
  *a_fVal = (value_type)iVal;
  return 1;
}

//---------------------------------------------------------------------------
int QmuParserInt::IsBinVal(const char_type *a_szExpr, int *a_iPos, value_type *a_fVal)
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
    throw exception_type(_T("Binary to integer conversion error (overflow)."));

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
  DefineNameChars( _T("0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") );
  DefineOprtChars( _T("+-*^/?<>=!%&|~'_") );
  DefineInfixOprtChars( _T("/+-*^?<>=!%&|~'_") );
}

//---------------------------------------------------------------------------
/** \brief Initialize the default functions. */
void QmuParserInt::InitFun()
{
  DefineFun( _T("sign"), Sign);
  DefineFun( _T("abs"), Abs);
  DefineFun( _T("if"), Ite);
  DefineFun( _T("sum"), Sum);
  DefineFun( _T("min"), Min);
  DefineFun( _T("max"), Max);
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
  DefineInfixOprt( _T("-"), UnaryMinus);
  DefineInfixOprt( _T("!"), Not);

  DefineOprt( _T("&"), LogAnd, prLOGIC);
  DefineOprt( _T("|"), LogOr, prLOGIC);
  DefineOprt( _T("&&"), And, prLOGIC);
  DefineOprt( _T("||"), Or, prLOGIC);

  DefineOprt( _T("<"), Less, prCMP);
  DefineOprt( _T(">"), Greater, prCMP);
  DefineOprt( _T("<="), LessEq, prCMP);
  DefineOprt( _T(">="), GreaterEq, prCMP);
  DefineOprt( _T("=="), Equal, prCMP);
  DefineOprt( _T("!="), NotEqual, prCMP);

  DefineOprt( _T("+"), Add, prADD_SUB);
  DefineOprt( _T("-"), Sub, prADD_SUB);

  DefineOprt( _T("*"), Mul, prMUL_DIV);
  DefineOprt( _T("/"), Div, prMUL_DIV);
  DefineOprt( _T("%"), Mod, prMUL_DIV);

  DefineOprt( _T("^"), Pow, prPOW, oaRIGHT);
  DefineOprt( _T(">>"), Shr, prMUL_DIV+1);
  DefineOprt( _T("<<"), Shl, prMUL_DIV+1);
}

} // namespace qmu
