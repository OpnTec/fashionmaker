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

#include "tst_vposter.h"
#include "tst_vabstractdetail.h"
#include "tst_vspline.h"
#include "tst_nameregexp.h"
#include "tst_vlayoutdetail.h"
#include "tst_varc.h"
#include "tst_vellipticalarc.h"
#include "tst_qmutokenparser.h"
#include "tst_vmeasurements.h"
#include "tst_vlockguard.h"
#include "tst_misc.h"
#include "tst_vcommandline.h"
#include "tst_vdetail.h"
#include "tst_findpoint.h"
#include "tst_vabstractcurve.h"
#include "tst_vcubicbezierpath.h"
#include "tst_vgobject.h"
#include "tst_vsplinepath.h"
#include "tst_vpointf.h"

#include "../vmisc/def.h"

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(schema);

    QApplication app( argc, argv );// For QPrinter

    int status = 0;
    auto ASSERT_TEST = [&status, argc, argv](QObject* obj)
    {
        status |= QTest::qExec(obj, argc, argv);
        delete obj;
    };

    ASSERT_TEST(new TST_FindPoint());
    ASSERT_TEST(new TST_VDetail());
    ASSERT_TEST(new TST_VPoster());
    ASSERT_TEST(new TST_VAbstractDetail());
    ASSERT_TEST(new TST_VSpline());
    ASSERT_TEST(new TST_VSplinePath());
    ASSERT_TEST(new TST_NameRegExp());
    ASSERT_TEST(new TST_VLayoutDetail());
    ASSERT_TEST(new TST_VArc());
    ASSERT_TEST(new TST_VEllipticalArc());
    ASSERT_TEST(new TST_QmuTokenParser());
    ASSERT_TEST(new TST_VMeasurements());
    ASSERT_TEST(new TST_VLockGuard());
    ASSERT_TEST(new TST_Misc());
    ASSERT_TEST(new TST_VCommandLine());
    ASSERT_TEST(new TST_VAbstractCurve());
    ASSERT_TEST(new TST_VCubicBezierPath());
    ASSERT_TEST(new TST_VGObject());
    ASSERT_TEST(new TST_VPointF());

    return status;
}
