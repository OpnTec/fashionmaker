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

#include "qmuparsertest.h"

#include <cstdio>
#include <cmath>
#include <iostream>
#include <limits>

#define PARSER_CONST_PI  3.141592653589793238462643
#define PARSER_CONST_E   2.718281828459045235360287

using namespace std;

/** \file
    \brief This file contains the implementation of parser test cases.
*/

namespace qmu
{
  namespace Test
  {
    int QmuParserTester::c_iCount = 0;

    //---------------------------------------------------------------------------------------------
    QmuParserTester::QmuParserTester()
      :m_vTestFun()
    {
      AddTest(&QmuParserTester::TestNames);
      AddTest(&QmuParserTester::TestSyntax);
      AddTest(&QmuParserTester::TestPostFix);
      AddTest(&QmuParserTester::TestInfixOprt);
      AddTest(&QmuParserTester::TestVarConst);
      AddTest(&QmuParserTester::TestMultiArg);
      AddTest(&QmuParserTester::TestExpression);
      AddTest(&QmuParserTester::TestIfThenElse);
      AddTest(&QmuParserTester::TestInterface);
      AddTest(&QmuParserTester::TestBinOprt);
      AddTest(&QmuParserTester::TestException);
      AddTest(&QmuParserTester::TestStrArg);

      QmuParserTester::c_iCount = 0;
    }

    //---------------------------------------------------------------------------------------------
    int QmuParserTester::IsHexVal(const char_type *a_szExpr, int *a_iPos, qreal *a_fVal)
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

    //---------------------------------------------------------------------------------------------
    int QmuParserTester::TestInterface()
    {
      int iStat = 0;
      qmu::console() << "testing member functions...";
   
      // Test RemoveVar
      qreal afVal[3] = {1,2,3};
      QmuParser p;
  
      try
      {
        p.DefineVar( "a", &afVal[0]);
        p.DefineVar( "b", &afVal[1]);
        p.DefineVar( "c", &afVal[2]);
        p.SetExpr( "a+b+c" );
        p.Eval();
      }
      catch(...)
      {
        iStat += 1;  // this is not supposed to happen 
      }

      try
      {
        p.RemoveVar( "c" );
        p.Eval();
        iStat += 1;  // not supposed to reach this, nonexisting variable "c" deleted...
      }
      catch(...)
      {
        // failure is expected...
      }

      if (iStat==0) 
        qmu::console() << "passed" << endl;
      else 
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }

    //---------------------------------------------------------------------------------------------
    int QmuParserTester::TestStrArg()
    {
      int iStat = 0;
      qmu::console() << "testing string arguments...";
 
      iStat += EqnTest("valueof(\"\")", 123, true);   // empty string arguments caused a crash
      iStat += EqnTest("valueof(\"aaa\")+valueof(\"bbb\")  ", 246, true);
      iStat += EqnTest("2*(valueof(\"aaa\")-23)+valueof(\"bbb\")", 323, true);
      // use in expressions with variables
      iStat += EqnTest("a*(atof(\"10\")-b)", 8, true);
      iStat += EqnTest("a-(atof(\"10\")*b)", -19, true);
      // string + numeric arguments
      iStat += EqnTest("strfun1(\"100\")", 100, true);
      iStat += EqnTest("strfun2(\"100\",1)", 101, true);
      iStat += EqnTest("strfun3(\"99\",1,2)", 102, true);

      if (iStat==0)
        qmu::console() << "passed" << endl;
      else 
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }

    //---------------------------------------------------------------------------------------------
    int QmuParserTester::TestBinOprt()
    {
      int iStat = 0;
      qmu::console() << "testing binary operators...";
   
      // built in operators
      // xor operator
      //iStat += EqnTest("1 xor 2", 3, true);
      //iStat += EqnTest("a xor b", 3, true);            // with a=1 and b=2
      //iStat += EqnTest("1 xor 2 xor 3", 0, true);
      //iStat += EqnTest("a xor b xor 3", 0, true);      // with a=1 and b=2
      //iStat += EqnTest("a xor b xor c", 0, true);      // with a=1 and b=2
      //iStat += EqnTest("(1 xor 2) xor 3", 0, true);
      //iStat += EqnTest("(a xor b) xor c", 0, true);    // with a=1 and b=2
      //iStat += EqnTest("(a) xor (b) xor c", 0, true);  // with a=1 and b=2
      //iStat += EqnTest("1 or 2"), 3, true;
      //iStat += EqnTest("a or b"), 3, true;             // with a=1 and b=2
      iStat += EqnTest("a++b", 3, true);
      iStat += EqnTest("a ++ b", 3, true);
      iStat += EqnTest("1++2", 3, true);
      iStat += EqnTest("1 ++ 2", 3, true);
      iStat += EqnTest("a add b", 3, true);
      iStat += EqnTest("1 add 2", 3, true);
      iStat += EqnTest("a<b", 1, true);
      iStat += EqnTest("b>a", 1, true);
      iStat += EqnTest("a>a", 0, true);
      iStat += EqnTest("a<a", 0, true);
      iStat += EqnTest("a>a", 0, true);
      iStat += EqnTest("a<=a", 1, true);
      iStat += EqnTest("a<=b", 1, true);
      iStat += EqnTest("b<=a", 0, true);
      iStat += EqnTest("a>=a", 1, true);
      iStat += EqnTest("b>=a", 1, true);
      iStat += EqnTest("a>=b", 0, true);

      // Test logical operators, expecially if user defined "&" and the internal "&&" collide
      iStat += EqnTest("1 && 1", 1, true);
      iStat += EqnTest("1 && 0", 0, true);
      iStat += EqnTest("(a<b) && (b>a)", 1, true);
      iStat += EqnTest("(a<b) && (a>b)", 0, true);
      //iStat += EqnTest("12 and 255", 12, true);
      //iStat += EqnTest("12 and 0", 0, true);
      iStat += EqnTest("12 & 255", 12, true);
      iStat += EqnTest("12 & 0", 0, true);
      iStat += EqnTest("12&255", 12, true);
      iStat += EqnTest("12&0", 0, true);

      // Assignement operator
      iStat += EqnTest("a = b", 2, true);
      iStat += EqnTest("a = sin(b)", 0.909297, true);
      iStat += EqnTest("a = 1+sin(b)", 1.909297, true);
      iStat += EqnTest("(a=b)*2", 4, true);
      iStat += EqnTest("2*(a=b)", 4, true);
      iStat += EqnTest("2*(a=b+1)", 6, true);
      iStat += EqnTest("(a=b+1)*2", 6, true);

      iStat += EqnTest("2^2^3", 256, true);
      iStat += EqnTest("1/2/3", 1.0/6.0, true);

      // reference: http://www.wolframalpha.com/input/?i=3%2B4*2%2F%281-5%29^2^3
      iStat += EqnTest("3+4*2/(1-5)^2^3", 3.0001220703125, true);

      // Test user defined binary operators
      iStat += EqnTestInt("1 | 2", 3, true);
      iStat += EqnTestInt("1 || 2", 1, true);
      iStat += EqnTestInt("123 & 456", 72, true);
      iStat += EqnTestInt("(123 & 456) % 10", 2, true);
      iStat += EqnTestInt("1 && 0", 0, true);
      iStat += EqnTestInt("123 && 456", 1, true);
      iStat += EqnTestInt("1 << 3", 8, true);
      iStat += EqnTestInt("8 >> 3", 1, true);
      iStat += EqnTestInt("9 / 4", 2, true);
      iStat += EqnTestInt("9 % 4", 1, true);
      iStat += EqnTestInt("if(5%2,1,0)", 1, true);
      iStat += EqnTestInt("if(4%2,1,0)", 0, true);
      iStat += EqnTestInt("-10+1", -9, true);
      iStat += EqnTestInt("1+2*3", 7, true);
      iStat += EqnTestInt("const1 != const2", 1, true);
      iStat += EqnTestInt("const1 != const2", 0, false);
      iStat += EqnTestInt("const1 == const2", 0, true);
      iStat += EqnTestInt("const1 == 1", 1, true);
      iStat += EqnTestInt("10*(const1 == 1)", 10, true);
      iStat += EqnTestInt("2*(const1 | const2)", 6, true);
      iStat += EqnTestInt("2*(const1 | const2)", 7, false);
      iStat += EqnTestInt("const1 < const2", 1, true);
      iStat += EqnTestInt("const2 > const1", 1, true);
      iStat += EqnTestInt("const1 <= 1", 1, true);
      iStat += EqnTestInt("const2 >= 2", 1, true);
      iStat += EqnTestInt("2*(const1 + const2)", 6, true);
      iStat += EqnTestInt("2*(const1 - const2)", -2, true);
      iStat += EqnTestInt("a != b", 1, true);
      iStat += EqnTestInt("a != b", 0, false);
      iStat += EqnTestInt("a == b", 0, true);
      iStat += EqnTestInt("a == 1", 1, true);
      iStat += EqnTestInt("10*(a == 1)", 10, true);
      iStat += EqnTestInt("2*(a | b)", 6, true);
      iStat += EqnTestInt("2*(a | b)", 7, false);
      iStat += EqnTestInt("a < b", 1, true);
      iStat += EqnTestInt("b > a", 1, true);
      iStat += EqnTestInt("a <= 1", 1, true);
      iStat += EqnTestInt("b >= 2", 1, true);
      iStat += EqnTestInt("2*(a + b)", 6, true);
      iStat += EqnTestInt("2*(a - b)", -2, true);
      iStat += EqnTestInt("a + (a << b)", 5, true);
      iStat += EqnTestInt("-2^2", -4, true);
      iStat += EqnTestInt("3--a", 4, true);
      iStat += EqnTestInt("3+-3^2", -6, true);

      // Test reading of hex values:
      iStat += EqnTestInt("0xff", 255, true);
      iStat += EqnTestInt("10+0xff", 265, true);
      iStat += EqnTestInt("0xff+10", 265, true);
      iStat += EqnTestInt("10*0xff", 2550, true);
      iStat += EqnTestInt("0xff*10", 2550, true);
      iStat += EqnTestInt("10+0xff+1", 266, true);
      iStat += EqnTestInt("1+0xff+10", 266, true);

// incorrect: '^' is your here, not power
//    iStat += EqnTestInt("-(1+2)^2", -9, true);
//    iStat += EqnTestInt("-1^3", -1, true);          

      // Test precedence
      // a=1, b=2, c=3
      iStat += EqnTestInt("a + b * c", 7, true);
      iStat += EqnTestInt("a * b + c", 5, true);
      iStat += EqnTestInt("a<b && b>10", 0, true);
      iStat += EqnTestInt("a<b && b<10", 1, true);

      iStat += EqnTestInt("a + b << c", 17, true);
      iStat += EqnTestInt("a << b + c", 7, true);
      iStat += EqnTestInt("c * b < a", 0, true);
      iStat += EqnTestInt("c * b == 6 * a", 1, true);
      iStat += EqnTestInt("2^2^3", 256, true);


      if (iStat==0)
        qmu::console() << "passed" << endl;
      else 
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }

