/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 11, 2016
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
    QTest::newRow("Seam test 1") << InputPointsCase1()
                                 << 37.795275590551185 // seam allowance width
                                 << OutputPointsCase1();

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
    // Code should clean loops in path.
    QTest::newRow("Issue 298. Case1") << InputPointsIssue298Case1()
                                      << 75.5906 // seam allowance width
                                      << OutputPointsIssue298Case1();

    QTest::newRow("Issue 298. Case2") << InputPointsIssue298Case2()
                                      << 37.7953 // seam allowance width
                                      << OutputPointsIssue298Case2();

    // See issue #548. Bug Detail tool. Case when seam allowance is wrong.
    // https://bitbucket.org/dismine/valentina/issues/548/bug-detail-tool-case-when-seam-allowance
    // Files: Steampunk_trousers.val and marie.vit
    // Actually buggy detail see in file src/app/share/collection/bugs/Steampunk_trousers_issue_#548.val
    // Code should clean loops in path.
    QTest::newRow("Issue 548. Case1") << InputPointsIssue548Case1()
                                      << 11.338582677165354 // seam allowance width (0.3 cm)
                                      << OutputPointsIssue548Case1();

#ifndef Q_OS_WIN // Disabled due to "undefined behavior" problem
#ifndef Q_CC_CLANG
    QTest::newRow("Issue 548. Case2") << InputPointsIssue548Case2()
                                      << 37.795275590551185 // seam allowance width (1.0 cm)
                                      << OutputPointsIssue548Case2();

    QTest::newRow("Issue 548. Case3") << InputPointsIssue548Case3()
                                      << 75.59055118110237 // seam allowance width (2.0 cm)
                                      << OutputPointsIssue548Case3();
