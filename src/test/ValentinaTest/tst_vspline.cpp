/************************************************************************
 **
 **  @file   tst_vspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "tst_vspline.h"
#include "../vgeometry/vspline.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VSpline::TST_VSpline(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints()
{
    // Input data taken from real case
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);

    QPointF begin(957.69885233364062, 943.84482037833141);
    QPointF end(681.33729132409951, 1815.7969526662778);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, false);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(957.698852334, 943.844820378));
    origPoints.append(QPointF(957.84724553, 975.602655091));
    origPoints.append(QPointF(957.728801633, 1035.13055094));
    origPoints.append(QPointF(956.808773853, 1094.37046246));
    origPoints.append(QPointF(955.328435862, 1138.53744981));
    origPoints.append(QPointF(953.925479436, 1167.84862152));
    origPoints.append(QPointF(952.117074266, 1197.03211968));
    origPoints.append(QPointF(949.857653068, 1226.07557151));
    origPoints.append(QPointF(947.101648559, 1254.96660424));
    origPoints.append(QPointF(943.803493456, 1283.6928451));
    origPoints.append(QPointF(939.917620476, 1312.24192132));
    origPoints.append(QPointF(935.398462336, 1340.60146013));
    origPoints.append(QPointF(930.200451752, 1368.75908875));
    origPoints.append(QPointF(924.278021443, 1396.70243441));
    origPoints.append(QPointF(917.585604124, 1424.41912434));
    origPoints.append(QPointF(910.077632512, 1451.89678578));
    origPoints.append(QPointF(901.708539325, 1479.12304594));
    origPoints.append(QPointF(892.432757279, 1506.08553207));
    origPoints.append(QPointF(882.204719092, 1532.77187137));
    origPoints.append(QPointF(870.978857479, 1559.1696911));
    origPoints.append(QPointF(858.709605159, 1585.26661847));
    origPoints.append(QPointF(845.351394848, 1611.05028071));
    origPoints.append(QPointF(830.858659262, 1636.50830504));
    origPoints.append(QPointF(815.18583112, 1661.62831871));
    origPoints.append(QPointF(798.287343137, 1686.39794894));
    origPoints.append(QPointF(780.117628031, 1710.80482295));
    origPoints.append(QPointF(760.631118518, 1734.83656798));
    origPoints.append(QPointF(739.782247316, 1758.48081125));
    origPoints.append(QPointF(717.525447141, 1781.72518));
    origPoints.append(QPointF(693.81515071, 1804.55730144));
    origPoints.append(QPointF(681.337291324, 1815.79695267));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints_issue412()
{
    // Input data taken from real case
    // See issue #412 https://bitbucket.org/dismine/valentina/issues/412/error-in-detail-using-segment-a-simple
    VPointF p1(869.11748031496063, -61.117228346456692, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(491.16472440944887, 316.83552755905515, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 270, 0, 1, 1, 1);

    QPointF begin(869.11748031496063, -61.117228346456692);
    QPointF end(758.41768107838425, 206.13572832247544);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, false);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(869.117480315, -61.1172283465));
    origPoints.append(QPointF(869.034855699, -51.3519540199));
    origPoints.append(QPointF(868.055234051, -32.0262592346));
    origPoints.append(QPointF(866.119738229, -12.967761819));
    origPoints.append(QPointF(863.252115711, 5.79979075097));
    origPoints.append(QPointF(859.476113972, 24.2526509993));
    origPoints.append(QPointF(854.815480488, 42.36707145));
    origPoints.append(QPointF(849.293962735, 60.1193046272));
    origPoints.append(QPointF(842.93530819, 77.4856030548));
    origPoints.append(QPointF(835.763264327, 94.4422192569));
    origPoints.append(QPointF(827.801578624, 110.965405758));
    origPoints.append(QPointF(819.073998555, 127.031415081));
    origPoints.append(QPointF(809.604271597, 142.616499751));
    origPoints.append(QPointF(799.416145227, 157.696912291));
    origPoints.append(QPointF(788.533366919, 172.248905226));
    origPoints.append(QPointF(776.97968415, 186.24873108));
    origPoints.append(QPointF(764.778844396, 199.672642377));
    origPoints.append(QPointF(758.417681078, 206.135728322));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints_TestPuzzle()
{
    // Input data taken from real case
    // See the file <root>/src/app/share/collection/TestPuzzle.val
    // Test issue with method IsPointOnLineSegment.
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);

    QPointF begin(1168.85828031, 39.9998740157);
    QPointF end(957.69883966, 943.844812978);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, false);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(1168.85828031, 39.9998740157));
    origPoints.append(QPointF(1158.40802126, 52.3497961203));
    origPoints.append(QPointF(1138.64676795, 77.3443304348));
    origPoints.append(QPointF(1120.21162184, 102.681357357));
    origPoints.append(QPointF(1103.0570157, 128.348504087));
    origPoints.append(QPointF(1087.13738226, 154.333397827));
    origPoints.append(QPointF(1072.40715426, 180.623665779));
    origPoints.append(QPointF(1058.82076446, 207.206935142));
    origPoints.append(QPointF(1046.33264559, 234.070833119));
    origPoints.append(QPointF(1034.8972304, 261.20298691));
    origPoints.append(QPointF(1024.46895163, 288.591023717));
    origPoints.append(QPointF(1015.00224202, 316.22257074));
    origPoints.append(QPointF(1006.45153433, 344.085255182));
    origPoints.append(QPointF(998.771261285, 372.166704242));
    origPoints.append(QPointF(991.915855642, 400.454545123));
    origPoints.append(QPointF(985.83975014, 428.936405026));
    origPoints.append(QPointF(980.497377524, 457.599911151));
    origPoints.append(QPointF(975.843170537, 486.432690699));
    origPoints.append(QPointF(971.831561923, 515.422370873));
    origPoints.append(QPointF(968.416984426, 544.556578873));
    origPoints.append(QPointF(965.55387079, 573.8229419));
    origPoints.append(QPointF(963.19665376, 603.209087156));
    origPoints.append(QPointF(961.299766078, 632.702641841));
    origPoints.append(QPointF(959.17457231, 677.107741373));
    origPoints.append(QPointF(957.595965967, 736.590833735));
    origPoints.append(QPointF(957.038466409, 796.280853437));
    origPoints.append(QPointF(957.305645106, 885.992413851));
    origPoints.append(QPointF(957.69883966, 943.844812978));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints_NullSegment()
{
    // Test check case where a detail contains one spline and one point on a spline.

    const VPointF p1(146.3718263928647, 6.4192815800656247, "A1", 5.0000125984251973, 9.9999874015748045);
    const QPointF p2(-231.44352755905513, -3.7711067716535434);
    const QPointF p3(-194.55587416421278, 355.17175586626462);
    const VPointF p4(182.31062201967654, 383.80617135964712, "A2", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p2, p3, p4, 1);

    const QPointF begin	(146.3718263928647, 6.419281580065625);
    const QPointF end	(146.3718263928647, 6.419281580065625);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, true);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(182.31062201967654, 383.8061713596471));
    origPoints.append(QPointF(173.5193148389493, 383.09473139115664));
    origPoints.append(QPointF(156.35256886351988, 381.27664737553243));
    origPoints.append(QPointF(139.68640422167817, 378.9987265205365));
    origPoints.append(QPointF(123.52253121382145, 376.2769777761686));
    origPoints.append(QPointF(107.862660140347, 373.127410092429));
    origPoints.append(QPointF(92.70850130165212, 369.5660324193175));
    origPoints.append(QPointF(78.06176499813408, 365.6088537068342));
    origPoints.append(QPointF(63.924161530190176, 361.2718829049793));
    origPoints.append(QPointF(50.29740119821767, 356.5711289637526));
    origPoints.append(QPointF(37.18319430261387, 351.5226008331541));
    origPoints.append(QPointF(24.583251143776067, 346.142307463184));
    origPoints.append(QPointF(12.499282022101543, 340.4462578038422));
    origPoints.append(QPointF(0.9329972379875793, 334.4504608051287));
    origPoints.append(QPointF(-10.113892908168534, 328.17092541704363));
    origPoints.append(QPointF(-20.63967811596951, 321.6236605895869));
    origPoints.append(QPointF(-30.64264808501806, 314.8246752727586));
    origPoints.append(QPointF(-40.121092514916896, 307.78997841655865));
    origPoints.append(QPointF(-49.07330110526874, 300.5355789709872));
    origPoints.append(QPointF(-57.49756355567631, 293.0774858860442));
    origPoints.append(QPointF(-65.39216956574231, 285.4317081117297));
    origPoints.append(QPointF(-72.75540883506945, 277.61425459804366));
    origPoints.append(QPointF(-79.58557106326043, 269.64113429498616));
    origPoints.append(QPointF(-85.88094594991799, 261.5283561525572));
    origPoints.append(QPointF(-91.63982319464486, 253.29192912075672));
    origPoints.append(QPointF(-96.86049249704371, 244.94786214958486));
    origPoints.append(QPointF(-101.54124355671726, 236.51216418904164));
    origPoints.append(QPointF(-105.68036607326823, 228.000844189127));
    origPoints.append(QPointF(-109.27614974629935, 219.42991109984098));
    origPoints.append(QPointF(-112.32688427541332, 210.81537387118362));
    origPoints.append(QPointF(-114.83085936021286, 202.1732414531549));
    origPoints.append(QPointF(-116.78636470030067, 193.51952279575488));
    origPoints.append(QPointF(-118.19168999527946, 184.8702268489836));
    origPoints.append(QPointF(-119.04512494475199, 176.24136256284103));
    origPoints.append(QPointF(-119.34495924832092, 167.64893888732718));
    origPoints.append(QPointF(-119.08948260558898, 159.10896477244216));
    origPoints.append(QPointF(-118.27698471615886, 150.63744916818587));
    origPoints.append(QPointF(-116.90575527963333, 142.2504010245584));
    origPoints.append(QPointF(-114.97408399561508, 133.96382929155976));
    origPoints.append(QPointF(-112.48026056370679, 125.79374291918995));
    origPoints.append(QPointF(-109.42257468351121, 117.756150857449));
    origPoints.append(QPointF(-105.79931605463103, 109.86706205633692));
    origPoints.append(QPointF(-101.608774376669, 102.14248546585374));
    origPoints.append(QPointF(-96.8492393492278, 94.59843003599951));
    origPoints.append(QPointF(-91.51900067191015, 87.25090471677419));
    origPoints.append(QPointF(-85.61634804431877, 80.11591845817783));
    origPoints.append(QPointF(-79.13957116605636, 73.20948021021047));
    origPoints.append(QPointF(-72.08695973672565, 66.54759892287208));
    origPoints.append(QPointF(-64.45680345592936, 60.14628354616272));
    origPoints.append(QPointF(-56.247392023270166, 54.0215430300824));
    origPoints.append(QPointF(-47.45701513835082, 48.189386324631116));
    origPoints.append(QPointF(-38.083962500774014, 42.66582237980891));
    origPoints.append(QPointF(-28.126523810142473, 37.4668601456158));
    origPoints.append(QPointF(-17.582988766058897, 32.608508572051804));
    origPoints.append(QPointF(-6.451647068126014, 28.106776609116928));
    origPoints.append(QPointF(5.269211584053471, 23.97767320681121));
    origPoints.append(QPointF(17.581297490876842, 20.23720731513466));
    origPoints.append(QPointF(30.486320952741384, 16.9013878840873));
    origPoints.append(QPointF(43.98599227004439, 13.986223863669146));
    origPoints.append(QPointF(58.082021743183134, 11.507724203880219));
    origPoints.append(QPointF(72.77611967255493, 9.481897854720536));
    origPoints.append(QPointF(88.069996358557, 7.92475376619012));
    origPoints.append(QPointF(103.96536210158672, 6.852300888288988));
    origPoints.append(QPointF(120.4639272020413, 6.280548171017158));
    origPoints.append(QPointF(137.5674019603181, 6.225504564374653));
    origPoints.append(QPointF(146.3718263928647, 6.419281580065625));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::GetSegmentPoints_RotateTool()
{
    // Input data taken from real case
    // See the file <root>/src/app/share/collection/bugs/IsPointOnLineSegment_RotateTool_issue.val
    // Test issue with method IsPointOnLineSegment.

    const VPointF p1(155.93961723681397, -42.472964170961042, "A", 5.0000125984251973, 9.9999874015748045);
    const VPointF p4(237.32422843061005, 485.80074940371367, "A2", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 231.11199999999994, "231.112", 145.33899999999997, "145.339", 207.44768503937021, "5.48872",
                337.50916535433066, "8.92993");

    const QPointF begin(237.32422843061005, 485.80074940371367);
    const QPointF end  (46.623829088412336, 167.78988631718659);

    QVector<QPointF> points;
    points << spl.GetSegmentPoints(begin, end, true);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(237.32422843061005, 485.80074940371367));
    origPoints.append(QPointF(224.47894722830574, 476.8115274500917));
    origPoints.append(QPointF(200.4405599713662, 458.9162132315404));
    origPoints.append(QPointF(178.3387458840754, 441.12004274890694));
    origPoints.append(QPointF(158.12235273581754, 423.422336123843));
    origPoints.append(QPointF(139.74022829597683, 405.8224134780004));
    origPoints.append(QPointF(123.1412203339375, 388.3195949330309));
    origPoints.append(QPointF(108.27417661908376, 370.91320061058616));
    origPoints.append(QPointF(95.08794492079983, 353.60255063231807));
    origPoints.append(QPointF(83.5313730084699, 336.38696511987837));
    origPoints.append(QPointF(73.55330865147822, 319.2657641949186));
    origPoints.append(QPointF(65.10259961920897, 302.2382679790908));
    origPoints.append(QPointF(58.12809368104641, 285.3037965940465));
    origPoints.append(QPointF(52.57863860637471, 268.4616701614375));
    origPoints.append(QPointF(48.40308216457812, 251.71120880291573));
    origPoints.append(QPointF(45.55027212504085, 235.05173264013274));
    origPoints.append(QPointF(43.9690562571471, 218.48256179474038));
    origPoints.append(QPointF(43.6082823302811, 202.00301638839034));
    origPoints.append(QPointF(44.416798113827056, 185.61241654273442));
    origPoints.append(QPointF(46.34345137716919, 169.31008237942433));
    origPoints.append(QPointF(46.623829088412336, 167.78988631718659));

    // Begin comparison
    Comparison(points, origPoints);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::CompareThreeWays()
{
    // Input data taken from real case
    // See the file <root>/src/app/share/collection/TestPuzzle.val
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl1(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);
    VSpline spl2(spl1.GetP1(), spl1.GetP2(), spl1.GetP3(), spl1.GetP4(), 1);
    VSpline spl3(spl1.GetP1(), spl1.GetP4(), spl1.GetStartAngle(), "", spl2.GetEndAngle(), "", spl2.GetC1Length(), "",
                 spl2.GetC2Length(), "", 1);

    QWARN("Comparing first and second splines.");
    CompareSplines(spl1, spl2);

    QWARN("Comparing second and third splines.");
    CompareSplines(spl2, spl3);

    QWARN("Comparing third and first splines.");
    CompareSplines(spl3, spl1);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::TestParametrT()
{
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);

    const qreal halfLength = spl.GetLength()/2.0;
    const qreal resLength = spl.LengthT(spl.GetParmT(halfLength));

    QVERIFY(qAbs(halfLength - resLength) < UnitConvertor(0.5, Unit::Mm, Unit::Px));
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::TestLengthByPoint_data()
{
    VPointF p1(1168.8582803149607, 39.999874015748034, "p1", 5.0000125984251973, 9.9999874015748045);
    VPointF p4(681.33729132409951, 1815.7969526662778, "p4", 5.0000125984251973, 9.9999874015748045);

    VSpline spl(p1, p4, 229.381, 41.6325, 0.96294100000000005, 1.00054, 1);

    QTest::addColumn<VSpline>("spl");
    QTest::addColumn<QPointF>("point");
    QTest::addColumn<qreal>("length");

    const qreal length = spl.GetLength();
    const qreal testLength = length*(2.0/3.0);
    VSpline spl1, spl2;
    const QPointF p = spl.CutSpline(testLength, spl1, spl2);

    QTest::newRow("Point on spline") << spl << p << testLength;
    QTest::newRow("Wrong point") << spl << QPointF(-10000, -10000) << -1.0;
    QTest::newRow("First point") << spl << p1.toQPointF() << 0.0;
    QTest::newRow("Last point") << spl << p4.toQPointF() << length;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::TestLengthByPoint()
{
    QFETCH(VSpline, spl);
    QFETCH(QPointF, point);
    QFETCH(qreal, length);

    const qreal resLength = spl.GetLengthByPoint(point);

    QVERIFY(qAbs(resLength - length) < ToPixel(0.5, Unit::Mm));
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VSpline::CompareSplines(const VSpline &spl1, const VSpline &spl2) const
{
    QCOMPARE(spl1.GetP1().toQPointF().toPoint(), spl2.GetP1().toQPointF().toPoint());
    QCOMPARE(spl1.GetP2().toQPointF().toPoint(), spl2.GetP2().toQPointF().toPoint());
    QCOMPARE(spl1.GetP3().toQPointF().toPoint(), spl2.GetP3().toQPointF().toPoint());
    QCOMPARE(spl1.GetP4().toQPointF().toPoint(), spl2.GetP4().toQPointF().toPoint());

    QCOMPARE(spl1.GetStartAngle(), spl2.GetStartAngle());
    QCOMPARE(spl1.GetEndAngle(), spl2.GetEndAngle());

    QCOMPARE(spl1.GetC1Length(), spl2.GetC1Length());
    QCOMPARE(spl1.GetC2Length(), spl2.GetC2Length());

    QCOMPARE(spl1.GetLength(), spl2.GetLength());

    QCOMPARE(spl1.GetKasm1(), spl2.GetKasm1());
    QCOMPARE(spl1.GetKasm2(), spl2.GetKasm2());

    QCOMPARE(spl1.GetKcurve(), spl2.GetKcurve());

    // Compare points
    Comparison(spl1.GetPoints(), spl2.GetPoints());
}