    //---------------------------------------------------------------------------------------------
    /** \brief Check muParser name restriction enforcement. */
    int QmuParserTester::TestNames()
    {
      int  iStat= 0,
           iErr = 0;

      qmu::console() << "testing name restriction enforcement...";
    
      QmuParser p;

  #define PARSER_THROWCHECK(DOMAIN, FAIL, EXPR, ARG) \
      iErr = 0;                                      \
      QmuParserTester::c_iCount++;                      \
      try                                            \
      {                                              \
        p.Define##DOMAIN(EXPR, ARG);                 \
      }                                              \
      catch(QmuParser::exception_type&)                 \
      {                                              \
        iErr = (FAIL==false) ? 0 : 1;                \
      }                                              \
      iStat += iErr;      
      
      // constant names
      PARSER_THROWCHECK(Const, false, "0a", 1)
      PARSER_THROWCHECK(Const, false, "9a", 1)
      PARSER_THROWCHECK(Const, false, "+a", 1)
      PARSER_THROWCHECK(Const, false, "-a", 1)
      PARSER_THROWCHECK(Const, false, "a-", 1)
      PARSER_THROWCHECK(Const, false, "a*", 1)
      PARSER_THROWCHECK(Const, false, "a?", 1)
      PARSER_THROWCHECK(Const, true, "a", 1)
      PARSER_THROWCHECK(Const, true, "a_min", 1)
      PARSER_THROWCHECK(Const, true, "a_min0", 1)
      PARSER_THROWCHECK(Const, true, "a_min9", 1)
      // variable names
      qreal a;
      p.ClearConst();
      PARSER_THROWCHECK(Var, false, "123abc", &a)
      PARSER_THROWCHECK(Var, false, "9a", &a)
      PARSER_THROWCHECK(Var, false, "0a", &a)
      PARSER_THROWCHECK(Var, false, "+a", &a)
      PARSER_THROWCHECK(Var, false, "-a", &a)
      PARSER_THROWCHECK(Var, false, "?a", &a)
      PARSER_THROWCHECK(Var, false, "!a", &a)
      PARSER_THROWCHECK(Var, false, "a+", &a)
      PARSER_THROWCHECK(Var, false, "a-", &a)
      PARSER_THROWCHECK(Var, false, "a*", &a)
      PARSER_THROWCHECK(Var, false, "a?", &a)
      PARSER_THROWCHECK(Var, true,  "a", &a)
      PARSER_THROWCHECK(Var, true,  "a_min", &a)
      PARSER_THROWCHECK(Var, true,  "a_min0", &a)
      PARSER_THROWCHECK(Var, true,  "a_min9", &a)
      PARSER_THROWCHECK(Var, false, "a_min9", 0)
      // Postfix operators
      // fail
      PARSER_THROWCHECK(PostfixOprt, false, "(k", f1of1)
      PARSER_THROWCHECK(PostfixOprt, false, "9+", f1of1)
      PARSER_THROWCHECK(PostfixOprt, false, "+", 0)
      // pass
      PARSER_THROWCHECK(PostfixOprt, true, "-a",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "?a",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "_",   f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "#",   f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "&&",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "||",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "&",   f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "|",   f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "++",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "--",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "?>",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "?<",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "**",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "xor", f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "and", f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "or",  f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "not", f1of1)
      PARSER_THROWCHECK(PostfixOprt, true, "!",   f1of1)
      // Binary operator
      // The following must fail with builtin operators activated
      // p.EnableBuiltInOp(true); -> this is the default
      p.ClearPostfixOprt();
      PARSER_THROWCHECK(Oprt, false, "+",  f1of2)
      PARSER_THROWCHECK(Oprt, false, "-",  f1of2)
      PARSER_THROWCHECK(Oprt, false, "*",  f1of2)
      PARSER_THROWCHECK(Oprt, false, "/",  f1of2)
      PARSER_THROWCHECK(Oprt, false, "^",  f1of2)
      PARSER_THROWCHECK(Oprt, false, "&&",  f1of2)
      PARSER_THROWCHECK(Oprt, false, "||",  f1of2)
      // without activated built in operators it should work
      p.EnableBuiltInOprt(false);
      PARSER_THROWCHECK(Oprt, true, "+",  f1of2)
      PARSER_THROWCHECK(Oprt, true, "-",  f1of2)
      PARSER_THROWCHECK(Oprt, true, "*",  f1of2)
      PARSER_THROWCHECK(Oprt, true, "/",  f1of2)
      PARSER_THROWCHECK(Oprt, true, "^",  f1of2)
      PARSER_THROWCHECK(Oprt, true, "&&",  f1of2)
      PARSER_THROWCHECK(Oprt, true, "||",  f1of2)
  #undef PARSER_THROWCHECK

      if (iStat==0) 
        qmu::console() << "passed" << endl;
      else 
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }

