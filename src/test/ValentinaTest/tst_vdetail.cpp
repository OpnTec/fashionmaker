/************************************************************************
 **
 **  @file   tst_vdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 1, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "tst_vdetail.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiece.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vgeometry/vsplinepath.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VDetail::TST_VDetail(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VDetail::ClearLoop()
{
    // Input data taken from real case
    // See file <root>/src/app/share/collection/jacketМ6_30-110.val
    // Check correct seam allowance
    const Unit unit = Unit::Mm;
    VContainer *data = new VContainer(nullptr, &unit);

    data->UpdateGObject(304, new VPointF(61.866708661417327, 446.92270866141735, "Ф1", 5.0000125984251973,
                                         9.9999874015748045));
    data->UpdateGObject(307, new VPointF(642.96276692900597, 581.21895343695326, "С1", 88.99993700787401,
                                         50.000125984251973));

    data->UpdateGObject(56, new VPointF(802.08718110236236, 850.6707401574804, "Г6", 20.733316535433072,
                                        18.132850393700789));
    data->UpdateGObject(57, new VPointF(690.47666217505162, 804.29700711628709, "З", -11.505637795275591,
                                        31.221543307086616));
    data->UpdateGObject(203, new VPointF(642.96276692900597, 581.21895343695326, "С1", 88.99993700787401,
                                         50.000125984251973));

    QVector<VFSplinePoint> points;

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(203);
        VFSplinePoint p(*point.data(), 0.79455646129695412, 449.62747641208136, 1.6867283804609809, 269.62747641208136);
        points.append(p);
    }

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(57);
        VFSplinePoint p(*point.data(), 0.4456850846354396, 120.24000000000034, 1.0255399999999999, 300.24000000000035);
        points.append(p);
    }

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(56);
        VFSplinePoint p(*point.data(), 1.0085299999999999, 184.58891, 1, 4.5889100000000003);
        points.append(p);
    }

    data->UpdateGObject(308, new VSplinePath(points));

    data->UpdateGObject(309, new VPointF(799.45989815267649, 850.6707401574804, "Г8", -30.431206299212597,
                                         29.487155905511813));
    data->UpdateGObject(310, new VPointF(802.08718110236236, 1653.9337322834645, "Н5", 5.0000125984251973,
                                         9.9999874015748045));

    VPiece detail;
    detail.SetSeamAllowance(true);
    detail.SetSAWidth(7);
    detail.GetPath().Append(VPieceNode(304, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(307, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(308, Tool::NodeSplinePath));
    detail.GetPath().Append(VPieceNode(309, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(310, Tool::NodePoint));
    // Closed
    detail.GetPath()[0].SetFormulaSABefore("0");
    detail.GetPath()[detail.GetPath().CountNodes()-1].SetFormulaSAAfter("0");

    const QVector<QPointF> pointsEkv = detail.SeamAllowancePoints(data);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(42.46405659601934, 415.2845470563871));
    origPoints.append(QPointF(669.4711112822802, 560.1912138528764));
    origPoints.append(QPointF(669.3860586912449, 594.8702688224456));
    origPoints.append(QPointF(669.8537241707239, 619.8499975627876));
    origPoints.append(QPointF(670.904422406071, 642.3178846003559));
    origPoints.append(QPointF(672.4760946214147, 662.4793325519112));
    origPoints.append(QPointF(674.5043075280212, 680.4882882996908));
    origPoints.append(QPointF(676.9236185537709, 696.5023899408525));
    origPoints.append(QPointF(679.6685049649096, 710.6850434378523));
    origPoints.append(QPointF(682.6751345782424, 723.2078546770477));
    origPoints.append(QPointF(685.8841825335202, 734.2530219317046));
    origPoints.append(QPointF(689.2446146317781, 744.0149891243127));
    origPoints.append(QPointF(692.7177992446996, 752.7004886151328));
    origPoints.append(QPointF(696.2448548679188, 760.4478278509594));
    origPoints.append(QPointF(701.7756191283249, 771.1817664136103));
    origPoints.append(QPointF(715.0901851898707, 793.875700151992));
    origPoints.append(QPointF(718.7808834775552, 799.1742815201673));
    origPoints.append(QPointF(722.5723522600899, 803.7317522627161));
    origPoints.append(QPointF(726.4900810611796, 807.6675956080389));
    origPoints.append(QPointF(730.558043384579, 811.0692054929614));
    origPoints.append(QPointF(734.8172463181712, 814.0137888810656));
    origPoints.append(QPointF(739.318992665584, 816.5616228424284));
    origPoints.append(QPointF(744.1159693320302, 818.7532201983325));
    origPoints.append(QPointF(749.2539447976853, 820.6109034502547));
    origPoints.append(QPointF(754.7662623591739, 822.1435546205067));
    origPoints.append(QPointF(760.6718473722125, 823.3525044481979));
    origPoints.append(QPointF(766.9761113390083, 824.236813134474));
    origPoints.append(QPointF(773.6735265709667, 824.7970381873482));
    origPoints.append(QPointF(780.6615727577812, 825.0343457026618));
    origPoints.append(QPointF(792.1099959092389, 824.8480813766124));
    origPoints.append(QPointF(825.8245486890017, 822.4865245986581));
    origPoints.append(QPointF(828.6858753986579, 1697.305833468011));

    // Begin comparison
    Comparison(pointsEkv, origPoints);
}
