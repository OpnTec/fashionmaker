/***************************************************************************************************
 **
 **  Original work Copyright (C) 2013 Ingo Berg
 **  Modified work Copyright 2014 Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "qmuparsererror.h"

#include <QTextStream>

namespace qmu
{
const QmuParserErrorMsg QmuParserErrorMsg::m_Instance;

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
const QmuParserErrorMsg& QmuParserErrorMsg::Instance()
{
    return m_Instance;
}

//---------------------------------------------------------------------------------------------------------------------
QString QmuParserErrorMsg::operator[] ( unsigned a_iIdx ) const
{
    return ( a_iIdx < static_cast<unsigned>( m_vErrMsg.size() ) ) ? m_vErrMsg[a_iIdx] : QString();
}

//---------------------------------------------------------------------------------------------------------------------
QmuParserErrorMsg::~QmuParserErrorMsg()
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserErrorMsg::QmuParserErrorMsg()
    : m_vErrMsg ( 0 )
{
    m_vErrMsg.resize ( ecCOUNT );

    m_vErrMsg[ecUNASSIGNABLE_TOKEN]     = "Unexpected token \"$TOK$\" found at position $POS$.";
    m_vErrMsg[ecINTERNAL_ERROR]         = "Internal error";
    m_vErrMsg[ecINVALID_NAME]           = "Invalid function-, variable- or constant name: \"$TOK$\".";
    m_vErrMsg[ecINVALID_BINOP_IDENT]    = "Invalid binary operator identifier: \"$TOK$\".";
    m_vErrMsg[ecINVALID_INFIX_IDENT]    = "Invalid infix operator identifier: \"$TOK$\".";
    m_vErrMsg[ecINVALID_POSTFIX_IDENT]  = "Invalid postfix operator identifier: \"$TOK$\".";
    m_vErrMsg[ecINVALID_FUN_PTR]        = "Invalid pointer to callback function.";
    m_vErrMsg[ecEMPTY_EXPRESSION]       = "Expression is empty.";
    m_vErrMsg[ecINVALID_VAR_PTR]        = "Invalid pointer to variable.";
    m_vErrMsg[ecUNEXPECTED_OPERATOR]    = "Unexpected operator \"$TOK$\" found at position $POS$";
    m_vErrMsg[ecUNEXPECTED_EOF]         = "Unexpected end of expression at position $POS$";
    m_vErrMsg[ecUNEXPECTED_ARG_SEP]     = "Unexpected argument separator at position $POS$";
    m_vErrMsg[ecUNEXPECTED_PARENS]      = "Unexpected parenthesis \"$TOK$\" at position $POS$";
    m_vErrMsg[ecUNEXPECTED_FUN]         = "Unexpected function \"$TOK$\" at position $POS$";
    m_vErrMsg[ecUNEXPECTED_VAL]         = "Unexpected value \"$TOK$\" found at position $POS$";
    m_vErrMsg[ecUNEXPECTED_VAR]         = "Unexpected variable \"$TOK$\" found at position $POS$";
    m_vErrMsg[ecUNEXPECTED_ARG]         = "Function arguments used without a function (position: $POS$)";
    m_vErrMsg[ecMISSING_PARENS]         = "Missing parenthesis";
    m_vErrMsg[ecTOO_MANY_PARAMS]        = "Too many parameters for function \"$TOK$\" at expression position $POS$";
    m_vErrMsg[ecTOO_FEW_PARAMS]         = "Too few parameters for function \"$TOK$\" at expression position $POS$";
    m_vErrMsg[ecDIV_BY_ZERO]            = "Divide by zero";
    m_vErrMsg[ecDOMAIN_ERROR]           = "Domain error";
    m_vErrMsg[ecNAME_CONFLICT]          = "Name conflict";
    m_vErrMsg[ecOPT_PRI]                = "Invalid value for operator priority (must be greater or equal to zero).";
    m_vErrMsg[ecBUILTIN_OVERLOAD]       = "user defined binary operator \"$TOK$\" conflicts with a built in operator.";
    m_vErrMsg[ecUNEXPECTED_STR]         = "Unexpected string token found at position $POS$.";
    m_vErrMsg[ecUNTERMINATED_STRING]    = "Unterminated string starting at position $POS$.";
    m_vErrMsg[ecSTRING_EXPECTED]        = "String function called with a non string type of argument.";
    m_vErrMsg[ecVAL_EXPECTED]           = "String value used where a numerical argument is expected.";
    m_vErrMsg[ecOPRT_TYPE_CONFLICT]     = "No suitable overload for operator \"$TOK$\" at position $POS$.";
    m_vErrMsg[ecSTR_RESULT]             = "Function result is a string.";
    m_vErrMsg[ecGENERIC]                = "Parser error.";
    m_vErrMsg[ecLOCALE]                 = "Decimal separator is identic to function argument separator.";
    m_vErrMsg[ecUNEXPECTED_CONDITIONAL] = "The \"$TOK$\" operator must be preceeded by a closing bracket.";
    m_vErrMsg[ecMISSING_ELSE_CLAUSE]    = "If-then-else operator is missing an else clause";
    m_vErrMsg[ecMISPLACED_COLON]        = "Misplaced colon at position $POS$";

#if defined(_DEBUG)
    for ( int i = 0; i < ecCOUNT; ++i )
    {
        if ( m_vErrMsg[i].length() == false)
        {
            assert ( false );
        }
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
//
//  QParserError class
//
//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Default constructor.
 */
