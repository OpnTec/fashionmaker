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

    auto ASSERT_TEST_CASE = [this](const char *title, const QString &input, const QString &output, qreal width)
    {
        QVector<VSAPoint> inputPoints;
        AbstractTest::VectorFromJson(input, inputPoints);

        QVector<QPointF> outputPoints;
        AbstractTest::VectorFromJson(output, outputPoints);

        QTest::newRow(title) << inputPoints << width << outputPoints;
    };

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
#if !defined(Q_PROCESSOR_S390) && !defined(Q_PROCESSOR_S390_X)
    QTest::newRow("Issue 298. Case1") << InputPointsIssue298Case1()
                                      << 75.59055118110237 // seam allowance width
                                      << OutputPointsIssue298Case1();
#endif

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

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 1") << InputPointsIssue923Test1()
                                       << 37.795275590551185 // seam allowance width (1.0 cm)
                                       << OutputPointsIssue923Test1();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 2") << InputPointsIssue923Test2()
                                       << 37.795275590551185 // seam allowance width (1.0 cm)
                                       << OutputPointsIssue923Test2();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 3") << InputPointsIssue923Test3()
                                       << 37.795275590551185 // seam allowance width (1.0 cm)
                                       << OutputPointsIssue923Test3();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 4") << InputPointsIssue923Test4()
                                       << 37.795275590551185 // seam allowance width (1.0 cm)
                                       << OutputPointsIssue923Test4();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 5") << InputPointsIssue923Test5()
                                       << 37.795275590551185 // seam allowance width (1.0 cm)
                                       << OutputPointsIssue923Test5();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 6") << InputPointsIssue923Test6()
                                       << 37.795275590551185 // seam allowance width (1.0 cm)
                                       << OutputPointsIssue923Test6();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 1.1") << InputPointsIssue923Test1_1()
                                         << 37.795275590551185 // seam allowance width (1.0 cm)
                                         << OutputPointsIssue923Test1_1();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 2.2") << InputPointsIssue923Test2_2()
                                         << 37.795275590551185 // seam allowance width (1.0 cm)
                                         << OutputPointsIssue923Test2_2();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 3.3") << InputPointsIssue923Test3_3()
                                         << 37.795275590551185 // seam allowance width (1.0 cm)
                                         << OutputPointsIssue923Test3_3();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 4.4") << InputPointsIssue923Test4_4()
                                         << 37.795275590551185 // seam allowance width (1.0 cm)
                                         << OutputPointsIssue923Test4_4();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 5.5") << InputPointsIssue923Test5_5()
                                         << 37.795275590551185 // seam allowance width (1.0 cm)
                                         << OutputPointsIssue923Test5_5();

    // See file src/app/share/collection/bugs/Issue_#923.val
    QTest::newRow("Issue 923. Test 6.6") << InputPointsIssue923Test6_6()
                                         << 37.795275590551185 // seam allowance width (1.0 cm)
                                         << OutputPointsIssue923Test6_6();

    // See file src/app/share/collection/bugs/loop_by_intersection.val
    ASSERT_TEST_CASE("Loop for angle by intersection",
                     QStringLiteral("://loop_by_intersection/input.json"),
                     QStringLiteral("://loop_by_intersection/output.json"),
                     39.685039370078741 /*seam allowance width (1.05 cm)*/);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::EquidistantRemoveLoop() const
{
    QFETCH(QVector<VSAPoint>, points);
    QFETCH(qreal, width);
    QFETCH(QVector<QPointF>, ekvOrig);

    const QVector<QPointF> ekv = VAbstractPiece::Equidistant(points, width, QString());

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

    points += QPointF(30.000000000000000, 970.393574803149590);
    points += QPointF(30.000000000000000, 2.202377824282394);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.746054423686445, -0.275895198174861);
    points += QPointF(119.502088280527815, -5.192452541281906);
    points += QPointF(148.425957227782732, -10.854963023883784);
    points += QPointF(167.556696145037904, -15.490385095876569);
    points += QPointF(186.612816424205562, -20.944957847774820);
    points += QPointF(205.319505657476242, -27.211791809853210);
    points += QPointF(223.517364434872150, -34.306381867523655);
    points += QPointF(241.050346988465208, -42.238453507357690);
    points += QPointF(257.767987517066160, -51.011647735560771);
    points += QPointF(273.528559017665714, -60.623983162339158);
    points += QPointF(284.944912181261145, -68.750463215889098);
    points += QPointF(556.951317720606994, -31.220946854728947);
    points += QPointF(539.610114105118896, 79.891315777781244);
    points += QPointF(585.916731584533181, -20.702420721823501);
    points += QPointF(1117.455910115990264, 212.663441880296688);
    points += QPointF(1101.848204047899571, 244.808279852815701);
    points += QPointF(1080.077560308018974, 292.371832556342895);
    points += QPointF(1061.808902544771854, 335.542929087147968);
    points += QPointF(1046.601368935565233, 374.798996504892784);
    points += QPointF(1034.184360344932202, 410.238463859529304);
    points += QPointF(1024.281900134065609, 441.966559264544458);
    points += QPointF(1016.612810319859591, 470.098751479176428);
    points += QPointF(1010.891548127258943, 494.765290321737893);
    points += QPointF(1006.830040664839203, 516.117153575137309);
    points += QPointF(1004.140951447516727, 534.333873042145569);
    points += QPointF(1002.542920964105861, 549.634087483703752);
    points += QPointF(1001.809643735979421, 561.617500931810469);
    points += QPointF(1001.632561636561945, 577.019027819914413);
    points += QPointF(1002.598207939668328, 593.497788561921766);
    points += QPointF(1003.041928141414928, 597.870394747796013);
    points += QPointF(1003.192654042237791, 599.136324774537911);
    points += QPointF(1004.471571421798103, 605.713997489163717);
    points += QPointF(1006.667223943402178, 613.597204585595591);
    points += QPointF(1010.141105239932244, 623.236034172292193);
    points += QPointF(1014.847498004049271, 633.833766237447549);
    points += QPointF(1020.666012128351554, 644.742650486572870);
    points += QPointF(1027.424304689066503, 655.429275184752441);
    points += QPointF(1034.913026324476050, 665.462342826411145);
    points += QPointF(1042.902926906874882, 674.515172081394439);
    points += QPointF(1051.171411576988021, 682.376198541660301);
    points += QPointF(1060.187810189582478, 689.465144441703842);
    points += QPointF(1066.614334108267713, 693.625487703081035);
    points += QPointF(1070.121252449300300, 695.602265955357211);
    points += QPointF(1074.383015113466399, 697.709664964041394);
    points += QPointF(1078.726580541966541, 699.565875909043029);
    points += QPointF(1083.206554246492033, 701.187803413440861);
    points += QPointF(1087.891993301368984, 702.585863211688206);
    points += QPointF(1092.864668865585145, 703.760519908174842);
    points += QPointF(1098.216223912539590, 704.699271425243978);
    points += QPointF(1104.044420848225172, 705.374446205546178);
    points += QPointF(1111.348801411629438, 705.793755950627201);
    points += QPointF(1290.270553665008265, 707.499620616224888);
    points += QPointF(1302.416131870576919, 881.655825604373376);
    points += QPointF(30.000000000000000, 970.393574803149590);

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

    points += QPointF(30.000000000000000, 970.393574803149590);
    points += QPointF(30.000000000000000, 2.202377824282394);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.746054423686445, -0.275895198174861);
    points += QPointF(119.502088280527815, -5.192452541281906);
    points += QPointF(148.425957227782732, -10.854963023883784);
    points += QPointF(167.556696145037904, -15.490385095876569);
    points += QPointF(186.612816424205562, -20.944957847774820);
    points += QPointF(205.319505657476242, -27.211791809853210);
    points += QPointF(223.517364434872150, -34.306381867523655);
    points += QPointF(241.050346988465208, -42.238453507357690);
    points += QPointF(257.767987517066160, -51.011647735560771);
    points += QPointF(273.528559017665714, -60.623983162339158);
    points += QPointF(284.495944387625912, -68.430875360872705);
    points += QPointF(334.913131416867543, -61.856202407499914);
    points += QPointF(556.951317720606994, -31.220946854728947);
    points += QPointF(539.610114105118896, 79.891315777781244);
    points += QPointF(585.916731584533181, -20.702420721823501);
    points += QPointF(1117.455910115990264, 212.663441880296688);
    points += QPointF(1101.848204047899571, 244.808279852815701);
    points += QPointF(1080.077560308018974, 292.371832556342895);
    points += QPointF(1061.808902544771854, 335.542929087147968);
    points += QPointF(1046.601368935565233, 374.798996504892784);
    points += QPointF(1034.184360344932202, 410.238463859529304);
    points += QPointF(1024.281900134065609, 441.966559264544458);
    points += QPointF(1016.612810319859591, 470.098751479176428);
    points += QPointF(1010.891548127258943, 494.765290321737893);
    points += QPointF(1006.830040664839203, 516.117153575137309);
    points += QPointF(1004.140951447516727, 534.333873042145569);
    points += QPointF(1002.542920964105861, 549.634087483703752);
    points += QPointF(1001.809643735979421, 561.617500931810469);
    points += QPointF(1001.632561636561945, 577.019027819914413);
    points += QPointF(1002.598207939668328, 593.497788561921766);
    points += QPointF(1003.041928141414928, 597.870394747796013);
    points += QPointF(1003.192654042237791, 599.136324774537911);
    points += QPointF(1004.471571421798103, 605.713997489163717);
    points += QPointF(1006.667223943402178, 613.597204585595591);
    points += QPointF(1010.141105239932244, 623.236034172292193);
    points += QPointF(1014.847498004049271, 633.833766237447549);
    points += QPointF(1020.666012128351554, 644.742650486572870);
    points += QPointF(1027.424304689066503, 655.429275184752441);
    points += QPointF(1034.913026324476050, 665.462342826411145);
    points += QPointF(1042.902926906874882, 674.515172081394439);
    points += QPointF(1051.171411576988021, 682.376198541660301);
    points += QPointF(1060.187810189582478, 689.465144441703842);
    points += QPointF(1066.614334108267713, 693.625487703081035);
    points += QPointF(1070.121252449300300, 695.602265955357211);
    points += QPointF(1074.383015113466399, 697.709664964041394);
    points += QPointF(1078.726580541966541, 699.565875909043029);
    points += QPointF(1083.206554246492033, 701.187803413440861);
    points += QPointF(1087.891993301368984, 702.585863211688206);
    points += QPointF(1092.864668865585145, 703.760519908174842);
    points += QPointF(1098.216223912539590, 704.699271425243978);
    points += QPointF(1104.044420848225172, 705.374446205546178);
    points += QPointF(1111.348801411629438, 705.793755950627201);
    points += QPointF(1290.270553665008265, 707.499620616224888);
    points += QPointF(1302.416131870576919, 881.655825604373376);
    points += QPointF(30.000000000000000, 970.393574803149590);

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

    points += QPointF(30.000000000000000, 970.393574803149590);
    points += QPointF(30.000000000000000, 2.202377824282394);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.746054423686445, -0.275895198174861);
    points += QPointF(119.502088280527815, -5.192452541281906);
    points += QPointF(148.425957227782732, -10.854963023883784);
    points += QPointF(167.556696145037904, -15.490385095876569);
    points += QPointF(186.612816424205562, -20.944957847774820);
    points += QPointF(205.319505657476242, -27.211791809853210);
    points += QPointF(223.517364434872150, -34.306381867523655);
    points += QPointF(241.050346988465208, -42.238453507357690);
    points += QPointF(257.767987517066160, -51.011647735560771);
    points += QPointF(273.528559017665714, -60.623983162339158);
    points += QPointF(288.203122586733002, -71.069748673417564);
    points += QPointF(301.895059431859636, -82.523535465339663);
    points += QPointF(330.086658189795969, -109.702493213952863);
    points += QPointF(334.913131416867600, -61.856202407499907);
    points += QPointF(556.951317720606994, -31.220946854728947);
    points += QPointF(539.610114105118896, 79.891315777781244);
    points += QPointF(585.916731584533181, -20.702420721823501);
    points += QPointF(1117.455910115990264, 212.663441880296688);
    points += QPointF(1101.848204047899571, 244.808279852815701);
    points += QPointF(1080.077560308018974, 292.371832556342895);
    points += QPointF(1061.808902544771854, 335.542929087147968);
    points += QPointF(1046.601368935565233, 374.798996504892784);
    points += QPointF(1034.184360344932202, 410.238463859529304);
    points += QPointF(1024.281900134065609, 441.966559264544458);
    points += QPointF(1016.612810319859591, 470.098751479176428);
    points += QPointF(1010.891548127258943, 494.765290321737893);
    points += QPointF(1006.830040664839203, 516.117153575137309);
    points += QPointF(1004.140951447516727, 534.333873042145569);
    points += QPointF(1002.542920964105861, 549.634087483703752);
    points += QPointF(1001.809643735979421, 561.617500931810469);
    points += QPointF(1001.632561636561945, 577.019027819914413);
    points += QPointF(1002.598207939668328, 593.497788561921766);
    points += QPointF(1003.041928141414928, 597.870394747796013);
    points += QPointF(1003.192654042237791, 599.136324774537911);
    points += QPointF(1004.471571421798103, 605.713997489163717);
    points += QPointF(1006.667223943402178, 613.597204585595591);
    points += QPointF(1010.141105239932244, 623.236034172292193);
    points += QPointF(1014.847498004049271, 633.833766237447549);
    points += QPointF(1020.666012128351554, 644.742650486572870);
    points += QPointF(1027.424304689066503, 655.429275184752441);
    points += QPointF(1034.913026324476050, 665.462342826411145);
    points += QPointF(1042.902926906874882, 674.515172081394439);
    points += QPointF(1051.171411576988021, 682.376198541660301);
    points += QPointF(1060.187810189582478, 689.465144441703842);
    points += QPointF(1066.614334108267713, 693.625487703081035);
    points += QPointF(1070.121252449300300, 695.602265955357211);
    points += QPointF(1074.383015113466399, 697.709664964041394);
    points += QPointF(1078.726580541966541, 699.565875909043029);
    points += QPointF(1083.206554246492033, 701.187803413440861);
    points += QPointF(1087.891993301368984, 702.585863211688206);
    points += QPointF(1092.864668865585145, 703.760519908174842);
    points += QPointF(1098.216223912539590, 704.699271425243978);
    points += QPointF(1104.044420848225172, 705.374446205546178);
    points += QPointF(1111.348801411629438, 705.793755950627201);
    points += QPointF(1290.270553665008265, 707.499620616224888);
    points += QPointF(1302.416131870576919, 881.655825604373376);
    points += QPointF(30.000000000000000, 970.393574803149590);

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

    points += QPointF(30.000000000000000, 970.393574803149590);
    points += QPointF(30.000000000000000, 2.202377824282394);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.746054423686445, -0.275895198174861);
    points += QPointF(119.502088280527815, -5.192452541281906);
    points += QPointF(148.425957227782732, -10.854963023883784);
    points += QPointF(167.556696145037904, -15.490385095876569);
    points += QPointF(186.612816424205562, -20.944957847774820);
    points += QPointF(205.319505657476242, -27.211791809853210);
    points += QPointF(223.517364434872150, -34.306381867523655);
    points += QPointF(241.050346988465208, -42.238453507357690);
    points += QPointF(257.767987517066160, -51.011647735560771);
    points += QPointF(273.528559017665714, -60.623983162339158);
    points += QPointF(286.074909490746791, -69.554826961869111);
    points += QPointF(556.951317720606994, -31.220946854728947);
    points += QPointF(539.610114105118896, 79.891315777781244);
    points += QPointF(585.916731584533181, -20.702420721823501);
    points += QPointF(1117.455910115990264, 212.663441880296688);
    points += QPointF(1101.848204047899571, 244.808279852815701);
    points += QPointF(1080.077560308018974, 292.371832556342895);
    points += QPointF(1061.808902544771854, 335.542929087147968);
    points += QPointF(1046.601368935565233, 374.798996504892784);
    points += QPointF(1034.184360344932202, 410.238463859529304);
    points += QPointF(1024.281900134065609, 441.966559264544458);
    points += QPointF(1016.612810319859591, 470.098751479176428);
    points += QPointF(1010.891548127258943, 494.765290321737893);
    points += QPointF(1006.830040664839203, 516.117153575137309);
    points += QPointF(1004.140951447516727, 534.333873042145569);
    points += QPointF(1002.542920964105861, 549.634087483703752);
    points += QPointF(1001.809643735979421, 561.617500931810469);
    points += QPointF(1001.632561636561945, 577.019027819914413);
    points += QPointF(1002.598207939668328, 593.497788561921766);
    points += QPointF(1003.041928141414928, 597.870394747796013);
    points += QPointF(1003.192654042237791, 599.136324774537911);
    points += QPointF(1004.471571421798103, 605.713997489163717);
    points += QPointF(1006.667223943402178, 613.597204585595591);
    points += QPointF(1010.141105239932244, 623.236034172292193);
    points += QPointF(1014.847498004049271, 633.833766237447549);
    points += QPointF(1020.666012128351554, 644.742650486572870);
    points += QPointF(1027.424304689066503, 655.429275184752441);
    points += QPointF(1034.913026324476050, 665.462342826411145);
    points += QPointF(1042.902926906874882, 674.515172081394439);
    points += QPointF(1051.171411576988021, 682.376198541660301);
    points += QPointF(1060.187810189582478, 689.465144441703842);
    points += QPointF(1066.614334108267713, 693.625487703081035);
    points += QPointF(1070.121252449300300, 695.602265955357211);
    points += QPointF(1074.383015113466399, 697.709664964041394);
    points += QPointF(1078.726580541966541, 699.565875909043029);
    points += QPointF(1083.206554246492033, 701.187803413440861);
    points += QPointF(1087.891993301368984, 702.585863211688206);
    points += QPointF(1092.864668865585145, 703.760519908174842);
    points += QPointF(1098.216223912539590, 704.699271425243978);
    points += QPointF(1104.044420848225172, 705.374446205546178);
    points += QPointF(1111.348801411629438, 705.793755950627201);
    points += QPointF(1290.270553665008265, 707.499620616224888);
    points += QPointF(1302.416131870576919, 881.655825604373376);
    points += QPointF(30.000000000000000, 970.393574803149590);

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

    points += QPointF(30.000000000000000, 970.393574803149590);
    points += QPointF(30.000000000000000, 2.202377824282394);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.746054423686445, -0.275895198174861);
    points += QPointF(119.502088280527815, -5.192452541281906);
    points += QPointF(148.425957227782732, -10.854963023883784);
    points += QPointF(167.556696145037904, -15.490385095876569);
    points += QPointF(186.612816424205562, -20.944957847774820);
    points += QPointF(205.319505657476242, -27.211791809853210);
    points += QPointF(223.517364434872150, -34.306381867523655);
    points += QPointF(241.050346988465208, -42.238453507357690);
    points += QPointF(257.767987517066160, -51.011647735560771);
    points += QPointF(273.528559017665714, -60.623983162339158);
    points += QPointF(288.203122586733002, -71.069748673417564);
    points += QPointF(301.895059431859636, -82.523535465339663);
    points += QPointF(308.680986198689595, -89.065712035901640);
    points += QPointF(334.913131416867543, -61.856202407499914);
    points += QPointF(556.951317720606994, -31.220946854728947);
    points += QPointF(539.610114105118896, 79.891315777781244);
    points += QPointF(585.916731584533181, -20.702420721823501);
    points += QPointF(1117.455910115990264, 212.663441880296688);
    points += QPointF(1101.848204047899571, 244.808279852815701);
    points += QPointF(1080.077560308018974, 292.371832556342895);
    points += QPointF(1061.808902544771854, 335.542929087147968);
    points += QPointF(1046.601368935565233, 374.798996504892784);
    points += QPointF(1034.184360344932202, 410.238463859529304);
    points += QPointF(1024.281900134065609, 441.966559264544458);
    points += QPointF(1016.612810319859591, 470.098751479176428);
    points += QPointF(1010.891548127258943, 494.765290321737893);
    points += QPointF(1006.830040664839203, 516.117153575137309);
    points += QPointF(1004.140951447516727, 534.333873042145569);
    points += QPointF(1002.542920964105861, 549.634087483703752);
    points += QPointF(1001.809643735979421, 561.617500931810469);
    points += QPointF(1001.632561636561945, 577.019027819914413);
    points += QPointF(1002.598207939668328, 593.497788561921766);
    points += QPointF(1003.041928141414928, 597.870394747796013);
    points += QPointF(1003.192654042237791, 599.136324774537911);
    points += QPointF(1004.471571421798103, 605.713997489163717);
    points += QPointF(1006.667223943402178, 613.597204585595591);
    points += QPointF(1010.141105239932244, 623.236034172292193);
    points += QPointF(1014.847498004049271, 633.833766237447549);
    points += QPointF(1020.666012128351554, 644.742650486572870);
    points += QPointF(1027.424304689066503, 655.429275184752441);
    points += QPointF(1034.913026324476050, 665.462342826411145);
    points += QPointF(1042.902926906874882, 674.515172081394439);
    points += QPointF(1051.171411576988021, 682.376198541660301);
    points += QPointF(1060.187810189582478, 689.465144441703842);
    points += QPointF(1066.614334108267713, 693.625487703081035);
    points += QPointF(1070.121252449300300, 695.602265955357211);
    points += QPointF(1074.383015113466399, 697.709664964041394);
    points += QPointF(1078.726580541966541, 699.565875909043029);
    points += QPointF(1083.206554246492033, 701.187803413440861);
    points += QPointF(1087.891993301368984, 702.585863211688206);
    points += QPointF(1092.864668865585145, 703.760519908174842);
    points += QPointF(1098.216223912539590, 704.699271425243978);
    points += QPointF(1104.044420848225172, 705.374446205546178);
    points += QPointF(1111.348801411629438, 705.793755950627201);
    points += QPointF(1290.270553665008265, 707.499620616224888);
    points += QPointF(1302.416131870576919, 881.655825604373376);
    points += QPointF(30.000000000000000, 970.393574803149590);

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

    points += QPointF(30.000000000000000, 970.393574803149590);
    points += QPointF(30.000000000000000, 2.202377824282394);
    points += QPointF(46.111397855976776, 2.027726815668483);
    points += QPointF(81.746054423686445, -0.275895198174861);
    points += QPointF(119.502088280527815, -5.192452541281906);
    points += QPointF(148.425957227782732, -10.854963023883784);
    points += QPointF(167.556696145037904, -15.490385095876569);
    points += QPointF(186.612816424205562, -20.944957847774820);
    points += QPointF(205.319505657476242, -27.211791809853210);
    points += QPointF(223.517364434872150, -34.306381867523655);
    points += QPointF(241.050346988465208, -42.238453507357690);
    points += QPointF(257.767987517066160, -51.011647735560771);
    points += QPointF(273.528559017665714, -60.623983162339158);
    points += QPointF(284.495944387625912, -68.430875360872705);
    points += QPointF(556.951317720606994, -31.220946854728947);
    points += QPointF(539.610114105118896, 79.891315777781244);
    points += QPointF(585.916731584533181, -20.702420721823501);
    points += QPointF(1117.455910115990264, 212.663441880296688);
    points += QPointF(1101.848204047899571, 244.808279852815701);
    points += QPointF(1080.077560308018974, 292.371832556342895);
    points += QPointF(1061.808902544771854, 335.542929087147968);
    points += QPointF(1046.601368935565233, 374.798996504892784);
    points += QPointF(1034.184360344932202, 410.238463859529304);
    points += QPointF(1024.281900134065609, 441.966559264544458);
    points += QPointF(1016.612810319859591, 470.098751479176428);
    points += QPointF(1010.891548127258943, 494.765290321737893);
    points += QPointF(1006.830040664839203, 516.117153575137309);
    points += QPointF(1004.140951447516727, 534.333873042145569);
    points += QPointF(1002.542920964105861, 549.634087483703752);
    points += QPointF(1001.809643735979421, 561.617500931810469);
    points += QPointF(1001.632561636561945, 577.019027819914413);
    points += QPointF(1002.598207939668328, 593.497788561921766);
    points += QPointF(1003.041928141414928, 597.870394747796013);
    points += QPointF(1003.192654042237791, 599.136324774537911);
    points += QPointF(1004.471571421798103, 605.713997489163717);
    points += QPointF(1006.667223943402178, 613.597204585595591);
    points += QPointF(1010.141105239932244, 623.236034172292193);
    points += QPointF(1014.847498004049271, 633.833766237447549);
    points += QPointF(1020.666012128351554, 644.742650486572870);
    points += QPointF(1027.424304689066503, 655.429275184752441);
    points += QPointF(1034.913026324476050, 665.462342826411145);
    points += QPointF(1042.902926906874882, 674.515172081394439);
    points += QPointF(1051.171411576988021, 682.376198541660301);
    points += QPointF(1060.187810189582478, 689.465144441703842);
    points += QPointF(1066.614334108267713, 693.625487703081035);
    points += QPointF(1070.121252449300300, 695.602265955357211);
    points += QPointF(1074.383015113466399, 697.709664964041394);
    points += QPointF(1078.726580541966541, 699.565875909043029);
    points += QPointF(1083.206554246492033, 701.187803413440861);
    points += QPointF(1087.891993301368984, 702.585863211688206);
    points += QPointF(1092.864668865585145, 703.760519908174842);
    points += QPointF(1098.216223912539590, 704.699271425243978);
    points += QPointF(1104.044420848225172, 705.374446205546178);
    points += QPointF(1111.348801411629438, 705.793755950627201);
    points += QPointF(1290.270553665008265, 707.499620616224888);
    points += QPointF(1302.416131870576919, 881.655825604373376);
    points += QPointF(30.000000000000000, 970.393574803149590);

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
    points += QPointF(445.748031496063049, 2.204598425196849);
    points += QPointF(445.748031496062993, 251.484462700721508);
    points += QPointF(420.167249883446971, 265.344673769224812);
    points += QPointF(408.168687600939791, 270.749698109742667);
    points += QPointF(395.757434861670447, 275.068217221903126);
    points += QPointF(383.280158875837515, 278.216263504481446);
    points += QPointF(370.828801869607219, 280.240944580682424);
    points += QPointF(358.240833427700807, 281.223585390784933);
    points += QPointF(340.128405605847377, 280.950325398745633);
    points += QPointF(317.100367222628677, 277.461014499073372);
    points += QPointF(295.151952890624273, 271.084708546139382);
    points += QPointF(274.688712908028663, 262.419614890377773);
    points += QPointF(255.813104536205742, 251.942354407911466);
    points += QPointF(238.610817650068327, 240.050870330341809);
    points += QPointF(223.168378143141126, 227.069612826995268);
    points += QPointF(209.577497139717309, 213.238868162572516);
    points += QPointF(197.938119214048157, 198.672641760685593);
    points += QPointF(195.608727121505524, 194.913640433289544);
    points += QPointF(185.710682244992483, 196.880382798455116);
    points += QPointF(173.809356654777872, 198.035763353667477);
    points += QPointF(156.584623464047553, 197.972079562468082);
    points += QPointF(133.847493016660877, 194.874025486507207);
    points += QPointF(111.245648921419928, 188.996325361642533);
    points += QPointF(89.230166381208790, 180.747600513519643);
    points += QPointF(68.084222734525255, 170.367865812562059);
    points += QPointF(48.147862270344362, 157.996030361007911);
    points += QPointF(29.800888286137585, 143.652949766683832);
    points += QPointF(13.683525778751441, 127.384450781512541);
    points += QPointF(3.399493823857615, 113.515501757126160);
    points += QPointF(-2.688329116224859, 103.081091306820881);
    points += QPointF(-7.591302295481950, 92.078476678883646);
    points += QPointF(-11.187498192466251, 80.235473513717892);
    points += QPointF(-13.151329445248951, 67.623242575048394);
    points += QPointF(-13.149174973239838, 54.474730078465917);
    points += QPointF(-10.943375405772395, 41.194056317965945);
    points += QPointF(-6.654890290296288, 28.745230957359055);
    points += QPointF(6.735602489575015, 2.204598425196849);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsCase3() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(30.000000000000000, 39.999874015748034);
    points += VSAPoint(407.952755905511822, 39.999874015748034);
    points += VSAPoint(407.952755905511822, 228.976251968503959);
    points += VSAPoint(407.952755905511822, 228.976251968503959);

    VSAPoint p = VSAPoint(404.672301299224387, 235.656234316977816);
    p.SetSABefore(36.9778);
    p.SetSAAfter(36.9778);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(397.222833003011260, 246.532690980461950);
    p.SetSABefore(35.5298);
    p.SetSAAfter(35.5298);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(388.920624996308561, 254.595637637351388);
    p.SetSABefore(34.2586);
    p.SetSAAfter(34.2586);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(380.002934156756680, 260.030296102714601);
    p.SetSABefore(33.1115);
    p.SetSAAfter(33.1115);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(370.707017361996236, 263.021888191619951);
    p.SetSABefore(32.0389);
    p.SetSAAfter(32.0389);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(361.270131489667733, 263.755635719135853);
    p.SetSABefore(30.9992);
    p.SetSAAfter(30.9992);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(351.929533417411790, 262.416760500330838);
    p.SetSABefore(29.9628);
    p.SetSAAfter(29.9628);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(342.922480022868967, 259.190484350273266);
    p.SetSABefore(28.9119);
    p.SetSAAfter(28.9119);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(334.486228183679657, 254.262029084031553);
    p.SetSABefore(27.8387);
    p.SetSAAfter(27.8387);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(326.858034777484477, 247.816616516674173);
    p.SetSABefore(26.7418);
    p.SetSAAfter(26.7418);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(320.275156681923932, 240.039468463269486);
    p.SetSABefore(25.6226);
    p.SetSAAfter(25.6226);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(314.974850774638639, 231.115806738886022);
    p.SetSABefore(24.4826);
    p.SetSAAfter(24.4826);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(311.194373933269048, 221.230853158592197);
    p.SetSABefore(23.3201);
    p.SetSAAfter(23.3201);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(309.170983035455720, 210.569829537456371);
    p.SetSABefore(22.1282);
    p.SetSAAfter(22.1282);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(309.141934958839045, 199.317957690547019);
    p.SetSABefore(20.8923);
    p.SetSAAfter(20.8923);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(311.344486581059869, 187.660459432932583);
    p.SetSABefore(19.5892);
    p.SetSAAfter(19.5892);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(313.464566929134321, 181.732157480315578);
    p.SetSABefore(18.8976);
    p.SetSAAfter(18.8976);
    points += p;

    p = VSAPoint(313.464566929134321, 181.732157480315578);
    p.SetSABefore(18.8976);
    p.SetSAAfter(18.8976);
    points += p;

    p = VSAPoint(313.464566929134321, 181.732157480315578);
    p.SetSABefore(18.8976);
    p.SetSAAfter(18.8976);
    points += p;

    p = VSAPoint(316.591345021592019, 173.284435565430954);
    p.SetSABefore(19.9768);
    p.SetSAAfter(19.9768);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(320.142638433108914, 158.635688769582430);
    p.SetSABefore(21.7825);
    p.SetSAAfter(21.7825);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(320.765122974842939, 146.512865040203337);
    p.SetSABefore(23.2367);
    p.SetSAAfter(23.2367);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(318.807583600455132, 136.726949517032153);
    p.SetSABefore(24.4322);
    p.SetSAAfter(24.4322);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(314.618805263606987, 129.088927339807270);
    p.SetSABefore(25.4758);
    p.SetSAAfter(25.4758);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(308.547572917959542, 123.409783648267165);
    p.SetSABefore(26.4717);
    p.SetSAAfter(26.4717);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(300.942671517174290, 119.500503582150259);
    p.SetSABefore(27.4961);
    p.SetSAAfter(27.4961);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(292.152886014912497, 117.172072281195000);
    p.SetSABefore(28.5854);
    p.SetSAAfter(28.5854);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(282.527001364835201, 116.235474885139837);
    p.SetSABefore(29.744);
    p.SetSAAfter(29.744);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(272.413802520603895, 116.501696533723177);
    p.SetSABefore(30.956);
    p.SetSAAfter(30.956);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(257.045176202602249, 118.651559471725193);
    p.SetSABefore(32.815);
    p.SetSAAfter(32.815);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(238.080312793150540, 124.132738713584303);
    p.SetSABefore(35.18);
    p.SetSAAfter(35.18);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(223.444321772985404, 131.022926649289388);
    p.SetSABefore(37.1179);
    p.SetSAAfter(37.1179);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(218.976377952756053, 134.488062992126459);
    points += VSAPoint(218.976377952756053, 134.488062992126459);
    points += VSAPoint(218.976377952756053, 134.488062992126459);

    p = VSAPoint(209.413110094717808, 142.833471271341665);
    p.SetSABefore(35.5678);
    p.SetSAAfter(35.5678);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(193.391595601190147, 154.054987746786708);
    p.SetSABefore(32.1352);
    p.SetSAAfter(32.1352);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(183.690040040596074, 158.450531399325257);
    p.SetSABefore(30.266);
    p.SetSAAfter(30.266);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(178.111093559349001, 159.860599171825072);
    p.SetSABefore(29.2562);
    p.SetSAAfter(29.2562);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(173.237617815947488, 160.079619284747537);
    p.SetSABefore(28.4001);
    p.SetSAAfter(28.4001);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(169.017046385061462, 159.212182985841025);
    p.SetSABefore(27.6439);
    p.SetSAAfter(27.6439);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(165.396812841360941, 157.362881522853911);
    p.SetSABefore(26.9305);
    p.SetSAAfter(26.9305);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(162.324350759515880, 154.636306143534711);
    p.SetSABefore(26.2096);
    p.SetSAAfter(26.2096);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(158.575695647501561, 149.207322310502946);
    p.SetSABefore(25.0519);
    p.SetSAAfter(25.0519);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(155.086603150983990, 139.745623026852769);
    p.SetSABefore(23.2821);
    p.SetSAAfter(23.2821);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(152.842400845947793, 128.657470537345603);
    p.SetSABefore(21.2968);
    p.SetSAAfter(21.2968);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(150.866158843535715, 110.747817409061298);
    p.SetSABefore(18.1347);
    p.SetSAAfter(18.1347);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(149.373821053326026, 94.001593653555375);
    p.SetSABefore(15.1843);
    p.SetSAAfter(15.1843);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(147.903865487813732, 84.774928160493630);
    p.SetSABefore(13.5447);
    p.SetSAAfter(13.5447);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(145.576143100581845, 78.105459371511586);
    p.SetSABefore(12.305);
    p.SetSAAfter(12.305);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(142.964662887243179, 75.410534564509078);
    p.SetSABefore(11.6464);
    p.SetSAAfter(11.6464);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(140.763228387362460, 74.778132056190998);
    p.SetSABefore(11.2445);
    p.SetSAAfter(11.2445);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(136.665272250398885, 75.785031833736099);
    p.SetSABefore(10.5039);
    p.SetSAAfter(10.5039);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(129.241060982167369, 81.807533048917350);
    p.SetSABefore(8.82629);
    p.SetSAAfter(8.82629);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(124.488188976378183, 87.243968503937310);
    p.SetSABefore(7.55906);
    points += p;

    p = VSAPoint(124.488188976378183, 87.243968503937310);
    p.SetSABefore(7.55906);
    points += p;

    p = VSAPoint(124.488188976378183, 87.243968503937310);
    p.SetSABefore(7.55906);
    points += p;

    p = VSAPoint(118.347357299687616, 94.279895957313556);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(106.583151413161048, 104.824701818694905);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(95.475737670842378, 111.519402254765254);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(85.064941832712051, 114.788834409007222);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(75.390589658750542, 115.057835424903487);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(66.492506908938267, 112.751242445936697);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(58.410519343255714, 108.293892615589485);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(51.184452721683314, 102.110623077344513);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(44.854132804201512, 94.626270974684417);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(39.459385350790782, 86.265673451091871);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(33.079189282762393, 72.987917323101186);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(28.381038280132728, 56.107295472113094);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(27.913136644118886, 46.188304541034846);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(28.968164934724022, 41.491814504983139);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(30.000000000000000, 39.999874015748034);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsCase3() const
{
    QVector<QPointF> points;

    points += QPointF(10.185624773347756, 2.204598425196849);
    points += QPointF(445.748031496063049, 2.204598425196849);
    points += QPointF(445.748031496062993, 239.543708774549259);
    points += QPointF(434.277249951658803, 257.602527551798858);
    points += QPointF(421.704109111683636, 272.232473108416229);
    points += QPointF(406.821528061699894, 283.815805783943347);
    points += QPointF(390.426869009588813, 291.492454813390282);
    points += QPointF(373.431680691615043, 294.972061096528250);
    points += QPointF(356.889253932553629, 294.445762616291631);
    points += QPointF(341.592611223119548, 290.511019644904309);
    points += QPointF(327.941579548832294, 283.860197889278709);
    points += QPointF(316.076627506977388, 275.060808621685567);
    points += QPointF(306.056835471849411, 264.515338115471252);
    points += QPointF(297.967063959684765, 252.511650805464001);
    points += QPointF(291.957956690120341, 239.289448398975935);
    points += QPointF(288.245701475112810, 225.099827304036467);
    points += QPointF(287.082912046801141, 210.253300532205827);
    points += QPointF(288.703123507393855, 195.143319989158869);
    points += QPointF(293.166186492321515, 180.515626372699529);
    points += QPointF(295.670561165402944, 175.368662126210381);
    points += QPointF(297.032836005697561, 169.787192181154381);
    points += QPointF(298.427368810404460, 158.085249530220608);
    points += QPointF(297.942387105948967, 150.956429005150937);
    points += QPointF(296.912819957584588, 147.814504880960072);
    points += QPointF(296.397338085742149, 147.093749248063460);
    points += QPointF(295.959645090416700, 146.772731325564138);
    points += QPointF(294.211098898799321, 146.123357146812708);
    points += QPointF(290.481043050038522, 145.598758393629993);
    points += QPointF(285.006359018652802, 145.720508052838397);
    points += QPointF(278.586587836716035, 146.661545108162670);
    points += QPointF(267.611351780445432, 149.560217090466921);
    points += QPointF(254.258155566962188, 155.214998010181603);
    points += QPointF(246.441613424006931, 160.107097914151410);
    points += QPointF(243.102668926984364, 163.402837598685920);
    points += QPointF(225.939985502865284, 173.778976885805150);
    points += QPointF(204.126745997020492, 183.975693844107269);
    points += QPointF(188.539896208493161, 187.983698484313294);
    points += QPointF(177.315121486754521, 188.814064306647623);
    points += QPointF(166.192228902931021, 187.373339931453700);
    points += QPointF(155.574422378957252, 183.176406510679953);
    points += QPointF(146.729251284035655, 176.508054524108871);
    points += QPointF(140.168555897046843, 168.248262793059951);
    points += QPointF(134.853718310273564, 156.677629794996079);
    points += QPointF(132.208495509666591, 142.105162309613490);
    points += QPointF(131.967439246091402, 132.827656859895768);
    points += QPointF(129.151336777046254, 135.351860712520221);
    points += QPointF(111.113552824916056, 146.223658854871644);
    points += QPointF(91.371995199602523, 152.423346590426377);
    points += QPointF(71.089251560144604, 152.987320130573352);
    points += QPointF(52.404520155294485, 148.143796926294669);
    points += QPointF(36.773898357312952, 139.523250451403811);
    points += QPointF(24.295634363907766, 128.845730621976628);
    points += QPointF(14.420075278530305, 117.169833926538999);
    points += QPointF(6.418159077086557, 104.768734272561886);
    points += QPointF(-2.435562868156214, 86.343350544456428);
    points += QPointF(-9.171676817365769, 62.140256123241244);
    points += QPointF(-10.080338624527474, 42.877642935055057);
    points += QPointF(-6.297329478268652, 26.037464411020956);
    points += QPointF(10.185624773347756, 2.204598425196849);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue627() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(11565.008125001964800, -71.444885494199326);
    p.SetSAAfter(0);
    points += p;

    p = VSAPoint(11774.053467225081476, -3376.830337135347690);
    p.SetSABefore(0);
    points += p;

    p = VSAPoint(11774.053467225081476, -3376.830337135347690);
    p.SetSABefore(0);
    points += p;

    p = VSAPoint(11821.637653562487685, -3457.444047544760906);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(11916.506852253827674, -3619.698047174160820);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12011.687139013727574, -3784.317013264594607);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12107.923065894336105, -3952.559914581167959);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12205.959184947796530, -4125.685719888986569);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12306.540048226263025, -4304.953397953153399);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12410.410207781878853, -4491.621917538775961);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12518.314215666796372, -4686.950247410958582);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12630.996623933155206, -4892.197356334805590);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12749.201984633113170, -5108.622213075424952);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12937.571227539614483, -5455.181123300273612);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(13216.054288166869810, -5970.204948863398386);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(13530.031095467164050, -6552.601203465501385);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(13704.042216387522785, -6875.648082494774826);
    p.SetSAAfter(0);
    points += p;

    p = VSAPoint(13704.042216387522785, -6875.648082494774826);
    p.SetSAAfter(0);
    points += p;

    p = VSAPoint(13493.259591532772902, -71.444885494204499);
    p.SetSABefore(0);
    points += p;

    p = VSAPoint(13493.259591532772902, -71.444885494204499);
    p.SetSABefore(0);
    points += p;

    p = VSAPoint(13458.265794168308275, -72.452878937007853);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(13303.675181471457108, -76.518823818897630);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(13154.908187746063049, -79.907111220472430);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(13010.905973179134890, -82.617741141732296);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12870.609697957679600, -84.650713582677156);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12732.960522268700515, -86.006028543307096);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12596.899606299211882, -86.683686023622045);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12461.368110236220673, -86.683686023622045);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12325.307194266732040, -86.006028543307096);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12187.658018577756593, -84.650713582677156);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(12047.361743356301304, -82.617741141732296);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(11903.359528789369506, -79.907111220472430);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(11754.592535063975447, -76.518823818897630);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(11600.001922367124280, -72.452878937007853);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(11565.008125001964800, -71.444885494199326);
    p.SetSAAfter(0);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue627() const
{
    QVector<QPointF> points;

    points += QPointF(11561.414612602904526, -14.624946442565696);
    points += QPointF(11738.564365107058620, -2815.683360191556858);
    points += QPointF(11781.963905683212943, -3501.908695340060149);
    points += QPointF(11867.565752333255659, -3648.313692662954054);
    points += QPointF(11962.607394275857587, -3812.694206301280701);
    points += QPointF(12058.712075176787039, -3980.708890064594925);
    points += QPointF(12156.626721853161143, -4153.621262002729964);
    points += QPointF(12257.097609428719807, -4332.693831870464237);
    points += QPointF(12360.870334229162836, -4519.187971963542623);
    points += QPointF(12468.689857584724450, -4714.363921245554593);
    points += QPointF(12581.300599294354470, -4919.480896487609243);
    points += QPointF(12699.446556901319127, -5135.797272217891987);
    points += QPointF(12887.760771573290185, -5482.255186619299820);
    points += QPointF(13166.184881983803280, -5997.170273914794052);
    points += QPointF(13480.128179706836818, -6579.504463988871066);
    points += QPointF(13707.971545534986944, -7002.489438444617917);
    points += QPointF(13491.504179598350674, -14.779021766204096);
    points += QPointF(13456.633436542824711, -15.783470639800996);
    points += QPointF(13302.184595664144581, -19.845509268540340);
    points += QPointF(13153.617296022339360, -23.228896475030687);
    points += QPointF(13009.839001467360504, -25.934868953220956);
    points += QPointF(12869.788271788002021, -27.963751349545099);
    points += QPointF(12732.402342177734681, -29.315863054406840);
    points += QPointF(12596.617248209338868, -29.991475781985521);
    points += QPointF(12461.368110236220673, -29.990772637795267);
    points += QPointF(12325.589552356605054, -29.313818301670572);
    points += QPointF(12188.216198668722427, -27.960548093776900);
    points += QPointF(12048.183169525978883, -25.930778908600239);
    points += QPointF(11904.426500501143892, -23.224239031961091);
    points += QPointF(11755.883426787699136, -19.840609073455887);
    points += QPointF(11601.492508174436807, -15.779564386650563);
    points += QPointF(11561.414612602904526, -14.624946442565696);

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

    points += QPointF(-3011.051754879010332, -958.321944400611301);
    points += QPointF(-2875.704655596311113, -988.873180340485987);
    points += QPointF(-2870.266139694495905, -995.294853951866457);
    points += QPointF(-2852.541502402287279, -1017.672534874126313);
    points += QPointF(-2834.561736510239371, -1042.007683734111424);
    points += QPointF(-2816.318152801346969, -1068.219702443143433);
    points += QPointF(-2788.352030367969292, -1110.859883556803652);
    points += QPointF(-2750.104578133508312, -1173.706685440264437);
    points += QPointF(-2709.576301328694626, -1242.298707376999573);
    points += QPointF(-2667.685724118163762, -1313.999219737975636);
    points += QPointF(-2603.306990389053681, -1428.314629665485654);
    points += QPointF(-2559.715942725519653, -1507.018455323058788);
    points += QPointF(-2514.227494695693167, -1585.119667416120365);
    points += QPointF(-2449.158395429499251, -1698.662018272644673);
    points += QPointF(-2408.901620525934959, -1771.183767809588062);
    points += QPointF(-2384.845551286147384, -1816.215840001470042);
    points += QPointF(-2370.880345941558971, -1841.237056353140815);
    points += QPointF(-2336.479706267199163, -1906.269233768278809);
    points += QPointF(-2314.171878902582193, -1952.123894247253929);
    points += QPointF(-2300.997366939366657, -1981.215654350529348);
    points += QPointF(-2289.299963085878971, -2009.119815234373618);
    points += QPointF(-2279.153348206748888, -2035.802573282930325);
    points += QPointF(-2276.421885291452327, -2043.762124623026239);
    points += QPointF(-2319.478871757543402, -2175.664782739318980);
    points += QPointF(-2117.171449255143671, -2194.168101599941110);
    points += QPointF(-2068.215402990954772, -2065.277909112684483);
    points += QPointF(-1952.636614028386703, -1890.801039106245071);
    points += QPointF(-1917.586670824047815, -1837.889915684416337);
    points += QPointF(-2058.560149041760724, -1695.491664502713775);
    points += QPointF(-2088.680400307789114, -1664.468919112514186);
    points += QPointF(-2140.427345188782056, -1608.621584484509185);
    points += QPointF(-2176.442570235218682, -1567.900890740904742);
    points += QPointF(-2212.334254034449714, -1525.262668863119416);
    points += QPointF(-2247.234202528754395, -1481.174098429168680);
    points += QPointF(-2279.766629673790248, -1436.693272413116574);
    points += QPointF(-2301.715318706475500, -1403.691786696854933);
    points += QPointF(-2315.030637739082067, -1382.066893805181053);
    points += QPointF(-2321.200039954151634, -1371.365087899206856);
    points += QPointF(-2327.092357559710763, -1360.835222350091499);
    points += QPointF(-2338.849983413490008, -1338.325403429812241);
    points += QPointF(-2355.959416743849943, -1302.574734893039931);
    points += QPointF(-2377.510297026410626, -1251.855288345610234);
    points += QPointF(-2397.512032044217449, -1199.303037736062379);
    points += QPointF(-2415.844466989297416, -1146.377438809215619);
    points += QPointF(-2432.371872429092036, -1094.604132032660573);
    points += QPointF(-2453.842769979148215, -1021.556698266764784);
    points += QPointF(-2465.056142821574667, -979.842028311906802);
    points += QPointF(-2515.136197939204976, -786.051916914061394);
    points += QPointF(-2578.531709722424239, -789.067688957792370);
    points += QPointF(-2787.581316069136847, -799.012322645365543);
    points += QPointF(-2923.361187022479953, -775.070667959158072);
    points += QPointF(-3011.051754879010332, -958.321944400611301);

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

    points += QPointF(-2956.503052984476199, -931.888772536086435);
    points += QPointF(-2845.899935373787230, -956.854671027579229);
    points += QPointF(-2836.585168992718536, -967.735542343798102);
    points += QPointF(-2817.807710164808213, -991.186353033625323);
    points += QPointF(-2798.863899498001501, -1016.546792885192417);
    points += QPointF(-2779.749114651553100, -1043.706642580126527);
    points += QPointF(-2750.758655190988520, -1087.407798716331399);
    points += QPointF(-2711.491790104854317, -1151.179153064404545);
    points += QPointF(-2671.741745541462024, -1220.149721048735501);
    points += QPointF(-2630.984188137075762, -1293.204742264258130);
    points += QPointF(-2569.022589526747197, -1408.526088240801528);
    points += QPointF(-2526.849928574272781, -1488.355444559092348);
    points += QPointF(-2479.812157489062429, -1565.559731854928032);
    points += QPointF(-2412.442076435102535, -1677.899792577346261);
    points += QPointF(-2370.412668357553684, -1750.194941488371569);
    points += QPointF(-2331.673284372557191, -1819.775992708568765);
    points += QPointF(-2297.016687030217327, -1886.135962896511273);
    points += QPointF(-2274.355914702014616, -1933.347197360681321);
    points += QPointF(-2260.825261304527430, -1963.649234778169330);
    points += QPointF(-2248.749446962910497, -1992.888230218159833);
    points += QPointF(-2238.231008875008683, -2020.995791005643241);
    points += QPointF(-2233.658271705575316, -2034.554538661309380);
    points += QPointF(-2268.843635583416926, -2142.342923385616359);
    points += QPointF(-2142.312261122111522, -2153.915659267328920);
    points += QPointF(-2102.051574703390088, -2047.918381746697833);
    points += QPointF(-1957.540323165073914, -1829.765289335081889);
    points += QPointF(-2074.675746476507811, -1711.446016505494981);
    points += QPointF(-2104.950100513636244, -1680.266092395088435);
    points += QPointF(-2157.060300446018118, -1624.035819072004870);
    points += QPointF(-2193.427447844696871, -1582.926460982949266);
    points += QPointF(-2229.902122399330437, -1539.606355026679239);
    points += QPointF(-2265.283071500467031, -1494.910143892727319);
    points += QPointF(-2298.369042538657141, -1449.672470270627628);
    points += QPointF(-2320.817401374072688, -1415.919693858456867);
    points += QPointF(-2334.512979024599190, -1393.677237702732782);
    points += QPointF(-2340.846420578639481, -1382.690879565373507);
    points += QPointF(-2347.040624073485560, -1371.625420238929109);
    points += QPointF(-2359.134023973919739, -1348.472766354998839);
    points += QPointF(-2376.632959709957049, -1311.908220986026436);
    points += QPointF(-2398.550531297711586, -1260.325775813514156);
    points += QPointF(-2418.827935681583767, -1207.049235756177950);
    points += QPointF(-2437.363294145462532, -1153.537798188641318);
    points += QPointF(-2454.054727973233639, -1101.250660485525259);
    points += QPointF(-2475.673988008996275, -1027.698473771005865);
    points += QPointF(-2486.955875679277597, -985.728923017021771);
    points += QPointF(-2528.614214395534418, -824.531092595292307);
    points += QPointF(-2789.994527521454074, -836.965136994369914);
    points += QPointF(-2901.657768807878711, -817.275894802197854);
    points += QPointF(-2956.503052984476199, -931.888772536086435);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue880_Piece_Detail() const
{
    QVector<VSAPoint> points;

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

    points += QPointF(207.975244350313460, 1768.545187243969849);
    points += QPointF(192.052357429286502, 1678.591759717724699);
    points += QPointF(173.066378231393685, 1567.418836695434493);
    points += QPointF(155.048521972231981, 1457.132094707775423);
    points += QPointF(137.977104738153116, 1347.744174880447417);
    points += QPointF(113.948652961709485, 1185.025355945024330);
    points += QPointF(85.132622460042427, 971.275956770753282);
    points += QPointF(59.563338142961662, 760.511333975782236);
    points += QPointF(37.029820911080137, 552.717794783143404);
    points += QPointF(17.240286265801252, 347.166821066865964);
    points += QPointF(-0.015008757567600, 143.834684637698103);
    points += QPointF(-12.288070724611142, -17.710595485938104);
    points += QPointF(26.316659671669068, -20.643155361401121);
    points += QPointF(45.449182757960564, -46.830007644639885);
    points += QPointF(76.809025421762314, -85.802751092762563);
    points += QPointF(108.321842237686539, -121.075495075611926);
    points += QPointF(140.039609778261479, -152.751809393767388);
    points += QPointF(172.010964379443408, -180.925087728824934);
    points += QPointF(204.276873967526058, -205.677524558723803);
    points += QPointF(236.865361166418040, -227.080787315939290);
    points += QPointF(269.786238060178277, -245.199233851581596);
    points += QPointF(303.027473208045649, -260.096016944701660);
    points += QPointF(336.555041813495848, -271.841356736794523);
    points += QPointF(370.317439186195884, -280.521015759425495);
    points += QPointF(404.254443948552364, -286.242355156443011);
    points += QPointF(438.307932392301666, -289.135939110304264);
    points += QPointF(472.431670530633255, -289.352342269912128);
    points += QPointF(506.393547409497444, -287.069334621112432);
    points += QPointF(523.743301608497973, -284.906308037942438);
    points += QPointF(536.868759697778614, -282.972642447483622);
    points += QPointF(561.258611782860839, -277.900419936185699);
    points += QPointF(584.473517653931367, -271.240539453433826);
    points += QPointF(606.329115257862554, -263.093202286243240);
    points += QPointF(626.844375715222100, -253.534485538173158);
    points += QPointF(646.038852158581562, -242.661981825671262);
    points += QPointF(663.942166489923579, -230.592040397069013);
    points += QPointF(680.601417624278497, -217.453108907810446);
    points += QPointF(696.084583779115746, -203.377170449140664);
    points += QPointF(710.479607415059718, -188.491772671512337);
    points += QPointF(723.890325734899875, -172.914481691892490);
    points += QPointF(736.508892979171492, -156.650006278643616);
    points += QPointF(753.948152387964797, -131.655764206155084);
    points += QPointF(775.231449650412515, -96.860472475085288);
    points += QPointF(804.659958042692779, -43.462597316917439);
    points += QPointF(834.310499706231440, 11.429285555756231);
    points += QPointF(855.028099173095029, 47.205385464279573);
    points += QPointF(877.561104827702366, 82.245582223782790);
    points += QPointF(896.095638842511903, 107.647098085992383);
    points += QPointF(909.193579340678184, 123.996505882547439);
    points += QPointF(923.186035221141992, 140.007722295159482);
    points += QPointF(938.093299030574826, 155.551520058117717);
    points += QPointF(954.013080954349675, 170.591351779598710);
    points += QPointF(967.478282407841220, 182.052480516597853);
    points += QPointF(1004.081045839154399, 191.471640041573494);
    points += QPointF(988.997625529862603, 250.085666468473050);
    points += QPointF(977.825907073445137, 296.824101606491240);
    points += QPointF(965.149517480306031, 354.356952623703251);
    points += QPointF(951.501406931069596, 421.671630375482096);
    points += QPointF(937.421248889072672, 497.717438848125312);
    points += QPointF(923.444794947316950, 581.454841288857779);
    points += QPointF(910.105733799937411, 671.848299769626351);
    points += QPointF(897.936248098548390, 767.863239128885880);
    points += QPointF(887.467042701909804, 868.464429717168059);
    points += QPointF(879.223426160358372, 972.660783700921684);
    points += QPointF(874.920744508827170, 1052.548385015779559);
    points += QPointF(872.967477678962268, 1106.266056241682008);
    points += QPointF(871.864931939666576, 1160.331215197386200);
    points += QPointF(871.679638670957502, 1214.566973456243431);
    points += QPointF(872.476082054519338, 1268.842132890775247);
    points += QPointF(874.318295975007572, 1323.025174797752015);
    points += QPointF(877.269773413631015, 1376.984247710641966);
    points += QPointF(881.393358356554131, 1430.587165663510405);
    points += QPointF(886.751117991388469, 1483.701422556393027);
    points += QPointF(893.404193466793458, 1536.194230504537472);
    points += QPointF(901.412628664752219, 1587.932592933028218);
    points += QPointF(910.854235153861168, 1638.886269055181629);
    points += QPointF(925.096547465388880, 1705.805202848466251);
    points += QPointF(207.975244350313460, 1768.545187243969849);

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

    points += QPointF(176.433003213139955, 1809.244423168889170);
    points += QPointF(153.783135418246673, 1677.324696280966691);
    points += QPointF(133.877936584754650, 1548.752501354019614);
    points += QPointF(116.130420641369653, 1418.876192132583355);
    points += QPointF(100.474835510405967, 1288.778602915610691);
    points += QPointF(86.827945098922910, 1159.415381247187725);
    points += QPointF(75.108566360069545, 1031.745225536971702);
    points += QPointF(65.237006914137893, 906.728608847666237);
    points += QPointF(57.134700301863127, 785.326536725989286);
    points += QPointF(50.723988203508114, 668.499145301099361);
    points += QPointF(45.928034219191510, 557.203853977457015);
    points += QPointF(42.670896438097856, 452.392571656716314);
    points += QPointF(40.877865366741680, 355.006959561548001);
    points += QPointF(40.476369775324613, 265.969547397013685);
    points += QPointF(41.398331990387867, 186.165273502188512);
    points += QPointF(43.586870527854131, 116.398178051280283);
    points += QPointF(47.000830659421496, 57.582630079953006);
    points += QPointF(65.689573122725463, -149.237047149766482);
    points += QPointF(140.973388670714428, -142.434213593128675);
    points += QPointF(202.095092404902573, 32.995960747903830);
    points += QPointF(215.176017935747808, 66.462923339575823);
    points += QPointF(228.912471861364821, 97.228356961062261);
    points += QPointF(243.321624076299429, 125.586283327675147);
    points += QPointF(258.319316717494701, 151.580306428778727);
    points += QPointF(273.821335471331167, 175.260981526085629);
    points += QPointF(289.745012058267321, 196.686849698159534);
    points += QPointF(306.011343576818319, 215.925178846087249);
    points += QPointF(322.547589766898170, 233.052170738609846);
    points += QPointF(339.290194711236211, 248.152367220173886);
    points += QPointF(356.187735703955013, 261.317017379122831);
    points += QPointF(373.203459652129993, 272.641277572404306);
    points += QPointF(390.316878091855926, 282.220316425788155);
    points += QPointF(407.523915572652811, 290.144660834123783);
    points += QPointF(424.835283411702221, 296.495377493676017);
    points += QPointF(442.273070729360143, 301.339841360756793);
    points += QPointF(459.865929253040349, 304.728815010403082);
    points += QPointF(477.643553068006611, 306.695329998346381);
    points += QPointF(495.631302782506054, 307.255486372267399);
    points += QPointF(513.845745520128617, 306.410894002139287);
    points += QPointF(532.291618477982070, 304.152195932363099);
    points += QPointF(550.960380069145117, 300.463010301535405);
    points += QPointF(569.830203595676267, 295.323697065753379);
    points += QPointF(588.867069143079107, 288.714535487886565);
    points += QPointF(608.026538118922531, 280.618108761520489);
    points += QPointF(627.255825567483498, 271.020871944014061);
    points += QPointF(646.495873502259997, 259.913999190145603);
    points += QPointF(665.683232713083612, 247.293664546600496);
    points += QPointF(684.751653548328704, 233.160921516904352);
    points += QPointF(703.633355735262512, 217.521328821535519);
    points += QPointF(722.259991845816671, 200.384439254023107);
    points += QPointF(740.775593812870966, 181.547299114609586);
    points += QPointF(827.890520660986795, 85.404756544033887);
    points += QPointF(902.528765906973376, 97.365683534123988);
    points += QPointF(875.674633126301046, 264.940098963036405);
    points += QPointF(858.006688452906701, 367.901611578837674);
    points += QPointF(840.922428439775899, 473.994397066476608);
    points += QPointF(828.863202448516518, 555.064398169016044);
    points += QPointF(816.775888751009120, 644.057952245166348);
    points += QPointF(805.207674362467287, 739.845495712355955);
    points += QPointF(794.697075928369372, 841.345528209589247);
    points += QPointF(785.781431600197493, 947.468510425481782);
    points += QPointF(778.990524885403715, 1057.201556595780858);
    points += QPointF(775.712914874765261, 1141.172167796453323);
    points += QPointF(774.416545265376726, 1197.523302280107373);
    points += QPointF(773.947565414695873, 1254.216246927699785);
    points += QPointF(774.372593569917512, 1311.026601052771866);
    points += QPointF(775.755623649198697, 1367.813432350162429);
    points += QPointF(778.159815481304577, 1424.435109901397027);
    points += QPointF(781.647320084431385, 1480.749273059121151);
    points += QPointF(786.279070102662104, 1536.612815122052325);
    points += QPointF(792.114529969787213, 1591.881891902256712);
    points += QPointF(799.233464070713808, 1646.581496557790842);
    points += QPointF(815.482414590454709, 1753.334844240462417);
    points += QPointF(176.433003213139955, 1809.244423168889170);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue883_Piece_ledge() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(1122.844724409448872, 91.856125984251975);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    points += VSAPoint(1122.844724409448872, -664.049385826771640);
    points += VSAPoint(1122.844724409448872, -664.049385826771640);

    p = VSAPoint(1123.260507101031408, -692.102756615184148);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1125.552134498865144, -746.904953834851995);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1129.501889709744546, -800.813185020781020);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1135.005387617497036, -854.573547056137045);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1141.958243105948213, -908.932136824086342);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1150.256071058924590, -964.635051207794959);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1164.860792708482904, -1051.940922516873343);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(1176.240685838687341, -1114.834837477910014);
    points += VSAPoint(1176.240685838687341, -1114.834837477910014);

    p = VSAPoint(1183.149091745073974, -1158.117487659903418);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1197.298845134992916, -1231.280297344683277);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1213.068331853193058, -1298.011272385507255);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1231.813333494903418, -1369.033917416923487);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(1242.934050298765897, -1410.354749999999967);
    points += VSAPoint(1242.934050298765897, -1410.354749999999967);
    points += VSAPoint(2006.309291338582852, -1344.364346456692829);

    p = VSAPoint(2006.309291338582852, -664.049385826771640);
    p.SetSAAfter(56.6929);
    points += p;

    p = VSAPoint(2006.309291338582852, 91.856125984251975);
    p.SetSABefore(56.6929);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue883_Piece_ledge() const
{
    QVector<QPointF> points;

    points += QPointF(1085.049448818897872, 129.651401574803174);
    points += QPointF(1085.049448818897645, -664.329454817907731);
    points += QPointF(1085.476936043251271, -693.172544862022960);
    points += QPointF(1087.814610680723717, -749.075920183547851);
    points += QPointF(1091.847534492653722, -804.119277961746889);
    points += QPointF(1097.455114447215692, -858.896354932157919);
    points += QPointF(1104.517936006646551, -914.114677877462441);
    points += QPointF(1112.923052462069791, -970.537814745901301);
    points += QPointF(1127.624814855215163, -1058.423787947681376);
    points += QPointF(1138.979568121719467, -1121.178760966856998);
    points += QPointF(1145.923845706225620, -1164.686154934395063);
    points += QPointF(1160.338379861934300, -1239.218041820865892);
    points += QPointF(1176.399099634731783, -1307.181413707870433);
    points += QPointF(1195.269438012278897, -1378.678945147347804);
    points += QPointF(1214.661499217332903, -1450.735018320358677);
    points += QPointF(2044.104566929134307, -1379.033344213213468);
    points += QPointF(2044.104566929134080, -664.049385826771640);
    points += QPointF(2063.002204724409694, -664.049385826771640);
    points += QPointF(2063.002204724409694, 129.651401574803003);
    points += QPointF(1085.049448818897872, 129.651401574803174);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue883_Piece_prong() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(1122.844724409448872, 91.856125984251975);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    points += VSAPoint(1122.844724409448872, -664.049385826771640);
    points += VSAPoint(1122.844724409448872, -664.049385826771640);

    p = VSAPoint(1123.260507101031408, -692.102756615184148);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1125.552134498865144, -746.904953834851995);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1129.501889709744546, -800.813185020781020);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1135.005387617497036, -854.573547056137045);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1141.958243105948213, -908.932136824086342);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1150.256071058924590, -964.635051207794959);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1164.860792708482904, -1051.940922516873343);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(1176.240685838687341, -1114.834837477910014);
    points += VSAPoint(1176.240685838687341, -1114.834837477910014);

    p = VSAPoint(1183.149091745073974, -1158.117487659903418);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1197.298845134992916, -1231.280297344683277);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1213.068331853193058, -1298.011272385507255);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1231.813333494903418, -1369.033917416923487);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(1242.934050298765897, -1410.354749999999967);
    points += VSAPoint(1242.934050298765897, -1410.354749999999967);
    points += VSAPoint(2006.309291338582852, -1344.364346456692829);

    p = VSAPoint(2006.309291338582852, -664.049385826771640);
    p.SetSAAfter(56.6929);
    points += p;

    points += VSAPoint(2006.309291338582852, 91.856125984251975);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue883_Piece_prong() const
{
    QVector<QPointF> points;

    points += QPointF(1085.049448818897872, 129.651401574803174);
    points += QPointF(1085.049448818897645, -664.329454817907731);
    points += QPointF(1085.476936043251271, -693.172544862022960);
    points += QPointF(1087.814610680723717, -749.075920183547851);
    points += QPointF(1091.847534492653722, -804.119277961746889);
    points += QPointF(1097.455114447215692, -858.896354932157919);
    points += QPointF(1104.517936006646551, -914.114677877462441);
    points += QPointF(1112.923052462069791, -970.537814745901301);
    points += QPointF(1127.624814855215163, -1058.423787947681376);
    points += QPointF(1138.979568121719467, -1121.178760966856998);
    points += QPointF(1145.923845706225620, -1164.686154934395063);
    points += QPointF(1160.338379861934300, -1239.218041820865892);
    points += QPointF(1176.399099634731783, -1307.181413707870433);
    points += QPointF(1195.269438012278897, -1378.678945147347804);
    points += QPointF(1214.661499217332903, -1450.735018320358677);
    points += QPointF(2044.104566929134307, -1379.033344213213468);
    points += QPointF(2044.104566929134080, -664.049385826771640);
    points += QPointF(2063.002204724409694, -664.049385826771640);
    points += QPointF(2043.159685039370288, 129.651401574803003);
    points += QPointF(1085.049448818897872, 129.651401574803174);

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

    path.removeLast();
    res.removeLast();
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
#if !defined(Q_OS_WIN) && !defined(Q_CC_CLANG) && !defined(Q_PROCESSOR_X86_64) && !defined(Q_PROCESSOR_S390)
    && !defined(Q_PROCESSOR_S390_X)
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

#if !defined (Q_PROCESSOR_S390) && !defined(Q_PROCESSOR_S390_X)
    // See the file "collection/bugs/Issue_#687.val"
    QTest::newRow("Issue #687.") << points << 37.795275590551185 << ekvOrig;
#endif

    // See private test cases in file jacket_issue_#767.val, piece "Fabric_TopCollar"
    // Curve approximation scale 0.5
    QTest::newRow("Issue #767.") << InputPointsIssue767_Fabric_TopCollar() << 37.795275590551185
                                 << OutputPointsIssue767_Fabric_TopCollar();

    // See private test cases in file jacket_issue_#767.val, piece "SkinFusing_TopCollar_Notch"
    // Curve approximation scale 0.5
    QTest::newRow("Issue #767.") << InputPointsIssue767_SkinFusing_TopCollar_Notch() << 37.795275590551185
                                 << OutputPointsIssue767_SkinFusing_TopCollar_Notch();

    // See private test cases in file collection/bugs/Issue_#883_case1.val, piece "ledge"
    QTest::newRow("Issue #883. Piece 'ledge'") << InputPointsIssue883_Piece_ledge()
                                               << 37.795275590551185
                                               << OutputPointsIssue883_Piece_ledge();


    // See private test cases in file collection/bugs/Issue_#883_case1.val, piece "prong"
    QTest::newRow("Issue #883. Piece 'prong'") << InputPointsIssue883_Piece_prong()
                                               << 37.795275590551185
                                               << OutputPointsIssue883_Piece_prong();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::BrokenDetailEquidistant() const
{
    QFETCH(QVector<VSAPoint>, points);
    QFETCH(qreal, width);
    QFETCH(QVector<QPointF>, ekvOrig);

    const QVector<QPointF> ekv = VAbstractPiece::Equidistant(points, width, QString());// Take result

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

    // See file src/app/share/collection/bugs/Issue_#937.val
    QTest::newRow("Issue 937. Case1") << InputPointsIssue937Case1()
                                      << 37.795275590551185 // seam allowance width (1.0 cm)
                                      << OutputPointsIssue937Case1();

    // See file src/app/share/collection/bugs/Issue_#937.val
    QTest::newRow("Issue 937. Case2") << InputPointsIssue937Case2()
                                      << 37.795275590551185 // seam allowance width (1.0 cm)
                                      << OutputPointsIssue937Case2();

    // See file src/app/share/collection/bugs/Issue_#937.val
    QTest::newRow("Issue 937. Case3") << InputPointsIssue937Case3()
                                      << 37.795275590551185 // seam allowance width (1.0 cm)
                                      << OutputPointsIssue937Case3();

    // See file src/app/share/collection/bugs/Issue_#937.val
    QTest::newRow("Issue 937. Case4") << InputPointsIssue937Case4()
                                      << 37.795275590551185 // seam allowance width (1.0 cm)
                                      << OutputPointsIssue937Case4();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::EquidistantAngleType() const
{
    QFETCH(QVector<VSAPoint>, points);
    QFETCH(qreal, width);
    QFETCH(QVector<QPointF>, ekvOrig);

    const QVector<QPointF> ekv = VAbstractPiece::Equidistant(points, width, QString());// Take result

    // Begin comparison
    Comparison(ekv, ekvOrig);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractPiece::CorrectEquidistantPoints_data() const
{
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

    points += QPointF(1181.370211203355211, -59.406362870594705);
    points += QPointF(1269.039464046917146, 38.629190783372337);
    points += QPointF(1216.924519205873821, 100.217664911215365);
    points += QPointF(1198.880813132726189, 123.039801916996382);
    points += QPointF(1182.221067861893971, 145.936740845325801);
    points += QPointF(1166.732941367353760, 169.110459436420683);
    points += QPointF(1152.362568406511855, 192.566565745224665);
    points += QPointF(1139.058546034391838, 216.311365426063503);
    points += QPointF(1126.772026102835753, 240.351296090144842);
    points += QPointF(1115.456677001792968, 264.692382764083675);
    points += QPointF(1105.068527046499639, 289.339750389149117);
    points += QPointF(1095.565711567573317, 314.297220920440736);
    points += QPointF(1086.908151663856643, 339.567011965302527);
    points += QPointF(1079.057194431329890, 365.149542878958755);
    points += QPointF(1071.975242726907936, 391.043344437292944);
    points += QPointF(1065.665089220277650, 417.081281156897660);
    points += QPointF(1057.340196822427288, 456.975106162385032);
    points += QPointF(1048.568245158969830, 511.317285018278824);
    points += QPointF(1042.091086301033556, 566.582363522894866);
    points += QPointF(1037.578000765741308, 622.844551248912239);
    points += QPointF(1034.714937128092288, 680.001664325593765);
    points += QPointF(1033.177151285076661, 737.946609060597439);
    points += QPointF(1032.630068889550103, 796.521290449247772);
    points += QPointF(1032.895861058649189, 885.767291298921236);
    points += QPointF(1033.622065798040012, 991.890644345887608);
    points += QPointF(940.875971287879679, 1031.297840617347674);
    points += QPointF(376.989021179537758, 480.040217570165055);
    points += QPointF(1181.370211203355211, -59.406362870594705);

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
    points += QPointF(995.420358378310425, 926.054352148911448);
    points += QPointF(995.739071214507590, 1005.591452546832215);
    points += QPointF(994.595477129484721, 1095.280975097498185);
    points += QPointF(992.476422619803429, 1155.061981412450905);
    points += QPointF(988.789854628764601, 1214.554597181484041);
    points += QPointF(983.146116203005363, 1273.717516824237237);
    points += QPointF(975.162259387381937, 1332.373994385974811);
    points += QPointF(967.294011183906719, 1376.030939480649295);
    points += QPointF(960.747162663436256, 1404.970782901272059);
    points += QPointF(953.776698619438662, 1433.733694668130738);
    points += QPointF(945.943213422085364, 1462.310028497729036);
    points += QPointF(937.651555831541259, 1490.826496264650814);
    points += QPointF(927.959066252289745, 1519.000254063481179);
    points += QPointF(917.252513621465937, 1546.935105471911356);
    points += QPointF(905.483201355421215, 1574.610862888628844);
    points += QPointF(892.603420021337570, 1602.006400609849152);
    points += QPointF(878.566640707536749, 1629.099822641174114);
    points += QPointF(863.327669349691632, 1655.868693058174813);
    points += QPointF(846.842744148650354, 1682.290319209389736);
    points += QPointF(829.069561733829687, 1708.342071091297839);
    points += QPointF(809.967223824731036, 1734.001714595385920);
    points += QPointF(789.496104075464928, 1759.247733255478124);
    points += QPointF(767.617643347237504, 1784.059613352606220);
    points += QPointF(744.294089369113522, 1808.418070909965081);
    points += QPointF(719.468376809343567, 1831.807034970131099);
    points += QPointF(664.990187962422283, 1881.389648383607664);
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

    points += QPointF(1391.208950895036196, 1170.962993556315041);
    points += QPointF(1352.834645669291376, 1362.834519685039368);
    points += QPointF(1350.666016620558821, 1367.755060614720833);
    points += QPointF(1344.677673221728583, 1376.720257702359277);
    points += QPointF(1336.792007163864582, 1384.692157069932364);
    points += QPointF(1327.102329227637938, 1391.734940946334518);
    points += QPointF(1315.701950193719540, 1397.912791560458118);
    points += QPointF(1302.684180842780279, 1403.289891141198041);
    points += QPointF(1288.142331955491045, 1407.930421917446893);
    points += QPointF(1272.169714312522956, 1411.898566118098188);
    points += QPointF(1245.881900239392053, 1416.794447590956224);
    points += QPointF(1206.471781787436385, 1421.466603285398833);
    points += QPointF(1163.018182364844506, 1424.604492797537659);
    points += QPointF(1116.251434660549648, 1426.328266269482128);
    points += QPointF(1041.780469940170860, 1429.338905293278913);
    points += QPointF(963.698699875478269, 1432.082545646022936);
    points += QPointF(911.224989280228101, 1435.250971713870058);
    points += QPointF(859.186715186551055, 1439.965878586304598);
    points += QPointF(808.330363839815391, 1446.740724094475354);
    points += QPointF(771.490553816808870, 1453.575022739363249);
    points += QPointF(747.648651058679661, 1458.988766186952716);
    points += QPointF(724.522127500305487, 1465.206314323497281);
    points += QPointF(702.303930175478968, 1472.598351272474929);
    points += QPointF(691.410064634745709, 1476.200359149115229);
    points += QPointF(674.314621551015989, 1482.390491353628022);
    points += QPointF(639.446635366404280, 1492.648328941974569);
    points += QPointF(603.762098322195470, 1500.410735703404043);
    points += QPointF(567.237176508641141, 1505.681613659816321);
    points += QPointF(529.848036015992648, 1508.464864833111733);
    points += QPointF(491.570842934501115, 1508.764391245191291);
    points += QPointF(452.381763354417899, 1506.584094917955554);
    points += QPointF(412.256963365994409, 1501.927877873304169);
    points += QPointF(371.172609059481772, 1494.799642133137695);
    points += QPointF(329.104866525131456, 1485.203289719357144);
    points += QPointF(286.029901853194701, 1473.142722653862620);
    points += QPointF(241.923881133922748, 1458.621842958554225);
    points += QPointF(196.762970457567008, 1441.644552655332973);
    points += QPointF(150.523335914378663, 1422.214753766098738);
    points += QPointF(103.181143594609125, 1400.336348312752534);
    points += QPointF(54.712559588509635, 1376.013238317194009);
    points += QPointF(30.000000000000000, 1362.834519685039368);
    points += QPointF(-7.795275590551185, 1173.858141732283457);
    points += QPointF(-7.795275590551185, 5.415841095792075);
    points += QPointF(61.101021830902567, -0.709641223911092);
    points += QPointF(70.804787644663136, 53.430485561778198);
    points += QPointF(79.621893949255821, 91.858207949387321);
    points += QPointF(90.246472404177823, 128.238336548445801);
    points += QPointF(102.598525719285533, 162.806896482859997);
    points += QPointF(116.561156626640255, 195.622141977860792);
    points += QPointF(132.020820327959171, 226.740557559210657);
    points += QPointF(148.866694189775330, 256.215996794830687);
    points += QPointF(166.989862068105765, 284.099067617716173);
    points += QPointF(186.282415544815365, 310.436809263543466);
    points += QPointF(206.636574872612272, 335.272659629850125);
    points += QPointF(227.943914251639484, 358.646672648665174);
    points += QPointF(250.094746479310828, 380.595920259885645);
    points += QPointF(272.977689667348102, 401.155004785507970);
    points += QPointF(296.479410672457789, 420.356612464162595);
    points += QPointF(320.484520119941976, 438.232052715083057);
    points += QPointF(344.875583217325925, 454.811745113349389);
    points += QPointF(369.533207535191309, 470.125632957113510);
    points += QPointF(394.336171069434499, 484.203516229699574);
    points += QPointF(419.161558577576955, 497.075306799206203);
    points += QPointF(443.884879192889571, 508.771215047678083);
    points += QPointF(468.380141996197040, 519.321880586368479);
    points += QPointF(492.685597452467846, 528.352504009663221);
    points += QPointF(516.320650508090694, 536.723905788816978);
    points += QPointF(539.117950666889328, 544.386269737582552);
    points += QPointF(572.344990077737634, 553.565659766025306);
    points += QPointF(613.189423788536715, 562.294425470246097);
    points += QPointF(648.933498530834640, 567.429209393712085);
    points += QPointF(678.669134099110011, 569.356067120192051);
    points += QPointF(690.568570597070675, 569.081735831889887);
    points += QPointF(705.392572803380290, 568.745199404635628);
    points += QPointF(735.474713539123286, 569.534006540479368);
    points += QPointF(765.566399788106537, 572.193038404045183);
    points += QPointF(795.412302351329231, 576.641052885977615);
    points += QPointF(824.965602108301027, 582.801544103358651);
    points += QPointF(854.183547042348096, 590.598325318004754);
    points += QPointF(883.026844991811231, 599.956221997473222);
    points += QPointF(911.458987786852845, 610.801528373960082);
    points += QPointF(939.445585340395041, 623.062258175182706);
    points += QPointF(966.953760734297589, 636.668233397471113);
    points += QPointF(993.951633091167878, 651.551057077311953);
    points += QPointF(1020.407896673002142, 667.644011275547427);
    points += QPointF(1046.291492516970493, 684.881913581870663);
    points += QPointF(1071.571362058354453, 703.200956917979283);
    points += QPointF(1096.216269205860954, 722.538549717668161);
    points += QPointF(1120.194676878594692, 742.833167322850727);
    points += QPointF(1143.474664990699466, 764.024220756791237);
    points += QPointF(1166.023878517905587, 786.051945759705632);
    points += QPointF(1187.809496086382978, 808.857312806304776);
    points += QPointF(1208.798211222750524, 832.381957496594623);
    points += QPointF(1228.956219823794299, 856.568129936486116);
    points += QPointF(1248.249208518187288, 881.358661316437406);
    points += QPointF(1266.642339372855531, 906.696945663310998);
    points += QPointF(1284.100226878135345, 932.526934559267829);
    points += QPointF(1300.586903355860159, 958.793142367925725);
    points += QPointF(1316.065768928307079, 985.440659091103726);
    points += QPointF(1330.499521993679309, 1012.415167249780893);
    points += QPointF(1343.850065889632106, 1039.662958067617637);
    points += QPointF(1356.078387161140199, 1067.130940509586480);
    points += QPointF(1367.144400813506536, 1094.766634287532270);
    points += QPointF(1377.006758382478893, 1122.518134525559162);
    points += QPointF(1385.200774121173936, 1150.078352724689012);
    points += QPointF(1391.208950895036196, 1170.962993556315041);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test1()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-773.697565712941014, -13.057776517652679);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.813861852517789, -25.693917945236084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-670.539289284542633, -33.532046871057645);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-612.770937464562508, -58.178811938993192);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-238.402815433872661, 116.441912166387880);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-251.054901827475362, 155.855307076335350);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-261.519858253217421, 193.398201754121601);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-269.924247729261879, 229.275078971710684);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-276.394633273771547, 263.690421501066567);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.057577904909294, 296.848712114153273);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);

    p = VSAPoint(-285.555799865587915, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524762284673, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017475591800, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986935988437835, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417624891165, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705242960059, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539751899079, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185015137103, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904896102669, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963258224429, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623964930921, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150879651022, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807865813101, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858786846093, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567506178364, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197887238680, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013793455750, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279088258053, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257635074298, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213297333365, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409938463448, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111421893314, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581611051955, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084369367682, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883560269031, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243047185108, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426693544112, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698362774920, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321918305955, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561223566212, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680141983886, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942536987969, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504883768996, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560675113, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029906371, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274391478, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test1()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-814.714960629921279, 372.254255263452592);
    points += QPointF(-814.714960629921279, -46.738127408018386);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test2()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    p.SetAngleType(PieceNodeAngle::ByPointsIntersection);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-784.796811799170428, -11.786677450886199);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-760.074257477017909, -14.982052994904290);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-708.501798871969640, -24.059737122589585);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-693.161045035388270, -27.541649295426971);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-678.023758751400692, -31.460713758337747);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-663.356394315905163, -35.837032981291159);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-620.995311558273102, -53.816395724048519);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-607.498846895736733, -61.326635727968828);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-234.809931494910188, 106.205894212251422);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-241.706487749582209, 126.418557245008685);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-248.032530946383588, 146.125254699982435);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-253.803881462584570, 165.351546923668138);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-259.036359675455628, 184.122994262561235);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-263.745785962267064, 202.465157063157335);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-267.947980700289236, 220.403595671951877);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-271.658764266792559, 237.963870435440356);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-274.893957039047393, 255.171541700118325);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-277.669379394324153, 272.052169812481225);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-280.000851709893027, 288.631315119024521);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.904194363024544, 304.934537966243795);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);

    p = VSAPoint(-285.555799902053877, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524798697998, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017511901614, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986936024594911, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417660736114, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705278224011, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539786415085, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185048751572, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904928675426, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963289628486, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623995052874, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150908390600, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807893083675, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858812573993, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567530303621, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197909714511, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013814248618, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279107348233, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257652454970, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213313011007, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409952458298, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111434238795, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581621794679, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084378567789, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883568000077, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243053533723, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426698610566, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698366672559, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321921161996, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561225520489, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680143190444, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942537613588, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504884082544, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560835866, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029963441, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274395912, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    p.SetAngleType(PieceNodeAngle::ByPointsIntersection);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test2()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, 372.254255263580490);
    points += QPointF(-814.714960629921279, 331.313196850393751);
    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-814.197926270521180, -46.775492612642196);
    points += QPointF(-789.140213051348155, -49.334956233568391);
    points += QPointF(-765.476716482266852, -52.393449188523491);
    points += QPointF(-737.995740821697950, -56.780221262468970);
    points += QPointF(-716.331603418113787, -61.039176834826399);
    points += QPointF(-702.082363720278977, -64.273346529881294);
    points += QPointF(-688.166746660068270, -67.876119132381135);
    points += QPointF(-674.983023423591590, -71.809762540754065);
    points += QPointF(-662.881277988431520, -76.026115008433663);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.414888772498330, -82.758750527910010);
    points += QPointF(-638.790539302210846, -87.166916645655860);
    points += QPointF(-626.931066473615147, -93.766236503837689);
    points += QPointF(-616.484087855132884, -100.486313320968108);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-183.958889296992538, 76.240441185503187);
    points += QPointF(-199.128208171590842, 118.670762821515268);
    points += QPointF(-205.825015171693934, 138.297993904680141);
    points += QPointF(-211.936233005300977, 157.335473918921082);
    points += QPointF(-217.496918215929867, 175.859969792696745);
    points += QPointF(-222.524915681533457, 193.897845117910578);
    points += QPointF(-227.038178126608216, 211.475993598800500);
    points += QPointF(-231.054715694933805, 228.621894034446882);
    points += QPointF(-234.592533040731553, 245.363653164792709);
    points += QPointF(-237.669555346488721, 261.730031318712463);
    points += QPointF(-240.303546018901216, 277.750445980247605);
    points += QPointF(-242.512020202613343, 293.454949161782793);
    points += QPointF(-244.303509443293109, 308.800083297848346);
    points += QPointF(-246.320840785439799, 331.491357717148105);
    points += QPointF(-247.367543015896416, 354.406709688632361);
    points += QPointF(-268.813396279081758, 365.986520876534257);
    points += QPointF(-274.509592515332827, 368.544465142791694);
    points += QPointF(-280.430827174231638, 370.738996867293565);
    points += QPointF(-286.539948814471813, 372.651089393430880);
    points += QPointF(-295.760523804982597, 375.070669023561095);
    points += QPointF(-309.108660700135488, 377.812272271492361);
    points += QPointF(-323.791841340653832, 380.189287868497217);
    points += QPointF(-339.746492413262274, 382.263575885230011);
    points += QPointF(-356.936262218254399, 384.077045039871223);
    points += QPointF(-375.281527693557905, 385.653208859605968);
    points += QPointF(-394.678657424405628, 387.006850647149122);
    points += QPointF(-414.787606725105888, 388.135795504602129);
    points += QPointF(-435.952027004089416, 389.076617984495158);
    points += QPointF(-457.785958265362524, 389.818484035946028);
    points += QPointF(-480.155299367881639, 390.367355935352180);
    points += QPointF(-502.923072150244479, 390.728524359008020);
    points += QPointF(-525.950383021197354, 390.906843496429246);
    points += QPointF(-549.097086414790738, 390.906868034847378);
    points += QPointF(-572.222272885870439, 390.732932110765603);
    points += QPointF(-595.184660117100748, 390.389190982768582);
    points += QPointF(-617.842942758232198, 389.879635420374768);
    points += QPointF(-640.056148638506556, 389.208081776043002);
    points += QPointF(-661.684051750900380, 388.378134611080782);
    points += QPointF(-682.587707912334736, 387.393111203971273);
    points += QPointF(-702.630213618706989, 386.255904969502012);
    points += QPointF(-721.913933499378572, 384.952760768147186);
    points += QPointF(-739.883081024661010, 383.510162372682714);
    points += QPointF(-756.628370636720547, 381.913999891494996);
    points += QPointF(-772.055707413409664, 380.156385631588421);
    points += QPointF(-786.107417618598561, 378.217868080892174);
    points += QPointF(-798.800506213738572, 376.051179682334919);
    points += QPointF(-810.181063246566509, 373.562456117590386);
    points += QPointF(-814.714960629921279, 372.254255263580490);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test3()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-773.697565712941014, -13.057776517652679);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.813861852517789, -25.693917945236084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-670.539289284542633, -33.532046871057645);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-612.770937464562508, -58.178811938993192);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-238.402815433872661, 116.441912166387880);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-251.054901827475362, 155.855307076335350);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-261.519858253217421, 193.398201754121601);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-269.924247729261879, 229.275078971710684);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-276.394633273771547, 263.690421501066567);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.057577904909294, 296.848712114153273);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);

    p = VSAPoint(-285.555799865587915, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524762284673, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017475591800, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986935988437835, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417624891165, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705242960059, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539751899079, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185015137103, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904896102669, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963258224429, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623964930921, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150879651022, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807865813101, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858786846093, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567506178364, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197887238680, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013793455750, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279088258053, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257635074298, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213297333365, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409938463448, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111421893314, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581611051955, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084369367682, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883560269031, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243047185108, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426693544112, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698362774920, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321918305955, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561223566212, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680141983886, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942536987969, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504883768996, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560675113, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029906371, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274391478, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test3()
{
    QVector<QPointF> points;

    points += QPointF(-862.690254965683266, 340.819961100893522);
    points += QPointF(-814.714960629921279, 331.313196850393751);
    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-818.092955780502393, 371.279575916538420);
    points += QPointF(-823.692975528084730, 369.271805550997954);
    points += QPointF(-829.575336882823422, 366.626948794191208);
    points += QPointF(-835.815139714856855, 362.892089667033019);
    points += QPointF(-862.690254965683266, 340.819961100893522);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test4()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-773.697565712941014, -13.057776517652679);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.813861852517789, -25.693917945236084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-670.539289284542633, -33.532046871057645);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-612.770937464562508, -58.178811938993192);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-238.402815433872661, 116.441912166387880);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-251.054901827475362, 155.855307076335350);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-261.519858253217421, 193.398201754121601);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-269.924247729261879, 229.275078971710684);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-276.394633273771547, 263.690421501066567);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.057577904909294, 296.848712114153273);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);

    p = VSAPoint(-285.555799865587915, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524762284673, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017475591800, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986935988437835, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417624891165, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705242960059, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539751899079, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185015137103, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904896102669, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963258224429, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623964930921, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150879651022, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807865813101, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858786846093, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567506178364, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197887238680, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013793455750, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279088258053, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257635074298, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213297333365, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409938463448, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111421893314, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581611051955, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084369367682, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883560269031, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243047185108, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426693544112, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698362774920, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321918305955, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561223566212, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680141983886, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942536987969, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504883768996, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560675113, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029906371, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274391478, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test4()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-814.714960629921279, 372.254255263452592);
    points += QPointF(-814.714960629921279, -46.738127408018386);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test5()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-773.697565712941014, -13.057776517652679);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.813861852517789, -25.693917945236084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-670.539289284542633, -33.532046871057645);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-612.770937464562508, -58.178811938993192);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-238.402815433872661, 116.441912166387880);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-251.054901827475362, 155.855307076335350);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-261.519858253217421, 193.398201754121601);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-269.924247729261879, 229.275078971710684);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-276.394633273771547, 263.690421501066567);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.057577904909294, 296.848712114153273);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);

    p = VSAPoint(-285.555799865587915, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524762284673, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017475591800, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986935988437835, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417624891165, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705242960059, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539751899079, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185015137103, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904896102669, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963258224429, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623964930921, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150879651022, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807865813101, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858786846093, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567506178364, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197887238680, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013793455750, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279088258053, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257635074298, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213297333365, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409938463448, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111421893314, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581611051955, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084369367682, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883560269031, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243047185108, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426693544112, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698362774920, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321918305955, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561223566212, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680141983886, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942536987969, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504883768996, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560675113, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029906371, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274391478, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test5()
{
    QVector<QPointF> points;

    points += QPointF(-838.702607797801647, 360.520655492237381);
    points += QPointF(-814.714960629921279, 331.313196850393751);
    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-818.092955780502393, 371.279575916538420);
    points += QPointF(-823.692975528084730, 369.271805550997954);
    points += QPointF(-829.575336882823422, 366.626948794191208);
    points += QPointF(-835.815139714856855, 362.892089667033019);
    points += QPointF(-838.702607797801647, 360.520655492237381);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test6()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-773.697565712941014, -13.057776517652679);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.813861852517789, -25.693917945236084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-670.539289284542633, -33.532046871057645);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-612.770937464562508, -58.178811938993192);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-238.402815433872661, 116.441912166387880);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-251.054901827475362, 155.855307076335350);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-261.519858253217421, 193.398201754121601);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-269.924247729261879, 229.275078971710684);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-276.394633273771547, 263.690421501066567);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.057577904909294, 296.848712114153273);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);
    points += VSAPoint(-284.147385597229004, 331.313196850393751);

    p = VSAPoint(-285.555799865587915, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524762284673, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017475591800, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986935988437835, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417624891165, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705242960059, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539751899079, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185015137103, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904896102669, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963258224429, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623964930921, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150879651022, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807865813101, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858786846093, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567506178364, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197887238680, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013793455750, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279088258053, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257635074298, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213297333365, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409938463448, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111421893314, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581611051955, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084369367682, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883560269031, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243047185108, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426693544112, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698362774920, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321918305955, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561223566212, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680141983886, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942536987969, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504883768996, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560675113, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029906371, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274391478, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(0);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test6()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, -46.738127408018386);
    points += QPointF(-778.560274683193597, -50.549491761193529);
    points += QPointF(-737.960891197081651, -56.790170478391879);
    points += QPointF(-709.273784936183347, -62.545071142283753);
    points += QPointF(-681.358329676571884, -69.772421317304676);
    points += QPointF(-662.734996979773655, -76.086662659765864);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.534742107878174, -82.700492263504771);
    points += QPointF(-631.585239468780856, -90.995118061836052);
    points += QPointF(-616.328020233970847, -100.594267156991847);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-184.048650148745537, 76.198374399107223);
    points += QPointF(-202.588712626694274, 128.531217087733864);
    points += QPointF(-214.844352980142077, 166.709615345349164);
    points += QPointF(-224.900721916665105, 202.786707734969326);
    points += QPointF(-232.934347140993879, 237.080856834901624);
    points += QPointF(-239.089037151700012, 269.817045957384835);
    points += QPointF(-243.506611386394923, 301.230502874877175);
    points += QPointF(-246.324245680472586, 331.565901059170528);
    points += QPointF(-247.367542640672326, 354.406709505752815);
    points += QPointF(-268.813396242720216, 365.986520876589054);
    points += QPointF(-274.509592479190928, 368.544465142904016);
    points += QPointF(-280.430827138279028, 370.738996867416517);
    points += QPointF(-286.539948778692178, 372.651089393540587);
    points += QPointF(-295.760523769485246, 375.070669023642552);
    points += QPointF(-309.108660665237892, 377.812272271559323);
    points += QPointF(-323.791841306461833, 380.189287868544909);
    points += QPointF(-339.746492379944868, 382.263575885265993);
    points += QPointF(-356.936262185776172, 384.077045039880318);
    points += QPointF(-375.281527662442841, 385.653208859628023);
    points += QPointF(-394.678657394273387, 387.006850647147644);
    points += QPointF(-414.787606696514558, 388.135795504608552);
    points += QPointF(-435.952026976937020, 389.076617984501581);
    points += QPointF(-457.785958239726995, 389.818484035948188);
    points += QPointF(-480.155299343825106, 390.367355935354340);
    points += QPointF(-502.923072127811622, 390.728524359010180);
    points += QPointF(-525.950383000425973, 390.906843496429246);
    points += QPointF(-549.097086395700558, 390.906868034847378);
    points += QPointF(-572.222272868468281, 390.732932110765603);
    points += QPointF(-595.184660101380132, 390.389190982770685);
    points += QPointF(-617.842942744172888, 389.879635420374768);
    points += QPointF(-640.056148626075128, 389.208081776045162);
    points += QPointF(-661.684051740050222, 388.378134611085102);
    points += QPointF(-682.587707903001387, 387.393111203977753);
    points += QPointF(-702.630213610816895, 386.255904969512756);
    points += QPointF(-721.913933492959586, 384.952760768153212);
    points += QPointF(-739.883081019170959, 383.510162372720174);
    points += QPointF(-756.628370632600195, 381.913999891517676);
    points += QPointF(-772.055707410166860, 380.156385631636169);
    points += QPointF(-786.107417616341536, 378.217868080938274);
    points += QPointF(-798.800506212061464, 376.051179682426266);
    points += QPointF(-810.181063245406790, 373.562456117723798);
    points += QPointF(-814.714960629921279, 372.254255263452592);
    points += QPointF(-814.714960629921279, -46.738127408018386);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test1_1()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-784.796811799170428, -11.786677450886199);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-760.074257477017909, -14.982052994904290);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-708.501798871969640, -24.059737122589585);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-693.161045035388270, -27.541649295426971);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-678.023758751400692, -31.460713758337747);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-663.356394315905163, -35.837032981291159);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-620.995311558273102, -53.816395724048519);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-607.498846895736733, -61.326635727968828);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-234.809931494910188, 106.205894212251422);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-241.706487749582209, 126.418557245008685);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-248.032530946383588, 146.125254699982435);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-253.803881462584570, 165.351546923668138);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-259.036359675455628, 184.122994262561235);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-263.745785962267064, 202.465157063157335);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-267.947980700289236, 220.403595671951877);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-271.658764266792559, 237.963870435440356);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-274.893957039047393, 255.171541700118325);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-277.669379394324153, 272.052169812481225);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-280.000851709893027, 288.631315119024521);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.904194363024544, 304.934537966243795);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);

    p = VSAPoint(-285.555799902053877, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524798697998, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017511901614, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986936024594911, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417660736114, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705278224011, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539786415085, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185048751572, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904928675426, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963289628486, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623995052874, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150908390600, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807893083675, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858812573993, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567530303621, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197909714511, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013814248618, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279107348233, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257652454970, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213313011007, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409952458298, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111434238795, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581621794679, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084378567789, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883568000077, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243053533723, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426698610566, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698366672559, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321921161996, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561225520489, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680143190444, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942537613588, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504884082544, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560835866, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029963441, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274395912, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test1_1()
{
    QVector<QPointF> points;

    points += QPointF(-941.393414608729813, 276.182237987752274);
    points += QPointF(-831.030698870537890, 27.866127576820318);
    points += QPointF(-797.079525240757903, -48.524013090184425);
    points += QPointF(-789.140213051348155, -49.334956233568391);
    points += QPointF(-765.476716482266852, -52.393449188523491);
    points += QPointF(-737.995740821697950, -56.780221262468970);
    points += QPointF(-716.331603418113787, -61.039176834826399);
    points += QPointF(-702.082363720278977, -64.273346529881294);
    points += QPointF(-688.166746660068270, -67.876119132381135);
    points += QPointF(-674.983023423591590, -71.809762540754065);
    points += QPointF(-662.881277988431520, -76.026115008433663);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.414888772498330, -82.758750527910010);
    points += QPointF(-638.790539302210846, -87.166916645655860);
    points += QPointF(-626.931066473615147, -93.766236503837689);
    points += QPointF(-616.484087855132884, -100.486313320968108);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-183.958889296992538, 76.240441185503187);
    points += QPointF(-199.128208171590842, 118.670762821515268);
    points += QPointF(-205.825015171693934, 138.297993904680141);
    points += QPointF(-211.936233005300977, 157.335473918921082);
    points += QPointF(-217.496918215929867, 175.859969792696745);
    points += QPointF(-222.524915681533457, 193.897845117910578);
    points += QPointF(-227.038178126608216, 211.475993598800500);
    points += QPointF(-231.054715694933805, 228.621894034446882);
    points += QPointF(-234.592533040731553, 245.363653164792709);
    points += QPointF(-237.669555346488721, 261.730031318712463);
    points += QPointF(-240.303546018901216, 277.750445980247605);
    points += QPointF(-242.512020202613343, 293.454949161782793);
    points += QPointF(-244.303509443293109, 308.800083297848346);
    points += QPointF(-246.320840785439799, 331.491357717148105);
    points += QPointF(-247.367543015896416, 354.406709688632361);
    points += QPointF(-268.813396279081758, 365.986520876534257);
    points += QPointF(-274.509592515332827, 368.544465142791694);
    points += QPointF(-280.430827174231638, 370.738996867293565);
    points += QPointF(-286.539948814471813, 372.651089393430880);
    points += QPointF(-295.760523804982597, 375.070669023561095);
    points += QPointF(-309.108660700135488, 377.812272271492361);
    points += QPointF(-323.791841340653832, 380.189287868497217);
    points += QPointF(-339.746492413262274, 382.263575885230011);
    points += QPointF(-356.936262218254399, 384.077045039871223);
    points += QPointF(-375.281527693557905, 385.653208859605968);
    points += QPointF(-394.678657424405628, 387.006850647149122);
    points += QPointF(-414.787606725105888, 388.135795504602129);
    points += QPointF(-435.952027004089416, 389.076617984495158);
    points += QPointF(-457.785958265362524, 389.818484035946028);
    points += QPointF(-480.155299367881639, 390.367355935352180);
    points += QPointF(-502.923072150244479, 390.728524359008020);
    points += QPointF(-525.950383021197354, 390.906843496429246);
    points += QPointF(-549.097086414790738, 390.906868034847378);
    points += QPointF(-572.222272885870439, 390.732932110765603);
    points += QPointF(-595.184660117100748, 390.389190982768582);
    points += QPointF(-617.842942758232198, 389.879635420374768);
    points += QPointF(-640.056148638506556, 389.208081776043002);
    points += QPointF(-661.684051750900380, 388.378134611080782);
    points += QPointF(-682.587707912334736, 387.393111203971273);
    points += QPointF(-702.630213618706989, 386.255904969502012);
    points += QPointF(-721.913933499378572, 384.952760768147186);
    points += QPointF(-739.883081024661010, 383.510162372682714);
    points += QPointF(-756.628370636720547, 381.913999891494996);
    points += QPointF(-772.055707413409664, 380.156385631588421);
    points += QPointF(-786.107417618598561, 378.217868080892174);
    points += QPointF(-798.800506213738572, 376.051179682334919);
    points += QPointF(-810.181063246566509, 373.562456117590386);
    points += QPointF(-818.092955781375508, 371.279575916359818);
    points += QPointF(-823.692975528761167, 369.271805550790248);
    points += QPointF(-829.575336883340356, 366.626948793946269);
    points += QPointF(-835.815139715093551, 362.892089666854304);
    points += QPointF(-941.393414608729813, 276.182237987752274);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test2_2()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByPointsIntersection);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-784.796811799170428, -11.786677450886199);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-760.074257477017909, -14.982052994904290);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-708.501798871969640, -24.059737122589585);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-693.161045035388270, -27.541649295426971);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-678.023758751400692, -31.460713758337747);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-663.356394315905163, -35.837032981291159);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-620.995311558273102, -53.816395724048519);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-607.498846895736733, -61.326635727968828);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-234.809931494910188, 106.205894212251422);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-241.706487749582209, 126.418557245008685);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-248.032530946383588, 146.125254699982435);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-253.803881462584570, 165.351546923668138);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-259.036359675455628, 184.122994262561235);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-263.745785962267064, 202.465157063157335);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-267.947980700289236, 220.403595671951877);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-271.658764266792559, 237.963870435440356);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-274.893957039047393, 255.171541700118325);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-277.669379394324153, 272.052169812481225);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-280.000851709893027, 288.631315119024521);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.904194363024544, 304.934537966243795);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);

    p = VSAPoint(-285.555799902053877, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524798697998, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017511901614, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986936024594911, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417660736114, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705278224011, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539786415085, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185048751572, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904928675426, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963289628486, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623995052874, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150908390600, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807893083675, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858812573993, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567530303621, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197909714511, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013814248618, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279107348233, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257652454970, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213313011007, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409952458298, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111434238795, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581621794679, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084378567789, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883568000077, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243053533723, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426698610566, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698366672559, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321921161996, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561225520489, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680143190444, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942537613588, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504884082544, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560835866, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029963441, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274395912, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByPointsIntersection);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test2_2()
{
    QVector<QPointF> points;

    points += QPointF(-814.714960629921279, 372.254255263580490);
    points += QPointF(-925.469085441181278, 240.352497360768297);
    points += QPointF(-831.030698870537890, 27.866127576820318);
    points += QPointF(-797.079525240757903, -48.524013090184425);
    points += QPointF(-789.140213051348155, -49.334956233568391);
    points += QPointF(-765.476716482266852, -52.393449188523491);
    points += QPointF(-737.995740821697950, -56.780221262468970);
    points += QPointF(-716.331603418113787, -61.039176834826399);
    points += QPointF(-702.082363720278977, -64.273346529881294);
    points += QPointF(-688.166746660068270, -67.876119132381135);
    points += QPointF(-674.983023423591590, -71.809762540754065);
    points += QPointF(-662.881277988431520, -76.026115008433663);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.414888772498330, -82.758750527910010);
    points += QPointF(-638.790539302210846, -87.166916645655860);
    points += QPointF(-626.931066473615147, -93.766236503837689);
    points += QPointF(-616.484087855132884, -100.486313320968108);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-183.958889296992538, 76.240441185503187);
    points += QPointF(-199.128208171590842, 118.670762821515268);
    points += QPointF(-205.825015171693934, 138.297993904680141);
    points += QPointF(-211.936233005300977, 157.335473918921082);
    points += QPointF(-217.496918215929867, 175.859969792696745);
    points += QPointF(-222.524915681533457, 193.897845117910578);
    points += QPointF(-227.038178126608216, 211.475993598800500);
    points += QPointF(-231.054715694933805, 228.621894034446882);
    points += QPointF(-234.592533040731553, 245.363653164792709);
    points += QPointF(-237.669555346488721, 261.730031318712463);
    points += QPointF(-240.303546018901216, 277.750445980247605);
    points += QPointF(-242.512020202613343, 293.454949161782793);
    points += QPointF(-244.303509443293109, 308.800083297848346);
    points += QPointF(-246.320840785439799, 331.491357717148105);
    points += QPointF(-247.367543015896416, 354.406709688632361);
    points += QPointF(-268.813396279081758, 365.986520876534257);
    points += QPointF(-274.509592515332827, 368.544465142791694);
    points += QPointF(-280.430827174231638, 370.738996867293565);
    points += QPointF(-286.539948814471813, 372.651089393430880);
    points += QPointF(-295.760523804982597, 375.070669023561095);
    points += QPointF(-309.108660700135488, 377.812272271492361);
    points += QPointF(-323.791841340653832, 380.189287868497217);
    points += QPointF(-339.746492413262274, 382.263575885230011);
    points += QPointF(-356.936262218254399, 384.077045039871223);
    points += QPointF(-375.281527693557905, 385.653208859605968);
    points += QPointF(-394.678657424405628, 387.006850647149122);
    points += QPointF(-414.787606725105888, 388.135795504602129);
    points += QPointF(-435.952027004089416, 389.076617984495158);
    points += QPointF(-457.785958265362524, 389.818484035946028);
    points += QPointF(-480.155299367881639, 390.367355935352180);
    points += QPointF(-502.923072150244479, 390.728524359008020);
    points += QPointF(-525.950383021197354, 390.906843496429246);
    points += QPointF(-549.097086414790738, 390.906868034847378);
    points += QPointF(-572.222272885870439, 390.732932110765603);
    points += QPointF(-595.184660117100748, 390.389190982768582);
    points += QPointF(-617.842942758232198, 389.879635420374768);
    points += QPointF(-640.056148638506556, 389.208081776043002);
    points += QPointF(-661.684051750900380, 388.378134611080782);
    points += QPointF(-682.587707912334736, 387.393111203971273);
    points += QPointF(-702.630213618706989, 386.255904969502012);
    points += QPointF(-721.913933499378572, 384.952760768147186);
    points += QPointF(-739.883081024661010, 383.510162372682714);
    points += QPointF(-756.628370636720547, 381.913999891494996);
    points += QPointF(-772.055707413409664, 380.156385631588421);
    points += QPointF(-786.107417618598561, 378.217868080892174);
    points += QPointF(-798.800506213738572, 376.051179682334919);
    points += QPointF(-810.181063246566509, 373.562456117590386);
    points += QPointF(-814.714960629921279, 372.254255263580490);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test3_3()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-784.796811799170428, -11.786677450886199);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-760.074257477017909, -14.982052994904290);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-708.501798871969640, -24.059737122589585);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-693.161045035388270, -27.541649295426971);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-678.023758751400692, -31.460713758337747);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-663.356394315905163, -35.837032981291159);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-620.995311558273102, -53.816395724048519);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-607.498846895736733, -61.326635727968828);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-234.809931494910188, 106.205894212251422);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-241.706487749582209, 126.418557245008685);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-248.032530946383588, 146.125254699982435);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-253.803881462584570, 165.351546923668138);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-259.036359675455628, 184.122994262561235);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-263.745785962267064, 202.465157063157335);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-267.947980700289236, 220.403595671951877);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-271.658764266792559, 237.963870435440356);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-274.893957039047393, 255.171541700118325);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-277.669379394324153, 272.052169812481225);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-280.000851709893027, 288.631315119024521);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.904194363024544, 304.934537966243795);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);

    p = VSAPoint(-285.555799902053877, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524798697998, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017511901614, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986936024594911, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417660736114, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705278224011, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539786415085, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185048751572, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904928675426, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963289628486, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623995052874, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150908390600, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807893083675, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858812573993, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567530303621, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197909714511, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013814248618, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279107348233, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257652454970, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213313011007, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409952458298, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111434238795, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581621794679, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084378567789, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883568000077, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243053533723, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426698610566, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698366672559, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321921161996, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561225520489, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680143190444, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942537613588, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504884082544, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560835866, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029963441, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274395912, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test3_3()
{
    QVector<QPointF> points;

    points += QPointF(-957.520050609572763, 262.937674017326458);
    points += QPointF(-925.469085441181164, 240.352497360767984);
    points += QPointF(-831.030698870537890, 27.866127576820318);
    points += QPointF(-797.079525240757903, -48.524013090184425);
    points += QPointF(-789.140213051348155, -49.334956233568391);
    points += QPointF(-765.476716482266852, -52.393449188523491);
    points += QPointF(-737.995740821697950, -56.780221262468970);
    points += QPointF(-716.331603418113787, -61.039176834826399);
    points += QPointF(-702.082363720278977, -64.273346529881294);
    points += QPointF(-688.166746660068270, -67.876119132381135);
    points += QPointF(-674.983023423591590, -71.809762540754065);
    points += QPointF(-662.881277988431520, -76.026115008433663);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.414888772498330, -82.758750527910010);
    points += QPointF(-638.790539302210846, -87.166916645655860);
    points += QPointF(-626.931066473615147, -93.766236503837689);
    points += QPointF(-616.484087855132884, -100.486313320968108);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-183.958889296992538, 76.240441185503187);
    points += QPointF(-199.128208171590842, 118.670762821515268);
    points += QPointF(-205.825015171693934, 138.297993904680141);
    points += QPointF(-211.936233005300977, 157.335473918921082);
    points += QPointF(-217.496918215929867, 175.859969792696745);
    points += QPointF(-222.524915681533457, 193.897845117910578);
    points += QPointF(-227.038178126608216, 211.475993598800500);
    points += QPointF(-231.054715694933805, 228.621894034446882);
    points += QPointF(-234.592533040731553, 245.363653164792709);
    points += QPointF(-237.669555346488721, 261.730031318712463);
    points += QPointF(-240.303546018901216, 277.750445980247605);
    points += QPointF(-242.512020202613343, 293.454949161782793);
    points += QPointF(-244.303509443293109, 308.800083297848346);
    points += QPointF(-246.320840785439799, 331.491357717148105);
    points += QPointF(-247.367543015896416, 354.406709688632361);
    points += QPointF(-268.813396279081758, 365.986520876534257);
    points += QPointF(-274.509592515332827, 368.544465142791694);
    points += QPointF(-280.430827174231638, 370.738996867293565);
    points += QPointF(-286.539948814471813, 372.651089393430880);
    points += QPointF(-295.760523804982597, 375.070669023561095);
    points += QPointF(-309.108660700135488, 377.812272271492361);
    points += QPointF(-323.791841340653832, 380.189287868497217);
    points += QPointF(-339.746492413262274, 382.263575885230011);
    points += QPointF(-356.936262218254399, 384.077045039871223);
    points += QPointF(-375.281527693557905, 385.653208859605968);
    points += QPointF(-394.678657424405628, 387.006850647149122);
    points += QPointF(-414.787606725105888, 388.135795504602129);
    points += QPointF(-435.952027004089416, 389.076617984495158);
    points += QPointF(-457.785958265362524, 389.818484035946028);
    points += QPointF(-480.155299367881639, 390.367355935352180);
    points += QPointF(-502.923072150244479, 390.728524359008020);
    points += QPointF(-525.950383021197354, 390.906843496429246);
    points += QPointF(-549.097086414790738, 390.906868034847378);
    points += QPointF(-572.222272885870439, 390.732932110765603);
    points += QPointF(-595.184660117100748, 390.389190982768582);
    points += QPointF(-617.842942758232198, 389.879635420374768);
    points += QPointF(-640.056148638506556, 389.208081776043002);
    points += QPointF(-661.684051750900380, 388.378134611080782);
    points += QPointF(-682.587707912334736, 387.393111203971273);
    points += QPointF(-702.630213618706989, 386.255904969502012);
    points += QPointF(-721.913933499378572, 384.952760768147186);
    points += QPointF(-739.883081024661010, 383.510162372682714);
    points += QPointF(-756.628370636720547, 381.913999891494996);
    points += QPointF(-772.055707413409664, 380.156385631588421);
    points += QPointF(-786.107417618598561, 378.217868080892174);
    points += QPointF(-798.800506213738572, 376.051179682334919);
    points += QPointF(-810.181063246566509, 373.562456117590386);
    points += QPointF(-818.092955781375508, 371.279575916359818);
    points += QPointF(-823.692975528761167, 369.271805550790248);
    points += QPointF(-829.575336883340356, 366.626948793946269);
    points += QPointF(-835.815139715093551, 362.892089666854304);
    points += QPointF(-957.520050609572763, 262.937674017326458);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test4_4()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-784.796811799170428, -11.786677450886199);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-760.074257477017909, -14.982052994904290);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-708.501798871969640, -24.059737122589585);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-693.161045035388270, -27.541649295426971);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-678.023758751400692, -31.460713758337747);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-663.356394315905163, -35.837032981291159);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-620.995311558273102, -53.816395724048519);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-607.498846895736733, -61.326635727968828);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-234.809931494910188, 106.205894212251422);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-241.706487749582209, 126.418557245008685);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-248.032530946383588, 146.125254699982435);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-253.803881462584570, 165.351546923668138);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-259.036359675455628, 184.122994262561235);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-263.745785962267064, 202.465157063157335);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-267.947980700289236, 220.403595671951877);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-271.658764266792559, 237.963870435440356);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-274.893957039047393, 255.171541700118325);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-277.669379394324153, 272.052169812481225);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-280.000851709893027, 288.631315119024521);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.904194363024544, 304.934537966243795);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);

    p = VSAPoint(-285.555799902053877, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524798697998, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017511901614, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986936024594911, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417660736114, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705278224011, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539786415085, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185048751572, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904928675426, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963289628486, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623995052874, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150908390600, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807893083675, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858812573993, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567530303621, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197909714511, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013814248618, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279107348233, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257652454970, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213313011007, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409952458298, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111434238795, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581621794679, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084378567789, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883568000077, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243053533723, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426698610566, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698366672559, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321921161996, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561225520489, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680143190444, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942537613588, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504884082544, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560835866, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029963441, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274395912, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test4_4()
{
    QVector<QPointF> points;

    points += QPointF(-831.030698870537890, 27.866127576820318);
    points += QPointF(-797.079525240757903, -48.524013090184425);
    points += QPointF(-789.140213051348155, -49.334956233568391);
    points += QPointF(-765.476716482266852, -52.393449188523491);
    points += QPointF(-737.995740821697950, -56.780221262468970);
    points += QPointF(-716.331603418113787, -61.039176834826399);
    points += QPointF(-702.082363720278977, -64.273346529881294);
    points += QPointF(-688.166746660068270, -67.876119132381135);
    points += QPointF(-674.983023423591590, -71.809762540754065);
    points += QPointF(-662.881277988431520, -76.026115008433663);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.414888772498330, -82.758750527910010);
    points += QPointF(-638.790539302210846, -87.166916645655860);
    points += QPointF(-626.931066473615147, -93.766236503837689);
    points += QPointF(-616.484087855132884, -100.486313320968108);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-183.958889296992538, 76.240441185503187);
    points += QPointF(-199.128208171590842, 118.670762821515268);
    points += QPointF(-205.825015171693934, 138.297993904680141);
    points += QPointF(-211.936233005300977, 157.335473918921082);
    points += QPointF(-217.496918215929867, 175.859969792696745);
    points += QPointF(-222.524915681533457, 193.897845117910578);
    points += QPointF(-227.038178126608216, 211.475993598800500);
    points += QPointF(-231.054715694933805, 228.621894034446882);
    points += QPointF(-234.592533040731553, 245.363653164792709);
    points += QPointF(-237.669555346488721, 261.730031318712463);
    points += QPointF(-240.303546018901216, 277.750445980247605);
    points += QPointF(-242.512020202613343, 293.454949161782793);
    points += QPointF(-244.303509443293109, 308.800083297848346);
    points += QPointF(-246.320840785439799, 331.491357717148105);
    points += QPointF(-247.367543015896416, 354.406709688632361);
    points += QPointF(-268.813396279081758, 365.986520876534257);
    points += QPointF(-274.509592515332827, 368.544465142791694);
    points += QPointF(-280.430827174231638, 370.738996867293565);
    points += QPointF(-286.539948814471813, 372.651089393430880);
    points += QPointF(-295.760523804982597, 375.070669023561095);
    points += QPointF(-309.108660700135488, 377.812272271492361);
    points += QPointF(-323.791841340653832, 380.189287868497217);
    points += QPointF(-339.746492413262274, 382.263575885230011);
    points += QPointF(-356.936262218254399, 384.077045039871223);
    points += QPointF(-375.281527693557905, 385.653208859605968);
    points += QPointF(-394.678657424405628, 387.006850647149122);
    points += QPointF(-414.787606725105888, 388.135795504602129);
    points += QPointF(-435.952027004089416, 389.076617984495158);
    points += QPointF(-457.785958265362524, 389.818484035946028);
    points += QPointF(-480.155299367881639, 390.367355935352180);
    points += QPointF(-502.923072150244479, 390.728524359008020);
    points += QPointF(-525.950383021197354, 390.906843496429246);
    points += QPointF(-549.097086414790738, 390.906868034847378);
    points += QPointF(-572.222272885870439, 390.732932110765603);
    points += QPointF(-595.184660117100748, 390.389190982768582);
    points += QPointF(-617.842942758232198, 389.879635420374768);
    points += QPointF(-640.056148638506556, 389.208081776043002);
    points += QPointF(-661.684051750900380, 388.378134611080782);
    points += QPointF(-682.587707912334736, 387.393111203971273);
    points += QPointF(-702.630213618706989, 386.255904969502012);
    points += QPointF(-721.913933499378572, 384.952760768147186);
    points += QPointF(-739.883081024661010, 383.510162372682714);
    points += QPointF(-756.628370636720547, 381.913999891494996);
    points += QPointF(-772.055707413409664, 380.156385631588421);
    points += QPointF(-786.107417618598561, 378.217868080892174);
    points += QPointF(-798.800506213738572, 376.051179682334919);
    points += QPointF(-810.181063246566509, 373.562456117590386);
    points += QPointF(-818.092955781375508, 371.279575916359818);
    points += QPointF(-823.692975528761167, 369.271805550790248);
    points += QPointF(-829.575336883340356, 366.626948793946269);
    points += QPointF(-835.815139715093551, 362.892089666854304);
    points += QPointF(-949.648673098164636, 294.756569588980881);
    points += QPointF(-831.030698870537890, 27.866127576820318);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test5_5()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-784.796811799170428, -11.786677450886199);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-760.074257477017909, -14.982052994904290);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-708.501798871969640, -24.059737122589585);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-693.161045035388270, -27.541649295426971);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-678.023758751400692, -31.460713758337747);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-663.356394315905163, -35.837032981291159);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-620.995311558273102, -53.816395724048519);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-607.498846895736733, -61.326635727968828);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-234.809931494910188, 106.205894212251422);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-241.706487749582209, 126.418557245008685);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-248.032530946383588, 146.125254699982435);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-253.803881462584570, 165.351546923668138);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-259.036359675455628, 184.122994262561235);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-263.745785962267064, 202.465157063157335);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-267.947980700289236, 220.403595671951877);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-271.658764266792559, 237.963870435440356);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-274.893957039047393, 255.171541700118325);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-277.669379394324153, 272.052169812481225);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-280.000851709893027, 288.631315119024521);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.904194363024544, 304.934537966243795);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);

    p = VSAPoint(-285.555799902053877, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524798697998, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017511901614, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986936024594911, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417660736114, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705278224011, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539786415085, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185048751572, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904928675426, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963289628486, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623995052874, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150908390600, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807893083675, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858812573993, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567530303621, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197909714511, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013814248618, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279107348233, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257652454970, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213313011007, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409952458298, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111434238795, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581621794679, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084378567789, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883568000077, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243053533723, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426698610566, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698366672559, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321921161996, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561225520489, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680143190444, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942537613588, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504884082544, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560835866, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029963441, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274395912, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeRightAngle);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test5_5()
{
    QVector<QPointF> points;

    points += QPointF(-949.456732609151800, 269.559956002538911);
    points += QPointF(-925.469085441181278, 240.352497360768297);
    points += QPointF(-831.030698870537890, 27.866127576820318);
    points += QPointF(-797.079525240757903, -48.524013090184425);
    points += QPointF(-789.140213051348155, -49.334956233568391);
    points += QPointF(-765.476716482266852, -52.393449188523491);
    points += QPointF(-737.995740821697950, -56.780221262468970);
    points += QPointF(-716.331603418113787, -61.039176834826399);
    points += QPointF(-702.082363720278977, -64.273346529881294);
    points += QPointF(-688.166746660068270, -67.876119132381135);
    points += QPointF(-674.983023423591590, -71.809762540754065);
    points += QPointF(-662.881277988431520, -76.026115008433663);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.414888772498330, -82.758750527910010);
    points += QPointF(-638.790539302210846, -87.166916645655860);
    points += QPointF(-626.931066473615147, -93.766236503837689);
    points += QPointF(-616.484087855132884, -100.486313320968108);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-183.958889296992538, 76.240441185503187);
    points += QPointF(-199.128208171590842, 118.670762821515268);
    points += QPointF(-205.825015171693934, 138.297993904680141);
    points += QPointF(-211.936233005300977, 157.335473918921082);
    points += QPointF(-217.496918215929867, 175.859969792696745);
    points += QPointF(-222.524915681533457, 193.897845117910578);
    points += QPointF(-227.038178126608216, 211.475993598800500);
    points += QPointF(-231.054715694933805, 228.621894034446882);
    points += QPointF(-234.592533040731553, 245.363653164792709);
    points += QPointF(-237.669555346488721, 261.730031318712463);
    points += QPointF(-240.303546018901216, 277.750445980247605);
    points += QPointF(-242.512020202613343, 293.454949161782793);
    points += QPointF(-244.303509443293109, 308.800083297848346);
    points += QPointF(-246.320840785439799, 331.491357717148105);
    points += QPointF(-247.367543015896416, 354.406709688632361);
    points += QPointF(-268.813396279081758, 365.986520876534257);
    points += QPointF(-274.509592515332827, 368.544465142791694);
    points += QPointF(-280.430827174231638, 370.738996867293565);
    points += QPointF(-286.539948814471813, 372.651089393430880);
    points += QPointF(-295.760523804982597, 375.070669023561095);
    points += QPointF(-309.108660700135488, 377.812272271492361);
    points += QPointF(-323.791841340653832, 380.189287868497217);
    points += QPointF(-339.746492413262274, 382.263575885230011);
    points += QPointF(-356.936262218254399, 384.077045039871223);
    points += QPointF(-375.281527693557905, 385.653208859605968);
    points += QPointF(-394.678657424405628, 387.006850647149122);
    points += QPointF(-414.787606725105888, 388.135795504602129);
    points += QPointF(-435.952027004089416, 389.076617984495158);
    points += QPointF(-457.785958265362524, 389.818484035946028);
    points += QPointF(-480.155299367881639, 390.367355935352180);
    points += QPointF(-502.923072150244479, 390.728524359008020);
    points += QPointF(-525.950383021197354, 390.906843496429246);
    points += QPointF(-549.097086414790738, 390.906868034847378);
    points += QPointF(-572.222272885870439, 390.732932110765603);
    points += QPointF(-595.184660117100748, 390.389190982768582);
    points += QPointF(-617.842942758232198, 389.879635420374768);
    points += QPointF(-640.056148638506556, 389.208081776043002);
    points += QPointF(-661.684051750900380, 388.378134611080782);
    points += QPointF(-682.587707912334736, 387.393111203971273);
    points += QPointF(-702.630213618706989, 386.255904969502012);
    points += QPointF(-721.913933499378572, 384.952760768147186);
    points += QPointF(-739.883081024661010, 383.510162372682714);
    points += QPointF(-756.628370636720547, 381.913999891494996);
    points += QPointF(-772.055707413409664, 380.156385631588421);
    points += QPointF(-786.107417618598561, 378.217868080892174);
    points += QPointF(-798.800506213738572, 376.051179682334919);
    points += QPointF(-810.181063246566509, 373.562456117590386);
    points += QPointF(-818.092955781375508, 371.279575916359818);
    points += QPointF(-823.692975528761167, 369.271805550790248);
    points += QPointF(-829.575336883340356, 366.626948793946269);
    points += QPointF(-835.815139715093551, 362.892089666854304);
    points += QPointF(-949.456732609151800, 269.559956002538911);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue923Test6_6()
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-814.714960629921279, -8.844283464566928);
    p.SetSABefore(0);
    p.SetSAAfter(37.7953);
    points += p;

    p = VSAPoint(-810.914791391285917, -9.118915311803084);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-784.796811799170428, -11.786677450886199);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-760.074257477017909, -14.982052994904290);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-731.369396039071717, -19.564193239769612);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-708.501798871969640, -24.059737122589585);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-693.161045035388270, -27.541649295426971);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-678.023758751400692, -31.460713758337747);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-663.356394315905163, -35.837032981291159);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-649.425406024800168, -40.690709434256448);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-636.497248173983621, -46.041845587202857);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-630.550935626324190, -48.932226964906654);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-620.995311558273102, -53.816395724048519);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-607.498846895736733, -61.326635727968828);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-595.504209442756292, -69.042252656202692);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-590.462992125984215, -72.529322834645669);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);
    points += VSAPoint(-231.118006951984967, 95.879158575193841);

    p = VSAPoint(-234.809931494910188, 106.205894212251422);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-241.706487749582209, 126.418557245008685);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-248.032530946383588, 146.125254699982435);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-253.803881462584570, 165.351546923668138);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-259.036359675455628, 184.122994262561235);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-263.745785962267064, 202.465157063157335);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-267.947980700289236, 220.403595671951877);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-271.658764266792559, 237.963870435440356);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-274.893957039047393, 255.171541700118325);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-277.669379394324153, 272.052169812481225);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-280.000851709893027, 288.631315119024521);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-281.904194363024544, 304.934537966243795);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-284.039644640838333, 328.954433582934655);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);
    points += VSAPoint(-284.147385633699400, 331.313196850393751);

    p = VSAPoint(-285.555799902053877, 332.073678193884462);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-288.834524798697998, 333.546028373225568);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-292.651017511901614, 334.960499310104979);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-296.986936024594911, 336.317597411974702);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-304.365417660736114, 338.253791617484694);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-315.933705278224011, 340.629827700589658);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-329.249539786415085, 342.785487777791332);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-344.166185048751572, 344.724823108705550);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-360.536904928675426, 346.451884952948035);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-378.214963289628486, 347.970724570134507);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-397.053623995052874, 349.285393219880916);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-416.906150908390600, 350.399942161802983);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-437.625807893083675, 351.318422655516372);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-459.065858812573993, 352.044885960637032);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-481.079567530303621, 352.583383336780628);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-503.520197909714511, 352.937966043563051);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-526.241013814248618, 353.112685340600024);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-549.095279107348233, 353.111592487507210);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-571.936257652454970, 352.938738743900558);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-594.617213313011007, 352.598175369395790);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-616.991409952458298, 352.093953623608741);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-638.912111434238795, 351.430124766155018);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-660.232581621794679, 350.610740056650570);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-680.806084378567789, 349.639850754711119);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-700.485883568000077, 348.521508119952557);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-719.125243053533723, 347.259763411990377);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-736.577426698610566, 345.858667890440699);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-752.695698366672559, 344.322272814919074);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-767.333321921161996, 342.654629445041451);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-780.343561225520489, 340.859789040423379);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-791.579680143190444, 338.941802860680923);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-800.894942537613588, 336.904722165429632);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-806.464504884082544, 335.297692803701807);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-809.543928560835866, 334.193629625334438);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-812.060599029963441, 333.062071651623739);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-813.996174274395912, 331.903525290021662);
    p.SetSABefore(37.7953);
    p.SetSAAfter(37.7953);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(-814.714960629921279, 331.313196850393751);
    p.SetSABefore(37.7953);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeRightAngle);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue923Test6_6()
{
    QVector<QPointF> points;

    points += QPointF(-831.030698870537890, 27.866127576820318);
    points += QPointF(-797.079525240757903, -48.524013090184425);
    points += QPointF(-789.140213051348155, -49.334956233568391);
    points += QPointF(-765.476716482266852, -52.393449188523491);
    points += QPointF(-737.995740821697950, -56.780221262468970);
    points += QPointF(-716.331603418113787, -61.039176834826399);
    points += QPointF(-702.082363720278977, -64.273346529881294);
    points += QPointF(-688.166746660068270, -67.876119132381135);
    points += QPointF(-674.983023423591590, -71.809762540754065);
    points += QPointF(-662.881277988431520, -76.026115008433663);
    points += QPointF(-652.000013358218780, -80.530014885308333);
    points += QPointF(-647.414888772498330, -82.758750527910010);
    points += QPointF(-638.790539302210846, -87.166916645655860);
    points += QPointF(-626.931066473615147, -93.766236503837689);
    points += QPointF(-616.484087855132884, -100.486313320968108);
    points += QPointF(-594.096383910663576, -115.972155307751649);
    points += QPointF(-183.958889296992538, 76.240441185503187);
    points += QPointF(-199.128208171590842, 118.670762821515268);
    points += QPointF(-205.825015171693934, 138.297993904680141);
    points += QPointF(-211.936233005300977, 157.335473918921082);
    points += QPointF(-217.496918215929867, 175.859969792696745);
    points += QPointF(-222.524915681533457, 193.897845117910578);
    points += QPointF(-227.038178126608216, 211.475993598800500);
    points += QPointF(-231.054715694933805, 228.621894034446882);
    points += QPointF(-234.592533040731553, 245.363653164792709);
    points += QPointF(-237.669555346488721, 261.730031318712463);
    points += QPointF(-240.303546018901216, 277.750445980247605);
    points += QPointF(-242.512020202613343, 293.454949161782793);
    points += QPointF(-244.303509443293109, 308.800083297848346);
    points += QPointF(-246.320840785439799, 331.491357717148105);
    points += QPointF(-247.367543015896416, 354.406709688632361);
    points += QPointF(-268.813396279081758, 365.986520876534257);
    points += QPointF(-274.509592515332827, 368.544465142791694);
    points += QPointF(-280.430827174231638, 370.738996867293565);
    points += QPointF(-286.539948814471813, 372.651089393430880);
    points += QPointF(-295.760523804982597, 375.070669023561095);
    points += QPointF(-309.108660700135488, 377.812272271492361);
    points += QPointF(-323.791841340653832, 380.189287868497217);
    points += QPointF(-339.746492413262274, 382.263575885230011);
    points += QPointF(-356.936262218254399, 384.077045039871223);
    points += QPointF(-375.281527693557905, 385.653208859605968);
    points += QPointF(-394.678657424405628, 387.006850647149122);
    points += QPointF(-414.787606725105888, 388.135795504602129);
    points += QPointF(-435.952027004089416, 389.076617984495158);
    points += QPointF(-457.785958265362524, 389.818484035946028);
    points += QPointF(-480.155299367881639, 390.367355935352180);
    points += QPointF(-502.923072150244479, 390.728524359008020);
    points += QPointF(-525.950383021197354, 390.906843496429246);
    points += QPointF(-549.097086414790738, 390.906868034847378);
    points += QPointF(-572.222272885870439, 390.732932110765603);
    points += QPointF(-595.184660117100748, 390.389190982768582);
    points += QPointF(-617.842942758232198, 389.879635420374768);
    points += QPointF(-640.056148638506556, 389.208081776043002);
    points += QPointF(-661.684051750900380, 388.378134611080782);
    points += QPointF(-682.587707912334736, 387.393111203971273);
    points += QPointF(-702.630213618706989, 386.255904969502012);
    points += QPointF(-721.913933499378572, 384.952760768147186);
    points += QPointF(-739.883081024661010, 383.510162372682714);
    points += QPointF(-756.628370636720547, 381.913999891494996);
    points += QPointF(-772.055707413409664, 380.156385631588421);
    points += QPointF(-786.107417618598561, 378.217868080892174);
    points += QPointF(-798.800506213738572, 376.051179682334919);
    points += QPointF(-810.181063246566509, 373.562456117590386);
    points += QPointF(-814.714960629921279, 372.254255263580490);
    points += QPointF(-965.896062992125962, 372.254255263580490);
    points += QPointF(-831.030698870537890, 27.866127576820318);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue937Case1() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(30.000000000000000, 39.999874015748034);
    points += VSAPoint(784.001052525227124, 93.624081022807857);

    VSAPoint p = VSAPoint(603.941048041563704, 630.489688374883372);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    p = VSAPoint(149.328168036696184, 622.197225159899745);
    p.SetSABefore(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue937Case1() const
{
    QVector<QPointF> points;

    points += QPointF(-17.032721988884830, -1.235810543606590);
    points += QPointF(835.348863103112194, 59.385174553028691);
    points += QPointF(643.563032046223725, 631.212421626215360);
    points += QPointF(688.453528504066298, 783.237507335320970);
    points += QPointF(73.980221969375577, 772.029075909062840);
    points += QPointF(110.602400699420087, 621.490839539868603);
    points += QPointF(-17.032721988884830, -1.235810543606590);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue937Case2() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(1133.858267716535465, 0.000000000000000);
    points += VSAPoint(1626.414077999261963, 28.823250050281736);

    VSAPoint p = VSAPoint(1708.662782184241905, 444.933722174073409);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    p = VSAPoint(978.059754412127063, 473.971248193821054);
    p.SetSABefore(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue937Case2() const
{
    QVector<QPointF> points;

    points += QPointF(1107.034334293208985, -39.429607754508027);
    points += QPointF(1657.820450488061852, -7.198852132733364);
    points += QPointF(1746.889006868886327, 443.414436116222419);
    points += QPointF(1729.223740724144136, 595.416995674621376);
    points += QPointF(1000.148236459051077, 624.393810881663740);
    points += QPointF(937.748317798696917, 475.573410168327996);
    points += QPointF(1107.034334293208985, -39.429607754508027);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue937Case3() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(1133.858267716535465, 0.000000000000000);
    points += VSAPoint(1626.414077999261963, 28.823250050281736);
    points += VSAPoint(1626.414077999261963, 28.823250050281736);

    VSAPoint p = VSAPoint(1627.925525207237797, 60.028107039649264);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1632.474444857430171, 117.651054706235584);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1638.880247750070112, 170.780189339920014);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1647.171437175555184, 220.804740431454292);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1657.376516424283182, 269.113937471590248);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1669.523988786652353, 317.097009951079542);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1683.642357553060720, 366.143187360674062);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1699.760126013905392, 417.641699191125440);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1708.662782184241905, 444.933722174073409);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    p = VSAPoint(1708.662782184241905, 444.933722174073409);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    p = VSAPoint(1708.662782184241905, 444.933722174073409);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    p = VSAPoint(1677.251229240104976, 451.246684764538941);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1605.086594041950320, 463.155242122690083);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1544.272339765547713, 471.094839513925308);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1501.274196476889529, 475.816264599789406);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1456.403111491504887, 479.956529052348401);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1409.875251575960647, 483.430528153818273);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1361.906783496823664, 486.153157186415115);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1312.713874020660569, 488.039311432354793);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1262.512689914038219, 489.003886173853402);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1211.519397943523927, 488.961776693126922);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1159.950164875684550, 487.827878272391445);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1108.021157477086035, 485.517086193862838);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1055.948542514296605, 481.944295739757194);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1003.948486753882094, 477.024402192290495);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(978.059754412127063, 473.971248193821054);
    p.SetSABefore(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    p = VSAPoint(978.059754412127063, 473.971248193821054);
    p.SetSABefore(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    p = VSAPoint(978.059754412127063, 473.971248193821054);
    p.SetSABefore(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    p = VSAPoint(984.500664049167881, 439.858339084691238);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(998.403341335729920, 374.895601998811230);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1013.735113164965696, 313.199834991297735);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1030.722718345428348, 254.050229972719023);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1049.592895685671465, 196.725978853643085);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1070.572383994247502, 140.506273544638077);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1093.887922079709597, 84.670305956272159);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(1119.766248750610885, 28.497267999113369);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(1133.858267716535465, 0.000000000000000);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue937Case3() const
{
    QVector<QPointF> points;

    points += QPointF(1111.075514893730315, -39.193127021735194);
    points += QPointF(1662.522212799755380, -6.923715654880951);
    points += QPointF(1665.648765146942878, 57.626086911110946);
    points += QPointF(1670.091199415827987, 113.900142527237790);
    points += QPointF(1676.303630411694712, 165.425468596995557);
    points += QPointF(1684.322260121831278, 213.805541833033089);
    points += QPointF(1694.200361802327961, 260.566879872136496);
    points += QPointF(1706.013423451030349, 307.229014708540717);
    points += QPointF(1719.842343091969724, 355.269667089451730);
    points += QPointF(1735.830074328750925, 406.352691903936886);
    points += QPointF(1745.972128474848660, 437.435445733357255);
    points += QPointF(1758.341772942237640, 589.153524481990871);
    points += QPointF(1704.460967178960573, 599.982263568295707);
    points += QPointF(1627.185999041022114, 612.734124474861687);
    points += QPointF(1562.310551416402404, 621.203930457757906);
    points += QPointF(1516.471248721796655, 626.237329974753152);
    points += QPointF(1468.977621912049472, 630.619576913233686);
    points += QPointF(1419.787864301955324, 634.292326402446633);
    points += QPointF(1369.087082921324964, 637.170038051159622);
    points += QPointF(1317.062578165990772, 639.164761306646824);
    points += QPointF(1263.902550577329066, 640.186187808469754);
    points += QPointF(1209.795108673992672, 640.141506710349972);
    points += QPointF(1154.927472101428293, 638.935083369183076);
    points += QPointF(1099.485310435284873, 636.467959223881508);
    points += QPointF(1043.652191269731475, 632.637154196705978);
    points += QPointF(987.972975362633747, 627.369163402013896);
    points += QPointF(933.137693238166889, 620.902235679736577);
    points += QPointF(940.434492417377555, 469.533961825902566);
    points += QPointF(947.446497080812378, 432.396369275163750);
    points += QPointF(961.574442359626801, 366.381027352609578);
    points += QPointF(977.219989780144260, 303.422612688453626);
    points += QPointF(994.595947496389158, 242.920792401422716);
    points += QPointF(1013.924089039867795, 184.205327277493041);
    points += QPointF(1035.416023464977798, 126.612396404806589);
    points += QPointF(1059.274426966970623, 69.476376649892870);
    points += QPointF(1085.656320891028145, 12.210265605542077);
    points += QPointF(1111.075514893730315, -39.193127021735194);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue937Case4() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(30.000000000000000, 39.999874015748034);
    points += VSAPoint(784.001052525227124, 93.624081022807857);
    points += VSAPoint(784.001052525227124, 93.624081022807857);

    VSAPoint p = VSAPoint(774.549887508210873, 111.576706533308567);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(756.790196946771857, 147.235978663837784);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(740.328372396175155, 182.599152839796318);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(725.082967905009809, 217.665203187846430);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(710.972537521864297, 252.433103834650353);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(697.915635295327547, 286.901828906870151);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(680.021245070692316, 338.079435747169612);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(659.251258140338564, 405.209521964051703);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(641.229356711019818, 471.124440162517772);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(625.303973171447183, 535.815983355865228);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(610.823539910331306, 599.275944557391995);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(603.941048041563704, 630.489688374883372);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    p = VSAPoint(603.941048041563704, 630.489688374883372);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    p = VSAPoint(603.941048041563704, 630.489688374883372);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::BySecondEdgeSymmetry);
    points += p;

    p = VSAPoint(593.373754598520122, 627.421080848202337);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(570.856225791667612, 621.724582828565644);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(546.814781491491203, 616.591938481713441);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(521.400104821985224, 612.062029176884039);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(494.762878907143659, 608.173736283315634);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(467.053786870960835, 604.965941170246310);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(438.423511837430851, 602.477525206914265);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(409.022736930547808, 600.747369762557469);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(379.002145274305917, 599.814356206414232);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(348.512419992699336, 599.717365907722524);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(317.704244209722162, 600.495280235720656);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(286.728301049368611, 602.186980559646713);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(255.735273635632836, 604.831348248738777);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(224.875845092508996, 608.467264672235046);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(194.300698543991246, 613.133611199373604);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(164.160517114073684, 618.869269199392420);
    p.SetSABefore(151.181);
    p.SetSAAfter(151.181);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(149.328168036696184, 622.197225159899745);
    p.SetSABefore(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    p = VSAPoint(149.328168036696184, 622.197225159899745);
    p.SetSABefore(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    p = VSAPoint(149.328168036696184, 622.197225159899745);
    p.SetSABefore(151.181);
    p.SetAngleType(PieceNodeAngle::ByFirstEdgeSymmetry);
    points += p;

    p = VSAPoint(148.780908204593317, 607.287773847850531);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(146.895584757900821, 576.215879988244183);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(142.454824990190019, 527.210027117314212);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(133.311692121295692, 457.608980413459847);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(120.868745144514008, 384.408601325998120);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(105.351458627140062, 308.706628739851681);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(86.985307136468961, 231.600801539943319);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(65.995765239795787, 154.188858611195911);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    p = VSAPoint(42.608307504415649, 77.568538838532248);
    p.SetAngleType(PieceNodeAngle::ByLengthCurve);
    points += p;

    points += VSAPoint(30.000000000000000, 39.999874015748034);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue937Case4() const
{
    QVector<QPointF> points;

    points += QPointF(-23.869134810321157, -1.722013069577457);
    points += QPointF(844.400089565605640, 60.028893603376154);
    points += QPointF(808.191896751323043, 128.806882860412060);
    points += QPointF(790.844266519939765, 163.638788579752315);
    points += QPointF(774.796911740885662, 198.111601746414436);
    points += QPointF(759.929306088834664, 232.308674618563742);
    points += QPointF(746.160069074303465, 266.235879636728441);
    points += QPointF(733.432348408725375, 299.835603124447687);
    points += QPointF(715.924983245483418, 349.906325841807984);
    points += QPointF(695.543108629380072, 415.782005245395908);
    points += QPointF(677.813698549770493, 480.627139355570648);
    points += QPointF(662.080521256304564, 544.537908971868887);
    points += QPointF(647.671698829034426, 607.684038352516041);
    points += QPointF(640.315187177502366, 641.052275448924206);
    points += QPointF(588.619170631467227, 783.466677004755525);
    points += QPointF(553.743081963821624, 773.339104721127455);
    points += QPointF(536.523003591512065, 768.982758907727316);
    points += QPointF(517.759929550071433, 764.977001819844645);
    points += QPointF(497.211523376831963, 761.314456058038559);
    points += QPointF(475.147325760354477, 758.093697895431433);
    points += QPointF(451.812536560994317, 755.392302170863218);
    points += QPointF(427.435437524786209, 753.273553318105542);
    points += QPointF(402.232663995440760, 751.790438809294415);
    points += QPointF(376.413029414726907, 750.987987300199620);
    points += QPointF(350.180371031112031, 750.904539077934487);
    points += QPointF(323.735723660862391, 751.572273196081824);
    points += QPointF(297.279016585358193, 753.017162854419439);
    points += QPointF(271.010406542700196, 755.258436761187795);
    points += QPointF(245.131291735141957, 758.307563170734056);
    points += QPointF(219.844984732118604, 762.166732600404089);
    points += QPointF(194.848860551284815, 766.923479667790730);
    points += QPointF(172.371358664737528, 771.966789799612116);
    points += QPointF(111.816375434788142, 630.613801194321582);
    points += QPointF(111.027644362461615, 609.125749575465761);
    points += QPointF(109.203764340933944, 579.066500834362159);
    points += QPointF(104.882425391242222, 531.378512325319548);
    points += QPointF(95.931314232348498, 463.239208789540044);
    points += QPointF(83.715017334998123, 391.372185197808790);
    points += QPointF(68.446160239316853, 316.882189799137393);
    points += QPointF(50.354130013853364, 240.927194166617255);
    points += QPointF(29.673297258902259, 164.653807504519932);
    points += QPointF(6.611479789332002, 89.100327128404913);
    points += QPointF(-23.869134810321157, -1.722013069577457);

    return points;
}
