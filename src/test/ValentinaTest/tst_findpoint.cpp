/************************************************************************
 **
 **  @file   tst_findpoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 1, 2016
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

#include "tst_findpoint.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectioncurves.h"
#include "../vtools/tools/drawTools/toolpoint/tooldoublepoint/vtooltruedarts.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoollineintersectaxis.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/vtooltriangle.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolshoulderpoint.h"
#include "../vtools/tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolcurveintersectaxis.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_FindPoint::TST_FindPoint(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestPointOfIntersectionCurves_data()
{
    QTest::addColumn<QVector<QPointF>>("curve1Points");
    QTest::addColumn<QVector<QPointF>>("curve2Points");
    QTest::addColumn<int>("vCross");
    QTest::addColumn<int>("hCross");
    QTest::addColumn<QPointF>("expect");

    QVector<QPointF> points1;
    QVector<QPointF> points2;

    VCrossCurvesPoint v = VCrossCurvesPoint::HighestPoint;
    HCrossCurvesPoint h = HCrossCurvesPoint::LeftmostPoint;
    QPointF p;

    QTest::newRow("Empty lists of points") << points1 << points2 << static_cast<int>(v) << static_cast<int>(h) << p;

    points1.clear();
    points1.append(QPointF(10, 10));
    points1.append(QPointF(15, 20));
    points1.append(QPointF(10, 30));

    points2.clear();
    points2.append(QPointF(30, 10));
    points2.append(QPointF(25, 20));
    points2.append(QPointF(30, 30));
    QTest::newRow("No intersections") << points1 << points2 << static_cast<int>(v) << static_cast<int>(h) << p;

    points1.clear();
    points1.append(QPointF(10, 10));
    points1.append(QPointF(20, 20));
    points1.append(QPointF(10, 30));

    points2.clear();
    points2.append(QPointF(30, 10));
    points2.append(QPointF(20, 20));
    points2.append(QPointF(30, 30));

    p = QPointF(20, 20);
    QTest::newRow("One intersection point") << points1 << points2 << static_cast<int>(v) << static_cast<int>(h) << p;

    points1.clear();
    points1.append(QPointF(10, 10));
    points1.append(QPointF(25, 20));
    points1.append(QPointF(10, 30));

    points2.clear();
    points2.append(QPointF(25, 10));
    points2.append(QPointF(15, 20));
    points2.append(QPointF(25, 30));

    p = QPointF(19, 16);

    h = HCrossCurvesPoint::LeftmostPoint;
    QTest::newRow("Two intersection points (highest point, leftmost point)") << points1 << points2
                                                                             << static_cast<int>(v)
                                                                             << static_cast<int>(h) << p;

    h = HCrossCurvesPoint::RightmostPoint;
    QTest::newRow("Two intersection points (highest point, rightmost point)") << points1 << points2
                                                                              << static_cast<int>(v)
                                                                              << static_cast<int>(h) << p;

    v = VCrossCurvesPoint::LowestPoint;
    p = QPointF(19, 24);

    h = HCrossCurvesPoint::LeftmostPoint;
    QTest::newRow("Two intersection points (lowest point, leftmost point)") << points1 << points2
                                                                            << static_cast<int>(v)
                                                                            << static_cast<int>(h) << p;

    h = HCrossCurvesPoint::RightmostPoint;
    QTest::newRow("Two intersection points (lowest point, rightmost point)") << points1 << points2
                                                                             << static_cast<int>(v)
                                                                             << static_cast<int>(h) << p;

    points1.clear();
    points1.append(QPointF(10, 10));
    points1.append(QPointF(20, 25));
    points1.append(QPointF(30, 10));

    points2.clear();
    points2.append(QPointF(10, 30));
    points2.append(QPointF(20, 15));
    points2.append(QPointF(30, 30));

    h = HCrossCurvesPoint::LeftmostPoint;
    p = QPointF(16.6667, 20);

    v = VCrossCurvesPoint::HighestPoint;
    QTest::newRow("Two intersection points (leftmost point, highest point)") << points1 << points2
                                                                             << static_cast<int>(v)
                                                                             << static_cast<int>(h) << p;

    v = VCrossCurvesPoint::LowestPoint;
    QTest::newRow("Two intersection points (leftmost point, lowest point)") << points1 << points2
                                                                            << static_cast<int>(v)
                                                                            << static_cast<int>(h) << p;

    h = HCrossCurvesPoint::RightmostPoint;
    p = QPointF(23.3333, 20);

    v = VCrossCurvesPoint::HighestPoint;
    QTest::newRow("Two intersection points (rightmost point, highest point)") << points1 << points2
                                                                              << static_cast<int>(v)
                                                                              << static_cast<int>(h) << p;

    v = VCrossCurvesPoint::LowestPoint;
    QTest::newRow("Two intersection points (rightmost point, lowest point)") << points1 << points2
                                                                             << static_cast<int>(v)
                                                                             << static_cast<int>(h) << p;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestPointOfIntersectionCurves()
{
    QFETCH(QVector<QPointF>, curve1Points);
    QFETCH(QVector<QPointF>, curve2Points);
    QFETCH(int, vCross);
    QFETCH(int, hCross);
    QFETCH(QPointF, expect);

    const QPointF result = VToolPointOfIntersectionCurves::FindPoint(curve1Points, curve2Points,
                                                                     static_cast<VCrossCurvesPoint>(vCross),
                                                                     static_cast<HCrossCurvesPoint>(hCross));
    QCOMPARE(result.toPoint(), expect.toPoint());
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestTrueDarts_data()
{
    QTest::addColumn<QPointF>("baseLineP1");
    QTest::addColumn<QPointF>("baseLineP2");
    QTest::addColumn<QPointF>("dartP1");
    QTest::addColumn<QPointF>("dartP2");
    QTest::addColumn<QPointF>("dartP3");
    QTest::addColumn<QPointF>("expectP1");
    QTest::addColumn<QPointF>("expectP2");

    const QPointF baseLineP1(30.0, 3094.31433071);
    const QPointF baseLineP2(621.006962676, 3222.38611313);
    const QPointF dartP1(196.220708253, 3130.33451951);
    const QPointF dartP2(196.220708253, 3470.49199983);
    const QPointF dartP3(270.096578587, 3146.34349232);
    const QPointF p1(196.220708253, 3106.93562497);
    const QPointF p2(277.006407384, 3116.02492305);

    QTest::newRow("Real case") << baseLineP1 << baseLineP2 << dartP1 << dartP2 << dartP3 << p1 << p2;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestTrueDarts()
{
    QFETCH(QPointF, baseLineP1);
    QFETCH(QPointF, baseLineP2);
    QFETCH(QPointF, dartP1);
    QFETCH(QPointF, dartP2);
    QFETCH(QPointF, dartP3);
    QFETCH(QPointF, expectP1);
    QFETCH(QPointF, expectP2);

    QPointF p1;
    QPointF p2;

    VToolTrueDarts::FindPoint(baseLineP1, baseLineP2, dartP1, dartP2, dartP3, p1, p2);

    QCOMPARE(p1.toPoint(), expectP1.toPoint());
    QCOMPARE(p2.toPoint(), expectP2.toPoint());
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestLineIntersectAxis_data()
{
    QTest::addColumn<QLineF>("axis");
    QTest::addColumn<QLineF>("line");
    QTest::addColumn<QPointF>("point");

    const QLineF axis(0, 0, 1, 0);
    const QLineF line(0, 1, 1, 1);
    const QPointF point(0, 0);

    QTest::newRow("Parallel lines") << axis << line << point;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestLineIntersectAxis()
{
    QFETCH(QLineF, axis);
    QFETCH(QLineF, line);
    QFETCH(QPointF, point);

    QPointF resultPoint = VToolLineIntersectAxis::FindPoint(axis, line);
    QCOMPARE(point, resultPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestTriangle_data()
{
    QTest::addColumn<QPointF>("axisP1");
    QTest::addColumn<QPointF>("axisP2");
    QTest::addColumn<QPointF>("firstPoint");
    QTest::addColumn<QPointF>("secondPoint");
    QTest::addColumn<QPointF>("point");

    const QPointF axisP1(0, 0);
    const QPointF axisP2(0, 1);
    const QPointF firstPoint(1, 0);
    const QPointF secondPoint(1, 1);
    const QPointF point(0, 0);

    QTest::newRow("Parallel lines") << axisP1 << axisP2 << firstPoint << secondPoint << point;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestTriangle()
{
    QFETCH(QPointF, axisP1);
    QFETCH(QPointF, axisP2);
    QFETCH(QPointF, firstPoint);
    QFETCH(QPointF, secondPoint);
    QFETCH(QPointF, point);

    QPointF resultPoint = VToolTriangle::FindPoint(axisP1, axisP2, firstPoint, secondPoint);
    QCOMPARE(point, resultPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestShoulderPoint_data()
{
    QTest::addColumn<QPointF>("p1");
    QTest::addColumn<QPointF>("p2");
    QTest::addColumn<QPointF>("pShoulder");
    QTest::addColumn<qreal>("length");
    QTest::addColumn<QPointF>("point");

    // See file <root>/src/app/share/collection/bugs/Issue_#647.val
    QTest::newRow("Value found") << QPointF(-234.5669291338583, 39.999874015748034)
                                 << QPointF(-574.724409448819, 115.5904251968504)
                                 << QPointF(-234.5669291338583, -35.590677165354336)
                                 << 566.92913385826773
                                 << QPointF(-767.2805101289953, 158.3806697924456);

    // The same file <root>/src/app/share/collection/bugs/Issue_#647.val
    // The length changed to get default value
    QPointF p2(-574.724409448819, 115.5904251968504);
    QTest::newRow("Value not found") << QPointF(-234.5669291338583, 39.999874015748034)
                                     << p2
                                     << QPointF(-234.5669291338583, -35.590677165354336)
                                     << 75.59055118110237
                                     << p2;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestShoulderPoint()
{
    QFETCH(QPointF, p1);
    QFETCH(QPointF, p2);
    QFETCH(QPointF, pShoulder);
    QFETCH(qreal, length);
    QFETCH(QPointF, point);

    QPointF resultPoint = VToolShoulderPoint::FindPoint(p1, p2, pShoulder, length);

    QCOMPARE(point, resultPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestCurveIntersectAxis_data()
{
    QTest::addColumn<QPointF>("basePoint");
    QTest::addColumn<qreal>("angle");
    QTest::addColumn<QVector<QPointF>>("curvePoints");
    QTest::addColumn<QPointF>("result");

    QVector<QPointF> curvePoints =
    {
        QPointF(30.0, 389.8454173228347),
        QPointF(33.41360430380303, 377.5329842829309),
        QPointF(38.88356333735831, 358.03346683620055),
        QPointF(42.732571055804584, 344.5850891640163),
        QPointF(46.76975879182068, 330.81193820645024),
        QPointF(51.0088665382775, 316.7698517299701),
        QPointF(54.3360342256197, 306.09003723760134),
        QPointF(56.61990934999518, 298.9196402662395),
        QPointF(58.96199322297076, 291.7134181587876),
        QPointF(61.3640033436553, 284.47835063605385),
        QPointF(63.82765721115766, 277.2214174188468),
        QPointF(66.35467232458672, 269.949598227975),
        QPointF(68.94676618305135, 262.669872784247),
        QPointF(71.6056562856604, 255.38922080847115),
        QPointF(74.33306013152277, 248.11462202145609),
        QPointF(77.13069521974728, 240.85305614401028),
        QPointF(80.00027904944284, 233.61150289694217),
        QPointF(82.94352911971828, 226.3969420010603),
        QPointF(85.9621629296825, 219.21635317717315),
        QPointF(89.05789797844437, 212.0767161460892),
        QPointF(92.23245176511276, 204.98501062861698),
        QPointF(95.48754178879652, 197.948216345565),
        QPointF(98.8248855486045, 190.9733130177417),
        QPointF(102.24620054364559, 184.06728036595564),
        QPointF(105.75320427302867, 177.23709811101529),
        QPointF(109.3476142358626, 170.4897459737291),
        QPointF(113.03114793125624, 163.83220367490566),
        QPointF(116.80552285831845, 157.2714509353534),
        QPointF(120.67245651615812, 150.8144674758809),
        QPointF(124.63366640388409, 144.46823301729654),
        QPointF(127.67051583651562, 139.7892775532494),
        QPointF(129.72343810820792, 136.70576556288137),
        QPointF(131.8008955306205, 133.65386691569014),
        QPointF(133.90310279114203, 130.6344540767768),
        QPointF(136.0302745771611, 127.64839951124239),
        QPointF(138.18262557606633, 124.69657568418802),
        QPointF(140.3603704752463, 121.77985506071471),
        QPointF(142.56372396208963, 118.89911010592354),
        QPointF(144.79290072398493, 116.05521328491558),
        QPointF(147.04811544832083, 113.24903706279188),
        QPointF(149.3295828224859, 110.48145390465352),
        QPointF(151.63751753386873, 107.75333627560153),
        QPointF(153.97213426985797, 105.06555664073699),
        QPointF(156.33364771784224, 102.41898746516097),
        QPointF(158.72227256521015, 99.81450121397455),
        QPointF(161.13822349935026, 97.25297035227874),
        QPointF(163.5817152076512, 94.73526734517463),
        QPointF(166.0529623775016, 92.2622646577633),
        QPointF(168.55217969629004, 89.83483475514578),
        QPointF(171.07958185140512, 87.45385010242313),
        QPointF(173.63538353023552, 85.12018316469644),
        QPointF(176.21979942016975, 82.83470640706676),
        QPointF(178.8330442085965, 80.59829229463514),
        QPointF(181.47533258290434, 78.4118132925027),
        QPointF(184.1468792304819, 76.2761418657704),
        QPointF(186.84789883871775, 74.1921504795394),
        QPointF(189.5786060950005, 72.1607115989107),
        QPointF(192.33921568671883, 70.1826976889854),
        QPointF(195.12994230126128, 68.25898121486452),
        QPointF(197.95100062601648, 66.39043464164916),
        QPointF(200.80260534837305, 64.57793043444039),
        QPointF(203.68497115571955, 62.82234105833922),
        QPointF(206.59831273544458, 61.12453897844675),
        QPointF(209.54284477493684, 59.48539665986404),
        QPointF(212.51878196158492, 57.90578656769215),
        QPointF(215.52633898277736, 56.38658116703215),
        QPointF(218.5657305259028, 54.92865292298508),
        QPointF(221.63717127834985, 53.532874300652026),
        QPointF(224.74087592750715, 52.20011776513403),
        QPointF(227.8770591607633, 50.93125578153217),
        QPointF(231.04593566550685, 49.727160814947496),
        QPointF(234.24772012912646, 48.588705330481076),
        QPointF(237.48262723901072, 47.51676179323398),
        QPointF(240.75087168254825, 46.51220266830725),
        QPointF(244.05266814712766, 45.57590042080197),
        QPointF(247.38823132013755, 44.708727515819184),
        QPointF(250.75777588896653, 43.911556418459966),
        QPointF(254.1615165410032, 43.18525959382538),
        QPointF(257.5996679636362, 42.53070950701647),
        QPointF(261.07244484425416, 41.94877862313432),
        QPointF(264.5800618702456, 41.44033940727998),
        QPointF(268.12273372899915, 41.006264324554515),
        QPointF(271.7006751079034, 40.64742584005899),
        QPointF(275.3141006943471, 40.36469641889447),
        QPointF(278.96322517571866, 40.15894852616201),
        QPointF(282.6482632394068, 40.03105462696267),
        QPointF(284.5058267716535, 39.999874015748034),
        QPointF(286.36444088974866, 39.98048479378051),
        QPointF(290.06193824498285, 39.99342662697727),
        QPointF(293.73568583217656, 40.06802832189854),
        QPointF(297.38560286144866, 40.20384046650693),
        QPointF(301.0116085429178, 40.40041364876503),
        QPointF(304.6136220867028, 40.65729845663546),
        QPointF(308.1915627029223, 40.974045478080825),
        QPointF(311.7453496016951, 41.35020530106372),
        QPointF(315.27490199314, 41.785328513546744),
        QPointF(318.7801390873757, 42.2789657034925),
        QPointF(322.2609800945208, 42.8306674588636),
        QPointF(325.7173442246942, 43.43998436762264),
        QPointF(329.14915068801463, 44.10646701773223),
        QPointF(332.5563186946008, 44.82966599715496),
        QPointF(335.93876745457146, 45.60913189385346),
        QPointF(339.2964161780453, 46.44441529579029),
        QPointF(342.62918407514115, 47.335066790928096),
        QPointF(345.93699035597774, 48.280636967229455),
        QPointF(349.2197542306738, 49.28067641265699),
        QPointF(352.47739490934805, 50.33473571517328),
        QPointF(355.7098316021192, 51.44236546274095),
        QPointF(358.9169835191061, 52.603116243322596),
        QPointF(362.0987698704274, 53.816538644880815),
        QPointF(365.2551098662019, 55.08218325537821),
        QPointF(368.3859227165484, 56.399600662777395),
        QPointF(371.4911276315854, 57.76834145504097),
        QPointF(374.5706438214319, 59.18795622013153),
        QPointF(377.6243904962065, 60.65799554601169),
        QPointF(380.65228686602796, 62.178010020644045),
        QPointF(383.65425214101515, 63.74755023199119),
        QPointF(386.6302055312866, 65.36616676801576),
        QPointF(389.5800662469612, 67.03341021668032),
        QPointF(392.5037534981577, 68.7488311659475),
        QPointF(395.4011864949948, 70.51198020377987),
        QPointF(398.27228444759123, 72.32240791814007),
        QPointF(401.11696656606574, 74.1796648969907),
        QPointF(403.93515206053706, 76.08330172829434),
        QPointF(406.72676014112403, 78.0328690000136),
        QPointF(409.4917100179452, 80.02791730011108),
        QPointF(412.2299209011194, 82.06799721654943),
        QPointF(414.9413120007655, 84.15265933729117),
        QPointF(417.62580252700207, 86.28145425029896),
        QPointF(420.28331168994794, 88.45393254353542),
        QPointF(422.9137586997218, 90.66964480496313),
        QPointF(425.5170627664425, 92.92814162254464),
        QPointF(428.09314310022864, 95.22897358424262),
        QPointF(430.6419189111992, 97.57169127801969),
        QPointF(433.1633094094726, 99.95584529183839),
        QPointF(435.6572338051678, 102.38098621366132),
        QPointF(438.1236113084035, 104.84666463145113),
        QPointF(440.5623611292983, 107.35243113317043),
        QPointF(442.97340247797115, 109.89783630678178),
        QPointF(445.3566545645407, 112.48243074024779),
        QPointF(447.7120365991257, 115.10576502153111),
        QPointF(450.03946779184486, 117.7673897385943),
        QPointF(452.33886735281703, 120.46685547939998),
        QPointF(454.61015449216075, 123.20371283191074),
        QPointF(456.853248419995, 125.97751238408917),
        QPointF(459.0680683464384, 128.7878047238979),
        QPointF(461.2545334816097, 131.63414043929953),
        QPointF(463.41256303562767, 134.51607011825666),
        QPointF(465.542076218611, 137.4331443487319),
        QPointF(467.64299224067844, 140.3849137186878),
        QPointF(469.7152303119488, 143.3709288160871),
        QPointF(471.7587096425408, 146.39074022889218),
        QPointF(473.7733494425731, 149.44389854506585),
        QPointF(475.7590689221645, 152.52995435257066),
        QPointF(477.7157872914338, 155.64845823936915),
        QPointF(479.6434237604996, 158.79896079342402),
        QPointF(481.54189753948083, 161.98101260269775),
        QPointF(483.4111278384961, 165.19416425515303),
        QPointF(485.2510338676642, 168.43796633875246),
        QPointF(487.06153483710386, 171.71196944145862),
        QPointF(488.8425499569338, 175.0157241512341),
        QPointF(490.5939984372728, 178.34878105604156),
        QPointF(492.3157994882395, 181.71069074384357),
        QPointF(494.0078723199529, 185.10100380260266),
        QPointF(495.6701361425315, 188.51927082028155),
        QPointF(497.302510166094, 191.9650423848428),
        QPointF(498.9049136007594, 195.43786908424903),
        QPointF(500.4772656566462, 198.93730150646275),
        QPointF(502.0194855438733, 202.46289023944666),
        QPointF(503.5314924725593, 206.01418587116336),
        QPointF(505.01320565282316, 209.59073898957544),
        QPointF(507.18260988245254, 214.99892662134033),
        QPointF(509.9633042738714, 222.29818483388794),
        QPointF(512.6213694537385, 229.69038357654387),
        QPointF(515.1561591030039, 237.17192755300903),
        QPointF(517.5670269026173, 244.73922146698413),
        QPointF(519.8533265335291, 252.38867002217006),
        QPointF(522.0144116766887, 260.1166779222676),
        QPointF(524.0496360130464, 267.91964987097765),
        QPointF(525.958353223552, 275.79399057200106),
        QPointF(527.7399169891553, 283.7361047290385),
        QPointF(529.3936809908065, 291.742397045791),
        QPointF(530.9189989094555, 299.80927222595915),
        QPointF(532.315224426052, 307.9331349732439),
        QPointF(533.5817112215462, 316.1103899913461),
        QPointF(534.717812976888, 324.3374419839666),
        QPointF(535.7228833730273, 332.61069565480614),
        QPointF(536.596276090914, 340.9265557075656),
        QPointF(537.3373448114982, 349.28142684594576),
        QPointF(537.9454432157295, 357.6717137736474),
        QPointF(538.4199249845582, 366.09382119437157),
        QPointF(538.7601437989341, 374.5441538118188),
        QPointF(538.9654533398071, 383.0191163296901),
        QPointF(539.011653543307, 387.2655118110236)
    };

    QPointF basePoint(283.4328705040981, 238.94051381477158);

    // See file <root>/src/app/share/collection/bugs/Issue_#773.val
    QTest::newRow("Angle 0 degree") << basePoint
                                    << 0.0
                                    << curvePoints
                                    << QPointF(515.7196136713028, 238.94051381477158);

    QTest::newRow("Angle 180 degree") << basePoint
                                      << 180.0
                                      << curvePoints
                                      << QPointF(77.88857126235962, 238.94051381477158);

    QTest::newRow("Ignore base point") << QPointF(77.88857126235962, 238.94051381477158)
                                       << 25.625900000000001
                                       << curvePoints
                                       << QPointF(409.3673746585846, 79.93820315599442);

}

//---------------------------------------------------------------------------------------------------------------------
void TST_FindPoint::TestCurveIntersectAxis()
{
    QFETCH(QPointF, basePoint);
    QFETCH(qreal, angle);
    QFETCH(QVector<QPointF>, curvePoints);
    QFETCH(QPointF, result);

    const QPointF resultPoint = VToolCurveIntersectAxis::FindPoint(basePoint, angle, curvePoints);

    QCOMPARE(result, resultPoint);
}
