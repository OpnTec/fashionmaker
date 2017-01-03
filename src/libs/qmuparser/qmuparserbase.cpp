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

#include "qmuparserbase.h"

#include <QList>
#include <QMessageLogger>
#include <QStack>
#include <QStringList>
#include <QTextStream>
#include <QtDebug>
#include <map>
#ifdef QMUP_USE_OPENMP
    #include <omp.h>
#endif
#include <assert.h>

#include "qmudef.h"
#include "../vmisc/vmath.h"

/**
 * @file
 * @brief This file contains the basic implementation of the muparser engine.
 */

namespace qmu
{

bool QmuParserBase::g_DbgDumpCmdCode = false;
bool QmuParserBase::g_DbgDumpStack = false;

/**
 * @brief Identifiers for built in binary operators.
 *
 * When defining custom binary operators with #AddOprt(...) make sure not to choose
 * names conflicting with these definitions.
 */
const QStringList QmuParserBase::c_DefaultOprt = QStringList()<< "<=" << ">=" << "!=" << "==" << "<"  << ">"  << "+"
                                                              << "-"  << "*"  << "/"  << "^"  << "&&" << "||" << "="
                                                              << "("  << ")"  << "?"  << ":";

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Constructor.
 */
QmuParserBase::QmuParserBase()
    : m_locale(QLocale::c()),
      m_decimalPoint(QLocale::c().decimalPoint()),
      m_thousandsSeparator(QLocale::c().groupSeparator()),
      m_pParseFormula(&QmuParserBase::ParseString),
      m_vRPN(),
      m_vStringBuf(),
      m_vStringVarBuf(),
      m_pTokenReader(),
      m_FunDef(),
      m_PostOprtDef(),
      m_InfixOprtDef(),
      m_OprtDef(),
      m_ConstDef(),
      m_StrVarDef(),
      m_VarDef(),
      m_bBuiltInOp(true),
      m_sNameChars(),
      m_sOprtChars(),
      m_sInfixOprtChars(),
      m_nIfElseCounter(0),
      m_vStackBuffer(),
      m_nFinalResultIdx(0),
      m_Tokens(QMap<int, QString>()),
      m_Numbers(QMap<int, QString>()),
      allowSubexpressions(true)
{
    InitTokenReader();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Copy constructor.
 *
 * Tha parser can be safely copy constructed but the bytecode is reset during copy construction.
 */
QmuParserBase::QmuParserBase(const QmuParserBase &a_Parser)
    : m_locale(a_Parser.getLocale()),
      m_decimalPoint(a_Parser.getDecimalPoint()),
      m_thousandsSeparator(a_Parser.getThousandsSeparator()),
      m_pParseFormula(&QmuParserBase::ParseString),
      m_vRPN(),
      m_vStringBuf(),
      m_vStringVarBuf(),
      m_pTokenReader(),
      m_FunDef(),
      m_PostOprtDef(),
      m_InfixOprtDef(),
      m_OprtDef(),
      m_ConstDef(),
      m_StrVarDef(),
      m_VarDef(),
      m_bBuiltInOp(true),
      m_sNameChars(),
      m_sOprtChars(),
      m_sInfixOprtChars(),
      m_nIfElseCounter(0),
      m_vStackBuffer(),
      m_nFinalResultIdx(0),
      m_Tokens(QMap<int, QString>()),
      m_Numbers(QMap<int, QString>()),
      allowSubexpressions(true)
{
    m_pTokenReader.reset(new token_reader_type(this));
    Assign(a_Parser);
}

//---------------------------------------------------------------------------------------------------------------------
QmuParserBase::~QmuParserBase()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Assignement operator.
 *
 * Implemented by calling Assign(a_Parser). Self assignement is suppressed.
 * @param a_Parser Object to copy to this.
 * @return *this
 * @throw nothrow
 */
QmuParserBase& QmuParserBase::operator=(const QmuParserBase &a_Parser)
{
    Assign(a_Parser);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Copy state of a parser object to this.
 *
 * Clears Variables and Functions of this parser.
 * Copies the states of all internal variables.
 * Resets parse function to string parse mode.
 *
 * @param a_Parser the source object.
 */
void QmuParserBase::Assign(const QmuParserBase &a_Parser)
{
    if (&a_Parser==this)
    {
      return;
    }

    // Don't copy bytecode instead cause the parser to create new bytecode
    // by resetting the parse function.
    ReInit();

    m_ConstDef        = a_Parser.m_ConstDef;         // Copy user define constants
    m_VarDef          = a_Parser.m_VarDef;           // Copy user defined variables
    m_bBuiltInOp      = a_Parser.m_bBuiltInOp;
    m_vStringBuf      = a_Parser.m_vStringBuf;
    m_vStackBuffer    = a_Parser.m_vStackBuffer;
    m_nFinalResultIdx = a_Parser.m_nFinalResultIdx;
    m_StrVarDef       = a_Parser.m_StrVarDef;
    m_vStringVarBuf   = a_Parser.m_vStringVarBuf;
    m_nIfElseCounter  = a_Parser.m_nIfElseCounter;
    m_pTokenReader.reset(a_Parser.m_pTokenReader->Clone(this));

    // Copy function and operator callbacks
    m_FunDef          = a_Parser.m_FunDef;             // Copy function definitions
    m_PostOprtDef     = a_Parser.m_PostOprtDef;   // post value unary operators
    m_InfixOprtDef    = a_Parser.m_InfixOprtDef; // unary operators for infix notation
    m_OprtDef         = a_Parser.m_OprtDef;           // binary operators

    m_sNameChars      = a_Parser.m_sNameChars;
    m_sOprtChars      = a_Parser.m_sOprtChars;
    m_sInfixOprtChars = a_Parser.m_sInfixOprtChars;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Resets the locale.
 *
 * The default locale used "." as decimal separator, no thousands separator and "," as function argument separator.
 */
// cppcheck-suppress unusedFunction
void QmuParserBase::ResetLocale()
{
    setLocale(QLocale::c());
    m_decimalPoint = m_locale.decimalPoint();
    m_thousandsSeparator = m_locale.groupSeparator();
    SetArgSep(';');
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Reset parser to string parsing mode and clear internal buffers.
 *
 * Clear bytecode, reset the token reader.
 * @throw nothrow
 */
void QmuParserBase::ReInit() const
{
    m_pParseFormula = &QmuParserBase::ParseString;
    m_vStringBuf.clear();
    m_vRPN.clear();
    m_pTokenReader->ReInit();
    m_nIfElseCounter = 0;
    m_Tokens.clear();
    m_Numbers.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParserBase::OnDetectVar(const QString &pExpr, int &nStart, int &nEnd)
{
    Q_UNUSED(pExpr)
    Q_UNUSED(nStart)
    Q_UNUSED(nEnd)
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParserBase::setAllowSubexpressions(bool value)
{
    allowSubexpressions = value;
}

//---------------------------------------------------------------------------------------------------------------------
QLocale QmuParserBase::getLocale() const
{
    return m_locale;
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParserBase::setLocale(const QLocale &value)
{
    m_locale = value;
    InitCharSets();
    InitOprt();
}

//---------------------------------------------------------------------------------------------------------------------
QChar QmuParserBase::getDecimalPoint() const
{
    return m_decimalPoint;
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParserBase::setDecimalPoint(const QChar &c)
{
    m_decimalPoint = c;
}

//---------------------------------------------------------------------------------------------------------------------
QChar QmuParserBase::getThousandsSeparator() const
{
    return m_thousandsSeparator;
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParserBase::setThousandsSeparator(const QChar &c)
{
    m_thousandsSeparator = c;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the version of muparser.
 * @param eInfo A flag indicating whether the full version info should be returned or not.
 *
 * Format is as follows: "MAJOR.MINOR (COMPILER_FLAGS)" The COMPILER_FLAGS are returned only if eInfo==pviFULL.
 */
// cppcheck-suppress unusedFunction
QString QmuParserBase::GetVersion(EParserVersionInfo eInfo)
{
    QString versionInfo;
    QTextStream ss(&versionInfo);

    ss << QMUP_VERSION;

    if (eInfo==pviFULL)
    {
        ss << " (" << QMUP_VERSION_DATE;
        ss << "; " << sizeof(void*)*8 << "BIT";

    #ifdef _DEBUG
        ss << "; DEBUG";
    #else
        ss << "; RELEASE";
    #endif

    #ifdef _UNICODE
        ss << "; UNICODE";
    #else
    #ifdef _MBCS
        ss << "; MBCS";
    #else
        ss << "; ASCII";
    #endif
    #endif

    #ifdef QMUP_USE_OPENMP
        ss << "; OPENMP";
    //#else
    //  ss << "; NO_OPENMP";
    #endif

    #if defined(MUP_MATH_EXCEPTIONS)
        ss << "; MATHEXC";
    //#else
    //  ss << "; NO_MATHEXC";
    #endif

        ss << ")";
    }
    return versionInfo;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a function or operator callback to the parser.
 */
void QmuParserBase::AddCallback(const QString &a_strName, const QmuParserCallback &a_Callback,
                                funmap_type &a_Storage, const QString &a_szCharSet )
{
    if (a_Callback.GetAddr()==0)
    {
        Error(ecINVALID_FUN_PTR);
    }

    const funmap_type *pFunMap = &a_Storage;

    // Check for conflicting operator or function names
    if ( pFunMap!=&m_FunDef && m_FunDef.find(a_strName)!=m_FunDef.end() )
    {
        Error(ecNAME_CONFLICT, -1, a_strName);
    }

    if ( pFunMap!=&m_PostOprtDef && m_PostOprtDef.find(a_strName)!=m_PostOprtDef.end() )
    {
        Error(ecNAME_CONFLICT, -1, a_strName);
    }

    if ( pFunMap!=&m_InfixOprtDef && pFunMap!=&m_OprtDef && m_InfixOprtDef.find(a_strName)!=m_InfixOprtDef.end() )
    {
        Error(ecNAME_CONFLICT, -1, a_strName);
    }

    if ( pFunMap!=&m_InfixOprtDef && pFunMap!=&m_OprtDef && m_OprtDef.find(a_strName)!=m_OprtDef.end() )
    {
        Error(ecNAME_CONFLICT, -1, a_strName);
    }

    CheckOprt(a_strName, a_Callback, a_szCharSet);
    a_Storage[a_strName] = a_Callback;
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Check if a name contains invalid characters.
 *
 * @throw ParserException if the name contains invalid charakters.
 */
// cppcheck-suppress
void QmuParserBase::CheckOprt(const QString &a_sName, const QmuParserCallback &a_Callback,
                              const QString &a_szCharSet) const
{
    const std::wstring a_sNameStd = a_sName.toStdWString();
    const std::wstring a_szCharSetStd = a_szCharSet.toStdWString();

    if ( a_sNameStd.length() == false || (a_sNameStd.find_first_not_of(a_szCharSetStd)!=string_type::npos) ||
         (a_sNameStd.at(0)>='0' && a_sNameStd.at(0)<='9'))
    {
        switch (a_Callback.GetCode())
        {
            case cmOPRT_POSTFIX:
                Error(ecINVALID_POSTFIX_IDENT, -1, a_sName);
                break;
            case cmOPRT_INFIX:
                Error(ecINVALID_INFIX_IDENT, -1, a_sName);
                break;
            default:
                Error(ecINVALID_NAME, -1, a_sName);
                break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Check if a name contains invalid characters.
 *
 * @throw ParserException if the name contains invalid charakters.
 */
// cppcheck-suppress
void QmuParserBase::CheckName(const QString &a_sName, const QString &a_szCharSet) const
{
    std::wstring a_sNameStd = a_sName.toStdWString();
    std::wstring a_szCharSetStd = a_szCharSet.toStdWString();

    if ( a_sNameStd.length() == false || (a_sNameStd.find_first_not_of(a_szCharSetStd)!=string_type::npos) ||
         (a_sNameStd[0]>='0' && a_sNameStd[0]<='9'))
    {
        Error(ecINVALID_NAME);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Set the formula.
 * @param a_sExpr Formula as string_type
 * @throw ParserException in case of syntax errors.
 *
 * Triggers first time calculation thus the creation of the bytecode and scanning of used variables.
 */
void QmuParserBase::SetExpr(const QString &a_sExpr)
{
    // Check locale compatibility
    std::locale loc;
    if (m_pTokenReader->GetArgSep()==std::use_facet<std::numpunct<char_type> >(loc).decimal_point())
    {
        Error(ecLOCALE);
    }

    // <ibg> 20060222: Bugfix for Borland-Kylix:
    // adding a space to the expression will keep Borlands KYLIX from going wild
    // when calling tellg on a stringstream created from the expression after
    // reading a value at the end of an expression. (qmu::QmuParser::IsVal function)
    // (tellg returns -1 otherwise causing the parser to ignore the value)
    QString sBuf(a_sExpr + " " );
    m_pTokenReader->SetFormula(sBuf);
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Virtual function that defines the characters allowed in name identifiers.
 * @sa #ValidOprtChars, #ValidPrefixOprtChars
 */
const QString& QmuParserBase::ValidNameChars() const
{
    assert(m_sNameChars.size());
    return m_sNameChars;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Virtual function that defines the characters allowed in operator definitions.
 * @sa #ValidNameChars, #ValidPrefixOprtChars
 */
const QString &QmuParserBase::ValidOprtChars() const
{
    assert(m_sOprtChars.size());
    return m_sOprtChars;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Virtual function that defines the characters allowed in infix operator definitions.
 * @sa #ValidNameChars, #ValidOprtChars
 */
const QString &QmuParserBase::ValidInfixOprtChars() const
{
    assert(m_sInfixOprtChars.size());
    return m_sInfixOprtChars;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a user defined operator.
 * @post Will reset the Parser to string parsing mode.
 */
void QmuParserBase::DefinePostfixOprt(const QString &a_sName, fun_type1 a_pFun, bool a_bAllowOpt)
{
    AddCallback(a_sName, QmuParserCallback(a_pFun, a_bAllowOpt, prPOSTFIX, cmOPRT_POSTFIX), m_PostOprtDef,
                ValidOprtChars() );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Initialize user defined functions.
 *
 * Calls the virtual functions InitFun(), InitConst() and InitOprt().
 */
// cppcheck-suppress unusedFunction
void QmuParserBase::Init()
{
    InitCharSets();
    InitFun();
    InitConst();
    InitOprt();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a user defined operator.
 * @post Will reset the Parser to string parsing mode.
 * @param [in] a_sName  operator Identifier
 * @param [in] a_pFun  Operator callback function
 * @param [in] a_iPrec  Operator Precedence (default=prSIGN)
 * @param [in] a_bAllowOpt  True if operator is volatile (default=false)
 * @sa EPrec
 */
void QmuParserBase::DefineInfixOprt(const QString &a_sName, fun_type1 a_pFun, int a_iPrec, bool a_bAllowOpt)
{
    AddCallback(a_sName, QmuParserCallback(a_pFun, a_bAllowOpt, a_iPrec, cmOPRT_INFIX), m_InfixOprtDef,
                ValidInfixOprtChars() );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Define a binary operator.
 * @param [in] a_sName The identifier of the operator.
 * @param [in] a_pFun Pointer to the callback function.
 * @param [in] a_iPrec Precedence of the operator.
 * @param [in] a_eAssociativity The associativity of the operator.
 * @param [in] a_bAllowOpt If this is true the operator may be optimized away.
 *
 * Adds a new Binary operator the the parser instance.
 */
void QmuParserBase::DefineOprt( const QString &a_sName, fun_type2 a_pFun, unsigned a_iPrec,
                                EOprtAssociativity a_eAssociativity, bool a_bAllowOpt )
{
    // Check for conflicts with built in operator names
    for (int i=0; m_bBuiltInOp && i<cmENDIF; ++i)
    {
        if (a_sName == c_DefaultOprt.at(i))
        {
            Error(ecBUILTIN_OVERLOAD, -1, a_sName);
        }
    }

    AddCallback(a_sName, QmuParserCallback(a_pFun, a_bAllowOpt, static_cast<int>(a_iPrec), a_eAssociativity), m_OprtDef,
                ValidOprtChars() );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Define a new string constant.
 * @param [in] a_strName The name of the constant.
 * @param [in] a_strVal the value of the constant.
 */
// cppcheck-suppress unusedFunction
void QmuParserBase::DefineStrConst(const QString &a_strName, const QString &a_strVal)
{
    // Test if a constant with that names already exists
    if (m_StrVarDef.find(a_strName)!=m_StrVarDef.end())
    {
        Error(ecNAME_CONFLICT);
    }

    CheckName(a_strName, ValidNameChars());

    m_vStringVarBuf.push_back(a_strVal);           // Store variable string in internal buffer
    m_StrVarDef[a_strName] = m_vStringBuf.size();  // bind buffer index to variable name

    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a user defined variable.
 * @param [in] a_sName the variable name
 * @param [in] a_pVar A pointer to the variable vaule.
 * @post Will reset the Parser to string parsing mode.
 * @throw ParserException in case the name contains invalid signs or a_pVar is NULL.
 */
void QmuParserBase::DefineVar(const QString &a_sName, qreal *a_pVar)
{
    if (a_pVar==0)
    {
        Error(ecINVALID_VAR_PTR);
    }

    // Test if a constant with that names already exists
    if (m_ConstDef.find(a_sName)!=m_ConstDef.end())
    {
        Error(ecNAME_CONFLICT);
    }

    CheckName(a_sName, ValidNameChars());
    m_VarDef[a_sName] = a_pVar;
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a user defined constant.
 * @param [in] a_sName The name of the constant.
 * @param [in] a_fVal the value of the constant.
 * @post Will reset the Parser to string parsing mode.
 * @throw ParserException in case the name contains invalid signs.
 */
void QmuParserBase::DefineConst(const QString &a_sName, qreal a_fVal)
{
    CheckName(a_sName, ValidNameChars());
    m_ConstDef[a_sName] = a_fVal;
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Get operator priority.
 * @throw ParserException if a_Oprt is no operator code
 */
int QmuParserBase::GetOprtPrecedence(const token_type &a_Tok) const
{
    switch (a_Tok.GetCode())
    {
        // built in operators
        case cmEND:
            return -5;
        case cmARG_SEP:
            return -4;
        case cmASSIGN:
            return -1;
        case cmELSE:
        case cmIF:
            return 0;
        case cmLAND:
            return prLAND;
        case cmLOR:
            return prLOR;
        case cmLT:
        case cmGT:
        case cmLE:
        case cmGE:
        case cmNEQ:
        case cmEQ:
            return prCMP;
        case cmADD:
        case cmSUB:
            return prADD_SUB;
        case cmMUL:
        case cmDIV:
            return prMUL_DIV;
        case cmPOW:
            return prPOW;
        // user defined binary operators
        case cmOPRT_INFIX:
        case cmOPRT_BIN:
            return a_Tok.GetPri();
        default:
            Error(ecINTERNAL_ERROR, 5);
            return 999;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Get operator priority.
 * @throw ParserException if a_Oprt is no operator code
 */
EOprtAssociativity QmuParserBase::GetOprtAssociativity(const token_type &a_Tok) const
{
    switch (a_Tok.GetCode())
    {
        case cmASSIGN:
        case cmLAND:
        case cmLOR:
        case cmLT:
        case cmGT:
        case cmLE:
        case cmGE:
        case cmNEQ:
        case cmEQ:
        case cmADD:
        case cmSUB:
        case cmMUL:
        case cmDIV:
            return oaLEFT;
        case cmPOW:
            return oaRIGHT;
        case cmOPRT_BIN:
            return a_Tok.GetAssociativity();
        default:
            return oaNONE;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return a map containing the used variables only.
 */
const varmap_type& QmuParserBase::GetUsedVar() const
{
    try
    {
        m_pTokenReader->IgnoreUndefVar(true);
        CreateRPN(); // try to create bytecode, but don't use it for any further calculations since it
                     // may contain references to nonexisting variables.
        m_pParseFormula = &QmuParserBase::ParseString;
        m_pTokenReader->IgnoreUndefVar(false);
    }
    catch (const QmuParserError &e)
    {
        Q_UNUSED(e)
        // Make sure to stay in string parse mode, dont call ReInit()
        // because it deletes the array with the used variables
        m_pParseFormula = &QmuParserBase::ParseString;
        m_pTokenReader->IgnoreUndefVar(false);
        throw;
    }
    return m_pTokenReader->GetUsedVar();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Execute a function that takes a single string argument.
 * @param a_FunTok Function token.
 * @throw QmuParserError If the function token is not a string function
 */
QmuParserBase::token_type QmuParserBase::ApplyStrFunc(const token_type &a_FunTok,
                                                      const QVector<token_type> &a_vArg) const
{
    if (a_vArg.back().GetCode()!=cmSTRING)
    {
        Error(ecSTRING_EXPECTED, m_pTokenReader->GetPos(), a_FunTok.GetAsString());
    }

    token_type  valTok;
    generic_fun_type pFunc = a_FunTok.GetFuncAddr();
    assert(pFunc);

    try
    {
        // Check function arguments; write dummy value into valtok to represent the result
        switch (a_FunTok.GetArgCount())
        {
            case 0:
                valTok.SetVal(1);
                a_vArg[0].GetAsString();
                break;
            case 1:
                valTok.SetVal(1);
                a_vArg[1].GetAsString();
                a_vArg[0].GetVal();
                break;
            case 2:
                valTok.SetVal(1);
                a_vArg[2].GetAsString();
                a_vArg[1].GetVal();
                a_vArg[0].GetVal();
                break;
            default:
                Error(ecINTERNAL_ERROR);
                break;
        }
    }
    catch (QmuParserError& )
    {
        Error(ecVAL_EXPECTED, m_pTokenReader->GetPos(), a_FunTok.GetAsString());
    }

    // string functions won't be optimized
    m_vRPN.AddStrFun(pFunc, a_FunTok.GetArgCount(), a_vArg.back().GetIdx());

    // Push dummy value representing the function result to the stack
    return valTok;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Apply a function token.
 * @param a_iArgCount Number of Arguments actually gathered used only for multiarg functions.
 * @post The result is pushed to the value stack
 * @post The function token is removed from the stack
 * @throw QmuParserError if Argument count does not mach function requirements.
 */
void QmuParserBase::ApplyFunc( QStack<token_type> &a_stOpt, QStack<token_type> &a_stVal, int a_iArgCount) const
{
    assert(m_pTokenReader.get());

    // Operator stack empty or does not contain tokens with callback functions
    if (a_stOpt.empty() || a_stOpt.top().GetFuncAddr()==0 )
    {
        return;
    }

    token_type funTok = a_stOpt.pop();
    assert(funTok.GetFuncAddr());

    // Binary operators must rely on their internal operator number
    // since counting of operators relies on commas for function arguments
    // binary operators do not have commas in their expression
    int iArgCount = (funTok.GetCode()==cmOPRT_BIN) ? funTok.GetArgCount() : a_iArgCount;

    // determine how many parameters the function needs. To remember iArgCount includes the
    // string parameter whilst GetArgCount() counts only numeric parameters.
    int iArgRequired = funTok.GetArgCount() + ((funTok.GetType()==tpSTR) ? 1 : 0);

    // Thats the number of numerical parameters
    int iArgNumerical = iArgCount - ((funTok.GetType()==tpSTR) ? 1 : 0);

    if (funTok.GetCode()==cmFUNC_STR && iArgCount-iArgNumerical>1)
    {
        Error(ecINTERNAL_ERROR);
    }

    if (funTok.GetArgCount()>=0 && iArgCount>iArgRequired)
    {
        Error(ecTOO_MANY_PARAMS, m_pTokenReader->GetPos()-1, funTok.GetAsString());
    }

    if (funTok.GetCode()!=cmOPRT_BIN && iArgCount<iArgRequired )
    {
        Error(ecTOO_FEW_PARAMS, m_pTokenReader->GetPos()-1, funTok.GetAsString());
    }

    if (funTok.GetCode()==cmFUNC_STR && iArgCount>iArgRequired )
    {
        Error(ecTOO_MANY_PARAMS, m_pTokenReader->GetPos()-1, funTok.GetAsString());
    }

    // Collect the numeric function arguments from the value stack and store them
    // in a vector
    QVector<token_type> stArg;
    for (int i=0; i<iArgNumerical; ++i)
    {
        if (a_stVal.isEmpty())// Check if stack is empty like in origin muparser.
        {
            Error(ecUNASSIGNABLE_TOKEN, m_pTokenReader->GetPos(), funTok.GetAsString());
        }
        stArg.push_back( a_stVal.pop() );
        if ( stArg.back().GetType()==tpSTR && funTok.GetType()!=tpSTR )
        {
            Error(ecVAL_EXPECTED, m_pTokenReader->GetPos(), funTok.GetAsString());
        }
    }

    switch (funTok.GetCode())
    {
        case cmFUNC_STR:
            stArg.push_back(a_stVal.pop());

            if ( stArg.back().GetType()==tpSTR && funTok.GetType()!=tpSTR )
            {
                Error(ecVAL_EXPECTED, m_pTokenReader->GetPos(), funTok.GetAsString());
            }

            ApplyStrFunc(funTok, stArg);
            break;
        case cmFUNC_BULK:
            m_vRPN.AddBulkFun(funTok.GetFuncAddr(), stArg.size());
            break;
        case cmOPRT_BIN:
        case cmOPRT_POSTFIX:
        case cmOPRT_INFIX:
        case cmFUNC:
            if (funTok.GetArgCount()==-1 && iArgCount==0)
            {
                Error(ecTOO_FEW_PARAMS, m_pTokenReader->GetPos(), funTok.GetAsString());
            }

            m_vRPN.AddFun(funTok.GetFuncAddr(), (funTok.GetArgCount()==-1) ? -iArgNumerical : iArgNumerical);
            break;
        default:
            break;
    }
    // Push dummy value representing the function result to the stack
    token_type token;
    token.SetVal(1);
    a_stVal.push(token);
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParserBase::ApplyIfElse(QStack<token_type> &a_stOpt, QStack<token_type> &a_stVal) const
{
    // Check if there is an if Else clause to be calculated
    while (a_stOpt.size() && a_stOpt.top().GetCode()==cmELSE)
    {
        token_type opElse = a_stOpt.pop();
        Q_ASSERT(a_stOpt.size()>0);

        // Take the value associated with the else branch from the value stack
        token_type vVal2 = a_stVal.pop();

        Q_ASSERT(a_stOpt.size()>0);
        Q_ASSERT(a_stVal.size()>=2);

        // it then else is a ternary operator Pop all three values from the value s
        // tack and just return the right value
        token_type vVal1 = a_stVal.pop();
        token_type vExpr = a_stVal.pop();

        a_stVal.push( not qFuzzyIsNull(vExpr.GetVal()) ? vVal1 : vVal2);

        token_type opIf = a_stOpt.pop();
        Q_ASSERT(opElse.GetCode()==cmELSE);
        Q_ASSERT(opIf.GetCode()==cmIF);

        m_vRPN.AddIfElse(cmENDIF);
    } // while pending if-else-clause found
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Performs the necessary steps to write code for the execution of binary operators into the bytecode.
 */
void QmuParserBase::ApplyBinOprt(QStack<token_type> &a_stOpt, QStack<token_type> &a_stVal) const
{
    // is it a user defined binary operator?
    if (a_stOpt.top().GetCode()==cmOPRT_BIN)
    {
        ApplyFunc(a_stOpt, a_stVal, 2);
    }
    else
    {
        Q_ASSERT(a_stVal.size()>=2);
        token_type valTok1 = a_stVal.pop(),
                   valTok2 = a_stVal.pop(),
                   optTok  = a_stOpt.pop(),
                   resTok;

        if ( valTok1.GetType()!=valTok2.GetType() || (valTok1.GetType()==tpSTR && valTok2.GetType()==tpSTR) )
        {
            Error(ecOPRT_TYPE_CONFLICT, m_pTokenReader->GetPos(), optTok.GetAsString());
        }

        if (optTok.GetCode()==cmASSIGN)
        {
            if (valTok2.GetCode()!=cmVAR)
            {
                Error(ecUNEXPECTED_OPERATOR, -1, "=");
            }
            m_vRPN.AddAssignOp(valTok2.GetVar());
        }
        else
        {
            m_vRPN.AddOp(optTok.GetCode());
        }
        resTok.SetVal(1);
        a_stVal.push(resTok);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Apply a binary operator.
 * @param stOpt The operator stack
 * @param stVal The value stack
 */
void QmuParserBase::ApplyRemainingOprt(QStack<token_type> &stOpt, QStack<token_type> &stVal) const
{
    while (stOpt.size() && stOpt.top().GetCode() != cmBO && stOpt.top().GetCode() != cmIF)
    {
        const ECmdCode code = stOpt.top().GetCode();

        if ((code >= cmLE && code <= cmASSIGN) || code == cmOPRT_INFIX || code == cmOPRT_BIN)
        {
            if (code==cmOPRT_INFIX)
            {
                ApplyFunc(stOpt, stVal, 1);
            }
            else
            {
                ApplyBinOprt(stOpt, stVal);
            }
        }
        else if (code == cmELSE)
        {
            ApplyIfElse(stOpt, stVal);
        }
        else
        {
            Error(ecINTERNAL_ERROR);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Parse the command code.
 * @sa ParseString(...)
 *
 * Command code contains precalculated stack positions of the values and the associated operators. The Stack is
 * filled beginning from index one the value at index zero is not used at all.
 */
qreal QmuParserBase::ParseCmdCode() const
{
    return ParseCmdCodeBulk(0, 0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Evaluate the RPN.
 * @param nOffset The offset added to variable addresses (for bulk mode)
 * @param nThreadID OpenMP Thread id of the calling thread
 */
qreal QmuParserBase::ParseCmdCodeBulk(int nOffset, int nThreadID) const
{
    assert(nThreadID<=s_MaxNumOpenMPThreads);

    // Note: The check for nOffset==0 and nThreadID here is not necessary but
    //       brings a minor performance gain when not in bulk mode.
    qreal *Stack = ((nOffset==0) && (nThreadID==0)) ? &m_vStackBuffer[0] : &m_vStackBuffer[nThreadID *
            (m_vStackBuffer.size() / s_MaxNumOpenMPThreads)];
    qreal buf;
    int sidx(0);
    for (const SToken *pTok = m_vRPN.GetBase(); pTok->Cmd!=cmEND ; ++pTok)
    {
        switch (pTok->Cmd)
        {
          // built in binary operators
            case cmLE:
                --sidx;
                Stack[sidx]  = Stack[sidx] <= Stack[sidx+1];
                continue;
            case cmGE:
                --sidx;
                Stack[sidx]  = Stack[sidx] >= Stack[sidx+1];
                continue;
            case cmNEQ:
                --sidx;
                Stack[sidx]  = not QmuFuzzyComparePossibleNulls(Stack[sidx], Stack[sidx+1]);
                continue;
            case cmEQ:
                --sidx;
                Stack[sidx]  = QmuFuzzyComparePossibleNulls(Stack[sidx], Stack[sidx+1]);
                continue;
            case cmLT:
                --sidx;
                Stack[sidx]  = Stack[sidx] < Stack[sidx+1];
                continue;
            case cmGT:
                --sidx;
                Stack[sidx]  = Stack[sidx] > Stack[sidx+1];
                continue;
            case cmADD:
                --sidx;
                Stack[sidx] += Stack[1+sidx];
                continue;
            case cmSUB:
                --sidx;
                Stack[sidx] -= Stack[1+sidx];
                continue;
            case cmMUL:
                --sidx;
                Stack[sidx] *= Stack[1+sidx];
                continue;
            case cmDIV:
                --sidx;
    #if defined(MUP_MATH_EXCEPTIONS)
                if (Stack[1+sidx]==0)
                {
                    Error(ecDIV_BY_ZERO);
                }
    #endif
                Stack[sidx] /= Stack[1+sidx];
                continue;
            case cmPOW:
                --sidx;
                Stack[sidx] = qPow(Stack[sidx], Stack[1+sidx]);
                continue;
            case cmLAND:
                --sidx;
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wfloat-equal")
                Stack[sidx] = static_cast<bool>(Stack[sidx]) && static_cast<bool>(Stack[sidx+1]);
QT_WARNING_POP
                continue;
            case cmLOR:
                --sidx;
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wfloat-equal")
                Stack[sidx] = static_cast<bool>(Stack[sidx]) || static_cast<bool>(Stack[sidx+1]);
QT_WARNING_POP
                continue;
            case cmASSIGN:
                // Bugfix for Bulkmode:
                // for details see:
                // https://groups.google.com/forum/embed/?place=forum/muparser-dev&showsearch=true&showpopout=true&
                // showtabs=false&parenturl=http://muparser.beltoforion.de/mup_forum.html&afterlogin&pli=1#!topic/
                // muparser-dev/szgatgoHTws
                --sidx;
                Stack[sidx] = *(pTok->Oprt.ptr + nOffset) = Stack[sidx + 1];
                continue;
                // original code:
                //--sidx;
                //Stack[sidx] = *pTok->Oprt.ptr = Stack[sidx+1];
                //continue;
            case cmIF:
                if (qFuzzyIsNull(Stack[sidx--]))
                {
                    pTok += pTok->Oprt.offset;
                }
                continue;
            case cmELSE:
                pTok += pTok->Oprt.offset;
                continue;
            case cmENDIF:
                continue;

            // value and variable tokens
            case cmVAR:
                Stack[++sidx] = *(pTok->Val.ptr + nOffset);
                continue;
            case cmVAL:
                Stack[++sidx] =  pTok->Val.data2;
                continue;
            case cmVARPOW2:
                buf = *(pTok->Val.ptr + nOffset);
                Stack[++sidx] = buf*buf;
                continue;
            case cmVARPOW3:
                buf = *(pTok->Val.ptr + nOffset);
                Stack[++sidx] = buf*buf*buf;
                continue;
            case cmVARPOW4:
                buf = *(pTok->Val.ptr + nOffset);
                Stack[++sidx] = buf*buf*buf*buf;
                continue;
            case cmVARMUL:
                Stack[++sidx] = *(pTok->Val.ptr + nOffset) * pTok->Val.data + pTok->Val.data2;
                continue;
            // Next is treatment of numeric functions
            case cmFUNC:
            {
                int iArgCount = pTok->Fun.argc;

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wundefined-reinterpret-cast")
QT_WARNING_DISABLE_MSVC(4191)

                // switch according to argument count
                switch (iArgCount)
                {
                    case 0:
                        sidx += 1;
                        Stack[sidx] = (*reinterpret_cast<fun_type0>(pTok->Fun.ptr))();
                        continue;
                    case 1:
                        Stack[sidx] = (*reinterpret_cast<fun_type1>(pTok->Fun.ptr))(Stack[sidx]);
                        continue;
                    case 2:
                        sidx -= 1;
                        Stack[sidx] = (*reinterpret_cast<fun_type2>(pTok->Fun.ptr))(Stack[sidx], Stack[sidx+1]);
                        continue;
                    case 3:
                        sidx -= 2;
                        Stack[sidx] = (*reinterpret_cast<fun_type3>(pTok->Fun.ptr))(Stack[sidx], Stack[sidx+1],
                                Stack[sidx+2]);
                        continue;
                    case 4:
                        sidx -= 3;
                        Stack[sidx] = (*reinterpret_cast<fun_type4>(pTok->Fun.ptr))(Stack[sidx], Stack[sidx+1],
                                Stack[sidx+2], Stack[sidx+3]);
                        continue;
                    case 5:
                        sidx -= 4;
                        Stack[sidx] = (*reinterpret_cast<fun_type5>(pTok->Fun.ptr))(Stack[sidx], Stack[sidx+1],
                                Stack[sidx+2], Stack[sidx+3], Stack[sidx+4]);
                        continue;
                    case 6:
                        sidx -= 5;
                        Stack[sidx] = (*reinterpret_cast<fun_type6>(pTok->Fun.ptr))(Stack[sidx], Stack[sidx+1],
                                Stack[sidx+2], Stack[sidx+3], Stack[sidx+4], Stack[sidx+5]);
                        continue;
                    case 7:
                        sidx -= 6;
                        Stack[sidx] = (*reinterpret_cast<fun_type7>(pTok->Fun.ptr))(Stack[sidx], Stack[sidx+1],
                                Stack[sidx+2], Stack[sidx+3], Stack[sidx+4], Stack[sidx+5], Stack[sidx+6]);
                        continue;
                    case 8:
                        sidx -= 7;
                        Stack[sidx] = (*reinterpret_cast<fun_type8>(pTok->Fun.ptr))(Stack[sidx], Stack[sidx+1],
                                Stack[sidx+2], Stack[sidx+3], Stack[sidx+4], Stack[sidx+5], Stack[sidx+6],
                                Stack[sidx+7]);
                        continue;
                    case 9:
                        sidx -= 8;
                        Stack[sidx] = (*reinterpret_cast<fun_type9>(pTok->Fun.ptr))(Stack[sidx], Stack[sidx+1],
                                Stack[sidx+2], Stack[sidx+3], Stack[sidx+4], Stack[sidx+5], Stack[sidx+6],
                                Stack[sidx+7], Stack[sidx+8]);
                        continue;
                    case 10:
                        sidx -= 9;
                        Stack[sidx] = (*reinterpret_cast<fun_type10>(pTok->Fun.ptr))(Stack[sidx], Stack[sidx+1],
                                Stack[sidx+2], Stack[sidx+3], Stack[sidx+4], Stack[sidx+5], Stack[sidx+6],
                                Stack[sidx+7], Stack[sidx+8], Stack[sidx+9]);
                        continue;
                    default:
                        if (iArgCount>0) // function with variable arguments store the number as a negative value
                        {
                            Error(ecINTERNAL_ERROR, 1);
                        }

                        sidx -= -iArgCount - 1;
                        Stack[sidx] =(*reinterpret_cast<multfun_type>(pTok->Fun.ptr))(&Stack[sidx], -iArgCount);
                        continue;
                }
            }
            // Next is treatment of string functions
            case cmFUNC_STR:
            {
                sidx -= pTok->Fun.argc -1;

                // The index of the string argument in the string table
                int iIdxStack = pTok->Fun.idx;
                Q_ASSERT( iIdxStack>=0 && iIdxStack<m_vStringBuf.size() );

                switch (pTok->Fun.argc)  // switch according to argument count
                {
                    case 0:
                        Stack[sidx] = (*reinterpret_cast<strfun_type1>(pTok->Fun.ptr))(m_vStringBuf.at(iIdxStack));
                        continue;
                    case 1:
                        Stack[sidx] = (*reinterpret_cast<strfun_type2>(pTok->Fun.ptr))(m_vStringBuf.at(iIdxStack),
                                                                                       Stack[sidx]);
                        continue;
                    case 2:
                        Stack[sidx] = (*reinterpret_cast<strfun_type3>(pTok->Fun.ptr))(m_vStringBuf.at(iIdxStack),
                                                                                       Stack[sidx], Stack[sidx+1]);
                        continue;
                    default:
                        break;
                }

                continue;
            }
            case cmFUNC_BULK:
            {
                int iArgCount = pTok->Fun.argc;

                // switch according to argument count
                switch (iArgCount)
                {
                    case 0:
                        sidx += 1;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type0>(pTok->Fun.ptr))(nOffset, nThreadID);
                        continue;
                    case 1:
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type1>(pTok->Fun.ptr))(nOffset, nThreadID,
                                                                                        Stack[sidx]);
                        continue;
                    case 2:
                        sidx -= 1;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type2>(pTok->Fun.ptr))(nOffset, nThreadID, Stack[sidx],
                                                                       Stack[sidx+1]);
                        continue;
                    case 3:
                        sidx -= 2;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type3>(pTok->Fun.ptr))(nOffset, nThreadID, Stack[sidx],
                                                                       Stack[sidx+1], Stack[sidx+2]);
                        continue;
                    case 4:
                        sidx -= 3;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type4>(pTok->Fun.ptr))(nOffset, nThreadID, Stack[sidx],
                                                                       Stack[sidx+1], Stack[sidx+2], Stack[sidx+3]);
                        continue;
                    case 5:
                        sidx -= 4;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type5>(pTok->Fun.ptr))(nOffset, nThreadID, Stack[sidx],
                                                                       Stack[sidx+1], Stack[sidx+2], Stack[sidx+3],
                                Stack[sidx+4]);
                        continue;
                    case 6:
                        sidx -= 5;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type6>(pTok->Fun.ptr))(nOffset, nThreadID, Stack[sidx],
                                                                       Stack[sidx+1], Stack[sidx+2],
                                Stack[sidx+3], Stack[sidx+4], Stack[sidx+5]);
                        continue;
                    case 7:
                        sidx -= 6;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type7>(pTok->Fun.ptr))(nOffset, nThreadID, Stack[sidx],
                                                                       Stack[sidx+1], Stack[sidx+2], Stack[sidx+3],
                                Stack[sidx+4], Stack[sidx+5], Stack[sidx+6]);
                        continue;
                    case 8:
                        sidx -= 7;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type8>(pTok->Fun.ptr))(nOffset, nThreadID, Stack[sidx],
                                                                       Stack[sidx+1], Stack[sidx+2], Stack[sidx+3],
                                Stack[sidx+4], Stack[sidx+5], Stack[sidx+6], Stack[sidx+7]);
                        continue;
                    case 9:
                        sidx -= 8;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type9>(pTok->Fun.ptr))(nOffset, nThreadID, Stack[sidx],
                                                                       Stack[sidx+1], Stack[sidx+2], Stack[sidx+3],
                                Stack[sidx+4], Stack[sidx+5], Stack[sidx+6], Stack[sidx+7], Stack[sidx+8]);
                        continue;
                    case 10:
                        sidx -= 9;
                        Stack[sidx] = (*reinterpret_cast<bulkfun_type10>(pTok->Fun.ptr))(nOffset, nThreadID,
                                                                                         Stack[sidx],
                                                                       Stack[sidx+1], Stack[sidx+2], Stack[sidx+3],
                                Stack[sidx+4], Stack[sidx+5], Stack[sidx+6], Stack[sidx+7], Stack[sidx+8],
                                Stack[sidx+9]);
                        continue;
                    default:
                        Error(ecINTERNAL_ERROR, 2);
                        continue;
                }
            }
            case cmSTRING:
            case cmOPRT_BIN:
            case cmOPRT_POSTFIX:
            case cmOPRT_INFIX:
            //      Q_ASSERT(INVALID_CODE_IN_BYTECODE);
            //      continue;
            case cmEND:
               //     return Stack[m_nFinalResultIdx];
            case cmPOW2:
            case cmUNKNOWN:
            case cmBO:  // unused, listed for compiler optimization purposes
            case cmBC:
            //    Q_ASSERT(INVALID_CODE_IN_BYTECODE);
            //    continue;
            case cmARG_SEP:
            //    Q_ASSERT(INVALID_CODE_IN_BYTECODE);
            //    continue;
            default:
                Error(ecINTERNAL_ERROR, 3);
                return 0;
        } // switch CmdCode

QT_WARNING_POP

    } // for all bytecode tokens

    return Stack[m_nFinalResultIdx];
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParserBase::CreateRPN() const
{
    if (m_pTokenReader->GetExpr().length() == false)
    {
        Error(ecUNEXPECTED_EOF, 0);
    }

    QStack<token_type> stOpt, stVal;
    QStack<int> stArgCount;
    token_type opta, opt;  // for storing operators
    //token_type val, tval;  // for storing value
    //string_type strBuf;    // buffer for string function arguments

    ReInit();

    // The outermost counter counts the number of seperated items
    // such as in "a=10,b=20,c=c+a"
    stArgCount.push(1);

    for (;;)
    {
        opt = m_pTokenReader->ReadNextToken(m_locale, m_decimalPoint, m_thousandsSeparator);

        switch (opt.GetCode())
        {
            //
            // Next three are different kind of value entries
            //
            case cmSTRING:
            {
                opt.SetIdx(m_vStringBuf.size());      // Assign buffer index to token
                stVal.push(opt);
                const QString &str = opt.GetAsString();
                m_vStringBuf.push_back(str); // Store string in internal buffer
                m_Tokens.insert(m_pTokenReader->GetPos()-str.length(), str);
                break;
            }
            case cmVAR:
            {
                stVal.push(opt);
                m_vRPN.AddVar( static_cast<qreal*>(opt.GetVar()) );
                const QString &str = opt.GetAsString();
                m_Tokens.insert(m_pTokenReader->GetPos()-str.length(), str);
                break;
            }
            case cmVAL:
            {
                stVal.push(opt);
                m_vRPN.AddVal( opt.GetVal() );
                const QString &str = opt.GetAsString();
                m_Numbers.insert(m_pTokenReader->GetPos()-str.length(), str);
                break;
            }
            case cmELSE:
                m_nIfElseCounter--;
                if (m_nIfElseCounter<0)
                {
                    Error(ecMISPLACED_COLON, m_pTokenReader->GetPos());
                }
                ApplyRemainingOprt(stOpt, stVal);
                m_vRPN.AddIfElse(cmELSE);
                stOpt.push(opt);
                break;
            case cmARG_SEP:
                if (stArgCount.empty())
                {
                    Error(ecUNEXPECTED_ARG_SEP, m_pTokenReader->GetPos());
                }
                if (stOpt.empty() && allowSubexpressions == false)
                {
                    Error(ecUNEXPECTED_ARG_SEP, m_pTokenReader->GetPos());
                }
                ++stArgCount.top();
                // fallthrough intentional (no break!)
            #ifdef Q_CC_CLANG
                [[clang::fallthrough]];
            #endif
            case cmEND:
                ApplyRemainingOprt(stOpt, stVal);
                break;
            case cmBC:
            {
                // The argument count for parameterless functions is zero
                // by default an opening bracket sets parameter count to 1
                // in preparation of arguments to come. If the last token
                // was an opening bracket we know better...
                if (opta.GetCode()==cmBO)
                {
                    --stArgCount.top();
                }

                ApplyRemainingOprt(stOpt, stVal);

                // Check if the bracket content has been evaluated completely
                if (stOpt.size() && stOpt.top().GetCode()==cmBO)
                {
                    // if opt is ")" and opta is "(" the bracket has been evaluated, now its time to check
                    // if there is either a function or a sign pending
                    // neither the opening nor the closing bracket will be pushed back to
                    // the operator stack
                    // Check if a function is standing in front of the opening bracket,
                    // if yes evaluate it afterwards check for infix operators
                    assert(stArgCount.size());
                    int iArgCount = stArgCount.pop();

                    stOpt.pop(); // Take opening bracket from stack

                    if (iArgCount>1 && ( stOpt.size()==0 || (stOpt.top().GetCode()!=cmFUNC &&
                                                             stOpt.top().GetCode()!=cmFUNC_BULK &&
                                                             stOpt.top().GetCode()!=cmFUNC_STR) ) )
                    {
                        Error(ecUNEXPECTED_ARG, m_pTokenReader->GetPos());
                    }

                    // The opening bracket was popped from the stack now check if there
                    // was a function before this bracket
                    if (stOpt.size() && stOpt.top().GetCode()!=cmOPRT_INFIX && stOpt.top().GetCode()!=cmOPRT_BIN &&
                            stOpt.top().GetFuncAddr()!=0)
                    {
                        ApplyFunc(stOpt, stVal, iArgCount);
                    }
                }
            } // if bracket content is evaluated
            break;
            //
            // Next are the binary operator entries
            //
            //case cmAND:   // built in binary operators
            //case cmOR:
            //case cmXOR:
            case cmIF:
                m_nIfElseCounter++;
                // fallthrough intentional (no break!)
            #ifdef Q_CC_CLANG
                [[clang::fallthrough]];
            #endif
            case cmLE:
            case cmGE:
            case cmNEQ:
            case cmEQ:
            case cmLT:
            case cmGT:
            case cmADD:
            case cmSUB:
            case cmMUL:
            case cmDIV:
            case cmPOW:
            case cmLAND:
            case cmLOR:
            case cmASSIGN:
            case cmOPRT_BIN:
                // A binary operator (user defined or built in) has been found.
                while ( stOpt.size() && stOpt.top().GetCode() != cmBO && stOpt.top().GetCode() != cmELSE &&
                        stOpt.top().GetCode() != cmIF)
                {
                    const token_type &topToken = stOpt.top();
                    int nPrec1 = GetOprtPrecedence(topToken),
                        nPrec2 = GetOprtPrecedence(opt);

                    const ECmdCode code = topToken.GetCode();
                    if (code==opt.GetCode())
                    {
                        // Deal with operator associativity
                        EOprtAssociativity eOprtAsct = GetOprtAssociativity(opt);
                        if ( (eOprtAsct==oaRIGHT && (nPrec1 <= nPrec2)) ||
                             (eOprtAsct==oaLEFT  && (nPrec1 <  nPrec2)) )
                        {
                            break;
                        }
                    }
                    else if (nPrec1 < nPrec2)
                    {
                        // In case the operators are not equal the precedence decides alone...
                        break;
                    }
                    if (code==cmOPRT_INFIX)
                    {
                        ApplyFunc(stOpt, stVal, 1);
                    }
                    else
                    {
                        ApplyBinOprt(stOpt, stVal);
                    }
                } // while ( ... )

                if (opt.GetCode()==cmIF)
                {
                    m_vRPN.AddIfElse(opt.GetCode());
                }

                      // The operator can't be evaluated right now, push back to the operator stack
                stOpt.push(opt);
                break;
            //
            // Last section contains functions and operators implicitely mapped to functions
            //
            case cmBO:
                stArgCount.push(1);
                stOpt.push(opt);
                break;
            case cmOPRT_INFIX:
            case cmFUNC:
            case cmFUNC_BULK:
            case cmFUNC_STR:
                stOpt.push(opt);
                m_Tokens.insert(m_pTokenReader->GetPos()-opt.GetAsString().length(), opt.GetAsString());
                break;
            case cmOPRT_POSTFIX:
                stOpt.push(opt);
                ApplyFunc(stOpt, stVal, 1);  // this is the postfix operator
                m_Tokens.insert(m_pTokenReader->GetPos()-opt.GetAsString().length(), opt.GetAsString());
                break;
            case cmENDIF:
            case cmVARPOW2:
            case cmVARPOW3:
            case cmVARPOW4:
            case cmVARMUL:
            case cmPOW2:
            case cmUNKNOWN:
            default:
                Error(ecINTERNAL_ERROR, 3);
        } // end of switch operator-token

        opta = opt;

        if ( opt.GetCode() == cmEND )
        {
            m_vRPN.Finalize();
            break;
        }

        if (QmuParserBase::g_DbgDumpStack)
        {
            StackDump(stVal, stOpt);
            m_vRPN.AsciiDump();
        }
    } // while (true)

    if (QmuParserBase::g_DbgDumpCmdCode)
    {
        m_vRPN.AsciiDump();
    }

    if (m_nIfElseCounter>0)
    {
        Error(ecMISSING_ELSE_CLAUSE);
    }

    // get the last value (= final result) from the stack
    Q_ASSERT(stArgCount.size()==1);
    m_nFinalResultIdx = stArgCount.top();
    if (m_nFinalResultIdx==0)
    {
        Error(ecINTERNAL_ERROR, 9);
    }

    if (stVal.size()==0)
    {
        Error(ecEMPTY_EXPRESSION);
    }

    if (stVal.top().GetType()!=tpDBL)
    {
        Error(ecSTR_RESULT);
    }

    m_vStackBuffer.resize(m_vRPN.GetMaxStackSize() * s_MaxNumOpenMPThreads);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief One of the two main parse functions.
 * @sa ParseCmdCode(...)
 *
 * Parse expression from input string. Perform syntax checking and create bytecode. After parsing the string and
 * creating the bytecode the function pointer #m_pParseFormula will be changed to the second parse routine the
 * uses bytecode instead of string parsing.
 */
qreal QmuParserBase::ParseString() const
{
    try
    {
        CreateRPN();
        m_pParseFormula = &QmuParserBase::ParseCmdCode;
        return (this->*m_pParseFormula)();
    }
    catch (qmu::QmuParserError &exc)
    {
        exc.SetFormula(m_pTokenReader->GetExpr());
        throw;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
* @brief Create an error containing the parse error position.
*
* This function will create an Parser Exception object containing the error text and its position.
*
* @param a_iErrc [in] The error code of type #EErrorCodes.
* @param a_iPos [in] The position where the error was detected.
* @param a_sTok [in] The token string representation associated with the error.
* @throw ParserException always throws thats the only purpose of this function.
*/
void Q_NORETURN QmuParserBase::Error(EErrorCodes a_iErrc, int a_iPos, const QString &a_sTok) const
{
    throw qmu::QmuParserError (a_iErrc, a_sTok, m_pTokenReader->GetExpr(), a_iPos);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear all user defined variables.
 * @throw nothrow
 *
 * Resets the parser to string parsing mode by calling #ReInit.
 */
// cppcheck-suppress unusedFunction
void QmuParserBase::ClearVar()
{
    m_VarDef.clear();
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Remove a variable from internal storage.
 * @throw nothrow
 *
 * Removes a variable if it exists. If the Variable does not exist nothing will be done.
 */
void QmuParserBase::RemoveVar(const QString &a_strVarName)
{
    varmap_type::iterator item = m_VarDef.find(a_strVarName);
    if (item!=m_VarDef.end())
    {
        m_VarDef.erase(item);
        ReInit();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear all functions.
 * @post Resets the parser to string parsing mode.
 * @throw nothrow
 */
// cppcheck-suppress unusedFunction
void QmuParserBase::ClearFun()
{
    m_FunDef.clear();
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear all user defined constants.
 *
 * Both numeric and string constants will be removed from the internal storage.
 * @post Resets the parser to string parsing mode.
 * @throw nothrow
 */
void QmuParserBase::ClearConst()
{
    m_ConstDef.clear();
    m_StrVarDef.clear();
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear all user defined postfix operators.
 * @post Resets the parser to string parsing mode.
 * @throw nothrow
 */
void QmuParserBase::ClearPostfixOprt()
{
    m_PostOprtDef.clear();
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear all user defined binary operators.
 * @post Resets the parser to string parsing mode.
 * @throw nothrow
 */
// cppcheck-suppress unusedFunction
void QmuParserBase::ClearOprt()
{
    m_OprtDef.clear();
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear the user defined Prefix operators.
 * @post Resets the parser to string parser mode.
 * @throw nothrow
 */
// cppcheck-suppress unusedFunction
void QmuParserBase::ClearInfixOprt()
{
    m_InfixOprtDef.clear();
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Enable or disable the formula optimization feature.
 * @post Resets the parser to string parser mode.
 * @throw nothrow
 */
void QmuParserBase::EnableOptimizer(bool a_bIsOn)
{
    m_vRPN.EnableOptimizer(a_bIsOn);
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Enable the dumping of bytecode amd stack content on the console.
 * @param bDumpCmd Flag to enable dumping of the current bytecode to the console.
 * @param bDumpStack Flag to enable dumping of the stack content is written to the console.
 *
 * This function is for debug purposes only!
 */
// cppcheck-suppress unusedFunction
void QmuParserBase::EnableDebugDump(bool bDumpCmd, bool bDumpStack)
{
    QmuParserBase::g_DbgDumpCmdCode = bDumpCmd;
    QmuParserBase::g_DbgDumpStack   = bDumpStack;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Enable or disable the built in binary operators.
 * @throw nothrow
 * @sa m_bBuiltInOp, ReInit()
 *
 * If you disable the built in binary operators there will be no binary operators defined. Thus you must add them
 * manually one by one. It is not possible to disable built in operators selectively. This function will Reinitialize
 * the parser by calling ReInit().
 */
void QmuParserBase::EnableBuiltInOprt(bool a_bIsOn)
{
    m_bBuiltInOp = a_bIsOn;
    ReInit();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Get the argument separator character.
 */
QChar QmuParserBase::GetArgSep() const
{
    return m_pTokenReader->GetArgSep();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Set argument separator.
 * @param cArgSep the argument separator character.
 */
void QmuParserBase::SetArgSep(char_type cArgSep)
{
    m_pTokenReader->SetArgSep(cArgSep);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Dump stack content.
 *
 * This function is used for debugging only.
 */
void QmuParserBase::StackDump(const QStack<token_type> &a_stVal, const QStack<token_type> &a_stOprt) const
{
    QStack<token_type> stOprt(a_stOprt),
                       stVal(a_stVal);

    qDebug() << "\nValue stack:\n";
    while ( stVal.empty() == false )
    {
        token_type val = stVal.pop();
        if (val.GetType()==tpSTR)
        {
            qDebug() << " \"" << val.GetAsString() << "\" ";
        }
        else
        {
            qDebug() << " " << val.GetVal() << " ";
        }
    }
    qDebug() << "\nOperator stack:\n";

    while ( stOprt.empty() == false )
    {
        const token_type &topToken = stOprt.top();
        if (topToken.GetCode()<=cmASSIGN)
        {
            qDebug() << "OPRT_INTRNL \"" << QmuParserBase::c_DefaultOprt[topToken.GetCode()] << "\" \n";
        }
        else
        {
            switch ( topToken.GetCode())
            {
                case cmVAR:
                    qDebug() << "VAR\n";
                    break;
                case cmVAL:
                    qDebug() << "VAL\n";
                    break;
                case cmFUNC:
                    qDebug() << "FUNC \"" << topToken.GetAsString() << "\"\n";
                    break;
                case cmFUNC_BULK:
                    qDebug() << "FUNC_BULK \"" << topToken.GetAsString() << "\"\n";
                    break;
                case cmOPRT_INFIX:
                    qDebug() << "OPRT_INFIX \"" << topToken.GetAsString() << "\"\n";
                    break;
                case cmOPRT_BIN:
                    qDebug() << "OPRT_BIN \"" << topToken.GetAsString() << "\"\n";
                    break;
                case cmFUNC_STR:
                    qDebug() << "FUNC_STR\n";
                    break;
                case cmEND:
                    qDebug() << "END\n";
                    break;
                case cmUNKNOWN:
                    qDebug() << "UNKNOWN\n";
                    break;
                case cmBO:
                    qDebug() << "BRACKET \"(\"\n";
                    break;
                case cmBC:
                    qDebug() << "BRACKET \")\"\n";
                    break;
                case cmIF:
                    qDebug() << "IF\n";
                    break;
                case cmELSE:
                    qDebug() << "ELSE\n";
                    break;
                case cmENDIF:
                    qDebug() << "ENDIF\n";
                    break;
                default:
                    qDebug() << topToken.GetCode() << " ";
                    break;
            }
        }
        stOprt.pop();
    }
    qDebug() << dec;
}

//---------------------------------------------------------------------------------------------------------------------
/** @brief Evaluate an expression containing comma seperated subexpressions
  * @param [out] nStackSize The total number of results available
  * @return Pointer to the array containing all expression results
  *
  * This member function can be used to retriev all results of an expression made up of multiple comma seperated
  * subexpressions (i.e. "x+y,sin(x),cos(y)")
  */
qreal* QmuParserBase::Eval(int &nStackSize) const
{
    (this->*m_pParseFormula)();
    nStackSize = m_nFinalResultIdx;

    // (for historic reasons the stack starts at position 1)
    return &m_vStackBuffer[1];
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParserBase::Eval(qreal *results, int nBulkSize) const
{
    CreateRPN();

    int i = 0;

    #ifdef QMUP_USE_OPENMP
    //#define DEBUG_OMP_STUFF
    #ifdef DEBUG_OMP_STUFF
        int *pThread = new int[nBulkSize];
        int *pIdx = new int[nBulkSize];
    #endif

    int nMaxThreads = qMin(omp_get_max_threads(), s_MaxNumOpenMPThreads);
    // cppcheck-suppress unreadVariable
    int ct=0;
    omp_set_num_threads(nMaxThreads);

    #pragma omp parallel for schedule(static, nBulkSize/nMaxThreads) private(nThreadID)
    for (i=0; i<nBulkSize; ++i)
    {
        int nThreadID = omp_get_thread_num();
        results[i] = ParseCmdCodeBulk(i, nThreadID);

        #ifdef DEBUG_OMP_STUFF
        #pragma omp critical
        {
            pThread[ct] = nThreadID;
            pIdx[ct] = i;
            ct++;
        }
        #endif
    }

    #ifdef DEBUG_OMP_STUFF
    FILE *pFile = fopen("bulk_dbg.txt", "w");
    for (i=0; i<nBulkSize; ++i)
    {
        fprintf(pFile, "idx: %d  thread: %d \n", pIdx[i], pThread[i]);
    }

    delete [] pIdx;
    delete [] pThread;

    fclose(pFile);
    #endif

    #else
    for (i=0; i<nBulkSize; ++i)
    {
        results[i] = ParseCmdCodeBulk(i, 0);
    }
    #endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Set a function that can create variable pointer for unknown expression variables.
 * @param a_pFactory A pointer to the variable factory.
 * @param pUserData A user defined context pointer.
 */
// cppcheck-suppress unusedFunction
void qmu::QmuParserBase::SetVarFactory(facfun_type a_pFactory, void *pUserData)
{
    m_pTokenReader->SetVarCreator(a_pFactory, pUserData);
}

} // namespace qmu
