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

#include "qmutranslation.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QLocale>

namespace qmu
{

//---------------------------------------------------------------------------------------------------------------------
QmuTranslation QmuTranslation::translate(const QString &context, const QString &sourceText,
                                         const QString &disambiguation, int n)
{
    if (n < 0)
    {
        n = -1;
    }
    QmuTranslation t(context, sourceText, disambiguation, n);
    return t;
}

//---------------------------------------------------------------------------------------------------------------------
QmuTranslation::QmuTranslation()
    : mcontext(), msourceText(), mdisambiguation(), mn(-1), localeName(), cachedTranslation()
{}

//---------------------------------------------------------------------------------------------------------------------
QmuTranslation::QmuTranslation(const QString &context, const QString &sourceText, const QString &disambiguation, int n)
    : mcontext(context),
      msourceText(sourceText),
      mdisambiguation(disambiguation),
      mn(n),
      localeName(),
      cachedTranslation()
{}

//---------------------------------------------------------------------------------------------------------------------
QmuTranslation &QmuTranslation::operator=(const QmuTranslation &tr)
{
    if ( &tr == this )
    {
        return *this;
    }
    this->mcontext = tr.getMcontext();
    this->msourceText = tr.getMsourceText();
    this->mdisambiguation = tr.getMdisambiguation();
    this->mn = tr.getN();
    this->localeName.clear();
    this->cachedTranslation.clear();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
QmuTranslation::QmuTranslation(const QmuTranslation &tr)
    : mcontext(tr.mcontext),
      msourceText(tr.msourceText),
      mdisambiguation(tr.mdisambiguation),
      mn(tr.mn),
      localeName(),
      cachedTranslation()
{}

//---------------------------------------------------------------------------------------------------------------------
QString QmuTranslation::translate(const QString &locale) const
{
    if (cachedTranslation.isEmpty() || locale.isEmpty() || localeName != locale)
    {
        if (not locale.isEmpty())
        {
            localeName = locale;
        }

        cachedTranslation = QCoreApplication::translate(mcontext.toUtf8().constData(), msourceText.toUtf8().constData(),
                                                        mdisambiguation.toUtf8().constData(), mn);
    }
    return cachedTranslation;
}

} // namespace qmu