    //---------------------------------------------------------------------------
    int QmuParserTester::TestSyntax()
    {
      int iStat = 0;
      qmu::console() << "testing syntax engine...";

      iStat += ThrowTest("1,", ecUNEXPECTED_EOF);  // incomplete hex definition
      iStat += ThrowTest("a,", ecUNEXPECTED_EOF);  // incomplete hex definition
      iStat += ThrowTest("sin(8),", ecUNEXPECTED_EOF);  // incomplete hex definition
      iStat += ThrowTest("(sin(8)),", ecUNEXPECTED_EOF);  // incomplete hex definition
      iStat += ThrowTest("a{m},", ecUNEXPECTED_EOF);  // incomplete hex definition

      iStat += EqnTest("(1+ 2*a)", 3, true);   // Spaces within formula
      iStat += EqnTest("sqrt((4))", 2, true);  // Multiple brackets
      iStat += EqnTest("sqrt((2)+2)", 2, true);// Multiple brackets
      iStat += EqnTest("sqrt(2+(2))", 2, true);// Multiple brackets
      iStat += EqnTest("sqrt(a+(3))", 2, true);// Multiple brackets
      iStat += EqnTest("sqrt((3)+a)", 2, true);// Multiple brackets
      iStat += EqnTest("order(1,2)", 1, true); // May not cause name collision with operator "or"
      iStat += EqnTest("(2+", 0, false);       // missing closing bracket
      iStat += EqnTest("2++4", 0, false);      // unexpected operator
      iStat += EqnTest("2+-4", 0, false);      // unexpected operator
      iStat += EqnTest("(2+)", 0, false);      // unexpected closing bracket
      iStat += EqnTest("--2", 0, false);       // double sign
      iStat += EqnTest("ksdfj", 0, false);     // unknown token
      iStat += EqnTest("()", 0, false);        // empty bracket without a function
      iStat += EqnTest("5+()", 0, false);      // empty bracket without a function
      iStat += EqnTest("sin(cos)", 0, false);  // unexpected function
      iStat += EqnTest("5t6", 0, false);       // unknown token
      iStat += EqnTest("5 t 6", 0, false);     // unknown token
      iStat += EqnTest("8*", 0, false);        // unexpected end of formula
      iStat += EqnTest(",3", 0, false);        // unexpected comma
      iStat += EqnTest("3,5", 0, false);       // unexpected comma
      iStat += EqnTest("sin(8,8)", 0, false);  // too many function args
      iStat += EqnTest("(7,8)", 0, false);     // too many function args
      iStat += EqnTest("sin)", 0, false);      // unexpected closing bracket
      iStat += EqnTest("a)", 0, false);        // unexpected closing bracket
      iStat += EqnTest("pi)", 0, false);       // unexpected closing bracket
      iStat += EqnTest("sin(())", 0, false);   // unexpected closing bracket
      iStat += EqnTest("sin()", 0, false);     // unexpected closing bracket

      if (iStat==0)
        qmu::console() << "passed" << endl;
      else 
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }

    //---------------------------------------------------------------------------
    int QmuParserTester::TestVarConst()
    {
      int iStat = 0;
      qmu::console() << "testing variable/constant detection...";

      // Test if the result changes when a variable changes
      iStat += EqnTestWithVarChange( "a", 1, 1, 2, 2 );
      iStat += EqnTestWithVarChange( "2*a", 2, 4, 3, 6 );

      // distinguish constants with same basename
      iStat += EqnTest( "const", 1, true);
      iStat += EqnTest( "const1", 2, true);
      iStat += EqnTest( "const2", 3, true);
      iStat += EqnTest( "2*const", 2, true);
      iStat += EqnTest( "2*const1", 4, true);
      iStat += EqnTest( "2*const2", 6, true);
      iStat += EqnTest( "2*const+1", 3, true);
      iStat += EqnTest( "2*const1+1", 5, true);
      iStat += EqnTest( "2*const2+1", 7, true);
      iStat += EqnTest( "const", 0, false);
      iStat += EqnTest( "const1", 0, false);
      iStat += EqnTest( "const2", 0, false);

      // distinguish variables with same basename
      iStat += EqnTest( "a", 1, true);
      iStat += EqnTest( "aa", 2, true);
      iStat += EqnTest( "2*a", 2, true);
      iStat += EqnTest( "2*aa", 4, true);
      iStat += EqnTest( "2*a-1", 1, true);
      iStat += EqnTest( "2*aa-1", 3, true);

      // custom value recognition
      iStat += EqnTest( "0xff", 255, true);
      iStat += EqnTest( "0x97 + 0xff", 406, true);

      // Finally test querying of used variables
      try
      {
        int idx;
        qmu::QmuParser p;
        qreal vVarVal[] = { 1, 2, 3, 4, 5};
        p.DefineVar( "a", &vVarVal[0]);
        p.DefineVar( "b", &vVarVal[1]);
        p.DefineVar( "c", &vVarVal[2]);
        p.DefineVar( "d", &vVarVal[3]);
        p.DefineVar( "e", &vVarVal[4]);

        // Test lookup of defined variables
        // 4 used variables
        p.SetExpr( "a+b+c+d" );
        qmu::varmap_type UsedVar = p.GetUsedVar();
        int iCount = (int)UsedVar.size();
        if (iCount!=4) 
          throw false;
        
        // the next check will fail if the parser 
        // erroneousely creates new variables internally
        if (p.GetVar().size()!=5)
          throw false;

        qmu::varmap_type::const_iterator item = UsedVar.begin();
        for (idx=0; item!=UsedVar.end(); ++item)
        {
          if (&vVarVal[idx++]!=item->second) 
            throw false;
        }

        // Test lookup of undefined variables
        p.SetExpr( "undef1+undef2+undef3" );
        UsedVar = p.GetUsedVar();
        iCount = (int)UsedVar.size();
        if (iCount!=3) 
          throw false;

        // the next check will fail if the parser 
        // erroneousely creates new variables internally
        if (p.GetVar().size()!=5)
          throw false;

        for (item = UsedVar.begin(); item!=UsedVar.end(); ++item)
        {
          if (item->second!=0) 
            throw false; // all pointers to undefined variables must be null
        }

        // 1 used variables
        p.SetExpr( "a+b" );
        UsedVar = p.GetUsedVar();
        iCount = (int)UsedVar.size();
        if (iCount!=2) throw false;
        item = UsedVar.begin();
        for (idx=0; item!=UsedVar.end(); ++item)
          if (&vVarVal[idx++]!=item->second) throw false;

      }
      catch(...)
      {
        iStat += 1;
      }

      if (iStat==0)  
        qmu::console() << "passed" << endl;
      else
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }

