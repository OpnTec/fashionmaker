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
//Supressing specific warnings on gcc/g++ http://www.mr-edd.co.uk/blog/supressing_gcc_warnings
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type0 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 0 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type1 a_pFun, bool a_bAllowOpti, int a_iPrec, ECmdCode a_iCode )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 1 ), m_iPri ( a_iPrec ), m_eOprtAsct ( oaNONE ),
      m_iCode ( a_iCode ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Constructor for constructing funcstion callbacks taking two arguments.
 * @throw nothrow
 */
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type2 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 2 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
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
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type2 a_pFun, bool a_bAllowOpti, int a_iPrec,
                                       EOprtAssociativity a_eOprtAsct )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 2 ), m_iPri ( a_iPrec ), m_eOprtAsct ( a_eOprtAsct ),
      m_iCode ( cmOPRT_BIN ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type3 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 3 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}


//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type4 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 4 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}


//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type5 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 5 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type6 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 6 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type7 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 7 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type8 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 8 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type9 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 9 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( fun_type10 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 10 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type0 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 0 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type1 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 1 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Constructor for constructing funcstion callbacks taking two arguments.
 * @throw nothrow
 */
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type2 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 2 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type3 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 3 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type4 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 4 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type5 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 5 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type6 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 6 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type7 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 7 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type8 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 8 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type9 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 9 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( bulkfun_type10 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 10 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_BULK ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( multfun_type a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( -1 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC ), m_iType ( tpDBL ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( strfun_type1 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 0 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_STR ), m_iType ( tpSTR ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( strfun_type2 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 1 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_STR ), m_iType ( tpSTR ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
#ifdef __GNUC__
__extension__
#endif
QmuParserCallback::QmuParserCallback ( strfun_type3 a_pFun, bool a_bAllowOpti )
    : m_pFun ( reinterpret_cast<void*> ( a_pFun ) ), m_iArgc ( 2 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ),
      m_iCode ( cmFUNC_STR ), m_iType ( tpSTR ), m_bAllowOpti ( a_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Default constructor.
 * @throw nothrow
 */
QmuParserCallback::QmuParserCallback()
    : m_pFun ( 0 ), m_iArgc ( 0 ), m_iPri ( -1 ), m_eOprtAsct ( oaNONE ), m_iCode ( cmUNKNOWN ), m_iType ( tpVOID ),
      m_bAllowOpti ( 0 )
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Copy constructor.
 * @throw nothrow
 */
QmuParserCallback::QmuParserCallback ( const QmuParserCallback &ref )
    : m_pFun ( ref.m_pFun ), m_iArgc ( ref.m_iArgc ), m_iPri ( ref.m_iPri ), m_eOprtAsct ( ref.m_eOprtAsct ),
      m_iCode ( ref.m_iCode ), m_iType ( ref.m_iType ), m_bAllowOpti ( ref.m_bAllowOpti )
{}

//---------------------------------------------------------------------------------------------------------------------
QmuParserCallback &QmuParserCallback::operator=(const QmuParserCallback &a_Fun)
{
    m_pFun       = a_Fun.m_pFun;
    m_iArgc      = a_Fun.m_iArgc;
    m_bAllowOpti = a_Fun.m_bAllowOpti;
    m_iCode      = a_Fun.m_iCode;
    m_iType      = a_Fun.m_iType;
    m_iPri       = a_Fun.m_iPri;
    m_eOprtAsct  = a_Fun.m_eOprtAsct;
    return *this;
}

} // namespace qmu
