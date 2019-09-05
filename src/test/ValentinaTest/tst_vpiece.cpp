/************************************************************************
 **
 **  @file   tst_vdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 1, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "tst_vpiece.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiece.h"
#include "../vpatterndb/vpassmark.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vgeometry/vsplinepath.h"
#include "../vmisc/vabstractapplication.h"

#include <QtTest>
#include "../vpatterndb/vpiece.h"

//---------------------------------------------------------------------------------------------------------------------
TST_VPiece::TST_VPiece(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPiece::Issue620()
{
    // See file <root>/src/app/share/collection/bugs/Issue_#620.vit
    // Check main path
    const Unit unit = Unit::Cm;
    QSharedPointer<VContainer> data(new VContainer(nullptr, &unit, VContainer::UniqueNamespace()));
    qApp->setPatternUnit(unit);

    VPiece detail;
    AbstractTest::PieceFromJson(QStringLiteral("://Issue_620/input.json"), detail, data);

    const QVector<QPointF> pointsEkv = detail.MainPathPoints(data.data());

    QVector<QPointF> origPoints;
    AbstractTest::VectorFromJson(QStringLiteral("://Issue_620/output.json"), origPoints);

    // Begin comparison
    Comparison(pointsEkv, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPiece::TestSAPassmark_data()
{
    QTest::addColumn<VPiecePassmarkData>("passmarkData");
    QTest::addColumn<QVector<QPointF>>("seamAllowance");
    QTest::addColumn<QVector<QLineF>>("expectedResult");

    auto ASSERT_TEST_CASE = [this](const char *title, const QString &passmarkData, const QString &seamAllowance,
            const QString &shape)
    {
        VPiecePassmarkData inputPassmarkData;
        AbstractTest::PassmarkDataFromJson(passmarkData, inputPassmarkData);

        QVector<QPointF> inputSeamAllowance;
        AbstractTest::VectorFromJson(seamAllowance, inputSeamAllowance);

        QVector<QLineF> inputOutputShape;
        AbstractTest::PassmarkShapeFromJson(shape, inputOutputShape);

        QTest::newRow(title) << inputPassmarkData << inputSeamAllowance << inputOutputShape;
    };

    // See file src/app/share/collection/bugs/Issue_#924.val
    ASSERT_TEST_CASE("Test 1.",
                     QStringLiteral("://Issue_924_Test_1/passmarkData.json"),
                     QStringLiteral("://Issue_924_Test_1/seamAllowance.json"),
                     QStringLiteral("://Issue_924_Test_1/passmarkShape.json"));
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPiece::TestSAPassmark()
{
    QFETCH(VPiecePassmarkData, passmarkData);
    QFETCH(QVector<QPointF>, seamAllowance);
    QFETCH(QVector<QLineF>, expectedResult);

    VPassmark passmark(passmarkData);

    Comparison(passmark.SAPassmark(seamAllowance, PassmarkSide::All), expectedResult);
}