    //---------------------------------------------------------------------------
    int QmuParserTester::TestMultiArg()
    {
      int iStat = 0;
      qmu::console() << "testing multiarg functions...";
    
      // Compound expressions
      iStat += EqnTest( "1,2,3", 3, true);
      iStat += EqnTest( "a,b,c", 3, true);
      iStat += EqnTest( "a=10,b=20,c=a*b", 200, true);
      iStat += EqnTest( "1,\n2,\n3", 3, true);
      iStat += EqnTest( "a,\nb,\nc", 3, true);
      iStat += EqnTest( "a=10,\nb=20,\nc=a*b", 200, true);
      iStat += EqnTest( "1,\r\n2,\r\n3", 3, true);
      iStat += EqnTest( "a,\r\nb,\r\nc", 3, true);
      iStat += EqnTest( "a=10,\r\nb=20,\r\nc=a*b", 200, true);

      // picking the right argument
      iStat += EqnTest( "f1of1(1)", 1, true);
      iStat += EqnTest( "f1of2(1, 2)", 1, true);
      iStat += EqnTest( "f2of2(1, 2)", 2, true);
      iStat += EqnTest( "f1of3(1, 2, 3)", 1, true);
      iStat += EqnTest( "f2of3(1, 2, 3)", 2, true);
      iStat += EqnTest( "f3of3(1, 2, 3)", 3, true);
      iStat += EqnTest( "f1of4(1, 2, 3, 4)", 1, true);
      iStat += EqnTest( "f2of4(1, 2, 3, 4)", 2, true);
      iStat += EqnTest( "f3of4(1, 2, 3, 4)", 3, true);
      iStat += EqnTest( "f4of4(1, 2, 3, 4)", 4, true);
      iStat += EqnTest( "f1of5(1, 2, 3, 4, 5)", 1, true);
      iStat += EqnTest( "f2of5(1, 2, 3, 4, 5)", 2, true);
      iStat += EqnTest( "f3of5(1, 2, 3, 4, 5)", 3, true);
      iStat += EqnTest( "f4of5(1, 2, 3, 4, 5)", 4, true);
      iStat += EqnTest( "f5of5(1, 2, 3, 4, 5)", 5, true);
      // Too few arguments / Too many arguments
      iStat += EqnTest( "1+ping()", 11, true);
      iStat += EqnTest( "ping()+1", 11, true);
      iStat += EqnTest( "2*ping()", 20, true);
      iStat += EqnTest( "ping()*2", 20, true);
      iStat += EqnTest( "ping(1,2)", 0, false);
      iStat += EqnTest( "1+ping(1,2)", 0, false);
      iStat += EqnTest( "f1of1(1,2)", 0, false);
      iStat += EqnTest( "f1of1()", 0, false);
      iStat += EqnTest( "f1of2(1, 2, 3)", 0, false);
      iStat += EqnTest( "f1of2(1)", 0, false);
      iStat += EqnTest( "f1of3(1, 2, 3, 4)", 0, false);
      iStat += EqnTest( "f1of3(1)", 0, false);
      iStat += EqnTest( "f1of4(1, 2, 3, 4, 5)", 0, false);
      iStat += EqnTest( "f1of4(1)", 0, false);
      iStat += EqnTest( "(1,2,3)", 0, false);
      iStat += EqnTest( "1,2,3", 0, false);
      iStat += EqnTest( "(1*a,2,3)", 0, false);
      iStat += EqnTest( "1,2*a,3", 0, false);
     
      // correct calculation of arguments
      iStat += EqnTest( "min(a, 1)",  1, true);
      iStat += EqnTest( "min(3*2, 1)",  1, true);
      iStat += EqnTest( "min(3*2, 1)",  6, false);
      iStat += EqnTest( "firstArg(2,3,4)", 2, true);
      iStat += EqnTest( "lastArg(2,3,4)", 4, true);
      iStat += EqnTest( "min(3*a+1, 1)",  1, true);
      iStat += EqnTest( "max(3*a+1, 1)",  4, true);
      iStat += EqnTest( "max(3*a+1, 1)*2",  8, true);
      iStat += EqnTest( "2*max(3*a+1, 1)+2",  10, true);

      // functions with Variable argument count
      iStat += EqnTest( "sum(a)", 1, true);
      iStat += EqnTest( "sum(1,2,3)",  6, true);
      iStat += EqnTest( "sum(a,b,c)",  6, true);
      iStat += EqnTest( "sum(1,-max(1,2),3)*2",  4, true);
      iStat += EqnTest( "2*sum(1,2,3)",  12, true);
      iStat += EqnTest( "2*sum(1,2,3)+2",  14, true);
      iStat += EqnTest( "2*sum(-1,2,3)+2",  10, true);
      iStat += EqnTest( "2*sum(-1,2,-(-a))+2",  6, true);
      iStat += EqnTest( "2*sum(-1,10,-a)+2",  18, true);
      iStat += EqnTest( "2*sum(1,2,3)*2",  24, true);
      iStat += EqnTest( "sum(1,-max(1,2),3)*2",  4, true);
      iStat += EqnTest( "sum(1*3, 4, a+2)",  10, true);
      iStat += EqnTest( "sum(1*3, 2*sum(1,2,2), a+2)",  16, true);
      iStat += EqnTest( "sum(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2)", 24, true);

      // some failures
      iStat += EqnTest( "sum()",  0, false);
      iStat += EqnTest( "sum(,)",  0, false);
      iStat += EqnTest( "sum(1,2,)",  0, false);
      iStat += EqnTest( "sum(,1,2)",  0, false);

      if (iStat==0) 
        qmu::console() << "passed" << endl;
      else
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;
  
      return iStat;
    }


    //---------------------------------------------------------------------------
    int QmuParserTester::TestInfixOprt()
    {
      int iStat(0);
      qmu::console() << "testing infix operators...";

      iStat += EqnTest( "-1",    -1, true);
      iStat += EqnTest( "-(-1)",  1, true);
      iStat += EqnTest( "-(-1)*2",  2, true);
      iStat += EqnTest( "-(-2)*sqrt(4)",  4, true);
      iStat += EqnTest( "-_pi", -PARSER_CONST_PI, true);
      iStat += EqnTest( "-a",  -1, true);
      iStat += EqnTest( "-(a)",  -1, true);
      iStat += EqnTest( "-(-a)",  1, true);
      iStat += EqnTest( "-(-a)*2",  2, true);
      iStat += EqnTest( "-(8)", -8, true);
      iStat += EqnTest( "-8", -8, true);
      iStat += EqnTest( "-(2+1)", -3, true);
      iStat += EqnTest( "-(f1of1(1+2*3)+1*2)", -9, true);
      iStat += EqnTest( "-(-f1of1(1+2*3)+1*2)", 5, true);
      iStat += EqnTest( "-sin(8)", -0.989358, true);
      iStat += EqnTest( "3-(-a)", 4, true);
      iStat += EqnTest( "3--a", 4, true);
      iStat += EqnTest( "-1*3",  -3, true);

      // Postfix / infix priorities
      iStat += EqnTest( "~2#", 8, true);
      iStat += EqnTest( "~f1of1(2)#", 8, true);
      iStat += EqnTest( "~(b)#", 8, true);
      iStat += EqnTest( "(~b)#", 12, true);
      iStat += EqnTest( "~(2#)", 8, true);
      iStat += EqnTest( "~(f1of1(2)#)", 8, true);
      //
      iStat += EqnTest( "-2^2",-4, true);
      iStat += EqnTest( "-(a+b)^2",-9, true);
      iStat += EqnTest( "(-3)^2",9, true);
      iStat += EqnTest( "-(-2^2)",4, true);
      iStat += EqnTest( "3+-3^2",-6, true);
      // The following assumes use of sqr as postfix operator ("") together
      // with a sign operator of low priority:
      iStat += EqnTest( "-2'", -4, true);
      iStat += EqnTest( "-(1+1)'",-4, true);
      iStat += EqnTest( "2+-(1+1)'",-2, true);
      iStat += EqnTest( "2+-2'", -2, true);
      // This is the classic behaviour of the infix sign operator (here: "$") which is
      // now deprecated:
      iStat += EqnTest( "$2^2",4, true);
      iStat += EqnTest( "$(a+b)^2",9, true);
      iStat += EqnTest( "($3)^2",9, true);
      iStat += EqnTest( "$($2^2)",-4, true);
      iStat += EqnTest( "3+$3^2",12, true);

      // infix operators sharing the first few characters
      iStat += EqnTest( "~ 123",  123+2, true);
      iStat += EqnTest( "~~ 123",  123+2, true);

      if (iStat==0)
        qmu::console() << "passed" << endl;
      else
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }


    //---------------------------------------------------------------------------
    int QmuParserTester::TestPostFix()
    {
      int iStat = 0;
      qmu::console() << "testing postfix operators...";

      // application
      iStat += EqnTest( "3{m}+5", 5.003, true);
      iStat += EqnTest( "1000{m}", 1, true);
      iStat += EqnTest( "1000 {m}", 1, true);
      iStat += EqnTest( "(a){m}", 1e-3, true);
      iStat += EqnTest( "a{m}", 1e-3, true);
      iStat += EqnTest( "a {m}", 1e-3, true);
      iStat += EqnTest( "-(a){m}", -1e-3, true);
      iStat += EqnTest( "-2{m}", -2e-3, true);
      iStat += EqnTest( "-2 {m}", -2e-3, true);
      iStat += EqnTest( "f1of1(1000){m}", 1, true);
      iStat += EqnTest( "-f1of1(1000){m}", -1, true);
      iStat += EqnTest( "-f1of1(-1000){m}", 1, true);
      iStat += EqnTest( "f4of4(0,0,0,1000){m}", 1, true);
      iStat += EqnTest( "2+(a*1000){m}", 3, true);

      // can postfix operators "m" und "meg" be told apart properly?
      iStat += EqnTest( "2*3000meg+2", 2*3e9+2, true);

      // some incorrect results
      iStat += EqnTest( "1000{m}", 0.1, false);
      iStat += EqnTest( "(a){m}", 2, false);
      // failure due to syntax checking
      iStat += ThrowTest("0x", ecUNASSIGNABLE_TOKEN);  // incomplete hex definition
      iStat += ThrowTest("3+", ecUNEXPECTED_EOF);
      iStat += ThrowTest( "4 + {m}", ecUNASSIGNABLE_TOKEN);
      iStat += ThrowTest( "{m}4", ecUNASSIGNABLE_TOKEN);
      iStat += ThrowTest( "sin({m})", ecUNASSIGNABLE_TOKEN);
      iStat += ThrowTest( "{m} {m}", ecUNASSIGNABLE_TOKEN);
      iStat += ThrowTest( "{m}(8)", ecUNASSIGNABLE_TOKEN);
      iStat += ThrowTest( "4,{m}", ecUNASSIGNABLE_TOKEN);
      iStat += ThrowTest( "-{m}", ecUNASSIGNABLE_TOKEN);
      iStat += ThrowTest( "2(-{m})", ecUNEXPECTED_PARENS);
      iStat += ThrowTest( "2({m})", ecUNEXPECTED_PARENS);
 
      iStat += ThrowTest( "multi*1.0", ecUNASSIGNABLE_TOKEN);

      if (iStat==0)
        qmu::console() << "passed" << endl;
      else
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }

