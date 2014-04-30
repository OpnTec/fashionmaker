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

#ifndef QMUQPARSERBASE_H
#define QMUQPARSERBASE_H

#include <QStack>
#include <QString>
#include <QStringList>

#include "qmuparserdef.h"
#include "qmuparsertokenreader.h"
#include "qmuparserbytecode.h"
#include "qmuparsererror.h"


namespace qmu
{
    /**
     * @file
     * @brief This file contains the class definition of the qmuparser engine.
     */

    /**
     * @brief Mathematical expressions parser (base parser engine).
     * @author (C) 2013 Ingo Berg
     *
     * This is the implementation of a bytecode based mathematical expressions parser.
     * The formula will be parsed from string and converted into a bytecode.
     * Future calculations will be done with the bytecode instead the formula string
     * resulting in a significant performance increase.
     * Complementary to a set of internally implemented functions the parser is able to handle
     * user defined functions and variables.
     */
    class QmuParserBase
    {
        friend class QmuParserTokenReader;
    public:
        /**
         * @brief Type of the error class.
         *
         * Included for backwards compatibility.
         */
        typedef QmuParserError exception_type;

        QmuParserBase();
        QmuParserBase(const QmuParserBase &a_Parser);
        QmuParserBase& operator=(const QmuParserBase &a_Parser);
        virtual ~QmuParserBase();

        static void        EnableDebugDump(bool bDumpCmd, bool bDumpStack);
        qreal              Eval() const;
        qreal*             Eval(int &nStackSize) const;
        void               Eval(qreal *results, int nBulkSize);
        int                GetNumResults() const;
        void               SetExpr(const QString &a_sExpr);
        void               SetVarFactory(facfun_type a_pFactory, void *pUserData = NULL);
        void               SetDecSep(char_type cDecSep);
        void               SetThousandsSep(char_type cThousandsSep = 0);
        void               ResetLocale();
        void               EnableOptimizer(bool a_bIsOn=true);
        void               EnableBuiltInOprt(bool a_bIsOn=true);
        bool               HasBuiltInOprt() const;
        void               AddValIdent(identfun_type a_pCallback);
        void               DefineOprt(const QString &a_strName, fun_type2 a_pFun, unsigned a_iPri=0,
                                      EOprtAssociativity a_eAssociativity = oaLEFT, bool a_bAllowOpt = false);
        void               DefineConst(const QString &a_sName, qreal a_fVal);
        void               DefineStrConst(const QString &a_sName, const QString &a_strVal);
        void               DefineVar(const QString &a_sName, qreal *a_fVar);
        void               DefinePostfixOprt(const QString &a_strFun, fun_type1 a_pOprt, bool a_bAllowOpt=true);
        void               DefineInfixOprt(const QString &a_strName, fun_type1 a_pOprt, int a_iPrec=prINFIX,
                                           bool a_bAllowOpt=true);
        // Clear user defined variables, constants or functions
        void               ClearVar();
        void               ClearFun();
        void               ClearConst();
        void               ClearInfixOprt();
        void               ClearPostfixOprt();
        void               ClearOprt();
        void               RemoveVar(const QString &a_strVarName);
        const varmap_type& GetUsedVar() const;
        const varmap_type& GetVar() const;
        const valmap_type& GetConst() const;
        const QString&     GetExpr() const;
        const funmap_type& GetFunDef() const;
        QString            GetVersion(EParserVersionInfo eInfo = pviFULL) const;
        const QStringList& GetOprtDef() const;
        void               DefineNameChars(const QString &a_szCharset);
        void               DefineOprtChars(const QString &a_szCharset);
        void               DefineInfixOprtChars(const QString &a_szCharset);
        const QString&     ValidNameChars() const;
        const QString&     ValidOprtChars() const;
        const QString&     ValidInfixOprtChars() const;
        void               SetArgSep(char_type cArgSep);
        QChar              GetArgSep() const;
        void               Error(EErrorCodes a_iErrc, int a_iPos = -1, const QString &a_strTok = QString() ) const;
        /**
         * @fn void qmu::QmuParserBase::DefineFun(const string_type &a_strName, fun_type0 a_pFun,
         * bool a_bAllowOpt = true)
         * @brief Define a parser function without arguments.
         * @param a_strName Name of the function
         * @param a_pFun Pointer to the callback function
         * @param a_bAllowOpt A flag indicating this function may be optimized
         */
        template<typename T>
        void DefineFun(const QString &a_strName, T a_pFun, bool a_bAllowOpt = true)
        {
            AddCallback( a_strName, QmuParserCallback(a_pFun, a_bAllowOpt), m_FunDef, ValidNameChars() );
        }
    protected:
        static const QStringList c_DefaultOprt;
        static std::locale s_locale;  ///< The locale used by the parser
        static bool g_DbgDumpCmdCode;
        static bool g_DbgDumpStack;
        void Init();
        virtual void InitCharSets() = 0;
        virtual void InitFun() = 0;
        virtual void InitConst() = 0;
        virtual void InitOprt() = 0;
        virtual void OnDetectVar(QString *pExpr, int &nStart, int &nEnd);
        /**
         * @brief A facet class used to change decimal and thousands separator.
         */
        template<class TChar>
        class change_dec_sep : public std::numpunct<TChar>
        {
            public:
                explicit change_dec_sep(char_type cDecSep, char_type cThousandsSep = 0, int nGroup = 3)
                :std::numpunct<TChar>(), m_nGroup(nGroup), m_cDecPoint(cDecSep), m_cThousandsSep(cThousandsSep)
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
    private:
        /**
         * @brief Typedef for the parse functions.
         *
         * The parse function do the actual work. The parser exchanges
         * the function pointer to the parser function depending on
         * which state it is in. (i.e. bytecode parser vs. string parser)
         */
        typedef qreal (QmuParserBase::*ParseFunction)() const;

