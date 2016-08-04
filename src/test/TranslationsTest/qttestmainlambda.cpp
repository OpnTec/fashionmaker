/************************************************************************
 **
 **  @file   qttestmainlambda.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 3, 2015
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

#include <QtTest>

#include "tst_measurementregexp.h"
#include "tst_qmuparsererrormsg.h"
#include "tst_tstranslation.h"

#include "../vmisc/def.h"

int main(int argc, char** argv)
{
    QApplication app( argc, argv );// For translation

    int status = 0;
    auto ASSERT_TEST = [&status, argc, argv](QObject* obj)
    {
        status |= QTest::qExec(obj, argc, argv);
        delete obj;
    };

    ASSERT_TEST(new TST_TSTranslation());

    {
        const QStringList locales = SupportedLocales();
        for(quint32 s = 0; s < TST_MeasurementRegExp::systemCounts; ++s)
        {
            for(int l = 0, sz = locales.size(); l < sz; ++l)
            {
                ASSERT_TEST(new TST_MeasurementRegExp(s, locales.at(l)));
            }
        }

        for(int l = 0, sz = locales.size(); l < sz; ++l)
        {
            ASSERT_TEST(new TST_QmuParserErrorMsg(locales.at(l)));
        }
    }



    return status;
}