    //---------------------------------------------------------------------------
    int QmuParserTester::TestExpression()
    {
      int iStat = 0;
      qmu::console() << "testing expression samples...";

      qreal b = 2;

      // Optimization
      iStat += EqnTest( "2*b*5", 20, true);
      iStat += EqnTest( "2*b*5 + 4*b", 28, true);
      iStat += EqnTest( "2*a/3", 2.0/3.0, true);

      // Addition auf cmVARMUL 
      iStat += EqnTest( "3+b", b+3, true);
      iStat += EqnTest( "b+3", b+3, true);
      iStat += EqnTest( "b*3+2", b*3+2, true);
      iStat += EqnTest( "3*b+2", b*3+2, true);
      iStat += EqnTest( "2+b*3", b*3+2, true);
      iStat += EqnTest( "2+3*b", b*3+2, true);
      iStat += EqnTest( "b+3*b", b+3*b, true);
      iStat += EqnTest( "3*b+b", b+3*b, true);

      iStat += EqnTest( "2+b*3+b", 2+b*3+b, true);
      iStat += EqnTest( "b+2+b*3", b+2+b*3, true);

      iStat += EqnTest( "(2*b+1)*4", (2*b+1)*4, true);
      iStat += EqnTest( "4*(2*b+1)", (2*b+1)*4, true);

      // operator precedencs
      iStat += EqnTest( "1+2-3*4/5^6", 2.99923, true);
      iStat += EqnTest( "1^2/3*4-5+6", 2.33333333, true);
      iStat += EqnTest( "1+2*3", 7, true);
      iStat += EqnTest( "1+2*3", 7, true);
      iStat += EqnTest( "(1+2)*3", 9, true);
      iStat += EqnTest( "(1+2)*(-3)", -9, true);
      iStat += EqnTest( "2/4", 0.5, true);

      iStat += EqnTest( "exp(ln(7))", 7, true);
      iStat += EqnTest( "e^ln(7)", 7, true);
      iStat += EqnTest( "e^(ln(7))", 7, true);
      iStat += EqnTest( "(e^(ln(7)))", 7, true);
      iStat += EqnTest( "1-(e^(ln(7)))", -6, true);
      iStat += EqnTest( "2*(e^(ln(7)))", 14, true);
      iStat += EqnTest( "10^log(5)", 5, true);
      iStat += EqnTest( "10^log10(5)", 5, true);
      iStat += EqnTest( "2^log2(4)", 4, true);
      iStat += EqnTest( "-(sin(0)+1)", -1, true);
      iStat += EqnTest( "-(2^1.1)", -2.14354692, true);

      iStat += EqnTest( "(cos(2.41)/b)", -0.372056, true);
      iStat += EqnTest( "(1*(2*(3*(4*(5*(6*(a+b)))))))", 2160, true);
      iStat += EqnTest( "(1*(2*(3*(4*(5*(6*(7*(a+b))))))))", 15120, true);
      iStat += EqnTest( "(a/((((b+(((e*(((((pi*((((3.45*((pi+a)+pi))+b)+b)*a))+0.68)+e)+a)/a))+a)+b))+b)*a)-pi))", 0.00377999, true);

      // long formula (Reference: Matlab)
      iStat += EqnTest(
        "(((-9))-e/(((((((pi-(((-7)+(-3)/4/e))))/(((-5))-2)-((pi+(-0))*(sqrt((e+e))*(-8))*(((-pi)+(-pi)-(-9)*(6*5))"
        "/(-e)-e))/2)/((((sqrt(2/(-e)+6)-(4-2))+((5/(-2))/(1*(-pi)+3))/8)*pi*((pi/((-2)/(-6)*1*(-1))*(-6)+(-e)))))/"
        "((e+(-2)+(-e)*((((-3)*9+(-e)))+(-9)))))))-((((e-7+(((5/pi-(3/1+pi)))))/e)/(-5))/(sqrt((((((1+(-7))))+((((-"
        "e)*(-e)))-8))*(-5)/((-e)))*(-6)-((((((-2)-(-9)-(-e)-1)/3))))/(sqrt((8+(e-((-6))+(9*(-9))))*(((3+2-8))*(7+6"
        "+(-5))+((0/(-e)*(-pi))+7)))+(((((-e)/e/e)+((-6)*5)*e+(3+(-5)/pi))))+pi))/sqrt((((9))+((((pi))-8+2))+pi))/e"
        "*4)*((-5)/(((-pi))*(sqrt(e)))))-(((((((-e)*(e)-pi))/4+(pi)*(-9)))))))+(-pi)", -12.23016549, true);

      // long formula (Reference: Matlab)
      iStat += EqnTest(
          "(atan(sin((((((((((((((((pi/cos((a/((((0.53-b)-pi)*e)/b))))+2.51)+a)-0.54)/0.98)+b)*b)+e)/a)+b)+a)+b)+pi)/e"
          ")+a)))*2.77)", -2.16995656, true);

      // long formula (Reference: Matlab)
      iStat += EqnTest( "1+2-3*4/5^6*(2*(1-5+(3*7^9)*(4+6*7-3)))+12", -7995810.09926, true);
	  
      if (iStat==0) 
        qmu::console() << "passed" << endl;
      else 
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }



