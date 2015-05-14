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
 **  Copyright (C) 2013-2015 Valentina project
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
#include "../core/vapplication.h"
#include "vcontainer.h"
#include "../core/vsettings.h"

using namespace qmu;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Calculator class wraper for QMuParser. Make easy initialization math parser.
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
    :QmuParser(), vVarVal(nullptr), data(data)
{
    SCASSERT(data != nullptr)
    InitCharacterSets();
    setAllowSubexpressions(false);//Only one expression per time

    SetSepForEval();
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
    :QmuParser(), vVarVal(nullptr), data(nullptr)
{
    InitCharacterSets();
    setAllowSubexpressions(false);//Only one expression per time
    SetVarFactory(AddVariable, this);

    SetSepForTr(fromUser);

    SetExpr(formula);
    //Need run for making tokens. Don't catch exception here, because we want know if formula has error.
    Eval();
}

//---------------------------------------------------------------------------------------------------------------------
Calculator::~Calculator()
{
    delete [] vVarVal;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief eval calculate formula.
 *
 * First we try eval expression without adding variables. If it fail, we take tokens from expression and add variables
 * to parser and try again.
 *
 * @param formula string of formula.
 * @return value of formula.
 */
qreal Calculator::EvalFormula(const QString &formula)
{
    // Parser doesn't know any variable on this stage. So, we just use variable factory that for each unknown variable
    // set value to 0.
    SetVarFactory(AddVariable, this);
    SetSepForEval();//Reset separators options

    SetExpr(formula);

    qreal result = 0;
    result = Eval();

    QMap<int, QString> tokens = this->GetTokens();

    // Remove "-" from tokens list if exist. If don't do that unary minus operation will broken.
    RemoveAll(tokens, QStringLiteral("-"));

    for (int i = 0; i < builInFunctions.size(); ++i)
    {
        if (tokens.isEmpty())
        {
            break;
        }
        RemoveAll(tokens, builInFunctions.at(i));
    }

    if (tokens.isEmpty())
    {
        return result; // We have found only numbers in expression.
    }

    // Add variables to parser because we have deal with expression with variables.
    InitVariables(data, tokens, formula);
    return Eval();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Calculator::InitVariables add variables to parser.
 *
 * For optimization purpose we try don't add variables that we don't need.
 *
 * @param data pointer to a variable container. Hold all informations about variables.
 * @param tokens all tokens (measurements names, variables with lengths) that parser have found in expression.
 * @param formula expression, need for throwing better error message.
 */
void Calculator::InitVariables(const VContainer *data, const QMap<int, QString> &tokens, const QString &formula)
{
    if (qApp->patternType() == MeasurementsType::Standard)
    {
        vVarVal = new qreal[2]; //stabdard measurements table have two additional variables
    }

    SCASSERT(data != nullptr)
    const QHash<QString, QSharedPointer<VInternalVariable> > *vars = data->DataVariables();

    QMap<int, QString>::const_iterator i = tokens.constBegin();
    while (i != tokens.constEnd())
    {
        bool found = false;
        if (vars->contains(i.value()))
        {
            QSharedPointer<VInternalVariable> var = vars->value(i.value());
            if ((qApp->patternType() == MeasurementsType::Standard) &&
                (var->GetType() == VarType::Measurement || var->GetType() == VarType::Increment))
            {
                QSharedPointer<VVariable> m = data->GetVariable<VVariable>(i.value());
                m->SetValue(data->size(), data->height());
            }
            DefineVar(i.value(), var->GetValue());
            found = true;
        }

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            if (i.value() == data->SizeName())
            {
                vVarVal[0] = data->size();
                DefineVar(data->SizeName(), &vVarVal[0]);
                found = true;
            }

            if (i.value() == data->HeightName())
            {
                vVarVal[1] = data->height();
                DefineVar(data->HeightName(), &vVarVal[1]);
                found = true;
            }
        }

        if (found == false)
        {
            throw qmu::QmuParserError (ecUNASSIGNABLE_TOKEN, i.value(), formula, i.key());
        }
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Calculator::InitCharacterSets init character set for parser.
 *
 * QMuParser require setting character set for legal characters. Because we try make our expresion language independent
 * we set all posible unique characters from all alphabets.
 *
 */
void Calculator::InitCharacterSets()
{
    //String with all unique symbols for supported alpabets.
    // See script alphabets.py for generation and more information.
    //Note. MSVC doen't support normal string concatenation for long string. Thats why we use QStringList in this place.
    QStringList symbols = QStringList() << "ցЀĆЈVӧĎАғΕĖӅИқΝĞơРңњΥĦШҫ̆جگĮаҳѕεشԶиһνԾрυلՆӝшËՎҔPÓՖXӛӟŞӣզhëծpóӞնxßվāŁЃֆĉЋ"
                                        << "CŬđҐГΒęҘЛΚŘġҠУGاհЫدԱҰгβطԹõлκKՁÀуςهՉÈыvیՑÐSOřӘћաőcӐթèkàѓżűðsķչøӥӔĀփїІĈЎ"
                                        << "ґĐΗЖҙĘȚΟОҡĠآΧЦتЮұİزηжԸغοоÁՀقχцÉՈيюÑՐђӋіәťӆўáŠĺѐfөըnñŰӤӨӹոľЁրăЉŭċБӸēłΔҖ"
                                        << "ЙŤěΜӜDСձģΤӰЩīņحҮбưԳصδHйԻŇμӲӴсՃمτƠщՋєLQŹՓŕÖYśÞaգĽæiŽիӓîqճöyջþĂօЄӦĊЌΑĒДҗј"
                                        << "ΙȘĚМΡéĵĢФūӚΩبĪЬүќαذԲдҷιظԺмρՂфÇωوՊьÏՒTŚĻJբdçժlïӪղtպӫAւąЇčŃЏĕӯЗΖEțŮĝПΞأĥ"
                                        << "ثĹЧΦÆӳЯIسŲԵзζԽпξكՅÄчφNMՍӌяӢՕÔWÎŝÜџёźեägխoӒյôwĶBžսüЂĄև̈ЊČƏљΓВҕĔӮΛКĜΣТҥĤک"
                                        << "ЪƯخγвŅԴŪضλкԼĴσтÅՄنъÍՌRӕՔZÝŜbåդﻩjíլļrӵմzýռپêЅքćچЍďӱҒЕůėژșΘØҚНğńءΠFҢХħΨҪ"
                                        << "ЭųįҶرҲеԷňعθҺнԿفπÂхՇψÊэšՏÒUəÚѝŻşҤӑâeէŐımկòuշÕúտŔ";

    // Defining identifier character sets
    DefineNameChars(QStringLiteral("0123456789_") + symbols.join(""));
    DefineOprtChars(symbols.join("") + QStringLiteral("+-*^/?<>=#!$%&|~_"));
}

//---------------------------------------------------------------------------------------------------------------------
// Factory function for creating new parser variables
// This could as well be a function performing database queries.
qreal* Calculator::AddVariable(const QString &a_szName, void *a_pUserData)
{
    Q_UNUSED(a_szName)
    Q_UNUSED(a_pUserData)

    static qreal value = 0;
    return &value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Calculator::SetSepForEval set separators for eval. Each expression eval in internal (C) locale.
 */
void Calculator::SetSepForEval()
{
    SetArgSep(';');
    SetThousandsSep(',');
    SetDecSep('.');
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Calculator::SetSepForTr set separators for translation expression.
 * @param fromUser true if expression come from user (from dialog).
 */
void Calculator::SetSepForTr(bool fromUser)
{
    if (fromUser)
    {
        const bool osSeparatorValue = qApp->getSettings()->GetOsSeparator();

        if (osSeparatorValue)
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
 * @brief Calculator::RemoveAll remove token from token list.
 *
 * Standard Qt class QMap doesn't have method RemoveAll.
 * Example: remove "-" from tokens list if exist. If don't do that unary minus operation will broken.
 *
 * @param map map with tokens
 * @param val token that need delete
 */
void Calculator::RemoveAll(QMap<int, QString> &map, const QString &val)
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
