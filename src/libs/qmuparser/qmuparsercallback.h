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

#ifndef QMUPARSERCALLBACK_H
#define QMUPARSERCALLBACK_H

#include <qcompilerdetection.h>
#include <QString>
#include <map>
#include <QTypeInfo>
#include <QSharedDataPointer>

#include "qmuparserdef.h"
#include "qmuparsercallback_p.h"

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
    QmuParserCallback(fun_type2  a_pFun, bool a_bAllowOpti, int a_iPrec, EOprtAssociativity a_eOprtAsct);
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

#ifdef Q_COMPILER_RVALUE_REFS
    QmuParserCallback(QmuParserCallback &&a_Fun) Q_DECL_NOTHROW;
    QmuParserCallback &operator=(QmuParserCallback &&a_Fun) Q_DECL_NOTHROW;
#endif

    Q_REQUIRED_RESULT QmuParserCallback* Clone() const;

    bool               IsOptimizable() const;
    void*              GetAddr() const;
    ECmdCode           GetCode() const;
    ETypeCode          GetType() const;
    int                GetPri()  const;
    EOprtAssociativity GetAssociativity() const;
    int                GetArgc() const;
private:
    QSharedDataPointer<QmuParserCallbackData> d;
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

} // namespace qmu

Q_DECLARE_TYPEINFO(qmu::QmuParserCallback, Q_MOVABLE_TYPE);

#endif