    //---------------------------------------------------------------------------
    int QmuParserTester::TestIfThenElse()
    {
      int iStat = 0;
      qmu::console() << "testing if-then-else operator...";

      // Test error detection
      iStat += ThrowTest(":3", ecUNEXPECTED_CONDITIONAL);
      iStat += ThrowTest("? 1 : 2", ecUNEXPECTED_CONDITIONAL);
      iStat += ThrowTest("(a<b) ? (b<c) ? 1 : 2", ecMISSING_ELSE_CLAUSE);
      iStat += ThrowTest("(a<b) ? 1", ecMISSING_ELSE_CLAUSE);
      iStat += ThrowTest("(a<b) ? a", ecMISSING_ELSE_CLAUSE);
      iStat += ThrowTest("(a<b) ? a+b", ecMISSING_ELSE_CLAUSE);
      iStat += ThrowTest("a : b", ecMISPLACED_COLON);
      iStat += ThrowTest("1 : 2", ecMISPLACED_COLON);
      iStat += ThrowTest("(1) ? 1 : 2 : 3", ecMISPLACED_COLON);
      iStat += ThrowTest("(true) ? 1 : 2 : 3", ecUNASSIGNABLE_TOKEN);

      iStat += EqnTest("1 ? 128 : 255", 128, true);
      iStat += EqnTest("1<2 ? 128 : 255", 128, true);
      iStat += EqnTest("a<b ? 128 : 255", 128, true);
      iStat += EqnTest("(a<b) ? 128 : 255", 128, true);
      iStat += EqnTest("(1) ? 10 : 11", 10, true);
      iStat += EqnTest("(0) ? 10 : 11", 11, true);
      iStat += EqnTest("(1) ? a+b : c+d", 3, true);
      iStat += EqnTest("(0) ? a+b : c+d", 1, true);
      iStat += EqnTest("(1) ? 0 : 1", 0, true);
      iStat += EqnTest("(0) ? 0 : 1", 1, true);
      iStat += EqnTest("(a<b) ? 10 : 11", 10, true);
      iStat += EqnTest("(a>b) ? 10 : 11", 11, true);
      iStat += EqnTest("(a<b) ? c : d", 3, true);
      iStat += EqnTest("(a>b) ? c : d", -2, true);

      iStat += EqnTest("(a>b) ? 1 : 0", 0, true);
      iStat += EqnTest("((a>b) ? 1 : 0) ? 1 : 2", 2, true);
      iStat += EqnTest("((a>b) ? 1 : 0) ? 1 : sum((a>b) ? 1 : 2)", 2, true);
      iStat += EqnTest("((a>b) ? 0 : 1) ? 1 : sum((a>b) ? 1 : 2)", 1, true);

      iStat += EqnTest("sum((a>b) ? 1 : 2)", 2, true);
      iStat += EqnTest("sum((1) ? 1 : 2)", 1, true);
      iStat += EqnTest("sum((a>b) ? 1 : 2, 100)", 102, true);
      iStat += EqnTest("sum((1) ? 1 : 2, 100)", 101, true);
      iStat += EqnTest("sum(3, (a>b) ? 3 : 10)", 13, true);
      iStat += EqnTest("sum(3, (a<b) ? 3 : 10)", 6, true);
      iStat += EqnTest("10*sum(3, (a>b) ? 3 : 10)", 130, true);
      iStat += EqnTest("10*sum(3, (a<b) ? 3 : 10)", 60, true);
      iStat += EqnTest("sum(3, (a>b) ? 3 : 10)*10", 130, true);
      iStat += EqnTest("sum(3, (a<b) ? 3 : 10)*10", 60, true);
      iStat += EqnTest("(a<b) ? sum(3, (a<b) ? 3 : 10)*10 : 99", 60, true);
      iStat += EqnTest("(a>b) ? sum(3, (a<b) ? 3 : 10)*10 : 99", 99, true);
      iStat += EqnTest("(a<b) ? sum(3, (a<b) ? 3 : 10,10,20)*10 : 99", 360, true);
      iStat += EqnTest("(a>b) ? sum(3, (a<b) ? 3 : 10,10,20)*10 : 99", 99, true);
      iStat += EqnTest("(a>b) ? sum(3, (a<b) ? 3 : 10,10,20)*10 : sum(3, (a<b) ? 3 : 10)*10", 60, true);

      // todo: auch fr muParserX hinzufgen!
      iStat += EqnTest("(a<b)&&(a<b) ? 128 : 255", 128, true);
      iStat += EqnTest("(a>b)&&(a<b) ? 128 : 255", 255, true);
      iStat += EqnTest("(1<2)&&(1<2) ? 128 : 255", 128, true);
      iStat += EqnTest("(1>2)&&(1<2) ? 128 : 255", 255, true);
      iStat += EqnTest("((1<2)&&(1<2)) ? 128 : 255", 128, true);
      iStat += EqnTest("((1>2)&&(1<2)) ? 128 : 255", 255, true);
      iStat += EqnTest("((a<b)&&(a<b)) ? 128 : 255", 128, true);
      iStat += EqnTest("((a>b)&&(a<b)) ? 128 : 255", 255, true);

      iStat += EqnTest("1>0 ? 1>2 ? 128 : 255 : 1>0 ? 32 : 64", 255, true);
      iStat += EqnTest("1>0 ? 1>2 ? 128 : 255 :(1>0 ? 32 : 64)", 255, true);
      iStat += EqnTest("1>0 ? 1>0 ? 128 : 255 : 1>2 ? 32 : 64", 128, true);
      iStat += EqnTest("1>0 ? 1>0 ? 128 : 255 :(1>2 ? 32 : 64)", 128, true);
      iStat += EqnTest("1>2 ? 1>2 ? 128 : 255 : 1>0 ? 32 : 64", 32, true);
      iStat += EqnTest("1>2 ? 1>0 ? 128 : 255 : 1>2 ? 32 : 64", 64, true);
      iStat += EqnTest("1>0 ? 50 :  1>0 ? 128 : 255", 50, true);
      iStat += EqnTest("1>0 ? 50 : (1>0 ? 128 : 255)", 50, true);
      iStat += EqnTest("1>0 ? 1>0 ? 128 : 255 : 50", 128, true);
      iStat += EqnTest("1>2 ? 1>2 ? 128 : 255 : 1>0 ? 32 : 1>2 ? 64 : 16", 32, true);
      iStat += EqnTest("1>2 ? 1>2 ? 128 : 255 : 1>0 ? 32 :(1>2 ? 64 : 16)", 32, true);
      iStat += EqnTest("1>0 ? 1>2 ? 128 : 255 :  1>0 ? 32 :1>2 ? 64 : 16", 255, true);
      iStat += EqnTest("1>0 ? 1>2 ? 128 : 255 : (1>0 ? 32 :1>2 ? 64 : 16)", 255, true);
      iStat += EqnTest("1 ? 0 ? 128 : 255 : 1 ? 32 : 64", 255, true);

      // assignment operators
      iStat += EqnTest("a= 0 ? 128 : 255, a", 255, true);
      iStat += EqnTest("a=((a>b)&&(a<b)) ? 128 : 255, a", 255, true);
      iStat += EqnTest("c=(a<b)&&(a<b) ? 128 : 255, c", 128, true);
      iStat += EqnTest("0 ? a=a+1 : 666, a", 1, true);
      iStat += EqnTest("1?a=10:a=20, a", 10, true);
      iStat += EqnTest("0?a=10:a=20, a", 20, true);
      iStat += EqnTest("0?a=sum(3,4):10, a", 1, true);  // a should not change its value due to lazy calculation
      
      iStat += EqnTest("a=1?b=1?3:4:5, a", 3, true);
      iStat += EqnTest("a=1?b=1?3:4:5, b", 3, true);
      iStat += EqnTest("a=0?b=1?3:4:5, a", 5, true);
      iStat += EqnTest("a=0?b=1?3:4:5, b", 2, true);

      iStat += EqnTest("a=1?5:b=1?3:4, a", 5, true);
      iStat += EqnTest("a=1?5:b=1?3:4, b", 2, true);
      iStat += EqnTest("a=0?5:b=1?3:4, a", 3, true);
      iStat += EqnTest("a=0?5:b=1?3:4, b", 3, true);

      if (iStat==0) 
        qmu::console() << "passed" << endl;
      else 
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }

    //---------------------------------------------------------------------------
    int QmuParserTester::TestException()
    {
      int  iStat = 0;
      qmu::console() << "testing error codes...";

      iStat += ThrowTest("3+",           ecUNEXPECTED_EOF);
      iStat += ThrowTest("3+)",          ecUNEXPECTED_PARENS);
      iStat += ThrowTest("()",           ecUNEXPECTED_PARENS);
      iStat += ThrowTest("3+()",         ecUNEXPECTED_PARENS);
      iStat += ThrowTest("sin(3,4)",     ecTOO_MANY_PARAMS);
      iStat += ThrowTest("sin()",        ecTOO_FEW_PARAMS);
      iStat += ThrowTest("(1+2",         ecMISSING_PARENS);
      iStat += ThrowTest("sin(3)3",      ecUNEXPECTED_VAL);
      iStat += ThrowTest("sin(3)xyz",    ecUNASSIGNABLE_TOKEN);
      iStat += ThrowTest("sin(3)cos(3)", ecUNEXPECTED_FUN);
      iStat += ThrowTest("a+b+c=10",     ecUNEXPECTED_OPERATOR);
      iStat += ThrowTest("a=b=3",        ecUNEXPECTED_OPERATOR);

#if defined(MUP_MATH_EXCEPTIONS)
      // divide by zero whilst constant folding
      iStat += ThrowTest("1/0",          ecDIV_BY_ZERO);
      // square root of a negative number
      iStat += ThrowTest("sqrt(-1)",     ecDOMAIN_ERROR);
      // logarithms of zero
      iStat += ThrowTest("ln(0)",        ecDOMAIN_ERROR);
      iStat += ThrowTest("log2(0)",      ecDOMAIN_ERROR);
      iStat += ThrowTest("log10(0)",     ecDOMAIN_ERROR);
      iStat += ThrowTest("log(0)",       ecDOMAIN_ERROR);
      // logarithms of negative values
      iStat += ThrowTest("ln(-1)",       ecDOMAIN_ERROR);
      iStat += ThrowTest("log2(-1)",     ecDOMAIN_ERROR);
      iStat += ThrowTest("log10(-1)",    ecDOMAIN_ERROR);
      iStat += ThrowTest("log(-1)",      ecDOMAIN_ERROR);
#endif

      // functions without parameter
      iStat += ThrowTest( "3+ping(2)",    ecTOO_MANY_PARAMS);
      iStat += ThrowTest( "3+ping(a+2)",  ecTOO_MANY_PARAMS);
      iStat += ThrowTest( "3+ping(sin(a)+2)",  ecTOO_MANY_PARAMS);
      iStat += ThrowTest( "3+ping(1+sin(a))",  ecTOO_MANY_PARAMS);

      // String function related
      iStat += ThrowTest( "valueof(\"xxx\")",  999, false);
      iStat += ThrowTest( "valueof()",          ecUNEXPECTED_PARENS);
      iStat += ThrowTest( "1+valueof(\"abc\"",  ecMISSING_PARENS);
      iStat += ThrowTest( "valueof(\"abc\"",    ecMISSING_PARENS);
      iStat += ThrowTest( "valueof(\"abc",      ecUNTERMINATED_STRING);
      iStat += ThrowTest( "valueof(\"abc\",3)", ecTOO_MANY_PARAMS);
      iStat += ThrowTest( "valueof(3)",         ecSTRING_EXPECTED);
      iStat += ThrowTest( "sin(\"abc\")",       ecVAL_EXPECTED);
      iStat += ThrowTest( "valueof(\"\\\"abc\\\"\")",  999, false);
      iStat += ThrowTest( "\"hello world\"",    ecSTR_RESULT);
      iStat += ThrowTest( "(\"hello world\")",  ecSTR_RESULT);
      iStat += ThrowTest( "\"abcd\"+100",       ecOPRT_TYPE_CONFLICT);
      iStat += ThrowTest( "\"a\"+\"b\"",        ecOPRT_TYPE_CONFLICT);
      iStat += ThrowTest( "strfun1(\"100\",3)",     ecTOO_MANY_PARAMS);
      iStat += ThrowTest( "strfun2(\"100\",3,5)",   ecTOO_MANY_PARAMS);
      iStat += ThrowTest( "strfun3(\"100\",3,5,6)", ecTOO_MANY_PARAMS);
      iStat += ThrowTest( "strfun2(\"100\")",       ecTOO_FEW_PARAMS);
      iStat += ThrowTest( "strfun3(\"100\",6)",   ecTOO_FEW_PARAMS);
      iStat += ThrowTest( "strfun2(1,1)",         ecSTRING_EXPECTED);
      iStat += ThrowTest( "strfun2(a,1)",         ecSTRING_EXPECTED);
      iStat += ThrowTest( "strfun2(1,1,1)",       ecTOO_MANY_PARAMS);
      iStat += ThrowTest( "strfun2(a,1,1)",       ecTOO_MANY_PARAMS);
      iStat += ThrowTest( "strfun3(1,2,3)",         ecSTRING_EXPECTED);
      iStat += ThrowTest( "strfun3(1, \"100\",3)",  ecSTRING_EXPECTED);
      iStat += ThrowTest( "strfun3(\"1\", \"100\",3)",  ecVAL_EXPECTED);
      iStat += ThrowTest( "strfun3(\"1\", 3, \"100\")",  ecVAL_EXPECTED);
      iStat += ThrowTest( "strfun3(\"1\", \"100\", \"100\", \"100\")",  ecTOO_MANY_PARAMS);

      // assignement operator
      iStat += ThrowTest( "3=4", ecUNEXPECTED_OPERATOR);
      iStat += ThrowTest( "sin(8)=4", ecUNEXPECTED_OPERATOR);
      iStat += ThrowTest( "\"test\"=a", ecUNEXPECTED_OPERATOR);

      // <ibg 20090529>
      // this is now legal, for reference see:
      // https://sourceforge.net/forum/message.php?msg_id=7411373
      //      iStat += ThrowTest( "sin=9"), ecUNEXPECTED_OPERATOR);
      // </ibg>

      iStat += ThrowTest( "(8)=5", ecUNEXPECTED_OPERATOR);
      iStat += ThrowTest( "(a)=5", ecUNEXPECTED_OPERATOR);
      iStat += ThrowTest( "a=\"tttt\"", ecOPRT_TYPE_CONFLICT);

      if (iStat==0) 
        qmu::console() << "passed" << endl;
      else 
        qmu::console() << "\n  failed with " << iStat << " errors" << endl;

      return iStat;
    }


