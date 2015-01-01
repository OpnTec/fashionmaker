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

#ifndef QMUPARSERCALLBACK_H
#define QMUPARSERCALLBACK_H

#include "qmuparserdef.h"

/**
 * @file
 * @brief Definition of the parser callback class.
 */

namespace qmu
{

/**
 * @brief Encapsulation of prototypes for a numerical parser function.
 *
 * Encapsulates the prototyp for numerical parser functions. The class stores the number of arguments for parser
 * functions as well as additional flags indication the function is non optimizeable. The pointer to the callback
 * function pointer is stored as void* and needs to be casted according to the argument count. Negative argument counts
 * indicate a parser function with a variable number of arguments.
 *
 * @author (C) 2004-2011 Ingo Berg
 */
class QmuParserCallback
{
public:
    QmuParserCallback(fun_type0  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(fun_type1  a_pFun, bool a_bAllowOpti, int a_iPrec = -1, ECmdCode a_iCode=cmFUNC);
    QmuParserCallback(fun_type2  a_pFun, bool a_bAllowOpti, int a_iPrec, EOprtAssociativity a_eAssociativity);
    QmuParserCallback(fun_type2  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(fun_type3  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(fun_type4  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(fun_type5  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(fun_type6  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(fun_type7  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(fun_type8  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(fun_type9  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(fun_type10 a_pFun, bool a_bAllowOpti);

    QmuParserCallback(bulkfun_type0  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type1  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type2  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type3  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type4  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type5  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type6  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type7  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type8  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type9  a_pFun, bool a_bAllowOpti);
    QmuParserCallback(bulkfun_type10 a_pFun, bool a_bAllowOpti);

    QmuParserCallback(multfun_type a_pFun, bool a_bAllowOpti);
    QmuParserCallback(strfun_type1 a_pFun, bool a_bAllowOpti);
    QmuParserCallback(strfun_type2 a_pFun, bool a_bAllowOpti);
    QmuParserCallback(strfun_type3 a_pFun, bool a_bAllowOpti);
    QmuParserCallback();
    QmuParserCallback(const QmuParserCallback &a_Fun);
    QmuParserCallback &operator=(const QmuParserCallback &a_Fun);

    QmuParserCallback* Clone() const;
    bool               IsOptimizable() const;
    void*              GetAddr() const;
    ECmdCode           GetCode() const;
    ETypeCode          GetType() const;
    int                GetPri()  const;
    EOprtAssociativity GetAssociativity() const;
    int                GetArgc() const;
private:
    void *m_pFun;                   ///< Pointer to the callback function, casted to void

    /**
     * @brief Number of numeric function arguments
     *
     * This number is negative for functions with variable number of arguments. in this cases
     * they represent the actual number of arguments found.
     */
    int   m_iArgc;
    int   m_iPri;                   ///< Valid only for binary and infix operators; Operator precedence.
    EOprtAssociativity m_eOprtAsct; ///< Operator associativity; Valid only for binary operators
    ECmdCode  m_iCode;
    ETypeCode m_iType;
    bool  m_bAllowOpti;             ///< Flag indication optimizeability
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Container for Callback objects.
 */
typedef std::map<QString, QmuParserCallback> funmap_type;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clone this instance and return a pointer to the new instance.
 */
inline QmuParserCallback* QmuParserCallback::Clone() const
{
    return new QmuParserCallback ( *this );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return true if the function is conservative.
 *
 * Conservative functions return always the same result for the same argument.
 * @throw nothrow
 */
// cppcheck-suppress unusedFunction
inline bool QmuParserCallback::IsOptimizable() const
{
    return m_bAllowOpti;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Get the callback address for the parser function.
 *
 * The type of the address is void. It needs to be recasted according to the argument number to the right type.
 *
 * @throw nothrow
 * @return #pFun
 */
inline void* QmuParserCallback::GetAddr() const
{
    return m_pFun;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the callback code.
*/
inline ECmdCode  QmuParserCallback::GetCode() const
{
    return m_iCode;
}

//---------------------------------------------------------------------------------------------------------------------
inline ETypeCode QmuParserCallback::GetType() const
{
    return m_iType;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the operator precedence.
 * @throw nothrown
 *
 * Only valid if the callback token is an operator token (binary or infix).
 */
inline int QmuParserCallback::GetPri()  const
{
    return m_iPri;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the operators associativity.
 * @throw nothrown
 *
 * Only valid if the callback token is a binary operator token.
 */
inline EOprtAssociativity QmuParserCallback::GetAssociativity() const
{
    return m_eOprtAsct;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the number of function Arguments.
 */
inline int QmuParserCallback::GetArgc() const
{
    return m_iArgc;
}

} // namespace qmu

#endif
