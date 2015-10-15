/************************************************************************
 **
 **  @file   tst_qmutokenparser.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 10, 2015
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

#include "tst_qmutokenparser.h"
#include "../qmuparser/qmutokenparser.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_QmuTokenParser::TST_QmuTokenParser(QObject *parent)
    :QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_QmuTokenParser::IsSingle_data()
{
    QTest::addColumn<QString>("formula");
    QTest::addColumn<bool>("result");

    QTest::newRow("Single value") << "15.5" << true;
    QTest::newRow("Two digits") << "2+2" << false;
    QTest::newRow("Negative single value") << "-2" << true;
    QTest::newRow("Digit and variable") << "2+a" << false;
    QTest::newRow("One variable twice") << "a+a" << false;
    QTest::newRow("Two variables") << "a+b" << false;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_QmuTokenParser::IsSingle()
{
    QFETCH(QString, formula);
    QFETCH(bool, result);

    QCOMPARE(qmu::QmuTokenParser::IsSingle(formula), result);
}
