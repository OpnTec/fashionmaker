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

#ifndef QMUPARSERBYTECODE_H
#define QMUPARSERBYTECODE_H

#include "qmuparserdef.h"
#include "qmuparsertoken.h"
#include "qmudef.h"

/**
 * @file
 * @brief Definition of the parser bytecode class.
 */

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wnested-anon-types")
namespace qmu
{
struct SToken
{
    ECmdCode Cmd;
    int StackPos;

    union //
    {
        struct //SValData
        {
            qreal *ptr;
            qreal  data;
            qreal  data2;
        } Val;

        struct //SFunData
        {
            // Note: generic_fun_type is merely a placeholder. The real type could be
            //       anything between gun_type1 and fun_type9. I can't use a void
            //       pointer due to constraints in the ANSI standard which allows
            //       data pointers and function pointers to differ in size.
            generic_fun_type ptr;
            int   argc;
            int   idx;
        } Fun;

        struct //SOprtData
        {
            qreal *ptr;
            int offset;
        } Oprt;
    };
};

QT_WARNING_POP

/**
 * @brief Bytecode implementation of the Math Parser.
 *
 * The bytecode contains the formula converted to revers polish notation stored in a continious
 * memory area. Associated with this data are operator codes, variable pointers, constant
 * values and function pointers. Those are necessary in order to calculate the result.
 * All those data items will be casted to the underlying datatype of the bytecode.
 *
 * @author (C) 2004-2013 Ingo Berg
 */
class QmuParserByteCode
{
public:
    QmuParserByteCode();
    QmuParserByteCode(const QmuParserByteCode &a_ByteCode);
    QmuParserByteCode& operator=(const QmuParserByteCode &a_ByteCode);
    void          Assign(const QmuParserByteCode &a_ByteCode);
    void          AddVar(qreal *a_pVar);
    void          AddVal(qreal a_fVal);
    void          AddOp(ECmdCode a_Oprt);
    void          AddIfElse(ECmdCode a_Oprt);
    void          AddAssignOp(qreal *a_pVar);
    void          AddFun(generic_fun_type a_pFun, int a_iArgc);
    void          AddBulkFun(generic_fun_type a_pFun, int a_iArgc);
    void          AddStrFun(generic_fun_type a_pFun, int a_iArgc, int a_iIdx);
    void          EnableOptimizer(bool bStat);
    void          Finalize();
    void          clear();
    int           GetMaxStackSize() const;
    int           GetSize() const;
    const SToken* GetBase() const;
    void          AsciiDump();
private:
    /** @brief Token type for internal use only. */
    typedef QmuParserToken<qreal, string_type> token_type;

    /** @brief Token vector for storing the RPN. */
    typedef QVector<SToken> rpn_type;

    /** @brief Position in the Calculation array. */
    unsigned m_iStackPos;

    /** @brief Maximum size needed for the stack. */
    unsigned m_iMaxStackSize;

    /** @brief The actual rpn storage. */
    rpn_type m_vRPN;

    bool     m_bEnableOptimizer;

    void ConstantFolding(ECmdCode a_Oprt);
};

//---------------------------------------------------------------------------------------------------------------------
inline void QmuParserByteCode::EnableOptimizer(bool bStat)
{
    m_bEnableOptimizer = bStat;
}

//---------------------------------------------------------------------------------------------------------------------
inline int QmuParserByteCode::GetMaxStackSize() const
{
    return static_cast<int>(m_iMaxStackSize+1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the number of entries in the bytecode.
 */
// cppcheck-suppress unusedFunction
inline int QmuParserByteCode::GetSize() const
{
    return m_vRPN.size();
}

} // namespace qmu
#endif
