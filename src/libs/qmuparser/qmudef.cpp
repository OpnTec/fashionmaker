/***************************************************************************************************
 **
 **  Copyright (C) 2016 Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "qmudef.h"

#include <QLocale>
#include <QSet>

enum State
{
    Init     = 0,
    Sign     = 1,
    Thousand = 2,
    Mantissa = 3,
    Dot	     = 4,
    Abscissa = 5,
    ExpMark	 = 6,
    ExpSign	 = 7,
    Exponent = 8,
    Done	 = 9
};

enum InputToken
{
    InputSign       = 1,
    InputThousand   = 2,
    InputDigit      = 3,
    InputDot        = 4,
    InputExp        = 5
};

static const QChar QmuEOF = QChar(static_cast<ushort>(0xffff)); //guaranteed not to be a character.

//---------------------------------------------------------------------------------------------------------------------
static QChar GetChar(const QString &formula, int &index)
{
    if (index >= formula.size())
    {
        return QmuEOF;
    }

    return formula.at(index++);
}

//---------------------------------------------------------------------------------------------------------------------
static QChar EatWhiteSpace(const QString &formula, int &index)
{
    QChar c;
    do
    {
        c = GetChar(formula, index);
    }
    while ( c != QmuEOF && c.isSpace() );

    return c;
}

//---------------------------------------------------------------------------------------------------------------------
static int CheckChar(QChar &c, const QLocale &locale, const QChar &decimal, const QChar &thousand)
{
    INIT_LOCALE_VARIABLES(locale);
    Q_UNUSED(decimalPoint)
    Q_UNUSED(groupSeparator)

    if (c == positiveSign)
    {
        c = '+';
        return InputToken::InputSign;
    }
    else if (c == negativeSign)
    {
        c = '-';
        return InputToken::InputSign;
    }
    else if (c == sign0)
    {
        c = '0';
        return InputToken::InputDigit;
    }
    else if (c == sign1)
    {
        c = '1';
        return InputToken::InputDigit;
    }
    else if (c == sign2)
    {
        c = '2';
        return InputToken::InputDigit;
    }
    else if (c == sign3)
    {
        c = '3';
        return InputToken::InputDigit;
    }
    else if (c == sign4)
    {
        c = '4';
        return InputToken::InputDigit;
    }
    else if (c == sign5)
    {
        c = '5';
        return InputToken::InputDigit;
    }
    else if (c == sign6)
    {
        c = '6';
        return InputToken::InputDigit;
    }
    else if (c == sign7)
    {
        c = '7';
        return InputToken::InputDigit;
    }
    else if (c == sign8)
    {
        c = '8';
        return InputToken::InputDigit;
    }
    else if (c == sign9)
    {
        c = '9';
        return InputToken::InputDigit;
    }
    else if (c == decimal)
    {
        return InputToken::InputDot;
    }
    else if (c == thousand)
    {
        return InputToken::InputThousand;
    }
    else if (c == expLower)
    {
        c = 'e';
        return InputToken::InputExp;
    }
    else if (c == expUpper)
    {
        c = 'E';
        return InputToken::InputExp;
    }
    else
    {
        return 0;
    }

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
int ReadVal(const QString &formula, qreal &val, const QLocale &locale, const QChar &decimal, const QChar &thousand)
{
    // Must not be equal
    if (decimal == thousand || formula.isEmpty())
    {
        val = 0;
        return -1;
    }

    INIT_LOCALE_VARIABLES(locale);
    Q_UNUSED(decimalPoint)
    Q_UNUSED(groupSeparator)

    QSet<QChar> reserved;
    reserved << positiveSign
             << negativeSign
             << sign0
             << sign1
             << sign2
             << sign3
             << sign4
             << sign5
             << sign6
             << sign7
             << sign8
             << sign9
             << expUpper
             << expLower;

    if (reserved.contains(decimal) || reserved.contains(thousand))
    {
        val = 0;
        return -1;
    }

    // row - current state, column - new state
    static uchar table[9][6] =
    {
        /*    None	     InputSign     InputThousand      InputDigit     InputDot      InputExp */
        { 0,	       State::Sign,    0,               State::Mantissa, State::Dot,  0,	         }, // Init
        { 0,	       0,	           0,               State::Mantissa, State::Dot,  0,	         }, // Sign
        { 0,	       0,	           0,               State::Mantissa, 0,	          0,	         }, // Thousand
        { State::Done, State::Done,    State::Thousand, State::Mantissa, State::Dot,  State::ExpMark,}, // Mantissa
        { 0,	       0,	           0,               State::Abscissa, 0,	          0,	         }, // Dot
        { State::Done, State::Done,    0,               State::Abscissa, 0,           State::ExpMark,}, // Abscissa
        { 0,	       State::ExpSign, 0,               State::Exponent, 0,	          0,	         }, // ExpMark
        { 0,	       0,	           0,               State::Exponent, 0,	          0,	         }, // ExpSign
        { State::Done, 0,              0,               State::Exponent, 0,           State::Done	 }  // Exponent
    };

    int state = State::Init;	// parse state
    QString buf;

    int index = 0; // start position
    QChar c = EatWhiteSpace(formula, index);

    while ( true )
    {
        const int input = CheckChar(c, locale, decimal, thousand);// input token

        state = table[state][input];

        if (state == 0)
        {
            val = 0;
            return -1;
        }
        else if (state == Done)
        {
            // Convert to C locale
            QLocale cLocale(QLocale::C);
            const QChar cDecimal = cLocale.decimalPoint();
            const QChar cThousand = cLocale.groupSeparator();
            if (locale != cLocale && (cDecimal != decimal || cThousand != thousand))
            {
                if (decimal == cThousand)
                {// Handle reverse to C locale case: thousand '.', decimal ','
                    const QChar tmpThousand = '@';
                    buf.replace(thousand, tmpThousand);
                    buf.replace(decimal, cDecimal);
                    buf.replace(tmpThousand, cThousand);
                }
                else
                {
                    buf.replace(thousand, cThousand);
                    buf.replace(decimal, cDecimal);
                }
            }

            bool ok = false;
            const double d = cLocale.toDouble(buf, &ok);
            if (ok)
            {
                val = d;
                return buf.size();
            }
            else
            {
                val = 0;
                return -1;
            }
        }

        buf.append(c);
        c = GetChar(formula, index);
    }

    return -1;
}

