/**********************************************************************************************************************
 **
 **  @file   qmuparsercallback_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 4, 2018
 **
 **  @brief
 **  @copyright
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
 **********************************************************************************************************************/
#ifndef QMUPARSERCALLBACK_P_H
#define QMUPARSERCALLBACK_P_H

#include <QSharedData>
#include "qmuparserdef.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/defglobal.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

namespace qmu
{

class QmuParserCallbackData : public QSharedData
{
public:
    QmuParserCallbackData();
    QmuParserCallbackData(fun_type0  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(fun_type1  a_pFun, bool a_bAllowOpti, int a_iPrec = -1, ECmdCode a_iCode=cmFUNC);
    QmuParserCallbackData(fun_type2  a_pFun, bool a_bAllowOpti, int a_iPrec, EOprtAssociativity a_eOprtAsct);
    QmuParserCallbackData(fun_type2  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(fun_type3  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(fun_type4  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(fun_type5  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(fun_type6  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(fun_type7  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(fun_type8  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(fun_type9  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(fun_type10 a_pFun, bool a_bAllowOpti);

    QmuParserCallbackData(bulkfun_type0  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type1  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type2  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type3  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type4  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type5  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type6  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type7  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type8  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type9  a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(bulkfun_type10 a_pFun, bool a_bAllowOpti);

    QmuParserCallbackData(multfun_type a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(strfun_type1 a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(strfun_type2 a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(strfun_type3 a_pFun, bool a_bAllowOpti);
    QmuParserCallbackData(const QmuParserCallbackData &a_Fun);
    virtual ~QmuParserCallbackData();

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

private:
    Q_DISABLE_ASSIGN(QmuParserCallbackData)
};

}
#endif // QMUPARSERCALLBACK_P_H
