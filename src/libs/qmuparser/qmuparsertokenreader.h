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

#ifndef QMUPARSERTOKENREADER_H
#define QMUPARSERTOKENREADER_H

#include "qmuparserdef.h"
#include "qmuparsertoken.h"

/**
 * @file
 * @brief This file contains the parser token reader definition.
 */

namespace qmu
{
// Forward declaration
class QmuParserBase;

/**
 * @brief Token reader for the ParserBase class.
 *
 */
class QmuParserTokenReader
{
private:
    typedef QmuParserToken<qreal, QString> token_type;
public:
    QmuParserTokenReader(QmuParserBase *a_pParent) Q_DECL_NOEXCEPT;
    QmuParserTokenReader* Clone(QmuParserBase *a_pParent) const Q_DECL_NOEXCEPT;

    void           AddValIdent(identfun_type a_pCallback);
    void           SetVarCreator(facfun_type a_pFactory, void *pUserData) Q_DECL_NOEXCEPT;
    void           SetFormula(const QString &a_strFormula) Q_DECL_NOEXCEPT;
    void           SetArgSep(char_type cArgSep) Q_DECL_NOEXCEPT;
    int            GetPos() const Q_DECL_NOEXCEPT;
    const QString& GetExpr() const Q_DECL_NOEXCEPT;
    varmap_type&   GetUsedVar() Q_DECL_NOEXCEPT;
    QChar          GetArgSep() const Q_DECL_NOEXCEPT;
    void           IgnoreUndefVar(bool bIgnore) Q_DECL_NOEXCEPT;
    void           ReInit() Q_DECL_NOEXCEPT;
    token_type     ReadNextToken();
private:

    /**
     * @brief Syntax codes.
     *
     * The syntax codes control the syntax check done during the first time parsing of
     * the expression string. They are flags that indicate which tokens are allowed next
     * if certain tokens are identified.
     */
    enum ESynCodes
    {
        noBO      = 1 << 0,  ///< to avoid i.e. "cos(7)("
        noBC      = 1 << 1,  ///< to avoid i.e. "sin)" or "()"
        noVAL     = 1 << 2,  ///< to avoid i.e. "tan 2" or "sin(8)3.14"
        noVAR     = 1 << 3,  ///< to avoid i.e. "sin a" or "sin(8)a"
        noARG_SEP = 1 << 4,  ///< to avoid i.e. ",," or "+," ...
        noFUN     = 1 << 5,  ///< to avoid i.e. "sqrt cos" or "(1)sin"
        noOPT     = 1 << 6,  ///< to avoid i.e. "(+)"
        noPOSTOP  = 1 << 7,  ///< to avoid i.e. "(5!!)" "sin!"
        noINFIXOP = 1 << 8,  ///< to avoid i.e. "++4" "!!4"
        noEND     = 1 << 9,  ///< to avoid unexpected end of formula
        noSTR     = 1 << 10, ///< to block numeric arguments on string functions
        noASSIGN  = 1 << 11, ///< to block assignement to constant i.e. "4=7"
        noIF      = 1 << 12,
        noELSE    = 1 << 13,
        sfSTART_OF_LINE = noOPT | noBC | noPOSTOP | noASSIGN | noIF | noELSE | noARG_SEP,
        noANY     = ~0       ///< All of he above flags set
    };

    QmuParserTokenReader(const QmuParserTokenReader &a_Reader) Q_DECL_NOEXCEPT;
    QmuParserTokenReader& operator=(const QmuParserTokenReader &a_Reader) Q_DECL_NOEXCEPT;
    void            Assign(const QmuParserTokenReader &a_Reader) Q_DECL_NOEXCEPT;

    void            SetParent(QmuParserBase *a_pParent) Q_DECL_NOEXCEPT;
    int             ExtractToken(const QString &a_szCharSet, QString &a_strTok, int a_iPos) const Q_DECL_NOEXCEPT;
    int             ExtractOperatorToken(QString &a_sTok, int a_iPos) const;

    bool            IsBuiltIn(token_type &a_Tok);
    bool            IsArgSep(token_type &a_Tok);
    bool            IsEOF(token_type &a_Tok);
    bool            IsInfixOpTok(token_type &a_Tok);
    bool            IsFunTok(token_type &a_Tok);
    bool            IsPostOpTok(token_type &a_Tok);
    bool            IsOprt(token_type &a_Tok);
    bool            IsValTok(token_type &a_Tok);
    bool            IsVarTok(token_type &a_Tok);
    bool            IsStrVarTok(token_type &a_Tok);
    bool            IsUndefVarTok(token_type &a_Tok) Q_DECL_NOEXCEPT;
    bool            IsString(token_type &a_Tok);
    void Q_NORETURN Error(EErrorCodes a_iErrc, int a_iPos = -1, const QString &a_sTok = QString() ) const;

    token_type& SaveBeforeReturn(const token_type &tok) Q_DECL_NOEXCEPT;

    QmuParserBase     *m_pParser;
    QString            m_strFormula;
    int                m_iPos;
    int                m_iSynFlags;
    bool               m_bIgnoreUndefVar;

    const funmap_type *m_pFunDef;
    const funmap_type *m_pPostOprtDef;
    const funmap_type *m_pInfixOprtDef;
    const funmap_type *m_pOprtDef;
    const valmap_type *m_pConstDef;
    const strmap_type *m_pStrVarDef;
    varmap_type       *m_pVarDef;         ///< The only non const pointer to parser internals
    facfun_type        m_pFactory;
    void              *m_pFactoryData;
    std::list<identfun_type> m_vIdentFun; ///< Value token identification function
    varmap_type        m_UsedVar;
    qreal              m_fZero;           ///< Dummy value of zero, referenced by undefined variables
    int                m_iBrackets;       ///< Keep count open brackets
    token_type         m_lastTok;
    QChar              m_cArgSep;         ///< The character used for separating function arguments
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the current position of the token reader in the formula string.
 *
 * @return #m_iPos
 * @throw nothrow
 */
inline int QmuParserTokenReader::GetPos() const Q_DECL_NOEXCEPT
{
    return m_iPos;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return a reference to the formula.
 *
 * @return #m_strFormula
 * @throw nothrow
 */
inline const QString& QmuParserTokenReader::GetExpr() const Q_DECL_NOEXCEPT
{
    return m_strFormula;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return a map containing the used variables only.
 */
inline varmap_type& QmuParserTokenReader::GetUsedVar() Q_DECL_NOEXCEPT
{
    return m_UsedVar;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Set Flag that contronls behaviour in case of undefined variables beeing found.
 *
 * If true, the parser does not throw an exception if an undefined variable is found. Otherwise it does. This variable
 * is used internally only! It supresses a "undefined variable" exception in GetUsedVar().
 * Those function should return a complete list of variables including
 * those the are not defined by the time of it's call.
 */
inline void QmuParserTokenReader::IgnoreUndefVar ( bool bIgnore ) Q_DECL_NOEXCEPT
{
    m_bIgnoreUndefVar = bIgnore;
}

//---------------------------------------------------------------------------------------------------------------------
inline void QmuParserTokenReader::SetArgSep ( char_type cArgSep ) Q_DECL_NOEXCEPT
{
    m_cArgSep = cArgSep;
}

//---------------------------------------------------------------------------------------------------------------------
inline QChar QmuParserTokenReader::GetArgSep() const Q_DECL_NOEXCEPT
{
    return m_cArgSep;
}
} // namespace qmu

#endif