    //---------------------------------------------------------------------------
    void QmuParserTester::AddTest(testfun_type a_pFun)
    {
      m_vTestFun.push_back(a_pFun);
    }

    //---------------------------------------------------------------------------
    void QmuParserTester::Run()
    {
      int iStat = 0;
      try
      {
        for (int i=0; i<(int)m_vTestFun.size(); ++i)
          iStat += (this->*m_vTestFun[i])();
      }
      catch(QmuParser::exception_type &e)
      {
        qmu::console() << "\n" << e.GetMsg() << endl;
        qmu::console() << e.GetToken() << endl;
        Abort();
      }
      catch(std::exception &e)
      {
        qmu::console() << e.what() << endl;
        Abort();
      }
      catch(...)
      {
        qmu::console() << "Internal error";
        Abort();
      }

      if (iStat==0) 
      {
        qmu::console() << "Test passed (" <<  QmuParserTester::c_iCount << " expressions)" << endl;
      }
      else 
      {
        qmu::console() << "Test failed with " << iStat
                  << " errors (" <<  QmuParserTester::c_iCount
                  << " expressions)" << endl;
      }
      QmuParserTester::c_iCount = 0;
    }


    //---------------------------------------------------------------------------
    int QmuParserTester::ThrowTest(const string_type &a_str, int a_iErrc, bool a_bFail)
    {
      QmuParserTester::c_iCount++;

      try
      {
        qreal fVal[] = {1,1,1};
        QmuParser p;

        p.DefineVar( "a", &fVal[0]);
        p.DefineVar( "b", &fVal[1]);
        p.DefineVar( "c", &fVal[2]);
        p.DefinePostfixOprt( "{m}", Milli);
        p.DefinePostfixOprt( "m", Milli);
        p.DefineFun( "ping", Ping);
        p.DefineFun( "valueof", ValueOf);
        p.DefineFun( "strfun1", StrFun1);
        p.DefineFun( "strfun2", StrFun2);
        p.DefineFun( "strfun3", StrFun3);
        p.SetExpr(a_str);
        p.Eval();
      }
      catch(QmuParserError &e)
      {
        // output the formula in case of an failed test
        if (a_bFail==false || (a_bFail==true && a_iErrc!=e.GetCode()) )
        {
          qmu::console() << "\n  "
                        << "Expression: " << a_str
                        << "  Code:" << e.GetCode() << "(" << e.GetMsg() << ")"
                        << "  Expected:" << a_iErrc;
        }

        return (a_iErrc==e.GetCode()) ? 0 : 1;
      }

      // if a_bFail==false no exception is expected
      bool bRet((a_bFail==false) ? 0 : 1);
      if (bRet==1)
      {
        qmu::console() << "\n  "
                      << "Expression: " << a_str
                      << "  did evaluate; Expected error:" << a_iErrc;
      }

      return bRet; 
    }

    //---------------------------------------------------------------------------
    /** \brief Evaluate a tet expression. 

        \return 1 in case of a failure, 0 otherwise.
    */
    int QmuParserTester::EqnTestWithVarChange(const string_type &a_str,
                                           double a_fVar1, 
                                           double a_fRes1, 
                                           double a_fVar2, 
                                           double a_fRes2)
    {
      QmuParserTester::c_iCount++;
      qreal fVal[2] = {-999, -999 }; // should be equalinitially

      try
      {
        QmuParser  p;

        // variable
        qreal var = 0;
        p.DefineVar( "a", &var);
        p.SetExpr(a_str);

        var = a_fVar1;
        fVal[0] = p.Eval();

        var = a_fVar2;
        fVal[1] = p.Eval();
        
        if ( fabs(a_fRes1-fVal[0]) > 0.0000000001)
          throw std::runtime_error("incorrect result (first pass)");

        if ( fabs(a_fRes2-fVal[1]) > 0.0000000001)
          throw std::runtime_error("incorrect result (second pass)");
      }
      catch(QmuParser::exception_type &e)
      {
        qmu::console() << "\n  fail: " << a_str.c_str() << " (" << e.GetMsg() << ")";
        return 1;
      }
      catch(std::exception &e)
      {
        qmu::console() << "\n  fail: " << a_str.c_str() << " (" << e.what() << ")";
        return 1;  // always return a failure since this exception is not expected
      }
      catch(...)
      {
        qmu::console() << "\n  fail: " << a_str.c_str() <<  " (unexpected exception)";
        return 1;  // exceptions other than ParserException are not allowed
      }

      return 0;
    }

