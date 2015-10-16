/************************************************************************
 **
 **  @file   tst_qmuparsererrormsg.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 10, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "tst_qmuparsererrormsg.h"
#include "../vmisc/def.h"
#include "../vmisc/logging.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_QmuParserErrorMsg::TST_QmuParserErrorMsg(QObject *parent) :
    AbstractTest(parent),
    appTranslator(nullptr),
    msg(nullptr)
{
}

//---------------------------------------------------------------------------------------------------------------------
TST_QmuParserErrorMsg::~TST_QmuParserErrorMsg()
{
    delete appTranslator;
    delete msg;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_QmuParserErrorMsg::TestEErrorCodes_data()
{
    {
        QDir dir(TranslationsPath());
        const QStringList fileNames = dir.entryList(QStringList("valentina_*.qm"));

        QVERIFY2(SupportedLocales().size() == fileNames.size(), "Unexpected count of files.");
    }

    QTest::addColumn<int>("code");
    QTest::addColumn<QString>("locale");
    QTest::addColumn<bool>("tok");
    QTest::addColumn<bool>("pos");

    AddCase(qmu::ecUNASSIGNABLE_TOKEN, true, true);
    AddCase(qmu::ecINTERNAL_ERROR, false, false);
    AddCase(qmu::ecINVALID_NAME, true, false);
    AddCase(qmu::ecINVALID_BINOP_IDENT, true, false);
    AddCase(qmu::ecINVALID_INFIX_IDENT, true, false);
    AddCase(qmu::ecINVALID_POSTFIX_IDENT, true, false);
    AddCase(qmu::ecINVALID_FUN_PTR, false, false);
    AddCase(qmu::ecEMPTY_EXPRESSION, false, false);
    AddCase(qmu::ecINVALID_VAR_PTR, false, false);
    AddCase(qmu::ecUNEXPECTED_OPERATOR, true, true);
    AddCase(qmu::ecUNEXPECTED_EOF, false, true);
    AddCase(qmu::ecUNEXPECTED_ARG_SEP, false, true);
    AddCase(qmu::ecUNEXPECTED_PARENS, true, true);
    AddCase(qmu::ecUNEXPECTED_FUN, true, true);
    AddCase(qmu::ecUNEXPECTED_VAL, true, true);
    AddCase(qmu::ecUNEXPECTED_VAR, true, true);
    AddCase(qmu::ecUNEXPECTED_ARG, false, true);
    AddCase(qmu::ecMISSING_PARENS, false, false);
    AddCase(qmu::ecTOO_MANY_PARAMS, true, true);
    AddCase(qmu::ecTOO_FEW_PARAMS, true, true);
    AddCase(qmu::ecDIV_BY_ZERO, false, false);
    AddCase(qmu::ecDOMAIN_ERROR, false, false);
    AddCase(qmu::ecNAME_CONFLICT, false, false);
    AddCase(qmu::ecOPT_PRI, false, false);
    AddCase(qmu::ecBUILTIN_OVERLOAD, true, false);
    AddCase(qmu::ecUNEXPECTED_STR, false, true);
    AddCase(qmu::ecUNTERMINATED_STRING, false, true);
    AddCase(qmu::ecSTRING_EXPECTED, false, false);
    AddCase(qmu::ecVAL_EXPECTED, false, false);
    AddCase(qmu::ecOPRT_TYPE_CONFLICT, true, true);
    AddCase(qmu::ecSTR_RESULT, false, false);
    AddCase(qmu::ecGENERIC, false, false);
    AddCase(qmu::ecLOCALE, false, false);
    AddCase(qmu::ecUNEXPECTED_CONDITIONAL, true, false);
    AddCase(qmu::ecMISSING_ELSE_CLAUSE, false, false);
    AddCase(qmu::ecMISPLACED_COLON, false, true);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_QmuParserErrorMsg::TestEErrorCodes()
{
    QFETCH(int, code);
    QFETCH(QString, locale);
    QFETCH(bool, tok);
    QFETCH(bool, pos);

    const int res = LoadTranslation(locale);

    switch(res)
    {
        case ErrorInstall:
        case ErrorSize:
        case ErrorLoad:
        {
            const QString message = QString("Failed to check translation for locale = %1").arg(locale);
            QSKIP(qUtf8Printable(message));
            break;
        }
        case NoError:
        {
            CheckStrings(code, tok, pos);

            if (not appTranslator.isNull())
            {
                const bool result = QCoreApplication::removeTranslator(appTranslator);

                if (result == false)
                {
                    const QString message = QString("Can't remove translation for locale = %1").arg(locale);
                    QWARN(qUtf8Printable(message));
                }
                delete appTranslator;
            }
            break;
        }
        default:
            QWARN("Unexpected state");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_QmuParserErrorMsg::AddCase(int code, bool tok, bool pos)
{
    const QStringList locales = SupportedLocales();

    for(int l = 0, sz = locales.size(); l < sz; ++l)
    {
        const QString locale = locales.at(l);
        const QString tag = QString("Check translation code=%1 in file valentina_%2.qm").arg(code).arg(locale);
        QTest::newRow(qUtf8Printable(tag)) << code << locale << tok << pos;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int TST_QmuParserErrorMsg::LoadTranslation(const QString &checkedLocale)
{
    const QString path = TranslationsPath();
    const QString file = QString("valentina_%1.qm").arg(checkedLocale);

    if (QFileInfo(path+"/"+file).size() <= 34)
    {
        const QString message = QString("Translation for locale = %1 is empty. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        return ErrorSize;
    }

    appTranslator = new QTranslator(this);

    if (not appTranslator->load(file, path))
    {
        const QString message = QString("Can't load translation for locale = %1. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete appTranslator;

        return ErrorLoad;
    }

    if (not QCoreApplication::installTranslator(appTranslator))
    {
        const QString message = QString("Can't install translation for locale = %1. \nFull path: %2/%3")
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(qUtf8Printable(message));

        delete appTranslator;

        return ErrorInstall;
    }

    delete msg;
    msg = new qmu::QmuParserErrorMsg();//Very important do it after load QM file.

    return NoError;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_QmuParserErrorMsg::CheckStrings(int code, bool tok, bool pos)
{
    const QString translated = (*msg)[code];
    QCOMPARE(translated.indexOf(QLatin1Literal("$TOK$")) != -1, tok);
    QCOMPARE(translated.indexOf(QLatin1Literal("$POS$")) != -1, pos);
}
