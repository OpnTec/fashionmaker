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

    QTest::newRow("Issue 548. Case2") << InputPointsIssue548Case2()
                                      << 37.795275590551185 // seam allowance width (1.0 cm)
                                      << OutputPointsIssue548Case2();

    QTest::newRow("Issue 548. Case3") << InputPointsIssue548Case3()
                                      << 75.59055118110237 // seam allowance width (2.0 cm)
                                      << OutputPointsIssue548Case3();
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
    points += QPointF(30.0, 2.1993068570824366);
    points += QPointF(114.18582053395916, -1.0122621856204752);
    points += QPointF(193.32267013775137, -14.358819619401812);
    points += QPointF(242.10707671244683, -32.17908690671806);
    points += QPointF(271.8971162152593, -50.207724037690014);
    points += QPointF(295.9981384039675, -73.05193874264779);
    points += QPointF(291.7916402578659, -67.42426694301096);
    points += QPointF(556.8930273120665, -30.847456102511416);
    points += QPointF(539.6101141051189, 79.89131577778163);
    points += QPointF(585.9167315845334, -20.702420721823447);
    points += QPointF(1117.424153174025, 212.64949937811681);
    points += QPointF(1075.303535889726, 303.4417262358707);
    points += QPointF(1034.367455532369, 409.91489458521534);
    points += QPointF(1016.7086496269483, 470.1391211588565);
    points += QPointF(1002.6502749276095, 549.8913110678527);
    points += QPointF(1002.7092529129402, 594.4292010581551);
    points += QPointF(1004.1768887194628, 604.1457258376107);
    points += QPointF(1009.2070121912753, 620.6931338009274);
    points += QPointF(1019.1613725437678, 641.9748709736807);
    points += QPointF(1033.0209611044565, 662.9678611314132);
    points += QPointF(1049.114087400246, 680.4406818558615);
    points += QPointF(1065.8570577426399, 692.968441452634);
    points += QPointF(1083.1677152645557, 700.9292457138567);
    points += QPointF(1105.2479226099044, 705.3103889738845);
    points += QPointF(1290.6502488400083, 712.9440934612204);
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
    points += QPointF(445.748031496063, 252.741597064484);
    points += QPointF(407.8726873421697, 270.9512393848742);
    points += QPointF(383.35720786819263, 278.3073990947694);
    points += QPointF(358.56601090183574, 281.29234689511463);
    points += QPointF(311.52157429670274, 276.0460457687877);
    points += QPointF(269.8658512160491, 259.9136567147091);
    points += QPointF(234.63885608185402, 236.87965105367914);
    points += QPointF(206.54666866938726, 209.66914671340476);
    points += QPointF(196.38622691315896, 194.30278046741873);
    points += QPointF(185.94813049770778, 196.9895807275079);
    points += QPointF(139.55536916394507, 195.8595124134453);
    points += QPointF(94.68200851242605, 182.9587775062589);
    points += QPointF(53.04420235483193, 161.22643763678528);
    points += QPointF(17.41948377984963, 131.4802352488134);
    points += QPointF(3.037173945815, 113.17182142233789);
    points += QPointF(-7.8903334763093635, 91.91076010344419);
    points += QPointF(-13.507576311258223, 67.42815708759109);
    points += QPointF(-11.452400232028667, 42.67167813028373);
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
    points += QPointF(446.1486580403833, 2.204598425196849);
    points += QPointF(443.58722975094355, 245.07202661708763);
    points += QPointF(425.1137484290017, 268.68251714626723);
    points += QPointF(410.710702976016, 281.17394700785115);
    points += QPointF(394.6305380089911, 289.87868829070504);
    points += QPointF(377.68935972259976, 294.42039446773254);
    points += QPointF(345.2891679003724, 291.71681887702744);
    points += QPointF(318.8954558500656, 277.387541146342);
    points += QPointF(299.836181304411, 255.59117266176213);
    points += QPointF(288.9960506469964, 228.68278781028684);
    points += QPointF(288.09978928853826, 198.89092343273913);
    points += QPointF(295.52398476771754, 175.7944621402967);
    points += QPointF(298.35692700564954, 155.95491384399136);
    points += QPointF(296.64544203481944, 147.53499165718873);
    points += QPointF(295.62311476307525, 146.6781151558174);
    points += QPointF(289.83631178459234, 145.65886137089512);
    points += QPointF(263.86302969143765, 151.02798077389028);
    points += QPointF(248.0011124529875, 159.32004134517877);
    points += QPointF(209.38086747344968, 181.82248571472272);
    points += QPointF(187.81719911027162, 188.34500166877712);
    points += QPointF(165.71167711395094, 187.9030433207368);
    points += QPointF(146.5634417196521, 177.22168648881313);
    points += QPointF(136.34913944009722, 161.0107028740396);
    points += QPointF(132.35548185589735, 132.14155778793625);
    points += QPointF(124.85769773414223, 138.3166750745855);
    points += QPointF(106.38084062481717, 148.13624736643095);
    points += QPointF(86.3477370523643, 153.01816260877774);
    points += QPointF(66.26289964510504, 152.16787828189516);
    points += QPointF(48.254978841826016, 146.22324827751663);
    points += QPointF(21.027781980991552, 125.39244823236324);
    points += QPointF(6.486477180841129, 105.14630485769516);
    points += QPointF(-4.426684646901568, 80.5250619079355);
    points += QPointF(-10.01311360817298, 55.62988319219602);
    points += QPointF(-8.687212589418436, 33.403943526036805);
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
    points.append(VSAPoint(787.5835464566929, 1701.3138897637796));
    points.append(VSAPoint(938.7646488188976, 1701.3138897637796));
    points.append(VSAPoint(928.6149958683911, 1732.4440719866434));
    points.append(VSAPoint(910.0209091217698, 1792.3369853889722));
    points.append(VSAPoint(893.3643262819251, 1849.7845131987456));
    points.append(VSAPoint(878.5244039283091, 1905.2261617043234));
    points.append(VSAPoint(865.3802986403739, 1959.101437194065));
    points.append(VSAPoint(863.9366982685195, 1965.6834024491068));
    points.append(VSAPoint(852.8936778444679, 1919.6965437838999));
    points.append(VSAPoint(837.0628180560684, 1860.2846653184251));
    points.append(VSAPoint(819.0677656132684, 1798.6758641921479));
    points.append(VSAPoint(798.7585839758027, 1734.54810216256));
    points.append(VSAPoint(787.5835464566929, 1701.3138897637796));

    qreal width = 37.795275590551185; // seam allowance width

    QVector<QPointF> ekvOrig;
    ekvOrig.append(QPointF(774.8748468280837, 1663.5186141732283));
    ekvOrig.append(QPointF(990.8407795072413, 1663.5186141732283));
    ekvOrig.append(QPointF(964.6314912875667, 1743.9055911653147));
    ekvOrig.append(QPointF(946.2221157804494, 1803.203536155223));
    ekvOrig.append(QPointF(929.7733291125676, 1859.9343877726233));
    ekvOrig.append(QPointF(915.1430746962241, 1914.5927211230298));
    ekvOrig.append(QPointF(902.2033544443959, 1967.630259856634));
    ekvOrig.append(QPointF(894.4064781634931, 2003.1794116713015));
    ekvOrig.append(QPointF(834.213891302752, 2003.7742535883901));
    ekvOrig.append(QPointF(816.2523103379473, 1928.9761772004185));
    ekvOrig.append(QPointF(800.6574884611877, 1870.4501290629887));
    ekvOrig.append(QPointF(782.9077417718742, 1809.6811695225983));
    ekvOrig.append(QPointF(786.7126382487066, 1698.723835966227));

    QTest::newRow("GAVAUDAN Laure.") << points << width << ekvOrig;

    points.clear();
    points.append(VSAPoint(97.33089106412862, -223.03306117556497));
    points.append(VSAPoint(990.7494050554426, 2.819093995045));
    points.append(VSAPoint(908.3966357321774, 379.5839357215547));
    points.append(VSAPoint(-135.41154226686143, 697.6417881399819));

    width = 11.338582677165354;

    ekvOrig.clear();
    ekvOrig.append(VSAPoint(100.10981413873267, -234.02583351343978));
    ekvOrig.append(VSAPoint(1004.1704360325447, -5.483401649771952));
    ekvOrig.append(VSAPoint(918.0553412376563, 388.4941212347381));
    ekvOrig.append(VSAPoint(-138.65807550610091, 710.4843173601864));

    // See the file "collection/bugs/Issue_#604.val" (since 0.5.0)
    QTest::newRow("Issue #604.") << points << width << ekvOrig;
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
    expect << QPointF(407.9527559055118, 3819.527433070866);
    expect << QPointF(30.0, 3819.527433070866);

    QTest::newRow("Test case issue #548") << before << expect;
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
QVector<QPointF> TST_VAbstractPiece::InputPointsIssue298Case1() const
{
    QVector<QPointF> points;

    points += QPointF(35, 39.9999);
    points += QPointF(412.953, 39.9999);
    points += QPointF(417.135, 417.929);
    points += QPointF(417.135, 417.929);
    points += QPointF(408.797, 405.589);
    points += QPointF(390.909, 377.669);
    points += QPointF(362.315, 330.86);
    points += QPointF(323.075, 264.247);
    points += QPointF(286.15, 201.448);
    points += QPointF(262.477, 162.745);
    points += QPointF(249.22, 142.455);
    points += QPointF(241.092, 131.261);
    points += QPointF(236.545, 125.75);
    points += QPointF(232.808, 122.058);
    points += QPointF(230.6, 120.629);
    points += QPointF(229.393, 120.277);
    points += QPointF(228.421, 120.456);
    points += QPointF(227.69, 121.185);
    points += QPointF(227.033, 123.272);
    points += QPointF(227.112, 128.232);
    points += QPointF(228.29, 135.699);
    points += QPointF(230.625, 145.81);
    points += QPointF(234.173, 158.703);
    points += QPointF(241.73, 183.168);
    points += QPointF(248.796, 204.144);
    points += QPointF(248.796, 204.144);
    points += QPointF(251.528, 212.406);
    points += QPointF(255.482, 227.075);
    points += QPointF(257.717, 239.591);
    points += QPointF(258.279, 247.554);
    points += QPointF(258.203, 252.278);
    points += QPointF(257.756, 256.51);
    points += QPointF(256.949, 260.264);
    points += QPointF(255.795, 263.547);
    points += QPointF(254.308, 266.372);
    points += QPointF(252.501, 268.749);
    points += QPointF(250.385, 270.688);
    points += QPointF(247.974, 272.201);
    points += QPointF(245.281, 273.296);
    points += QPointF(242.319, 273.986);
    points += QPointF(239.1, 274.28);
    points += QPointF(233.846, 274.05);
    points += QPointF(226.022, 272.393);
    points += QPointF(217.402, 269.345);
    points += QPointF(208.09, 264.991);
    points += QPointF(198.186, 259.414);
    points += QPointF(187.795, 252.7);
    points += QPointF(177.019, 244.933);
    points += QPointF(165.96, 236.197);
    points += QPointF(154.721, 226.576);
    points += QPointF(143.405, 216.157);
    points += QPointF(132.113, 205.022);
    points += QPointF(120.95, 193.257);
    points += QPointF(110.017, 180.946);
    points += QPointF(99.4167, 168.174);
    points += QPointF(89.2522, 155.024);
    points += QPointF(79.626, 141.582);
    points += QPointF(70.6405, 127.933);
    points += QPointF(62.3985, 114.16);
    points += QPointF(55.0025, 100.348);
    points += QPointF(48.5551, 86.5823);
    points += QPointF(43.159, 72.9466);
    points += QPointF(38.9167, 59.5258);
    points += QPointF(35.9309, 46.4042);
    points += QPointF(35, 39.9999);

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
QVector<QPointF> TST_VAbstractPiece::InputPointsIssue298Case2() const
{
    QVector<QPointF> points;

    points += QPointF(35, 39.9999);
    points += QPointF(35, 39.9999);
    points += QPointF(35.9309, 46.4042);
    points += QPointF(38.9167, 59.5258);
    points += QPointF(43.159, 72.9466);
    points += QPointF(48.5551, 86.5823);
    points += QPointF(55.0025, 100.348);
    points += QPointF(62.3985, 114.16);
    points += QPointF(70.6405, 127.933);
    points += QPointF(79.626, 141.582);
    points += QPointF(89.2522, 155.024);
    points += QPointF(99.4167, 168.174);
    points += QPointF(110.017, 180.946);
    points += QPointF(120.95, 193.257);
    points += QPointF(132.113, 205.022);
    points += QPointF(143.405, 216.157);
    points += QPointF(154.721, 226.576);
    points += QPointF(165.96, 236.197);
    points += QPointF(177.019, 244.933);
    points += QPointF(187.795, 252.7);
    points += QPointF(198.186, 259.414);
    points += QPointF(208.09, 264.991);
    points += QPointF(217.402, 269.345);
    points += QPointF(226.022, 272.393);
    points += QPointF(233.846, 274.05);
    points += QPointF(239.1, 274.28);
    points += QPointF(242.319, 273.986);
    points += QPointF(245.281, 273.296);
    points += QPointF(247.974, 272.201);
    points += QPointF(250.385, 270.688);
    points += QPointF(252.501, 268.749);
    points += QPointF(254.308, 266.372);
    points += QPointF(255.795, 263.547);
    points += QPointF(256.949, 260.264);
    points += QPointF(257.756, 256.51);
    points += QPointF(258.203, 252.278);
    points += QPointF(258.279, 247.554);
    points += QPointF(257.717, 239.591);
    points += QPointF(255.482, 227.075);
    points += QPointF(251.528, 212.406);
    points += QPointF(248.796, 204.144);
    points += QPointF(248.796, 204.144);
    points += QPointF(241.73, 183.168);
    points += QPointF(234.173, 158.703);
    points += QPointF(230.625, 145.81);
    points += QPointF(228.29, 135.699);
    points += QPointF(227.112, 128.232);
    points += QPointF(227.033, 123.272);
    points += QPointF(227.69, 121.185);
    points += QPointF(228.421, 120.456);
    points += QPointF(229.393, 120.277);
    points += QPointF(230.6, 120.629);
    points += QPointF(232.808, 122.058);
    points += QPointF(236.545, 125.75);
    points += QPointF(241.092, 131.261);
    points += QPointF(249.22, 142.455);
    points += QPointF(262.477, 162.745);
    points += QPointF(286.15, 201.448);
    points += QPointF(323.075, 264.247);
    points += QPointF(362.315, 330.86);
    points += QPointF(390.909, 377.669);
    points += QPointF(408.797, 405.589);
    points += QPointF(417.135, 417.929);
    points += QPointF(417.135, 417.929);
    points += QPointF(35, 417.953);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue298Case2() const
{
    QVector<QPointF> points;

    points += QPointF(-2.7952999999999975, 4.8384699505981095);
    points += QPointF(67.34448942068963, -0.23248582689164274);
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
    points += QPointF(215.3306509043856, 223.3387762725701);
    points += QPointF(205.75073516810195, 194.75155680967347);
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
    points += QPointF(318.5597512322288, 182.00074197391842);
    points += QPointF(394.73028222951507, 311.42213969492946);
    points += QPointF(422.9514429826756, 357.62079373755);
    points += QPointF(440.37197676737753, 384.8111617646563);
    points += QPointF(488.2841719585649, 455.71983154868764);
    points += QPointF(-2.795300000000013, 455.7506738094777);
    points += QPointF(-2.7952999999999975, 4.8384699505981095);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::InputPointsIssue548Case1() const
{
    QVector<QPointF> points;

    points += QPointF(236.97989607468364, 65.89325192030674);
    points += QPointF(198.93409106041895, 172.04876297154925);
    points += QPointF(260.32251114299453, 75.38027418944861);
    points += QPointF(324.54110236213444, 101.48031496062993);
    points += QPointF(29.858267716535437, 300.85039370078744);
    points += QPointF(99.86433649395013, 10.166060970128015);

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
QVector<QPointF> TST_VAbstractPiece::InputPointsIssue548Case2() const
{
    QVector<QPointF> points;
    points << QPointF(99.86433649395013, 10.166060970128015);
    points << QPointF(236.97989607468364, 65.89325192030674);
    points << QPointF(198.93409106041895, 172.04876297154925);
    points << QPointF(260.32251114299453, 75.38027418944861);
    points << QPointF(324.54110236213444, 101.48031496062993);
    points << QPointF(29.858267716535437, 300.85039370078744);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue548Case2() const
{
    QVector<QPointF> points;
    points << QPointF(73.40376616581447, -41.38574336196901);
    points << QPointF(245.32830125796568, 28.488685370970344);
    points << QPointF(245.32830125796573, 28.488685370970277);
    points << QPointF(404.3486874792147, 93.11854543221973);
    points << QPointF(29.598648843228922, 346.6587450186291);
    points << QPointF(-12.946885351826726, 317.1657644661815);
    points << QPointF(73.40376616581447, -41.38574336196901);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::InputPointsIssue548Case3() const
{
    QVector<QPointF> points;

    points += QPointF(99.86433649395013, 10.166060970128015);
    points += QPointF(236.97989607468364, 65.89325192030674);
    points += QPointF(198.93409106041895, 172.04876297154925);
    points += QPointF(260.32251114299453, 75.38027418944861);
    points += QPointF(324.54110236213444, 101.48031496062993);
    points += QPointF(29.858267716535437, 300.85039370078744);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractPiece::OutputPointsIssue548Case3() const
{
    QVector<QPointF> points;

    points += QPointF(46.94319583767885, -92.9375476940661);
#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
    points += QPointF(234.2633962639462, -16.805935717278903);
#else
    points += QPointF(238.798634936, -14.9627013515);
#endif
    points += QPointF(484.15627259629446, 84.75677590380938);
    points += QPointF(29.339029969922702, 392.46709633647066);
    points += QPointF(-55.75203842018885, 333.48113523157537);
    points += QPointF(46.94319583767885, -92.9375476940661);

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