#endif
#endif

    QTest::newRow("Issue 646.") << InputPointsIssue646()
                                << 37.795275590551185 // seam allowance width (1.0 cm
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
QVector<VSAPoint> TST_VAbstractPiece::InputPointsCase1() const
{

    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(30.0, 894.8030236220472);
    p.SetSAAfter(0);
    p.SetSABefore(75.59055118110237);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    //////p.SetSAAfter(-1);
    p.SetSABefore(0);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    //////p.SetSAAfter(-1);
    p.SetSABefore(0);
    points.append(p);

    p = VSAPoint(47.64159471849116, 39.94788491648882);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(76.58589406160007, 39.22029871626823);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(96.8510955083063, 38.214779786537555);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(117.72526111352542, 36.63514697516433);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(139.00155591749595, 34.36192982961571);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(160.47314496045635, 31.27565789735886);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(181.9331932826451, 27.25686072586095);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(203.17486592430072, 22.18606786258914);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(223.99132792566166, 15.943808855010598);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(239.1756066691363, 10.378329140641144);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(249.07102191481937, 6.266596479372291);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(258.7437898355629, 1.7947951524770347);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(268.16805606139667, -3.0520086466112275);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(277.3179662223507, -8.2887487244591);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(286.16766594845456, -13.93035888763319);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(294.69130086973814, -19.991772942700095);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(302.86301661623133, -26.48792469622643);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(310.6569588179638, -33.433747954778795);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(318.04727310496537, -40.84417652492378);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(325.00810510726603, -48.73414421322801);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(331.51360045489537, -57.118584826258086);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(334.56692913385825, -61.522435695538036);
    p.SetSAAfter(0);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(334.56692913385825, -61.522435695538036);
    p.SetSAAfter(0);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(519.4465667350105, -36.01405338211436);
    //////p.SetSAAfter(-1);
    p.SetSABefore(0);
    points.append(p);

    p = VSAPoint(460.3937007874016, 342.36207874015753);
    //////p.SetSAAfter(-1);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(620.5290071875437, -5.50631876178551);
    p.SetSAAfter(0);
    //////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(944.2945933263424, 136.63905516701567);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(0);
    points.append(p);

    p = VSAPoint(944.2945933263424, 136.63905516701567);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(0);
    points.append(p);

    p = VSAPoint(937.525981220313, 150.5000566598672);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(924.7444201119979, 177.43472401968558);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(912.8425448338431, 203.43742191080565);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(901.7902332810357, 228.52240953585334);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(891.5573633487625, 252.7039460974546);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(882.1138129322104, 275.99629079823535);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(869.2733175133695, 309.4054580978942);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(854.7606685168345, 350.82610676604395);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(842.8630996965477, 388.94665363817853);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(833.3396342140044, 423.88117233530545);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(825.9492952306994, 455.7437364784321);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(820.4511059081283, 484.6484196885659);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(816.6040894077858, 510.7092955867147);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(814.1672688911672, 534.0404377938855);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(812.8996675197675, 554.7559199310863);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(812.5603084550817, 572.9698156193242);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(812.9082148586052, 588.7961984796068);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(814.1809574610598, 608.5715133604979);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(815.1968503937007, 618.5825511811024);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(815.1968503937007, 618.5825511811024);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(815.7375399808675, 623.4475087782134);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(817.2456173381056, 633.4974833757362);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(819.2665464515061, 643.8914388413946);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(821.8012316951542, 654.584830268716);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(824.8505774431355, 665.5331127512278);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(828.4154880695357, 676.6917413824576);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(832.4968679484402, 688.0161712559327);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(837.0956214539344, 699.4618574651809);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(842.212652960104, 710.9842551037295);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(847.8488668410344, 722.5388192651058);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(854.0051674708111, 734.0810050428374);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(860.6824592235198, 745.5662675304518);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(867.8816464732456, 756.9500618214765);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(875.6036335940746, 768.187843009439);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(883.8493249600917, 779.2350661878667);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(892.6196249453828, 790.0471864502871);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(901.9154379240335, 800.5796588902277);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(911.7376682701291, 810.7879386012161);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(922.0872203577552, 820.6274806767794);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(932.9649985609971, 830.0537402104453);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(944.3719072539405, 839.0221722957415);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(956.3088508106711, 847.4882320261951);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(968.776733605274, 855.4073744953336);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(981.7764600118351, 862.7350547966848);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(995.3089344044396, 869.4267280237759);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(1009.3750611571733, 875.4378492701345);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(1023.9757446441214, 880.7238736292878);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(1039.1118892393697, 885.2402561947638);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(1054.784399317004, 888.9424520600894);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(1070.994179251109, 891.7859163187926);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(1087.7421334157707, 893.7261040644007);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(1105.0291661850742, 894.7184703904409);
    p.SetSAAfter(188.97637795275591);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSAAfter(0);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    p = VSAPoint(1113.8976377952758, 894.8030236220472);
    p.SetSAAfter(0);
    p.SetSABefore(188.97637795275591);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsCase1() const
{
    QVector<QPointF> points;

    points += QPointF(30.0, 970.3935748031496);
    points += QPointF(30.0, 2.204434307474013);
    points += QPointF(47.110950439162494, 2.154008996440882);
    points += QPointF(75.17428353484098, 1.4485680510386028);
    points += QPointF(94.48820285018608, 0.4902498458165696);
    points += QPointF(114.29080866669788, -1.0082936883322242);
    points += QPointF(134.30369213200785, -3.146524533710341);
    points += QPointF(154.30324182399514, -6.0212092507409025);
    points += QPointF(174.06202812984606, -9.721413151776975);
    points += QPointF(193.35089649585723, -14.32603434620921);
    points += QPointF(212.04908234619523, -19.93308318294329);
    points += QPointF(225.4146962614004, -24.83196935145066);
    points += QPointF(233.88383173589082, -28.35105572413872);
    points += QPointF(242.16354279128632, -32.17883523742874);
    points += QPointF(250.12919225691988, -36.27548734456109);
    points += QPointF(257.76141086050376, -40.64361165848513);
    points += QPointF(265.0438386703493, -45.28609897626606);
    points += QPointF(271.9635110924753, -50.20688688697322);
    points += QPointF(278.5110730948284, -55.4119084151831);
    points += QPointF(284.68071875165725, -60.91018735939974);
    points += QPointF(290.46975730117975, -66.71498198586181);
    points += QPointF(295.87773947883056, -72.84482460043537);
    points += QPointF(301.0311752209645, -79.48669791277798);
    points += QPointF(292.5914051639158, -67.31392104851938);
    points += QPointF(556.8930273120665, -30.847456102511416);
    points += QPointF(539.6101141051189, 79.89131577778163);
    points += QPointF(585.9167315845334, -20.702420721823447);
    points += QPointF(1117.4712505569892, 212.6701769158142);
    points += QPointF(1107.8013393916237, 232.47256047676322);
    points += QPointF(1096.0328222042483, 257.2724337531229);
    points += QPointF(1085.2347243947604, 280.86364678273935);
    points += QPointF(1075.284743777034, 303.4467181585846);
    points += QPointF(1066.1500107021461, 325.033221582634);
    points += QPointF(1057.8916931702825, 345.40221101096097);
    points += QPointF(1046.6844069650929, 374.5619971088514);
    points += QPointF(1034.1843603449327, 410.2384638595294);
    points += QPointF(1024.281900134066, 441.9665592645446);
    points += QPointF(1016.6128103198599, 470.0987514791772);
    points += QPointF(1010.8915481272591, 494.7652903217387);
    points += QPointF(1006.8300406648394, 516.1171535751373);
    points += QPointF(1004.140951447517, 534.3338730421456);
    points += QPointF(1002.5429209641061, 549.6340874837038);
    points += QPointF(1001.7684597861733, 562.2905401031172);
    points += QPointF(1001.5753826870504, 572.6532694631434);
    points += QPointF(1001.7511114738644, 580.6472328726268);
    points += QPointF(1002.5434394846042, 592.958077085046);
    points += QPointF(1003.1165308093626, 598.6055609315013);
    points += QPointF(1003.5253444923072, 601.4269775229475);
    points += QPointF(1004.0349710465857, 604.0480668398186);
    points += QPointF(1004.8346806929111, 607.4219012430418);
    points += QPointF(1005.9446128953437, 611.4069696127774);
    points += QPointF(1007.3734895026099, 615.879537116226);
    points += QPointF(1009.1216689960736, 620.7301358517914);
    points += QPointF(1011.1831553003773, 625.860901250618);
    points += QPointF(1013.5469326629959, 631.1835932583286);
    points += QPointF(1016.1978697144372, 636.6181674061058);
    points += QPointF(1019.1173567112357, 642.0917891993197);
    points += QPointF(1022.283791807397, 647.5382096308747);
    points += QPointF(1025.6730034935645, 652.8974380139866);
    points += QPointF(1029.258679857382, 658.1156604426072);
    points += QPointF(1033.0128662502555, 663.1453581470851);
    points += QPointF(1036.906587776057, 667.9455799725583);
    points += QPointF(1040.910648783598, 672.4823173517011);
    points += QPointF(1044.9966554234593, 676.7289189086724);
    points += QPointF(1049.138296281844, 680.666466074281);
    points += QPointF(1053.3128972071952, 684.2840128781046);
    points += QPointF(1057.5032369521043, 687.5785757108713);
    points += QPointF(1061.6995688685456, 690.5547476668297);
    points += QPointF(1065.9017414956827, 693.2238137995831);
    points += QPointF(1070.1212524492987, 695.6022659553563);
    points += QPointF(1074.3830151134682, 697.7096649640421);
    points += QPointF(1078.7265805419665, 699.565875909043);
    points += QPointF(1083.206554246492, 701.1878034134409);
    points += QPointF(1087.8919933013688, 702.5858632116882);
    points += QPointF(1092.8646688655851, 703.7605199081748);
    points += QPointF(1098.2162239125396, 704.699271425244);
    points += QPointF(1104.0444208482252, 705.3744462055462);
    points += QPointF(1111.3488014116317, 705.7937559506274);
    points += QPointF(1290.2705536650083, 707.499620616225);
    points += QPointF(1302.4161318705774, 881.6558256043734);
    points += QPointF(30.0, 970.3935748031496);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsCase2() const
{

    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(30.0, 39.999874015748034);
    ////p.SetSAAfter(-1);
    ////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(407.9527559055118, 39.999874015748034);
    ////p.SetSAAfter(-1);
    ////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(407.9527559055118, 228.97625196850396);
    ////p.SetSAAfter(-1);
    ////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(407.9527559055118, 228.97625196850396);
    ////p.SetSAAfter(-1);
    ////p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(403.3769166670402, 231.4465511704684);
    p.SetSAAfter(37.803178025111038);
    p.SetSABefore(37.803178025111038);
    points.append(p);

    p = VSAPoint(394.1607984354165, 235.58132461572228);
    p.SetSAAfter(37.818528535007879);
    p.SetSABefore(37.818528535007879);
    points.append(p);

    p = VSAPoint(384.8923171505302, 238.7905285112787);
    p.SetSAAfter(37.833434086432739);
    p.SetSABefore(37.833434086432739);
    points.append(p);

    p = VSAPoint(375.59921131499664, 241.12018552459608);
    p.SetSAAfter(37.847993627262561);
    p.SetSABefore(37.847993627262561);
    points.append(p);

    p = VSAPoint(366.3092194314313, 242.61631832313287);
    p.SetSAAfter(37.862293350550544);
    p.SetSABefore(37.862293350550544);
    points.append(p);

    p = VSAPoint(357.0500800024495, 243.3249495743475);
    p.SetSAAfter(37.876405426208798);
    p.SetSABefore(37.876405426208798);
    points.append(p);

    p = VSAPoint(347.8495315306667, 243.2921019456984);
    p.SetSAAfter(37.890387402651413);
    p.SetSABefore(37.890387402651413);
    points.append(p);

    p = VSAPoint(338.73531251869827, 242.56379810464406);
    p.SetSAAfter(37.904282247411182);
    p.SetSABefore(37.904282247411182);
    points.append(p);

    p = VSAPoint(329.7351614691596, 241.18606071864286);
    p.SetSAAfter(37.91811891950119);
    p.SetSABefore(37.91811891950119);
    points.append(p);

    p = VSAPoint(320.876816884666, 239.20491245515328);
    p.SetSAAfter(37.931913327979281);
    p.SetSABefore(37.931913327979281);
    points.append(p);

    p = VSAPoint(312.18801726783295, 236.6663759816338);
    p.SetSAAfter(37.945669528152067);
    p.SetSABefore(37.945669528152067);
    points.append(p);

    p = VSAPoint(303.6965011212758, 233.61647396554275);
    p.SetSAAfter(37.959381027682795);
    p.SetSABefore(37.959381027682795);
    points.append(p);

    p = VSAPoint(295.43000694760997, 230.10122907433865);
    p.SetSAAfter(37.973032106608528);
    p.SetSABefore(37.973032106608528);
    points.append(p);

    p = VSAPoint(287.41627324945074, 226.16666397547993);
    p.SetSAAfter(37.986599088053786);
    p.SetSABefore(37.986599088053786);
    points.append(p);

    p = VSAPoint(279.6830385294136, 221.85880133642502);
    p.SetSAAfter(38.000051524725791);
    p.SetSABefore(38.000051524725791);
    points.append(p);

    p = VSAPoint(272.2580412901139, 217.2236638246324);
    p.SetSAAfter(38.013353288194111);
    p.SetSABefore(38.013353288194111);
    points.append(p);

    p = VSAPoint(265.16902003416703, 212.3072741075605);
    p.SetSAAfter(38.026463563868212);
    p.SetSABefore(38.026463563868212);
    points.append(p);

    p = VSAPoint(258.44371326418843, 207.15565485266765);
    p.SetSAAfter(38.03933776597485);
    p.SetSABefore(38.03933776597485);
    points.append(p);

    p = VSAPoint(252.1098594827934, 201.81482872741242);
    p.SetSAAfter(38.051928395587325);
    p.SetSABefore(38.051928395587325);
    points.append(p);

    p = VSAPoint(246.19519719259745, 196.33081839925325);
    p.SetSAAfter(38.064185872874937);
    p.SetSABefore(38.064185872874937);
    points.append(p);

    p = VSAPoint(240.72746489621585, 190.74964653564848);
    p.SetSAAfter(38.076059384395691);
    p.SetSABefore(38.076059384395691);
    points.append(p);

    p = VSAPoint(235.73440109626404, 185.11733580405664);
    p.SetSAAfter(38.087497800100167);
    p.SetSABefore(38.087497800100167);
    points.append(p);

    p = VSAPoint(231.24374429535737, 179.47990887193612);
    p.SetSAAfter(38.098450736467157);
    p.SetSABefore(38.098450736467157);
    points.append(p);

    p = VSAPoint(227.2832329961113, 173.88338840674544);
    p.SetSAAfter(38.108869877607624);
    p.SetSABefore(38.108869877607624);
    points.append(p);

    p = VSAPoint(223.88060570114112, 168.37379707594295);
    p.SetSAAfter(38.118710724548052);
    p.SetSABefore(38.118710724548052);
    points.append(p);

    p = VSAPoint(221.06360091306237, 162.99715754698713);
    p.SetSAAfter(38.127935039089436);
    p.SetSABefore(38.127935039089436);
    points.append(p);

    p = VSAPoint(218.8599571344903, 157.79949248733644);
    p.SetSAAfter(38.136514404353001);
    p.SetSABefore(38.136514404353001);
    points.append(p);

    p = VSAPoint(217.2974128680403, 152.82682456444928);
    p.SetSAAfter(38.144435562325597);
    p.SetSABefore(38.144435562325597);
    points.append(p);

    p = VSAPoint(216.40370661632784, 148.12517644578412);
    p.SetSAAfter(38.15170849722444);
    p.SetSABefore(38.15170849722444);
    points.append(p);

    p = VSAPoint(216.20657688196826, 143.7405707987994);
    p.SetSAAfter(38.158378424653918);
    p.SetSABefore(38.158378424653918);
    points.append(p);

    p = VSAPoint(216.7337621675769, 139.71903029095353);
    p.SetSAAfter(38.164542166418755);
    p.SetSABefore(38.164542166418755);
    points.append(p);

    p = VSAPoint(218.01300097576927, 136.10657758970495);
    p.SetSAAfter(38.17036598425517);
    p.SetSABefore(38.17036598425517);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    p.SetSAAfter(38.173228346456696);
    p.SetSABefore(38.173228346456696);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    p.SetSAAfter(38.173228346456696);
    p.SetSABefore(38.173228346456696);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    p.SetSAAfter(38.173228346456696);
    p.SetSABefore(38.173228346456696);
    points.append(p);

    p = VSAPoint(217.1202289172026, 137.73030103616844);
    p.SetSAAfter(38.167995197589178);
    p.SetSABefore(38.167995197589178);
    points.append(p);

    p = VSAPoint(212.6973363405255, 143.4397404435662);
    p.SetSAAfter(38.157878743288215);
    p.SetSABefore(38.157878743288215);
    points.append(p);

    p = VSAPoint(207.48487786706698, 148.24866810991395);
    p.SetSAAfter(38.147944713012919);
    p.SetSABefore(38.147944713012919);
    points.append(p);

    p = VSAPoint(201.55301739671896, 152.18989767496004);
    p.SetSAAfter(38.137968823998783);
    p.SetSABefore(38.137968823998783);
    points.append(p);

    p = VSAPoint(194.9719188293733, 155.29624277845284);
    p.SetSAAfter(38.127775030254007);
    p.SetSABefore(38.127775030254007);
    points.append(p);

    p = VSAPoint(187.81174606492203, 157.6005170601407);
    p.SetSAAfter(38.117238836985599);
    p.SetSABefore(38.117238836985599);
    points.append(p);

    p = VSAPoint(180.14266300325704, 159.13553415977202);
    p.SetSAAfter(38.106283289982827);
    p.SetSABefore(38.106283289982827);
    points.append(p);

    p = VSAPoint(172.0348335442702, 159.93410771709506);
    p.SetSAAfter(38.094871286509886);
    p.SetSABefore(38.094871286509886);
    points.append(p);

    p = VSAPoint(163.55842158785353, 160.02905137185826);
    p.SetSAAfter(38.082997200945435);
    p.SetSABefore(38.082997200945435);
    points.append(p);

    p = VSAPoint(154.78359103389897, 159.4531787638099);
    p.SetSAAfter(38.070679409075119);
    p.SetSABefore(38.070679409075119);
    points.append(p);

    p = VSAPoint(145.78050578229832, 158.23930353269841);
    p.SetSAAfter(38.057954219352141);
    p.SetSABefore(38.057954219352141);
    points.append(p);

    p = VSAPoint(136.61932973294367, 156.42023931827214);
    p.SetSAAfter(38.044871166362007);
    p.SetSABefore(38.044871166362007);
    points.append(p);

    p = VSAPoint(127.37022678572683, 154.0287997602794);
    p.SetSAAfter(38.031489421641503);
    p.SetSABefore(38.031489421641503);
    points.append(p);

    p = VSAPoint(118.10336084053982, 151.09779849846862);
    p.SetSAAfter(38.017875048227786);
    p.SetSABefore(38.017875048227786);
    points.append(p);

    p = VSAPoint(108.88889579727454, 147.66004917258803);
    p.SetSAAfter(38.00409885919936);
    p.SetSABefore(38.00409885919936);
    points.append(p);

    p = VSAPoint(99.79699555582292, 143.7483654223861);
    p.SetSAAfter(37.990234686678903);
    p.SetSABefore(37.990234686678903);
    points.append(p);

    p = VSAPoint(90.8978240160769, 139.3955608876111);
    p.SetSAAfter(37.976357907221328);
    p.SetSABefore(37.976357907221328);
    points.append(p);

    p = VSAPoint(82.26154507792839, 134.63444920801146);
    p.SetSAAfter(37.962544096747962);
    p.SetSABefore(37.962544096747962);
    points.append(p);

    p = VSAPoint(73.95832264126932, 129.49784402333552);
    p.SetSAAfter(37.94886770258455);
    p.SetSABefore(37.94886770258455);
    points.append(p);

    p = VSAPoint(66.05832060599164, 124.01855897333157);
    p.SetSAAfter(37.935400622146403);
    p.SetSABefore(37.935400622146403);
    points.append(p);

    p = VSAPoint(58.63170287198729, 118.22940769774803);
    p.SetSAAfter(37.922210567500777);
    p.SetSABefore(37.922210567500777);
    points.append(p);

    p = VSAPoint(51.74863333914817, 112.16320383633325);
    p.SetSAAfter(37.90935907207141);
    p.SetSABefore(37.90935907207141);
    points.append(p);

    p = VSAPoint(45.47927590736623, 105.85276102883554);
    p.SetSAAfter(37.896898960241472);
    p.SetSABefore(37.896898960241472);
    points.append(p);

    p = VSAPoint(39.8937944765334, 99.33089291500332);
    p.SetSAAfter(37.884871055952601);
    p.SetSABefore(37.884871055952601);
    points.append(p);

    p = VSAPoint(35.062352946541615, 92.63041313458488);
    p.SetSAAfter(37.873299866072891);
    p.SetSABefore(37.873299866072891);
    points.append(p);

    p = VSAPoint(31.055115217282804, 85.78413532732864);
    p.SetSAAfter(37.86218797621639);
    p.SetSABefore(37.86218797621639);
    points.append(p);

    p = VSAPoint(27.94224518864889, 78.82487313298289);
    p.SetSAAfter(37.851509027526909);
    p.SetSABefore(37.851509027526909);
    points.append(p);

    p = VSAPoint(25.793906760531815, 71.78544019129603);
    p.SetSAAfter(37.841199561247805);
    p.SetSABefore(37.841199561247805);
    points.append(p);

    p = VSAPoint(24.68026383282351, 64.69865014201642);
    p.SetSAAfter(37.831150915024352);
    p.SetSABefore(37.831150915024352);
    points.append(p);

    p = VSAPoint(24.671480305415898, 57.597316624892386);
    p.SetSAAfter(37.821203708860601);
    p.SetSABefore(37.821203708860601);
    points.append(p);

    p = VSAPoint(25.837720078200917, 50.514253279672296);
    p.SetSAAfter(37.811148513024627);
    p.SetSABefore(37.811148513024627);
    points.append(p);

    p = VSAPoint(28.2491470510705, 43.48227374610451);
    p.SetSAAfter(37.800735391906976);
    p.SetSABefore(37.800735391906976);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    ////p.SetSAAfter(-1);
    ////p.SetSABefore(-1);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsCase2() const
{
    QVector<QPointF> points;

    points += QPointF(6.735602739585184, 2.204598425196849);
    points += QPointF(445.748031496063, 2.204598425196849);
    points += QPointF(445.748031496063, 251.48446223243934);
    points += QPointF(420.1672505436216, 265.34467354701);
    points += QPointF(408.16868834563587, 270.74969811280204);
    points += QPointF(395.75743565485675, 275.06821746196044);
    points += QPointF(383.28015968092507, 278.216263983686);
    points += QPointF(370.8288026520235, 280.2409452967658);
    points += QPointF(358.4874104434599, 281.2043381226534);
    points += QPointF(346.3281093878975, 281.1794055890767);
    points += QPointF(334.4094715702431, 280.2452307190486);
    points += QPointF(322.77695514341303, 278.48262025768446);
    points += QPointF(311.46472096401544, 275.97070996938834);
    points += QPointF(300.4980766475991, 272.78475431818316);
    points += QPointF(289.8959627998986, 268.994968265893);
    points += QPointF(279.67312801429796, 264.6661417252929);
    points += QPointF(269.84184205634284, 259.8577269753623);
    points += QPointF(260.4131332789275, 254.62414553625888);
    points += QPointF(251.39760921382512, 249.0151257350064);
    points += QPointF(242.80594716150756, 243.07593816151942);
    points += QPointF(234.64914462550888, 236.84743207794344);
    points += QPointF(226.93861367892654, 230.36578881474733);
    points += QPointF(219.6862008839008, 223.6618981488572);
    points += QPointF(212.90422632696564, 216.76022967199233);
    points += QPointF(206.60567695694368, 209.67701009653408);
    points += QPointF(200.8047886617048, 202.41742546763192);
    points += QPointF(195.51846445950318, 194.97144814919497);
    points += QPointF(195.49646472111115, 194.93594659378365);
    points += QPointF(185.71068181674897, 196.88038239808796);
    points += QPointF(174.06579870156523, 198.0108674081686);
    points += QPointF(162.4698365016369, 198.12450807507133);
    points += QPointF(150.93599464376967, 197.35134057907845);
    points += QPointF(139.4731688630524, 195.78946993073865);
    points += QPointF(128.09385198069478, 193.51339391764853);
    points += QPointF(116.81695231109029, 190.58079085562875);
    points += QPointF(105.66811291289228, 187.03735678012447);
    points += QPointF(94.6789709925457, 182.91996300992662);
    points += QPointF(83.88608373098256, 178.2585220934185);
    points += QPointF(73.32986575781915, 173.07687844159756);
    points += QPointF(63.05370660182625, 167.39293026852914);
    points += QPointF(53.10337911388009, 161.21809122399117);
    points += QPointF(43.52687054865665, 154.55612780781823);
    points += QPointF(34.37485617471233, 147.40137397927876);
    points += QPointF(25.702201154685437, 139.73635741878132);
    points += QPointF(17.571137619050678, 131.52904674063026);
    points += QPointF(10.057109595108484, 122.7304016692299);
    points += QPointF(3.2585649313074336, 113.27395345986065);
    points += QPointF(-2.688329688884947, 103.08109119247632);
    points += QPointF(-7.591302782642628, 92.07847647311537);
    points += QPointF(-11.187498579572614, 80.23547322967522);
    points += QPointF(-13.151329717912063, 67.62324223181253);
    points += QPointF(-13.149175120030888, 54.474729701833986);
    points += QPointF(-10.94337542157982, 41.194055939258014);
    points += QPointF(-6.654890183208465, 28.745230608885407);
    points += QPointF(6.735602739585184, 2.204598425196849);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsCase3() const
{
    QVector<VSAPoint> points;

    VSAPoint p = VSAPoint(30.0, 39.999874015748034);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(407.9527559055118, 39.999874015748034);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(407.9527559055118, 228.97625196850396);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(407.9527559055118, 228.97625196850396);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(406.33770829042953, 232.38064611626584);
    p.SetSAAfter(37.381380099110068);
    p.SetSABefore(37.381380099110068);
    points.append(p);

    p = VSAPoint(402.870644734503, 238.5569810399819);
    p.SetSAAfter(36.603370955765051);
    p.SetSABefore(36.603370955765051);
    points.append(p);

    p = VSAPoint(399.14591058639644, 243.99520937172397);
    p.SetSAAfter(35.879339211886929);
    p.SetSABefore(35.879339211886929);
    points.append(p);

    p = VSAPoint(395.19316295581496, 248.71848383837556);
    p.SetSAAfter(35.202813177025682);
    p.SetSABefore(35.202813177025682);
    points.append(p);

    p = VSAPoint(391.04205895246355, 252.7499571668203);
    p.SetSAAfter(34.56719781657663);
    p.SetSABefore(34.56719781657663);
    points.append(p);

    p = VSAPoint(386.72225568604733, 256.1127820839417);
    p.SetSAAfter(33.965870279175178);
    p.SetSABefore(33.965870279175178);
    points.append(p);

    p = VSAPoint(382.2634102662714, 258.8301113166233);
    p.SetSAAfter(33.392312673818843);
    p.SetSABefore(33.392312673818843);
    points.append(p);

    p = VSAPoint(377.6951798028408, 260.9250975917487);
    p.SetSAAfter(32.840273714828662);
    p.SetSABefore(32.840273714828662);
    points.append(p);

    p = VSAPoint(373.0472214054606, 262.4208936362013);
    p.SetSAAfter(32.303940879158468);
    p.SetSABefore(32.303940879158468);
    points.append(p);

    p = VSAPoint(368.34919218383584, 263.34065217686486);
    p.SetSAAfter(31.778098176885351);
    p.SetSABefore(31.778098176885351);
    points.append(p);

    p = VSAPoint(363.6307492476716, 263.7075259406228);
    p.SetSAAfter(31.25824538248542);
    p.SetSABefore(31.25824538248542);
    points.append(p);

    p = VSAPoint(358.921549706673, 263.5446676543588);
    p.SetSAAfter(30.740662987580134);
    p.SetSABefore(30.740662987580134);
    points.append(p);

    p = VSAPoint(354.251250670545, 262.87523004495625);
    p.SetSAAfter(30.222419479548449);
    p.SetSABefore(30.222419479548449);
    points.append(p);

    p = VSAPoint(349.6495092489928, 261.72236583929885);
    p.SetSAAfter(29.701328542831337);
    p.SetSABefore(29.701328542831337);
    points.append(p);

    p = VSAPoint(345.1459825517213, 260.1092277642701);
    p.SetSAAfter(29.175869934623716);
    p.SetSABefore(29.175869934623716);
    points.append(p);

    p = VSAPoint(340.7703276884358, 258.0589685467535);
    p.SetSAAfter(28.645088729374258);
    p.SetSABefore(28.645088729374258);
    points.append(p);

    p = VSAPoint(336.5522017688411, 255.59474091363262);
    p.SetSAAfter(28.108485094878901);
    p.SetSABefore(28.108485094878901);
    points.append(p);

    p = VSAPoint(332.52126190264244, 252.73969759179104);
    p.SetSAAfter(27.565902911541084);
    p.SetSABefore(27.565902911541084);
    points.append(p);

    p = VSAPoint(328.7071651995449, 249.51699130811238);
    p.SetSAAfter(27.017421899167815);
    p.SetSABefore(27.017421899167815);
    points.append(p);

    p = VSAPoint(325.1395687692534, 245.94977478948007);
    p.SetSAAfter(26.463255170503039);
    p.SetSABefore(26.463255170503039);
    points.append(p);

    p = VSAPoint(321.84812972147313, 242.06120076277773);
    p.SetSAAfter(25.903652411137717);
    p.SetSABefore(25.903652411137717);
    points.append(p);

    p = VSAPoint(318.86250516590917, 237.87442195488893);
    p.SetSAAfter(25.338808050829666);
    p.SetSABefore(25.338808050829666);
    points.append(p);

    p = VSAPoint(316.2123522122665, 233.4125910926972);
    p.SetSAAfter(24.768773642603289);
    p.SetSABefore(24.768773642603289);
    points.append(p);

    p = VSAPoint(313.9273279702502, 228.69886090308609);
    p.SetSAAfter(24.19337403613525);
    p.SetSABefore(24.19337403613525);
    points.append(p);

    p = VSAPoint(312.03708954956545, 223.75638411293914);
    p.SetSAAfter(23.612127698143627);
    p.SetSABefore(23.612127698143627);
    points.append(p);

    p = VSAPoint(310.57129405991714, 218.60831344913998);
    p.SetSAAfter(23.024172586797707);
    p.SetSABefore(23.024172586797707);
    points.append(p);

    p = VSAPoint(309.55959861101053, 213.27780163857204);
    p.SetSAAfter(22.42820018785644);
    p.SetSABefore(22.42820018785644);
    points.append(p);

    p = VSAPoint(309.03166031255046, 207.788001408119);
    p.SetSAAfter(21.822401444747854);
    p.SetSABefore(21.822401444747854);
    points.append(p);

    p = VSAPoint(309.01713627424215, 202.1620654846643);
    p.SetSAAfter(21.204429040574471);
    p.SetSABefore(21.204429040574471);
    points.append(p);

    p = VSAPoint(309.5456836057906, 196.42314659509157);
    p.SetSAAfter(20.57138043760828);
    p.SetSABefore(20.57138043760828);
    points.append(p);

    p = VSAPoint(310.64695941690104, 190.59439746628436);
    p.SetSAAfter(19.919804940549692);
    p.SetSABefore(19.919804940549692);
    points.append(p);

    p = VSAPoint(312.35062081727835, 184.6989708251262);
    p.SetSAAfter(19.24573577317911);
    p.SetSABefore(19.24573577317911);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSAAfter(18.897637795275593);
    p.SetSABefore(18.897637795275593);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSAAfter(18.897637795275593);
    p.SetSABefore(18.897637795275593);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSAAfter(18.897637795275593);
    p.SetSABefore(18.897637795275593);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSAAfter(18.897637795275593);
    p.SetSABefore(18.897637795275593);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSAAfter(18.897637795275593);
    p.SetSABefore(18.897637795275593);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSAAfter(18.897637795275593);
    p.SetSABefore(18.897637795275593);
    points.append(p);

    p = VSAPoint(313.4645669291343, 181.73215748031558);
    p.SetSAAfter(18.897637795275593);
    p.SetSABefore(18.897637795275593);
    points.append(p);

    p = VSAPoint(315.0998723566189, 177.44976641127198);
    p.SetSAAfter(19.446824166328746);
    p.SetSABefore(19.446824166328746);
    points.append(p);

    p = VSAPoint(317.6731184586346, 169.45847196043132);
    p.SetSAAfter(20.452631980165769);
    p.SetSABefore(20.452631980165769);
    points.append(p);

    p = VSAPoint(319.4487651643931, 162.13409856250706);
    p.SetSAAfter(21.355546141728666);
    p.SetSABefore(21.355546141728666);
    points.append(p);

    p = VSAPoint(320.470410593102, 155.4530193599665);
    p.SetSAAfter(22.165276674578177);
    p.SetSABefore(22.165276674578177);
    points.append(p);

    p = VSAPoint(320.78165286396893, 149.39160749527696);
    p.SetSAAfter(22.892420458564697);
    p.SetSABefore(22.892420458564697);
    points.append(p);

    p = VSAPoint(320.4260900962016, 143.92623611090573);
    p.SetSAAfter(23.548583135932272);
    p.SetSABefore(23.548583135932272);
    points.append(p);

    p = VSAPoint(319.44732040900783, 139.03327834932014);
    p.SetSAAfter(24.146396878545747);
    p.SetSABefore(24.146396878545747);
    points.append(p);

    p = VSAPoint(317.88894192159495, 134.68910735298743);
    p.SetSAAfter(24.699324443869902);
    p.SetSABefore(24.699324443869902);
    points.append(p);

    p = VSAPoint(315.79455275317093, 130.870096264375);
    p.SetSAAfter(25.221147659605332);
    p.SetSABefore(25.221147659605332);
    points.append(p);

    p = VSAPoint(313.2077510229431, 127.55261822595011);
    p.SetSAAfter(25.725143604791594);
    p.SetSABefore(25.725143604791594);
    points.append(p);

    p = VSAPoint(310.1721348501194, 124.71304638018006);
    p.SetSAAfter(26.223136485113741);
    p.SetSABefore(26.223136485113741);
    points.append(p);

    p = VSAPoint(306.73130235390744, 122.32775386953216);
    p.SetSAAfter(26.724730852906816);
    p.SetSABefore(26.724730852906816);
    points.append(p);

    p = VSAPoint(302.9288516535148, 120.3731138364737);
    p.SetSAAfter(27.236947738833074);
    p.SetSABefore(27.236947738833074);
    points.append(p);

    p = VSAPoint(298.80838086814924, 118.82549942347202);
    p.SetSAAfter(27.764271724915222);
    p.SetSABefore(27.764271724915222);
    points.append(p);

    p = VSAPoint(294.41348811701835, 117.66128377299438);
    p.SetSAAfter(28.308962477298437);
    p.SetSABefore(28.308962477298437);
    points.append(p);

    p = VSAPoint(289.7877715193297, 116.85684002750813);
    p.SetSAAfter(28.871463995346204);
    p.SetSABefore(28.871463995346204);
    points.append(p);

    p = VSAPoint(282.5270013648352, 116.23547488513984);
    p.SetSAAfter(29.744519639698659);
    p.SetSABefore(29.744519639698659);
    points.append(p);

    p = VSAPoint(272.4138025206039, 116.50169653372318);
    p.SetSAAfter(30.956550523683344);
    p.SetSABefore(30.956550523683344);
    points.append(p);

    p = VSAPoint(262.16207443587984, 117.7817223666835);
    p.SetSAAfter(32.194294958045219);
    p.SetSABefore(32.194294958045219);
    points.append(p);

    p = VSAPoint(252.12060206432426, 119.88653752375922);
    p.SetSAAfter(33.423457501774081);
    p.SetSABefore(33.423457501774081);
    points.append(p);

    p = VSAPoint(242.63817035959835, 122.62712714468876);
    p.SetSAAfter(34.605995611034089);
    p.SetSABefore(34.605995611034089);
    points.append(p);

    p = VSAPoint(234.06356427536352, 125.81447636921058);
    p.SetSAAfter(35.70195258686585);
    p.SetSABefore(35.70195258686585);
    points.append(p);

    p = VSAPoint(226.74556876528095, 129.25957033706317);
    p.SetSAAfter(36.670979260736637);
    p.SetSABefore(36.670979260736637);
    points.append(p);

    p = VSAPoint(221.03296878301197, 132.77339418798488);
    p.SetSAAfter(37.474483357433364);
    p.SetSABefore(37.474483357433364);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    p.SetSAAfter(37.795275590551185);
    p.SetSABefore(37.795275590551185);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    p.SetSAAfter(37.795275590551185);
    p.SetSABefore(37.795275590551185);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    p.SetSAAfter(37.795275590551185);
    p.SetSABefore(37.795275590551185);
    points.append(p);

    p = VSAPoint(218.97637795275608, 134.48806299212646);
    p.SetSAAfter(37.795275590551185);
    p.SetSABefore(37.795275590551185);
    points.append(p);

    p = VSAPoint(214.11186185064025, 138.80795875858354);
    p.SetSAAfter(36.653502722568419);
    p.SetSABefore(36.653502722568419);
    points.append(p);

    p = VSAPoint(205.18536816471078, 146.02812964687715);
    p.SetSAAfter(34.638570080274732);
    p.SetSABefore(34.638570080274732);
    points.append(p);

    p = VSAPoint(197.17461091794695, 151.63888788459965);
    p.SetSAAfter(32.922124316374287);
    p.SetSABefore(32.922124316374287);
    points.append(p);

    p = VSAPoint(190.02702368501878, 155.74482471949955);
    p.SetSAAfter(31.475467886854414);
    p.SetSABefore(31.475467886854414);
    points.append(p);

    p = VSAPoint(183.69004004059613, 158.45053139932526);
    p.SetSAAfter(30.266183845834068);
    p.SetSABefore(30.266183845834068);
    points.append(p);

    p = VSAPoint(178.11109355934906, 159.86059917182507);
    p.SetSAAfter(29.256280085066027);
    p.SetSABefore(29.256280085066027);
    points.append(p);

    p = VSAPoint(173.23761781594754, 160.07961928474754);
    p.SetSAAfter(28.400113855274189);
    p.SetSABefore(28.400113855274189);
    points.append(p);

    p = VSAPoint(169.01704638506152, 159.21218298584108);
    p.SetSAAfter(27.643914239586497);
    p.SetSABefore(27.643914239586497);
    points.append(p);

    p = VSAPoint(165.396812841361, 157.36288152285397);
    p.SetSAAfter(26.930461655777069);
    p.SetSABefore(26.930461655777069);
    points.append(p);

    p = VSAPoint(162.32435075951594, 154.63630614353477);
    p.SetSAAfter(26.20953145720112);
    p.SetSABefore(26.20953145720112);
    points.append(p);

    p = VSAPoint(159.7470937141963, 151.13704809563185);
    p.SetSAAfter(25.44681460859611);
    p.SetSABefore(25.44681460859611);
    points.append(p);

    p = VSAPoint(157.61247528007215, 146.96969862689363);
    p.SetSAAfter(24.625073374270738);
    p.SetSABefore(24.625073374270738);
    points.append(p);

    p = VSAPoint(155.86792903181333, 142.23884898506853);
    p.SetSAAfter(23.740148635253075);
    p.SetSABefore(23.740148635253075);
    points.append(p);

    p = VSAPoint(154.46088854408993, 137.049090417905);
    p.SetSAAfter(22.796456338245449);
    p.SetSABefore(22.796456338245449);
    points.append(p);

    p = VSAPoint(152.84240084594785, 128.65747053734566);
    p.SetSAAfter(21.296571460906105);
    p.SetSABefore(21.296571460906105);
    points.append(p);

    p = VSAPoint(150.86615884353574, 110.74781740906135);
    p.SetSAAfter(18.134320368513631);
    p.SetSABefore(18.134320368513631);
    points.append(p);

    p = VSAPoint(149.37382105332603, 94.00159365355543);
    p.SetSAAfter(15.183683963183684);
    p.SetSABefore(15.183683963183684);
    points.append(p);

    p = VSAPoint(147.90386548781373, 84.77492816049366);
    p.SetSAAfter(13.543967770207646);
    p.SetSABefore(13.543967770207646);
    points.append(p);

    p = VSAPoint(146.22482573007983, 79.58685396281504);
    p.SetSAAfter(12.586956110891197);
    p.SetSABefore(12.586956110891197);
    points.append(p);

    p = VSAPoint(144.767673193039, 77.0483056159666);
    p.SetSAAfter(12.073257080584666);
    p.SetSABefore(12.073257080584666);
    points.append(p);

    p = VSAPoint(142.9646628872432, 75.4105345645091);
    p.SetSAAfter(11.645769545279848);
    p.SetSABefore(11.645769545279848);
    points.append(p);

    p = VSAPoint(140.76322838736246, 74.77813205619103);
    p.SetSAAfter(11.243788560650403);
    p.SetSABefore(11.243788560650403);
    points.append(p);

    p = VSAPoint(138.11080326806675, 75.25568933876076);
    p.SetSAAfter(10.77079881073082);
    p.SetSABefore(10.77079881073082);
    points.append(p);

    p = VSAPoint(134.95482110402602, 76.94779765996674);
    p.SetSAAfter(10.142330100270035);
    p.SetSABefore(10.142330100270035);
    points.append(p);

    p = VSAPoint(131.24271546991025, 79.95904826755734);
    p.SetSAAfter(9.3034514353721871);
    p.SetSABefore(9.3034514353721871);
    points.append(p);

    p = VSAPoint(126.92191994038947, 84.39403240928104);
    p.SetSAAfter(8.216780859744869);
    p.SetSABefore(8.216780859744869);
    points.append(p);

    p = VSAPoint(124.4881889763782, 87.24396850393732);
    p.SetSAAfter(7.559055118110237);
    p.SetSABefore(7.559055118110237);
    points.append(p);

    p = VSAPoint(124.4881889763782, 87.24396850393732);
    p.SetSAAfter(7.559055118110237);
    p.SetSABefore(7.559055118110237);
    points.append(p);

    p = VSAPoint(124.4881889763782, 87.24396850393732);
    //p.SetSAAfter(-1);
    p.SetSABefore(7.559055118110237);
    points.append(p);

    p = VSAPoint(124.4881889763782, 87.24396850393732);
    //p.SetSAAfter(-1);
    p.SetSABefore(7.559055118110237);
    points.append(p);

    p = VSAPoint(124.4881889763782, 87.24396850393732);
    //p.SetSAAfter(-1);
    p.SetSABefore(7.559055118110237);
    points.append(p);

    p = VSAPoint(124.4881889763782, 87.24396850393732);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(124.4881889763782, 87.24396850393732);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(121.4053345233613, 90.85541892105327);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(115.36650087404239, 97.1700051724747);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(109.48439793077911, 102.44240810316538);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(103.76400391356904, 106.72573235606063);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(98.2102970424097, 110.0730825740958);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(92.8282555372987, 112.53756340020624);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(87.62285761823352, 114.17227947732721);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(82.59908150521179, 115.03033544839411);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(77.76190541823104, 115.16483595634224);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(73.11630757728881, 114.62888564410696);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(68.66726620238268, 113.47558915462355);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(64.41975951351019, 111.7580511308274);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(60.3787657306689, 109.52937621565376);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(56.54926307385639, 106.84266905203805);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(52.93622976307019, 103.75103428291555);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(49.54464401830785, 100.30757655122164);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(44.854132804201555, 94.62627097468447);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(39.45938535079082, 86.2656734510919);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(35.040036121431584, 77.45366765004954);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(31.635910876104337, 68.61509071504);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(29.28683537478949, 60.17477978954592);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(28.03263537746753, 52.55757201704999);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(27.913136644118886, 46.188304541034846);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(28.62975536807624, 42.54811226326452);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(29.458368599363588, 40.697718569632514);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    p = VSAPoint(30.0, 39.999874015748034);
    //p.SetSAAfter(-1);
    //p.SetSABefore(-1);
    points.append(p);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsCase3() const
{
    QVector<QPointF> points;

    points += QPointF(11.491153250328935, 2.204598425196849);
    points += QPointF(445.748031496063, 2.204598425196849);
    points += QPointF(445.748031496063, 239.25772323190495);
    points += QPointF(437.3011718875349, 252.9620940028659);
    points += QPointF(431.5210396453687, 261.04286502132345);
    points += QPointF(425.0742362286929, 268.54423500057675);
    points += QPointF(418.11166285006243, 275.22268157509825);
    points += QPointF(410.66293904201643, 281.0200956238725);
    points += QPointF(402.78657384181963, 285.87157614021027);
    points += QPointF(394.5732042205309, 289.71737969963584);
    points += QPointF(386.1419584708077, 292.51680859777775);
    points += QPointF(377.62937764888807, 294.2596521460023);
    points += QPointF(369.17363576502424, 294.9709765895621);
    points += QPointF(360.89908762820056, 294.7076951308071);
    points += QPointF(352.90578450956633, 293.54885121734077);
    points += QPointF(345.2659401705694, 291.58363457828597);
    points += QPointF(338.02638381419206, 288.9008829382743);
    points += QPointF(331.214452254774, 285.58206377611566);
    points += QPointF(324.8447893417784, 281.69791760615396);
    points += QPointF(318.92540323684557, 277.307901893045);
    points += QPointF(313.4622927999198, 272.4613294067597);
    points += QPointF(308.4626029358523, 267.199302528306);
    points += QPointF(303.93656720661477, 261.5568795640949);
    points += QPointF(299.8985555961715, 255.5651950634023);
    points += QPointF(296.3674859139379, 249.25344082283073);
    points += QPointF(293.36675891047514, 242.65070430384247);
    points += QPointF(290.9237823363554, 235.7876787648548);
    points += QPointF(289.0690799250026, 228.6982245333719);
    points += QPointF(287.8349531366277, 221.42069049439027);
    points += QPointF(287.2536910795491, 213.9988201772971);
    points += QPointF(287.3554164994589, 206.48200110960104);
    points += QPointF(288.165803158891, 198.9246157503596);
    points += QPointF(289.7040578703973, 191.38436170566055);
    points += QPointF(291.98164916416846, 183.91963819443603);
    points += QPointF(294.9455451728963, 176.72393356713948);
    points += QPointF(295.79237435003455, 175.03780734840188);
    points += QPointF(296.245263052401, 173.2550222963373);
    points += QPointF(297.59693481965496, 166.31235205234043);
    points += QPointF(298.2857333094973, 160.53208953533525);
    points += QPointF(298.43819035278193, 155.84335424804448);
    points += QPointF(298.1940021729857, 152.25436961670644);
    points += QPointF(297.7243212024687, 149.73346552056378);
    points += QPointF(297.2141642634996, 148.17665975044923);
    points += QPointF(296.81807124385705, 147.38451061538373);
    points += QPointF(296.30769954702157, 146.83360641173047);
    points += QPointF(294.9800374980412, 146.2693748418327);
    points += QPointF(293.59863859999393, 145.93124637657365);
    points += QPointF(291.70740855306775, 145.66527555041208);
    points += QPointF(289.51401190033005, 145.54903057599236);
    points += QPointF(285.1770288005626, 145.6959639947203);
    points += QPointF(278.3617236899766, 146.6950293230864);
    points += QPointF(271.14652259400714, 148.48714038237037);
    points += QPointF(263.9194864200908, 150.92917578435245);
    points += QPointF(257.2170705229372, 153.76755541765795);
    points += QPointF(251.60582957396716, 156.6541701343772);
    points += QPointF(247.78435692308207, 159.04593361720316);
    points += QPointF(246.25434386618122, 160.2598011722792);
    points += QPointF(243.67170607597288, 162.99575598015517);
    points += QPointF(232.3489004893592, 169.9758475438662);
    points += QPointF(220.7012128062336, 176.43583903855628);
    points += QPointF(209.5894888372958, 181.6299305429989);
    points += QPointF(198.8701621522812, 185.51474135189684);
    points += QPointF(188.2122212804039, 188.0080506184889);
    points += QPointF(177.3148127107667, 188.81408257358973);
    points += QPointF(166.19194469464537, 187.37324105698534);
    points += QPointF(155.57420181410697, 183.17620217869953);
    points += QPointF(146.72912660379956, 176.5077748886884);
    points += QPointF(140.4165418057526, 168.5601760939351);
    points += QPointF(136.398647544906, 160.4725908270882);
    points += QPointF(134.00844184248987, 152.736031987575);
    points += QPointF(132.67609691771358, 145.39573210082384);
    points += QPointF(132.02477039749073, 138.2597894386109);
    points += QPointF(131.93387537548404, 132.84913141002448);
    points += QPointF(124.89904457045232, 138.11668139199287);
    points += QPointF(115.8895831399309, 143.54689645077573);
    points += QPointF(106.4111230253516, 147.88716086368487);
    points += QPointF(96.50699490577813, 150.99747783054613);
    points += QPointF(86.32454442321412, 152.7366302609282);
    points += QPointF(76.11298204383012, 153.0205687205323);
    points += QPointF(66.1751535607697, 151.8740677833447);
    points += QPointF(56.79043276012441, 149.44132674047353);
    points += QPointF(48.150859730789605, 145.94779600016403);
    points += QPointF(40.340747704632996, 141.64039002708367);
    points += QPointF(33.35598458760446, 136.7400113658245);
    points += QPointF(27.139387101639755, 131.42053399597268);
    points += QPointF(21.454249077112927, 125.64844566928473);
    points += QPointF(14.297732218982075, 116.98023078208763);
    points += QPointF(6.592092174102573, 105.03828991585031);
    points += QPointF(0.43109819620648937, 92.75351406859683);
    points += QPointF(-4.287700865002457, 80.50147704363812);
    points += QPointF(-7.673598677574605, 68.33582707450634);
    points += QPointF(-9.704701735666333, 56.0002075290815);
    points += QPointF(-9.95133102840429, 42.85489720779573);
    points += QPointF(-7.622895431362345, 31.027194419301413);
    points += QPointF(-3.18150493921823, 21.10903158131186);
    points += QPointF(11.491153250328935, 2.204598425196849);

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

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
#else
    path.removeLast();
#endif
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

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
#else
    path.removeLast();
#endif
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

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
    res.remove(res.size() - 1);
#else
    path.removeLast();
    res.removeLast();
#endif
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

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
    res.remove(res.size() - 1);
#else
    path.removeLast();
    res.removeLast();
#endif
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

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
    res.remove(res.size() - 1);
#else
    path.removeLast();
    res.removeLast();
#endif
    QTest::newRow("One loop, the second loop, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(20, 20);
    path << QPointF(10, 20);
    path << QPointF(20, 15);
    path << QPointF(10, 10);
    path << QPointF(20, 10);
    QTest::newRow("Correct closed a path, point on line (four unique points)") << path << path;

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
#else
    path.removeLast();
#endif
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

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
    path.remove(path.size() - 1);
#else
    path.removeLast();
#endif
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

    VSAPoint point = VSAPoint(787.5835464566929, 1701.3138897637796);
    point.SetSAAfter(-1);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(863.1740976377953, 1701.3138897637796);
    point.SetSAAfter(-1);
    point.SetSAAfter(-1);
    points.append(point);

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

    point = VSAPoint(797.0323653543306, 2608.4005039370077);
    point.SetSAAfter(0);
    point.SetSABefore(-1);
    points.append(point);

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

#ifndef Q_OS_WIN // Disabled due to "undefined behavior" problem
#ifndef Q_CC_CLANG
    QTest::newRow("GAVAUDAN Laure.") << points << 37.795275590551185 << ekvOrig;
#endif
#endif

    points.clear();
    point = VSAPoint(97.33089106412862, -223.03306117556497);
    point.SetSAAfter(-1);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(990.7494050554426, 2.819093995045);
    point.SetSAAfter(-1);
    point.SetSABefore(-1);
    points.append(point);

    point = VSAPoint(908.3966357321774, 379.5839357215547);
    point.SetSAAfter(-1);
    point.SetSABefore(-1);
    points.append(point);

    point = VSAPoint(-135.41154226686143, 697.6417881399819);
    point.SetSAAfter(0);
    point.SetSABefore(-1);
    points.append(point);

    ekvOrig.clear();
    ekvOrig.append(QPointF(100.10981413873267, -234.02583351343978));
    ekvOrig.append(QPointF(1004.1704360325447, -5.483401649771952));
    ekvOrig.append(QPointF(918.0553412376563, 388.4941212347381));
    ekvOrig.append(QPointF(-138.65807550610091, 710.4843173601864));
    ekvOrig.append(QPointF(100.10981413873267, -234.02583351343978));

    // See the file "collection/bugs/Issue_#604.val" (since 0.5.0)
    QTest::newRow("Issue #604.") << points << 11.338582677165354 << ekvOrig;

    points.clear();

    point = VSAPoint(11565.008125001967, -71.44488549419934);
    point.SetSAAfter(0);
    points.append(point);

    point = VSAPoint(11774.053467225081, -3376.8303371353477);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(11774.053467225081, -3376.8303371353477);
    point.SetSABefore(0);
    points.append(point);

    points.append(VSAPoint(11821.637653562488, -3457.444047544761));
    points.append(VSAPoint(11916.506852253828, -3619.698047174161));
    points.append(VSAPoint(12011.687139013728, -3784.3170132645946));
    points.append(VSAPoint(12107.923065894336, -3952.559914581168));
    points.append(VSAPoint(12205.959184947797, -4125.685719888987));
    points.append(VSAPoint(12306.540048226263, -4304.953397953153));
    points.append(VSAPoint(12410.410207781879, -4491.621917538776));
    points.append(VSAPoint(12518.314215666796, -4686.950247410959));
    points.append(VSAPoint(12630.996623933155, -4892.197356334806));
    points.append(VSAPoint(12749.201984633113, -5108.622213075425));
    points.append(VSAPoint(12937.571227539614, -5455.181123300274));
    points.append(VSAPoint(13216.05428816687, -5970.204948863398));
    points.append(VSAPoint(13530.031095467164, -6552.6012034655));

    point = VSAPoint(13704.042216387523, -6875.648082494774);
    point.SetSAAfter(0);
    points.append(point);

    point = VSAPoint(13704.042216387523, -6875.648082494774);
    point.SetSAAfter(0);
    points.append(point);

    point = VSAPoint(13493.259591532773, -71.44488549420451);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(13493.259591532773, -71.44488549420451);
    point.SetSABefore(0);
    points.append(point);

    points.append(VSAPoint(13458.265794168308, -72.45287893700785));
    points.append(VSAPoint(13303.675181471457, -76.51882381889763));
    points.append(VSAPoint(13154.908187746063, -79.90711122047243));
    points.append(VSAPoint(13010.905973179135, -82.6177411417323));
    points.append(VSAPoint(12870.60969795768, -84.65071358267716));
    points.append(VSAPoint(12732.9605222687, -86.0060285433071));
    points.append(VSAPoint(12596.899606299212, -86.68368602362204));
    points.append(VSAPoint(12461.36811023622, -86.68368602362204));
    points.append(VSAPoint(12325.307194266732, -86.0060285433071));
    points.append(VSAPoint(12187.658018577757, -84.65071358267716));
    points.append(VSAPoint(12047.361743356301, -82.6177411417323));
    points.append(VSAPoint(11903.35952878937, -79.90711122047243));
    points.append(VSAPoint(11754.592535063975, -76.51882381889763));
    points.append(VSAPoint(11600.001922367124, -72.45287893700785));

    point = VSAPoint(11565.008125001967, -71.44488549419934);
    point.SetSAAfter(0);
    points.append(point);

    ekvOrig.clear();
    ekvOrig.append(QPointF(11561.414612602906, -14.624946442565701));
    ekvOrig.append(QPointF(11781.95342513335, -3501.7429788659483));
    ekvOrig.append(QPointF(11772.755890703675, -3486.161176715607));
    ekvOrig.append(QPointF(11867.496139886347, -3648.194635075343));
    ekvOrig.append(QPointF(11962.541506347354, -3812.5802499029824));
    ekvOrig.append(QPointF(12058.651107851038, -3980.6023049242845));
    ekvOrig.append(QPointF(12156.571541308, -4153.523816526403));
    ekvOrig.append(QPointF(12257.048738448357, -4332.606727953389));
    ekvOrig.append(QPointF(12360.827974888123, -4519.11184658243));
    ekvOrig.append(QPointF(12468.653939048814, -4714.298901347144));
    ekvOrig.append(QPointF(12581.270838581826, -4919.426688379175));
    ekvOrig.append(QPointF(12699.418991549643, -5135.746802194771));
    ekvOrig.append(QPointF(12887.73123714858, -5482.200849627318));
    ekvOrig.append(QPointF(13166.168107919768, -5997.139252126215));
    ekvOrig.append(QPointF(13480.123425536643, -6579.495645468361));
    ekvOrig.append(QPointF(13707.971545534987, -7002.489438444617));
    ekvOrig.append(QPointF(13491.50417959835, -14.779021766203327));
    ekvOrig.append(QPointF(13456.704320007799, -15.781428847446987));
    ekvOrig.append(QPointF(13302.284438389732, -19.842883268086513));
    ekvOrig.append(QPointF(13153.729251428536, -23.226346601010544));
    ekvOrig.append(QPointF(13009.961769808971, -25.932558019731804));
    ekvOrig.append(QPointF(12869.919891108724, -27.961844111086332));
    ekvOrig.append(QPointF(12732.540250691125, -29.3145051859673));
    ekvOrig.append(QPointF(12596.758426378788, -29.990772637795267));
    ekvOrig.append(QPointF(12461.509290156644, -29.990772637795267));
    ekvOrig.append(QPointF(12325.727465844308, -29.3145051859673));
    ekvOrig.append(QPointF(12188.347825426712, -27.961844111086332));
    ekvOrig.append(QPointF(12048.305946726465, -25.9325580197318));
    ekvOrig.append(QPointF(11904.538465106896, -23.226346601010544));
    ekvOrig.append(QPointF(11755.9832781457, -19.842883268086513));
    ekvOrig.append(QPointF(11601.563396526772, -15.78142884742432));
    ekvOrig.append(QPointF(11561.414612602906, -14.624946442565701));

    // See the file "collection/bugs/Issue_#627.val"
    QTest::newRow("Issue #627.") << points << 56.692913385826778 << ekvOrig;

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
    path << QPointF(-670.6449010946802, 4046.36220472441);
    path << QPointF(-1025.9051277126944, 4046.36220472441);
    path << QPointF(-1026.4460203880594, 4010.5247429150854);
    path << QPointF(-1027.2972172274538, 3924.202328582098);
    path << QPointF(-1028.1383921346433, 3768.5948526129496);
    path << QPointF(-1028.5065585022217, 3521.575730066707);
    path << QPointF(-1028.2712136539103, 3252.2436039362233);
    path << QPointF(-1027.2910122410117, 2850.1024469719814);
    path << QPointF(-1025.9446023682538, 2439.350819630564);
    path << QPointF(-1025.8983315247287, 2338.629525677473);
    path << QPointF(-1025.3536572186458, 2309.970015878699);
    path << QPointF(-1024.2100836932389, 2281.714612342931);
    path << QPointF(-1022.5102766116828, 2253.846781520112);
    path << QPointF(-1020.2969016371525, 2226.349989860186);
    path << QPointF(-1017.6126244328227, 2199.207703813094);
    path << QPointF(-1014.5001106618688, 2172.403389828782);
    path << QPointF(-1011.0020259874652, 2145.9205143571917);
    path << QPointF(-1005.1601480132764, 2106.7277181407126);
    path << QPointF(-996.3625412018714, 2055.4921956731814);
    path << QPointF(-986.7906327138169, 2005.2448233555149);
    path << QPointF(-976.785747854512, 1955.8533327872588);
    path << QPointF(-961.6606968634906, 1883.0158867454916);
    path << QPointF(-947.5864881030896, 1811.4914675744105);
    path << QPointF(-939.2629508127773, 1764.2008199992524);
    path << QPointF(-933.8852659113251, 1728.8707137815559);
    path << QPointF(-930.742733377741, 1705.3464944792456);
    path << QPointF(-928.0252775410311, 1681.829576238578);
    path << QPointF(-925.7755640643697, 1658.3034255094963);
    path << QPointF(-924.036258610932, 1634.7515087419433);
    path << QPointF(-922.850026843893, 1611.1572923858625);
    path << QPointF(-922.2595344264276, 1587.504242891197);
    path << QPointF(-922.3074470217107, 1563.7758267078902);
    path << QPointF(-922.613405031688, 1551.8740157480315);
    path << QPointF(-960.4086806222392, 841.3228346456693);
    path << QPointF(-954.9336313684444, 841.5464781141166);
    path << QPointF(-944.0363771538431, 841.3102753632543);
    path << QPointF(-933.2160856340209, 840.291423017261);
    path << QPointF(-922.4878118569704, 838.5316299985567);
    path << QPointF(-911.8666108706839, 836.0726052295611);
    path << QPointF(-901.3675377231535, 832.9560576326933);
    path << QPointF(-891.005647462372, 829.2236961303737);
    path << QPointF(-880.7959951363317, 824.9172296450213);
    path << QPointF(-870.7536357930251, 820.0783670990559);
    path << QPointF(-860.893624480444, 814.7488174148973);
    path << QPointF(-851.2310162465817, 808.9702895149649);
    path << QPointF(-841.7808661394299, 802.7844923216785);
    path << QPointF(-832.5582292069812, 796.2331347574575);
    path << QPointF(-823.578160497228, 789.3579257447218);
    path << QPointF(-810.5607800373014, 778.5565764202543);
    path << QPointF(-794.2367125298769, 763.3635567727296);
    path << QPointF(-779.1539087770976, 747.6258919346988);
    path << QPointF(-765.4328091629026, 731.6772532855191);
    path << QPointF(-753.193854071231, 715.8513122045474);
    path << QPointF(-742.557483886022, 700.4817400711408);
    path << QPointF(-733.644138991215, 685.9022082646563);
    path << QPointF(-726.5742597707488, 672.446388164451);
    path << QPointF(-721.4682866085625, 660.447951149882);
    path << QPointF(-718.6229063234249, 651.1532303788147);
    path << QPointF(-716.6036430255488, 642.9038041285014);
    path << QPointF(-714.137568179324, 630.1235656609365);
    path << QPointF(-711.8605525364693, 612.2344502588126);
    path << QPointF(-710.4560555432737, 593.4222205889721);
    path << QPointF(-709.4234847119759, 563.5940176156308);
    path << QPointF(-708.952111561728, 520.4666582691573);
    path << QPointF(-708.4401766852314, 497.3858267716535);
    path << QPointF(-400.92922424489655, 469.03937007874015);
    path << QPointF(-708.4401766852314, 440.6929133858268);
    path << QPointF(-708.7078446526739, 341.66122584661264);
    path << QPointF(-709.3427685457568, 299.60322373665383);
    path << QPointF(-710.6909230403871, 257.048095841136);
    path << QPointF(-713.0251717477311, 214.57984397612822);
    path << QPointF(-715.632864794307, 183.1716335401434);
    path << QPointF(-717.7953694429818, 162.55016633308693);
    path << QPointF(-720.3578834261159, 142.27891915519677);
    path << QPointF(-723.3545146951046, 122.43089223348173);
    path << QPointF(-725.0465030138121, 112.71059563115871);
    path << QPointF(-219.59055118110237, -35.52755905511811);
    path << QPointF(-218.99352387527398, -33.21125072212394);
    path << QPointF(-217.35724543521775, -28.699086141666157);
    path << QPointF(-215.20035586903225, -24.33136255454731);
    path << QPointF(-212.53403014110648, -20.10796717265881);
    path << QPointF(-209.36944321582945, -16.02878720789205);
    path << QPointF(-205.71777005759026, -12.093709872138447);
    path << QPointF(-201.59018563077785, -8.302622377289406);
    path << QPointF(-196.99786489978123, -4.65541193523633);
    path << QPointF(-189.3170483291933, 0.5638303631539586);
    path << QPointF(-177.47808861476295, 6.996342387787443);
    path << QPointF(-163.981333042598, 12.855376387191757);
    path << QPointF(-148.91618132781048, 18.141834666235646);
    path << QPointF(-132.37203318551252, 22.856619529787864);
    path << QPointF(-114.43828833081622, 27.00063328271716);
    path << QPointF(-95.20434647883366, 30.574778229892296);
    path << QPointF(-74.75960734467688, 33.57995667618201);
    path << QPointF(-53.193470643458, 36.01707092645505);
    path << QPointF(-30.595336090289106, 37.887023285580185);
    path << QPointF(-7.0546034002822875, 39.19071605842615);
    path << QPointF(17.339327711450373, 39.929051549861704);
    path << QPointF(29.858267716535437, 40.06299212598426);
    path << QPointF(-45.73228346456693, 1589.6692913385828);
    path << QPointF(-45.73228346456693, 4046.36220472441);
    path << QPointF(-297.70078740157487, 4046.36220472441);
    path << QPointF(-297.70078740157487, 2118.8031496062995);
    path << QPointF(-222.1102362204725, 1589.6692913385828);
    path << QPointF(-297.70078740157487, 1060.535433070866);
    path << QPointF(-373.2913385826772, 1589.6692913385828);
    path << QPointF(-297.70078740157487, 2118.8031496062995);
    path << QPointF(-297.70078740157487, 4046.36220472441);
    path << QPointF(-670.6449010946802, 4046.36220472441);
    path << QPointF(-670.6449010946802, 2024.3149606299214);
    path << QPointF(-622.7555214134819, 1570.7716535433071);
    path << QPointF(-670.6449010946802, 1117.2283464566929);
    path << QPointF(-718.5342807758785, 1570.7716535433071);
    path << QPointF(-670.6449010946802, 2024.3149606299214);

    QVector<QPointF> expect;
    expect << QPointF(-670.6449010946802, 4046.36220472441);
    expect << QPointF(-670.6449010946802, 4046.36220472441);
    expect << QPointF(-670.6449010946802, 2024.3149606299214);
    expect << QPointF(-670.6449010946802, 2024.3149606299214);
    expect << QPointF(-670.6449010946802, 2024.3149606299214);

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

    points += VSAPoint(35, 39.9999);
    points += VSAPoint(412.953, 39.9999);
    points += VSAPoint(417.135, 417.929);
    points += VSAPoint(417.135, 417.929);
    points += VSAPoint(408.797, 405.589);
    points += VSAPoint(390.909, 377.669);
    points += VSAPoint(362.315, 330.86);
    points += VSAPoint(323.075, 264.247);
    points += VSAPoint(286.15, 201.448);
    points += VSAPoint(262.477, 162.745);
    points += VSAPoint(249.22, 142.455);
    points += VSAPoint(241.092, 131.261);
    points += VSAPoint(236.545, 125.75);
    points += VSAPoint(232.808, 122.058);
    points += VSAPoint(230.6, 120.629);
    points += VSAPoint(229.393, 120.277);
    points += VSAPoint(228.421, 120.456);
    points += VSAPoint(227.69, 121.185);
    points += VSAPoint(227.033, 123.272);
    points += VSAPoint(227.112, 128.232);
    points += VSAPoint(228.29, 135.699);
    points += VSAPoint(230.625, 145.81);
    points += VSAPoint(234.173, 158.703);
    points += VSAPoint(241.73, 183.168);
    points += VSAPoint(248.796, 204.144);
    points += VSAPoint(248.796, 204.144);
    points += VSAPoint(251.528, 212.406);
    points += VSAPoint(255.482, 227.075);
    points += VSAPoint(257.717, 239.591);
    points += VSAPoint(258.279, 247.554);
    points += VSAPoint(258.203, 252.278);
    points += VSAPoint(257.756, 256.51);
    points += VSAPoint(256.949, 260.264);
    points += VSAPoint(255.795, 263.547);
    points += VSAPoint(254.308, 266.372);
    points += VSAPoint(252.501, 268.749);
    points += VSAPoint(250.385, 270.688);
    points += VSAPoint(247.974, 272.201);
    points += VSAPoint(245.281, 273.296);
    points += VSAPoint(242.319, 273.986);
    points += VSAPoint(239.1, 274.28);
    points += VSAPoint(233.846, 274.05);
    points += VSAPoint(226.022, 272.393);
    points += VSAPoint(217.402, 269.345);
    points += VSAPoint(208.09, 264.991);
    points += VSAPoint(198.186, 259.414);
    points += VSAPoint(187.795, 252.7);
    points += VSAPoint(177.019, 244.933);
    points += VSAPoint(165.96, 236.197);
    points += VSAPoint(154.721, 226.576);
    points += VSAPoint(143.405, 216.157);
    points += VSAPoint(132.113, 205.022);
    points += VSAPoint(120.95, 193.257);
    points += VSAPoint(110.017, 180.946);
    points += VSAPoint(99.4167, 168.174);
    points += VSAPoint(89.2522, 155.024);
    points += VSAPoint(79.626, 141.582);
    points += VSAPoint(70.6405, 127.933);
    points += VSAPoint(62.3985, 114.16);
    points += VSAPoint(55.0025, 100.348);
    points += VSAPoint(48.5551, 86.5823);
    points += VSAPoint(43.159, 72.9466);
    points += VSAPoint(38.9167, 59.5258);
    points += VSAPoint(35.9309, 46.4042);
    points += VSAPoint(35, 39.9999);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue298Case1() const
{
    QVector<QPointF> points;
    points += QPointF(-52.3724798442221, -35.5907);
    points += QPointF(487.7117748779425, -35.5907);
    points += QPointF(493.3432017362585, 473.32371517914754);
    points += QPointF(385.98559977345093, 506.8445742667132);
    points += QPointF(345.64704646524604, 447.1446764706891);
    points += QPointF(326.82411403464874, 417.76541252489994);
    points += QPointF(297.4844355409708, 369.73572061014266);
    points += QPointF(280.35686644039447, 340.63425704493835);
    points += QPointF(268.2336759982877, 345.56366422433183);
    points += QPointF(254.38869069377708, 348.78886336684104);
    points += QPointF(240.8928242225697, 350.0214774527481);
    points += QPointF(224.29748398011193, 349.2949970081793);
    points += QPointF(205.50330859478322, 345.31468660256957);
    points += QPointF(188.72568121178054, 339.38217984347546);
    points += QPointF(173.487571907339, 332.2573164509149);
    points += QPointF(159.09346043909582, 324.15190856941325);
    points += QPointF(145.1562378134811, 315.1465661857729);
    points += QPointF(131.46917217609203, 305.28136213922494);
    points += QPointF(117.9345600633141, 294.589765121662);
    points += QPointF(104.5254725457231, 283.11108988305153);
    points += QPointF(91.25156649455745, 270.88938370179534);
    points += QPointF(78.14294517511125, 257.9630200468154);
    points += QPointF(65.25722328495372, 244.3823949426573);
    points += QPointF(52.65759889494496, 230.19470850111355);
    points += QPointF(40.412239584772514, 215.4406233233806);
    points += QPointF(28.600027181043494, 200.15894757848054);
    points += QPointF(17.304913602921047, 184.38648111018338);
    points += QPointF(6.6105681133211736, 168.14173996194046);
    points += QPointF(-3.3897319816688407, 151.43048866270516);
    points += QPointF(-12.592267484961765, 134.24479093805914);
    points += QPointF(-20.880547263016442, 116.54866956498358);
    points += QPointF(-28.111192294561146, 98.27715746242171);
    points += QPointF(-34.098213657706594, 79.33681465062016);
    points += QPointF(-38.441724866417594, 60.24852451858777);
    points += QPointF(-52.3724798442221, -35.5907);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> TST_VAbstractPiece::InputPointsIssue298Case2() const
{
    QVector<VSAPoint> points;

    points += VSAPoint(35, 39.9999);
    points += VSAPoint(35, 39.9999);
    points += VSAPoint(35.9309, 46.4042);
    points += VSAPoint(38.9167, 59.5258);
    points += VSAPoint(43.159, 72.9466);
    points += VSAPoint(48.5551, 86.5823);
    points += VSAPoint(55.0025, 100.348);
    points += VSAPoint(62.3985, 114.16);
    points += VSAPoint(70.6405, 127.933);
    points += VSAPoint(79.626, 141.582);
    points += VSAPoint(89.2522, 155.024);
    points += VSAPoint(99.4167, 168.174);
    points += VSAPoint(110.017, 180.946);
    points += VSAPoint(120.95, 193.257);
    points += VSAPoint(132.113, 205.022);
    points += VSAPoint(143.405, 216.157);
    points += VSAPoint(154.721, 226.576);
    points += VSAPoint(165.96, 236.197);
    points += VSAPoint(177.019, 244.933);
    points += VSAPoint(187.795, 252.7);
    points += VSAPoint(198.186, 259.414);
    points += VSAPoint(208.09, 264.991);
    points += VSAPoint(217.402, 269.345);
    points += VSAPoint(226.022, 272.393);
    points += VSAPoint(233.846, 274.05);
    points += VSAPoint(239.1, 274.28);
    points += VSAPoint(242.319, 273.986);
    points += VSAPoint(245.281, 273.296);
    points += VSAPoint(247.974, 272.201);
    points += VSAPoint(250.385, 270.688);
    points += VSAPoint(252.501, 268.749);
    points += VSAPoint(254.308, 266.372);
    points += VSAPoint(255.795, 263.547);
    points += VSAPoint(256.949, 260.264);
    points += VSAPoint(257.756, 256.51);
    points += VSAPoint(258.203, 252.278);
    points += VSAPoint(258.279, 247.554);
    points += VSAPoint(257.717, 239.591);
    points += VSAPoint(255.482, 227.075);
    points += VSAPoint(251.528, 212.406);
    points += VSAPoint(248.796, 204.144);
    points += VSAPoint(248.796, 204.144);
    points += VSAPoint(241.73, 183.168);
    points += VSAPoint(234.173, 158.703);
    points += VSAPoint(230.625, 145.81);
    points += VSAPoint(228.29, 135.699);
    points += VSAPoint(227.112, 128.232);
    points += VSAPoint(227.033, 123.272);
    points += VSAPoint(227.69, 121.185);
    points += VSAPoint(228.421, 120.456);
    points += VSAPoint(229.393, 120.277);
    points += VSAPoint(230.6, 120.629);
    points += VSAPoint(232.808, 122.058);
    points += VSAPoint(236.545, 125.75);
    points += VSAPoint(241.092, 131.261);
    points += VSAPoint(249.22, 142.455);
    points += VSAPoint(262.477, 162.745);
    points += VSAPoint(286.15, 201.448);
    points += VSAPoint(323.075, 264.247);
    points += VSAPoint(362.315, 330.86);
    points += VSAPoint(390.909, 377.669);
    points += VSAPoint(408.797, 405.589);
    points += VSAPoint(417.135, 417.929);
    points += VSAPoint(417.135, 417.929);
    points += VSAPoint(35, 417.953);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue298Case2() const
{
    QVector<QPointF> points;

    points += QPointF(-2.7952999999999975, 4.838469950598078);
    points += QPointF(67.34448942068963, -0.2324858268916558);
    points += QPointF(73.11721243320879, 39.48203774070609);
    points += QPointF(75.42415682885321, 49.62029267468959);
    points += QPointF(78.79409614728041, 60.281321268788744);
    points += QPointF(83.27292363150828, 71.59911521750833);
    points += QPointF(88.79988374248082, 83.39960453097031);
    points += QPointF(95.2926159908344, 95.5247556686474);
    points += QPointF(102.65546594334339, 107.82863001903641);
    points += QPointF(110.78654319853989, 120.17975944490887);
    points += QPointF(119.5782864094781, 132.4565262107595);
    points += QPointF(128.91893020761376, 144.54068833830968);
    points += QPointF(138.69670055252752, 156.3216457494432);
    points += QPointF(148.79638835752286, 167.69430252867102);
    points += QPointF(159.09802741244354, 178.55148997659143);
    points += QPointF(169.48171675272164, 188.79080814910267);
    points += QPointF(179.81876372713828, 198.30845505847407);
    points += QPointF(189.9727199683426, 207.00061743916868);
    points += QPointF(199.7939139119543, 214.75881893038778);
    points += QPointF(209.1143810932559, 221.476716907111);
    points += QPointF(216.03386663545683, 225.9476461661168);
    points += QPointF(215.3109698947021, 223.26576141899216);
    points += QPointF(212.94471478165408, 216.10983179087987);
    points += QPointF(205.75950336032207, 194.7799429182702);
    points += QPointF(197.88802785264718, 169.29686123304236);
    points += QPointF(193.97579117825833, 155.08026950731082);
    points += QPointF(191.1640933645057, 142.90507610480435);
    points += QPointF(189.3638602852325, 131.49392126360493);
    points += QPointF(189.14507682295456, 117.75764312564759);
    points += QPointF(194.42693552963567, 100.97950138920423);
    points += QPointF(210.03879336533757, 85.41035725481989);
    points += QPointF(231.36634627769158, 81.48275234606332);
    points += QPointF(246.4916615881645, 85.89378050620131);
    points += QPointF(256.60614755001956, 92.43979519799973);
    points += QPointF(264.4750900046005, 100.21398185636762);
    points += QPointF(270.9888544453203, 108.1087159300009);
    points += QPointF(280.35077918473866, 121.00209505562212);
    points += QPointF(294.42535276480356, 142.5434013797918);
    points += QPointF(318.5638508136392, 182.00744438169633);
    points += QPointF(355.64786176301806, 245.0768771131813);
    points += QPointF(394.7264228866019, 311.415821866629);
    points += QPointF(422.9514429826756, 357.62079373755);
    points += QPointF(440.37197676737753, 384.8111617646563);
    points += QPointF(488.2841719585649, 455.71983154868764);
    points += QPointF(-2.795300000000013, 455.7506738094777);
    points += QPointF(-2.7952999999999975, 4.838469950598078);

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

    points += VSAPoint(1352.8346456692914, 1173.8581417322835);

    VSAPoint point = VSAPoint(1352.8346456692914, 1362.8345196850394);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1352.8346456692914, 1362.8345196850394);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1351.7927746622177, 1365.3174895470343);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1349.2904293989368, 1370.0604466887874);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1346.2962576995217, 1374.5430452326066);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1342.8219234115563, 1378.7733079571037);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1338.8790903826243, 1382.7592576408904);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1334.4794224603097, 1386.5089170625781);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1329.6345834921963, 1390.0303090007792);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1324.356237325868, 1393.3314562341047);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1315.7019501937195, 1397.9127915604581);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1302.6841808427803, 1403.289891141198);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1288.142331955491, 1407.930421917447);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1272.169714312523, 1411.8985661180982);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1254.8596386945478, 1415.2585059720464);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1236.3054158822356, 1418.074423708184);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1216.6003566562576, 1420.4105015554055);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1195.8377717972853, 1422.3309217426045);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1163.0181823648445, 1424.6044927975377);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1116.2675882169847, 1426.7215739585208);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(1041.771617936041, 1429.0735145188771);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(963.6986998754783, 1432.082545646023);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(924.3395298428601, 1434.378255703845);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(898.1648782802928, 1436.3169978244687);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(872.1457412334543, 1438.6744512606858);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(846.3754294830155, 1441.5147982413903);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(820.9472538096477, 1444.9022209954755);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(795.9545249940218, 1448.9009017518354);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(771.4905538168089, 1453.5750227393632);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(747.6486510586797, 1458.9887661869527);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(724.5221275003055, 1465.2063143234973);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(702.2042939223572, 1472.2918493778907);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(691.4173228346457, 1476.2203464566928);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(691.4173228346457, 1476.2203464566928);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(691.4173228346457, 1476.2203464566928);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(691.4173228346457, 1476.2203464566928);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(691.4173228346457, 1476.2203464566928);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(691.4173228346457, 1476.2203464566928);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(691.4173228346457, 1476.2203464566928);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(682.8822388593928, 1479.3575289855721);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(665.6479146239083, 1485.1110371155814);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(648.2139215316024, 1490.2399559097548);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(630.5772803437565, 1494.74477312083);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(612.7350118216522, 1498.6259765015448);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(594.6841367265706, 1501.884053804636);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(576.4216758197934, 1504.5194927828425);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(557.9446498626021, 1506.5327811889006);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(539.2500796162778, 1507.9244067755485);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(520.334985842102, 1508.6948572955234);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(501.1963893013563, 1508.844620501563);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(481.8313107553219, 1508.3741841464052);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(462.23677096528036, 1507.2840359827874);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(442.40979069251296, 1505.5746637634468);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(422.3473906983012, 1503.2465552411209);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(402.0465917439264, 1500.300198168548);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(381.50441459067014, 1496.7360802984645);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(360.7178799998137, 1492.554689383609);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(339.6840087326385, 1487.7565131767187);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(318.399821550426, 1482.3420394305313);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(296.8623392144576, 1476.3117558977842);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(275.06858248601475, 1469.6661503312146);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(253.01557212637874, 1462.4057104835604);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(230.70032889683108, 1454.530924107559);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(208.11987355865324, 1446.0422789559484);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(185.27122687312647, 1436.9402627814657);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(162.15140960153235, 1427.2253633368487);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(138.7574425051522, 1416.8980683748348);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(115.08634634526746, 1405.9588656481617);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(91.13514188315949, 1394.4082429095668);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(66.90084988010975, 1382.2466879117876);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(42.38049109739966, 1369.4746884075616);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(30.0, 1362.8345196850394);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    point = VSAPoint(30.0, 1362.8345196850394);
    point.SetSAAfter(0);
    point.SetSABefore(0);
    points.append(point);

    points += VSAPoint(30.0, 1173.8581417322835);
    points += VSAPoint(30.0, 39.999874015748034);
    points += VSAPoint(30.0, 39.999874015748034);
    points += VSAPoint(31.836416744798022, 50.53779899603764);
    points += VSAPoint(35.952598805592906, 71.21252386912929);
    points += VSAPoint(40.586864315463195, 91.4101105071633);
    points += VSAPoint(45.72373012289418, 111.13523876561466);
    points += VSAPoint(51.347713076371164, 130.39258849995826);
    points += VSAPoint(57.44333002437946, 149.186839565669);
    points += VSAPoint(63.995097815404385, 167.52267181822185);
    points += VSAPoint(70.98753329793121, 185.40476511309168);
    points += VSAPoint(78.4051533204453, 202.83779930575355);
    points += VSAPoint(86.23247473143188, 219.82645425168224);
    points += VSAPoint(94.4540143793763, 236.37540980635274);
    points += VSAPoint(103.05428911276391, 252.48934582523998);
    points += VSAPoint(112.01781578007993, 268.17294216381885);
    points += VSAPoint(121.32911122980973, 283.43087867756435);
    points += VSAPoint(130.97269231043856, 298.2678352219515);
    points += VSAPoint(140.93307587045177, 312.688491652455);
    points += VSAPoint(151.19477875833468, 326.69752782454987);
    points += VSAPoint(161.74231782257255, 340.2996235937111);
    points += VSAPoint(172.5602099116507, 353.4994588154136);
    points += VSAPoint(183.63297187405442, 366.30171334513216);
    points += VSAPoint(194.94512055826902, 378.7110670383419);
    points += VSAPoint(206.4811728127799, 390.7321997505177);
    points += VSAPoint(218.22564548607224, 402.3697913371344);
    points += VSAPoint(230.1630554266314, 413.62852165366695);
    points += VSAPoint(242.27791948294265, 424.51307055559033);
    points += VSAPoint(254.55475450349132, 435.02811789837943);
    points += VSAPoint(273.2244253126398, 450.16283591309366);
    points += VSAPoint(298.5641233209854, 469.0320795095655);
    points += VSAPoint(324.2730084590162, 486.5075542680491);
    points += VSAPoint(350.2272155146146, 502.6266990323438);
    points += VSAPoint(376.30287927566314, 517.426952646249);
    points += VSAPoint(402.37613453004406, 530.9457539535642);
    points += VSAPoint(428.3231160656401, 543.220541798089);
    points += VSAPoint(454.0199586703335, 554.2887550236223);
    points += VSAPoint(479.34279713200675, 564.1878324739641);
    points += VSAPoint(504.1677662385423, 572.9552129929135);
    points += VSAPoint(528.3710007778225, 580.62833542427);
    points += VSAPoint(551.8286355377302, 587.2446386118332);
    points += VSAPoint(574.4168053061474, 592.8415613994023);
    points += VSAPoint(596.0116448709566, 597.4565426307768);
    points += VSAPoint(616.4892890200404, 601.127021149756);
    points += VSAPoint(635.725872541281, 603.8904358001398);
    points += VSAPoint(653.5975302225612, 605.7842254257271);
    points += VSAPoint(669.9803968517632, 606.8458288703175);
    points += VSAPoint(684.7506072167696, 607.1126849777106);
    points += VSAPoint(691.417322834647, 606.9290078739997);
    points += VSAPoint(691.417322834647, 606.9290078739997);
    points += VSAPoint(691.417322834647, 606.9290078739997);
    points += VSAPoint(691.417322834647, 606.9290078739997);
    points += VSAPoint(691.417322834647, 606.9290078739997);
    points += VSAPoint(691.417322834647, 606.9290078739997);
    points += VSAPoint(691.417322834647, 606.9290078739997);
    points += VSAPoint(698.4117938885747, 606.70357708988);
    points += VSAPoint(712.3872092557954, 606.6270580415836);
    points += VSAPoint(726.3421370199347, 606.9929810221732);
    points += VSAPoint(740.2718488317498, 607.7938249260419);
    points += VSAPoint(754.1716163419968, 609.0220686475833);
    points += VSAPoint(768.0367112014326, 610.6701910811905);
    points += VSAPoint(781.8624050608131, 612.7306711212568);
    points += VSAPoint(795.6439695708956, 615.1959876621759);
    points += VSAPoint(809.3766763824362, 618.0586195983409);
    points += VSAPoint(823.0557971461919, 621.3110458241451);
    points += VSAPoint(843.4718370533695, 626.8577230967079);
    points += VSAPoint(870.4518221032482, 635.6110880188724);
    points += VSAPoint(897.1323526756041, 645.7882501986742);
    points += VSAPoint(923.4756019764898, 657.3290407912602);
    points += VSAPoint(949.4437432119576, 670.1732909517772);
    points += VSAPoint(974.99894958806, 684.2608318353717);
    points += VSAPoint(1000.1033943108496, 699.5314945971908);
    points += VSAPoint(1024.7192505863786, 715.9251103923807);
    points += VSAPoint(1048.8086916206998, 733.381510376089);
    points += VSAPoint(1072.333890619865, 751.8405257034619);
    points += VSAPoint(1095.257020789927, 771.2419875296464);
    points += VSAPoint(1117.5402553369386, 791.5257270097889);
    points += VSAPoint(1139.1457674669518, 812.6315752990365);
    points += VSAPoint(1160.0357303860196, 834.499363552536);
    points += VSAPoint(1180.1723173001938, 857.0689229254342);
    points += VSAPoint(1199.517701415527, 880.2800845728775);
    points += VSAPoint(1218.0340559380716, 904.072679650013);
    points += VSAPoint(1235.6835540738803, 928.3865393119875);
    points += VSAPoint(1252.4283690290056, 953.1614947139476);
    points += VSAPoint(1268.2306740094996, 978.33737701104);
    points += VSAPoint(1283.0526422214152, 1003.8540173584115);
    points += VSAPoint(1296.8564468708046, 1029.651246911209);
    points += VSAPoint(1309.60426116372, 1055.6688968245794);
    points += VSAPoint(1321.258258306214, 1081.846798253669);
    points += VSAPoint(1331.780611504339, 1108.1247823536248);
    points += VSAPoint(1341.1334939641479, 1134.4426802795938);
    points += VSAPoint(1349.2790788916927, 1160.7403231867224);
    points += VSAPoint(1352.8346456692914, 1173.8581417322835);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue646() const
{
    QVector<QPointF> points;

    points += QPointF(1391.2089508950362, 1170.962993556315);
    points += QPointF(1352.8346456692914, 1362.8345196850394);
    points += QPointF(1351.7927746622177, 1365.3174895470343);
    points += QPointF(1349.2904293989368, 1370.0604466887874);
    points += QPointF(1346.2962576995217, 1374.5430452326066);
    points += QPointF(1342.8219234115563, 1378.7733079571037);
    points += QPointF(1338.8790903826243, 1382.7592576408904);
    points += QPointF(1334.4794224603097, 1386.5089170625781);
    points += QPointF(1329.6345834921963, 1390.0303090007792);
    points += QPointF(1324.356237325868, 1393.3314562341047);
    points += QPointF(1315.7019501937195, 1397.9127915604581);
    points += QPointF(1302.6841808427803, 1403.289891141198);
    points += QPointF(1288.142331955491, 1407.930421917447);
    points += QPointF(1272.169714312523, 1411.8985661180982);
    points += QPointF(1254.8596386945478, 1415.2585059720464);
    points += QPointF(1236.3054158822356, 1418.074423708184);
    points += QPointF(1216.6003566562576, 1420.4105015554055);
    points += QPointF(1195.8377717972853, 1422.3309217426045);
    points += QPointF(1163.0181823648445, 1424.6044927975377);
    points += QPointF(1116.2675882169847, 1426.7215739585208);
    points += QPointF(1041.771617936041, 1429.0735145188771);
    points += QPointF(963.6986998754783, 1432.082545646023);
    points += QPointF(924.3395298428601, 1434.378255703845);
    points += QPointF(898.1648782802928, 1436.3169978244687);
    points += QPointF(872.1457412334543, 1438.6744512606858);
    points += QPointF(846.3754294830155, 1441.5147982413903);
    points += QPointF(820.9472538096477, 1444.9022209954755);
    points += QPointF(795.9545249940218, 1448.9009017518354);
    points += QPointF(771.4905538168089, 1453.5750227393632);
    points += QPointF(747.6486510586797, 1458.9887661869527);
    points += QPointF(724.5221275003055, 1465.2063143234973);
    points += QPointF(702.2042939223572, 1472.2918493778907);
    points += QPointF(691.4173228346457, 1476.2203464566928);
    points += QPointF(682.8822388593928, 1479.3575289855721);
    points += QPointF(665.6479146239083, 1485.1110371155814);
    points += QPointF(648.2139215316024, 1490.2399559097548);
    points += QPointF(630.5772803437565, 1494.74477312083);
    points += QPointF(612.7350118216522, 1498.6259765015448);
    points += QPointF(594.6841367265706, 1501.884053804636);
    points += QPointF(576.4216758197934, 1504.5194927828425);
    points += QPointF(557.9446498626021, 1506.5327811889006);
    points += QPointF(539.2500796162778, 1507.9244067755485);
    points += QPointF(520.334985842102, 1508.6948572955234);
    points += QPointF(501.1963893013563, 1508.844620501563);
    points += QPointF(481.8313107553219, 1508.3741841464052);
    points += QPointF(462.23677096528036, 1507.2840359827874);
    points += QPointF(442.40979069251296, 1505.5746637634468);
    points += QPointF(422.3473906983012, 1503.2465552411209);
    points += QPointF(402.0465917439264, 1500.300198168548);
    points += QPointF(381.50441459067014, 1496.7360802984645);
    points += QPointF(360.7178799998137, 1492.554689383609);
    points += QPointF(339.6840087326385, 1487.7565131767187);
    points += QPointF(318.399821550426, 1482.3420394305313);
    points += QPointF(296.8623392144576, 1476.3117558977842);
    points += QPointF(275.06858248601475, 1469.6661503312146);
    points += QPointF(253.01557212637874, 1462.4057104835604);
    points += QPointF(230.70032889683108, 1454.530924107559);
    points += QPointF(208.11987355865324, 1446.0422789559484);
    points += QPointF(185.27122687312647, 1436.9402627814657);
    points += QPointF(162.15140960153235, 1427.2253633368487);
    points += QPointF(138.7574425051522, 1416.8980683748348);
    points += QPointF(115.08634634526746, 1405.9588656481617);
    points += QPointF(91.13514188315949, 1394.4082429095668);
    points += QPointF(66.90084988010975, 1382.2466879117876);
    points += QPointF(42.38049109739966, 1369.4746884075616);
    points += QPointF(30.0, 1362.8345196850394);
    points += QPointF(-7.795275590551185, 1173.8581417322835);
    points += QPointF(-7.795275590551185, 5.332135175169643);
    points += QPointF(61.282362682462484, -0.6418380334156654);
    points += QPointF(68.99270783774104, 43.602497599341376);
    points += QPointF(72.9132813035755, 63.2947212555562);
    points += QPointF(77.3013929112068, 82.41949176853801);
    points += QPointF(82.1591612486953, 101.07290945528996);
    points += QPointF(87.47127192119272, 119.26236235620561);
    points += QPointF(93.22265002333297, 136.995242382529);
    points += QPointF(99.39845834352354, 154.27891130896086);
    points += QPointF(105.98409057569768, 171.12066698560935);
    points += QPointF(112.96515967796651, 187.5277107532596);
    points += QPointF(120.3274817141749, 203.50711700306462);
    points += QPointF(128.05705569238026, 219.06580572797276);
    points += QPointF(136.140040064258, 234.21051878050557);
    points += QPointF(144.56272665980467, 248.94780038335693);
    points += QPointF(153.31151289915712, 263.28398225332904);
    points += QPointF(162.37287314390684, 277.2251735037908);
    points += QPointF(171.7333300269448, 290.7772553015979);
    points += QPointF(181.37942653744733, 303.94588008167347);
    points += QPointF(191.29769954207885, 316.73647497324424);
    points += QPointF(201.4746553074722, 329.15424897723653);
    points += QPointF(211.8967474561181, 341.2042033492513);
    points += QPointF(222.55035765443486, 352.89114459720207);
    points += QPointF(233.42177919919385, 364.21969948550475);
    points += QPointF(244.49720354953487, 375.1943314526637);
    points += QPointF(255.7627097438719, 385.81935788241924);
    points += QPointF(267.20425655639707, 396.09896772498274);
    points += QPointF(278.75265348524636, 405.99011151219827);
    points += QPointF(296.42128624929416, 420.3133300264941);
    points += QPointF(320.48452011994203, 438.2320527150828);
    points += QPointF(344.87558321732604, 454.8117451133493);
    points += QPointF(369.53320753519137, 470.12563295711334);
    points += QPointF(394.3361710694346, 484.20351622969935);
    points += QPointF(419.16155857757707, 497.07530679920615);
    points += QPointF(443.88487919291146, 508.77121504768843);
    points += QPointF(468.38014199617356, 519.321880586359);
    points += QPointF(492.51986728337727, 528.7584613476223);
    points += QPointF(516.1750084818626, 537.1126962804665);
    points += QPointF(539.2147513582057, 544.4169583447066);
    points += QPointF(561.5061404146093, 550.7043180882887);
    points += QPointF(582.9134517220549, 556.0086464028718);
    points += QPointF(603.297176144298, 560.3648031296603);
    points += QPointF(622.512375480477, 563.8089969972691);
    points += QPointF(640.4059816430324, 566.3794874049449);
    points += QPointF(656.8122283884328, 568.1179931683923);
    points += QPointF(671.5446156432315, 569.0726461855179);
    points += QPointF(684.5714272485307, 569.308003986851);
    points += QPointF(690.2881064563063, 569.1505016813809);
    points += QPointF(697.6994465521921, 568.9116352687084);
    points += QPointF(712.7791990595856, 568.829069686213);
    points += QPointF(727.9225489586405, 569.2261552050226);
    points += QPointF(743.0205601588, 570.0941667183644);
    points += QPointF(758.0665280331999, 571.4236936735902);
    points += QPointF(773.0540417864709, 573.2052363093372);
    points += QPointF(787.9769792031311, 575.4292414839725);
    points += QPointF(802.8294963913348, 578.0861355014533);
    points += QPointF(817.6060134704758, 581.1663534276446);
    points += QPointF(832.3840681765656, 584.6800684918327);
    points += QPointF(854.2646050023646, 590.6246236981764);
    points += QPointF(883.0268449918112, 599.956221997473);
    points += QPointF(911.458987786853, 610.8015283739599);
    points += QPointF(939.4455853403952, 623.0622581751827);
    points += QPointF(966.9537607342976, 636.6682333974711);
    points += QPointF(993.9516330911679, 651.551057077312);
    points += QPointF(1020.4078966730023, 667.6440112755474);
    points += QPointF(1046.2914925169705, 684.8819135818707);
    points += QPointF(1071.5713620583545, 703.2009569179793);
    points += QPointF(1096.216269205861, 722.5385497176682);
    points += QPointF(1120.1946768785947, 742.8331673228507);
    points += QPointF(1143.4746649906992, 764.024220756791);
    points += QPointF(1166.0238785179056, 786.0519457597054);
    points += QPointF(1187.8094960863832, 808.8573128063047);
    points += QPointF(1208.7982112227505, 832.3819574965946);
    points += QPointF(1228.9562198237943, 856.5681299364858);
    points += QPointF(1248.2492085181873, 881.3586613164372);
    points += QPointF(1266.6423393728558, 906.6969456633109);
    points += QPointF(1284.1002268781353, 932.5269345592678);
    points += QPointF(1300.5869033558602, 958.7931423679255);
    points += QPointF(1316.065768928307, 985.4406590911037);
    points += QPointF(1330.4995219936793, 1012.4151672497809);
    points += QPointF(1343.850065889632, 1039.6629580676176);
    points += QPointF(1356.0783871611402, 1067.1309405095865);
    points += QPointF(1367.1444008135065, 1094.7666342875323);
    points += QPointF(1377.006758382479, 1122.5181345255592);
    points += QPointF(1385.5816561538945, 1150.2017940249868);
    points += QPointF(1391.2089508950362, 1170.962993556315);
    return points;
}
