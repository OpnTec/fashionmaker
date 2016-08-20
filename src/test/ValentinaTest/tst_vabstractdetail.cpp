/************************************************************************
 **
 **  @file   tst_vabstractdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 4, 2015
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

#include "tst_vabstractdetail.h"
#include "../vlayout/vabstractdetail.h"

#include <QPointF>
#include <QVector>

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VAbstractDetail::TST_VAbstractDetail(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::EquidistantRemoveLoop_data()
{
    QTest::addColumn<QVector<QPointF>>("points");
    QTest::addColumn<int>("eqv");
    QTest::addColumn<qreal>("width");
    QTest::addColumn<QVector<QPointF>>("ekvOrig");

    QTest::newRow("Case1") << InputPointsCase1()
                           << static_cast<int>(EquidistantType::CloseEquidistant)
                           << 75.5906 // seam allowance width
                           << OutputPointsCase1();

    QTest::newRow("Case2") << InputPointsCase2()
                           << static_cast<int>(EquidistantType::CloseEquidistant)
                           << 37.7953 // seam allowance width
                           << OutputPointsCase2();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_VAbstractDetail::EquidistantRemoveLoop() const
{
    // These are two real cases where equdistant has loop.
    // See issue #298. Segmented Curve isn't selected in Seam Allowance tool.
    // https://bitbucket.org/dismine/valentina/issue/298/segmented-curve-isnt-selected-in-seam
    // Code should clean loops in path.
    QFETCH(QVector<QPointF>, points);
    QFETCH(int, eqv);
    QFETCH(qreal, width);
    QFETCH(QVector<QPointF>, ekvOrig);

    const QVector<QPointF> ekv = VAbstractDetail::Equidistant(points, static_cast<EquidistantType>(eqv), width);

    // Begin comparison
    Comparison(ekv, ekvOrig);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::SumTrapezoids() const
{
    // Case3 checks that the method 'SumTrapezoids' returns negative value for three clockwise allocated points
    // Case4 checks that the method 'SumTrapezoids' returns positive value for three counterclock-wise allocated points
    // Case5 checks that the method 'SumTrapezoids' returns 0 for one point
    Case3();
    Case4();
    Case5();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::PathRemoveLoop_data() const
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
    path << QPointF(10, 20);
    path << QPointF(10, 10);
    path << QPointF(20, 20);
    path << QPointF(20, 10);

    QVector<QPointF> res;
    res << QPointF(20, 10);
    res << QPointF(15, 15);
    res << QPointF(20, 20);
    res << QPointF(20, 10);
    QTest::newRow("One loop, closed a path (four unique points)") << path << res;

    path.removeLast();
    res.removeLast();
    QTest::newRow("One loop, unclosed a path (four unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 20);
    path << QPointF(0, 10);
    path << QPointF(0, 20);
    path << QPointF(10, 10);
    path << QPointF(20, 20);
    path << QPointF(20, 10);

    res.clear();
    res << QPointF(20, 10);
    res << QPointF(15, 15);
    res << QPointF(20, 20);
    res << QPointF(20, 10);
    QTest::newRow("Two loops, closed a path (six unique points)") << path << res;

    path.removeLast();
    res.removeLast();
    QTest::newRow("Two loops, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 20);
    path << QPointF(0, 20);
    path << QPointF(0, 10);
    path << QPointF(10, 10);
    path << QPointF(20, 20);
    path << QPointF(20, 10);

    res.clear();
    res << QPointF(20, 10);
    res << QPointF(15, 15);
    res << QPointF(20, 20);
    res << QPointF(20, 10);
    QTest::newRow("One loop, the first loop, closed a path (six unique points)") << path << res;

    path.removeLast();
    res.removeLast();
    QTest::newRow("One loop, the first loop, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 10);
    path << QPointF(0, 20);
    path << QPointF(0, 10);
    path << QPointF(10, 20);
    path << QPointF(20, 20);
    path << QPointF(20, 10);

    res.clear();
    res << QPointF(20, 10);
    res << QPointF(10, 10);
    res << QPointF(5, 15);
    res << QPointF(10, 20);
    res << QPointF(20, 20);
    res << QPointF(20, 10);
    QTest::newRow("One loop, the second loop, closed a path (six unique points)") << path << res;

    path.removeLast();
    res.removeLast();
    QTest::newRow("One loop, the second loop, unclosed a path (six unique points)") << path << res;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 10);
    path << QPointF(20, 15);
    path << QPointF(10, 20);
    path << QPointF(20, 20);
    path << QPointF(20, 10);
    QTest::newRow("Correct closed a path, point on line (four unique points)") << path << path;

    path.removeLast();
    QTest::newRow("Corect unclosed a path, point on line (four unique points)") << path << path;

    path.clear();
    path << QPointF(20, 10);
    path << QPointF(10, 10);
    path << QPointF(0, 10);
    path << QPointF(10, 15);
    path << QPointF(0, 20);
    path << QPointF(10, 20);
    path << QPointF(20, 20);
    path << QPointF(10, 15);
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
    res << QPointF(1001.3385826771654, 1822.488188976378);
    res << QPointF(1001.3385826771654, 2680.44094488189);
    res << QPointF(-22.11646613738226, 2680.44094488189);
    res << QPointF(100.96979100571033, 1797.6153764073072);

    // See the file "collection/bugs/Issue_#493.val"
    QTest::newRow("Test case issue #493") << path << res;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::PathRemoveLoop() const
{
    QFETCH(QVector<QPointF>, path);
    QFETCH(QVector<QPointF>, expect);

    QVector<QPointF> res = VAbstractDetail::CheckLoops(path);
    Comparison(res, expect);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::BrokenDetailEquidistant() const
{
    // For more details see the file "collection/bugs/GAVAUDAN Laure - corsage - figure 4.val".
    // We will test only one detail. The second require too accurate data that we cannot get from debuger.
    // The test check an open equdistant of correct detail.
    QVector<QPointF> points;// Input points.
    points.append(QPointF(787.5835464566929, 1701.3138897637796));
    points.append(QPointF(938.7646488188976, 1701.3138897637796));
    points.append(QPointF(910.0209091217698, 1792.3369853889722));
    points.append(QPointF(878.5244039283091, 1905.2261617043234));
    points.append(QPointF(863.9159293830619, 1968.2534932384856));
    points.append(QPointF(852.8936778444679, 1919.6965437838999));
    points.append(QPointF(819.0677656132684, 1798.6758641921479));
    points.append(QPointF(787.5835464566929, 1701.3138897637796));
    points.append(QPointF(797.0323653543306, 2608.4005039370077));

    const EquidistantType eqv = EquidistantType::OpenEquidistant; // Open path
    const qreal width = 37.795275590551185; // seam allowance width

    const QVector<QPointF> ekv = VAbstractDetail::Equidistant(points, eqv, width);// Take result

    QVector<QPointF> ekvOrig;
    ekvOrig.append(QPointF(787.1898456692913, 1663.5186141732283));
    ekvOrig.append(QPointF(989.3402780205395, 1663.5186141732283));
    ekvOrig.append(QPointF(915.0896841461371, 1914.8556948468406));
    ekvOrig.append(QPointF(894.0594908835558, 2005.5891221381557));
    ekvOrig.append(QPointF(834.9361130712198, 2006.4994568398874));
    ekvOrig.append(QPointF(788.9513624221928, 1832.6242224517807));

    // Begin comparison
    Comparison(ekv, ekvOrig);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::TestCorrectEquidistantPoints_data()
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
void TST_VAbstractDetail::TestCorrectEquidistantPoints() const
{
    QFETCH(QVector<QPointF>, before);
    QFETCH(QVector<QPointF>, expect);

    QVector<QPointF> after = VAbstractDetail::CorrectEquidistantPoints(before);
    Comparison(after, expect);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::Case3() const
{
    const QVector<QPointF> points = InputPointsCase3(); // Input points.

    const qreal result = VAbstractDetail::SumTrapezoids(points);
    QVERIFY(result < 0);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::Case4() const
{
    const QVector<QPointF> points = InputPointsCase4(); // Input points.

    const qreal result = VAbstractDetail::SumTrapezoids(points);
    QVERIFY(result > 0);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::Case5() const
{
    const QVector<QPointF> points = InputPointsCase5(); // Input points.

    const qreal result = VAbstractDetail::SumTrapezoids(points);
    QVERIFY(qFuzzyIsNull(result));
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsCase1() const
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
QVector<QPointF> TST_VAbstractDetail::OutputPointsCase1() const
{
    QVector<QPointF> points;

    points += QPointF(-52.3724798442221, -35.5907);
    points += QPointF(487.7117748779425, -35.5907);
    points += QPointF(493.3486932130227, 473.81998224542247);
    points += QPointF(384.7625023736152, 506.7228642416019);
    points += QPointF(326.77984549201204, 417.71265429523794);
    points += QPointF(280.4343843787976, 340.220616520921);
    points += QPointF(269.00223298277206, 346.06212334710335);
    points += QPointF(239.6571136552229, 350.73379418002804);
    points += QPointF(205.89523544191223, 345.8623563310819);
    points += QPointF(173.89027296099863, 332.6512960877336);
    points += QPointF(145.31523414712046, 315.34576260355936);
    points += QPointF(117.9921341644787, 294.6948297428524);
    points += QPointF(91.25349438209683, 270.93768759082707);
    points += QPointF(65.22541125346564, 244.39379519957222);
    points += QPointF(40.34791988062461, 215.4153238967542);
    points += QPointF(17.205314383747528, 184.31949780808853);
    points += QPointF(-3.534416178847685, 151.30687894362717);
    points += QPointF(-21.090087790322336, 116.33389217738734);
    points += QPointF(-33.795079640648055, 80.66898804409438);
    points += QPointF(-38.441724866417594, 60.24852451858777);
    points += QPointF(-52.3724798442221, -35.5907);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsCase2() const
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
QVector<QPointF> TST_VAbstractDetail::OutputPointsCase2() const
{
    QVector<QPointF> points;

    points += QPointF(-2.7952999999999975, 5.7719918429762656);
    points += QPointF(65.32544836315374, -0.992801551243895);
    points += QPointF(75.43676015393824, 49.41505784459415);
    points += QPointF(83.37769389516122, 71.70650391130641);
    points += QPointF(95.36495808942361, 95.58656052818594);
    points += QPointF(110.83634280812595, 120.21325109595534);
    points += QPointF(128.9510900596877, 144.55333805162292);
    points += QPointF(148.8122943732712, 167.68860240021857);
    points += QPointF(169.48075280895182, 188.76665620458672);
    points += QPointF(189.9439329177606, 206.94808512857375);
    points += QPointF(209.03488292644147, 221.3771186982216);
    points += QPointF(215.73126220022726, 225.4325608097815);
    points += QPointF(215.09342206269645, 222.63086681417994);
    points += QPointF(205.6867557893074, 194.5606373284329);
    points += QPointF(193.90240551299544, 154.91725528228594);
    points += QPointF(189.00923093023508, 130.4332749760628);
    points += QPointF(191.70730467606634, 97.53824783614445);
    points += QPointF(229.19819583315143, 77.54897644999551);
    points += QPointF(256.3345313737502, 91.70119126633715);
    points += QPointF(270.9082046450185, 107.89162042078927);
    points += QPointF(294.3626891097502, 142.42241251279827);
    points += QPointF(355.51936324849004, 244.86019492195868);
    points += QPointF(422.97357725399365, 357.6471728523805);
    points += QPointF(486.8597146913536, 455.7199210117685);
    points += QPointF(-2.795300000000013, 455.7506738094777);
    points += QPointF(-2.7952999999999975, 5.7719918429762656);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsCase3() const
{
    QVector<QPointF> points;

    points += QPointF(35, 35);
    points += QPointF(50, 50);
    points += QPointF(15, 50);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsCase4() const
{
    QVector<QPointF> points;

    points += QPointF(15, 15);
    points += QPointF(15, 50);
    points += QPointF(50, 50);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VAbstractDetail::InputPointsCase5() const
{
    QVector<QPointF> points;

    points += QPointF(35, 35);

    return points;
}