    //---------------------------------------------------------------------------
    /** \brief Evaluate a tet expression. 

        \return 1 in case of a failure, 0 otherwise.
    */
    int QmuParserTester::EqnTest(const string_type &a_str, double a_fRes, bool a_fPass)
    {
      QmuParserTester::c_iCount++;
      int iRet(0);
      qreal fVal[5] = {-999, -998, -997, -996, -995}; // initially should be different

      try
      {
        std::auto_ptr<QmuParser> p1;
        QmuParser  p2, p3;   // three parser objects
                          // they will be used for testing copy and assihnment operators
        // p1 is a pointer since i'm going to delete it in order to test if
        // parsers after copy construction still refer to members of it.
        // !! If this is the case this function will crash !!
      
        p1.reset(new qmu::QmuParser());
        // Add constants
        p1->DefineConst( "pi", (qreal)PARSER_CONST_PI);
        p1->DefineConst( "e", (qreal)PARSER_CONST_E);
        p1->DefineConst( "const", 1);
        p1->DefineConst( "const1", 2);
        p1->DefineConst( "const2", 3);
        // variables
        qreal vVarVal[] = { 1, 2, 3, -2};
        p1->DefineVar( "a", &vVarVal[0]);
        p1->DefineVar( "aa", &vVarVal[1]);
        p1->DefineVar( "b", &vVarVal[1]);
        p1->DefineVar( "c", &vVarVal[2]);
        p1->DefineVar( "d", &vVarVal[3]);
        
        // custom value ident functions
        p1->AddValIdent(&QmuParserTester::IsHexVal);

        // functions
        p1->DefineFun( "ping", Ping);
        p1->DefineFun( "f1of1", f1of1);  // one parameter
        p1->DefineFun( "f1of2", f1of2);  // two parameter
        p1->DefineFun( "f2of2", f2of2);
        p1->DefineFun( "f1of3", f1of3);  // three parameter
        p1->DefineFun( "f2of3", f2of3);
        p1->DefineFun( "f3of3", f3of3);
        p1->DefineFun( "f1of4", f1of4);  // four parameter
        p1->DefineFun( "f2of4", f2of4);
        p1->DefineFun( "f3of4", f3of4);
        p1->DefineFun( "f4of4", f4of4);
        p1->DefineFun( "f1of5", f1of5);  // five parameter
        p1->DefineFun( "f2of5", f2of5);
        p1->DefineFun( "f3of5", f3of5);
        p1->DefineFun( "f4of5", f4of5);
        p1->DefineFun( "f5of5", f5of5);

        // binary operators
        p1->DefineOprt( "add", add, 0);
        p1->DefineOprt( "++", add, 0);
        p1->DefineOprt( "&", land, prLAND);

        // sample functions
        p1->DefineFun( "min", Min);
        p1->DefineFun( "max", Max);
        p1->DefineFun( "sum", Sum);
        p1->DefineFun( "valueof", ValueOf);
        p1->DefineFun( "atof", StrToFloat);
        p1->DefineFun( "strfun1", StrFun1);
        p1->DefineFun( "strfun2", StrFun2);
        p1->DefineFun( "strfun3", StrFun3);
        p1->DefineFun( "lastArg", LastArg);
        p1->DefineFun( "firstArg", FirstArg);
        p1->DefineFun( "order", FirstArg);

        // infix / postfix operator
        // Note: Identifiers used here do not have any meaning 
        //       they are mere placeholders to test certain features.
        p1->DefineInfixOprt( "$", sign, prPOW+1);  // sign with high priority
        p1->DefineInfixOprt( "~", plus2);          // high priority
        p1->DefineInfixOprt( "~~", plus2);
        p1->DefinePostfixOprt( "{m}", Milli);
        p1->DefinePostfixOprt( "{M}", Mega);
        p1->DefinePostfixOprt( "m", Milli);
        p1->DefinePostfixOprt( "meg", Mega);
        p1->DefinePostfixOprt( "#", times3);
        p1->DefinePostfixOprt( "'", sqr);
        p1->SetExpr(a_str);

        // Test bytecode integrity
        // String parsing and bytecode parsing must yield the same result
        fVal[0] = p1->Eval(); // result from stringparsing
        fVal[1] = p1->Eval(); // result from bytecode
        if (fVal[0]!=fVal[1])
          throw QmuParser::exception_type( "Bytecode / string parsing mismatch." );

        // Test copy and assignement operators
        try
        {
          // Test copy constructor
          std::vector<qmu::QmuParser> vParser;
          vParser.push_back(*(p1.get()));
          qmu::QmuParser p2 = vParser[0];   // take parser from vector
        
          // destroy the originals from p2
          vParser.clear();              // delete the vector
          p1.reset(0);

          fVal[2] = p2.Eval();

          // Test assignement operator
          // additionally  disable Optimizer this time
          qmu::QmuParser p3;
          p3 = p2;
          p3.EnableOptimizer(false);
          fVal[3] = p3.Eval();

          // Test Eval function for multiple return values
          // use p2 since it has the optimizer enabled!
          int nNum;
          qreal *v = p2.Eval(nNum);
          fVal[4] = v[nNum-1];
        }
        catch(std::exception &e)
        {
          qmu::console() << "\n  " << e.what() << "\n";
        }

        // limited floating point accuracy requires the following test
        bool bCloseEnough(true);
        for (unsigned i=0; i<sizeof(fVal)/sizeof(qreal); ++i)
        {
          bCloseEnough &= (fabs(a_fRes-fVal[i]) <= fabs(fVal[i]*0.00001));

          // The tests equations never result in infinity, if they do thats a bug.
          // reference:
          // http://sourceforge.net/projects/muparser/forums/forum/462843/topic/5037825
          if (numeric_limits<qreal>::has_infinity)
            bCloseEnough &= (fabs(fVal[i]) != numeric_limits<qreal>::infinity());
        }

        iRet = ((bCloseEnough && a_fPass) || (!bCloseEnough && !a_fPass)) ? 0 : 1;
        
        
        if (iRet==1)
        {
          qmu::console() << "\n  fail: " << a_str.c_str()
                        << " (incorrect result; expected: " << a_fRes
                        << " ;calculated: " << fVal[0] << ","
                                                << fVal[1] << ","
                                                << fVal[2] << ","
                                                << fVal[3] << ","
                                                << fVal[4] << ").";
        }
      }
      catch(QmuParser::exception_type &e)
      {
        if (a_fPass)
        {
          if (fVal[0]!=fVal[2] && fVal[0]!=-999 && fVal[1]!=-998)
            qmu::console() << "\n  fail: " << a_str.c_str() << " (copy construction)";
          else
            qmu::console() << "\n  fail: " << a_str.c_str() << " (" << e.GetMsg() << ")";
          return 1;
        }
      }
      catch(std::exception &e)
      {
        qmu::console() << "\n  fail: " << a_str.c_str() << " (" << e.what() << ")";
        return 1;  // always return a failure since this exception is not expected
      }
      catch(...)
      {
        qmu::console() << "\n  fail: " << a_str.c_str() <<  " (unexpected exception)";
        return 1;  // exceptions other than ParserException are not allowed
      }

      return iRet;
    }

    //---------------------------------------------------------------------------
    int QmuParserTester::EqnTestInt(const string_type &a_str, double a_fRes, bool a_fPass)
    {
      QmuParserTester::c_iCount++;

      qreal vVarVal[] = {1, 2, 3};   // variable values
      qreal fVal[2] = {-99, -999};   // results: initially should be different
      int iRet(0);

      try
      {
        QmuParserInt p;
        p.DefineConst( "const1", 1);
        p.DefineConst( "const2", 2);
        p.DefineVar( "a", &vVarVal[0]);
        p.DefineVar( "b", &vVarVal[1]);
        p.DefineVar( "c", &vVarVal[2]);

        p.SetExpr(a_str);
        fVal[0] = p.Eval(); // result from stringparsing
        fVal[1] = p.Eval(); // result from bytecode

        if (fVal[0]!=fVal[1])
          throw QmuParser::exception_type( "Bytecode corrupt." );

        iRet =  ( (a_fRes==fVal[0] &&  a_fPass) || 
                  (a_fRes!=fVal[0] && !a_fPass) ) ? 0 : 1;
        if (iRet==1)
        {
          qmu::console() << "\n  fail: " << a_str.c_str()
                        << " (incorrect result; expected: " << a_fRes
                        << " ;calculated: " << fVal[0]<< ").";
        }
      }
      catch(QmuParser::exception_type &e)
      {
        if (a_fPass)
        {
          qmu::console() << "\n  fail: " << e.GetExpr() << " : " << e.GetMsg();
          iRet = 1;
        }
      }
      catch(...)
      {
        qmu::console() << "\n  fail: " << a_str.c_str() <<  " (unexpected exception)";
        iRet = 1;  // exceptions other than ParserException are not allowed
      }

      return iRet;
    }

    //---------------------------------------------------------------------------
    /** \brief Internal error in test class Test is going to be aborted. */
    void QmuParserTester::Abort() const
    {
      qmu::console() << "Test failed (internal error in test class)" << endl;
      while (!getchar());
      exit(-1);
    }
  } // namespace test
} // namespace qmu
