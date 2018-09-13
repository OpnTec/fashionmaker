/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 11, 2016
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

#include "tst_vabstractpiece.h"
#include "../vlayout/vabstractpiece.h"

#include <QPointF>
#include <QVector>

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VAbstractPiece::TST_VAbstractPiece(QObject *parent)
    : AbstractTest(parent)
{}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::EquidistantRemoveLoop_data()
{
    QTest::addColumn<QVector<VSAPoint>>("points");
    QTest::addColumn<qreal>("width");
    QTest::addColumn<QVector<QPointF>>("ekvOrig");

    // See file src/app/share/collection/test/seamtest1.val
    QTest::newRow("Seam test 1. Piece. By angle.") << InputPointsSeamTest1PieceByAngle()
                                                   << 37.795275590551185 // seam allowance width
                                                   << OutputPointsSeamTest1PieceByAngle();

    // See file src/app/share/collection/test/seamtest1.val
    QTest::newRow("Seam test 1. Piece. By intersection.") << InputPointsSeamTest1PieceByIntersection()
                                                          << 37.795275590551185 // seam allowance width
                                                          << OutputPointsSeamTest1PieceByIntersection();

    // See file src/app/share/collection/test/seamtest1.val
    QTest::newRow("Seam test 1. Piece. By first edge symmetry.") << InputPointsSeamTest1PieceByFirstEdgeSymmerty()
                                                                 << 37.795275590551185 // seam allowance width
                                                                 << OutputPointsSeamTest1PieceByFirstEdgeSymmerty();

    // See file src/app/share/collection/test/seamtest1.val
    QTest::newRow("Seam test 1. Piece. By second edge symmetry.") << InputPointsSeamTest1PieceBySecondEdgeSymmerty()
                                                                  << 37.795275590551185 // seam allowance width
                                                                  << OutputPointsSeamTest1PieceBySecondEdgeSymmerty();

    // See file src/app/share/collection/test/seamtest1.val
    QTest::newRow("Seam test 1. Piece. By first right angle.") << InputPointsSeamTest1PieceByFirstRightAngle()
                                                               << 37.795275590551185 // seam allowance width
                                                               << OutputPointsSeamTest1PieceByFirstRightAngle();

    // See file src/app/share/collection/test/seamtest1.val
    QTest::newRow("Seam test 1. Piece. By second right angle.") << InputPointsSeamTest1PieceBySecondRightAngle()
                                                                << 37.795275590551185 // seam allowance width
                                                                << OutputPointsSeamTest1PieceBySecondRightAngle();

    // See file src/app/share/collection/test/seamtest2.val
    QTest::newRow("Seam test 2") << InputPointsCase2()
                                 << 37.795275590551185 // seam allowance width
                                 << OutputPointsCase2();

    // See file src/app/share/collection/test/seamtest3.val
    QTest::newRow("Seam test 3") << InputPointsCase3()
                                 << 37.795275590551185 // seam allowance width
                                 << OutputPointsCase3();

    // These are two real cases where equdistant has loop.
    // See issue #298. Segmented Curve isn't selected in Seam Allowance tool.
    // https://bitbucket.org/dismine/valentina/issue/298/segmented-curve-isnt-selected-in-seam
    // See file src/app/share/collection/TestPuzzle.val
    // Code should clean loops in path.
    QTest::newRow("Issue 298. Case1") << InputPointsIssue298Case1()
                                      << 75.59055118110237 // seam allowance width
                                      << OutputPointsIssue298Case1();

    QTest::newRow("Issue 298. Case2") << InputPointsIssue298Case2()
                                      << 37.795275590551185 // seam allowance width
                                      << OutputPointsIssue298Case2();

    // See issue #548. Bug Detail tool. Case when seam allowance is wrong.
    // https://bitbucket.org/dismine/valentina/issues/548/bug-detail-tool-case-when-seam-allowance
    // Files: Steampunk_trousers.val and marie.vit
    // Actually buggy detail see in file src/app/share/collection/bugs/Steampunk_trousers_issue_#548.val
    // Code should clean loops in path.
    QTest::newRow("Issue 548. Case1") << InputPointsIssue548Case1()
                                      << 11.338582677165354 // seam allowance width (0.3 cm)
                                      << OutputPointsIssue548Case1();

    // Disabled due to "undefined behavior" problem
#if !defined(Q_OS_WIN) && !defined(Q_CC_CLANG) && !defined(Q_PROCESSOR_X86_64)
    QTest::newRow("Issue 548. Case2") << InputPointsIssue548Case2()
                                      << 37.795275590551185 // seam allowance width (1.0 cm)
                                      << OutputPointsIssue548Case2();

    QTest::newRow("Issue 548. Case3") << InputPointsIssue548Case3()
                                      << 75.59055118110237 // seam allowance width (2.0 cm)
                                      << OutputPointsIssue548Case3();
#endif

    // See file src/app/share/collection/bugs/Issue_#646.val
    QTest::newRow("Issue 646.") << InputPointsIssue646()
                                << 37.795275590551185 // seam allowance width (1.0 cm)
                                << OutputPointsIssue646();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::EquidistantRemoveLoop() const
{
    QFETCH(QVector<VSAPoint>, points);
    QFETCH(qreal, width);
    QFETCH(QVector<QPointF>, ekvOrig);

    const QVector<QPointF> ekv = VAbstractPiece::Equidistant(points, width);

    // Begin comparison
    Comparison(ekv, ekvOrig);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsSeamTest1PieceByAngle() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(30.0, 894.8030236220472);
    p.SetSABefore(75.59055118110237);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    points.append(VSAPoint(47.53637144700478, 39.80977598775412));
    points.append(VSAPoint(85.4092492068762, 37.36146293134895));
    points.append(VSAPoint(125.57923134501877, 32.13056337758208));
    points.append(VSAPoint(156.5108311435474, 26.074993549693005));
    points.append(VSAPoint(177.21129897469388, 21.059222548891512));
    points.append(VSAPoint(197.8216319874312, 15.159776113211533));
    points.append(VSAPoint(218.19418331220902, 8.334870062533497));
    points.append(VSAPoint(238.18130607947683, 0.5427202167378373));
    points.append(VSAPoint(257.6353534196842, -8.258457604295018));
    points.append(VSAPoint(276.4086784632808, -18.110447580684635));
    points.append(VSAPoint(294.3536343407161, -29.055033892550586));
    points.append(VSAPoint(311.32257418243984, -41.13400072001244));
    points.append(VSAPoint(327.16785111890135, -54.389132243189756));

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    points.append(p);

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    points.append(p);

    p = VSAPoint(519.4465667350105, -36.01405338211433);
    p.SetSABefore(0.3779527559055119);
    points.append(p);

    points.append(VSAPoint(460.3937007874016, 342.36207874015753));

    p = VSAPoint(620.5290071875436, -5.506318761785565);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(930.9115139455133, 164.2021651072791);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(907.1077633892038, 216.20756088951924);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(886.6420235246574, 264.5706340127217);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(869.2733175133691, 309.4054580978941);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(854.760668516834, 350.82610676604384);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(842.8630996965472, 388.9466536381784);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(833.3396342140039, 423.88117233530534);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(825.9492952306991, 455.74373647843197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.451105908128, 484.6484196885659);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.6040894077855, 510.7092955867147);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1672688911669, 534.0404377938855);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.8996675197673, 554.7559199310863);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.5925673946043, 581.4656260538284);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1809574610598, 608.5715133604979);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(815.1968503937007, 618.5825511811024);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.3634787672067, 628.380932746632);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.4053369940075, 649.1688436779486);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(826.5040284899703, 671.0654086429722);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(834.6667882477791, 693.7142683899226);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(844.9008512601183, 716.7590636670197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(857.2134525196718, 739.8434352224829);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(871.6118270191237, 762.6110238045324);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(888.1032097511581, 784.7054701613878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(906.6948357084594, 805.7704150412691);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(927.3939398837115, 825.4494991923958);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(950.2077572695983, 843.3863633629879);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(968.776733605274, 855.4073744953336);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(981.7764600118351, 862.7350547966848);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(995.3089344044396, 869.4267280237759);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1009.3750611571733, 875.4378492701345);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1023.9757446441214, 880.7238736292878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1039.1118892393697, 885.2402561947638);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1054.784399317004, 888.9424520600894);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1070.994179251109, 891.7859163187926);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1087.7421334157707, 893.7261040644007);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1105.0291661850742, 894.7184703904409);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsSeamTest1PieceByAngle() const
{
    QVector<QPointF> points;

    points += QPointF(30.0, 970.3935748031496);
    points += QPointF(30.0, 2.2023778242823937);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.74605442368645, -0.2758951981748612);
    points += QPointF(119.50208828052781, -5.192452541281906);
    points += QPointF(148.42595722778273, -10.854963023883784);
    points += QPointF(167.5566961450379, -15.490385095876569);
    points += QPointF(186.61281642420556, -20.94495784777482);
    points += QPointF(205.31950565747624, -27.21179180985321);
    points += QPointF(223.51736443487215, -34.306381867523655);
    points += QPointF(241.0503469884652, -42.23845350735769);
    points += QPointF(257.76798751706616, -51.01164773556077);
    points += QPointF(273.5285590176657, -60.62398316233916);
    points += QPointF(284.94491218126115, -68.7504632158891);
    points += QPointF(556.951317720607, -31.220946854728947);
    points += QPointF(539.6101141051189, 79.89131577778124);
    points += QPointF(585.9167315845332, -20.7024207218235);
    points += QPointF(1117.4559101159903, 212.6634418802967);
    points += QPointF(1101.8482040478996, 244.8082798528157);
    points += QPointF(1080.077560308019, 292.3718325563429);
    points += QPointF(1061.8089025447719, 335.54292908714797);
    points += QPointF(1046.6013689355652, 374.7989965048928);
    points += QPointF(1034.1843603449322, 410.2384638595293);
    points += QPointF(1024.2819001340656, 441.96655926454446);
    points += QPointF(1016.6128103198596, 470.0987514791764);
    points += QPointF(1010.891548127259, 494.7652903217379);
    points += QPointF(1006.8300406648392, 516.1171535751373);
    points += QPointF(1004.1409514475167, 534.3338730421456);
    points += QPointF(1002.5429209641059, 549.6340874837038);
    points += QPointF(1001.8096437359794, 561.6175009318105);
    points += QPointF(1001.632561636562, 577.0190278199144);
    points += QPointF(1002.5982079396683, 593.4977885619218);
    points += QPointF(1003.0419281414149, 597.870394747796);
    points += QPointF(1003.1926540422378, 599.1363247745379);
    points += QPointF(1004.4715714217981, 605.7139974891637);
    points += QPointF(1006.6672239434022, 613.5972045855956);
    points += QPointF(1010.1411052399322, 623.2360341722922);
    points += QPointF(1014.8474980040493, 633.8337662374475);
    points += QPointF(1020.6660121283516, 644.7426504865729);
    points += QPointF(1027.4243046890665, 655.4292751847524);
    points += QPointF(1034.913026324476, 665.4623428264111);
    points += QPointF(1042.9029269068749, 674.5151720813944);
    points += QPointF(1051.171411576988, 682.3761985416603);
    points += QPointF(1060.1878101895825, 689.4651444417038);
    points += QPointF(1066.6143341082677, 693.625487703081);
    points += QPointF(1070.1212524493003, 695.6022659553572);
    points += QPointF(1074.3830151134664, 697.7096649640414);
    points += QPointF(1078.7265805419665, 699.565875909043);
    points += QPointF(1083.206554246492, 701.1878034134409);
    points += QPointF(1087.891993301369, 702.5858632116882);
    points += QPointF(1092.8646688655851, 703.7605199081748);
    points += QPointF(1098.2162239125396, 704.699271425244);
    points += QPointF(1104.0444208482252, 705.3744462055462);
    points += QPointF(1111.3488014116294, 705.7937559506272);
    points += QPointF(1290.2705536650083, 707.4996206162249);
    points += QPointF(1302.416131870577, 881.6558256043734);
    points += QPointF(30.0, 970.3935748031496);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsSeamTest1PieceByIntersection() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(30.0, 894.8030236220472);
    p.SetSABefore(75.59055118110237);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    points.append(VSAPoint(47.53637144700478, 39.80977598775412));
    points.append(VSAPoint(85.4092492068762, 37.36146293134895));
    points.append(VSAPoint(125.57923134501877, 32.13056337758208));
    points.append(VSAPoint(156.5108311435474, 26.074993549693005));
    points.append(VSAPoint(177.21129897469388, 21.059222548891512));
    points.append(VSAPoint(197.8216319874312, 15.159776113211533));
    points.append(VSAPoint(218.19418331220902, 8.334870062533497));
    points.append(VSAPoint(238.18130607947683, 0.5427202167378373));
    points.append(VSAPoint(257.6353534196842, -8.258457604295018));
    points.append(VSAPoint(276.4086784632808, -18.110447580684635));
    points.append(VSAPoint(294.3536343407161, -29.055033892550586));
    points.append(VSAPoint(311.32257418243984, -41.13400072001244));
    points.append(VSAPoint(327.16785111890135, -54.389132243189756));

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::ByPointsIntersection);
    points.append(p);

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::ByPointsIntersection);
    points.append(p);

    p = VSAPoint(519.4465667350105, -36.01405338211433);
    p.SetSABefore(0.3779527559055119);
    points.append(p);

    points.append(VSAPoint(460.3937007874016, 342.36207874015753));

    p = VSAPoint(620.5290071875436, -5.506318761785565);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(930.9115139455133, 164.2021651072791);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(907.1077633892038, 216.20756088951924);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(886.6420235246574, 264.5706340127217);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(869.2733175133691, 309.4054580978941);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(854.760668516834, 350.82610676604384);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(842.8630996965472, 388.9466536381784);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(833.3396342140039, 423.88117233530534);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(825.9492952306991, 455.74373647843197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.451105908128, 484.6484196885659);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.6040894077855, 510.7092955867147);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1672688911669, 534.0404377938855);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.8996675197673, 554.7559199310863);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.5925673946043, 581.4656260538284);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1809574610598, 608.5715133604979);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(815.1968503937007, 618.5825511811024);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.3634787672067, 628.380932746632);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.4053369940075, 649.1688436779486);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(826.5040284899703, 671.0654086429722);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(834.6667882477791, 693.7142683899226);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(844.9008512601183, 716.7590636670197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(857.2134525196718, 739.8434352224829);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(871.6118270191237, 762.6110238045324);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(888.1032097511581, 784.7054701613878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(906.6948357084594, 805.7704150412691);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(927.3939398837115, 825.4494991923958);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(950.2077572695983, 843.3863633629879);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(968.776733605274, 855.4073744953336);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(981.7764600118351, 862.7350547966848);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(995.3089344044396, 869.4267280237759);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1009.3750611571733, 875.4378492701345);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1023.9757446441214, 880.7238736292878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1039.1118892393697, 885.2402561947638);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1054.784399317004, 888.9424520600894);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1070.994179251109, 891.7859163187926);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1087.7421334157707, 893.7261040644007);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1105.0291661850742, 894.7184703904409);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsSeamTest1PieceByIntersection() const
{
    QVector<QPointF> points;

    points += QPointF(30.0, 970.3935748031496);
    points += QPointF(30.0, 2.2023778242823937);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.74605442368645, -0.2758951981748612);
    points += QPointF(119.50208828052781, -5.192452541281906);
    points += QPointF(148.42595722778273, -10.854963023883784);
    points += QPointF(167.5566961450379, -15.490385095876569);
    points += QPointF(186.61281642420556, -20.94495784777482);
    points += QPointF(205.31950565747624, -27.21179180985321);
    points += QPointF(223.51736443487215, -34.306381867523655);
    points += QPointF(241.0503469884652, -42.23845350735769);
    points += QPointF(257.76798751706616, -51.01164773556077);
    points += QPointF(273.5285590176657, -60.62398316233916);
    points += QPointF(284.4959443876259, -68.4308753608727);
    points += QPointF(334.91313141686754, -61.856202407499914);
    points += QPointF(556.951317720607, -31.220946854728947);
    points += QPointF(539.6101141051189, 79.89131577778124);
    points += QPointF(585.9167315845332, -20.7024207218235);
    points += QPointF(1117.4559101159903, 212.6634418802967);
    points += QPointF(1101.8482040478996, 244.8082798528157);
    points += QPointF(1080.077560308019, 292.3718325563429);
    points += QPointF(1061.8089025447719, 335.54292908714797);
    points += QPointF(1046.6013689355652, 374.7989965048928);
    points += QPointF(1034.1843603449322, 410.2384638595293);
    points += QPointF(1024.2819001340656, 441.96655926454446);
    points += QPointF(1016.6128103198596, 470.0987514791764);
    points += QPointF(1010.891548127259, 494.7652903217379);
    points += QPointF(1006.8300406648392, 516.1171535751373);
    points += QPointF(1004.1409514475167, 534.3338730421456);
    points += QPointF(1002.5429209641059, 549.6340874837038);
    points += QPointF(1001.8096437359794, 561.6175009318105);
    points += QPointF(1001.632561636562, 577.0190278199144);
    points += QPointF(1002.5982079396683, 593.4977885619218);
    points += QPointF(1003.0419281414149, 597.870394747796);
    points += QPointF(1003.1926540422378, 599.1363247745379);
    points += QPointF(1004.4715714217981, 605.7139974891637);
    points += QPointF(1006.6672239434022, 613.5972045855956);
    points += QPointF(1010.1411052399322, 623.2360341722922);
    points += QPointF(1014.8474980040493, 633.8337662374475);
    points += QPointF(1020.6660121283516, 644.7426504865729);
    points += QPointF(1027.4243046890665, 655.4292751847524);
    points += QPointF(1034.913026324476, 665.4623428264111);
    points += QPointF(1042.9029269068749, 674.5151720813944);
    points += QPointF(1051.171411576988, 682.3761985416603);
    points += QPointF(1060.1878101895825, 689.4651444417038);
    points += QPointF(1066.6143341082677, 693.625487703081);
    points += QPointF(1070.1212524493003, 695.6022659553572);
    points += QPointF(1074.3830151134664, 697.7096649640414);
    points += QPointF(1078.7265805419665, 699.565875909043);
    points += QPointF(1083.206554246492, 701.1878034134409);
    points += QPointF(1087.891993301369, 702.5858632116882);
    points += QPointF(1092.8646688655851, 703.7605199081748);
    points += QPointF(1098.2162239125396, 704.699271425244);
    points += QPointF(1104.0444208482252, 705.3744462055462);
    points += QPointF(1111.3488014116294, 705.7937559506272);
    points += QPointF(1290.2705536650083, 707.4996206162249);
    points += QPointF(1302.416131870577, 881.6558256043734);
    points += QPointF(30.0, 970.3935748031496);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsSeamTest1PieceByFirstEdgeSymmerty() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(30.0, 894.8030236220472);
    p.SetSABefore(75.59055118110237);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    points.append(VSAPoint(47.53637144700478, 39.80977598775412));
    points.append(VSAPoint(85.4092492068762, 37.36146293134895));
    points.append(VSAPoint(125.57923134501877, 32.13056337758208));
    points.append(VSAPoint(156.5108311435474, 26.074993549693005));
    points.append(VSAPoint(177.21129897469388, 21.059222548891512));
    points.append(VSAPoint(197.8216319874312, 15.159776113211533));
    points.append(VSAPoint(218.19418331220902, 8.334870062533497));
    points.append(VSAPoint(238.18130607947683, 0.5427202167378373));
    points.append(VSAPoint(257.6353534196842, -8.258457604295018));
    points.append(VSAPoint(276.4086784632808, -18.110447580684635));
    points.append(VSAPoint(294.3536343407161, -29.055033892550586));
    points.append(VSAPoint(311.32257418243984, -41.13400072001244));
    points.append(VSAPoint(327.16785111890135, -54.389132243189756));

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points.append(p);

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points.append(p);

    p = VSAPoint(519.4465667350105, -36.01405338211433);
    p.SetSABefore(0.3779527559055119);
    points.append(p);

    points.append(VSAPoint(460.3937007874016, 342.36207874015753));

    p = VSAPoint(620.5290071875436, -5.506318761785565);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(930.9115139455133, 164.2021651072791);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(907.1077633892038, 216.20756088951924);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(886.6420235246574, 264.5706340127217);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(869.2733175133691, 309.4054580978941);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(854.760668516834, 350.82610676604384);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(842.8630996965472, 388.9466536381784);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(833.3396342140039, 423.88117233530534);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(825.9492952306991, 455.74373647843197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.451105908128, 484.6484196885659);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.6040894077855, 510.7092955867147);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1672688911669, 534.0404377938855);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.8996675197673, 554.7559199310863);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.5925673946043, 581.4656260538284);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1809574610598, 608.5715133604979);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(815.1968503937007, 618.5825511811024);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.3634787672067, 628.380932746632);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.4053369940075, 649.1688436779486);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(826.5040284899703, 671.0654086429722);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(834.6667882477791, 693.7142683899226);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(844.9008512601183, 716.7590636670197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(857.2134525196718, 739.8434352224829);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(871.6118270191237, 762.6110238045324);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(888.1032097511581, 784.7054701613878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(906.6948357084594, 805.7704150412691);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(927.3939398837115, 825.4494991923958);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(950.2077572695983, 843.3863633629879);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(968.776733605274, 855.4073744953336);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(981.7764600118351, 862.7350547966848);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(995.3089344044396, 869.4267280237759);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1009.3750611571733, 875.4378492701345);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1023.9757446441214, 880.7238736292878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1039.1118892393697, 885.2402561947638);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1054.784399317004, 888.9424520600894);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1070.994179251109, 891.7859163187926);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1087.7421334157707, 893.7261040644007);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1105.0291661850742, 894.7184703904409);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsSeamTest1PieceByFirstEdgeSymmerty() const
{
    QVector<QPointF> points;

    points += QPointF(30.0, 970.3935748031496);
    points += QPointF(30.0, 2.2023778242823937);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.74605442368645, -0.2758951981748612);
    points += QPointF(119.50208828052781, -5.192452541281906);
    points += QPointF(148.42595722778273, -10.854963023883784);
    points += QPointF(167.5566961450379, -15.490385095876569);
    points += QPointF(186.61281642420556, -20.94495784777482);
    points += QPointF(205.31950565747624, -27.21179180985321);
    points += QPointF(223.51736443487215, -34.306381867523655);
    points += QPointF(241.0503469884652, -42.23845350735769);
    points += QPointF(257.76798751706616, -51.01164773556077);
    points += QPointF(273.5285590176657, -60.62398316233916);
    points += QPointF(288.203122586733, -71.06974867341756);
    points += QPointF(301.89505943185964, -82.52353546533966);
    points += QPointF(315.2716370538558, -95.41962699710956);
    points += QPointF(334.9131314168676, -61.85620240749991);
    points += QPointF(556.951317720607, -31.220946854728947);
    points += QPointF(539.6101141051189, 79.89131577778124);
    points += QPointF(585.9167315845332, -20.7024207218235);
    points += QPointF(1117.4559101159903, 212.6634418802967);
    points += QPointF(1101.8482040478996, 244.8082798528157);
    points += QPointF(1080.077560308019, 292.3718325563429);
    points += QPointF(1061.8089025447719, 335.54292908714797);
    points += QPointF(1046.6013689355652, 374.7989965048928);
    points += QPointF(1034.1843603449322, 410.2384638595293);
    points += QPointF(1024.2819001340656, 441.96655926454446);
    points += QPointF(1016.6128103198596, 470.0987514791764);
    points += QPointF(1010.891548127259, 494.7652903217379);
    points += QPointF(1006.8300406648392, 516.1171535751373);
    points += QPointF(1004.1409514475167, 534.3338730421456);
    points += QPointF(1002.5429209641059, 549.6340874837038);
    points += QPointF(1001.8096437359794, 561.6175009318105);
    points += QPointF(1001.632561636562, 577.0190278199144);
    points += QPointF(1002.5982079396683, 593.4977885619218);
    points += QPointF(1003.0419281414149, 597.870394747796);
    points += QPointF(1003.1926540422378, 599.1363247745379);
    points += QPointF(1004.4715714217981, 605.7139974891637);
    points += QPointF(1006.6672239434022, 613.5972045855956);
    points += QPointF(1010.1411052399322, 623.2360341722922);
    points += QPointF(1014.8474980040493, 633.8337662374475);
    points += QPointF(1020.6660121283516, 644.7426504865729);
    points += QPointF(1027.4243046890665, 655.4292751847524);
    points += QPointF(1034.913026324476, 665.4623428264111);
    points += QPointF(1042.9029269068749, 674.5151720813944);
    points += QPointF(1051.171411576988, 682.3761985416603);
    points += QPointF(1060.1878101895825, 689.4651444417038);
    points += QPointF(1066.6143341082677, 693.625487703081);
    points += QPointF(1070.1212524493003, 695.6022659553572);
    points += QPointF(1074.3830151134664, 697.7096649640414);
    points += QPointF(1078.7265805419665, 699.565875909043);
    points += QPointF(1083.206554246492, 701.1878034134409);
    points += QPointF(1087.891993301369, 702.5858632116882);
    points += QPointF(1092.8646688655851, 703.7605199081748);
    points += QPointF(1098.2162239125396, 704.699271425244);
    points += QPointF(1104.0444208482252, 705.3744462055462);
    points += QPointF(1111.3488014116294, 705.7937559506272);
    points += QPointF(1290.2705536650083, 707.4996206162249);
    points += QPointF(1302.416131870577, 881.6558256043734);
    points += QPointF(30.0, 970.3935748031496);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsSeamTest1PieceBySecondEdgeSymmerty() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(30.0, 894.8030236220472);
    p.SetSABefore(75.59055118110237);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    points.append(VSAPoint(47.53637144700478, 39.80977598775412));
    points.append(VSAPoint(85.4092492068762, 37.36146293134895));
    points.append(VSAPoint(125.57923134501877, 32.13056337758208));
    points.append(VSAPoint(156.5108311435474, 26.074993549693005));
    points.append(VSAPoint(177.21129897469388, 21.059222548891512));
    points.append(VSAPoint(197.8216319874312, 15.159776113211533));
    points.append(VSAPoint(218.19418331220902, 8.334870062533497));
    points.append(VSAPoint(238.18130607947683, 0.5427202167378373));
    points.append(VSAPoint(257.6353534196842, -8.258457604295018));
    points.append(VSAPoint(276.4086784632808, -18.110447580684635));
    points.append(VSAPoint(294.3536343407161, -29.055033892550586));
    points.append(VSAPoint(311.32257418243984, -41.13400072001244));
    points.append(VSAPoint(327.16785111890135, -54.389132243189756));

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points.append(p);

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points.append(p);

    p = VSAPoint(519.4465667350105, -36.01405338211433);
    p.SetSABefore(0.3779527559055119);
    points.append(p);

    points.append(VSAPoint(460.3937007874016, 342.36207874015753));

    p = VSAPoint(620.5290071875436, -5.506318761785565);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(930.9115139455133, 164.2021651072791);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(907.1077633892038, 216.20756088951924);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(886.6420235246574, 264.5706340127217);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(869.2733175133691, 309.4054580978941);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(854.760668516834, 350.82610676604384);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(842.8630996965472, 388.9466536381784);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(833.3396342140039, 423.88117233530534);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(825.9492952306991, 455.74373647843197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.451105908128, 484.6484196885659);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.6040894077855, 510.7092955867147);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1672688911669, 534.0404377938855);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.8996675197673, 554.7559199310863);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.5925673946043, 581.4656260538284);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1809574610598, 608.5715133604979);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(815.1968503937007, 618.5825511811024);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.3634787672067, 628.380932746632);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.4053369940075, 649.1688436779486);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(826.5040284899703, 671.0654086429722);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(834.6667882477791, 693.7142683899226);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(844.9008512601183, 716.7590636670197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(857.2134525196718, 739.8434352224829);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(871.6118270191237, 762.6110238045324);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(888.1032097511581, 784.7054701613878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(906.6948357084594, 805.7704150412691);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(927.3939398837115, 825.4494991923958);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(950.2077572695983, 843.3863633629879);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(968.776733605274, 855.4073744953336);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(981.7764600118351, 862.7350547966848);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(995.3089344044396, 869.4267280237759);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1009.3750611571733, 875.4378492701345);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1023.9757446441214, 880.7238736292878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1039.1118892393697, 885.2402561947638);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1054.784399317004, 888.9424520600894);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1070.994179251109, 891.7859163187926);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1087.7421334157707, 893.7261040644007);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1105.0291661850742, 894.7184703904409);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsSeamTest1PieceBySecondEdgeSymmerty() const
{
    QVector<QPointF> points;

    points += QPointF(30.0, 970.3935748031496);
    points += QPointF(30.0, 2.2023778242823937);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.74605442368645, -0.2758951981748612);
    points += QPointF(119.50208828052781, -5.192452541281906);
    points += QPointF(148.42595722778273, -10.854963023883784);
    points += QPointF(167.5566961450379, -15.490385095876569);
    points += QPointF(186.61281642420556, -20.94495784777482);
    points += QPointF(205.31950565747624, -27.21179180985321);
    points += QPointF(223.51736443487215, -34.306381867523655);
    points += QPointF(241.0503469884652, -42.23845350735769);
    points += QPointF(257.76798751706616, -51.01164773556077);
    points += QPointF(273.5285590176657, -60.62398316233916);
    points += QPointF(284.94491218126115, -68.7504632158891);
    points += QPointF(286.89008170747013, -68.48208250840415);
    points += QPointF(556.951317720607, -31.220946854728947);
    points += QPointF(539.6101141051189, 79.89131577778124);
    points += QPointF(585.9167315845332, -20.7024207218235);
    points += QPointF(1117.4559101159903, 212.6634418802967);
    points += QPointF(1101.8482040478996, 244.8082798528157);
    points += QPointF(1080.077560308019, 292.3718325563429);
    points += QPointF(1061.8089025447719, 335.54292908714797);
    points += QPointF(1046.6013689355652, 374.7989965048928);
    points += QPointF(1034.1843603449322, 410.2384638595293);
    points += QPointF(1024.2819001340656, 441.96655926454446);
    points += QPointF(1016.6128103198596, 470.0987514791764);
    points += QPointF(1010.891548127259, 494.7652903217379);
    points += QPointF(1006.8300406648392, 516.1171535751373);
    points += QPointF(1004.1409514475167, 534.3338730421456);
    points += QPointF(1002.5429209641059, 549.6340874837038);
    points += QPointF(1001.8096437359794, 561.6175009318105);
    points += QPointF(1001.632561636562, 577.0190278199144);
    points += QPointF(1002.5982079396683, 593.4977885619218);
    points += QPointF(1003.0419281414149, 597.870394747796);
    points += QPointF(1003.1926540422378, 599.1363247745379);
    points += QPointF(1004.4715714217981, 605.7139974891637);
    points += QPointF(1006.6672239434022, 613.5972045855956);
    points += QPointF(1010.1411052399322, 623.2360341722922);
    points += QPointF(1014.8474980040493, 633.8337662374475);
    points += QPointF(1020.6660121283516, 644.7426504865729);
    points += QPointF(1027.4243046890665, 655.4292751847524);
    points += QPointF(1034.913026324476, 665.4623428264111);
    points += QPointF(1042.9029269068749, 674.5151720813944);
    points += QPointF(1051.171411576988, 682.3761985416603);
    points += QPointF(1060.1878101895825, 689.4651444417038);
    points += QPointF(1066.6143341082677, 693.625487703081);
    points += QPointF(1070.1212524493003, 695.6022659553572);
    points += QPointF(1074.3830151134664, 697.7096649640414);
    points += QPointF(1078.7265805419665, 699.565875909043);
    points += QPointF(1083.206554246492, 701.1878034134409);
    points += QPointF(1087.891993301369, 702.5858632116882);
    points += QPointF(1092.8646688655851, 703.7605199081748);
    points += QPointF(1098.2162239125396, 704.699271425244);
    points += QPointF(1104.0444208482252, 705.3744462055462);
    points += QPointF(1111.3488014116294, 705.7937559506272);
    points += QPointF(1290.2705536650083, 707.4996206162249);
    points += QPointF(1302.416131870577, 881.6558256043734);
    points += QPointF(30.0, 970.3935748031496);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsSeamTest1PieceByFirstRightAngle() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(30.0, 894.8030236220472);
    p.SetSABefore(75.59055118110237);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    points.append(VSAPoint(47.53637144700478, 39.80977598775412));
    points.append(VSAPoint(85.4092492068762, 37.36146293134895));
    points.append(VSAPoint(125.57923134501877, 32.13056337758208));
    points.append(VSAPoint(156.5108311435474, 26.074993549693005));
    points.append(VSAPoint(177.21129897469388, 21.059222548891512));
    points.append(VSAPoint(197.8216319874312, 15.159776113211533));
    points.append(VSAPoint(218.19418331220902, 8.334870062533497));
    points.append(VSAPoint(238.18130607947683, 0.5427202167378373));
    points.append(VSAPoint(257.6353534196842, -8.258457604295018));
    points.append(VSAPoint(276.4086784632808, -18.110447580684635));
    points.append(VSAPoint(294.3536343407161, -29.055033892550586));
    points.append(VSAPoint(311.32257418243984, -41.13400072001244));
    points.append(VSAPoint(327.16785111890135, -54.389132243189756));

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points.append(p);

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points.append(p);

    p = VSAPoint(519.4465667350105, -36.01405338211433);
    p.SetSABefore(0.3779527559055119);
    points.append(p);

    points.append(VSAPoint(460.3937007874016, 342.36207874015753));

    p = VSAPoint(620.5290071875436, -5.506318761785565);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(930.9115139455133, 164.2021651072791);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(907.1077633892038, 216.20756088951924);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(886.6420235246574, 264.5706340127217);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(869.2733175133691, 309.4054580978941);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(854.760668516834, 350.82610676604384);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(842.8630996965472, 388.9466536381784);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(833.3396342140039, 423.88117233530534);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(825.9492952306991, 455.74373647843197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.451105908128, 484.6484196885659);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.6040894077855, 510.7092955867147);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1672688911669, 534.0404377938855);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.8996675197673, 554.7559199310863);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.5925673946043, 581.4656260538284);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1809574610598, 608.5715133604979);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(815.1968503937007, 618.5825511811024);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.3634787672067, 628.380932746632);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.4053369940075, 649.1688436779486);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(826.5040284899703, 671.0654086429722);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(834.6667882477791, 693.7142683899226);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(844.9008512601183, 716.7590636670197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(857.2134525196718, 739.8434352224829);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(871.6118270191237, 762.6110238045324);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(888.1032097511581, 784.7054701613878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(906.6948357084594, 805.7704150412691);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(927.3939398837115, 825.4494991923958);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(950.2077572695983, 843.3863633629879);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(968.776733605274, 855.4073744953336);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(981.7764600118351, 862.7350547966848);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(995.3089344044396, 869.4267280237759);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1009.3750611571733, 875.4378492701345);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1023.9757446441214, 880.7238736292878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1039.1118892393697, 885.2402561947638);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1054.784399317004, 888.9424520600894);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1070.994179251109, 891.7859163187926);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1087.7421334157707, 893.7261040644007);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1105.0291661850742, 894.7184703904409);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsSeamTest1PieceByFirstRightAngle() const
{
    QVector<QPointF> points;

    points += QPointF(30.0, 970.3935748031496);
    points += QPointF(30.0, 2.2023778242823937);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.74605442368645, -0.2758951981748612);
    points += QPointF(119.50208828052781, -5.192452541281906);
    points += QPointF(148.42595722778273, -10.854963023883784);
    points += QPointF(167.5566961450379, -15.490385095876569);
    points += QPointF(186.61281642420556, -20.94495784777482);
    points += QPointF(205.31950565747624, -27.21179180985321);
    points += QPointF(223.51736443487215, -34.306381867523655);
    points += QPointF(241.0503469884652, -42.23845350735769);
    points += QPointF(257.76798751706616, -51.01164773556077);
    points += QPointF(273.5285590176657, -60.62398316233916);
    points += QPointF(288.203122586733, -71.06974867341756);
    points += QPointF(301.89505943185964, -82.52353546533966);
    points += QPointF(308.6809861986896, -89.06571203590164);
    points += QPointF(334.91313141686754, -61.856202407499914);
    points += QPointF(556.951317720607, -31.220946854728947);
    points += QPointF(539.6101141051189, 79.89131577778124);
    points += QPointF(585.9167315845332, -20.7024207218235);
    points += QPointF(1117.4559101159903, 212.6634418802967);
    points += QPointF(1101.8482040478996, 244.8082798528157);
    points += QPointF(1080.077560308019, 292.3718325563429);
    points += QPointF(1061.8089025447719, 335.54292908714797);
    points += QPointF(1046.6013689355652, 374.7989965048928);
    points += QPointF(1034.1843603449322, 410.2384638595293);
    points += QPointF(1024.2819001340656, 441.96655926454446);
    points += QPointF(1016.6128103198596, 470.0987514791764);
    points += QPointF(1010.891548127259, 494.7652903217379);
    points += QPointF(1006.8300406648392, 516.1171535751373);
    points += QPointF(1004.1409514475167, 534.3338730421456);
    points += QPointF(1002.5429209641059, 549.6340874837038);
    points += QPointF(1001.8096437359794, 561.6175009318105);
    points += QPointF(1001.632561636562, 577.0190278199144);
    points += QPointF(1002.5982079396683, 593.4977885619218);
    points += QPointF(1003.0419281414149, 597.870394747796);
    points += QPointF(1003.1926540422378, 599.1363247745379);
    points += QPointF(1004.4715714217981, 605.7139974891637);
    points += QPointF(1006.6672239434022, 613.5972045855956);
    points += QPointF(1010.1411052399322, 623.2360341722922);
    points += QPointF(1014.8474980040493, 633.8337662374475);
    points += QPointF(1020.6660121283516, 644.7426504865729);
    points += QPointF(1027.4243046890665, 655.4292751847524);
    points += QPointF(1034.913026324476, 665.4623428264111);
    points += QPointF(1042.9029269068749, 674.5151720813944);
    points += QPointF(1051.171411576988, 682.3761985416603);
    points += QPointF(1060.1878101895825, 689.4651444417038);
    points += QPointF(1066.6143341082677, 693.625487703081);
    points += QPointF(1070.1212524493003, 695.6022659553572);
    points += QPointF(1074.3830151134664, 697.7096649640414);
    points += QPointF(1078.7265805419665, 699.565875909043);
    points += QPointF(1083.206554246492, 701.1878034134409);
    points += QPointF(1087.891993301369, 702.5858632116882);
    points += QPointF(1092.8646688655851, 703.7605199081748);
    points += QPointF(1098.2162239125396, 704.699271425244);
    points += QPointF(1104.0444208482252, 705.3744462055462);
    points += QPointF(1111.3488014116294, 705.7937559506272);
    points += QPointF(1290.2705536650083, 707.4996206162249);
    points += QPointF(1302.416131870577, 881.6558256043734);
    points += QPointF(30.0, 970.3935748031496);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsSeamTest1PieceBySecondRightAngle() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(30.0, 894.8030236220472);
    p.SetSABefore(75.59055118110237);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetSABefore(0.0);
    points.append(p);

    points.append(VSAPoint(47.53637144700478, 39.80977598775412));
    points.append(VSAPoint(85.4092492068762, 37.36146293134895));
    points.append(VSAPoint(125.57923134501877, 32.13056337758208));
    points.append(VSAPoint(156.5108311435474, 26.074993549693005));
    points.append(VSAPoint(177.21129897469388, 21.059222548891512));
    points.append(VSAPoint(197.8216319874312, 15.159776113211533));
    points.append(VSAPoint(218.19418331220902, 8.334870062533497));
    points.append(VSAPoint(238.18130607947683, 0.5427202167378373));
    points.append(VSAPoint(257.6353534196842, -8.258457604295018));
    points.append(VSAPoint(276.4086784632808, -18.110447580684635));
    points.append(VSAPoint(294.3536343407161, -29.055033892550586));
    points.append(VSAPoint(311.32257418243984, -41.13400072001244));
    points.append(VSAPoint(327.16785111890135, -54.389132243189756));

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points.append(p);

    p = VSAPoint(334.5669291338582, -61.522435695538036);
    p.SetSAAfter(0.3779527559055119);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points.append(p);

    p = VSAPoint(519.4465667350105, -36.01405338211433);
    p.SetSABefore(0.3779527559055119);
    points.append(p);

    points.append(VSAPoint(460.3937007874016, 342.36207874015753));

    p = VSAPoint(620.5290071875436, -5.506318761785565);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(944.294593326342, 136.63905516701556);
    p.SetSABefore(0.0);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(930.9115139455133, 164.2021651072791);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(907.1077633892038, 216.20756088951924);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(886.6420235246574, 264.5706340127217);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(869.2733175133691, 309.4054580978941);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(854.760668516834, 350.82610676604384);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(842.8630996965472, 388.9466536381784);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(833.3396342140039, 423.88117233530534);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(825.9492952306991, 455.74373647843197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.451105908128, 484.6484196885659);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.6040894077855, 510.7092955867147);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1672688911669, 534.0404377938855);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.8996675197673, 554.7559199310863);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(812.5925673946043, 581.4656260538284);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(814.1809574610598, 608.5715133604979);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(815.1968503937007, 618.5825511811024);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(816.3634787672067, 628.380932746632);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(820.4053369940075, 649.1688436779486);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(826.5040284899703, 671.0654086429722);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(834.6667882477791, 693.7142683899226);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(844.9008512601183, 716.7590636670197);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(857.2134525196718, 739.8434352224829);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(871.6118270191237, 762.6110238045324);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(888.1032097511581, 784.7054701613878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(906.6948357084594, 805.7704150412691);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(927.3939398837115, 825.4494991923958);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(950.2077572695983, 843.3863633629879);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(968.776733605274, 855.4073744953336);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(981.7764600118351, 862.7350547966848);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(995.3089344044396, 869.4267280237759);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1009.3750611571733, 875.4378492701345);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1023.9757446441214, 880.7238736292878);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1039.1118892393697, 885.2402561947638);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1054.784399317004, 888.9424520600894);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1070.994179251109, 891.7859163187926);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1087.7421334157707, 893.7261040644007);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1105.0291661850742, 894.7184703904409);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(188.9763779527559);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSABefore(188.9763779527559);
    p.SetSAAfter(0.0);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsSeamTest1PieceBySecondRightAngle() const
{
    QVector<QPointF> points;

    points += QPointF(30.0, 970.3935748031496);
    points += QPointF(30.0, 2.2023778242823937);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.74605442368645, -0.2758951981748612);
    points += QPointF(119.50208828052781, -5.192452541281906);
    points += QPointF(148.42595722778273, -10.854963023883784);
    points += QPointF(167.5566961450379, -15.490385095876569);
    points += QPointF(186.61281642420556, -20.94495784777482);
    points += QPointF(205.31950565747624, -27.21179180985321);
    points += QPointF(223.51736443487215, -34.306381867523655);
    points += QPointF(241.0503469884652, -42.23845350735769);
    points += QPointF(257.76798751706616, -51.01164773556077);
    points += QPointF(273.5285590176657, -60.62398316233916);
    points += QPointF(284.4959443876259, -68.4308753608727);
    points += QPointF(556.951317720607, -31.220946854728947);
    points += QPointF(539.6101141051189, 79.89131577778124);
    points += QPointF(585.9167315845332, -20.7024207218235);
    points += QPointF(1117.4559101159903, 212.6634418802967);
    points += QPointF(1101.8482040478996, 244.8082798528157);
    points += QPointF(1080.077560308019, 292.3718325563429);
    points += QPointF(1061.8089025447719, 335.54292908714797);
    points += QPointF(1046.6013689355652, 374.7989965048928);
    points += QPointF(1034.1843603449322, 410.2384638595293);
    points += QPointF(1024.2819001340656, 441.96655926454446);
    points += QPointF(1016.6128103198596, 470.0987514791764);
    points += QPointF(1010.891548127259, 494.7652903217379);
    points += QPointF(1006.8300406648392, 516.1171535751373);
    points += QPointF(1004.1409514475167, 534.3338730421456);
    points += QPointF(1002.5429209641059, 549.6340874837038);
    points += QPointF(1001.8096437359794, 561.6175009318105);
    points += QPointF(1001.632561636562, 577.0190278199144);
    points += QPointF(1002.5982079396683, 593.4977885619218);
    points += QPointF(1003.0419281414149, 597.870394747796);
    points += QPointF(1003.1926540422378, 599.1363247745379);
    points += QPointF(1004.4715714217981, 605.7139974891637);
    points += QPointF(1006.6672239434022, 613.5972045855956);
    points += QPointF(1010.1411052399322, 623.2360341722922);
    points += QPointF(1014.8474980040493, 633.8337662374475);
    points += QPointF(1020.6660121283516, 644.7426504865729);
    points += QPointF(1027.4243046890665, 655.4292751847524);
    points += QPointF(1034.913026324476, 665.4623428264111);
    points += QPointF(1042.9029269068749, 674.5151720813944);
    points += QPointF(1051.171411576988, 682.3761985416603);
    points += QPointF(1060.1878101895825, 689.4651444417038);
    points += QPointF(1066.6143341082677, 693.625487703081);
    points += QPointF(1070.1212524493003, 695.6022659553572);
    points += QPointF(1074.3830151134664, 697.7096649640414);
    points += QPointF(1078.7265805419665, 699.565875909043);
    points += QPointF(1083.206554246492, 701.1878034134409);
    points += QPointF(1087.891993301369, 702.5858632116882);
    points += QPointF(1092.8646688655851, 703.7605199081748);
    points += QPointF(1098.2162239125396, 704.699271425244);
    points += QPointF(1104.0444208482252, 705.3744462055462);
    points += QPointF(1111.3488014116294, 705.7937559506272);
    points += QPointF(1290.2705536650083, 707.4996206162249);
    points += QPointF(1302.416131870577, 881.6558256043734);
    points += QPointF(30.0, 970.3935748031496);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsCase2() const
{

    QVector<VSAPoint> points;

//    VSAPoint p = VSAPoint(927.3939398837115, 825.4494991923958);
//    p.SetSABefore(188.9763779527559);
//    p.SetSAAfter(188.9763779527559);
//    points.append(p);

    points.append(VSAPoint(30.0, 39.999874015748034));
    points.append(VSAPoint(407.9527559055118, 39.999874015748034));
    points.append(VSAPoint(407.9527559055118, 228.97625196850396));
    points.append(VSAPoint(407.9527559055118, 228.97625196850396));

    VSAPoint p = VSAPoint(403.3769166670402, 231.4465511704684);
    p.SetSABefore(37.80317793023137);
    p.SetSAAfter(37.80317793023137);
    points.append(p);

    p = VSAPoint(394.1607984354165, 235.58132461572228);
    p.SetSABefore(37.8185282558241);
    p.SetSAAfter(37.8185282558241);
    points.append(p);

    p = VSAPoint(384.8923171505302, 238.7905285112787);
    p.SetSABefore(37.83343362828719);
    p.SetSAAfter(37.83343362828719);
    points.append(p);

    p = VSAPoint(375.59921131499664, 241.12018552459608);
    p.SetSABefore(37.84799299430957);
    p.SetSAAfter(37.84799299430957);
    points.append(p);

    p = VSAPoint(366.3092194314313, 242.61631832313287);
    p.SetSABefore(37.86229254590959);
    p.SetSAAfter(37.86229254590959);
    points.append(p);

    p = VSAPoint(357.0500800024495, 243.3249495743475);
    p.SetSABefore(37.87640445213285);
    p.SetSAAfter(37.87640445213285);
    points.append(p);

    p = VSAPoint(343.267372346902, 243.09606124484026);
    p.SetSABefore(37.897352390596396);
    p.SetSAAfter(37.897352390596396);
    points.append(p);

    p = VSAPoint(325.2670702478247, 240.34058647283786);
    p.SetSABefore(37.92502540251962);
    p.SetSAAfter(37.92502540251962);
    points.append(p);

    p = VSAPoint(307.8894710141585, 235.26351352579883);
    p.SetSABefore(37.952537472540754);
    p.SetSAAfter(37.952537472540754);
    points.append(p);

    p = VSAPoint(291.35648266682676, 228.23302374339065);
    p.SetSABefore(37.97983930259204);
    p.SetSAAfter(37.97983930259204);
    points.append(p);

    p = VSAPoint(275.89001322675244, 219.61729846528084);
    p.SetSABefore(38.00674385290581);
    p.SetSAAfter(38.00674385290581);
    points.append(p);

    p = VSAPoint(261.71197071485875, 209.784519031137);
    p.SetSABefore(38.032964089440014);
    p.SetSAAfter(38.032964089440014);
    points.append(p);

    p = VSAPoint(249.04426315206877, 199.10286678062653);
    p.SetSABefore(38.058145046329095);
    p.SetSAAfter(38.058145046329095);
    points.append(p);

    p = VSAPoint(238.10879855930557, 187.94052305341705);
    p.SetSABefore(38.081891784254736);
    p.SetSAAfter(38.081891784254736);
    points.append(p);

    p = VSAPoint(229.12748495749224, 176.66566918917607);
    p.SetSABefore(38.10379739397839);
    p.SetSAAfter(38.10379739397839);
    points.append(p);

    p = VSAPoint(222.32223036755198, 165.64648652757114);
    p.SetSABefore(38.12347885155327);
    p.SetSAAfter(38.12347885155327);
    points.append(p);

    p = VSAPoint(217.91494281040778, 155.2511564082697);
    p.SetSABefore(38.140637376066095);
    p.SetSAAfter(38.140637376066095);
    points.append(p);

    p = VSAPoint(216.12753030698286, 145.84786017093933);
    p.SetSABefore(38.15518307122048);
    p.SetSAAfter(38.15518307122048);
    points.append(p);

    p = VSAPoint(216.7337621675769, 139.71903029095353);
    p.SetSABefore(38.16454227070837);
    p.SetSAAfter(38.16454227070837);
    points.append(p);

    p = VSAPoint(218.01300097576924, 136.10657758970495);
    p.SetSABefore(38.17036601862179);
    p.SetSAAfter(38.17036601862179);
    points.append(p);

    p = VSAPoint(218.97637795275605, 134.48806299212646);
    p.SetSABefore(38.173228346456696);
    p.SetSAAfter(38.173228346456696);
    points.append(p);

    p = VSAPoint(218.97637795275605, 134.48806299212646);
    p.SetSABefore(38.173228346456696);
    p.SetSAAfter(38.173228346456696);
    points.append(p);

    p = VSAPoint(218.97637795275605, 134.48806299212646);
    p.SetSABefore(38.173228346456696);
    p.SetSAAfter(38.173228346456696);
    points.append(p);

    p = VSAPoint(217.1202289172026, 137.73030103616844);
    p.SetSABefore(38.16799523335242);
    p.SetSAAfter(38.16799523335242);
    points.append(p);

    p = VSAPoint(212.6973363405255, 143.4397404435662);
    p.SetSABefore(38.15787884818713);
    p.SetSAAfter(38.15787884818713);
    points.append(p);

    p = VSAPoint(207.48487786706698, 148.24866810991395);
    p.SetSABefore(38.14794488580081);
    p.SetSAAfter(38.14794488580081);
    points.append(p);

    p = VSAPoint(201.55301739671896, 152.18989767496004);
    p.SetSABefore(38.13796906496172);
    p.SetSAAfter(38.13796906496172);
    points.append(p);

    p = VSAPoint(194.9719188293733, 155.29624277845284);
    p.SetSABefore(38.12777534088115);
    p.SetSAAfter(38.12777534088115);
    points.append(p);

    p = VSAPoint(187.81174606492203, 157.6005170601407);
    p.SetSABefore(38.1172392196169);
    p.SetSAAfter(38.1172392196169);
    points.append(p);

    p = VSAPoint(180.14266300325704, 159.13553415977202);
    p.SetSABefore(38.10628374748414);
    p.SetSAAfter(38.10628374748414);
    points.append(p);

    p = VSAPoint(172.0348335442702, 159.93410771709506);
    p.SetSABefore(38.09487182200063);
    p.SetSAAfter(38.09487182200063);
    points.append(p);

    p = VSAPoint(159.23684047277425, 159.9047174285684);
    p.SetSABefore(38.07694509829885);
    p.SetSAAfter(38.07694509829885);
    points.append(p);

    p = VSAPoint(141.23066996957303, 157.47696696634546);
    p.SetSABefore(38.051494892780326);
    p.SetSAAfter(38.051494892780326);
    points.append(p);

    p = VSAPoint(122.73246407513939, 152.69408785036);
    p.SetSABefore(38.02473158624051);
    p.SetSAAfter(38.02473158624051);
    points.append(p);

    p = VSAPoint(104.30353398860882, 145.81858919859886);
    p.SetSABefore(37.99717939647611);
    p.SetSAAfter(37.99717939647611);
    points.append(p);

    p = VSAPoint(86.50519090911675, 137.11298012904894);
    p.SetSABefore(37.969426027228295);
    p.SetSAAfter(37.969426027228295);
    points.append(p);

    p = VSAPoint(69.89874603579864, 126.83976975969698);
    p.SetSABefore(37.94207342582994);
    p.SetSAAfter(37.94207342582994);
    points.append(p);

    p = VSAPoint(55.045510567789904, 115.26146720852991);
    p.SetSABefore(37.91569349681988);
    p.SetSAAfter(37.91569349681988);
    points.append(p);

    p = VSAPoint(42.506795704226036, 102.64058159353453);
    p.SetSABefore(37.890773443464376);
    p.SetSAAfter(37.890773443464376);
    points.append(p);

    p = VSAPoint(35.06235294654161, 92.63041313458488);
    p.SetSABefore(37.87329933285639);
    p.SetSAAfter(37.87329933285639);
    points.append(p);

    p = VSAPoint(31.055115217282797, 85.78413532732864);
    p.SetSABefore(37.86218751893835);
    p.SetSAAfter(37.86218751893835);
    points.append(p);

    p = VSAPoint(27.94224518864889, 78.82487313298289);
    p.SetSABefore(37.851508643228605);
    p.SetSAAfter(37.851508643228605);
    points.append(p);

    p = VSAPoint(25.793906760531815, 71.78544019129603);
    p.SetSABefore(37.84119924740421);
    p.SetSAAfter(37.84119924740421);
    points.append(p);

    p = VSAPoint(24.68026383282351, 64.69865014201642);
    p.SetSABefore(37.83115066985303);
    p.SetSAAfter(37.83115066985303);
    points.append(p);

    p = VSAPoint(24.671480305415898, 57.597316624892386);
    p.SetSABefore(37.8212035316683);
    p.SetSAAfter(37.8212035316683);
    points.append(p);

    p = VSAPoint(25.837720078200917, 50.514253279672296);
    p.SetSABefore(37.81114840454936);
    p.SetSAAfter(37.81114840454936);
    points.append(p);

    p = VSAPoint(28.2491470510705, 43.48227374610451);
    p.SetSABefore(37.80073535459479);
    p.SetSAAfter(37.80073535459479);
    points.append(p);

    points.append(VSAPoint(30.0, 39.999874015748034));

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsCase2() const
{
    QVector<QPointF> points;

    points += QPointF(6.735602489575015, 2.204598425196849);
    points += QPointF(445.74803149606305, 2.204598425196849);
    points += QPointF(445.748031496063, 251.4844627007215);
    points += QPointF(421.3353745697487, 264.71175899157447);
    points += QPointF(418.8511557813181, 265.9375381882219);
    points += QPointF(409.64132100578746, 270.0863170137037);
    points += QPointF(406.534680371908, 271.3182540481497);
    points += QPointF(397.2710759906872, 274.5415428980511);
    points += QPointF(394.0919849710222, 275.48841417378816);
    points += QPointF(384.8024194256035, 277.83219356243256);
    points += QPointF(381.6170070948525, 278.48670406257685);
    points += QPointF(372.3292888267279, 279.99695450428766);
    points += QPointF(369.1984911888747, 280.3682097399039);
    points += QPointF(358.2408334277008, 281.22358539078493);
    points += QPointF(340.1284056058474, 280.95032539874563);
    points += QPointF(317.1003672226287, 277.46101449907337);
    points += QPointF(295.1519528906243, 271.0847085461394);
    points += QPointF(274.68871290802866, 262.4196148903778);
    points += QPointF(255.81310453620574, 251.94235440791147);
    points += QPointF(238.61081765006833, 240.0508703303418);
    points += QPointF(223.16837814314113, 227.06961282699527);
    points += QPointF(209.5774971397173, 213.23886816257252);
    points += QPointF(197.93811921404816, 198.6726417606856);
    points += QPointF(195.59837660985662, 194.8969375402249);
    points += QPointF(195.2927772651506, 194.97641969682226);
    points += QPointF(187.62154404176044, 196.50069439568804);
    points += QPointF(183.8778341349861, 197.05831567247884);
    points += QPointF(173.80935665477787, 198.03576335366748);
    points += QPointF(156.58462346404755, 197.97207956246808);
    points += QPointF(133.84749301666088, 194.8740254865072);
    points += QPointF(111.24564892141993, 188.99632536164253);
    points += QPointF(89.23016638120879, 180.74760051351964);
    points += QPointF(68.08422273452526, 170.36786581256206);
    points += QPointF(48.14786227034436, 157.9960303610079);
    points += QPointF(29.800888286137585, 143.65294976668383);
    points += QPointF(13.683525778751441, 127.38445078151254);
    points += QPointF(3.3994938238576147, 113.51550175712616);
    points += QPointF(-1.6212069307357524, 104.9101180366734);
    points += QPointF(-3.5070767733614048, 101.24376431773773);
    points += QPointF(-7.59130229548195, 92.07847667888365);
    points += QPointF(-11.18749819246625, 80.23547351371789);
    points += QPointF(-13.151329445248951, 67.6232425750484);
    points += QPointF(-13.149174973239838, 54.47473007846592);
    points += QPointF(-10.943375405772395, 41.194056317965945);
    points += QPointF(-7.507583651313517, 31.220471153284027);
    points += QPointF(-5.523329117990919, 26.502419601905103);
    points += QPointF(6.735602489575015, 2.204598425196849);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsCase3() const
{
    QVector<VSAPoint> points;

//    p = VSAPoint(25.837720078200917, 50.514253279672296);
//    p.SetSABefore(37.81114840454936);
//    p.SetSAAfter(37.81114840454936);
//    points.append(p);

    points.append(VSAPoint(30.0, 39.999874015748034));
    points.append(VSAPoint(407.9527559055118, 39.999874015748034));
    points.append(VSAPoint(407.9527559055118, 228.97625196850396));
    points.append(VSAPoint(407.9527559055118, 228.97625196850396));

    VSAPoint p = VSAPoint(404.6723012992244, 235.65623431697782);
    p.SetSABefore(36.977845511186636);
    p.SetSAAfter(36.977845511186636);
    points.append(p);

    p = VSAPoint(397.22283300301126, 246.53269098046195);
    p.SetSABefore(35.52982410231174);
    p.SetSAAfter(35.52982410231174);
    points.append(p);

    p = VSAPoint(388.92062499630856, 254.5956376373514);
    p.SetSABefore(34.258630321759945);
    p.SetSAAfter(34.258630321759945);
    points.append(p);

    p = VSAPoint(380.0029341567567, 260.0302961027146);
    p.SetSABefore(33.111548444754845);
    p.SetSAAfter(33.111548444754845);
    points.append(p);

    p = VSAPoint(370.70701736199624, 263.02188819161995);
    p.SetSABefore(32.03891394371319);
    p.SetSAAfter(32.03891394371319);
    points.append(p);

    p = VSAPoint(361.27013148966773, 263.75563571913585);
    p.SetSABefore(30.99923856743404);
    p.SetSAAfter(30.99923856743404);
    points.append(p);

    p = VSAPoint(351.9295334174118, 262.41676050033084);
    p.SetSABefore(29.962781670363757);
    p.SetSAAfter(29.962781670363757);
    points.append(p);

    p = VSAPoint(342.92248002286897, 259.19048435027327);
    p.SetSABefore(28.91189499226486);
    p.SetSAAfter(28.91189499226486);
    points.append(p);

    p = VSAPoint(334.48622818367966, 254.26202908403155);
    p.SetSABefore(27.838718909311048);
    p.SetSAAfter(27.838718909311048);
    points.append(p);

    p = VSAPoint(326.8580347774845, 247.81661651667417);
    p.SetSABefore(26.741788760884347);
    p.SetSAAfter(26.741788760884347);
    points.append(p);

    p = VSAPoint(320.27515668192393, 240.0394684632695);
    p.SetSABefore(25.622615764841164);
    p.SetSAAfter(25.622615764841164);
    points.append(p);

    p = VSAPoint(314.97485077463864, 231.11580673888602);
    p.SetSABefore(24.482580077336745);
    p.SetSAAfter(24.482580077336745);
    points.append(p);

    p = VSAPoint(311.19437393326905, 221.2308531585922);
    p.SetSABefore(23.320121181910643);
    p.SetSAAfter(23.320121181910643);
    points.append(p);

    p = VSAPoint(309.1709830354557, 210.56982953745637);
    p.SetSABefore(22.12821102042642);
    p.SetSAAfter(22.12821102042642);
    points.append(p);

    p = VSAPoint(309.14193495883904, 199.31795769054702);
    p.SetSABefore(20.89230212706225);
    p.SetSAAfter(20.89230212706225);
    points.append(p);

    p = VSAPoint(311.34448658105987, 187.66045943293258);
    p.SetSABefore(19.58918900085386);
    p.SetSAAfter(19.58918900085386);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSABefore(18.897637795275593);
    p.SetSAAfter(18.897637795275593);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSABefore(18.897637795275593);
    p.SetSAAfter(18.897637795275593);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSABefore(18.897637795275593);
    p.SetSAAfter(18.897637795275593);
    points.append(p);

    p = VSAPoint(316.591345021592, 173.28443556543095);
    p.SetSABefore(19.976796375788812);
    p.SetSAAfter(19.976796375788812);
    points.append(p);

    p = VSAPoint(320.1426384331089, 158.63568876958243);
    p.SetSABefore(21.782587487730186);
    p.SetSAAfter(21.782587487730186);
    points.append(p);

    p = VSAPoint(320.76512297484294, 146.51286504020334);
    p.SetSABefore(23.23684508842329);
    p.SetSAAfter(23.23684508842329);
    points.append(p);

    p = VSAPoint(318.80758360045513, 136.72694951703215);
    p.SetSABefore(24.432447936360937);
    p.SetSAAfter(24.432447936360937);
    points.append(p);

    p = VSAPoint(314.618805263607, 129.08892733980727);
    p.SetSABefore(25.476072862288163);
    p.SetSAAfter(25.476072862288163);
    points.append(p);

    p = VSAPoint(308.54757291795954, 123.40978364826717);
    p.SetSABefore(26.47203809949214);
    p.SetSAAfter(26.47203809949214);
    points.append(p);

    p = VSAPoint(300.9426715171743, 119.50050358215026);
    p.SetSABefore(27.49645076170011);
    p.SetSAAfter(27.49645076170011);
    points.append(p);

    p = VSAPoint(292.1528860149125, 117.172072281195);
    p.SetSABefore(28.585809818498724);
    p.SetSAAfter(28.585809818498724);
    points.append(p);

    p = VSAPoint(282.5270013648352, 116.23547488513984);
    p.SetSABefore(29.74446055333082);
    p.SetSAAfter(29.74446055333082);
    points.append(p);

    p = VSAPoint(272.4138025206039, 116.50169653372318);
    p.SetSABefore(30.95646646201516);
    p.SetSAAfter(30.95646646201516);
    points.append(p);

    p = VSAPoint(262.16207443587984, 117.7817223666835);
    p.SetSABefore(32.19418539121919);
    p.SetSAAfter(32.19418539121919);
    points.append(p);

    p = VSAPoint(252.12060206432426, 119.88653752375922);
    p.SetSABefore(33.42332260663002);
    p.SetSAAfter(33.42332260663002);
    points.append(p);

    p = VSAPoint(238.08031279315054, 124.1327387135843);
    p.SetSABefore(35.18062492767001);
    p.SetSAAfter(35.18062492767001);
    points.append(p);

    p = VSAPoint(226.74556876528095, 129.25957033706317);
    p.SetSABefore(36.67100242816486);
    p.SetSAAfter(36.67100242816486);
    points.append(p);

    p = VSAPoint(221.03296878301194, 132.77339418798488);
    p.SetSABefore(37.47448996772893);
    p.SetSAAfter(37.47448996772893);
    points.append(p);

    points.append(VSAPoint(218.97637795275605, 134.48806299212646));
    points.append(VSAPoint(218.97637795275605, 134.48806299212646));
    points.append(VSAPoint(218.97637795275605, 134.48806299212646));

    p = VSAPoint(209.4131100947178, 142.83347127134166);
    p.SetSABefore(35.567798456628196);
    p.SetSAAfter(35.567798456628196);
    points.append(p);

    p = VSAPoint(197.1746109179469, 151.63888788459965);
    p.SetSABefore(32.921869998532244);
    p.SetSAAfter(32.921869998532244);
    points.append(p);

    p = VSAPoint(190.02702368501872, 155.74482471949955);
    p.SetSABefore(31.47527442778118);
    p.SetSAAfter(31.47527442778118);
    points.append(p);

    p = VSAPoint(183.69004004059607, 158.45053139932526);
    p.SetSABefore(30.26604125961245);
    p.SetSAAfter(30.26604125961245);
    points.append(p);

    p = VSAPoint(178.111093559349, 159.86059917182507);
    p.SetSABefore(29.256179984052537);
    p.SetSAAfter(29.256179984052537);
    points.append(p);

    p = VSAPoint(173.2376178159475, 160.07961928474754);
    p.SetSABefore(28.40004977195057);
    p.SetSAAfter(28.40004977195057);
    points.append(p);

    p = VSAPoint(169.01704638506146, 159.21218298584103);
    p.SetSABefore(27.643881968500146);
    p.SetSAAfter(27.643881968500146);
    points.append(p);

    p = VSAPoint(165.39681284136094, 157.3628815228539);
    p.SetSABefore(26.930459398621437);
    p.SetSAAfter(26.930459398621437);
    points.append(p);

    p = VSAPoint(162.32435075951588, 154.6363061435347);
    p.SetSABefore(26.20955952854878);
    p.SetSAAfter(26.20955952854878);
    points.append(p);

    p = VSAPoint(158.57569564750156, 149.20732231050295);
    p.SetSABefore(25.05174589376139);
    p.SetSAAfter(25.05174589376139);
    points.append(p);

    p = VSAPoint(155.086603150984, 139.74562302685277);
    p.SetSABefore(23.28197087076461);
    p.SetSAAfter(23.28197087076461);
    points.append(p);

    p = VSAPoint(152.8424008459478, 128.6574705373456);
    p.SetSABefore(21.29660756707718);
    p.SetSAAfter(21.29660756707718);
    points.append(p);

    p = VSAPoint(150.86615884353571, 110.7478174090613);
    p.SetSABefore(18.1344895060695);
    p.SetSAAfter(18.1344895060695);
    points.append(p);

    p = VSAPoint(149.37382105332603, 94.00159365355537);
    p.SetSABefore(15.183977229796845);
    p.SetSAAfter(15.183977229796845);
    points.append(p);

    p = VSAPoint(147.90386548781373, 84.77492816049363);
    p.SetSABefore(13.544330017337995);
    p.SetSAAfter(13.544330017337995);
    points.append(p);

    p = VSAPoint(145.57614310058185, 78.10545937151159);
    p.SetSABefore(12.304638548569304);
    p.SetSAAfter(12.304638548569304);
    points.append(p);

    p = VSAPoint(142.96466288724318, 75.41053456450908);
    p.SetSABefore(11.646068510270936);
    p.SetSAAfter(11.646068510270936);
    points.append(p);

    p = VSAPoint(140.76322838736246, 74.778132056191);
    p.SetSABefore(11.244104436407117);
    p.SetSAAfter(11.244104436407117);
    points.append(p);

    p = VSAPoint(138.11080326806675, 75.25568933876073);
    p.SetSABefore(10.771134584490436);
    p.SetSAAfter(10.771134584490436);
    points.append(p);

    p = VSAPoint(134.95482110402602, 76.94779765996671);
    p.SetSABefore(10.142692312810262);
    p.SetSAAfter(10.142692312810262);
    points.append(p);

    p = VSAPoint(129.24106098216737, 81.80753304891735);
    p.SetSABefore(8.826320717353354);
    p.SetSAAfter(8.826320717353354);
    points.append(p);

    p = VSAPoint(124.48818897637818, 87.24396850393731);
    p.SetSABefore(7.559055118110236);
    points.append(p);

    p = VSAPoint(124.48818897637818, 87.24396850393731);
    p.SetSABefore(7.559055118110236);
    points.append(p);

    p = VSAPoint(124.48818897637818, 87.24396850393731);
    p.SetSABefore(7.559055118110236);
    points.append(p);

    points.append(VSAPoint(118.34735729968762, 94.27989595731356));
    points.append(VSAPoint(106.58315141316105, 104.8247018186949));
    points.append(VSAPoint(95.47573767084238, 111.51940225476525));
    points.append(VSAPoint(85.06494183271205, 114.78883440900722));
    points.append(VSAPoint(75.39058965875054, 115.05783542490349));
    points.append(VSAPoint(66.49250690893827, 112.7512424459367));
    points.append(VSAPoint(58.410519343255714, 108.29389261558948));
    points.append(VSAPoint(51.184452721683314, 102.11062307734451));
    points.append(VSAPoint(44.85413280420151, 94.62627097468442));
    points.append(VSAPoint(39.45938535079078, 86.26567345109187));
    points.append(VSAPoint(35.04003612143157, 77.45366765004948));
    points.append(VSAPoint(31.63591087610432, 68.61509071503997));
    points.append(VSAPoint(29.286835374789483, 60.17477978954592));
    points.append(VSAPoint(28.032635377467525, 52.55757201704999));
    points.append(VSAPoint(27.913136644118886, 46.188304541034846));
    points.append(VSAPoint(28.968164934724022, 41.49181450498314));
    points.append(VSAPoint(30.0, 39.999874015748034));

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsCase3() const
{
    QVector<QPointF> points;

    points += QPointF(10.185624773347756, 2.204598425196849);
    points += QPointF(445.74803149606305, 2.204598425196849);
    points += QPointF(445.748031496063, 239.54370877454926);
    points += QPointF(434.2772499516588, 257.60252755179886);
    points += QPointF(421.70410911168364, 272.23247310841623);
    points += QPointF(406.8215280616999, 283.81580578394335);
    points += QPointF(390.4268690095888, 291.4924548133903);
    points += QPointF(373.43168069161504, 294.97206109652825);
    points += QPointF(356.88925393255363, 294.44576261629163);
    points += QPointF(341.59261122311955, 290.5110196449043);
    points += QPointF(327.9415795488323, 283.8601978892787);
    points += QPointF(316.0766275069774, 275.06080862168557);
    points += QPointF(306.0568354718494, 264.51533811547125);
    points += QPointF(297.96706395968476, 252.511650805464);
    points += QPointF(291.95795669012034, 239.28944839897594);
    points += QPointF(288.2457014751128, 225.09982730403647);
    points += QPointF(287.08291204680114, 210.25330053220583);
    points += QPointF(288.70312350739385, 195.14331998915887);
    points += QPointF(293.1661864923215, 180.51562637269953);
    points += QPointF(295.67056116540294, 175.36866212621038);
    points += QPointF(297.03278914138764, 169.78727745627393);
    points += QPointF(298.4272653364355, 158.08534773323754);
    points += QPointF(297.94225297413215, 150.9565650350354);
    points += QPointF(296.9126887137917, 147.81469899066454);
    points += QPointF(296.3972469066644, 147.0940064443774);
    points += QPointF(294.21114346945865, 146.12370067508155);
    points += QPointF(290.48115362247165, 145.59912885424706);
    points += QPointF(285.00652997672506, 145.7209025858372);
    points += QPointF(278.36163578883384, 146.69497061341855);
    points += QPointF(266.15085284299454, 149.72785233297387);
    points += QPointF(268.76683081190356, 149.29112994946044);
    points += QPointF(264.11356456471754, 150.8634687956155);
    points += QPointF(254.1976334991026, 155.20769360077045);
    points += QPointF(247.5665032053911, 159.21880811798434);
    points += QPointF(240.66667729218545, 164.69290058326322);
    points += QPointF(245.03051571796473, 161.55629083592723);
    points += QPointF(243.53306498622524, 163.14262925675627);
    points += QPointF(226.1305060641393, 173.66370879502696);
    points += QPointF(209.78346359977252, 181.5593960732169);
    points += QPointF(205.70527201989748, 183.03741399670108);
    points += QPointF(202.38658536147173, 184.69191672318945);
    points += QPointF(188.21241736536905, 188.00791613383734);
    points += QPointF(177.31502181326212, 188.81402560575697);
    points += QPointF(166.19214722698052, 187.3732642424327);
    points += QPointF(155.57437225205163, 183.1763006101058);
    points += QPointF(146.72924182900442, 176.50793105628702);
    points += QPointF(140.1685902867883, 168.24813556221721);
    points += QPointF(134.85380559162166, 156.67751165744627);
    points += QPointF(132.20863928510303, 142.1050670962771);
    points += QPointF(131.96761370902018, 132.827500480644);
    points += QPointF(129.15133677704625, 135.35186071252022);
    points += QPointF(111.11355282491606, 146.22365885487164);
    points += QPointF(91.37199519960252, 152.42334659042638);
    points += QPointF(71.0892515601446, 152.98732013057335);
    points += QPointF(52.404520155294485, 148.14379692629467);
    points += QPointF(36.77389835731295, 139.5232504514038);
    points += QPointF(24.295634363907766, 128.84573062197663);
    points += QPointF(14.420075278530305, 117.169833926539);
    points += QPointF(6.592092174103988, 105.03828991585254);
    points += QPointF(0.43109819620662115, 92.75351406859714);
    points += QPointF(-3.633892542136806, 82.1990453602945);
    points += QPointF(-4.775453591602712, 78.74896433599663);
    points += QPointF(-7.124529092917548, 70.30865341050259);
    points += QPointF(-8.006297887627618, 66.31522508588391);
    points += QPointF(-9.70470173566634, 56.00020752908148);
    points += QPointF(-9.960881193587724, 42.34587460321338);
    points += QPointF(-6.297329478268652, 26.037464411020956);
    points += QPointF(10.185624773347756, 2.204598425196849);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue627() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(11565.008125001965, -71.44488549419933);
    p.SetSAAfter(0);
    points.append(p);

    p = VSAPoint(11774.053467225081, -3376.8303371353477);
    p.SetSABefore(0);
    points.append(p);

    p = VSAPoint(11774.053467225081, -3376.8303371353477);
    p.SetSABefore(0);
    points.append(p);

    points.append(VSAPoint(11821.637653562488, -3457.444047544761));
    points.append(VSAPoint(11916.506852253828, -3619.698047174161));
    points.append(VSAPoint(12011.687139013728, -3784.3170132645946));
    points.append(VSAPoint(12107.923065894336, -3952.559914581168));
    points.append(VSAPoint(12205.959184947797, -4125.685719888987));
    points.append(VSAPoint(12306.540048226263, -4304.953397953153));
    points.append(VSAPoint(12463.260680635496, -4586.963758807588));
    points.append(VSAPoint(12688.625497168217, -4997.457976655285));
    points.append(VSAPoint(12937.571227539614, -5455.181123300274));
    points.append(VSAPoint(13363.424703096609, -6243.3010001396));

    p = VSAPoint(13704.042216387523, -6875.648082494775);
    p.SetSAAfter(0);
    points.append(p);

    p = VSAPoint(13704.042216387523, -6875.648082494775);
    p.SetSAAfter(0);
    points.append(p);

    p = VSAPoint(13493.259591532773, -71.4448854942045);
    p.SetSABefore(0);
    points.append(p);

    p = VSAPoint(13493.259591532773, -71.4448854942045);
    p.SetSABefore(0);
    points.append(p);

    points.append(VSAPoint(13227.96813484252, -78.38238188976378));
    points.append(VSAPoint(12939.963705708662, -83.80364173228347));
    points.append(VSAPoint(12664.66535433071, -86.51427165354332));
    points.append(VSAPoint(12393.602362204725, -86.51427165354332));
    points.append(VSAPoint(12118.304010826774, -83.80364173228347));
    points.append(VSAPoint(11830.299581692912, -78.38238188976378));

    p = VSAPoint(11565.008125001965, -71.44488549419933);
    p.SetSAAfter(0);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue627() const
{
    QVector<QPointF> points;

    points += QPointF(11561.41547872419, -14.638641388671703);
    points += QPointF(11760.258895120205, -3158.7131695287526);
    points += QPointF(11781.963967565265, -3501.9096738073645);
    points += QPointF(11867.496139886349, -3648.1946350753437);
    points += QPointF(11962.541506347354, -3812.580249902983);
    points += QPointF(12058.65110785104, -3980.602304924286);
    points += QPointF(12156.571541308, -4153.523816526403);
    points += QPointF(12257.041160881332, -4332.593222274458);
    points += QPointF(12413.634876614758, -4614.375203387231);
    points += QPointF(12638.875580780275, -5024.643354851167);
    points += QPointF(12887.730780405262, -5482.200047474014);
    points += QPointF(13313.529827372417, -6270.219194236017);
    points += QPointF(13707.971545534987, -7002.489438444618);
    points += QPointF(13491.504163312642, -14.778496052733317);
    points += QPointF(13226.693612049832, -21.703416545128476);
    points += QPointF(12939.151113441983, -27.11598122480561);
    points += QPointF(12664.386257521342, -29.82135826771654);
    points += QPointF(12393.881459014092, -29.82135826771654);
    points += QPointF(12119.116603093453, -27.11598122480561);
    points += QPointF(11831.5741044856, -21.70341654512845);
    points += QPointF(11561.41547872419, -14.638641388671703);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue767_Fabric_TopCollar() const
{
    QVector<VSAPoint> points;

    //    VSAPoint p = VSAPoint(-2845.8986453348375, -956.881848270395);
    //    p.SetSAAfter(43.46456692913386);
    //    points.append(p);

    points.append(VSAPoint(-2956.503052984476, -931.8887725360864));

    VSAPoint p = VSAPoint(-2845.8999353737872, -956.8546710275792);
    p.SetSAAfter(43.46456692913386);
    points.append(p);

    p = VSAPoint(-2845.8999353737872, -956.8546710275792);
    p.SetSAAfter(43.46456692913386);
    points.append(p);

    p = VSAPoint(-2836.2633663612596, -967.4426562856014);
    p.SetSABefore(43.975870702724364);
    p.SetSAAfter(43.975870702724364);
    points.append(p);

    p = VSAPoint(-2816.781399798798, -990.3120607327689);
    p.SetSABefore(45.04880285928504);
    p.SetSAAfter(45.04880285928504);
    points.append(p);

    p = VSAPoint(-2797.0552023132714, -1015.1078931004015);
    p.SetSABefore(46.18040144375942);
    p.SetSAAfter(46.18040144375942);
    points.append(p);

    p = VSAPoint(-2777.0918442622597, -1041.7127169475177);
    p.SetSABefore(47.36830653518281);
    p.SetSAAfter(47.36830653518281);
    points.append(p);

    p = VSAPoint(-2746.7450331166083, -1084.5654944804478);
    p.SetSABefore(49.243630059870085);
    p.SetSAAfter(49.243630059870085);
    points.append(p);

    p = VSAPoint(-2705.4801979126455, -1147.2198537598051);
    p.SetSABefore(51.9229573891882);
    p.SetSAAfter(51.9229573891882);
    points.append(p);

    p = VSAPoint(-2690.741587315989, -1170.7115882155053);
    p.SetSABefore(52.91338582677166);
    p.SetSAAfter(52.91338582677166);
    points.append(p);

    p = VSAPoint(-2690.741587315989, -1170.7115882155053);
    p.SetSABefore(52.91338582677166);
    p.SetSAAfter(52.91338582677166);
    points.append(p);

    p = VSAPoint(-2690.741587315989, -1170.7115882155053);
    p.SetSABefore(52.91338582677166);
    p.SetSAAfter(52.91338582677166);
    points.append(p);

    p = VSAPoint(-2663.393986883198, -1214.996323019432);
    p.SetSABefore(53.464106209918555);
    p.SetSAAfter(53.464106209918555);
    points.append(p);

    p = VSAPoint(-2620.5429628889106, -1286.9554107314775);
    p.SetSABefore(54.35027638570753);
    p.SetSAAfter(54.35027638570753);
    points.append(p);

    p = VSAPoint(-2555.034059572721, -1400.452077976559);
    p.SetSABefore(55.73686065985781);
    p.SetSAAfter(55.73686065985781);
    points.append(p);

    p = VSAPoint(-2510.4169214986496, -1479.023939177109);
    p.SetSABefore(56.69291338582678);
    p.SetSAAfter(56.69291338582678);
    points.append(p);

    p = VSAPoint(-2510.4169214986496, -1479.023939177109);
    p.SetSABefore(56.69291338582678);
    p.SetSAAfter(56.69291338582678);
    points.append(p);

    p = VSAPoint(-2510.41692149865, -1479.0239391771088);
    p.SetSABefore(56.69291338582678);
    p.SetSAAfter(56.69291338582678);
    points.append(p);

    p = VSAPoint(-2465.770203618369, -1557.5789960796274);
    p.SetSABefore(55.73686065985781);
    p.SetSAAfter(55.73686065985781);
    points.append(p);

    p = VSAPoint(-2401.820804615958, -1671.9616182591394);
    p.SetSABefore(54.35027638570753);
    p.SetSAAfter(54.35027638570753);
    points.append(p);

    p = VSAPoint(-2361.950025594169, -1745.613784160113);
    p.SetSABefore(53.464106209918555);
    p.SetSAAfter(53.464106209918555);
    points.append(p);

    p = VSAPoint(-2337.912872786133, -1791.7791853927329);
    p.SetSABefore(52.91338582677166);
    p.SetSAAfter(52.91338582677166);
    points.append(p);

    p = VSAPoint(-2337.912872786133, -1791.7791853927329);
    p.SetSABefore(52.91338582677166);
    p.SetSAAfter(52.91338582677166);
    points.append(p);

    p = VSAPoint(-2337.912872786133, -1791.7791853927329);
    p.SetSABefore(52.91338582677166);
    p.SetSAAfter(52.91338582677166);
    points.append(p);

    p = VSAPoint(-2325.280421100769, -1816.4674294468161);
    p.SetSABefore(51.92295738918819);
    p.SetSAAfter(51.92295738918819);
    points.append(p);

    p = VSAPoint(-2292.590031111732, -1883.9929339875566);
    p.SetSABefore(49.24363005987009);
    p.SetSAAfter(49.24363005987009);
    points.append(p);

    p = VSAPoint(-2271.318537325938, -1932.0014024188404);
    p.SetSABefore(47.36830653518281);
    p.SetSAAfter(47.36830653518281);
    points.append(p);

    p = VSAPoint(-2258.687165687168, -1962.7715302562779);
    p.SetSABefore(46.18040144375943);
    p.SetSAAfter(46.18040144375943);
    points.append(p);

    p = VSAPoint(-2247.488245160975, -1992.4117115831486);
    p.SetSABefore(45.048802859285026);
    p.SetSAAfter(45.048802859285026);
    points.append(p);

    p = VSAPoint(-2237.8190307443556, -2020.8557438829903);
    p.SetSABefore(43.97587070272435);
    p.SetSAAfter(43.97587070272435);
    points.append(p);

    p = VSAPoint(-2233.6582717055753, -2034.5545386613094);
    p.SetSABefore(43.46456692913386);
    points.append(p);

    p = VSAPoint(-2233.6582717055753, -2034.5545386613094);
    p.SetSABefore(43.46456692913386);
    points.append(p);

    points.append(VSAPoint(-2268.843635583417, -2142.3429233856164));
    points.append(VSAPoint(-2142.3122611221115, -2153.915659267329));
    points.append(VSAPoint(-2102.05157470339, -2047.9183817466978));
    points.append(VSAPoint(-1984.1456543967456, -1869.9284917765406));

    p = VSAPoint(-1957.540323165074, -1829.765289335082);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2074.675746476508, -1711.446016505495);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2074.675746476508, -1711.446016505495);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2104.9501005136362, -1680.2660923950884);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2157.060300446018, -1624.0358190720049);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2193.427447844697, -1582.9264609829493);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2229.9021223993304, -1539.6063550266792);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2265.283071500467, -1494.9101438927273);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2298.369042538657, -1449.6724702706276);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2320.8174013740727, -1415.9196938584569);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2334.512979024599, -1393.6772377027328);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2340.8464205786395, -1382.6908795653735);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2340.8464205786395, -1382.690879565374);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2340.8464205786395, -1382.690879565374);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2347.0406240734856, -1371.625420238929);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2359.1340239739197, -1348.4727663549988);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2376.632959709957, -1311.9082209860264);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2398.5505312977116, -1260.3257758135142);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2418.8279356815838, -1207.049235756178);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2437.3632941454625, -1153.5377981886413);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2454.0547279732336, -1101.2506604855253);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2475.6739880089963, -1027.6984737710059);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2486.9558756792776, -985.7289230170218);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2486.9558756792776, -985.7289230170218);
    p.SetSABefore(22.67716535433071);
    p.SetSAAfter(22.67716535433071);
    points.append(p);

    p = VSAPoint(-2528.6142143955344, -824.5310925952923);
    p.SetSABefore(22.67716535433071);
    points.append(p);

    points.append(VSAPoint(-2576.735791027546, -826.8202720880508));
    points.append(VSAPoint(-2789.994527521454, -836.9651369943699));
    points.append(VSAPoint(-2901.6577688078787, -817.2758948021979));

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue767_Fabric_TopCollar() const
{
    QVector<QPointF> points;

    points += QPointF(-3011.0517548790103, -958.3219444006113);
    points += QPointF(-2875.704655596311, -988.873180340486);
    points += QPointF(-2868.785856897648, -997.0427375924612);
    points += QPointF(-2869.7392507631107, -995.9600609286653);
    points += QPointF(-2852.5415024022873, -1017.6725348741263);
    points += QPointF(-2834.5617365102394, -1042.0076837341114);
    points += QPointF(-2816.318152801347, -1068.2197024431434);
    points += QPointF(-2788.3520303679693, -1110.8598835568037);
    points += QPointF(-2748.84326164774, -1175.7792341372995);
    points += QPointF(-2749.463357194371, -1174.8146939231094);
    points += QPointF(-2735.5637236883463, -1198.8327989055008);
    points += QPointF(-2709.3979303070278, -1242.234412525294);
    points += QPointF(-2667.7155430266657, -1313.9462711856888);
    points += QPointF(-2603.3069903890537, -1428.3146296654857);
    points += QPointF(-2559.7159427255197, -1507.0184553230588);
    points += QPointF(-2514.227494695693, -1585.1196674161204);
    points += QPointF(-2449.1583954294993, -1698.6620182726447);
    points += QPointF(-2408.901620525935, -1771.183767809588);
    points += QPointF(-2384.8455512861474, -1816.21584000147);
    points += QPointF(-2371.503784159133, -1840.1189454999956);
    points += QPointF(-2372.0148030467776, -1839.0924387985017);
    points += QPointF(-2336.479706267199, -1906.2692337682788);
    points += QPointF(-2314.171878902582, -1952.123894247254);
    points += QPointF(-2300.9973669393667, -1981.2156543505293);
    points += QPointF(-2289.299963085879, -2009.1198152343736);
    points += QPointF(-2279.45497012318, -2035.0093920383335);
    points += QPointF(-2279.8968116139968, -2033.636102295412);
    points += QPointF(-2276.4218852914523, -2043.7621246230262);
    points += QPointF(-2319.4788717575434, -2175.664782739319);
    points += QPointF(-2117.1714492551437, -2194.168101599941);
    points += QPointF(-2068.2154029909548, -2065.2779091126845);
    points += QPointF(-1952.6366140283867, -1890.801039106245);
    points += QPointF(-1917.5866708240478, -1837.8899156844163);
    points += QPointF(-2058.482716230266, -1695.5698799023787);
    points += QPointF(-2088.4965443596293, -1664.6582745816295);
    points += QPointF(-2140.249137288814, -1608.8138823251727);
    points += QPointF(-2176.2589010959327, -1568.108510006008);
    points += QPointF(-2212.3342540344497, -1525.2626688631194);
    points += QPointF(-2247.2342025287544, -1481.1740984291687);
    points += QPointF(-2279.7666296737902, -1436.6932724131166);
    points += QPointF(-2301.7153187064755, -1403.691786696855);
    points += QPointF(-2315.030637739082, -1382.066893805181);
    points += QPointF(-2321.1285274245947, -1371.4891377154586);
    points += QPointF(-2327.0923575597108, -1360.8352223500915);
    points += QPointF(-2338.84998341349, -1338.3254034298122);
    points += QPointF(-2355.95941674385, -1302.57473489304);
    points += QPointF(-2377.5102970264106, -1251.8552883456102);
    points += QPointF(-2397.5120320442174, -1199.3030377360624);
    points += QPointF(-2415.8444669892974, -1146.3774388092156);
    points += QPointF(-2432.371872429092, -1094.6041320326606);
    points += QPointF(-2453.842769979148, -1021.5566982667648);
    points += QPointF(-2465.02757138208, -979.9483164185621);
    points += QPointF(-2515.136197939205, -786.0519169140614);
    points += QPointF(-2578.5317097224242, -789.0676889577924);
    points += QPointF(-2787.581316069137, -799.0123226453655);
    points += QPointF(-2923.36118702248, -775.0706679591581);
    points += QPointF(-3011.0517548790103, -958.3219444006113);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue767_SkinFusing_TopCollar_Notch() const
{
    QVector<VSAPoint> points;

    //    VSAPoint p = VSAPoint(-2956.4986596431745, -931.902205605136);
    //    p.SetSAAfter(0.0);
    //    p.SetSABefore(0.0);
    //    points.append(p);

    VSAPoint p = VSAPoint(-2956.503052984476, -931.8887725360864);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2845.8999353737872, -956.8546710275792);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2845.8999353737872, -956.8546710275792);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2836.2633663612596, -967.4426562856014);
    p.SetSABefore(0.43513121779510383);
    p.SetSAAfter(0.43513121779510383);
    points.append(p);

    p = VSAPoint(-2816.781399798798, -990.3120607327689);
    p.SetSABefore(1.3482210481660497);
    p.SetSAAfter(1.3482210481660497);
    points.append(p);

    p = VSAPoint(-2797.0552023132714, -1015.1078931004015);
    p.SetSABefore(2.3112373518788916);
    p.SetSAAfter(2.3112373518788916);
    points.append(p);

    p = VSAPoint(-2777.0918442622597, -1041.7127169475177);
    p.SetSABefore(3.3221717821615027);
    p.SetSAAfter(3.3221717821615027);
    points.append(p);

    p = VSAPoint(-2746.7450331166083, -1084.5654944804478);
    p.SetSABefore(4.918115037824846);
    p.SetSAAfter(4.918115037824846);
    points.append(p);

    p = VSAPoint(-2705.4801979126455, -1147.2198537598051);
    p.SetSABefore(7.1982839391629785);
    p.SetSAAfter(7.1982839391629785);
    points.append(p);

    p = VSAPoint(-2663.393986883198, -1214.996323019432);
    p.SetSABefore(9.623062142943597);
    p.SetSAAfter(9.623062142943597);
    points.append(p);

    p = VSAPoint(-2620.5429628889106, -1286.9554107314775);
    p.SetSABefore(12.168538502609866);
    p.SetSAAfter(12.168538502609866);
    points.append(p);

    p = VSAPoint(-2555.034059572721, -1400.452077976559);
    p.SetSABefore(16.151427553679724);
    p.SetSAAfter(16.151427553679724);
    points.append(p);

    p = VSAPoint(-2510.4169214986496, -1479.023939177109);
    p.SetSABefore(18.897637795275593);
    p.SetSAAfter(18.897637795275593);
    points.append(p);

    p = VSAPoint(-2510.4169214986496, -1479.023939177109);
    p.SetSABefore(18.897637795275593);
    p.SetSAAfter(18.897637795275593);
    points.append(p);

    p = VSAPoint(-2510.41692149865, -1479.0239391771088);
    p.SetSABefore(18.897637795275593);
    p.SetSAAfter(18.897637795275593);
    points.append(p);

    p = VSAPoint(-2465.770203618369, -1557.5789960796274);
    p.SetSABefore(16.15142755367971);
    p.SetSAAfter(16.15142755367971);
    points.append(p);

    p = VSAPoint(-2401.820804615958, -1671.9616182591394);
    p.SetSABefore(12.168538502609852);
    p.SetSAAfter(12.168538502609852);
    points.append(p);

    p = VSAPoint(-2361.950025594169, -1745.613784160113);
    p.SetSABefore(9.62306214294358);
    p.SetSAAfter(9.62306214294358);
    points.append(p);

    p = VSAPoint(-2325.280421100769, -1816.4674294468161);
    p.SetSABefore(7.198283939162968);
    p.SetSAAfter(7.198283939162968);
    points.append(p);

    p = VSAPoint(-2292.590031111732, -1883.9929339875566);
    p.SetSABefore(4.918115037824851);
    p.SetSAAfter(4.918115037824851);
    points.append(p);

    p = VSAPoint(-2271.318537325938, -1932.0014024188404);
    p.SetSABefore(3.3221717821615044);
    p.SetSAAfter(3.3221717821615044);
    points.append(p);

    p = VSAPoint(-2258.687165687168, -1962.7715302562779);
    p.SetSABefore(2.3112373518789013);
    p.SetSAAfter(2.3112373518789013);
    points.append(p);

    p = VSAPoint(-2247.488245160975, -1992.4117115831486);
    p.SetSABefore(1.3482210481660388);
    p.SetSAAfter(1.3482210481660388);
    points.append(p);

    p = VSAPoint(-2237.8190307443556, -2020.8557438829903);
    p.SetSABefore(0.4351312177950959);
    p.SetSAAfter(0.4351312177950959);
    points.append(p);

    p = VSAPoint(-2233.6582717055753, -2034.5545386613094);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2233.6582717055753, -2034.5545386613094);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2268.843635583417, -2142.3429233856164);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2142.3122611221115, -2153.915659267329);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2102.05157470339, -2047.9183817466978);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-1957.540323165074, -1829.765289335082);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2074.675746476508, -1711.446016505495);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2074.675746476508, -1711.446016505495);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2104.9501005136362, -1680.2660923950884);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2157.060300446018, -1624.0358190720049);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2193.427447844697, -1582.9264609829493);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2229.9021223993304, -1539.6063550266792);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2265.283071500467, -1494.9101438927273);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2298.369042538657, -1449.6724702706276);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2320.8174013740727, -1415.9196938584569);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2334.512979024599, -1393.6772377027328);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2340.8464205786395, -1382.6908795653735);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2340.8464205786395, -1382.690879565374);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2340.8464205786395, -1382.690879565374);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2347.0406240734856, -1371.625420238929);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2359.1340239739197, -1348.4727663549988);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2376.632959709957, -1311.9082209860264);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2398.5505312977116, -1260.3257758135142);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2418.8279356815838, -1207.049235756178);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2437.3632941454625, -1153.5377981886413);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2454.0547279732336, -1101.2506604855253);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2475.6739880089963, -1027.6984737710059);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2486.9558756792776, -985.7289230170218);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2486.9558756792776, -985.7289230170218);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2528.6142143955344, -824.5310925952923);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2789.994527521454, -836.9651369943699);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(-2901.6577688078787, -817.2758948021979);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue767_SkinFusing_TopCollar_Notch() const
{
    QVector<QPointF> points;

    points += QPointF(-2956.503052984476, -931.8887725360864);
    points += QPointF(-2845.8999353737872, -956.8546710275792);
    points += QPointF(-2836.5851689927185, -967.7355423437981);
    points += QPointF(-2817.8474487692724, -991.136726507766);
    points += QPointF(-2798.925476794025, -1016.4643697746903);
    points += QPointF(-2779.83354016522, -1043.5867062304706);
    points += QPointF(-2750.887165791067, -1087.2141100776162);
    points += QPointF(-2711.65687579555, -1150.9111085764941);
    points += QPointF(-2671.760583609897, -1219.7424702384794);
    points += QPointF(-2631.217741006335, -1292.785732104274);
    points += QPointF(-2569.022589526747, -1408.5260882408015);
    points += QPointF(-2526.849928574273, -1488.3554445590923);
    points += QPointF(-2479.8121574890624, -1565.559731854928);
    points += QPointF(-2412.2945618796525, -1678.145787075361);
    points += QPointF(-2370.3106788529753, -1750.3703933828758);
    points += QPointF(-2331.6136749119755, -1819.8830711074306);
    points += QPointF(-2296.985964337713, -1886.194796550727);
    points += QPointF(-2274.3320398964784, -1933.3969433585041);
    points += QPointF(-2260.809956115591, -1963.683515123611);
    points += QPointF(-2248.7424468838362, -1992.9051810211831);
    points += QPointF(-2238.2310088750087, -2020.9957910056432);
    points += QPointF(-2233.6582717055753, -2034.5545386613094);
    points += QPointF(-2268.843635583417, -2142.3429233856164);
    points += QPointF(-2142.3122611221115, -2153.915659267329);
    points += QPointF(-2102.05157470339, -2047.9183817466978);
    points += QPointF(-1957.540323165074, -1829.765289335082);
    points += QPointF(-2074.675746476508, -1711.446016505495);
    points += QPointF(-2104.9501005136362, -1680.2660923950884);
    points += QPointF(-2157.060300446018, -1624.0358190720049);
    points += QPointF(-2193.427447844697, -1582.9264609829493);
    points += QPointF(-2229.9021223993304, -1539.6063550266792);
    points += QPointF(-2265.283071500467, -1494.9101438927273);
    points += QPointF(-2298.369042538657, -1449.6724702706276);
    points += QPointF(-2320.8174013740727, -1415.9196938584569);
    points += QPointF(-2334.512979024599, -1393.6772377027328);
    points += QPointF(-2340.8464205786395, -1382.6908795653735);
    points += QPointF(-2347.0406240734856, -1371.625420238929);
    points += QPointF(-2359.1340239739197, -1348.4727663549988);
    points += QPointF(-2376.632959709957, -1311.9082209860264);
    points += QPointF(-2398.5505312977116, -1260.3257758135142);
    points += QPointF(-2418.8279356815838, -1207.049235756178);
    points += QPointF(-2437.3632941454625, -1153.5377981886413);
    points += QPointF(-2454.0547279732336, -1101.2506604855253);
    points += QPointF(-2475.6739880089963, -1027.6984737710059);
    points += QPointF(-2486.9558756792776, -985.7289230170218);
    points += QPointF(-2528.6142143955344, -824.5310925952923);
    points += QPointF(-2789.994527521454, -836.9651369943699);
    points += QPointF(-2901.6577688078787, -817.2758948021979);
    points += QPointF(-2956.503052984476, -931.8887725360864);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue880_Piece_Detail() const
{
    QVector<VSAPoint> points;

    //    VSAPoint p = VSAPoint(30.0, 39.999874015748034);
    //    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    //    points.append(p);

    points += VSAPoint(239.15935115817035, 1727.8772840128854);
    points += VSAPoint(239.15935115817035, 1727.8772840128854);
    points += VSAPoint(229.26906627866828, 1672.003934630032);
    points += VSAPoint(210.32230605803002, 1561.0565494697453);
    points += VSAPoint(192.34933775540307, 1451.038491596672);
    points += VSAPoint(175.3204063396455, 1341.9165954116006);
    points += VSAPoint(151.37328308463964, 1179.739848589476);
    points += VSAPoint(122.62258120063842, 966.4750391568086);
    points += VSAPoint(97.11243630356083, 756.1978956903439);
    points += VSAPoint(74.60480814427176, 548.64309339639);
    points += VSAPoint(54.861656473636025, 343.5453074812554);
    points += VSAPoint(37.64494104251851, 140.6392131512489);

    VSAPoint p = VSAPoint(30.0, 39.999874015748034);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points.append(p);

    points.append(VSAPoint(45.17616605845943, 17.610441101076585));
    points.append(VSAPoint(75.44740737398081, -23.822072592206776));
    points.append(VSAPoint(105.6438731200462, -61.349018284438976));
    points.append(VSAPoint(135.79060039912326, -95.09267882253144));
    points.append(VSAPoint(165.91262631367965, -125.1753370533956));
    points.append(VSAPoint(196.03498796618314, -151.71927582394284));
    points.append(VSAPoint(226.1827224591014, -174.84677798108459));
    points.append(VSAPoint(256.3808668949021, -194.68012637173229));
    points.append(VSAPoint(286.65445837605307, -211.3416038427973));
    points.append(VSAPoint(317.0285340050218, -224.9534932411911));
    points.append(VSAPoint(347.52813088427615, -235.63807741382516));
    points.append(VSAPoint(378.17828611628374, -243.51763920761078));
    points.append(VSAPoint(409.00403680351224, -248.71446146945948));
    points.append(VSAPoint(440.0304200484296, -251.35082704628257));
    points.append(VSAPoint(471.2824729535032, -251.54901878499163));
    points.append(VSAPoint(502.78523262120086, -249.43131953249798));
    points.append(VSAPoint(518.6504625195685, -247.45337139417512));
    points.append(VSAPoint(518.6504625195685, -247.45337139417512));
    points.append(VSAPoint(518.6504625195685, -247.45337139417512));
    points.append(VSAPoint(530.2610383503521, -245.74288092468555));
    points.append(VSAPoint(552.1872278041676, -241.18301269150965));
    points.append(VSAPoint(572.6437092641386, -235.3144676119963));
    points.append(VSAPoint(591.7240901252175, -228.20167669358835));
    points.append(VSAPoint(609.5219777823567, -219.9090709437288));
    points.append(VSAPoint(626.130979630509, -210.50108136986046));
    points.append(VSAPoint(641.6447030646272, -200.04213897942634));
    points.append(VSAPoint(656.1567554796634, -188.59667477986926));
    points.append(VSAPoint(669.7607442705705, -176.2291197786322));
    points.append(VSAPoint(682.5502768323008, -163.00390498315804));
    points.append(VSAPoint(694.6189605598073, -148.98546140088973));
    points.append(VSAPoint(706.060402848042, -134.2382200392702));
    points.append(VSAPoint(722.300407627205, -110.96277002193168));
    points.append(VSAPoint(742.5431323074765, -77.86867271190005));
    points.append(VSAPoint(771.5587695889417, -25.219950033288583));
    points.append(VSAPoint(801.3225470991345, 29.88442488061677));
    points.append(VSAPoint(822.7622961453703, 66.90756576711863));
    points.append(VSAPoint(846.3734491821488, 103.62434739887975));
    points.append(VSAPoint(866.0660930267316, 130.61304884599105));
    points.append(VSAPoint(880.202278000172, 148.25843766499418));
    points.append(VSAPoint(895.3025472485346, 165.537297136819));
    points.append(VSAPoint(911.4605081667719, 182.38519625402265));
    points.append(VSAPoint(928.7697681498366, 198.73770400916214));
    points.append(VSAPoint(947.3239345926813, 214.5303893947946));

    p = VSAPoint(957.1548316121191, 222.1693111391918);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points.append(p);

    p = VSAPoint(957.1548316121191, 222.1693111391918);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points.append(p);

    p = VSAPoint(957.1548316121191, 222.1693111391918);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points.append(p);

    points.append(VSAPoint(952.3136401198107, 240.982134772638));
    points.append(VSAPoint(940.9881447982464, 288.3639167749205));
    points.append(VSAPoint(928.1711209417526, 346.5350481212475));
    points.append(VSAPoint(914.3961310718933, 414.47551510225964));
    points.append(VSAPoint(900.1967377102318, 491.1653040085978));
    points.append(VSAPoint(886.1065033783323, 575.5844011309023));
    points.append(VSAPoint(872.6589905977585, 666.712792759814));
    points.append(VSAPoint(860.3877618900742, 763.5304651859733));
    points.append(VSAPoint(849.8263797768434, 865.017404700021));
    points.append(VSAPoint(841.5084067796297, 970.1535975925977));
    points.append(VSAPoint(837.1624212037573, 1050.8452199630021));
    points.append(VSAPoint(835.1862064061293, 1105.1939935906626));
    points.append(VSAPoint(834.0709728058853, 1159.8813228514407));
    points.append(VSAPoint(833.8834157182207, 1214.7797060316668));
    points.append(VSAPoint(834.6902304583309, 1269.7616414176705));
    points.append(VSAPoint(836.5581123414115, 1324.699627295782));
    points.append(VSAPoint(839.5537566826579, 1379.4661619523315));
    points.append(VSAPoint(843.7438587972656, 1433.933743673649));
    points.append(VSAPoint(849.1951140004301, 1487.9748707460644));
    points.append(VSAPoint(855.9742176073469, 1541.462041455908));
    points.append(VSAPoint(864.1478649332113, 1594.2677540895102));
    points.append(VSAPoint(873.782751293219, 1646.2645069332007));
    points.append(VSAPoint(879.2340548139807, 1671.8780036332896));
    points.append(VSAPoint(879.2340548139807, 1671.8780036332896));

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue880_Piece_Detail() const
{
    QVector<QPointF> points;

    points += QPointF(207.97524435031346, 1768.5451872439698);
    points += QPointF(192.0523574292865, 1678.5917597177247);
    points += QPointF(173.06637823139368, 1567.4188366954345);
    points += QPointF(155.04852197223198, 1457.1320947077754);
    points += QPointF(137.95319893013118, 1347.5909857555691);
    points += QPointF(113.94865296170948, 1185.0253559450243);
    points += QPointF(85.13262246004243, 971.2759567707533);
    points += QPointF(59.56333814296166, 760.5113339757822);
    points += QPointF(37.00526380453169, 552.491340651304);
    points += QPointF(17.2197909125064, 346.95390918468786);
    points += QPointF(-0.029116486330288183, 143.66841923390743);
    points += QPointF(-12.288070724611142, -17.710595485938104);
    points += QPointF(26.31665967166907, -20.64315536140112);
    points += QPointF(45.449182757960564, -46.830007644639885);
    points += QPointF(76.80902542176231, -85.80275109276256);
    points += QPointF(108.32184223768654, -121.07549507561193);
    points += QPointF(140.03960977826148, -152.7518093937674);
    points += QPointF(172.0109643794434, -180.92508772882493);
    points += QPointF(204.27687396752606, -205.6775245587238);
    points += QPointF(236.86536116641804, -227.0807873159393);
    points += QPointF(269.7862380601783, -245.1992338515816);
    points += QPointF(303.02747320804565, -260.09601694470166);
    points += QPointF(336.55504181349585, -271.8413567367945);
    points += QPointF(370.3174391861959, -280.5210157594255);
    points += QPointF(404.25444394855236, -286.242355156443);
    points += QPointF(438.30793239230167, -289.13593911030426);
    points += QPointF(472.43167053063326, -289.3523422699121);
    points += QPointF(506.39354740949744, -287.06933462111243);
    points += QPointF(523.3262726926384, -284.95829981953244);
    points += QPointF(524.1590713325393, -284.84505611031716);
    points += QPointF(535.7696471633229, -283.1345656408276);
    points += QPointF(537.9564634081299, -282.7464387285754);
    points += QPointF(561.2586117828608, -277.9004199361857);
    points += QPointF(584.4735176539314, -271.2405394534338);
    points += QPointF(606.3291152578626, -263.09320228624324);
    points += QPointF(626.8443757152221, -253.53448553817316);
    points += QPointF(646.0388521585816, -242.66198182567126);
    points += QPointF(663.9421664899236, -230.592040397069);
    points += QPointF(680.6014176242785, -217.45310890781045);
    points += QPointF(696.0845837791157, -203.37717044914066);
    points += QPointF(710.4796074150597, -188.49177267151234);
    points += QPointF(723.8903257348999, -172.9144816918925);
    points += QPointF(736.5088929791715, -156.65000627864362);
    points += QPointF(753.9481523879648, -131.65576420615508);
    points += QPointF(775.2314496504125, -96.86047247508529);
    points += QPointF(804.6599580426928, -43.46259731691744);
    points += QPointF(834.3104997062314, 11.42928555575623);
    points += QPointF(855.028099173095, 47.20538546427957);
    points += QPointF(877.5611048277024, 82.24558222378279);
    points += QPointF(896.0956388425119, 107.64709808599238);
    points += QPointF(909.1935793406782, 123.99650588254744);
    points += QPointF(923.186035221142, 140.00772229515948);
    points += QPointF(938.0932990305748, 155.55152005811772);
    points += QPointF(954.0130809543497, 170.5913517795987);
    points += QPointF(967.4782824078412, 182.05248051659785);
    points += QPointF(1004.0810458391544, 191.4716400415735);
    points += QPointF(988.9976255298626, 250.08566646847305);
    points += QPointF(977.8259070734451, 296.82410160649124);
    points += QPointF(965.149517480306, 354.35695262370325);
    points += QPointF(951.5014069310696, 421.6716303754821);
    points += QPointF(937.4212488890727, 497.7174388481253);
    points += QPointF(923.444794947317, 581.4548412888578);
    points += QPointF(910.1057337999374, 671.8482997696264);
    points += QPointF(897.9362480985484, 767.8632391288859);
    points += QPointF(887.4670427019098, 868.4644297171681);
    points += QPointF(879.2234261603584, 972.6607837009217);
    points += QPointF(874.9207445088272, 1052.5483850157796);
    points += QPointF(872.9674776789623, 1106.266056241682);
    points += QPointF(871.8649319396666, 1160.3312151973862);
    points += QPointF(871.6796386709575, 1214.5669734562434);
    points += QPointF(872.4760820545193, 1268.8421328907752);
    points += QPointF(874.3182959750076, 1323.025174797752);
    points += QPointF(877.269773413631, 1376.984247710642);
    points += QPointF(881.3933583565541, 1430.5871656635104);
    points += QPointF(886.7511179913885, 1483.701422556393);
    points += QPointF(893.4041934667935, 1536.1942305045375);
    points += QPointF(901.4126286647522, 1587.9325929330282);
    points += QPointF(910.8542351538612, 1638.8862690551816);
    points += QPointF(925.0965474653889, 1705.8052028484663);
    points += QPointF(207.97524435031346, 1768.5451872439698);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue880_Piece_Detail_1() const
{
    QVector<VSAPoint> points;

    //    VSAPoint p = VSAPoint(124.48818897637796, 39.999874015748034);
    //    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    //    points.append(p);

    points += VSAPoint(239.15935115817035, 1727.8772840128854);
    points += VSAPoint(239.15935115817035, 1727.8772840128854);
    points += VSAPoint(228.38870291910868, 1665.145749022502);
    points += VSAPoint(208.68143263818018, 1537.8520195183164);
    points += VSAPoint(191.107067165158, 1409.242825317507);
    points += VSAPoint(175.590132468123, 1280.2974183439387);
    points += VSAPoint(162.05515451515572, 1151.9950505214774);
    points += VSAPoint(150.42665927433694, 1025.314973773988);
    points += VSAPoint(140.62917271374727, 901.2364400253355);
    points += VSAPoint(132.58722080146742, 780.7387011993851);
    points += VSAPoint(126.22532950557809, 664.801009220002);
    points += VSAPoint(121.46802479415996, 554.4026160110516);
    points += VSAPoint(118.23983263529368, 450.5227734963987);
    points += VSAPoint(116.46527899705998, 354.1407335999088);
    points += VSAPoint(116.06888984753957, 266.235748245447);
    points += VSAPoint(116.97519115481305, 187.78706935687848);
    points += VSAPoint(119.10870888696115, 119.77394885806838);
    points += VSAPoint(122.39396901206462, 63.17563867288196);

    VSAPoint p = VSAPoint(124.48818897637796, 39.999874015748034);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points.append(p);

    p = VSAPoint(124.48818897637796, 39.999874015748034);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points.append(p);

    p = VSAPoint(124.48818897637796, 39.999874015748034);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points.append(p);

    points += VSAPoint(131.17745572117823, 59.19926080571054);
    points += VSAPoint(145.42358161389507, 95.6473363798392);
    points += VSAPoint(160.66298139295492, 129.7789066433697);
    points += VSAPoint(176.84250283902833, 161.62100748114375);
    points += VSAPoint(193.90899373278594, 191.2006747780029);
    points += VSAPoint(211.80930185489825, 218.54494441878893);
    points += VSAPoint(230.49027498603584, 243.68085228834343);
    points += VSAPoint(249.8987609068692, 266.6354342715081);
    points += VSAPoint(269.98160739806895, 287.4357262531245);
    points += VSAPoint(290.68566224030565, 306.1087641180344);
    points += VSAPoint(311.95777321424987, 322.6815837510794);
    points += VSAPoint(333.7447881005721, 337.1812210371011);
    points += VSAPoint(355.99355467994303, 349.6347118609412);
    points += VSAPoint(378.650920733033, 360.06909210744146);
    points += VSAPoint(401.66373404051285, 368.5113976614434);
    points += VSAPoint(424.9788423830528, 374.98866440778875);
    points += VSAPoint(448.5430935413237, 379.52792823131904);
    points += VSAPoint(472.3033352959959, 382.1562250168761);
    points += VSAPoint(496.2064154277402, 382.9005906493014);
    points += VSAPoint(520.1991817172269, 381.7880610134368);
    points += VSAPoint(544.2284819451269, 378.8456719941238);
    points += VSAPoint(568.2411638921103, 374.10045947620404);
    points += VSAPoint(592.1840753388481, 367.57945934451936);
    points += VSAPoint(616.0040640660104, 359.3097074839112);
    points += VSAPoint(639.647977854268, 349.31823977922147);
    points += VSAPoint(663.0626644842916, 337.6320921152916);
    points += VSAPoint(686.1949717367515, 324.27830037696316);
    points += VSAPoint(708.9917473923185, 309.2839004490781);
    points += VSAPoint(731.399839231663, 292.67592821647787);
    points += VSAPoint(753.3660950354556, 274.48141956400417);
    points += VSAPoint(774.8373625843669, 254.72741037649877);
    points += VSAPoint(795.7604896590673, 233.4409365388032);

    p = VSAPoint(805.9737292499144, 222.1693111391918);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points.append(p);

    p = VSAPoint(805.9737292499144, 222.1693111391918);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points.append(p);

    p = VSAPoint(805.9737292499144, 222.1693111391918);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points.append(p);

    points += VSAPoint(801.1024217701829, 252.56710912274912);
    points += VSAPoint(783.4393334619424, 355.5003208404381);
    points += VSAPoint(766.2212290655575, 462.42427627224754);
    points += VSAPoint(754.024845322645, 544.4163414513537);
    points += VSAPoint(741.7977429994414, 634.4390974603281);
    points += VSAPoint(730.0853694039338, 731.420313160728);
    points += VSAPoint(719.4331718441081, 834.2877574141107);
    points += VSAPoint(710.3865976279511, 941.9691990820332);
    points += VSAPoint(703.4910940634496, 1053.3924070260528);
    points += VSAPoint(700.1562772589527, 1138.828625614281);
    points += VSAPoint(698.833185316681, 1196.3413376010737);
    points += VSAPoint(698.3546751480656, 1254.1863611415165);
    points += VSAPoint(698.7889276666046, 1312.2296673433043);
    points += VSAPoint(700.2041237857968, 1370.3372273141313);
    points += VSAPoint(702.6684444191402, 1428.3750121616927);
    points += VSAPoint(706.2500704801332, 1486.2089929936824);
    points += VSAPoint(711.0171828822739, 1543.7051409177961);
    points += VSAPoint(717.037962539061, 1600.7294270417278);
    points += VSAPoint(724.3805903639925, 1657.1478224731723);
    points += VSAPoint(728.6282421890804, 1685.0543048990771);
    points += VSAPoint(728.6282421890804, 1685.0543048990771);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue880_Piece_Detail_1() const
{
    QVector<QPointF> points;

    points += QPointF(176.43300321313995, 1809.2444231688892);
    points += QPointF(153.78313541824667, 1677.3246962809667);
    points += QPointF(133.87793658475465, 1548.7525013540196);
    points += QPointF(116.13042064136965, 1418.8761921325834);
    points += QPointF(100.47483551040597, 1288.7786029156107);
    points += QPointF(86.82794509892291, 1159.4153812471877);
    points += QPointF(75.10856636006955, 1031.7452255369717);
    points += QPointF(65.2370069141379, 906.7286088476662);
    points += QPointF(57.13470030186313, 785.3265367259893);
    points += QPointF(50.723988203508114, 668.4991453010994);
    points += QPointF(45.92803421919151, 557.203853977457);
    points += QPointF(42.670896438097856, 452.3925716567163);
    points += QPointF(40.87786536674168, 355.006959561548);
    points += QPointF(40.47636977532461, 265.9695473970137);
    points += QPointF(41.39833199038787, 186.1652735021885);
    points += QPointF(43.58687052785413, 116.39817805128028);
    points += QPointF(47.000830659421496, 57.582630079953006);
    points += QPointF(65.68957312272546, -149.23704714976648);
    points += QPointF(140.97338867071443, -142.43421359312867);
    points += QPointF(202.09509240490257, 32.99596074790383);
    points += QPointF(215.1760179357478, 66.46292333957582);
    points += QPointF(228.91247186136482, 97.22835696106226);
    points += QPointF(243.32162407629943, 125.58628332767515);
    points += QPointF(258.3193167174947, 151.58030642877873);
    points += QPointF(273.82133547133117, 175.26098152608563);
    points += QPointF(289.7450120582673, 196.68684969815953);
    points += QPointF(306.0113435768183, 215.92517884608725);
    points += QPointF(322.54758976689817, 233.05217073860985);
    points += QPointF(339.2901947112362, 248.1523672201739);
    points += QPointF(356.187735703955, 261.31701737912283);
    points += QPointF(373.20345965213, 272.6412775724043);
    points += QPointF(390.3168780918559, 282.22031642578816);
    points += QPointF(407.5239155726528, 290.1446608341238);
    points += QPointF(424.8352834117022, 296.495377493676);
    points += QPointF(442.27307072936014, 301.3398413607568);
    points += QPointF(459.86592925304035, 304.7288150104031);
    points += QPointF(477.6435530680066, 306.6953299983464);
    points += QPointF(495.63130278250605, 307.2554863722674);
    points += QPointF(513.8457455201286, 306.4108940021393);
    points += QPointF(532.2916184779821, 304.1521959323631);
    points += QPointF(550.9603800691451, 300.4630103015354);
    points += QPointF(569.8302035956763, 295.3236970657534);
    points += QPointF(588.8670691430791, 288.71453548788656);
    points += QPointF(608.0265381189225, 280.6181087615205);
    points += QPointF(627.2558255674835, 271.02087194401406);
    points += QPointF(646.49587350226, 259.9139991901456);
    points += QPointF(665.6832327130836, 247.2936645466005);
    points += QPointF(684.7516535483287, 233.16092151690435);
    points += QPointF(703.6333557352625, 217.52132882153552);
    points += QPointF(722.2599918458167, 200.3844392540231);
    points += QPointF(740.775593812871, 181.5472991146096);
    points += QPointF(827.8905206609868, 85.40475654403389);
    points += QPointF(902.5287659069734, 97.36568353412399);
    points += QPointF(875.7406670161695, 264.52803611283815);
    points += QPointF(875.6040535669327, 265.3514075972125);
    points += QPointF(858.0066884529067, 367.9016115788377);
    points += QPointF(840.9224284397759, 473.9943970664766);
    points += QPointF(828.8632024485165, 555.064398169016);
    points += QPointF(816.7758887510091, 644.0579522451663);
    points += QPointF(805.2076743624673, 739.845495712356);
    points += QPointF(794.6970759283694, 841.3455282095892);
    points += QPointF(785.7814316001975, 947.4685104254818);
    points += QPointF(778.9905248854037, 1057.2015565957809);
    points += QPointF(775.7129148747653, 1141.1721677964533);
    points += QPointF(774.4165452653767, 1197.5233022801074);
    points += QPointF(773.9475654146959, 1254.2162469276998);
    points += QPointF(774.3725935699175, 1311.0266010527719);
    points += QPointF(775.7556236491987, 1367.8134323501624);
    points += QPointF(778.1598154813046, 1424.435109901397);
    points += QPointF(781.6473200844314, 1480.7492730591212);
    points += QPointF(786.2790701026621, 1536.6128151220523);
    points += QPointF(792.1145299697872, 1591.8818919022567);
    points += QPointF(799.2334640707138, 1646.5814965577908);
    points += QPointF(815.4824145904547, 1753.3348442404624);
    points += QPointF(176.43300321313995, 1809.2444231688892);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::SumTrapezoids() const
{
    // Case3 checks that the method 'SumTrapezoids' returns negative value for three clockwise allocated points
    // Case4 checks that the method 'SumTrapezoids' returns positive value for three counterclock-wise allocated points
    // Case5 checks that the method 'SumTrapezoids' returns 0 for one point
    Case3();
    Case4();
    Case5();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::PathRemoveLoop_data() const
{
    QTest::addColumn<QVector<QPointF>>("path");
    QTest::addColumn<QVector<QPointF>>("expect");

    QVector<QPointF> path;
    path << QPointF(10, 10);
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 20);
    path << QPointF(10, 10);
    QTest::newRow("Correct closed a path (four unique points)") << path << path;

    path.removeLast();

    QTest::newRow("Correct unclosed a path (four unique points)") << path << path;

    path.clear();
    path << QPointF(0, 10);
    path << QPointF(10, 10);
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 20);
    path << QPointF(0, 20);
    path << QPointF(0, 10);
    QTest::newRow("Correct closed a path (six unique points)") << path << path;

    path.removeLast();

    QTest::newRow("Correct unclosed a path (six unique points)") << path << path;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 10);
    path << QPointF(10, 20);
    path << QPointF(20, 10);

    QVector<QPointF> res;
    res << QPointF(20, 10);
    res << QPointF(20, 20);
    res << QPointF(15, 15);
    res << QPointF(20, 10);
    QTest::newRow("One loop, closed a path (four unique points)") << path << res;

    path.removeLast();
    res.removeLast();

    QTest::newRow("One loop, unclosed a path (four unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 10);
    path << QPointF(0, 20);
    path << QPointF(0, 10);
    path << QPointF(10, 20);
    path << QPointF(20, 10);

    res.clear();
    res << QPointF(20, 10);
    res << QPointF(20, 20);
    res << QPointF(15, 15);
    res << QPointF(20, 10);
    QTest::newRow("Two loops, closed a path (six unique points)") << path << res;

    path.removeLast();
    res.removeLast();

    QTest::newRow("Two loops, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 10);
    path << QPointF(0, 10);
    path << QPointF(0, 20);
    path << QPointF(10, 20);
    path << QPointF(20, 10);

    res.clear();
    res << QPointF(20, 10);
    res << QPointF(20, 20);
    res << QPointF(15, 15);
    res << QPointF(20, 10);
    QTest::newRow("One loop, the first loop, closed a path (six unique points)") << path << res;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
    res.remove(res.size() - 1);
#else
    path.removeLast();
    res.removeLast();
#endif
    QTest::newRow("One loop, the first loop, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 20);
    path << QPointF(0, 10);
    path << QPointF(0, 20);
    path << QPointF(10, 10);
    path << QPointF(20, 10);

    res.clear();
    res << QPointF(20, 10);
    res << QPointF(20, 20);
    res << QPointF(10, 20);
    res << QPointF(5, 15);
    res << QPointF(10, 10);
    res << QPointF(20, 10);
    QTest::newRow("One loop, the second loop, closed a path (six unique points)") << path << res;

    path.removeLast();
    res.removeLast();

    QTest::newRow("One loop, the second loop, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 20);
    path << QPointF(20, 15);
    path << QPointF(10, 10);
    path << QPointF(20, 10);
    QTest::newRow("Correct closed a path, point on line (four unique points)") << path << path;

    path.removeLast();

    QTest::newRow("Corect unclosed a path, point on line (four unique points)") << path << path;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 15);
    path << QPointF(20, 20);
    path << QPointF(10, 20);
    path << QPointF(0, 20);
    path << QPointF(10, 15);
    path << QPointF(0, 10);
    path << QPointF(10, 10);
    path << QPointF(20, 10);

    QTest::newRow("Correct closed a path, point on line (six unique points)") << path << path;

    path.removeLast();

    QTest::newRow("Corect unclosed a path, point on line (six unique points)") << path << path;

    path.clear();
    path << QPointF(100.96979100571033, 1797.6153764073072);
    path << QPointF(168.3888427659865, 1807.2395034187866);
    path << QPointF(206.78076137364403, 1812.2910842036706);
    path << QPointF(239.1630793382262, 1815.951361623424);
    path << QPointF(267.5320085054171, 1818.4827543754482);
    path << QPointF(293.9502505847841, 1820.144031725603);
    path << QPointF(320.48133946750147, 1821.175819320443);
    path << QPointF(364.5960626489172, 1822.0507669842166);
    path << QPointF(400.66867742260206, 1822.488188976378);
    path << QPointF(623.3126833308274, 1822.488188976378);
    path << QPointF(653.5489038032683, 2162.6456692913384);
    path << QPointF(570.545584385708, 2162.6456692913384);
    path << QPointF(600.7818048581489, 1822.488188976378);
    path << QPointF(1001.3385826771654, 1822.488188976378);
    path << QPointF(1001.3385826771654, 2680.44094488189);
    path << QPointF(-22.11646613738226, 2680.44094488189);
    path << QPointF(100.96979100571033, 1797.6153764073072);

    res.clear();
    res << QPointF(100.96979100571033, 1797.6153764073072);
    res << QPointF(168.3888427659865, 1807.2395034187866);
    res << QPointF(206.78076137364403, 1812.2910842036706);
    res << QPointF(239.1630793382262, 1815.951361623424);
    res << QPointF(267.5320085054171, 1818.4827543754482);
    res << QPointF(293.9502505847841, 1820.144031725603);
    res << QPointF(320.48133946750147, 1821.175819320443);
    res << QPointF(364.5960626489172, 1822.0507669842166);
    res << QPointF(400.66867742260206, 1822.488188976378);
    res << QPointF(1001.3385826771654, 1822.488188976378);
    res << QPointF(1001.3385826771654, 2680.44094488189);
    res << QPointF(-22.11646613738226, 2680.44094488189);
    res << QPointF(100.96979100571033, 1797.6153764073072);

    // See the file "collection/bugs/Issue_#493.val"
    QTest::newRow("Test case issue #493") << path << res;

    path.clear();
    path << QPointF(-685.2149804319953, -3568.7982439212556);
    path << QPointF(-700.7415523087261, -3623.900571239949);
    path << QPointF(-675.4694480627154, -3639.3631430823175);
    path << QPointF(-684.7497934439581, -3631.3546395862268);
    path << QPointF(-683.1356602239256, -3633.2868478418427);
    path << QPointF(-686.8764821039574, -3627.927414863926);
    path << QPointF(-684.7670104817863, -3631.587853202178);
    path << QPointF(-682.2386030572435, -3636.8469922361573);
    path << QPointF(-676.4708011186385, -3650.307478525872);
    path << QPointF(-666.3050989871189, -3676.5286567894937);
    path << QPointF(-654.0449409043066, -3710.198553447806);
    path << QPointF(-640.1333287371614, -3750.0101920374505);
    path << QPointF(-617.0729873733014, -3818.3303697354913);
    path << QPointF(-583.8128392515604, -3920.9726624886944);
    path << QPointF(-550.5307668482033, -4027.6970214479597);
    path << QPointF(-527.4164674104215, -4104.7034088569535);
    path << QPointF(-513.4302533332675, -4152.73879565781);
    path << QPointF(-501.0373006826446, -4196.767296675345);
    path << QPointF(-490.59311078227046, -4235.660899517831);
    path << QPointF(-477.25724163384456, -4288.293444470835);
    path << QPointF(-405.3839593893572, -4272.013803282615);
    path << QPointF(-545.9786893428341, -3568.830152982464);
    path << QPointF(-685.2149804319953, -3568.7982439212556);

    res.clear();
    res << QPointF(-685.2149804319953, -3568.7982439212556);
    res << QPointF(-700.7415523087261, -3623.900571239949);
    res << QPointF(-683.3457668881176, -3634.5440688767967);
    res << QPointF(-682.2386030572435, -3636.8469922361573);
    res << QPointF(-676.4708011186385, -3650.307478525872);
    res << QPointF(-666.3050989871189, -3676.5286567894937);
    res << QPointF(-654.0449409043066, -3710.198553447806);
    res << QPointF(-640.1333287371614, -3750.0101920374505);
    res << QPointF(-617.0729873733014, -3818.3303697354913);
    res << QPointF(-583.8128392515604, -3920.9726624886944);
    res << QPointF(-550.5307668482033, -4027.6970214479597);
    res << QPointF(-527.4164674104215, -4104.7034088569535);
    res << QPointF(-513.4302533332675, -4152.73879565781);
    res << QPointF(-501.0373006826446, -4196.767296675345);
    res << QPointF(-490.59311078227046, -4235.660899517831);
    res << QPointF(-477.25724163384456, -4288.293444470835);
    res << QPointF(-405.3839593893572, -4272.013803282615);
    res << QPointF(-545.9786893428341, -3568.830152982464);
    res << QPointF(-685.2149804319953, -3568.7982439212556);

    // See the file "collection/bugs/Issue_#515.val"
    // Check a seam allowance path.
    // The curve that causes the issue is the first in the list.
    QTest::newRow("Test case issue #515. Big loop in seam allowance path.") << path << res;

    path.clear();
    path << QPointF(-449.6699112298347, -4243.2921010175705);
    path << QPointF(-576.966638263205, -3606.6183279948636);
    path << QPointF(-656.9465284876832, -3606.6183279948636);
    path << QPointF(-656.5996104603414, -3606.6000783462687);
    path << QPointF(-655.7439133016985, -3607.1236310612317);
    path << QPointF(-654.129780081666, -3609.0558393168476);
    path << QPointF(-651.3154902471701, -3613.939306009108);
    path << QPointF(-647.8207651830382, -3621.2084054506768);
    path << QPointF(-641.4701586077349, -3636.0289997859454);
    path << QPointF(-630.9244502073004, -3663.23035747934);
    path << QPointF(-618.4465305467888, -3697.4982896415795);
    path << QPointF(-604.3873016966293, -3737.732371148936);
    path << QPointF(-581.1891087215608, -3806.460957656939);
    path << QPointF(-547.7936207285052, -3909.520915257629);
    path << QPointF(-514.3891332445846, -4016.6378180116963);
    path << QPointF(-491.17181635142833, -4093.9874129706236);
    path << QPointF(-477.094588519539, -4142.335384784734);
    path << QPointF(-464.5941701318652, -4186.745679830414);
    path << QPointF(-454.0214632588362, -4226.117872983938);

    res.clear();
    res << QPointF(-449.6699112298347, -4243.2921010175705);
    res << QPointF(-576.966638263205, -3606.6183279948636);
    res << QPointF(-656.5697831440032, -3606.6183279948636);
    res << QPointF(-655.7439133016985, -3607.1236310612317);
    res << QPointF(-654.129780081666, -3609.0558393168476);
    res << QPointF(-651.3154902471701, -3613.939306009108);
    res << QPointF(-647.8207651830382, -3621.2084054506768);
    res << QPointF(-641.4701586077349, -3636.0289997859454);
    res << QPointF(-630.9244502073004, -3663.23035747934);
    res << QPointF(-618.4465305467888, -3697.4982896415795);
    res << QPointF(-604.3873016966293, -3737.732371148936);
    res << QPointF(-581.1891087215608, -3806.460957656939);
    res << QPointF(-547.7936207285052, -3909.520915257629);
    res << QPointF(-514.3891332445846, -4016.6378180116963);
    res << QPointF(-491.17181635142833, -4093.9874129706236);
    res << QPointF(-477.094588519539, -4142.335384784734);
    res << QPointF(-464.5941701318652, -4186.745679830414);
    res << QPointF(-454.0214632588362, -4226.117872983938);

    // See the file "collection/bugs/Issue_#515.val"
    // Check a seam allowance path.
    // The curve that causes the issue is the last in the list.
    QTest::newRow("Test case issue #515. Small loop in seam allowance path.") << path << res;

    path.clear();
    path << QPointF(1229.6503937007876, 937.6667716535435);
    path << QPointF(203.08931117793543, 937.6667716535435);
    path << QPointF(459.7677349767701, -2166.704563141019);
    path << QPointF(1229.6503937007876, -1990.077167189857);
    path << QPointF(1229.6503937007876, -555.2466141732282);
    path << QPointF(920.1053824527112, -555.2466141732282);
    path << QPointF(887.034516310979, -63.90803149606281);
    path << QPointF(816.3607592795726, -63.908031496062826);
    path << QPointF(780.7580397937137, -592.8627210002539);
    path << QPointF(816.0241340748559, -1202.917917917055);
    path << QPointF(887.3711415156957, -1202.917917917055);
    path << QPointF(920.4420076574283, -630.8371653543306);
    path << QPointF(1229.6503937007876, -630.8371653543306);
    path << QPointF(1229.6503937007876, 937.6667716535435);

    res.clear();
    res << QPointF(1229.6503937007876, 937.6667716535435);
    res << QPointF(203.08931117793543, 937.6667716535435);
    res << QPointF(459.7677349767702, -2166.704563141019);
    res << QPointF(1229.6503937007876, -1990.077167189857);
    res << QPointF(1229.6503937007876, 937.6667716535435);

    // See the file "collection/bugs/Issue_#603.val"
    // Point H1 is first in the list
    QTest::newRow("Test issue 603.") << path << res;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::PathRemoveLoop() const
{
    QFETCH(QVector<QPointF>, path);
    QFETCH(QVector<QPointF>, expect);

    QVector<QPointF> res = VAbstractPiece::CheckLoops(path);
    Comparison(res, expect);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::PathLoopsCase_data() const
{
    QTest::addColumn<QVector<QPointF>>("path");
    QTest::addColumn<QVector<QPointF>>("expect");

    QVector<QPointF> path;
    path << QPointF(61.86670866141733, 446.92270866141735);
    path << QPointF(650.6504606788366, 473.2192016666484);
    path << QPointF(649.4426552757304, 480.5376973511262);
    path << QPointF(646.5769170924987, 501.9977838630714);
    path << QPointF(644.6382908004568, 523.6358081043691);
    path << QPointF(643.4592698551749, 551.9888717674471);
    path << QPointF(642.9134698671897, 584.1776423714557);
    path << QPointF(643.1914832622404, 613.2382010061506);
    path << QPointF(644.2199668178571, 639.3780275889782);
    path << QPointF(645.9255773115714, 662.8046020373845);
    path << QPointF(648.2349715209137, 683.7254042688159);
    path << QPointF(651.0748062234152, 702.3479142007185);
    path << QPointF(654.3717381966065, 718.8796117505387);
    path << QPointF(658.0524242180187, 733.5279768357226);
    path << QPointF(662.0435210651824, 746.5004893737165);
    path << QPointF(666.2716855156286, 758.0046292819667);
    path << QPointF(670.6635743468883, 768.2478764779191);
    path << QPointF(677.400406718071, 781.7952098705392);
    path << QPointF(691.6740007010135, 806.2608114022295);
    path << QPointF(694.5877745571677, 810.2150054671212);
    path << QPointF(699.9560352035193, 816.1706553153153);
    path << QPointF(708.9007628091615, 824.0594196166176);
    path << QPointF(719.3794725391945, 831.7499791040799);
    path << QPointF(730.9568541500198, 839.0942359684872);
    path << QPointF(743.1975973980386, 845.9440924006244);
    path << QPointF(755.6663920396528, 852.1514505912763);
    path << QPointF(767.9279278312633, 857.568212731228);
    path << QPointF(779.5468945292718, 862.046281011264);
    path << QPointF(790.0879818900794, 865.4375576221694);
    path << QPointF(799.115879670088, 867.5939447547289);
    path << QPointF(804.5608128209333, 868.2650594004886);
    path << QPointF(807.5317661719646, 868.2782441618697);
    path << QPointF(809.8795601157717, 867.8994015359809);
    path << QPointF(811.5497808719051, 867.1100192966705);
    path << QPointF(812.4880146599148, 865.8915852177861);
    path << QPointF(812.6398476993509, 864.2255870731761);
    path << QPointF(811.9508662097637, 862.0935126366886);
    path << QPointF(810.3666564107034, 859.4768496821717);
    path << QPointF(806.3216663321919, 854.66911491981);
    path << QPointF(802.0871811023624, 850.6707401574804);
    path << QPointF(799.4598981526765, 850.6707401574804);
    path << QPointF(802.0871811023624, 1653.9337322834645);
    path << QPointF(61.86670866141733, 1653.9337322834645);

    QVector<QPointF> res;
    res << QPointF(61.86670866141733, 446.92270866141735);
    res << QPointF(650.6504606788366, 473.2192016666484);
    res << QPointF(649.4426552757304, 480.5376973511262);
    res << QPointF(646.5769170924987, 501.9977838630714);
    res << QPointF(644.6382908004568, 523.6358081043691);
    res << QPointF(643.4592698551749, 551.9888717674471);
    res << QPointF(642.9134698671897, 584.1776423714557);
    res << QPointF(643.1914832622404, 613.2382010061506);
    res << QPointF(644.2199668178571, 639.3780275889782);
    res << QPointF(645.9255773115714, 662.8046020373845);
    res << QPointF(648.2349715209137, 683.7254042688159);
    res << QPointF(651.0748062234152, 702.3479142007185);
    res << QPointF(654.3717381966065, 718.8796117505387);
    res << QPointF(658.0524242180187, 733.5279768357226);
    res << QPointF(662.0435210651824, 746.5004893737165);
    res << QPointF(666.2716855156286, 758.0046292819667);
    res << QPointF(670.6635743468883, 768.2478764779191);
    res << QPointF(677.400406718071, 781.7952098705392);
    res << QPointF(691.6740007010135, 806.2608114022295);
    res << QPointF(694.5877745571677, 810.2150054671212);
    res << QPointF(699.9560352035193, 816.1706553153153);
    res << QPointF(708.9007628091615, 824.0594196166176);
    res << QPointF(719.3794725391945, 831.7499791040799);
    res << QPointF(730.9568541500198, 839.0942359684872);
    res << QPointF(743.1975973980386, 845.9440924006244);
    res << QPointF(755.6663920396528, 852.1514505912763);
    res << QPointF(767.9279278312633, 857.568212731228);
    res << QPointF(779.5468945292718, 862.046281011264);
    res << QPointF(790.0879818900794, 865.4375576221694);
    res << QPointF(799.115879670088, 867.5939447547289);
    res << QPointF(799.5154110117976, 867.6431889469776);
    res << QPointF(802.0871811023624, 1653.9337322834645);
    res << QPointF(61.86670866141733, 1653.9337322834645);

    // See file "collection/bugs/Issue_#609_case1.val"
    // Clear a main path. Bound intersection. External loop. Outside a loop. Start point Ф1.
    QTest::newRow("Issue 609. Case1a") << path << res;

    path.clear();
    path << QPointF(-365.68188649000314, -2143.126579528016);
    path << QPointF(-195.75487873249062, -2116.7935769656237);
    path << QPointF(-195.75487873249062, -1836.0319480765759);
    path << QPointF(-233.39027086052477, -1838.4849618976993);
    path << QPointF(-231.15080237392075, -1855.5915146519483);
    path << QPointF(-225.84473077299972, -1889.4811404382626);
    path << QPointF(-219.39861487985402, -1922.986407729537);
    path << QPointF(-211.6695159016421, -1955.9990283342697);
    path << QPointF(-204.87723909172885, -1980.439660924953);
    path << QPointF(-199.87970909142098, -1996.6270828437923);
    path << QPointF(-194.48099536000245, -2012.6451713592935);
    path << QPointF(-188.65032933731845, -2028.5246588116781);
    path << QPointF(-182.36812965707693, -2044.2602109802488);
    path << QPointF(-175.61499879935675, -2059.8462252736344);
    path << QPointF(-168.3717693169516, -2075.2768492268588);
    path << QPointF(-160.6424572210866, -2090.5008865466684);
    path << QPointF(-150.22847685877994, -2109.7385074212525);
    path << QPointF(194.23861004296444, -2056.3576305273214);
    path << QPointF(302.4787663409577, -1301.003761061316);
    path << QPointF(279.86810151275455, -1288.330749878147);
    path << QPointF(-641.7062267185897, -2051.118466118487);
    path << QPointF(-365.68188649000314, -2143.126579528016);

    res.clear();
    res << QPointF(-365.68188649000314, -2143.126579528016);
    res << QPointF(-195.75487873249062, -2116.7935769656237);
    res << QPointF(-195.75487873249062, -2008.8655346469059);
    res << QPointF(-194.48099536000245, -2012.6451713592935);
    res << QPointF(-188.65032933731845, -2028.5246588116781);
    res << QPointF(-182.36812965707693, -2044.2602109802488);
    res << QPointF(-175.61499879935675, -2059.8462252736344);
    res << QPointF(-168.3717693169516, -2075.2768492268588);
    res << QPointF(-160.6424572210866, -2090.5008865466684);
    res << QPointF(-150.22847685877994, -2109.7385074212525);
    res << QPointF(194.23861004296444, -2056.3576305273214);
    res << QPointF(302.4787663409577, -1301.003761061316);
    res << QPointF(279.86810151275455, -1288.330749878147);
    res << QPointF(-641.7062267185897, -2051.118466118487);
    res << QPointF(-365.68188649000314, -2143.126579528016);

    // See file "collection/bugs/Issue_#609_case2.val"
    // Clear an equdistant. Bound intersection. Internal loop. Outside a loop. Start point А2.
    QTest::newRow("Issue 609. Case2b") << path << res;

    path.clear();
    path << QPointF(0, 10);
    path << QPointF(5, 10);
    path << QPointF(2.5, 15);
    path << QPointF(7.5, 15);
    path << QPointF(5, 10);
    path << QPointF(10, 10);
    path << QPointF(10, 20);
    path << QPointF(0, 20);
    path << QPointF(0, 10);

    QTest::newRow("Internal loop. Valid case.") << path << path;

    path.clear();
    path << QPointF(0, 10);
    path << QPointF(5, 10);
    path << QPointF(7.5, 15);
    path << QPointF(2.5, 15);
    path << QPointF(5, 10);
    path << QPointF(10, 10);
    path << QPointF(10, 20);
    path << QPointF(0, 20);
    path << QPointF(0, 10);

    res.clear();
    res << QPointF(0, 10);
    res << QPointF(10, 10);
    res << QPointF(10, 20);
    res << QPointF(0, 20);
    res << QPointF(0, 10);

    QTest::newRow("Internal loop. Invalid case.") << path << res;

    path.clear();
    path << QPointF(0, 10);
    path << QPointF(5, 10);
    path << QPointF(0, 0);
    path << QPointF(10, 0);
    path << QPointF(5, 10);
    path << QPointF(10, 10);
    path << QPointF(10, 20);
    path << QPointF(0, 20);
    path << QPointF(0, 10);

    QTest::newRow("External loop. Valid case.") << path << path;

    path.clear();
    path << QPointF(0, 10);
    path << QPointF(5, 10);
    path << QPointF(10, 0);
    path << QPointF(0, 0);
    path << QPointF(5, 10);
    path << QPointF(10, 10);
    path << QPointF(10, 20);
    path << QPointF(0, 20);
    path << QPointF(0, 10);

    res.clear();
    res << QPointF(0, 10);
    res << QPointF(10, 10);
    res << QPointF(10, 20);
    res << QPointF(0, 20);
    res << QPointF(0, 10);

    QTest::newRow("External loop. Invalid case.") << path << res;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::PathLoopsCase() const
{
    QFETCH(QVector<QPointF>, path);
    QFETCH(QVector<QPointF>, expect);

    const QVector<QPointF> res = VAbstractPiece::CheckLoops(path);
    Comparison(res, expect);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::BrokenDetailEquidistant_data() const
{
    QTest::addColumn<QVector<VSAPoint>>("points");
    QTest::addColumn<qreal>("width");
    QTest::addColumn<QVector<QPointF>>("ekvOrig");

    // For more details see the file "collection/bugs/GAVAUDAN Laure - corsage - figure 4.val".
    // We will test only one detail. The second require too accurate data that we cannot get from debuger.
    // The test check an open equdistant of correct detail.
    QVector<VSAPoint> points;// Input points.

    VSAPoint p = VSAPoint(787.5835464566929, 1701.3138897637796);
    p.SetSAAfter(-1);
    p.SetSABefore(0);
    points.append(p);

    p = VSAPoint(863.1740976377953, 1701.3138897637796);
    p.SetSAAfter(-1);
    p.SetSAAfter(-1);
    points.append(p);

    points.append(VSAPoint(938.7646488188976, 1701.3138897637796));
    points.append(VSAPoint(928.6149944255945, 1732.4440762118775));
    points.append(VSAPoint(910.0209054382323, 1792.3369946802652));
    points.append(VSAPoint(893.3643210561819, 1849.7845240486258));
    points.append(VSAPoint(878.5243977752426, 1905.2261712206746));
    points.append(VSAPoint(865.3802920912136, 1959.1014431001254));
    points.append(VSAPoint(853.8111604998944, 2011.8498465906928));
    points.append(VSAPoint(843.6961594970844, 2063.910888596092));
    points.append(VSAPoint(834.9144455785826, 2115.7240760200366));
    points.append(VSAPoint(827.3451752401882, 2167.7289157662426));
    points.append(VSAPoint(820.8675049777007, 2220.364914738423));
    points.append(VSAPoint(815.3605912869193, 2274.0715798402925));
    points.append(VSAPoint(810.703590663643, 2329.2884179755656));
    points.append(VSAPoint(806.7756596036716, 2386.454936047957));
    points.append(VSAPoint(803.455954602804, 2446.0106409611817));
    points.append(VSAPoint(800.6236321568394, 2508.395039618954));
    points.append(VSAPoint(798.1578487615775, 2574.047638924988));
    points.append(VSAPoint(797.0323653543306, 2608.4005039370077));
    points.append(VSAPoint(929.3158299212598, 2608.4005039370077));
    points.append(VSAPoint(927.9285659612306, 2548.9599884455793));
    points.append(VSAPoint(925.157717598664, 2463.8329634071292));
    points.append(VSAPoint(922.7222742526749, 2408.6782012856274));
    points.append(VSAPoint(919.6220036804666, 2354.5469017384876));
    points.append(VSAPoint(915.706969354324, 2301.1170261784787));
    points.append(VSAPoint(910.8272347465313, 2248.066536018368));
    points.append(VSAPoint(904.8328633293736, 2195.073392670922));
    points.append(VSAPoint(897.5739185751353, 2141.8155575489095));
    points.append(VSAPoint(888.9004639561011, 2087.9709920650976));
    points.append(VSAPoint(878.6625629445558, 2033.2176576322527));
    points.append(VSAPoint(866.7102790127839, 1977.233515663143));
    points.append(VSAPoint(852.8936756330698, 1919.696527570536));
    points.append(VSAPoint(837.0628162776984, 1860.284654767199));
    points.append(VSAPoint(819.0677644189545, 1798.675858665899));
    points.append(VSAPoint(798.7585835291225, 1734.548100679404));
    points.append(VSAPoint(787.5835464566929, 1701.3138897637796));

    p = VSAPoint(797.0323653543306, 2608.4005039370077);
    p.SetSAAfter(0);
    p.SetSABefore(-1);
    points.append(p);

    QVector<QPointF> ekvOrig;
    ekvOrig.append(QPointF(787.1898456692913, 1663.5186141732283));
    ekvOrig.append(QPointF(863.1740976377953, 1663.5186141732283));
    ekvOrig.append(QPointF(990.8407796109454, 1663.5186141732283));
    ekvOrig.append(QPointF(964.6314897747087, 1743.9055956070622));
    ekvOrig.append(QPointF(946.222111945205, 1803.203545947388));
    ekvOrig.append(QPointF(929.7733236875301, 1859.9343993344141));
    ekvOrig.append(QPointF(915.1430683369846, 1914.5927314447797));
    ekvOrig.append(QPointF(902.2033477151627, 1967.6302665424967));
    ekvOrig.append(QPointF(890.8261161082305, 2019.5037195040304));
    ekvOrig.append(QPointF(880.8841829577946, 2070.673996127427));
    ekvOrig.append(QPointF(872.2520522462703, 2121.604624314014));
    ekvOrig.append(QPointF(864.8064761358401, 2172.759620123457));
    ekvOrig.append(QPointF(864.2562272534083, 2177.2308109121955));
    ekvOrig.append(QPointF(860.1867773842832, 2147.3738416825267));
    ekvOrig.append(QPointF(851.6617474319463, 2094.450692409028));
    ekvOrig.append(QPointF(841.5996933370075, 2040.6378051462616));
    ekvOrig.append(QPointF(829.8479530577714, 1985.5930036729653));
    ekvOrig.append(QPointF(828.2738301865061, 1979.0378260789357));
    ekvOrig.append(QPointF(834.4319111572987, 2570.213599275029));
    ekvOrig.append(QPointF(796.554931640625, 2597.28125));
    ekvOrig.append(QPointF(787.1898456692913, 1663.5186141732283));

    // Disabled due to "undefined behavior" problem
#if !defined(Q_OS_WIN) && !defined(Q_CC_CLANG) && !defined(Q_PROCESSOR_X86_64)
    QTest::newRow("GAVAUDAN Laure.") << points << 37.795275590551185 << ekvOrig;
#endif

    points.clear();
    p = VSAPoint(97.33089106412862, -223.03306117556497);
    p.SetSAAfter(-1);
    p.SetSABefore(0);
    points.append(p);

    p = VSAPoint(990.7494050554426, 2.819093995045);
    p.SetSAAfter(-1);
    p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(908.3966357321774, 379.5839357215547);
    p.SetSAAfter(-1);
    p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(-135.41154226686143, 697.6417881399819);
    p.SetSAAfter(0);
    p.SetSABefore(-1);
    points.append(p);

    ekvOrig.clear();
    ekvOrig.append(QPointF(100.10981413873267, -234.02583351343978));
    ekvOrig.append(QPointF(1004.1704360325447, -5.483401649771952));
    ekvOrig.append(QPointF(918.0553412376563, 388.4941212347381));
    ekvOrig.append(QPointF(-138.65807550610091, 710.4843173601864));
    ekvOrig.append(QPointF(100.10981413873267, -234.02583351343978));

    // See the file "collection/bugs/Issue_#604.val" (since 0.5.0)
    QTest::newRow("Issue #604.") << points << 11.338582677165354 << ekvOrig;

    // See the file "collection/bugs/Issue_#627.val"
    QTest::newRow("Issue #627.") << InputPointsIssue627() << 56.692913385826778 << OutputPointsIssue627();

    points.clear();
    points.append(VSAPoint(-3139.2651132139254, 2368.0698020087843));
    points.append(VSAPoint(-3120.18086167807, 3642.1607394538896));
    points.append(VSAPoint(-3800.4958223079916, 3642.1607394538896));
    points.append(VSAPoint(-3838.2910978985424, 2357.1213693751492));
    points.append(VSAPoint(-3838.2910978985424, 2357.1213693751492));
    points.append(VSAPoint(-3842.1333590068875, 2294.1227800112047));
    points.append(VSAPoint(-3847.641880263068, 2206.284811917889));
    points.append(VSAPoint(-3852.504240762188, 2136.1966065393));
    points.append(VSAPoint(-3858.4130229769607, 2058.523232892052));
    points.append(VSAPoint(-3865.3928767793295, 1975.408848578495));
    points.append(VSAPoint(-3873.4684520412366, 1888.9976112009829));
    points.append(VSAPoint(-3882.664398634626, 1801.4336783618676));
    points.append(VSAPoint(-3890.3477903485245, 1736.3753540292228));
    points.append(VSAPoint(-3895.8091513987765, 1693.739013765668));
    points.append(VSAPoint(-3901.5644708348673, 1652.020588288036));
    points.append(VSAPoint(-3907.616829890789, 1611.4880972966203));
    points.append(VSAPoint(-3913.9693098005355, 1572.4095604917156));
    points.append(VSAPoint(-3920.6249917980986, 1535.0529975736154));
    points.append(VSAPoint(-3927.5869571174717, 1499.6864282426134));
    points.append(VSAPoint(-3934.8582869926468, 1466.5778721990046));
    points.append(VSAPoint(-3942.4420626576175, 1435.9953491430822));
    points.append(VSAPoint(-3950.341365346377, 1408.206878775141));
    points.append(VSAPoint(-3958.559276292918, 1383.4804807954743));
    points.append(VSAPoint(-3964.943774739856, 1367.2167450059815));
    points.append(VSAPoint(-3969.294575063331, 1357.40136877638));
    points.append(VSAPoint(-3971.5038697013774, 1352.8740677166752));

    ekvOrig.clear();
    ekvOrig.append(QPointF(-3101.671677349305, 2354.311657146295));
    ekvOrig.append(QPointF(-3081.815221558208, 3679.956015044441));
    ekvOrig.append(QPointF(-3837.1958160496015, 3679.956015044441));
    ekvOrig.append(QPointF(-3876.0525286101615, 2358.8277879853945));
    ekvOrig.append(QPointF(-3879.856561637429, 2296.4559949567497));
    ekvOrig.append(QPointF(-3885.3552068987756, 2208.77550784222));
    ekvOrig.append(QPointF(-3890.2001782461225, 2138.9379573546207));
    ekvOrig.append(QPointF(-3896.088149066635, 2061.538158048703));
    ekvOrig.append(QPointF(-3903.0407093929557, 1978.7487777426454));
    ekvOrig.append(QPointF(-3911.079614345216, 1892.7299244032736));
    ekvOrig.append(QPointF(-3920.2274574737066, 1805.6240334336896));
    ekvOrig.append(QPointF(-3927.860399345099, 1740.9928879223924));
    ekvOrig.append(QPointF(-3933.274855194803, 1698.7227322183812));
    ekvOrig.append(QPointF(-3938.9763839040743, 1657.3942181006862));
    ekvOrig.append(QPointF(-3944.9616042037396, 1617.3113536115984));
    ekvOrig.append(QPointF(-3951.228909124134, 1578.7567876029068));
    ekvOrig.append(QPointF(-3957.774475984356, 1542.0182714992184));
    ekvOrig.append(QPointF(-3964.590902808316, 1507.3910332410135));
    ekvOrig.append(QPointF(-3971.664775737263, 1475.1815581746985));
    ekvOrig.append(QPointF(-3978.9721167358443, 1445.7137924549359));
    ekvOrig.append(QPointF(-3986.4693843470664, 1419.339616522746));
    ekvOrig.append(QPointF(-3994.1083479844083, 1396.355179689852));
    ekvOrig.append(QPointF(-3999.8272408176463, 1381.7869935087092));
    ekvOrig.append(QPointF(-4003.5659287794874, 1373.352538644726));
    ekvOrig.append(QPointF(-4020.22165565684, 1339.2215139079456));
    ekvOrig.append(QPointF(-3963.5989171180895, 1302.9008086818892));
    ekvOrig.append(QPointF(-3101.671677349305, 2354.311657146295));

    // See the file "collection/bugs/Issue_#687.val"
    QTest::newRow("Issue #687.") << points << 37.795275590551185 << ekvOrig;

    // See private test cases in file jacket_issue_#767.val, piece "Fabric_TopCollar"
    // Curve approximation scale 0.5
    QTest::newRow("Issue #767.") << InputPointsIssue767_Fabric_TopCollar() << 37.795275590551185
                                 << OutputPointsIssue767_Fabric_TopCollar();

    // See private test cases in file jacket_issue_#767.val, piece "SkinFusing_TopCollar_Notch"
    // Curve approximation scale 0.5
    QTest::newRow("Issue #767.") << InputPointsIssue767_SkinFusing_TopCollar_Notch() << 37.795275590551185
                                 << OutputPointsIssue767_SkinFusing_TopCollar_Notch();

    points.clear();
    p = VSAPoint(1122.8447244094489, 91.85612598425197);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points.append(p);

    points.append(VSAPoint(1122.8447244094489, -664.0493858267716));
    points.append(VSAPoint(1122.8447244094489, -664.0493858267716));
    points.append(VSAPoint(1123.2605071010314, -692.1027566151841));
    points.append(VSAPoint(1125.5521344988651, -746.904953834852));
    points.append(VSAPoint(1129.5018897097445, -800.813185020781));
    points.append(VSAPoint(1135.005387617497, -854.573547056137));
    points.append(VSAPoint(1141.9582431059482, -908.9321368240863));
    points.append(VSAPoint(1150.2560710589246, -964.635051207795));
    points.append(VSAPoint(1164.860792708483, -1051.9409225168733));
    points.append(VSAPoint(1176.2406858386873, -1114.83483747791));
    points.append(VSAPoint(1176.2406858386873, -1114.83483747791));
    points.append(VSAPoint(1179.7035125223924, -1136.9452686438185));
    points.append(VSAPoint(1186.627664934387, -1177.145289266175));
    points.append(VSAPoint(1197.298845134993, -1231.2802973446833));
    points.append(VSAPoint(1213.068331853193, -1298.0112723855073));
    points.append(VSAPoint(1231.8133334949034, -1369.0339174169235));
    points.append(VSAPoint(1242.934050298766, -1410.35475));
    points.append(VSAPoint(1242.934050298766, -1410.35475));
    points.append(VSAPoint(2006.3092913385829, -1344.3643464566928));

    p = VSAPoint(2006.3092913385829, -664.0493858267716);
    p.SetSAAfter(56.69291338582678);
    points.append(p);

    p = VSAPoint(2006.3092913385829, 91.85612598425197);
    p.SetSABefore(56.69291338582678);
    points.append(p);

    ekvOrig.clear();
    ekvOrig.append(QPointF(1085.0494488188979, 129.65140157480317));
    ekvOrig.append(QPointF(1085.0494488188976, -664.0493858267716));
    ekvOrig.append(QPointF(1085.0535993023548, -664.60949305358));
    ekvOrig.append(QPointF(1085.4769360432513, -693.172544862023));
    ekvOrig.append(QPointF(1087.8146106807237, -749.0759201835479));
    ekvOrig.append(QPointF(1091.8475344926537, -804.1192779617469));
    ekvOrig.append(QPointF(1097.4551144472157, -858.8963549321579));
    ekvOrig.append(QPointF(1104.5179360066466, -914.1146778774624));
    ekvOrig.append(QPointF(1112.9230524620698, -970.5378147459013));
    ekvOrig.append(QPointF(1127.6248148552152, -1058.4237879476814));
    ekvOrig.append(QPointF(1138.969731038904, -1121.1243937910851));
    ekvOrig.append(QPointF(1142.3634069799264, -1142.7932918177955));
    ekvOrig.append(QPointF(1142.4567081704688, -1143.360751675907));
    ekvOrig.append(QPointF(1149.4580345095278, -1184.0088261935134));
    ekvOrig.append(QPointF(1160.3539624659516, -1239.2839819734102));
    ekvOrig.append(QPointF(1176.3990996347318, -1307.1814137078704));
    ekvOrig.append(QPointF(1195.269438012279, -1378.6789451473478));
    ekvOrig.append(QPointF(1214.661499217333, -1450.7350183203587));
    ekvOrig.append(QPointF(2044.1045669291343, -1379.0333442132135));
    ekvOrig.append(QPointF(2044.104566929134, -664.0493858267716));
    ekvOrig.append(QPointF(2063.0022047244097, -664.0493858267716));
    ekvOrig.append(QPointF(2063.0022047244097, 129.651401574803));
    ekvOrig.append(QPointF(1085.0494488188979, 129.65140157480317));

    // See private test cases in file collection/bugs/Issue_#883_case1.val, piece "ledge"
    QTest::newRow("Issue #883. Piece 'ledge'") << points << 37.795275590551185 << ekvOrig;

    points.clear();
    p = VSAPoint(1122.8447244094489, 91.85612598425197);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points.append(p);

    points.append(VSAPoint(1122.8447244094489, -664.0493858267716));
    points.append(VSAPoint(1122.8447244094489, -664.0493858267716));
    points.append(VSAPoint(1123.2605071010314, -692.1027566151841));
    points.append(VSAPoint(1125.5521344988651, -746.904953834852));
    points.append(VSAPoint(1129.5018897097445, -800.813185020781));
    points.append(VSAPoint(1135.005387617497, -854.573547056137));
    points.append(VSAPoint(1141.9582431059482, -908.9321368240863));
    points.append(VSAPoint(1150.2560710589246, -964.635051207795));
    points.append(VSAPoint(1164.860792708483, -1051.9409225168733));
    points.append(VSAPoint(1176.2406858386873, -1114.83483747791));
    points.append(VSAPoint(1176.2406858386873, -1114.83483747791));
    points.append(VSAPoint(1179.7035125223924, -1136.9452686438185));
    points.append(VSAPoint(1186.627664934387, -1177.145289266175));
    points.append(VSAPoint(1197.298845134993, -1231.2802973446833));
    points.append(VSAPoint(1213.068331853193, -1298.0112723855073));
    points.append(VSAPoint(1231.8133334949034, -1369.0339174169235));
    points.append(VSAPoint(1242.934050298766, -1410.35475));
    points.append(VSAPoint(1242.934050298766, -1410.35475));
    points.append(VSAPoint(2006.3092913385829, -1344.3643464566928));

    p = VSAPoint(2006.3092913385829, -664.0493858267716);
    p.SetSAAfter(56.69291338582678);
    points.append(p);

    points.append(VSAPoint(2006.3092913385829, 91.85612598425197));

    ekvOrig.clear();
    ekvOrig.append(QPointF(1085.0494488188979, 129.65140157480317));
    ekvOrig.append(QPointF(1085.0494488188976, -664.0493858267716));
    ekvOrig.append(QPointF(1085.0535993023548, -664.60949305358));
    ekvOrig.append(QPointF(1085.4769360432513, -693.172544862023));
    ekvOrig.append(QPointF(1087.8146106807237, -749.0759201835479));
    ekvOrig.append(QPointF(1091.8475344926537, -804.1192779617469));
    ekvOrig.append(QPointF(1097.4551144472157, -858.8963549321579));
    ekvOrig.append(QPointF(1104.5179360066466, -914.1146778774624));
    ekvOrig.append(QPointF(1112.9230524620698, -970.5378147459013));
    ekvOrig.append(QPointF(1127.6248148552152, -1058.4237879476814));
    ekvOrig.append(QPointF(1138.969731038904, -1121.1243937910851));
    ekvOrig.append(QPointF(1142.3634069799264, -1142.7932918177955));
    ekvOrig.append(QPointF(1142.4567081704688, -1143.360751675907));
    ekvOrig.append(QPointF(1149.4580345095278, -1184.0088261935134));
    ekvOrig.append(QPointF(1160.3539624659516, -1239.2839819734102));
    ekvOrig.append(QPointF(1176.3990996347318, -1307.1814137078704));
    ekvOrig.append(QPointF(1195.269438012279, -1378.6789451473478));
    ekvOrig.append(QPointF(1214.661499217333, -1450.7350183203587));
    ekvOrig.append(QPointF(2044.1045669291343, -1379.0333442132135));
    ekvOrig.append(QPointF(2044.104566929134, -664.0493858267716));
    ekvOrig.append(QPointF(2063.0022047244097, -664.0493858267716));
    ekvOrig.append(QPointF(2043.1596850393703, 129.651401574803));
    ekvOrig.append(QPointF(1085.0494488188979, 129.65140157480317));

    // See private test cases in file collection/bugs/Issue_#883_case1.val, piece "prong"
    QTest::newRow("Issue #883. Piece 'prong'") << points << 37.795275590551185 << ekvOrig;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::BrokenDetailEquidistant() const
{
    QFETCH(QVector<VSAPoint>, points);
    QFETCH(qreal, width);
    QFETCH(QVector<QPointF>, ekvOrig);

    const QVector<QPointF> ekv = VAbstractPiece::Equidistant(points, width);// Take result

    // Begin comparison
    Comparison(ekv, ekvOrig);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::EquidistantAngleType_data() const
{
    QTest::addColumn<QVector<VSAPoint>>("points");
    QTest::addColumn<qreal>("width");
    QTest::addColumn<QVector<QPointF>>("ekvOrig");

    // See the file "collection/bugs/Issue_#880.val"
    QTest::newRow("Issue #880. Piece: Detail.") << InputPointsIssue880_Piece_Detail() << 37.795275590551185
                                                << OutputPointsIssue880_Piece_Detail();

    // See the file "collection/bugs/Issue_#880.val"
    QTest::newRow("Issue #880. Piece: Detail_1.") << InputPointsIssue880_Piece_Detail_1() << 75.59055118110237
                                                  << OutputPointsIssue880_Piece_Detail_1();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::EquidistantAngleType() const
{
    QFETCH(QVector<VSAPoint>, points);
    QFETCH(qreal, width);
    QFETCH(QVector<QPointF>, ekvOrig);

    const QVector<QPointF> ekv = VAbstractPiece::Equidistant(points, width);// Take result

    // Begin comparison
    Comparison(ekv, ekvOrig);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::CorrectEquidistantPoints_data() const
{
    // See file zigzag.val
    QTest::addColumn<QVector<QPointF>>("points");
    QTest::addColumn<QVector<QPointF>>("expect");
    QTest::addColumn<bool>("removeFirstAndLast");

    QVector<QPointF> points;
    points.append(QPointF(-741.7894588053705, 1065.7336503858917));
    points.append(QPointF(-759.696551643576, -115.81420543069257));
    points.append(QPointF(-278.17249953019325, -217.1037453126913));
    points.append(QPointF(-244.64654130659474, 1077.9548221866635));
    points.append(QPointF(-741.7894588053705, 1065.7336503858917));

    QVector<QPointF> expect;
    expect.append(QPointF(-741.7894588053705, 1065.7336503858917));
    expect.append(QPointF(-759.696551643576, -115.81420543069257));
    expect.append(QPointF(-278.17249953019325, -217.1037453126913));
    expect.append(QPointF(-244.64654130659474, 1077.9548221866635));
    expect.append(QPointF(-741.7894588053705, 1065.7336503858917));

    QTest::newRow("Closed seam allowance. Last point equal first.") << points << expect << false;

    points.clear();
    points.append(QPointF(-704.5489521643801, 1028.8424328418016));
    points.append(QPointF(-721.4335720065426, -85.24049234531904));
    points.append(QPointF(-707.7852899705758, 755.7064514429209));
    points.append(QPointF(-721.4335720065426, -85.24049234531904));
    points.append(QPointF(-314.78124296268265, -170.7806167067443));
    points.append(QPointF(-283.4579031023758, 1039.1940357173805));

    expect.clear();
    expect.append(QPointF(-704.5489521643801, 1028.8424328418016));
    expect.append(QPointF(-721.4335720065426, -85.24049234531904));
    expect.append(QPointF(-314.78124296268265, -170.7806167067443));
    expect.append(QPointF(-283.4579031023758, 1039.1940357173805));

    QTest::newRow("Clearing bad main path.") << points << expect << true;

    points.clear();
    points.append(VSAPoint(1710.822186539242, -337.9528818897638));
    points.append(VSAPoint(2241.0236220472443, -337.9528818897638));
    points.append(VSAPoint(2241.0236220472443, -73.38595275590546));
    points.append(VSAPoint(1756.74661444881, -77.4022977175182));
    points.append(VSAPoint(1458.7808499813127, 33.04551440097044));
    points.append(VSAPoint(1458.7808499813127, 33.04551440097044));
    points.append(VSAPoint(1452.5774742317267, 17.66797799862358));
    points.append(VSAPoint(1443.6033514625851, -2.685141373445197));
    points.append(VSAPoint(1432.9463842425444, -25.199242282717215));
    points.append(VSAPoint(1420.460146989685, -50.18887828170325));
    points.append(VSAPoint(1405.9982141220862, -77.96860292291412));
    points.append(VSAPoint(1389.4141600578282, -108.85296975886055));
    points.append(VSAPoint(1360.549818901188, -161.20243176452524));
    points.append(VSAPoint(1337.599051333226, -201.88618662356873));
    points.append(VSAPoint(1337.599051333226, -201.88618662356873));
    points.append(VSAPoint(1710.8221865392422, -337.95288188976184));

    expect.clear();
    expect.append(VSAPoint(1710.822186539242, -337.9528818897638));
    expect.append(VSAPoint(2241.0236220472443, -337.9528818897638));
    expect.append(VSAPoint(2241.0236220472443, -73.38595275590546));
    expect.append(VSAPoint(1756.74661444881, -77.4022977175182));
    expect.append(VSAPoint(1458.7808499813127, 33.04551440097044));
    expect.append(VSAPoint(1452.5774742317267, 17.66797799862358));
    expect.append(VSAPoint(1443.6033514625851, -2.685141373445197));
    expect.append(VSAPoint(1432.9463842425444, -25.199242282717215));
    expect.append(VSAPoint(1420.460146989685, -50.18887828170325));
    expect.append(VSAPoint(1405.9982141220862, -77.96860292291412));
    expect.append(VSAPoint(1389.4141600578282, -108.85296975886055));
    expect.append(VSAPoint(1360.549818901188, -161.20243176452524));
    expect.append(VSAPoint(1337.599051333226, -201.88618662356873));

    // See the file "collection/bugs/Issue_#686.val"
    QTest::newRow("Issue #686.") << points << expect << true;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::CorrectEquidistantPoints() const
{
    QFETCH(QVector<QPointF>, points);
    QFETCH(QVector<QPointF>, expect);
    QFETCH(bool, removeFirstAndLast);

    const QVector<QPointF> res = VAbstractPiece::CorrectEquidistantPoints(points, removeFirstAndLast);

    // Begin comparison
    Comparison(res, expect);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::TestCorrectEquidistantPoints_data()
{
    QTest::addColumn<QVector<QPointF>>("before");
    QTest::addColumn<QVector<QPointF>>("expect");

    QVector<QPointF> before;
    before << QPointF(30.0, 39.999874015748034);
    before << QPointF(785.9055118110236, 39.999874015748034);
    before << QPointF(785.9055118110236, 3819.527433070866);
    before << QPointF(483.54330708661416, 3819.527433070866);
    before << QPointF(483.54330708661416, 1929.763653543307);
    before << QPointF(407.9527559055629, 984.8817637795973);
    before << QPointF(407.9527559055118, 1929.763653543307);
    before << QPointF(407.9527559055118, 3819.527433070866);
    before << QPointF(30.0, 3819.527433070866);

    QVector<QPointF> expect;
    expect << QPointF(30.0, 39.999874015748034);
    expect << QPointF(785.9055118110236, 39.999874015748034);
    expect << QPointF(785.9055118110236, 3819.527433070866);
    expect << QPointF(483.54330708661416, 3819.527433070866);
    expect << QPointF(483.54330708661416, 1929.763653543307);
    expect << QPointF(407.9527559055629, 984.8817637795973);
    expect << QPointF(407.9527559055118, 1929.763653543307);
    expect << QPointF(407.9527559055118, 3819.527433070866);
    expect << QPointF(30.0, 3819.527433070866);

    QTest::newRow("Test case issue #548") << before << expect;

    before.clear();
    before << QPointF(2555.0992166373157, 1774.4207500831487);
    before << QPointF(2533.267784793755, 1653.2290736835712);
    before << QPointF(2506.535007601057, 1502.652017289041);
    before << QPointF(2491.542377709352, 1415.6778721390556);
    before << QPointF(2478.773102822018, 1338.265755656977);
    before << QPointF(2468.038937020391, 1268.6117806424259);
    before << QPointF(2459.151634385811, 1204.9120598950217);
    before << QPointF(2451.9229489996146, 1145.3627062143837);
    before << QPointF(2446.16463494314, 1088.1598324001313);
    before << QPointF(2441.688446297725, 1031.4995512518844);
    before << QPointF(2438.3061371447066, 973.5779755692625);
    before << QPointF(2435.829461565424, 912.5912181518852);
    before << QPointF(2434.0701736412148, 846.7353917993719);
    before << QPointF(2432.8400274534156, 774.2066093113424);
    before << QPointF(2431.567845112944, 650.353473841431);
    before << QPointF(2430.8388882820395, 551.3113535235192);
    before << QPointF(2430.8388882820395, 551.3113535235192);
    before << QPointF(2521.6176598985803, 672.644700521423);
    before << QPointF(2521.6176454460388, 672.6446812044021);
    before << QPointF(2524.825194988043, 676.8586168908913);
    before << QPointF(2531.556290527688, 684.9826746886265);
    before << QPointF(2538.6519462028364, 692.7334672321138);
    before << QPointF(2546.0959002232003, 700.1013401169566);
    before << QPointF(2553.8718907984908, 707.0766389387593);
    before << QPointF(2561.9636561384195, 713.6497092931257);
    before << QPointF(2570.354934452699, 719.8108967756598);
    before << QPointF(2579.0294639510394, 725.5505469819657);
    before << QPointF(2587.9709828431537, 730.8590055076468);
    before << QPointF(2597.1632293387524, 735.7266179483076);
    before << QPointF(2606.5899416475477, 740.1437298995519);
    before << QPointF(2616.2348579792506, 744.1006869569835);
    before << QPointF(2626.0817165435737, 747.5878347162065);
    before << QPointF(2636.1142555502283, 750.5955187728248);
    before << QPointF(2646.3162132089255, 753.1140847224424);
    before << QPointF(2656.6713277293766, 755.1338781606632);
    before << QPointF(2661.9066021773087, 755.9323325983765);
    before << QPointF(2669.4674223109823, 756.929551265488);
    before << QPointF(2684.673455582832, 758.0191371470853);
    before << QPointF(2692.299212598425, 758.1101102362205);
    before << QPointF(2692.299212598425, 758.1101102362205);
    before << QPointF(2692.299212598425, 758.1101102362205);
    before << QPointF(2699.3540234509323, 757.9931392559722);
    before << QPointF(2713.2964524768695, 756.6087867033591);
    before << QPointF(2726.9543839401804, 753.8786800188636);
    before << QPointF(2740.2346330100504, 749.8414176232166);
    before << QPointF(2753.0440148556645, 744.5355979371486);
    before << QPointF(2765.2893446462085, 737.9998193813908);
    before << QPointF(2776.8774375508674, 730.2726803766734);
    before << QPointF(2787.7151087388274, 721.3927793437279);
    before << QPointF(2792.78632430596, 716.4869857232671);
    before << QPointF(2795.193441837398, 714.0407970608542);
    before << QPointF(2797.5205646867075, 711.5183868986292);
    before << QPointF(2797.55905511811, 711.5533198040212);
    before << QPointF(2797.55905511811, 711.5533198040212);
    before << QPointF(2797.5653033070657, 778.3129219994751);
    before << QPointF(2798.042165185835, 888.6599947271147);
    before << QPointF(2799.2551263764, 959.1215703859448);
    before << QPointF(2800.6047354960533, 1003.4500031833654);
    before << QPointF(2802.525679148114, 1047.064797090694);
    before << QPointF(2805.1184282354075, 1091.416649923951);
    before << QPointF(2808.4834536607586, 1137.9562594991576);
    before << QPointF(2812.7212263269944, 1188.1343236323337);
    before << QPointF(2817.932217136939, 1243.4015401395004);
    before << QPointF(2824.216896993419, 1305.208606836678);
    before << QPointF(2835.7112555523727, 1412.0840031200023);
    before << QPointF(2855.928400378448, 1592.3456114466708);
    before << QPointF(2868.5464960059594, 1703.7728336081707);

    expect.clear();
    expect << QPointF(2555.0992166373157, 1774.4207500831487);
    expect << QPointF(2533.267784793755, 1653.2290736835712);
    expect << QPointF(2506.535007601057, 1502.652017289041);
    expect << QPointF(2491.542377709352, 1415.6778721390556);
    expect << QPointF(2478.773102822018, 1338.265755656977);
    expect << QPointF(2468.038937020391, 1268.6117806424259);
    expect << QPointF(2459.151634385811, 1204.9120598950217);
    expect << QPointF(2451.9229489996146, 1145.3627062143837);
    expect << QPointF(2446.16463494314, 1088.1598324001313);
    expect << QPointF(2441.688446297725, 1031.4995512518844);
    expect << QPointF(2438.3061371447066, 973.5779755692625);
    expect << QPointF(2435.829461565424, 912.5912181518852);
    expect << QPointF(2434.0701736412148, 846.7353917993719);
    expect << QPointF(2432.8400274534156, 774.2066093113424);
    expect << QPointF(2431.567845112944, 650.353473841431);
    expect << QPointF(2430.8388882820395, 551.3113535235192);
    expect << QPointF(2521.6176598985803, 672.644700521423);
    expect << QPointF(2524.825194988043, 676.8586168908913);
    expect << QPointF(2531.556290527688, 684.9826746886265);
    expect << QPointF(2538.6519462028364, 692.7334672321138);
    expect << QPointF(2546.0959002232003, 700.1013401169566);
    expect << QPointF(2553.8718907984908, 707.0766389387593);
    expect << QPointF(2561.9636561384195, 713.6497092931257);
    expect << QPointF(2570.354934452699, 719.8108967756598);
    expect << QPointF(2579.0294639510394, 725.5505469819657);
    expect << QPointF(2587.9709828431537, 730.8590055076468);
    expect << QPointF(2597.1632293387524, 735.7266179483076);
    expect << QPointF(2606.5899416475477, 740.1437298995519);
    expect << QPointF(2616.2348579792506, 744.1006869569835);
    expect << QPointF(2626.0817165435737, 747.5878347162065);
    expect << QPointF(2636.1142555502283, 750.5955187728248);
    expect << QPointF(2646.3162132089255, 753.1140847224424);
    expect << QPointF(2656.6713277293766, 755.1338781606632);
    expect << QPointF(2661.9066021773087, 755.9323325983765);
    expect << QPointF(2669.4674223109823, 756.929551265488);
    expect << QPointF(2684.673455582832, 758.0191371470853);
    expect << QPointF(2692.299212598425, 758.1101102362205);
    expect << QPointF(2699.3540234509323, 757.9931392559722);
    expect << QPointF(2713.2964524768695, 756.6087867033591);
    expect << QPointF(2726.9543839401804, 753.8786800188636);
    expect << QPointF(2740.2346330100504, 749.8414176232166);
    expect << QPointF(2753.0440148556645, 744.5355979371486);
    expect << QPointF(2765.2893446462085, 737.9998193813908);
    expect << QPointF(2776.8774375508674, 730.2726803766734);
    expect << QPointF(2787.7151087388274, 721.3927793437279);
    expect << QPointF(2792.78632430596, 716.4869857232671);
    expect << QPointF(2795.193441837398, 714.0407970608542);
    expect << QPointF(2797.5205646867075, 711.5183868986292);
    expect << QPointF(2797.5653033070657, 778.3129219994751);
    expect << QPointF(2798.042165185835, 888.6599947271147);
    expect << QPointF(2799.2551263764, 959.1215703859448);
    expect << QPointF(2800.6047354960533, 1003.4500031833654);
    expect << QPointF(2802.525679148114, 1047.064797090694);
    expect << QPointF(2805.1184282354075, 1091.416649923951);
    expect << QPointF(2808.4834536607586, 1137.9562594991576);
    expect << QPointF(2812.7212263269944, 1188.1343236323337);
    expect << QPointF(2817.932217136939, 1243.4015401395004);
    expect << QPointF(2824.216896993419, 1305.208606836678);
    expect << QPointF(2835.7112555523727, 1412.0840031200023);
    expect << QPointF(2855.928400378448, 1592.3456114466708);
    expect << QPointF(2868.5464960059594, 1703.7728336081707);

    // See the file "collection/bugs/Issue_#642.val"
    QTest::newRow("Test case issue #642") << before << expect;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::TestCorrectEquidistantPoints() const
{
    QFETCH(QVector<QPointF>, before);
    QFETCH(QVector<QPointF>, expect);

    QVector<QPointF> after = VAbstractPiece::CorrectEquidistantPoints(before);
    Comparison(after, expect);
}

#ifndef Q_OS_WIN
//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::PossibleInfiniteClearLoops_data() const
{
    QTest::addColumn<QVector<QPointF>>("path");
    QTest::addColumn<QVector<QPointF>>("expect");

    QVector<QPointF> path;
    path << QPointF(-632.849625504129, 4084.1574803149606);
    path << QPointF(-1063.1826742777307, 4084.157480314961);
    path << QPointF(-1064.731669053407, 3971.857538662673);
    path << QPointF(-1065.9335577016843, 3768.725259939178);
    path << QPointF(-1066.3064907565738, 3389.730086730216);
    path << QPointF(-1064.2157605861642, 2590.593584914368);
    path << QPointF(-1063.7002274798474, 2353.011824334406);
    path << QPointF(-1063.5730500368766, 2323.3511600791016);
    path << QPointF(-1061.2277890821085, 2265.404475234159);
    path << QPointF(-1056.6870752377622, 2208.99511667452);
    path << QPointF(-1050.3195580618494, 2154.1593979456843);
    path << QPointF(-1042.4881941434103, 2100.7853411906426);
    path << QPointF(-1033.5464923745499, 2048.7106381836265);
    path << QPointF(-1018.9140925348769, 1972.9298208633923);
    path << QPointF(-998.7073870145719, 1875.5283486595902);
    path << QPointF(-984.743852909139, 1804.5663725038373);
    path << QPointF(-976.5700399899772, 1758.126392406043);
    path << QPointF(-969.6857320440495, 1712.1364207718836);
    path << QPointF(-964.4132563582218, 1666.50830864049);
    path << QPointF(-961.0600449902615, 1621.1025089368645);
    path << QPointF(-959.9236450523174, 1575.5823241167095);
    path << QPointF(-960.3877919929658, 1553.2712054750111);
    path << QPointF(-1000.3562458795624, 801.8642724069981);
    path << QPointF(-954.5715133777537, 803.7344759255401);
    path << QPointF(-946.442743379301, 803.5582812808202);
    path << QPointF(-934.1053985018102, 802.2501117597377);
    path << QPointF(-917.288618935481, 798.3566841198939);
    path << QPointF(-900.5071817401116, 792.3119964649148);
    path << QPointF(-883.7147885675736, 784.220662585902);
    path << QPointF(-867.1260743218684, 774.3001167278098);
    path << QPointF(-850.9648599763401, 762.8198974992881);
    path << QPointF(-835.447350962678, 750.0867405201303);
    path << QPointF(-820.7766612220335, 736.4325408317882);
    path << QPointF(-807.1431378915719, 722.2070808806341);
    path << QPointF(-794.7272455389146, 707.7755421404892);
    path << QPointF(-783.7035642522673, 693.5210466193306);
    path << QPointF(-774.2455412061727, 679.8541880091894);
    path << QPointF(-766.5311200750891, 667.2357322468891);
    path << QPointF(-760.7485749482415, 656.23004395692);
    path << QPointF(-756.9262152588784, 647.247947787771);
    path << QPointF(-755.9943940773794, 644.3431344612932);
    path << QPointF(-754.3168933441153, 638.3087537000382);
    path << QPointF(-751.4615751472361, 624.0253762908213);
    path << QPointF(-749.4776362727711, 608.4387853126539);
    path << QPointF(-748.2151911179384, 591.5292381129454);
    path << QPointF(-746.9479910213331, 542.1593888936636);
    path << QPointF(-745.6944329547541, 462.8644227516688);
    path << QPointF(-746.1883646034397, 475.16878663047623);
    path << QPointF(-746.2871460461391, 402.8988101963411);
    path << QPointF(-746.6738371191002, 321.4553283654042);
    path << QPointF(-748.4567648247277, 258.62151802315526);
    path << QPointF(-750.7314894991164, 217.2362191509505);
    path << QPointF(-754.2100293185232, 176.77268647996522);
    path << QPointF(-759.1411805604763, 137.7638902918793);
    path << QPointF(-767.9139385171914, 85.8954360711639);
    path << QPointF(-193.4615393890781, -82.57772819388018);
    path << QPointF(-182.84881484970134, -44.475315462966144);
    path << QPointF(-181.7932001092176, -42.33768474083549);
    path << QPointF(-179.7528132464526, -39.70760839675285);
    path << QPointF(-175.73839974706917, -36.02046568979708);
    path << QPointF(-169.4580785677048, -31.6599263589561);
    path << QPointF(-160.89874469571305, -27.009347306192694);
    path << QPointF(-150.1810493329971, -22.35672237054398);
    path << QPointF(-137.46941549286484, -17.896128595814076);
    path << QPointF(-122.93209920666818, -13.753254741366646);
    path << QPointF(-106.85016673407813, -10.037145814867706);
    path << QPointF(-79.81702769536884, -5.199728384059749);
    path << QPointF(-38.971630241556646, -0.5839317964807336);
    path << QPointF(6.260161746717735, 1.9210182870405856);
    path << QPointF(69.51425695996898, 2.8396504979139623);
    path << QPointF(-7.937007874015748, 1590.5905795945973);
    path << QPointF(-7.937007874015748, 2353.1338582677163);
    path << QPointF(-7.937007874015748, 4084.157480314961);
    path << QPointF(-335.49606299212604, 4084.157480314961);
    path << QPointF(-335.49606299212604, 2116.117122071213);
    path << QPointF(-260.2892300303194, 1589.6692913385828);
    path << QPointF(-297.70078740157464, 1327.7883897398);
    path << QPointF(-335.11234477282926, 1589.6692913385828);
    path << QPointF(-259.9055118110237, 2116.1171220712135);
    path << QPointF(-259.9055118110237, 4084.1574803149606);
    path << QPointF(-708.4401766852314, 4084.157480314961);
    path << QPointF(-708.4401766852314, 2022.3251007369636);
    path << QPointF(-660.76090517769, 1570.7716535433071);
    path << QPointF(-670.6449010946802, 1477.1638385065562);
    path << QPointF(-680.5288970116704, 1570.7716535433071);
    path << QPointF(-632.849625504129, 2022.3251007369636);
    path << QPointF(-632.849625504129, 4084.1574803149606);

    QVector<QPointF> expect;
    expect << QPointF(-632.849625504129, 4084.1574803149606);
    expect << QPointF(-708.4401766852314, 4084.1574803149606);
    expect << QPointF(-708.4401766852314, 2022.3251007369636);
    expect << QPointF(-670.6449010946802, 1664.3794685800578);
    expect << QPointF(-632.849625504129, 2022.3251007369636);
    expect << QPointF(-632.849625504129, 4084.1574803149606);

    // See the file "collection/bugs/possible_inf_loop.val"
    QTest::newRow("Possible infinite loop") << path << expect;

    path.clear();
    path << QPointF(-3289.176398074803, 40.766068311507865);
    path << QPointF(-3279.1328092842814, 44.06682404485686);
    path << QPointF(-3243.351504267316, 53.60167948872853);
    path << QPointF(-3161.3876778623217, 73.60874477474869);
    path << QPointF(-3008.115647695134, 110.74574486101676);
    path << QPointF(-2873.894366307849, 145.10523784601025);
    path << QPointF(-2781.1049285829295, 170.2972871433074);
    path << QPointF(-2688.4201916276343, 197.16160944863682);
    path << QPointF(-2598.5847429336654, 225.43041639032083);
    path << QPointF(-2534.9681776342786, 247.42186866516417);
    path << QPointF(-2494.760470256757, 262.35858401482426);
    path << QPointF(-2456.8089154086483, 277.4957895645043);
    path << QPointF(-2421.456586526415, 292.80001176774465);
    path << QPointF(-2389.0465570465194, 308.2377770780854);
    path << QPointF(-2359.921900405425, 323.77561194906696);
    path << QPointF(-2343.699902721956, 333.52460494453004);
    path << QPointF(-2334.425690039593, 339.3800428342297);
    path << QPointF(-2312.900999385486, 355.0175961871137);
    path << QPointF(-2299.713043139396, 366.746565883471);
    path << QPointF(-2292.2509689428207, 374.5588029608982);
    path << QPointF(-2285.9747534821863, 382.360491785372);
    path << QPointF(-2280.9272809370514, 390.1474481635852);
    path << QPointF(-2277.151435486974, 397.91548790223015);
    path << QPointF(-2274.690101311511, 405.6604268079995);
    path << QPointF(-2273.5861625902216, 413.37808068758574);
    path << QPointF(-2273.882503502662, 421.06426534768144);
    path << QPointF(-2274.6302053736044, 424.8926764304512);
    path << QPointF(-2309.0574148331825, 416.983936602572);
    path << QPointF(-2348.386009272143, 408.01259973815553);
    path << QPointF(-2274.6302053736044, 424.8926764304512);
    path << QPointF(-2309.0574148331825, 416.983936602572);
    path << QPointF(-2433.4520429888726, 388.9054430736358);
    path << QPointF(-2559.7817761868337, 362.0219130711671);
    path << QPointF(-2653.193005658679, 343.24683221834414);
    path << QPointF(-2751.5676350679787, 324.7189598281001);
    path << QPointF(-2852.1850451511273, 307.35120581311685);
    path << QPointF(-2952.3246166445188, 292.0564800860765);
    path << QPointF(-3025.315374842363, 282.6097398689543);
    path << QPointF(-3072.47620108711, 277.37300126453033);
    path << QPointF(-3117.98721934864, 273.1680315579757);
    path << QPointF(-3161.5083522190007, 270.1089444883756);
    path << QPointF(-3202.6995222902424, 268.3098537948152);
    path << QPointF(-3241.220652154415, 267.8848732163797);
    path << QPointF(-3276.7316644035664, 268.94811649215455);
    path << QPointF(-3308.8924816297463, 271.6136973612248);
    path << QPointF(-3337.3630264250037, 275.9957295626757);
    path << QPointF(-3355.9556969262208, 280.5372011460504);
    path << QPointF(-3367.1044424307042, 284.1225373764538);
    path << QPointF(-3377.139326285485, 288.2011754181878);
    path << QPointF(-3386.017838814569, 292.7873794886382);
    path << QPointF(-3393.697470341963, 297.8954138051904);
    path << QPointF(-3400.135711191672, 303.53954258523015);
    path << QPointF(-3405.2900516877025, 309.7340300461432);
    path << QPointF(-3409.117982154061, 316.49314040531516);
    path << QPointF(-3411.5769929147536, 323.8311378801317);
    path << QPointF(-3412.624574293787, 331.7622866879784);
    path << QPointF(-3412.5443282791143, 335.98035656218394);
    path << QPointF(-3412.0938304049364, 344.2251941754913);
    path << QPointF(-3410.765527360113, 357.87225607730386);
    path << QPointF(-3408.9498259459265, 368.2785594139036);
    path << QPointF(-3406.6749154805993, 375.6334124347902);
    path << QPointF(-3403.9689852823512, 380.1261233894636);
    path << QPointF(-3400.8602246694027, 381.9460005274235);
    path << QPointF(-3397.376822959974, 381.28235209816967);
    path << QPointF(-3393.546969472286, 378.3244863512019);
    path << QPointF(-3387.2487536004646, 370.2277603925629);
    path << QPointF(-3377.848607774182, 352.8184239863409);
    path << QPointF(-3367.5700625628333, 329.63976680025917);
    path << QPointF(-3356.6386325121834, 302.20625483031586);
    path << QPointF(-3339.521778740742, 256.44961474613956);
    path << QPointF(-3316.447784152815, 194.2270540550615);
    path << QPointF(-3300.0812187711917, 154.2221830826118);
    path << QPointF(-3289.9682851457983, 133.06332832546101);
    path << QPointF(-3283.0129680216724, 121.51001513705947);
    path << QPointF(-3278.693072922096, 115.97435552382811);
    path << QPointF(-3274.675080363335, 112.49856833137764);
    path << QPointF(-3270.987179663609, 111.27196180920784);
    path << QPointF(-3269.2913385826773, 111.6668077543023);
    path << QPointF(-2348.386009272143, 408.01259973815553);
    path << QPointF(-2403.6231556871917, 395.59097294992216);
    path << QPointF(-2433.4520429888726, 388.9054430736358);
    path << QPointF(-2458.895984464084, 383.4393884984055);
    path << QPointF(-2514.4918458536054, 371.5235994548911);
    path << QPointF(-2559.7817761868337, 362.0219130711671);
    path << QPointF(-2653.193005658679, 343.24683221834414);
    path << QPointF(-2751.5676350679787, 324.7189598281001);
    path << QPointF(-2852.1850451511273, 307.35120581311685);
    path << QPointF(-2952.3246166445188, 292.0564800860765);
    path << QPointF(-3025.315374842363, 282.6097398689543);
    path << QPointF(-3072.47620108711, 277.37300126453033);
    path << QPointF(-3117.98721934864, 273.1680315579757);
    path << QPointF(-3161.5083522190007, 270.1089444883756);
    path << QPointF(-3202.6995222902424, 268.3098537948152);
    path << QPointF(-3241.220652154415, 267.8848732163797);
    path << QPointF(-3276.7316644035664, 268.94811649215455);
    path << QPointF(-3308.8924816297463, 271.6136973612248);
    path << QPointF(-3337.3630264250037, 275.9957295626757);
    path << QPointF(-3355.9556969262208, 280.5372011460504);
    path << QPointF(-3367.1044424307042, 284.1225373764538);
    path << QPointF(-3377.139326285485, 288.2011754181878);
    path << QPointF(-3386.017838814569, 292.7873794886382);
    path << QPointF(-3393.697470341963, 297.8954138051904);
    path << QPointF(-3400.135711191672, 303.53954258523015);
    path << QPointF(-3405.2900516877025, 309.7340300461432);
    path << QPointF(-3409.117982154061, 316.49314040531516);
    path << QPointF(-3411.5769929147536, 323.8311378801317);
    path << QPointF(-3412.624574293787, 331.7622866879784);
    path << QPointF(-3412.5443282791143, 335.98035656218394);
    path << QPointF(-3412.0938304049364, 344.2251941754913);
    path << QPointF(-3410.765527360113, 357.87225607730386);
    path << QPointF(-3408.9498259459265, 368.2785594139036);
    path << QPointF(-3406.6749154805993, 375.6334124347902);
    path << QPointF(-3403.9689852823512, 380.1261233894636);
    path << QPointF(-3400.8602246694027, 381.9460005274235);
    path << QPointF(-3397.376822959974, 381.28235209816967);
    path << QPointF(-3393.546969472286, 378.3244863512019);
    path << QPointF(-3387.2487536004646, 370.2277603925629);
    path << QPointF(-3377.848607774182, 352.8184239863409);
    path << QPointF(-3367.5700625628333, 329.63976680025917);
    path << QPointF(-3356.6386325121834, 302.20625483031586);
    path << QPointF(-3339.521778740742, 256.44961474613956);
    path << QPointF(-3316.447784152815, 194.2270540550615);
    path << QPointF(-3300.0812187711917, 154.2221830826118);
    path << QPointF(-3289.9682851457983, 133.06332832546101);
    path << QPointF(-3283.0129680216724, 121.51001513705947);
    path << QPointF(-3278.693072922096, 115.97435552382811);
    path << QPointF(-3274.675080363335, 112.49856833137764);
    path << QPointF(-3270.987179663609, 111.27196180920784);
    path << QPointF(-3269.2913385826773, 111.6668077543023);
    path << QPointF(0.0, 0.0);
    path << QPointF(-2274.6302053736044, 424.8926764304512);
    path << QPointF(-2309.0574148331825, 416.983936602572);
    path << QPointF(-2433.4520429888726, 388.9054430736358);
    path << QPointF(-2559.7817761868337, 362.0219130711671);
    path << QPointF(-2653.193005658679, 343.24683221834414);
    path << QPointF(-2751.5676350679787, 324.7189598281001);
    path << QPointF(-2852.1850451511273, 307.35120581311685);
    path << QPointF(-2952.3246166445188, 292.0564800860765);
    path << QPointF(-3025.315374842363, 282.6097398689543);
    path << QPointF(-3072.47620108711, 277.37300126453033);
    path << QPointF(-3117.98721934864, 273.1680315579757);
    path << QPointF(-3161.5083522190007, 270.1089444883756);
    path << QPointF(-3202.6995222902424, 268.3098537948152);
    path << QPointF(-3241.220652154415, 267.8848732163797);
    path << QPointF(-3276.7316644035664, 268.94811649215455);
    path << QPointF(-3308.8924816297463, 271.6136973612248);
    path << QPointF(-3337.3630264250037, 275.9957295626757);
    path << QPointF(-3355.9556969262208, 280.5372011460504);
    path << QPointF(-3367.1044424307042, 284.1225373764538);
    path << QPointF(-3377.139326285485, 288.2011754181878);
    path << QPointF(-3386.017838814569, 292.7873794886382);
    path << QPointF(-3393.697470341963, 297.8954138051904);
    path << QPointF(-3400.135711191672, 303.53954258523015);
    path << QPointF(-3405.2900516877025, 309.7340300461432);
    path << QPointF(-3409.117982154061, 316.49314040531516);
    path << QPointF(-3411.5769929147536, 323.8311378801317);
    path << QPointF(-3412.624574293787, 331.7622866879784);
    path << QPointF(-3412.5443282791143, 335.98035656218394);
    path << QPointF(-3412.0938304049364, 344.2251941754913);
    path << QPointF(-3410.765527360113, 357.87225607730386);
    path << QPointF(-3408.9498259459265, 368.2785594139036);
    path << QPointF(-3406.6749154805993, 375.6334124347902);
    path << QPointF(-3403.9689852823512, 380.1261233894636);
    path << QPointF(-3400.8602246694027, 381.9460005274235);
    path << QPointF(-3397.376822959974, 381.28235209816967);
    path << QPointF(-3393.546969472286, 378.3244863512019);
    path << QPointF(-3387.2487536004646, 370.2277603925629);
    path << QPointF(-3377.848607774182, 352.8184239863409);
    path << QPointF(-3367.5700625628333, 329.63976680025917);
    path << QPointF(-3356.6386325121834, 302.20625483031586);
    path << QPointF(-3339.521778740742, 256.44961474613956);
    path << QPointF(-3316.447784152815, 194.2270540550615);
    path << QPointF(-3300.7674537894027, 155.9181480771437);
    path << QPointF(-3300.0812187711917, 154.2221830826118);
    path << QPointF(-3289.9682851457983, 133.06332832546101);
    path << QPointF(-3283.0129680216724, 121.51001513705947);
    path << QPointF(-3278.693072922096, 115.97435552382811);
    path << QPointF(-3274.675080363335, 112.49856833137764);
    path << QPointF(-3270.987179663609, 111.27196180920784);
    path << QPointF(-3269.2913385826773, 111.6668077543023);
    path << QPointF(-3311.8508415297547, 110.88125437102264);
    path << QPointF(-3685.6514075461932, 107.5655498368449);
    path << QPointF(-4814.263342905703, 99.20635184924856);
    path << QPointF(-7383.401022460118, 81.87816887323245);
    path << QPointF(-10941.483518468456, 59.05418050192678);
    path << QPointF(-15308.1805254157, 32.04268850318564);
    path << QPointF(-20303.161737786842, 2.15199464486304);
    path << QPointF(-25746.09685006686, -29.30959930518696);
    path << QPointF(-31456.65555674075, -61.033791579110314);
    path << QPointF(-37254.50755229348, -91.71228040905298);
    path << QPointF(-42959.322531210055, -120.03676402716093);
    path << QPointF(-47055.629053465884, -138.8031751224513);
    path << QPointF(-49669.199119026714, -149.97338011526602);
    path << QPointF(-52158.074133576316, -159.81916406070422);
    path << QPointF(-54499.712808925295, -168.17698923778406);
    path << QPointF(-56671.57385688429, -174.8833179255238);
    path << QPointF(-58651.11598926391, -179.77461240294173);
    path << QPointF(-60415.79791787479, -182.68733494905598);
    path << QPointF(-61943.07835452756, -183.45794784288492);
    path << QPointF(-62910.53243592165, -182.4558854979162);
    path << QPointF(-63474.98907760189, -181.08129309219626);
    path << QPointF(-63967.41587168608, -179.07918330535267);
    path << QPointF(-64384.99515715055, -176.4291139222629);
    path << QPointF(-64724.90927297162, -173.11064272780402);
    path << QPointF(-64984.340558125616, -169.10332750685347);
    path << QPointF(-65121.73298586071, -165.61084561925315);
    path << QPointF(-65188.44494822773, -163.07139041223155);
    path << QPointF(-65233.45126860215, -160.34822545258044);
    path << QPointF(-65252.05224375721, -158.17787437745008);
    path << QPointF(-65257.945977798765, -156.67611401106774);
    path << QPointF(-65258.55112619724, -155.90551181102362);
    path << QPointF(0.0, 0.0);
    path << QPointF(-3278.855552452029, 0.0);
    path << QPointF(-3297.926352438392, 0.0667852955320001);
    path << QPointF(-3333.3540289999983, 0.8343743582029102);
    path << QPointF(-3365.6017560461587, 2.3018835161185054);
    path << QPointF(-3394.7548208856488, 4.401643801852556);
    path << QPointF(-3420.8985108272445, 7.065986247978833);
    path << QPointF(-3444.1181131797193, 10.227241887071106);
    path << QPointF(-3464.4989152518488, 13.817741751703146);
    path << QPointF(-3482.1262043524084, 17.769816874448722);
    path << QPointF(-3497.0852677901735, 22.0157982878816);
    path << QPointF(-3509.461392873918, 26.48801702457557);
    path << QPointF(-3519.3398669124185, 31.118804117104382);
    path << QPointF(-3526.805977214449, 35.84049059804182);
    path << QPointF(-3531.9450110887838, 40.58540749996163);
    path << QPointF(-3534.8422558442003, 45.28588585543761);
    path << QPointF(-3535.582998789472, 49.87425669704352);
    path << QPointF(-3534.252527233375, 54.28285105735314);
    path << QPointF(-3530.9361284846827, 58.44399996894022);
    path << QPointF(-3525.7190898521712, 62.29003446437855);
    path << QPointF(-3518.6866986446166, 65.75328557624188);
    path << QPointF(-3509.9242421707922, 68.76608433710399);
    path << QPointF(-3499.517007739474, 71.26076177953868);
    path << QPointF(-3487.5502826594366, 73.16964893611967);
    path << QPointF(-3474.109354239456, 74.42507683942075);
    path << QPointF(-3459.2795097883068, 74.9593765220157);
    path << QPointF(-3443.1460366147635, 74.70487901647832);
    path << QPointF(-3425.794222027602, 73.59391535538228);
    path << QPointF(-3407.309353335596, 71.55881657130146);
    path << QPointF(-3387.7767178475224, 68.53191369680957);
    path << QPointF(-3367.281602872156, 64.4455377644804);
    path << QPointF(-3345.909295718271, 59.23201980688771);
    path << QPointF(-3323.7450836946427, 52.82369085660529);
    path << QPointF(-3300.8742541100464, 45.15288194620689);
    path << QPointF(-3289.176398074803, 40.766068311507865);
    path << QPointF(-2433.84144243598, 1.1920928955078125e-07);
    path << QPointF(-3278.855552452029, 0.0);
    path << QPointF(-3297.926352438392, 0.0667852955320001);
    path << QPointF(-3333.3540289999983, 0.8343743582029102);
    path << QPointF(-3365.6017560461587, 2.3018835161185054);
    path << QPointF(-3394.7548208856488, 4.401643801852556);
    path << QPointF(-3420.8985108272445, 7.065986247978833);
    path << QPointF(-3444.1181131797193, 10.227241887071106);
    path << QPointF(-3464.4989152518488, 13.817741751703146);
    path << QPointF(-3482.1262043524084, 17.769816874448722);
    path << QPointF(-3497.0852677901735, 22.0157982878816);
    path << QPointF(-3509.461392873918, 26.48801702457557);
    path << QPointF(-3519.3398669124185, 31.118804117104382);
    path << QPointF(-3526.805977214449, 35.84049059804182);
    path << QPointF(-3531.9450110887838, 40.58540749996163);
    path << QPointF(-3534.8422558442003, 45.28588585543761);
    path << QPointF(-3535.582998789472, 49.87425669704352);
    path << QPointF(-3534.252527233375, 54.28285105735314);
    path << QPointF(-3530.9361284846827, 58.44399996894022);
    path << QPointF(-3525.7190898521712, 62.29003446437855);
    path << QPointF(-3518.6866986446166, 65.75328557624188);
    path << QPointF(-3509.9242421707922, 68.76608433710399);
    path << QPointF(-3499.517007739474, 71.26076177953868);
    path << QPointF(-3487.5502826594366, 73.16964893611967);
    path << QPointF(-3474.109354239456, 74.42507683942075);
    path << QPointF(-3459.2795097883068, 74.9593765220157);
    path << QPointF(-3443.1460366147635, 74.70487901647832);
    path << QPointF(-3425.794222027602, 73.59391535538228);
    path << QPointF(-3407.309353335596, 71.55881657130146);
    path << QPointF(-3387.7767178475224, 68.53191369680957);
    path << QPointF(-3367.281602872156, 64.4455377644804);
    path << QPointF(-3345.909295718271, 59.23201980688771);
    path << QPointF(-3323.7450836946427, 52.82369085660529);
    path << QPointF(-3300.8742541100464, 45.15288194620689);
    path << QPointF(-3289.176398074803, 40.766068311507865);
    path << QPointF(-2375.6588995456696, -1.1920928955078125e-07);
    path << QPointF(-3278.855552452029, 0.0);
    path << QPointF(-3297.926352438392, 0.0667852955320001);
    path << QPointF(-3333.3540289999983, 0.8343743582029102);
    path << QPointF(-3365.6017560461587, 2.3018835161185054);
    path << QPointF(-3394.7548208856488, 4.401643801852556);
    path << QPointF(-3420.8985108272445, 7.065986247978833);
    path << QPointF(-3444.1181131797193, 10.227241887071106);
    path << QPointF(-3464.4989152518488, 13.817741751703146);
    path << QPointF(-3482.1262043524084, 17.769816874448722);
    path << QPointF(-3497.0852677901735, 22.0157982878816);
    path << QPointF(-3509.461392873918, 26.48801702457557);
    path << QPointF(-3519.3398669124185, 31.118804117104382);
    path << QPointF(-3526.805977214449, 35.84049059804182);
    path << QPointF(-3531.9450110887838, 40.58540749996163);
    path << QPointF(-3534.8422558442003, 45.28588585543761);
    path << QPointF(-3535.582998789472, 49.87425669704352);
    path << QPointF(-3534.252527233375, 54.28285105735314);
    path << QPointF(-3530.9361284846827, 58.44399996894022);
    path << QPointF(-3525.7190898521712, 62.29003446437855);
    path << QPointF(-3518.6866986446166, 65.75328557624188);
    path << QPointF(-3509.9242421707922, 68.76608433710399);
    path << QPointF(-3499.517007739474, 71.26076177953868);
    path << QPointF(-3487.5502826594366, 73.16964893611967);
    path << QPointF(-3474.109354239456, 74.42507683942075);
    path << QPointF(-3459.2795097883068, 74.9593765220157);
    path << QPointF(-3443.1460366147635, 74.70487901647832);
    path << QPointF(-3425.794222027602, 73.59391535538228);
    path << QPointF(-3407.309353335596, 71.55881657130146);
    path << QPointF(-3387.7767178475224, 68.53191369680957);
    path << QPointF(-3367.281602872156, 64.4455377644804);
    path << QPointF(-3345.909295718271, 59.23201980688771);
    path << QPointF(-3323.7450836946427, 52.82369085660529);
    path << QPointF(-3300.8742541100464, 45.15288194620689);
    path << QPointF(-3289.176398074803, 40.766068311507865);
    path << QPointF(-2317.7482029497623, 1.1920928955078125e-07);
    path << QPointF(-3159.9587076218127, -3.0211712963817957e-16);
    path << QPointF(-3278.855552452029, 0.0);
    path << QPointF(-3297.926352438392, 0.0667852955320001);
    path << QPointF(-3333.3540289999983, 0.8343743582029102);
    path << QPointF(-3365.6017560461587, 2.3018835161185054);
    path << QPointF(-3394.7548208856488, 4.401643801852556);
    path << QPointF(-3420.8985108272445, 7.065986247978833);
    path << QPointF(-3444.1181131797193, 10.227241887071106);
    path << QPointF(-3464.4989152518488, 13.817741751703146);
    path << QPointF(-3482.1262043524084, 17.769816874448722);
    path << QPointF(-3497.0852677901735, 22.0157982878816);
    path << QPointF(-3509.461392873918, 26.48801702457557);
    path << QPointF(-3519.3398669124185, 31.118804117104382);
    path << QPointF(-3526.805977214449, 35.84049059804182);
    path << QPointF(-3531.9450110887838, 40.58540749996163);
    path << QPointF(-3534.8422558442003, 45.28588585543761);
    path << QPointF(-3535.582998789472, 49.87425669704352);
    path << QPointF(-3534.252527233375, 54.28285105735314);
    path << QPointF(-3530.9361284846827, 58.44399996894022);
    path << QPointF(-3525.7190898521712, 62.29003446437855);
    path << QPointF(-3518.6866986446166, 65.75328557624188);
    path << QPointF(-3509.9242421707922, 68.76608433710399);
    path << QPointF(-3499.517007739474, 71.26076177953868);
    path << QPointF(-3487.5502826594366, 73.16964893611967);
    path << QPointF(-3474.109354239456, 74.42507683942075);
    path << QPointF(-3459.2795097883068, 74.9593765220157);
    path << QPointF(-3443.1460366147635, 74.70487901647832);
    path << QPointF(-3425.794222027602, 73.59391535538228);
    path << QPointF(-3407.309353335596, 71.55881657130146);
    path << QPointF(-3387.7767178475224, 68.53191369680957);
    path << QPointF(-3367.281602872156, 64.4455377644804);
    path << QPointF(-3345.909295718271, 59.23201980688771);
    path << QPointF(-3323.7450836946427, 52.82369085660529);
    path << QPointF(-3300.8742541100464, 45.15288194620689);

    expect.clear();
    expect << QPointF(-3289.176398074803, 40.766068311507865);
    expect << QPointF(-3279.1328092842814, 44.06682404485686);
    expect << QPointF(-3243.351504267316, 53.60167948872853);
    expect << QPointF(-3161.3876778623217, 73.60874477474869);
    expect << QPointF(-3037.0528777920667, 103.7344074667516);
    expect << QPointF(0.0, 0.0);
    expect << QPointF(-2274.6302053736044, 424.8926764304512);
    expect << QPointF(-2309.0574148331825, 416.983936602572);
    expect << QPointF(-2433.4520429888726, 388.9054430736358);
    expect << QPointF(-2559.7817761868337, 362.0219130711671);
    expect << QPointF(-2653.193005658679, 343.24683221834414);
    expect << QPointF(-2751.5676350679787, 324.7189598281001);
    expect << QPointF(-2852.1850451511273, 307.35120581311685);
    expect << QPointF(-2952.3246166445188, 292.0564800860765);
    expect << QPointF(-3025.315374842363, 282.6097398689543);
    expect << QPointF(-3072.47620108711, 277.37300126453033);
    expect << QPointF(-3117.98721934864, 273.1680315579757);
    expect << QPointF(-3161.5083522190007, 270.1089444883756);
    expect << QPointF(-3202.6995222902424, 268.3098537948152);
    expect << QPointF(-3241.220652154415, 267.8848732163797);
    expect << QPointF(-3276.7316644035664, 268.94811649215455);
    expect << QPointF(-3308.8924816297463, 271.6136973612248);
    expect << QPointF(-3337.3630264250037, 275.9957295626757);
    expect << QPointF(-3347.7860794585613, 278.5416790087658);
    expect << QPointF(-3339.521778740742, 256.44961474613956);
    expect << QPointF(-3316.447784152815, 194.2270540550615);
    expect << QPointF(-3300.7674537894027, 155.9181480771437);
    expect << QPointF(-3300.0812187711917, 154.2221830826118);
    expect << QPointF(-3289.9682851457983, 133.06332832546101);
    expect << QPointF(-3283.0129680216724, 121.51001513705947);
    expect << QPointF(-3278.693072922096, 115.97435552382811);
    expect << QPointF(-3274.675080363335, 112.49856833137764);
    expect << QPointF(-3272.0227399663845, 111.61639218434759);
    expect << QPointF(-3311.8508415297547, 110.88125437102264);
    expect << QPointF(-3685.6514075461932, 107.5655498368449);
    expect << QPointF(-4814.263342905703, 99.20635184924856);
    expect << QPointF(-7383.401022460118, 81.87816887323245);
    expect << QPointF(-10941.483518468456, 59.05418050192678);
    expect << QPointF(-15308.1805254157, 32.04268850318564);
    expect << QPointF(-20303.161737786842, 2.15199464486304);
    expect << QPointF(-25746.09685006686, -29.30959930518696);
    expect << QPointF(-31456.65555674075, -61.033791579110314);
    expect << QPointF(-36320.89133149458, -86.77218624604188);
    expect << QPointF(0.0, 0.0);
    expect << QPointF(-3159.958705487363, 0.0);
    expect << QPointF(-3159.9587076218127, -3.0211712963817957e-16);
    expect << QPointF(-3278.855552452029, 0.0);
    expect << QPointF(-3297.926352438392, 0.0667852955320001);
    expect << QPointF(-3333.3540289999983, 0.8343743582029102);
    expect << QPointF(-3365.6017560461587, 2.3018835161185054);
    expect << QPointF(-3394.7548208856488, 4.401643801852556);
    expect << QPointF(-3420.8985108272445, 7.065986247978833);
    expect << QPointF(-3444.1181131797193, 10.227241887071106);
    expect << QPointF(-3464.4989152518488, 13.817741751703146);
    expect << QPointF(-3482.1262043524084, 17.769816874448722);
    expect << QPointF(-3497.0852677901735, 22.0157982878816);
    expect << QPointF(-3509.461392873918, 26.48801702457557);
    expect << QPointF(-3519.3398669124185, 31.118804117104382);
    expect << QPointF(-3526.805977214449, 35.84049059804182);
    expect << QPointF(-3531.9450110887838, 40.58540749996163);
    expect << QPointF(-3534.8422558442003, 45.28588585543761);
    expect << QPointF(-3535.582998789472, 49.87425669704352);
    expect << QPointF(-3534.252527233375, 54.28285105735314);
    expect << QPointF(-3530.9361284846827, 58.44399996894022);
    expect << QPointF(-3525.7190898521712, 62.29003446437855);
    expect << QPointF(-3518.6866986446166, 65.75328557624188);
    expect << QPointF(-3509.9242421707922, 68.76608433710399);
    expect << QPointF(-3499.517007739474, 71.26076177953868);
    expect << QPointF(-3487.5502826594366, 73.16964893611967);
    expect << QPointF(-3474.109354239456, 74.42507683942075);
    expect << QPointF(-3459.2795097883068, 74.9593765220157);
    expect << QPointF(-3443.1460366147635, 74.70487901647832);
    expect << QPointF(-3425.794222027602, 73.59391535538228);
    expect << QPointF(-3407.309353335596, 71.55881657130146);
    expect << QPointF(-3387.7767178475224, 68.53191369680957);
    expect << QPointF(-3367.281602872156, 64.4455377644804);
    expect << QPointF(-3345.909295718271, 59.23201980688771);
    expect << QPointF(-3323.7450836946427, 52.82369085660529);
    expect << QPointF(-3300.8742541100464, 45.15288194620689);

    // See the file "collection/bugs/Issue_#861.val from private collection"
    QTest::newRow("Check if hangs") << path << expect;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::PossibleInfiniteClearLoops() const
{
    QFETCH(QVector<QPointF>, path);
    QFETCH(QVector<QPointF>, expect);

    QVector<QPointF> res = VAbstractPiece::CheckLoops(path);
    Comparison(res, expect);
}
#endif //#ifndef Q_OS_WIN

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::Case3() const
{
    const QVector<QPointF> points = InputPointsCase3a(); // Input points.

    const qreal result = VAbstractPiece::SumTrapezoids(points);
    QVERIFY(result < 0);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::Case4() const
{
    const QVector<QPointF> points = InputPointsCase4a(); // Input points.

    const qreal result = VAbstractPiece::SumTrapezoids(points);
    QVERIFY(result > 0);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::Case5() const
{
    const QVector<QPointF> points = InputPointsCase5a(); // Input points.

    const qreal result = VAbstractPiece::SumTrapezoids(points);
    QVERIFY(qFuzzyIsNull(result));
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue298Case1() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(1168.8582803149607, 39.999874015748034);
    points += VSAPoint(1168.8582803149607, 39.999874015748034);
    points += VSAPoint(1158.4080212626877, 52.34979612030506);
    points += VSAPoint(1138.6467679457483, 77.34433043475516);
    points += VSAPoint(1120.211621843451, 102.68135735665871);
    points += VSAPoint(1103.0570156997037, 128.3485040871826);
    points += VSAPoint(1087.1373822584126, 154.33339782749366);
    points += VSAPoint(1072.4071542634842, 180.62366577875878);
    points += VSAPoint(1058.8207644588258, 207.20693514214486);
    points += VSAPoint(1046.3326455883434, 234.07083311881877);
    points += VSAPoint(1034.8972303959442, 261.2029869099473);
    points += VSAPoint(1024.4689516255348, 288.5910237166974);
    points += VSAPoint(1015.0022420210217, 316.22257074023594);
    points += VSAPoint(1006.451534326312, 344.08525518172974);
    points += VSAPoint(998.7712612853122, 372.16670424234576);
    points += VSAPoint(991.9158556419289, 400.4545451232507);
    points += VSAPoint(982.9908265175081, 443.2242931323019);
    points += VSAPoint(973.6824125433218, 500.88985222984854);
    points += VSAPoint(966.8532575499223, 559.1582682295549);
    points += VSAPoint(962.1388234885637, 617.9305587407562);
    points += VSAPoint(959.1745723104998, 677.107741372787);
    points += VSAPoint(957.5959659669836, 736.5908337349825);
    points += VSAPoint(957.0384664092692, 796.2808534366778);
    points += VSAPoint(957.3056451064515, 885.992413850848);
    points += VSAPoint(957.688768266514, 942.023249737111);
    points += VSAPoint(957.688768266514, 942.023249737111);
    points += VSAPoint(496.34439043247056, 491.011561876428);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue298Case1() const
{
    QVector<QPointF> points;

    points += QPointF(1181.3702112033552, -59.406362870594705);
    points += QPointF(1269.0394640469171, 38.62919078337234);
    points += QPointF(1216.9245192058738, 100.21766491121537);
    points += QPointF(1198.8808131327262, 123.03980191699638);
    points += QPointF(1182.221067861894, 145.9367408453258);
    points += QPointF(1166.7329413673538, 169.11045943642068);
    points += QPointF(1152.3625684065119, 192.56656574522466);
    points += QPointF(1139.0585460343918, 216.3113654260635);
    points += QPointF(1126.7720261028358, 240.35129609014484);
    points += QPointF(1115.456677001793, 264.6923827640837);
    points += QPointF(1105.0685270464996, 289.3397503891491);
    points += QPointF(1095.5657115675733, 314.29722092044074);
    points += QPointF(1086.9081516638566, 339.5670119653025);
    points += QPointF(1079.05719443133, 365.14954287895875);
    points += QPointF(1071.975242726908, 391.04334443729294);
    points += QPointF(1065.6650892202777, 417.08128115689766);
    points += QPointF(1057.3401968224273, 456.97510616238503);
    points += QPointF(1048.5682451589698, 511.3172850182788);
    points += QPointF(1042.0910863010336, 566.5823635228949);
    points += QPointF(1037.5780007657413, 622.8445512489122);
    points += QPointF(1034.7149371280923, 680.0016643255938);
    points += QPointF(1033.1771512850767, 737.9466090605974);
    points += QPointF(1032.63006888955, 796.5212904492478);
    points += QPointF(1032.8954266362925, 885.6214237282716);
    points += QPointF(1033.62206579804, 991.8906443458876);
    points += QPointF(940.8759712878797, 1031.2978406173477);
    points += QPointF(376.98902117953776, 480.04021757016505);
    points += QPointF(1181.3702112033552, -59.406362870594705);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue298Case2() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(35.0000125984252, 39.999874015748034);
    points += VSAPoint(957.688768266514, 942.023249737111);
    points += VSAPoint(957.688768266514, 942.023249737111);
    points += VSAPoint(957.9428301208773, 1005.4262286032558);
    points += VSAPoint(956.808738701602, 1094.3704783643723);
    points += VSAPoint(954.7225808350249, 1153.2234318990995);
    points += VSAPoint(951.1057634130252, 1211.5904360386712);
    points += VSAPoint(945.5937483868565, 1269.3725083924223);
    points += VSAPoint(937.8219977077727, 1326.4706665696876);
    points += VSAPoint(930.2003883425905, 1368.7591370124287);
    points += VSAPoint(924.2779568426622, 1396.7024844774487);
    points += VSAPoint(917.5855387770855, 1424.4191758035984);
    points += VSAPoint(910.0775668897668, 1451.896838192044);
    points += VSAPoint(901.7084739246127, 1479.1230988439531);
    points += VSAPoint(892.4326926255303, 1506.0855849604923);
    points += VSAPoint(882.2046557364258, 1532.7719237428282);
    points += VSAPoint(870.9787960012063, 1559.1697423921285);
    points += VSAPoint(858.7095461637784, 1585.266668109559);
    points += VSAPoint(845.3513389680488, 1611.0503280962876);
    points += VSAPoint(830.8586071579242, 1636.5083495534805);
    points += VSAPoint(815.1857834773112, 1661.6283596823046);
    points += VSAPoint(798.2873006701168, 1686.397985683927);
    points += VSAPoint(780.1175914802475, 1710.8048547595142);
    points += VSAPoint(760.6310886516098, 1734.8365941102336);
    points += VSAPoint(739.7822249281108, 1758.4808309372515);
    points += VSAPoint(717.5254330536571, 1781.725192441735);
    points += VSAPoint(693.8151457721555, 1804.5573058248515);
    points += VSAPoint(681.3372913240995, 1815.7969526662778);
    points += VSAPoint(681.3372913240995, 1815.7969526662778);
    points += VSAPoint(35.0000125984252, 39.999874015748034);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue298Case2() const
{
    QVector<QPointF> points;

    points += QPointF(-10.830702765714818, 24.587011512350955);
    points += QPointF(39.855615789834275, -8.108689868294974);
    points += QPointF(995.4203583783104, 926.0543521489114);
    points += QPointF(995.7390712145076, 1005.5914525468322);
    points += QPointF(994.5954771294847, 1095.2809750974982);
    points += QPointF(992.4764226198034, 1155.061981412451);
    points += QPointF(988.7898546287646, 1214.554597181484);
    points += QPointF(983.1461162030054, 1273.7175168242372);
    points += QPointF(975.1622593873819, 1332.3739943859748);
    points += QPointF(967.2940111839067, 1376.0309394806493);
    points += QPointF(961.2519128953934, 1404.5389014377904);
    points += QPointF(961.0174119331466, 1405.5735214303666);
    points += QPointF(954.3249938675699, 1433.2902127565162);
    points += QPointF(954.0443121744452, 1434.3811384277756);
    points += QPointF(946.5363402871264, 1461.8588008162212);
    points += QPointF(946.2045616347446, 1463.0019319630142);
    points += QPointF(937.6515558315413, 1490.8264962646508);
    points += QPointF(927.9590662522897, 1519.0002540634812);
    points += QPointF(917.2525136214659, 1546.9351054719114);
    points += QPointF(905.4832013554212, 1574.6108628886288);
    points += QPointF(892.6034200213376, 1602.0064006098492);
    points += QPointF(878.5666407075367, 1629.0998226411741);
    points += QPointF(863.3276693496916, 1655.8686930581748);
    points += QPointF(846.8427441486504, 1682.2903192093897);
    points += QPointF(829.0695617338297, 1708.3420710912978);
    points += QPointF(809.967223824731, 1734.001714595386);
    points += QPointF(789.4961040754649, 1759.2477332554781);
    points += QPointF(767.6176433472375, 1784.0596133526062);
    points += QPointF(744.2940893691135, 1808.418070909965);
    points += QPointF(720.0315154937306, 1831.7820154759668);
    points += QPointF(719.1107590616128, 1832.6395921505045);
    points += QPointF(664.9901879624223, 1881.3896483836077);
    points += QPointF(-10.830702765714818, 24.587011512350955);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue548Case1() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(236.97989607468364, 65.89325192030674);
    points += VSAPoint(198.93409106041895, 172.04876297154925);
    points += VSAPoint(260.32251114299453, 75.38027418944861);
    points += VSAPoint(324.54110236213444, 101.48031496062993);
    points += VSAPoint(29.858267716535437, 300.85039370078744);
    points += VSAPoint(99.86433649395013, 10.166060970128015);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue548Case1() const
{
    QVector<QPointF> points;

    points += QPointF(251.32210577118798, 59.48301432799721);
    points += QPointF(243.9841262159756, 79.95746530820585);
    points += QPointF(255.82424817748586, 61.31279754390509);
    points += QPointF(348.48337789725855, 98.9717841021069);
    points += QPointF(29.780382054543473, 314.59289909613994);
    points += QPointF(17.01672179602679, 305.7450049304056);
    points += QPointF(91.92616539550944, -5.299480329501037);
    points += QPointF(251.32210577118798, 59.48301432799721);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue548Case2() const
{
    QVector<VSAPoint> points;
    points << VSAPoint(99.86433649395013, 10.166060970128015);
    points << VSAPoint(176.0178302829931, 57.36978169486653);
    points << VSAPoint(115.46606095399079, 156.67924434657942);
    points << VSAPoint(197.43414263641347, 70.64467660756823);
    points << VSAPoint(247.18110236220474, 101.48031496062993);
    points << VSAPoint(29.858267716535437, 300.85039370078744);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue548Case2() const
{
    QVector<QPointF> points;
    points << QPointF(75.35612582031402, -49.49247429729551);
    points << QPointF(207.99390662262346, 32.7230151178754);
    points << QPointF(309.47290565612207, 95.62474281894228);
    points << QPointF(34.78597607721976, 347.62014343263024);
    points << QPointF(-13.438975506560153, 319.209057294505);
    points << QPointF(75.35612582031402, -49.49247429729551);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue548Case3() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(99.86433649395013, 10.166060970128015);
    points += VSAPoint(176.0178302829931, 57.36978169486653);
    points += VSAPoint(115.46606095399079, 156.67924434657942);
    points += VSAPoint(197.43414263641347, 70.64467660756823);
    points += VSAPoint(247.18110236220474, 101.48031496062993);
    points += VSAPoint(29.858267716535437, 300.85039370078744);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue548Case3() const
{
    QVector<QPointF> points;

    points += QPointF(50.84791514667799, -109.15100956471929);
    points += QPointF(220.96071459087483, -3.7066408675763003);
    points += QPointF(371.76470895003956, 89.76917067725468);
    points += QPointF(39.71368443790398, 394.38989316447305);
    points += QPointF(-56.73621872965576, 337.56772088822254);
    points += QPointF(50.84791514667799, -109.15100956471929);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::InputPointsCase3a() const
{
    QVector<QPointF> points;

    points += QPointF(35, 35);
    points += QPointF(50, 50);
    points += QPointF(15, 50);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::InputPointsCase4a() const
{
    QVector<QPointF> points;

    points += QPointF(15, 15);
    points += QPointF(15, 50);
    points += QPointF(50, 50);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::InputPointsCase5a() const
{
    QVector<QPointF> points;

    points += QPointF(35, 35);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue646() const
{
    QVector<VSAPoint> points;

    points.append(VSAPoint(1352.8346456692914, 1173.8581417322835));

    VSAPoint p = VSAPoint(1352.8346456692914, 1362.8345196850394);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1352.8346456692914, 1362.8345196850394);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1350.6660166205588, 1367.7550606147208);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1344.6776732217286, 1376.7202577023593);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1336.7920071638646, 1384.6921570699324);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1327.102329227638, 1391.7349409463345);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1315.7019501937195, 1397.9127915604581);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1302.6841808427803, 1403.289891141198);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1288.142331955491, 1407.930421917447);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1272.169714312523, 1411.8985661180982);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1245.881900239392, 1416.7944475909562);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1206.4717817874364, 1421.4666032853988);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1163.0181823648445, 1424.6044927975377);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1116.2675882169847, 1426.7215739585208);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(1041.771617936041, 1429.0735145188771);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(963.6986998754783, 1432.082545646023);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(911.2249892802281, 1435.25097171387);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(859.186715186551, 1439.9658785863046);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(808.3303638398154, 1446.7407240944754);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(771.4905538168089, 1453.5750227393632);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(747.6486510586797, 1458.9887661869527);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(724.5221275003055, 1465.2063143234973);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(702.2042939223572, 1472.2918493778907);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(691.4173228346457, 1476.2203464566928);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(691.4173228346457, 1476.2203464566928);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(691.4173228346457, 1476.2203464566928);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(674.314621551016, 1482.390491353628);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(639.4466353664043, 1492.6483289419746);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(603.7620983221955, 1500.410735703404);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(567.2371765086411, 1505.6816136598163);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(529.8480360159926, 1508.4648648331117);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(491.5708429345011, 1508.7643912451913);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(452.3817633544179, 1506.5840949179556);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(412.2569633659944, 1501.9278778733042);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(371.1726090594818, 1494.7996421331377);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(329.10486652513146, 1485.2032897193571);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(286.0299018531947, 1473.1427226538626);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(241.92388113392275, 1458.6218429585542);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(196.762970457567, 1441.644552655333);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(150.52333591437866, 1422.2147537660987);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(103.18114359460913, 1400.3363483127525);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(54.712559588509635, 1376.013238317194);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(30.0, 1362.8345196850394);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    p = VSAPoint(30.0, 1362.8345196850394);
    p.SetSABefore(0.0);
    p.SetSAAfter(0.0);
    points.append(p);

    points.append(VSAPoint(30.0, 1173.8581417322835));
    points.append(VSAPoint(30.0, 39.999874015748034));
    points.append(VSAPoint(30.0, 39.999874015748034));
    points.append(VSAPoint(33.76305151898726, 60.995030973282226));
    points.append(VSAPoint(43.0315825387278, 101.39020424935029));
    points.append(VSAPoint(54.27954844568175, 139.90490371803747));
    points.append(VSAPoint(67.38308402773156, 176.57656822314317));
    points.append(VSAPoint(82.21832407275966, 211.44263660846684));
    points.append(VSAPoint(98.66140336864854, 244.5405477178079));
    points.append(VSAPoint(116.58845670328057, 275.9077403949657));
    points.append(VSAPoint(135.87561886453827, 305.58165348373984));
    points.append(VSAPoint(156.39902464030405, 333.59972582792955));
    points.append(VSAPoint(178.03480881846033, 359.9993962713345));
    points.append(VSAPoint(200.65910618688957, 384.8181036577539));
    points.append(VSAPoint(224.1480515334743, 408.0932868309873));
    points.append(VSAPoint(248.37777964609688, 429.8623846348342));
    points.append(VSAPoint(273.2244253126397, 450.1628359130938));
    points.append(VSAPoint(298.5641233209853, 469.03207950956573));
    points.append(VSAPoint(324.27300845901607, 486.50755426804926));
    points.append(VSAPoint(350.2272155146145, 502.62669903234394));
    points.append(VSAPoint(376.30287927566303, 517.4269526462492));
    points.append(VSAPoint(402.37613453004406, 530.9457539535643));
    points.append(VSAPoint(428.3231160656401, 543.220541798089));
    points.append(VSAPoint(454.0199586703335, 554.2887550236226));
    points.append(VSAPoint(479.34279713200675, 564.1878324739644));
    points.append(VSAPoint(504.1677662385423, 572.9552129929136));
    points.append(VSAPoint(528.3710007778225, 580.6283354242702));
    points.append(VSAPoint(563.3555698213261, 590.2932652501413));
    points.append(VSAPoint(606.5452489509445, 599.5232277128903));
    points.append(VSAPoint(645.018415993426, 605.0500570136577));
    points.append(VSAPoint(677.7841492518301, 607.1732639028387));
    points.append(VSAPoint(691.4173228346472, 606.9290078739998));
    points.append(VSAPoint(691.4173228346472, 606.9290078739998));
    points.append(VSAPoint(691.4173228346472, 606.9290078739998));
    points.append(VSAPoint(705.4040324293001, 606.5537669203095));
    points.append(VSAPoint(733.313887957579, 607.2856128814886));
    points.append(VSAPoint(761.113422978073, 609.7421003245712));
    points.append(VSAPoint(788.7648106968343, 613.8630604047041));
    points.append(VSAPoint(816.2302243199158, 619.5883242770342));
    points.append(VSAPoint(843.4718370533695, 626.8577230967081));
    points.append(VSAPoint(870.4518221032482, 635.6110880188726));
    points.append(VSAPoint(897.1323526756041, 645.7882501986744));
    points.append(VSAPoint(923.4756019764898, 657.3290407912602));
    points.append(VSAPoint(949.4437432119576, 670.1732909517772));
    points.append(VSAPoint(974.99894958806, 684.2608318353717));
    points.append(VSAPoint(1000.1033943108496, 699.5314945971908));
    points.append(VSAPoint(1024.7192505863786, 715.9251103923807));
    points.append(VSAPoint(1048.8086916206998, 733.381510376089));
    points.append(VSAPoint(1072.333890619865, 751.8405257034619));
    points.append(VSAPoint(1095.257020789927, 771.2419875296464));
    points.append(VSAPoint(1117.5402553369386, 791.5257270097891));
    points.append(VSAPoint(1139.1457674669518, 812.6315752990367));
    points.append(VSAPoint(1160.0357303860196, 834.4993635525362));
    points.append(VSAPoint(1180.1723173001938, 857.0689229254342));
    points.append(VSAPoint(1199.517701415527, 880.2800845728777));
    points.append(VSAPoint(1218.0340559380716, 904.0726796500132));
    points.append(VSAPoint(1235.6835540738803, 928.3865393119877));
    points.append(VSAPoint(1252.4283690290056, 953.1614947139476));
    points.append(VSAPoint(1268.2306740094996, 978.3373770110402));
    points.append(VSAPoint(1283.0526422214152, 1003.8540173584116));
    points.append(VSAPoint(1296.8564468708046, 1029.651246911209));
    points.append(VSAPoint(1309.60426116372, 1055.6688968245794));
    points.append(VSAPoint(1321.258258306214, 1081.846798253669));
    points.append(VSAPoint(1331.780611504339, 1108.1247823536248));
    points.append(VSAPoint(1341.1334939641479, 1134.4426802795938));
    points.append(VSAPoint(1349.2790788916927, 1160.7403231867224));
    points.append(VSAPoint(1352.8346456692914, 1173.8581417322835));

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue646() const
{
    QVector<QPointF> points;

    points += QPointF(1391.2089508950362, 1170.962993556315);
    points += QPointF(1352.8346456692914, 1362.8345196850394);
    points += QPointF(1350.6660166205588, 1367.7550606147208);
    points += QPointF(1344.6776732217286, 1376.7202577023593);
    points += QPointF(1336.7920071638646, 1384.6921570699324);
    points += QPointF(1327.102329227638, 1391.7349409463345);
    points += QPointF(1315.7019501937195, 1397.9127915604581);
    points += QPointF(1302.6841808427803, 1403.289891141198);
    points += QPointF(1288.142331955491, 1407.930421917447);
    points += QPointF(1272.169714312523, 1411.8985661180982);
    points += QPointF(1245.881900239392, 1416.7944475909562);
    points += QPointF(1206.4717817874364, 1421.4666032853988);
    points += QPointF(1163.0181823648445, 1424.6044927975377);
    points += QPointF(1116.2675882169847, 1426.7215739585208);
    points += QPointF(1041.771617936041, 1429.0735145188771);
    points += QPointF(963.6986998754783, 1432.082545646023);
    points += QPointF(911.2249892802281, 1435.25097171387);
    points += QPointF(859.186715186551, 1439.9658785863046);
    points += QPointF(808.3303638398154, 1446.7407240944754);
    points += QPointF(771.4905538168089, 1453.5750227393632);
    points += QPointF(747.6486510586797, 1458.9887661869527);
    points += QPointF(724.5221275003055, 1465.2063143234973);
    points += QPointF(702.2042939223572, 1472.2918493778907);
    points += QPointF(691.4173228346457, 1476.2203464566928);
    points += QPointF(674.314621551016, 1482.390491353628);
    points += QPointF(639.4466353664043, 1492.6483289419746);
    points += QPointF(603.7620983221955, 1500.410735703404);
    points += QPointF(567.2371765086411, 1505.6816136598163);
    points += QPointF(529.8480360159926, 1508.4648648331117);
    points += QPointF(491.5708429345011, 1508.7643912451913);
    points += QPointF(452.3817633544179, 1506.5840949179556);
    points += QPointF(412.2569633659944, 1501.9278778733042);
    points += QPointF(371.1726090594818, 1494.7996421331377);
    points += QPointF(329.10486652513146, 1485.2032897193571);
    points += QPointF(286.0299018531947, 1473.1427226538626);
    points += QPointF(241.92388113392275, 1458.6218429585542);
    points += QPointF(196.762970457567, 1441.644552655333);
    points += QPointF(150.52333591437866, 1422.2147537660987);
    points += QPointF(103.18114359460913, 1400.3363483127525);
    points += QPointF(54.712559588509635, 1376.013238317194);
    points += QPointF(30.0, 1362.8345196850394);
    points += QPointF(-7.795275590551185, 1173.8581417322835);
    points += QPointF(-7.795275590551185, 5.415841095792075);
    points += QPointF(61.10102183090257, -0.7096412239110919);
    points += QPointF(70.80478764466314, 53.4304855617782);
    points += QPointF(79.62189394925582, 91.85820794938732);
    points += QPointF(90.24647240417782, 128.2383365484458);
    points += QPointF(102.59852571928553, 162.80689648286);
    points += QPointF(116.56115662664025, 195.6221419778608);
    points += QPointF(132.02082032795917, 226.74055755921066);
    points += QPointF(148.86669418977533, 256.2159967948307);
    points += QPointF(166.98986206810577, 284.0990676177162);
    points += QPointF(186.28241554481536, 310.43680926354347);
    points += QPointF(206.63657487261227, 335.2726596298501);
    points += QPointF(227.94391425163948, 358.6466726486652);
    points += QPointF(250.09474647931083, 380.59592025988565);
    points += QPointF(272.9776896673481, 401.15500478550797);
    points += QPointF(296.4794106724578, 420.3566124641626);
    points += QPointF(320.484520119942, 438.23205271508306);
    points += QPointF(344.8755832173259, 454.8117451133494);
    points += QPointF(369.5332075351913, 470.1256329571135);
    points += QPointF(394.3361710694345, 484.2035162296996);
    points += QPointF(419.16155857757695, 497.0753067992062);
    points += QPointF(443.88487919288957, 508.7712150476781);
    points += QPointF(468.38014199619704, 519.3218805863685);
    points += QPointF(492.51986728337727, 528.7584613476225);
    points += QPointF(516.1750084818626, 537.1126962804666);
    points += QPointF(539.1179506668893, 544.3862697375826);
    points += QPointF(572.3449900777376, 553.5656597660253);
    points += QPointF(613.1894237885367, 562.2944254702461);
    points += QPointF(648.9334985308346, 567.4292093937121);
    points += QPointF(678.66913409911, 569.356067120192);
    points += QPointF(690.5719737333277, 569.1428122736005);
    points += QPointF(705.3925728033803, 568.7451994046356);
    points += QPointF(735.4747135391233, 569.5340065404794);
    points += QPointF(765.5663997881065, 572.1930384040452);
    points += QPointF(795.4123023513292, 576.6410528859776);
    points += QPointF(824.965602108301, 582.8015441033587);
    points += QPointF(854.1835470423481, 590.5983253180048);
    points += QPointF(883.0268449918112, 599.9562219974732);
    points += QPointF(911.4589877868528, 610.8015283739601);
    points += QPointF(939.445585340395, 623.0622581751827);
    points += QPointF(966.9537607342976, 636.6682333974711);
    points += QPointF(993.9516330911679, 651.551057077312);
    points += QPointF(1020.4078966730021, 667.6440112755474);
    points += QPointF(1046.2914925169705, 684.8819135818707);
    points += QPointF(1071.5713620583545, 703.2009569179793);
    points += QPointF(1096.216269205861, 722.5385497176682);
    points += QPointF(1120.1946768785947, 742.8331673228507);
    points += QPointF(1143.4746649906995, 764.0242207567912);
    points += QPointF(1166.0238785179056, 786.0519457597056);
    points += QPointF(1187.809496086383, 808.8573128063048);
    points += QPointF(1208.7982112227505, 832.3819574965946);
    points += QPointF(1228.9562198237943, 856.5681299364861);
    points += QPointF(1248.2492085181873, 881.3586613164374);
    points += QPointF(1266.6423393728555, 906.696945663311);
    points += QPointF(1284.1002268781353, 932.5269345592678);
    points += QPointF(1300.5869033558602, 958.7931423679257);
    points += QPointF(1316.065768928307, 985.4406590911037);
    points += QPointF(1330.4995219936793, 1012.4151672497809);
    points += QPointF(1343.850065889632, 1039.6629580676176);
    points += QPointF(1356.0783871611402, 1067.1309405095865);
    points += QPointF(1367.1444008135065, 1094.7666342875323);
    points += QPointF(1377.006758382479, 1122.5181345255592);
    points += QPointF(1385.3821076646018, 1149.5575609882246);
    points += QPointF(1385.7580931557095, 1150.8527362570703);
    points += QPointF(1391.2089508950362, 1170.962993556315);

    return points;
}