QmuParserError::QmuParserError()
    : m_strMsg(), m_strFormula(), m_strTok(), m_iPos ( -1 ), m_iErrc ( ecUNDEFINED ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This Constructor is used for internal exceptions only.
 *
 * It does not contain any information but the error code.
 */
QmuParserError::QmuParserError ( EErrorCodes a_iErrc )
    : m_strMsg(), m_strFormula(), m_strTok(), m_iPos ( -1 ), m_iErrc ( a_iErrc ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
    m_strMsg = m_ErrMsg[m_iErrc];
    ReplaceSubString ( m_strMsg, "$POS$", QString().setNum ( m_iPos ) );
    ReplaceSubString ( m_strMsg, "$TOK$", m_strTok );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Construct an error from a message text.
 */
QmuParserError::QmuParserError ( const QString &sMsg )
    : m_strMsg(sMsg), m_strFormula(), m_strTok(), m_iPos ( -1 ), m_iErrc ( ecUNDEFINED ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Construct an error object.
 * @param [in] a_iErrc the error code.
 * @param [in] sTok The token string related to this error.
 * @param [in] sExpr The expression related to the error.
 * @param [in] a_iPos the position in the expression where the error occured.
 */
QmuParserError::QmuParserError ( EErrorCodes iErrc,
                                 const QString &sTok,
                                 const QString &sExpr,
                                 int iPos )
    : m_strMsg(), m_strFormula ( sExpr ), m_strTok ( sTok ), m_iPos ( iPos ), m_iErrc ( iErrc ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
    m_strMsg = m_ErrMsg[m_iErrc];
    ReplaceSubString ( m_strMsg, "$POS$", QString().setNum ( m_iPos ) );
    ReplaceSubString ( m_strMsg, "$TOK$", m_strTok );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Construct an error object.
 * @param [in] iErrc the error code.
 * @param [in] iPos the position in the expression where the error occured.
 * @param [in] sTok The token string related to this error.
 */
QmuParserError::QmuParserError ( EErrorCodes iErrc, int iPos, const QString &sTok )
    : m_strMsg(), m_strFormula(), m_strTok ( sTok ), m_iPos ( iPos ), m_iErrc ( iErrc ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
    m_strMsg = m_ErrMsg[m_iErrc];
    ReplaceSubString ( m_strMsg, "$POS$", QString().setNum ( m_iPos ) );
    ReplaceSubString ( m_strMsg, "$TOK$", m_strTok );
}

//---------------------------------------------------------------------------------------------------------------------
/** @brief Construct an error object.
  * @param [in] szMsg The error message text.
  * @param [in] iPos the position related to the error.
  * @param [in] sTok The token string related to this error.
  */
QmuParserError::QmuParserError ( const QString &szMsg, int iPos, const QString &sTok )
    : m_strMsg ( szMsg ), m_strFormula(), m_strTok ( sTok ), m_iPos ( iPos ), m_iErrc ( ecGENERIC ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
    ReplaceSubString ( m_strMsg, "$POS$", QString().setNum ( m_iPos ) );
    ReplaceSubString ( m_strMsg, "$TOK$", m_strTok );
}

//---------------------------------------------------------------------------------------------------------------------
/** @brief Copy constructor. */
QmuParserError::QmuParserError ( const QmuParserError &a_Obj )
    : m_strMsg ( a_Obj.m_strMsg ), m_strFormula ( a_Obj.m_strFormula ), m_strTok ( a_Obj.m_strTok ),
      m_iPos ( a_Obj.m_iPos ), m_iErrc ( a_Obj.m_iErrc ), m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
}

//---------------------------------------------------------------------------------------------------------------------
/** @brief Assignment operator. */
QmuParserError& QmuParserError::operator= ( const QmuParserError &a_Obj )
{
    if ( this == &a_Obj )
    {
        return *this;
    }

    m_strMsg = a_Obj.m_strMsg;
    m_strFormula = a_Obj.m_strFormula;
    m_strTok = a_Obj.m_strTok;
    m_iPos = a_Obj.m_iPos;
    m_iErrc = a_Obj.m_iErrc;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
QmuParserError::~QmuParserError()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Replace all ocuurences of a substring with another string.
 * @param strFind The string that shall be replaced.
 * @param strReplaceWith The string that should be inserted instead of strFind
 */
void QmuParserError::ReplaceSubString ( QString &strSource, const QString &strFind, const QString &strReplaceWith )
{
    QString strResult;
    int iPos ( 0 );

    for ( ;; )
    {
        int iNext = strSource.indexOf ( strFind, iPos );
        strResult.append ( strSource.mid ( iPos, iNext - iPos ) );

        if ( iNext == -1 )
        {
            break;
        }

        strResult.append ( strReplaceWith );
        iPos = iNext + strFind.length();
    }

    strSource.swap ( strResult );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Reset the erro object.
 */
// cppcheck-suppress unusedFunction
void QmuParserError::Reset()
{
    m_strMsg.clear();
    m_strFormula.clear();
    m_strTok.clear();
    m_iPos = -1;
    m_iErrc = ecUNDEFINED;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Set the expression related to this error.
 */
void QmuParserError::SetFormula ( const QString &a_strFormula )
{
    m_strFormula = a_strFormula;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief gets the expression related tp this error.
 */
const QString& QmuParserError::GetExpr() const
{
    return m_strFormula;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the message string for this error.
 */
const QString& QmuParserError::GetMsg() const
{
    return m_strMsg;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the formula position related to the error.
 *
 * If the error is not related to a distinct position this will return -1
 */
std::size_t QmuParserError::GetPos() const
{
    return m_iPos;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return string related with this token (if available).
 */
const QString& QmuParserError::GetToken() const
{
    return m_strTok;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the error code.
 */
EErrorCodes QmuParserError::GetCode() const
{
    return m_iErrc;
}
} // namespace qmu
