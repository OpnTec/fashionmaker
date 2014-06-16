/************************************************************************
 **
 **  @file   calculator.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "calculator.h"
#include <QDebug>
#include <QSettings>
#include "../widgets/vapplication.h"
#include "vcontainer.h"

using namespace qmu;

int Calculator::iVal = -1;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Calculator class constructor. Make easy initialization math parser.
 *
 * This constructor hide initialization variables, operators, character sets.
 * Use this constuctor for evaluation formula. All formulas must be converted to internal look.
 * Example:
 *
 * const QString formula = qApp->FormulaFromUser(edit->text());
 * Calculator *cal = new Calculator(data);
 * const qreal result = cal->EvalFormula(formula);
 * delete cal;
 *
 * @param data pointer to a variable container.
 */
Calculator::Calculator(const VContainer *data)
    :QmuParser(), vVarVal(nullptr)
{
    InitCharacterSets();

    // Add variables
    InitVariables(data);

    // Add unary operators
    DefinePostfixOprt(cm_Oprt, CmUnit);
    DefinePostfixOprt(mm_Oprt, MmUnit);
    DefinePostfixOprt(in_Oprt, InchUnit);

    SetArgSep(',');
    SetDecSep('.');
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Calculator class constructor. Make easy initialization math parser.
 *
 * This constructor hide initialization variables, operators, character sets.
 * Use this constuctor to get tokens from formula. All formulas must be converted to external look.
 * Example:
 *
 * Calculator *cal = new Calculator(formula, false);
 * tokens = cal->GetTokens();
 * numbers = cal->GetNumbers();
 * delete cal;
 *
 * @param formula string with formula.
 * @param fromUser true if we parse formula from user
 */
Calculator::Calculator(const QString &formula, bool fromUser)
    :QmuParser(), vVarVal(nullptr)
{
    InitCharacterSets();
    SetVarFactory(AddVariable, this);

    // Add unary operators
    if(fromUser)
    {
        DefinePostfixOprt(qApp->PostfixOperator(cm_Oprt), CmUnit);
        DefinePostfixOprt(qApp->PostfixOperator(mm_Oprt), MmUnit);
        DefinePostfixOprt(qApp->PostfixOperator(in_Oprt), InchUnit);

        QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                           QApplication::applicationName());
        bool osSeparatorValue = settings.value("configuration/osSeparator", 1).toBool();

        if (osSeparatorValue)
        {
            QLocale loc = QLocale::system();
            SetDecSep(loc.decimalPoint().toLatin1());
            SetThousandsSep(loc.groupSeparator().toLatin1());
            SetArgSep(';');
        }
        else
        {
            SetArgSep(',');
            SetDecSep('.');
        }
    }
    else
    {
        DefinePostfixOprt(cm_Oprt, CmUnit);
        DefinePostfixOprt(mm_Oprt, MmUnit);
        DefinePostfixOprt(in_Oprt, InchUnit);

        SetArgSep(',');
        SetDecSep('.');
    }

    SetExpr(formula);
    //Need run for making tokens. Don't catch exception here, because it will show us in dialog that formula has error.
    Eval();
}

