/***************************************************************************************************
 **
 **  Copyright (C) 2014 Roman Telezhynskyi <dismine(at)gmail.com>
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

#ifndef QMUTRANSLATION_H
#define QMUTRANSLATION_H

#include "qmuparser_global.h"
#include <QString>

namespace qmu
{

/**
 * @file
 * @brief The VTranslation class help store string for translation.
 *
 * I took idea from this article http://ololoepepe.blogspot.com/2013/08/qt.html.
 * As you know, if wrap string to a function translate, it will be marked for translation. No matter what namespace
 * contains this function. In class Translation used this circumstance.
 * This mean never change name of method translate!!!!!.
 * Instead of using QT_TRANSLATE_NOOP3 macros we can store strings in QMap.
 * Example:
 * create map and fill up its
 * QMap<QString, VTranslation> map;
 * map.insert("head_girth", VTranslation::translate("Measurements", "head_girth", "Around fullest part of Head."));
 * get translated string
 * map.value(measurement).translate();
 */
class QMUPARSERSHARED_EXPORT QmuTranslation
{
public:
    QmuTranslation();
    ~QmuTranslation(){}
    QmuTranslation(const QString &context, const QString &sourceText, const QString &disambiguation = 0, int n = -1);
    QmuTranslation &operator=(const QmuTranslation &tr);
    QmuTranslation(const QmuTranslation &tr);
    QString translate() const;
    static QmuTranslation translate(const QString &context, const QString &sourceText,
                                    const QString &disambiguation = 0, int n = -1);
    QString getMcontext() const;
    QString getMsourceText() const;
    QString getMdisambiguation() const;
    int     getN() const;
private:
    QString mcontext;
    QString msourceText;
    QString mdisambiguation;
    int     mn;
};

//---------------------------------------------------------------------------------------------------------------------
inline QString QmuTranslation::getMcontext() const
{
    return mcontext;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString QmuTranslation::getMsourceText() const
{
    return msourceText;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString QmuTranslation::getMdisambiguation() const
{
    return mdisambiguation;
}

//---------------------------------------------------------------------------------------------------------------------
inline int QmuTranslation::getN() const
{
    return mn;
}

} // namespace qmu

#endif // QMUTRANSLATION_H
