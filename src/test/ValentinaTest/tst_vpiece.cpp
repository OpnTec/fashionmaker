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

#include "tst_vpiece.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiece.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vgeometry/vsplinepath.h"
#include "../vmisc/vabstractapplication.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VPiece::TST_VPiece(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPiece::ClearLoop()
{
    // Input data taken from real case
    // See file <root>/src/app/share/collection/jacketМ6_30-110.val
    // Check correct seam allowance
    const Unit unit = Unit::Mm;
    QScopedPointer<VContainer> data(new VContainer(nullptr, &unit));
    qApp->setPatternUnit(unit);

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

    const QVector<QPointF> pointsEkv = detail.SeamAllowancePoints(data.data());

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(42.46405659601932, 415.2845470563871));
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
    origPoints.append(QPointF(701.8005387196029, 771.2301295961994));
    origPoints.append(QPointF(709.4908502689113, 784.4621360005407));
    origPoints.append(QPointF(713.2090350731621, 790.7616409320319));
    origPoints.append(QPointF(715.0121915355467, 793.763727920337));
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
    origPoints.append(QPointF(792.109995909239, 824.8480813766124));
    origPoints.append(QPointF(826.0032661558732, 877.1274330708662));
    origPoints.append(QPointF(828.6858753986579, 1697.305833468011));
    origPoints.append(QPointF(42.46405659601932, 415.2845470563871));

    // Begin comparison
    Comparison(pointsEkv, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPiece::Issue620()
{
    // See file <root>/src/app/share/collection/bugs/Issue_#620.vit
    // Check main path
    const Unit unit = Unit::Cm;
    QScopedPointer<VContainer> data(new VContainer(nullptr, &unit));
    qApp->setPatternUnit(unit);

    data->UpdateGObject(1, new VPointF(30, 39.999874015748034, "A", 5.0000125984251973, 9.9999874015748045));
    data->UpdateGObject(2, new VPointF(333.80102715408322, 37.242158125518621, "A1", 5.0000125984251973,
                                       9.9999874015748045));
    data->UpdateGObject(3, new VPointF(345.43524385831239, 572.57275904711241, "A2", 5.0000125984251973,
                                       9.9999874015748045));
    VPointF *p4 = new VPointF(-43.770684129917051, 567.84465074396087, "A3", 5.0000125984251973,
                              9.9999874015748045);
    data->UpdateGObject(4, p4);

    VPointF *p5 = new VPointF(101.73836126698214, 289.83563666815587, "A4", 5.0000125984251973, 9.9999874015748045);
    data->UpdateGObject(5, p5);
    data->UpdateGObject(6, new VPointF(34.070501467722302, 568.79027240459118, "A5", 5.0000125984251973,
                                       9.9999874015748045));

    QVector<VSplinePoint> points;

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(6);
        VSplinePoint p(*point.data(), 239.37700000000001, "239.377", 419.37700000000001, "59.3765",
                       0, "0", 109.55943307086613, "2.89876");
        points.append(p);
    }

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(5);
        VSplinePoint p(*point.data(), 273.97199999999998, "273.972", 453.97199999999998, "93.9724",
                       88.161637795275595, "2.33261", 56.135055118110238, "1.48524");
        points.append(p);
    }

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(1);
        VSplinePoint p(*point.data(), 337.32600000000002, "337.326", 157.32599999999999, "157.326",
                       71.189669291338589, "1.88356", 50.093858267716534, "1.3254");
        points.append(p);
    }

    data->UpdateGObject(7, new VSplinePath(points));

    data->UpdateGObject(8, new VSpline(*p4, *p5, 59.932499999999997, "59.9325", 257.56999999999999,
                                       "257.57", 170.46425196850396, "4.5102", 150.6164409448819,
                                       "3.98506"));

    VPiece detail;
    detail.SetSeamAllowance(false);
    detail.SetSAWidth(7);
    detail.GetPath().Append(VPieceNode(1, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(2, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(3, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(4, Tool::NodePoint));
    detail.GetPath().Append(VPieceNode(8, Tool::NodeSpline));
    detail.GetPath().Append(VPieceNode(7, Tool::NodeSplinePath));

    const QVector<QPointF> pointsEkv = detail.MainPathPoints(data.data());

    QVector<QPointF> origPoints;

    origPoints.append(QPointF(30.0, 39.999874015748034));
    origPoints.append(QPointF(333.8010271540832, 37.24215812551862));
    origPoints.append(QPointF(345.4352438583124, 572.5727590471124));
    origPoints.append(QPointF(-43.77068412991705, 567.8446507439609));
    origPoints.append(QPointF(-35.87661251446703, 554.334665281764));
    origPoints.append(QPointF(-21.170117796937717, 530.280231438072));
    origPoints.append(QPointF(-7.679257977133303, 509.352260960953));
    origPoints.append(QPointF(4.687459520857305, 491.0705525945524));
    origPoints.append(QPointF(21.441835284502346, 467.3785713839818));
    origPoints.append(QPointF(40.52832726053379, 440.9303122452387));
    origPoints.append(QPointF(52.86115234049136, 422.54889846842264));
    origPoints.append(QPointF(60.404360001732, 410.06053639339046));
    origPoints.append(QPointF(67.46387337253645, 396.85702763809604));
    origPoints.append(QPointF(74.13118502881582, 382.4581709466849));
    origPoints.append(QPointF(80.49778754648119, 366.3837650633026));
    origPoints.append(QPointF(86.65517350144364, 348.15360873209477));
    origPoints.append(QPointF(92.69483546961428, 327.287500697207));
    origPoints.append(QPointF(98.70826602690417, 303.30523970278495));
    origPoints.append(QPointF(101.73836126698214, 289.83563666815587));
    origPoints.append(QPointF(100.33414592841483, 265.38578532087524));
    origPoints.append(QPointF(96.68026149584162, 212.73003048920282));
    origPoints.append(QPointF(93.50326130663731, 183.65468712210458));
    origPoints.append(QPointF(90.71558574640727, 163.99193793167092));
    origPoints.append(QPointF(87.25149211813451, 144.48122290748523));
    origPoints.append(QPointF(83.01539723892088, 125.48779016127345));
    origPoints.append(QPointF(77.91171792586823, 107.37688780476151));
    origPoints.append(QPointF(73.4247676790375, 94.64014475361638));
    origPoints.append(QPointF(70.13263046316489, 86.58901147237839));
    origPoints.append(QPointF(66.56983159845231, 78.96396285141125));
    origPoints.append(QPointF(62.72442318703749, 71.81065490468066));
    origPoints.append(QPointF(58.58445733105817, 65.17474364615236));
    origPoints.append(QPointF(54.13798613265208, 59.10188508979212));
    origPoints.append(QPointF(49.373061693956934, 53.63773524956568));
    origPoints.append(QPointF(44.277736117110486, 48.827950139438784));
    origPoints.append(QPointF(38.84006150425045, 44.71818577337716));
    origPoints.append(QPointF(33.04808995751456, 41.35409816534657));

    // Begin comparison
    Comparison(pointsEkv, origPoints);
}
