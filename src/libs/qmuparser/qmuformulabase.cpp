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

#include "qmuformulabase.h"

namespace qmu
{

//---------------------------------------------------------------------------------------------------------------------
QmuFormulaBase::QmuFormulaBase()
    :QmuParser()
{
}

//---------------------------------------------------------------------------------------------------------------------
QmuFormulaBase::~QmuFormulaBase()
{
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief InitCharacterSets init character set for parser.
 *
 * QMuParser require setting character set for legal characters. Because we try make our expresion language independent
 * we set all posible unique characters from all alphabets.
 *
 */
void QmuFormulaBase::InitCharacterSets()
{
    //String with all unique symbols for supported alphabets.
    //See script alphabets.py for generation and more information.
    //Note. MSVC doesn't support normal string concatenation for long string. That's why we use QStringList in this
    //case.
    const QStringList symbols = QStringList()
                                        << "ցЀĆЈVӧĎАғΕĖӅИқΝĞơРңњΥҔĦШҫ̆جگĮаҳѕεشԶиһνԾрÃυلՆӝшËՎїPÓՖXӛӟŞãզhëծpóӞնxßվāŁЃֆĉЋ"
                                        << "CŬđҐГΒęҘЛΚŘġҠУGاհЫدԱҰгβطԹõлκKՁÀуςهՉÈыvیՑÐSOřӘћաőcӐթèkàѓżűðsķչøӥӔĀփӣІĈЎ"
                                        << "ґĐΗЖҙĘȚΟОҡĠآΧЦتЮұİزηжԸغẽοоÁՀقχцÉՈيюÑՐђӋіәťӆўáŠĺѐfөըnñŰӤӨӹոľЁրăЉŭċБӸēłΔҖ"
                                        << "ЙŤěΜӜDСձģΤӰЩīņحҮбưԳصδHйԻŇμӲӴсՃمτƠщՋєLQŹՓŕÖYśÞaգĽæiŽիӓîqճöyջþĂօЄӦĊЌΑĒДҗј"
                                        << "ΙȘĚМΡéĵĢФūӚΩبĪЬүќαذԲдҷιظԺмρՂфÇωوՊьÏՒTŚĻJբdçժlïӪղtպӫAւąЇčŃЏĕӯЗΖEțŮĝПΞأĥ"
                                        << "ĹЧΦثÆӳЯIسŲԵзζԽпξكՅÄчφNMՍӌяӢՕÔWÎŝÜџёźեägխoӒյôwĶBžսüЂĄև̈ЊČƏљΓВҕĔӮΛКĜΣТҥĤک"
                                        << "ЪƯخγвŅԴŪضλкԼĴσтÅՄنъÍՌRӕՔZÝŜbåդﻩjíլļrӵմzýռپêЅքćچЍďӱҒЕůėژșΘØҚНğńءΠFҢХħΨҪ"
                                        << "ЭųįҶرҲеԷňعθҺнԿفπÂхՇψÊэšՏÒUəÚѝŻşҤӑâeէŐımկòuշÕúտŔ";

    // Defining identifier character sets
    DefineNameChars(QStringLiteral("0123456789_@#") + symbols.join(""));
    DefineOprtChars(symbols.join("") + QStringLiteral("+-*^/?<>=!$%&|~_"));
}

//---------------------------------------------------------------------------------------------------------------------
// Factory function for creating new parser variables
// This could as well be a function performing database queries.
qreal *QmuFormulaBase::AddVariable(const QString &a_szName, void *a_pUserData)
{
    Q_UNUSED(a_szName)
    Q_UNUSED(a_pUserData)

    static qreal value = 0;
    return &value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSepForTr set separators for translation expression.
 * @param fromUser true if expression come from user (from dialog).
 */
void QmuFormulaBase::SetSepForTr(bool osSeparator, bool fromUser)
{
    if (fromUser)
    {
        if (osSeparator)
        {
            const QLocale loc = QLocale::system();
            SetDecSep(loc.decimalPoint().toLatin1());
            SetThousandsSep(loc.groupSeparator().toLatin1());
            SetArgSep(';');
            return;
        }
    }

    SetSepForEval();//Same separators (internal) as for eval.
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSepForEval set separators for eval. Each expression eval in internal (C) locale.
 */
void QmuFormulaBase::SetSepForEval()
{
    SetArgSep(';');
    SetThousandsSep(',');
    SetDecSep('.');
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RemoveAll remove token from token list.
 *
 * Standard Qt class QMap doesn't have method RemoveAll.
 * Example: remove "-" from tokens list if exist. If don't do that unary minus operation will broken.
 *
 * @param map map with tokens
 * @param val token that need delete
 */
void QmuFormulaBase::RemoveAll(QMap<int, QString> &map, const QString &val)
{
    const QList<int> listKeys = map.keys(val);//Take all keys that contain token.
    if (listKeys.size() > 0)
    {
        for (int i = 0; i < listKeys.size(); ++i)
        {
            map.remove(listKeys.at(i));
        }
    }
}

}// namespace qmu
