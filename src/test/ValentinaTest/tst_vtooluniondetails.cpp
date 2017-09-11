/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 9, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "tst_vtooluniondetails.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vtools/tools/vtooluniondetails.h"

#include <QtTest>

typedef QPair<bool, VPieceNode> UnitedPathNode;

Q_DECLARE_METATYPE(UnitedPathNode)

//---------------------------------------------------------------------------------------------------------------------
TST_VToolUnionDetails::TST_VToolUnionDetails(QObject *parent)
    : QObject (parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VToolUnionDetails::TestUnitingMainPaths_data()
{
    QTest::addColumn<VPiecePath>("d1");
    QTest::addColumn<VPiecePath>("d2");
    QTest::addColumn<quint32>("indexD2");
    QTest::addColumn<quint32>("pRotate");
    QTest::addColumn<QVector<UnitedPathNode>>("result");

    VPieceNode d1p0(1205, Tool::NodePoint);
    VPieceNode d1p1(1206, Tool::NodePoint);
    VPieceNode d1p2(1207, Tool::NodeSpline);
    VPieceNode d1p3(1208, Tool::NodePoint);
    VPieceNode d1p4(1209, Tool::NodePoint);
    VPieceNode d1p5(1204, Tool::NodeSpline);

    VPieceNode d2p0(1211, Tool::NodePoint);
    VPieceNode d2p1(1212, Tool::NodePoint);
    VPieceNode d2p2(1213, Tool::NodeSpline);
    VPieceNode d2p3(1214, Tool::NodePoint);
    VPieceNode d2p4(1215, Tool::NodePoint);

    QVector<UnitedPathNode> result;
    result.append(qMakePair(true, d1p0));
    result.append(qMakePair(true, d1p1));
    result.append(qMakePair(true, d1p2));
    result.append(qMakePair(true, d1p3));
    result.append(qMakePair(false, d2p2));
    result.append(qMakePair(false, d2p3));
    result.append(qMakePair(false, d2p4));
    result.append(qMakePair(false, d2p0));
    result.append(qMakePair(true, d1p4));
    result.append(qMakePair(true, d1p5));

    VPiecePath d1;
    d1.Append(d1p0);
    d1.Append(d1p1);
    d1.Append(d1p2);
    d1.Append(d1p3);
    d1.Append(d1p4);
    d1.Append(d1p5);

    VPiecePath d2;
    d2.Append(d2p0);
    d2.Append(d2p1);
    d2.Append(d2p2);
    d2.Append(d2p3);
    d2.Append(d2p4);

    quint32 indexD2 = 0;
    quint32 pRotate = 1208;

    // See the file <root>/src/app/share/collection/Issue_#752.val
    QTest::newRow("Case 1") << d1 << d2 << indexD2 << pRotate << result;

    result.clear();
    result.append(qMakePair(true, d1p1));
    result.append(qMakePair(true, d1p2));
    result.append(qMakePair(true, d1p3));
    result.append(qMakePair(false, d2p2));
    result.append(qMakePair(false, d2p3));
    result.append(qMakePair(false, d2p4));
    result.append(qMakePair(false, d2p0));
    result.append(qMakePair(true, d1p4));
    result.append(qMakePair(true, d1p5));
    result.append(qMakePair(true, d1p0));

    d1.Clear();
    d1.Append(d1p1);
    d1.Append(d1p2);
    d1.Append(d1p3);
    d1.Append(d1p4);
    d1.Append(d1p5);
    d1.Append(d1p0);

    QTest::newRow("Case 2") << d1 << d2 << indexD2 << pRotate << result;

    result.clear();
    result.append(qMakePair(true, d1p2));
    result.append(qMakePair(true, d1p3));
    result.append(qMakePair(false, d2p2));
    result.append(qMakePair(false, d2p3));
    result.append(qMakePair(false, d2p4));
    result.append(qMakePair(false, d2p0));
    result.append(qMakePair(true, d1p4));
    result.append(qMakePair(true, d1p5));
    result.append(qMakePair(true, d1p0));
    result.append(qMakePair(true, d1p1));

    d1.Clear();
    d1.Append(d1p2);
    d1.Append(d1p3);
    d1.Append(d1p4);
    d1.Append(d1p5);
    d1.Append(d1p0);
    d1.Append(d1p1);

    QTest::newRow("Case 3") << d1 << d2 << indexD2 << pRotate << result;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VToolUnionDetails::TestUnitingMainPaths()
{
    QFETCH(VPiecePath, d1);
    QFETCH(VPiecePath, d2);
    QFETCH(quint32, indexD2);
    QFETCH(quint32, pRotate);
    QFETCH(QVector<UnitedPathNode>, result);

    const QVector<UnitedPathNode> out = VToolUnionDetails::CalcUnitedPath(d1, d2, indexD2, pRotate);

    QVERIFY(result.size() == out.size());

    for (int i=0; i < out.size(); ++i)
    {
        const VPieceNode resP = result.at(i).second;
        const VPieceNode outP = out.at(i).second;

        const QString msg = QString("Index: %1. Got item with id = %2, type = %3; Expected item with id = %4, "
                                    "type = %5.")
                .arg(i).arg(outP.GetId()).arg(static_cast<ToolVisHolderType>(outP.GetTypeTool()))
                .arg(resP.GetId()).arg(static_cast<ToolVisHolderType>(resP.GetTypeTool()));

        QVERIFY2(out.at(i).first == result.at(i).first
                 && outP.GetId() == resP.GetId()
                 && outP.GetTypeTool() == resP.GetTypeTool(), qUtf8Printable(msg));
    }
}
