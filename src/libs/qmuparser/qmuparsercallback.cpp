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

#include "qmuparsercallback.h"

/**
 * @file
 * @brief Implementation of the parser callback class.
 */

namespace qmu
{
//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type0 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type1 a_pFun, bool a_bAllowOpti, int a_iPrec, ECmdCode a_iCode )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti, a_iPrec, a_iCode))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Constructor for constructing funcstion callbacks taking two arguments.
 * @throw nothrow
 */
QmuParserCallback::QmuParserCallback ( fun_type2 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Constructor for constructing binary operator callbacks.
 * @param a_pFun Pointer to a static function taking two arguments
 * @param a_bAllowOpti A flag indicating this funcation can be optimized
 * @param a_iPrec The operator precedence
 * @param a_eOprtAsct The operators associativity
 * @throw nothrow
 */
QmuParserCallback::QmuParserCallback (fun_type2 a_pFun, bool a_bAllowOpti, int a_iPrec, EOprtAssociativity a_eOprtAsct)
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti, a_iPrec, a_eOprtAsct))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type3 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type4 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type5 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type6 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type7 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type8 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type9 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( fun_type10 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type0 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type1 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Constructor for constructing funcstion callbacks taking two arguments.
 * @throw nothrow
 */
QmuParserCallback::QmuParserCallback ( bulkfun_type2 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type3 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type4 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type5 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type6 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type7 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type8 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type9 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( bulkfun_type10 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( multfun_type a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( strfun_type1 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( strfun_type2 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback::QmuParserCallback ( strfun_type3 a_pFun, bool a_bAllowOpti )
    : d (new QmuParserCallbackData(a_pFun, a_bAllowOpti))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Default constructor.
 * @throw nothrow
 */
QmuParserCallback::QmuParserCallback()
    : d (new QmuParserCallbackData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Copy constructor.
 * @throw nothrow
 */
QmuParserCallback::QmuParserCallback (const QmuParserCallback &a_Fun )
    : d (a_Fun.d)
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback &QmuParserCallback::operator=(const QmuParserCallback &a_Fun)
{
    if ( &a_Fun == this )
    {
        return *this;
    }
    d = a_Fun.d;
    return *this;
}

#ifdef Q_COMPILER_RVALUE_REFS
//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback &QmuParserCallback::operator=(QmuParserCallback &&a_Fun) Q_DECL_NOTHROW
{
    Swap(a_Fun);
    return *this;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
void QmuParserCallback::Swap(QmuParserCallback &a_Fun) Q_DECL_NOTHROW
{
    std::swap(d, a_Fun.d);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return true if the function is conservative.
 *
 * Conservative functions return always the same result for the same argument.
 * @throw nothrow
 */
// cppcheck-suppress unusedFunction
bool QmuParserCallback::IsOptimizable() const
{
    return d->m_bAllowOpti;
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
void* QmuParserCallback::GetAddr() const
{
    return d->m_pFun;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the callback code.
*/
ECmdCode  QmuParserCallback::GetCode() const
{
    return d->m_iCode;
}

//---------------------------------------------------------------------------------------------------------------------
ETypeCode QmuParserCallback::GetType() const
{
    return d->m_iType;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the operator precedence.
 * @throw nothrown
 *
 * Only valid if the callback token is an operator token (binary or infix).
 */
int QmuParserCallback::GetPri()  const
{
    return d->m_iPri;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Return the operators associativity.
 * @throw nothrown
 *
 * Only valid if the callback token is a binary operator token.
 */
EOprtAssociativity QmuParserCallback::GetAssociativity() const
{
    return d->m_eOprtAsct;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the number of function Arguments.
 */
int QmuParserCallback::GetArgc() const
{
    return d->m_iArgc;
}

//---------------------------------------------------------------------------------------------------------------------
//Supressing specific warnings on gcc/g++ http://www.mr-edd.co.uk/blog/supressing_gcc_warnings
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData()
    : m_pFun ( nullptr ), m_iArgc ( 0 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ), m_iCode ( cmUNKNOWN ),
      m_iType ( tpVOID ), m_bAllowOpti ( 0 )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type0 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 0 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type1 a_pFun, bool a_bAllowOpti, int a_iPrec, ECmdCode a_iCode )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 1 ), m_iPri ( a_iPrec ), m_eOprtAsct ( oaNONE ),
      m_iCode ( a_iCode ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type2 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 2 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData (fun_type2 a_pFun, bool a_bAllowOpti, int a_iPrec, EOprtAssociativity a_eOprtAsct)
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 2 ), m_iPri ( a_iPrec ), m_eOprtAsct ( a_eOprtAsct ),
      m_iCode ( cmOPRT_BIN ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type3 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 3 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type4 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 4 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type5 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 5 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type6 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 6 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type7 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 7 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type8 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 8 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type9 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 9 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( fun_type10 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 10 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type0 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 0 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type1 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 1 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type2 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 2 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type3 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 3 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type4 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 4 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type5 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 5 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type6 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 6 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type7 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 7 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type8 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 8 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type9 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 9 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( bulkfun_type10 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 10 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( multfun_type a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( -1 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( strfun_type1 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 0 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_STR ), m_iType ( tpSTR ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( strfun_type2 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 1 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_STR ), m_iType ( tpSTR ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallbackData::QmuParserCallbackData ( strfun_type3 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 2 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_STR ), m_iType ( tpSTR ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallbackData::QmuParserCallbackData(const QmuParserCallbackData &a_Fun)
    : QSharedData(a_Fun), m_pFun(a_Fun.m_pFun), m_iArgc(a_Fun.m_iArgc), m_iPri(a_Fun.m_iPri),
      m_eOprtAsct(a_Fun.m_eOprtAsct), m_iCode(a_Fun.m_iCode), m_iType(a_Fun.m_iType), m_bAllowOpti(a_Fun.m_bAllowOpti)
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallbackData::~QmuParserCallbackData()
{}

} // namespace qmu
