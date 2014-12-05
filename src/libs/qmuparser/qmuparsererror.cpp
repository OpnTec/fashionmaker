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

#ifdef Q_OS_WIN
#include <assert.h>
#endif

namespace qmu
{
const QmuParserErrorMsg QmuParserErrorMsg::m_Instance;

//---------------------------------------------------------------------------------------------------------------------
QmuParserErrorMsg::~QmuParserErrorMsg()
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserErrorMsg::QmuParserErrorMsg()
    : m_vErrMsg ()
{
    m_vErrMsg.clear();

    m_vErrMsg.insert(ecUNASSIGNABLE_TOKEN,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unexpected token \"$TOK$\" found at position $POS$.",
                                               "Math parser error messages. Left untouched \"$TOK$\" and $POS$"));
    m_vErrMsg.insert(ecINTERNAL_ERROR,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Internal error",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecINVALID_NAME,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Invalid function-, variable- or constant name: \"$TOK$\".",
                                               "Math parser error messages. Left untouched \"$TOK$\""));
    m_vErrMsg.insert(ecINVALID_BINOP_IDENT,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Invalid binary operator identifier: \"$TOK$\".",
                                               "Math parser error messages. Left untouched \"$TOK$\""));
    m_vErrMsg.insert(ecINVALID_INFIX_IDENT,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Invalid infix operator identifier: \"$TOK$\".",
                                               "Math parser error messages. Left untouched \"$TOK$\""));
    m_vErrMsg.insert(ecINVALID_POSTFIX_IDENT,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Invalid postfix operator identifier: \"$TOK$\".",
                                               "Math parser error messages. Left untouched \"$TOK$\""));
    m_vErrMsg.insert(ecINVALID_FUN_PTR,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Invalid pointer to callback function.",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecEMPTY_EXPRESSION,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Expression is empty.",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecINVALID_VAR_PTR,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Invalid pointer to variable.",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecUNEXPECTED_OPERATOR,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unexpected operator \"$TOK$\" found at position $POS$",
                                               "Math parser error messages. Left untouched \"$TOK$\" and $POS$"));
    m_vErrMsg.insert(ecUNEXPECTED_EOF,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unexpected end of expression at position $POS$",
                                               "Math parser error messages. Left untouched $POS$"));
    m_vErrMsg.insert(ecUNEXPECTED_ARG_SEP,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unexpected argument separator at position $POS$",
                                               "Math parser error messages. Left untouched $POS$"));
    m_vErrMsg.insert(ecUNEXPECTED_PARENS,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unexpected parenthesis \"$TOK$\" at position $POS$",
                                               "Math parser error messages. Left untouched \"$TOK$\" and $POS$"));
    m_vErrMsg.insert(ecUNEXPECTED_FUN,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unexpected function \"$TOK$\" at position $POS$",
                                               "Math parser error messages. Left untouched \"$TOK$\" and $POS$"));
    m_vErrMsg.insert(ecUNEXPECTED_VAL,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unexpected value \"$TOK$\" found at position $POS$",
                                               "Math parser error messages. Left untouched \"$TOK$\" and $POS$"));
    m_vErrMsg.insert(ecUNEXPECTED_VAR,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unexpected variable \"$TOK$\" found at position $POS$",
                                               "Math parser error messages. Left untouched \"$TOK$\" and $POS$"));
    m_vErrMsg.insert(ecUNEXPECTED_ARG,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Function arguments used without a function (position: $POS$)",
                                               "Math parser error messages. Left untouched $POS$"));
    m_vErrMsg.insert(ecMISSING_PARENS,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Missing parenthesis",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecTOO_MANY_PARAMS,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                              "Too many parameters for function \"$TOK$\" at expression position $POS$",
                                               "Math parser error messages. Left untouched \"$TOK$\" and $POS$"));
    m_vErrMsg.insert(ecTOO_FEW_PARAMS,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Too few parameters for function \"$TOK$\" at expression position $POS$",
                                               "Math parser error messages. Left untouched \"$TOK$\" and $POS$"));
    m_vErrMsg.insert(ecDIV_BY_ZERO,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Divide by zero",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecDOMAIN_ERROR,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Domain error",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecNAME_CONFLICT,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Name conflict",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecOPT_PRI,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                              "Invalid value for operator priority (must be greater or equal to zero).",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecBUILTIN_OVERLOAD,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                           "user defined binary operator \"$TOK$\" conflicts with a built in operator.",
                                               "Math parser error messages. Left untouched \"$TOK$\""));
    m_vErrMsg.insert(ecUNEXPECTED_STR,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unexpected string token found at position $POS$.",
                                               "Math parser error messages. Left untouched $POS$"));
    m_vErrMsg.insert(ecUNTERMINATED_STRING,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Unterminated string starting at position $POS$.",
                                               "Math parser error messages. Left untouched $POS$"));
    m_vErrMsg.insert(ecSTRING_EXPECTED,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "String function called with a non string type of argument.",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecVAL_EXPECTED,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "String value used where a numerical argument is expected.",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecOPRT_TYPE_CONFLICT,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "No suitable overload for operator \"$TOK$\" at position $POS$.",
                                               "Math parser error messages. Left untouched \"$TOK$\" and $POS$"));
    m_vErrMsg.insert(ecSTR_RESULT,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Function result is a string.",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecGENERIC,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Parser error.",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecLOCALE,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Decimal separator is identic to function argument separator.",
                                               "Math parser error messages."));
    m_vErrMsg.insert(ecUNEXPECTED_CONDITIONAL,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "The \"$TOK$\" operator must be preceeded by a closing bracket.",
                                               "Math parser error messages. Left untouched \"$TOK$\""));
    m_vErrMsg.insert(ecMISSING_ELSE_CLAUSE,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "If-then-else operator is missing an else clause",
                                               "Math parser error messages. Do not translate operator name."));
    m_vErrMsg.insert(ecMISPLACED_COLON,
                     QmuTranslation::translate("QmuParserErrorMsg",
                                               "Misplaced colon at position $POS$",
                                               "Math parser error messages. Left untouched $POS$"));
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
 * @param [in] iErrc the error code.
 * @param [in] sTok The token string related to this error.
 * @param [in] sExpr The expression related to the error.
 * @param [in] iPos the position in the expression where the error occured.
 */
QmuParserError::QmuParserError ( EErrorCodes iErrc, const QString &sTok, const QString &sExpr, int iPos )
    : QException(), m_sMsg(), m_sExpr ( sExpr ), m_sTok ( sTok ), m_iPos ( iPos ), m_iErrc ( iErrc ),
      m_ErrMsg ( QmuParserErrorMsg::Instance() )
{
    m_sMsg = m_ErrMsg[m_iErrc];
    qDebug()<<m_sMsg;
    m_sMsg.replace("$POS$", QString().setNum ( m_iPos ));
    qDebug()<<m_sMsg;
    m_sMsg.replace("$TOK$", m_sTok );
    qDebug()<<m_sMsg;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Construct an error object.
 * @param [in] a_iErrc the error code.
 * @param [in] a_iPos the position in the expression where the error occured.
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief raise method raise for exception
 */
Q_NORETURN void QmuParserError::raise() const
{
    throw *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clone clone exception
 * @return new exception
 */
QmuParserError *QmuParserError::clone() const
{
    return new QmuParserError(*this);
}

} // namespace qmu