Calculator::~Calculator()
{
    delete [] vVarVal;
    Calculator::iVal = -1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief eval calculate formula.
 * @param formula string of formula.
 * @return value of formula.
 */
qreal Calculator::EvalFormula(const QString &formula)
{
    SetExpr(formula);
    return Eval();
}

//---------------------------------------------------------------------------------------------------------------------
void Calculator::InitVariables(const VContainer *data)
{
    int num = 0;
    if (qApp->patternType() == MeasurementsType::Standard)
    {
        num +=2;
    }

    const QHash<QString, qreal> *lengthLines = data->DataLengthLines();
    num += lengthLines->size();

    const QHash<QString, qreal> *lengthSplines = data->DataLengthSplines();
    num += lengthSplines->size();

    const QHash<QString, qreal> *lengthArcs = data->DataLengthArcs();
    num += lengthArcs->size();

    const QHash<QString, qreal> *lineAngles = data->DataLineAngles();
    num += lineAngles->size();

    const QHash<QString, VMeasurement> *measurements = data->DataMeasurements();
    num += measurements->size();

    const QHash<QString, VIncrement> *increments = data->DataIncrements();
    num += increments->size();

    vVarVal = new qreal[num];
    int j = 0;

    if (qApp->patternType() == MeasurementsType::Standard)
    {
        vVarVal[j] = data->size();
        DefineVar(data->SizeName(), &vVarVal[j]);
        ++j;

        vVarVal[j] = data->height();
        DefineVar(data->HeightName(), &vVarVal[j]);
        ++j;
    }

    {
        QHash<QString, qreal>::const_iterator i = lengthLines->constBegin();
        while (i != lengthLines->constEnd())
        {
            vVarVal[j] = i.value();
            DefineVar(i.key(), &vVarVal[j]);
            ++j;
            ++i;
        }
    }

    {
        QHash<QString, qreal>::const_iterator i = lengthSplines->constBegin();
        while (i != lengthSplines->constEnd())
        {
            vVarVal[j] = i.value();
            DefineVar(i.key(), &vVarVal[j]);
            ++j;
            ++i;
        }
    }

    {
        QHash<QString, qreal>::const_iterator i = lengthArcs->constBegin();
        while (i != lengthArcs->constEnd())
        {
            vVarVal[j] = i.value();
            DefineVar(i.key(), &vVarVal[j]);
            ++j;
            ++i;
        }
    }

    {
        QHash<QString, qreal>::const_iterator i = lineAngles->constBegin();
        while (i != lineAngles->constEnd())
        {
            vVarVal[j] = i.value();
            DefineVar(i.key(), &vVarVal[j]);
            ++j;
            ++i;
        }
    }

    {
        QHash<QString, VMeasurement>::const_iterator i = measurements->constBegin();
        while (i != measurements->constEnd())
        {
            if (qApp->patternType() == MeasurementsType::Standard)
            {
                vVarVal[j] = i.value().GetValue(data->size(), data->height());
            }
            else
            {
                vVarVal[j] = i.value().GetValue();
            }
            DefineVar(i.key(), &vVarVal[j]);
            ++j;
            ++i;
        }
    }

    {
        QHash<QString, VIncrement>::const_iterator i = increments->constBegin();
        while (i != increments->constEnd())
        {
            if (qApp->patternType() == MeasurementsType::Standard)
            {
                vVarVal[j] = i.value().GetValue(data->size(), data->height());
            }
            else
            {
                vVarVal[j] = i.value().GetValue();
            }
            DefineVar(i.key(), &vVarVal[j]);
            ++j;
            ++i;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Calculator::InitCharacterSets()
{
    //String with all unique symbols for supported alpabets.
    // See script alphabets.py for generation and more information.
    const QString symbols = QStringLiteral("ցЀĆЈVӧĎАғΕĖӅИқΝĞơРңњΥĦШҫ̆جگĮаҳѕεشԶиһνԾрυلՆӝшËՎҔPÓՖXӛӟŞӣզhëծpóӞնxßվāŁЃֆĉЋCŬđ"
                                           "ҐГΒęҘЛΚŘġҠУGاհЫدԱҰгβطԹõлκKՁÀуςهՉÈыvیՑÐSOřӘћաőcӐթèkàѓżűðsķչøӥӔĀփїІĈЎґĐΗЖҙĘȚ"
                                           "ΟОҡĠآΧЦتЮұİزηжԸغοоÁՀقχцÉՈيюÑՐђӋіәťӆўáŠĺѐfөըnñŰӤӨӹոľЁրăЉŭċБӸēłΔҖЙŤěΜӜDСձģΤӰ"
                                           "ЩīņحҮбưԳصδHйԻŇμӲӴсՃمτƠщՋєLQŹՓŕÖYśÞaգĽæiŽիӓîqճöyջþĂօЄӦĊЌΑĒДҗјΙȘĚМΡéĵĢФūӚΩبĪ"
                                           "ЬүќαذԲдҷιظԺмρՂфÇωوՊьÏՒTŚĻJբdçժlïӪղtպӫAւąЇčŃЏĕӯЗΖEțŮĝПΞأĥĹЧΦثÆӳЯIسŲԵзζԽпξكՅ"
                                           "ÄчφNMՍӌяӢՕÔWÎŝÜџёźեägխoӒյôwĶBžսüЂĄև̈ЊČƏљΓВҕĔӮΛКĜΣТҥĤکЪƯخγвŅԴŪضλкԼĴσтÅՄنъÍՌR"
                                           "ӕՔZÝŜbåդﻩjíլļrӵմzýռپêЅքćچЍďӱҒЕůėژșΘØҚНğńءΠFҢХħΨҪЭųįҶرҲеԷňعθҺнԿفπÂхՇψÊэšՏÒU"
                                           "əÚѝŻşҤӑâeէŐımկòuշÕúտŔ");

    // Defining identifier character sets
    DefineNameChars(QStringLiteral("0123456789_") + symbols);
    DefineOprtChars(symbols + QStringLiteral("+-*^/?<>=#!$%&|~'_"));
}

//---------------------------------------------------------------------------------------------------------------------
qreal Calculator::CmUnit(qreal val)
{
    qreal unit = val;
    switch(qApp->patternUnit())
    {
        case Unit::Mm:
            unit = val * 10.0;
            break;
        case Unit::Cm:
            break;
        case Unit::Inch:
            unit = val / 2.54;
            break;
        default:
            break;
    }

    return unit;
}

//---------------------------------------------------------------------------------------------------------------------
qreal Calculator::MmUnit(qreal val)
{
    qreal unit = val;
    switch(qApp->patternUnit())
    {
        case Unit::Mm:
            break;
        case Unit::Cm:
            unit = val / 10.0;
            break;
        case Unit::Inch:
            unit = val / 25.4;
            break;
        default:
            break;
    }

    return unit;
}

//---------------------------------------------------------------------------------------------------------------------
qreal Calculator::InchUnit(qreal val)
{
    qreal unit = val;
    switch(qApp->patternUnit())
    {
        case Unit::Mm:
            unit = val * 25.4;
            break;
        case Unit::Cm:
            unit = val * 2.54;
            break;
        case Unit::Inch:
            break;
        default:
            break;
    }

    return unit;
}

//---------------------------------------------------------------------------
// Factory function for creating new parser variables
// This could as well be a function performing database queries.
qreal* Calculator::AddVariable(const QString &a_szName, void *a_pUserData)
{
    // I don't want dynamic allocation here, so i used this static buffer
    // If you want dynamic allocation you must allocate all variables dynamically
    // in order to delete them later on. Or you find other ways to keep track of
    // variables that have been created implicitely.
    static qreal afValBuf[100];

    ++iVal;

    Q_UNUSED(a_szName)
    Q_UNUSED(a_pUserData)
    //  qDebug() << "Generating new variable \""
    //           << a_szName << "\" (slots left: "
    //           << 99-iVal << ")"
    //           << " User data pointer is:"
    //           << QString::number(a_pUserData, 16);
    afValBuf[iVal] = 0;

    if (iVal>=99)
    {
        throw qmu::QmuParserError( "Variable buffer overflow." );
    }
    else
    {
        return &afValBuf[iVal];
    }
}
