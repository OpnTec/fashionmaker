/***************************************************************************************************
 **
 **  Copyright (C) 2015 Roman Telezhynskyi
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

#ifndef QMUFORMULABASE_H
#define QMUFORMULABASE_H

#include "qmuparser_global.h"
#include "qmuparser.h"

namespace qmu
{

class QMUPARSERSHARED_EXPORT QmuFormulaBase : public QmuParser
{
public:
    QmuFormulaBase();
    virtual ~QmuFormulaBase() Q_DECL_OVERRIDE;

protected:
    void          InitCharacterSets();
    static qreal* AddVariable(const QString &a_szName, void *a_pUserData);
    void          SetSepForTr(bool osSeparator, bool fromUser);
    void          SetSepForEval();

    static void   RemoveAll(QMap<int, QString> &map, const QString &val);

private:
    Q_DISABLE_COPY(QmuFormulaBase)
};

} // namespace qmu

#endif // QMUFORMULABASE_H