        /**
         * @brief Type used for storing an array of values.
         */
        typedef QVector<qreal> valbuf_type;

        /**
         * @brief Type for a vector of strings.
         */
        typedef QVector<QString> stringbuf_type;

        /**
         * @brief Typedef for the token reader.
         */
        typedef QmuParserTokenReader token_reader_type;

        /**
         * @brief Type used for parser tokens.
         */
        typedef QmuParserToken<qreal, QString> token_type;

        /**
         * @brief Maximum number of threads spawned by OpenMP when using the bulk mode.
         */
        static const int s_MaxNumOpenMPThreads = 4;

        /**
         * @brief Pointer to the parser function.
         *
         * Eval() calls the function whose address is stored there.
         */
        mutable ParseFunction     m_pParseFormula;
        mutable QmuParserByteCode m_vRPN;        ///< The Bytecode class.
        mutable stringbuf_type    m_vStringBuf; ///< String buffer, used for storing string function arguments
        stringbuf_type            m_vStringVarBuf;

        std::auto_ptr<token_reader_type> m_pTokenReader; ///< Managed pointer to the token reader object.

        funmap_type  m_FunDef;         ///< Map of function names and pointers.
        funmap_type  m_PostOprtDef;    ///< Postfix operator callbacks
        funmap_type  m_InfixOprtDef;   ///< unary infix operator.
        funmap_type  m_OprtDef;        ///< Binary operator callbacks
        valmap_type  m_ConstDef;       ///< user constants.
        strmap_type  m_StrVarDef;      ///< user defined string constants
        varmap_type  m_VarDef;         ///< user defind variables.

        bool m_bBuiltInOp;             ///< Flag that can be used for switching built in operators on and off

        QString m_sNameChars;      ///< Charset for names
        QString m_sOprtChars;      ///< Charset for postfix/ binary operator tokens
        QString m_sInfixOprtChars; ///< Charset for infix operator tokens

        mutable int m_nIfElseCounter;  ///< Internal counter for keeping track of nested if-then-else clauses

        // items merely used for caching state information
        mutable valbuf_type m_vStackBuffer; ///< This is merely a buffer used for the stack in the cmd parsing routine
        mutable int m_nFinalResultIdx;

        void               Assign(const QmuParserBase &a_Parser);
        void               InitTokenReader();
        void               ReInit() const;
        void               AddCallback(const QString &a_strName, const QmuParserCallback &a_Callback,
                                       funmap_type &a_Storage, const QString &a_szCharSet );
        void               ApplyRemainingOprt(QStack<token_type> &a_stOpt, QStack<token_type> &a_stVal) const;
        void               ApplyBinOprt(QStack<token_type> &a_stOpt, QStack<token_type> &a_stVal) const;
        void               ApplyIfElse(QStack<token_type> &a_stOpt, QStack<token_type> &a_stVal) const;
        void               ApplyFunc(QStack<token_type> &a_stOpt, QStack<token_type> &a_stVal, int iArgCount) const;
        token_type         ApplyStrFunc(const token_type &a_FunTok, const QVector<token_type> &a_vArg) const;
        int                GetOprtPrecedence(const token_type &a_Tok) const;
        EOprtAssociativity GetOprtAssociativity(const token_type &a_Tok) const;
        void               CreateRPN() const;
        qreal              ParseString() const;
        qreal              ParseCmdCode() const;
        qreal              ParseCmdCodeBulk(int nOffset, int nThreadID) const;
        void               CheckName(const QString &a_strName, const QString &a_CharSet) const;
        void               CheckOprt(const QString &a_sName, const QmuParserCallback &a_Callback,
                                     const QString &a_szCharSet) const;
        void               StackDump(const QStack<token_type > &a_stVal, const QStack<token_type > &a_stOprt) const;
    };

} // namespace qmu

#endif

