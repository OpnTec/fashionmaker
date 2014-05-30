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
    : QException(), m_sMsg(), m_sExpr(), m_sTok(), m_iPos ( -1 ), m_iErrc ( ecUNDEFINED ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This Constructor is used for internal exceptions only.
 *
 * It does not contain any information but the error code.
 */
QmuParserError::QmuParserError ( EErrorCodes a_iErrc )
    : QException(), m_sMsg(), m_sExpr(), m_sTok(), m_iPos ( -1 ), m_iErrc ( a_iErrc ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
    m_sMsg = m_ErrMsg[m_iErrc];
    m_sMsg.replace("$POS$", QString().setNum ( m_iPos ));
    m_sMsg.replace("$TOK$", m_sTok );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Construct an error from a message text.
 */
QmuParserError::QmuParserError ( const QString &sMsg )
    : QException(), m_sMsg(sMsg), m_sExpr(), m_sTok(), m_iPos ( -1 ), m_iErrc ( ecUNDEFINED ),
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
QmuParserError::QmuParserError ( EErrorCodes iErrc, const QString &sTok, const QString &sExpr, int iPos )
    : QException(), m_sMsg(), m_sExpr ( sExpr ), m_sTok ( sTok ), m_iPos ( iPos ), m_iErrc ( iErrc ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
    m_sMsg = m_ErrMsg[m_iErrc];
    m_sMsg.replace("$POS$", QString().setNum ( m_iPos ));
    m_sMsg.replace("$TOK$", m_sTok );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Construct an error object.
 * @param [in] iErrc the error code.
 * @param [in] iPos the position in the expression where the error occured.
 * @param [in] sTok The token string related to this error.
 */
QmuParserError::QmuParserError ( EErrorCodes a_iErrc, int a_iPos, const QString &sTok )
    : QException(), m_sMsg(), m_sExpr(), m_sTok ( sTok ), m_iPos ( a_iPos ), m_iErrc ( a_iErrc ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
    m_sMsg = m_ErrMsg[m_iErrc];
    m_sMsg.replace("$POS$", QString().setNum ( m_iPos ));
    m_sMsg.replace("$TOK$", m_sTok );
}

//---------------------------------------------------------------------------------------------------------------------
/** @brief Construct an error object.
  * @param [in] szMsg The error message text.
  * @param [in] iPos the position related to the error.
  * @param [in] sTok The token string related to this error.
  */
QmuParserError::QmuParserError ( const QString &szMsg, int iPos, const QString &sTok )
    : QException(), m_sMsg ( szMsg ), m_sExpr(), m_sTok ( sTok ), m_iPos ( iPos ), m_iErrc ( ecGENERIC ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
    m_sMsg.replace("$POS$", QString().setNum ( m_iPos ));
    m_sMsg.replace("$TOK$", m_sTok );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Copy constructor.
 */
QmuParserError::QmuParserError ( const QmuParserError &a_Obj )
    : QException(), m_sMsg ( a_Obj.m_sMsg ), m_sExpr ( a_Obj.m_sExpr ), m_sTok ( a_Obj.m_sTok ),
      m_iPos ( a_Obj.m_iPos ), m_iErrc ( a_Obj.m_iErrc ), m_ErrMsg ( QmuParserErrorMsg::Instance() )
{}

//---------------------------------------------------------------------------------------------------------------------
/** @brief Assignment operator. */
QmuParserError& QmuParserError::operator= ( const QmuParserError &a_Obj )
{
    if ( this == &a_Obj )
    {
        return *this;
    }

    m_sMsg = a_Obj.m_sMsg;
    m_sExpr = a_Obj.m_sExpr;
    m_sTok = a_Obj.m_sTok;
    m_iPos = a_Obj.m_iPos;
    m_iErrc = a_Obj.m_iErrc;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Reset the erro object.
 */
// cppcheck-suppress unusedFunction
void QmuParserError::Reset()
{
    m_sMsg.clear();
    m_sExpr.clear();
    m_sTok.clear();
    m_iPos = -1;
    m_iErrc = ecUNDEFINED;
}

} // namespace qmu
