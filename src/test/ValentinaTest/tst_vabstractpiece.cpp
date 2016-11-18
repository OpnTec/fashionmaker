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