//---------------------------------------------------------------------------------------------------------------------
QString NameRegExp()
{
    static QString regex;

    if (regex.isEmpty())
    {
        const QList<QLocale> allLocales =
                QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);

        QString positiveSigns;
        QString negativeSigns;
        QString decimalPoints;
        QString groupSeparators;

        for(int i = 0; i < allLocales.size(); ++i)
        {
            if (not positiveSigns.contains(allLocales.at(i).positiveSign()))
            {
                positiveSigns.append(allLocales.at(i).positiveSign());
            }

            if (not negativeSigns.contains(allLocales.at(i).negativeSign()))
            {
                negativeSigns.append(allLocales.at(i).negativeSign());
            }

            if (not decimalPoints.contains(allLocales.at(i).decimalPoint()))
            {
                decimalPoints.append(allLocales.at(i).decimalPoint());
            }

            if (not groupSeparators.contains(allLocales.at(i).groupSeparator()))
            {
                groupSeparators.append(allLocales.at(i).groupSeparator());
            }
        }

        negativeSigns.replace('-', "\\-");
        groupSeparators.remove('\'');

        //Same regexp in pattern.xsd shema file. Don't forget to synchronize.
        // \p{Nd} - \p{Decimal_Digit_Number}
        // \p{Zs} - \p{Space_Separator}
        regex = QString("^([^\\p{Nd}\\p{Zs}*/&|!<>^\\()%1%2%3%4=?:;'\"]){1,1}"
                        "([^\\p{Zs}*/&|!<>^\\()%1%2%3%4=?:;\"]){0,}$")
                .arg(negativeSigns).arg(positiveSigns).arg(decimalPoints).arg(groupSeparators);
    }

    return regex;
}
