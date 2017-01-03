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

#ifndef QMUPARSERTEST_H
#define QMUPARSERTEST_H

#include <stdlib.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <locale>

#include "../qmuparser/qmuparser_global.h"
#include "qmuparser.h"
#include "qmuparser_global.h"
#include "qmuparserdef.h"
#include "qmuparsererror.h"

/**
 * @file
 * @brief This file contains the parser test class.
 */

namespace qmu
{
/**
 * @brief Namespace for test cases.
 */
namespace Test
{
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Test cases for unit testing.
 *
 * (C) 2004-2011 Ingo Berg
 */
class QMUPARSERSHARED_EXPORT QmuParserTester : public QObject // final
{
    Q_OBJECT
public:
    typedef int ( QmuParserTester::*testfun_type ) ();

    explicit QmuParserTester(QObject *parent = nullptr);

private slots:
    void Run();

private:
    Q_DISABLE_COPY(QmuParserTester)
    QVector<testfun_type> m_vTestFun;
    static int c_iCount;

    void AddTest ( testfun_type a_pFun );

    // Test Double Parser
    static int EqnTest ( const QString &a_str, double a_fRes, bool a_fPass );
    static int EqnTestWithVarChange ( const QString &a_str, double a_fRes1, double a_fVar1, double a_fRes2,
                                      double a_fVar2 );
    static int ThrowTest ( const QString &a_str, int a_iErrc, bool a_bFail = true );

    // Test Bulkmode
    static int EqnTestBulk(const QString &a_str, double a_fRes[4], bool a_fPass);

    // Multiarg callbacks
    static qreal f1of1 ( qreal v )
    {
        return v;
    }

    static qreal f1of2 ( qreal v, qreal  )
    {
        return v;
    }

    static qreal f2of2 ( qreal, qreal v )
    {
        return v;
    }

    static qreal f1of3 ( qreal v, qreal, qreal  )
    {
        return v;
    }

    static qreal f2of3 ( qreal, qreal v, qreal  )
    {
        return v;
    }

    static qreal f3of3 ( qreal, qreal, qreal v )
    {
        return v;
    }

    static qreal f1of4 ( qreal v, qreal,   qreal, qreal  )
    {
        return v;
    }

    static qreal f2of4 ( qreal, qreal v, qreal, qreal  )
    {
        return v;
    }

    static qreal f3of4 ( qreal, qreal,   qreal v, qreal  )
    {
        return v;
    }

    static qreal f4of4 ( qreal, qreal,   qreal, qreal v )
    {
        return v;
    }

    static qreal f1of5 ( qreal v, qreal,   qreal, qreal, qreal  )
    {
        return v;
    }

    static qreal f2of5 ( qreal, qreal v, qreal, qreal, qreal  )
    {
        return v;
    }

    static qreal f3of5 ( qreal, qreal,   qreal v, qreal, qreal  )
    {
        return v;
    }

    static qreal f4of5 ( qreal, qreal,   qreal, qreal v, qreal  )
    {
        return v;
    }

    static qreal f5of5 ( qreal, qreal,   qreal, qreal, qreal v )
    {
        return v;
    }

    static qreal Min ( qreal a_fVal1, qreal a_fVal2 )
    {
        return ( a_fVal1 < a_fVal2 ) ? a_fVal1 : a_fVal2;
    }

    static qreal Max ( qreal a_fVal1, qreal a_fVal2 )
    {
        return ( a_fVal1 > a_fVal2 ) ? a_fVal1 : a_fVal2;
    }

    static qreal plus2 ( qreal v1 )
    {
        return v1 + 2;
    }

    static qreal times3 ( qreal v1 )
    {
        return v1 * 3;
    }

    static qreal sqr ( qreal v1 )
    {
        return v1 * v1;
    }

    static qreal sign ( qreal v )
    {
        return -v;
    }

    static qreal add ( qreal v1, qreal v2 )
    {
        return v1 + v2;
    }

    static qreal land ( qreal v1, qreal v2 )
    {
        return static_cast<int>( v1 ) & static_cast<int>( v2 );
    }

    static qreal FirstArg ( const qreal* a_afArg, int a_iArgc )
    {
        if ( a_iArgc == 0)
        {
            throw QmuParserError ( "too few arguments for function FirstArg." );
        }

        return a_afArg[0];
    }

    static qreal LastArg ( const qreal* a_afArg, int a_iArgc )
    {
        if ( a_iArgc == 0)
        {
            throw QmuParserError ( "too few arguments for function LastArg." );
        }

        return a_afArg[a_iArgc - 1];
    }

    static qreal Sum ( const qreal* a_afArg, int a_iArgc )
    {
        if ( a_iArgc == 0)
        {
            throw QmuParserError ( "too few arguments for function sum." );
        }

        qreal fRes = 0;
        for ( int i = 0; i < a_iArgc; ++i )
        {
            fRes += a_afArg[i];
        }
        return fRes;
    }

    static qreal Rnd ( qreal v )
    {
        return static_cast<qreal>( ( 1 + ( v * qrand() / ( RAND_MAX + 1.0 ) ) ) );
    }

    static qreal RndWithString ( const char_type* )
    {
        return static_cast<qreal>( ( 1 + ( 1000.0f * static_cast<qreal>(qrand()) / ( RAND_MAX + 1.0 ) ) ) );
    }

    static qreal Ping()
    {
        return 10;
    }

    static qreal ValueOf ( const QString & )
    {
        return 123;
    }

    static qreal StrFun1 ( const QString & v1 )
    {
        int val = v1.toInt();
        return static_cast<qreal>(val);
    }

    static qreal StrFun2 ( const QString & v1, qreal v2 )
    {
        int val = v1.toInt();
        return static_cast<qreal>( val + v2 );
    }

    static qreal StrFun3 ( const QString & v1, qreal v2, qreal v3 )
    {
        int val = v1.toInt();
        return val + v2 + v3;
    }

    static qreal StrToFloat ( const QString & a_szMsg )
    {
        qreal val = a_szMsg.toDouble();
        return val;
    }

    // postfix operator callback
    static qreal Mega ( qreal a_fVal )
    {
        return a_fVal * static_cast<qreal>( 1e6 );
    }

    static qreal Micro ( qreal a_fVal )
    {
        return a_fVal * static_cast<qreal>( 1e-6 );
    }

    static qreal Milli ( qreal a_fVal )
    {
        return a_fVal / static_cast<qreal>( 1e3 );
    }

    // Custom value recognition
    static int IsHexVal (const QString &a_szExpr, int *a_iPos, qreal *a_fVal, const QLocale &locale,
                         const QChar &decimal, const QChar &thousand);

    // cppcheck-suppress functionStatic
    int TestNames();
    // cppcheck-suppress functionStatic
    int TestSyntax();
    // cppcheck-suppress functionStatic
    int TestMultiArg();
    // cppcheck-suppress functionStatic
    int TestPostFix();
    // cppcheck-suppress functionStatic
    int TestExpression();
    // cppcheck-suppress functionStatic
    int TestInfixOprt();
    // cppcheck-suppress functionStatic
    int TestBinOprt();
    // cppcheck-suppress functionStatic
    int TestVarConst();
    // cppcheck-suppress functionStatic
    int TestInterface();
    // cppcheck-suppress functionStatic
    int TestException();
    // cppcheck-suppress functionStatic
    int TestStrArg();
    // cppcheck-suppress functionStatic
    int TestIfThenElse();
    // cppcheck-suppress functionStatic
    int TestBulkMode();

    static void Abort();
};
} // namespace Test
} // namespace qmu

#endif
