/************************************************************************
 **
 **  @file   tst_nameregexp.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 5, 2015
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

#include "tst_nameregexp.h"
#include "../qmuparser/qmudef.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_NameRegExp::TST_NameRegExp(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_NameRegExp::TestNameRegExp_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("result");

    QTest::newRow("First character can't be 0") << "0a" << false;
    QTest::newRow("First character can't be 1") << "1a" << false;
    QTest::newRow("First character can't be 2") << "2a" << false;
    QTest::newRow("First character can't be 3") << "3a" << false;
    QTest::newRow("First character can't be 4") << "4a" << false;
    QTest::newRow("First character can't be 5") << "5a" << false;
    QTest::newRow("First character can't be 6") << "6a" << false;
    QTest::newRow("First character can't be 7") << "7a" << false;
    QTest::newRow("First character can't be 8") << "8a" << false;
    QTest::newRow("First character can't be 9") << "9a" << false;
    QTest::newRow("First character can't be \"*\"") << "*a" << false;
    QTest::newRow("First character can't be \"/\"") << "/a" << false;
    QTest::newRow("First character can't be \"^\"") << "^a" << false;
    QTest::newRow("First character can't be \"+\"") << "+a" << false;
    QTest::newRow("First character can't be \"=\"") << "=a" << false;
    QTest::newRow("First character can't be \"-\"") << "-a" << false;
    QTest::newRow("First character can't be whitespace") << " a" << false;
    QTest::newRow("First character can't be \"(\"") << "(a" << false;
    QTest::newRow("First character can't be \")\"") << ")a" << false;
    QTest::newRow("First character can't be \"?\"") << "?a" << false;
    QTest::newRow("First character can't be \"%\"") << "%a" << false;
    QTest::newRow("First character can't be \":\"") << ":a" << false;
    QTest::newRow("First character can't be \";\"") << ";a" << false;
    QTest::newRow("First character can't be \"!\"") << "!a" << false;
    QTest::newRow("First character can't be \".\"") << ".a" << false;
    QTest::newRow("First character can't be \",\"") << ",a" << false;
    QTest::newRow("First character can't be \"`\"") << "`a" << false;
    QTest::newRow("First character can't be \"\"\"") << "\"a" << false;

    QTest::newRow("Any next character can't be \"-\"") << "a-" << false;
    QTest::newRow("Any next character can't be \"*\"") << "a*" << false;
    QTest::newRow("Any next character can't be \"/\"") << "a/" << false;
    QTest::newRow("Any next character can't be \"^\"") << "a^" << false;
    QTest::newRow("Any next character can't be \"+\"") << "a+" << false;
    QTest::newRow("Any next character can't be \"=\"") << "a=" << false;
    QTest::newRow("Any next character can't be whitespace") << "L bust" << false;
    QTest::newRow("Any next character can't be \"(\"") << "a(" << false;
    QTest::newRow("Any next character can't be \")\"") << "a)" << false;
    QTest::newRow("Any next character can't be \"?\"") << "a?" << false;
    QTest::newRow("Any next character can't be \"%\"") << "a%" << false;
    QTest::newRow("Any next character can't be \":\"") << "a:" << false;
    QTest::newRow("Any next character can't be \";\"") << "a:" << false;
    QTest::newRow("Any next character can't be \"!\"") << "a!" << false;
    QTest::newRow("Any next character can't be \".\"") << "a." << false;
    QTest::newRow("Any next character can't be \",\"") << "a," << false;
    QTest::newRow("Any next character can't be \"`\"") << "a`" << false;
    QTest::newRow("Any next character can't be \"\"\"") << "a\"" << false;

    QTest::newRow("Good name \"p12\"") << "p12" << true;
    QTest::newRow("Good name \"height\"") << "height" << true;
    QTest::newRow("Good name \"A_1\"") << "A_1" << true;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_NameRegExp::TestNameRegExp()
{
    const QRegularExpression re(NameRegExp());

    QFETCH(QString, name);
    QFETCH(bool, result);

    QCOMPARE(re.match(name).hasMatch(), result);
}
