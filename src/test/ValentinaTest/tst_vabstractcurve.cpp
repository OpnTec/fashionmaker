/************************************************************************
 **
 **  @file   tst_vabstractcurve.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 3, 2016
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

#include "tst_vabstractcurve.h"
#include "../vgeometry/vabstractcurve.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VAbstractCurve::TST_VAbstractCurve(QObject *parent)
    : AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractCurve::IsPointOnCurve_data() const
{
    QTest::addColumn<QVector<QPointF>>("points");
    QTest::addColumn<QPointF>("point");
    QTest::addColumn<bool>("expectedResult");

    QVector<QPointF> points;
    points << QPointF(714.5704733515146, 229.44783247230293);
    points << QPointF(713.2432059361518, 236.799577781511);
    points << QPointF(709.8892587314249, 252.1363394689535);
    points << QPointF(703.6056072956214, 276.4001729111941);
    points << QPointF(687.7559494358588, 329.1513838344773);
    points << QPointF(670.3756426535148, 387.2408887452223);
    points << QPointF(662.3317449567428, 417.643760273044);
    points << QPointF(657.4471488294345, 438.31881594794856);
    points << QPointF(653.1084257285696, 459.2974181766972);
    points << QPointF(649.4426552757304, 480.5376973511262);
    points << QPointF(646.5769170924987, 501.9977838630714);
    points << QPointF(644.6382908004568, 523.6358081043691);
    points << QPointF(644.1029291338583, 534.5132598425197);
    points << QPointF(644.1029291338583, 534.5132598425197);
    points << QPointF(643.4592698551749, 551.9888717674471);
    points << QPointF(642.9134698671897, 584.1776423714557);
    points << QPointF(643.1914832622404, 613.2382010061506);
    points << QPointF(644.2199668178571, 639.3780275889782);
    points << QPointF(645.9255773115714, 662.8046020373845);
    points << QPointF(648.2349715209137, 683.7254042688159);
    points << QPointF(651.0748062234152, 702.3479142007185);
    points << QPointF(654.3717381966065, 718.8796117505387);
    points << QPointF(658.0524242180187, 733.5279768357226);
    points << QPointF(662.0435210651824, 746.5004893737165);
    points << QPointF(666.2716855156286, 758.0046292819667);
    points << QPointF(670.6635743468883, 768.2478764779191);
    points << QPointF(677.400406718071, 781.7952098705392);
    points << QPointF(686.2864119958404, 797.2061069980141);
    points << QPointF(690.4766621750516, 804.2970071162871);
    points << QPointF(690.4766621750516, 804.2970071162871);
    points << QPointF(692.7921674626707, 808.1521079045636);
    points << QPointF(697.7183992280718, 815.2245015705212);
    points << QPointF(702.9886930214004, 821.5595818277402);
    points << QPointF(708.5917117312482, 827.1885221028615);
    points << QPointF(714.5161182462067, 832.1424958225257);
    points << QPointF(720.750575454867, 836.4526764133732);
    points << QPointF(727.2837462458206, 840.1502373020446);
    points << QPointF(734.1042935076591, 843.2663519151808);
    points << QPointF(741.200880128974, 845.8321936794223);
    points << QPointF(748.5621689983566, 847.8789360214096);
    points << QPointF(756.1768230043983, 849.4377523677833);
    points << QPointF(764.0335050356908, 850.5398161451842);
    points << QPointF(772.1208779808252, 851.2163007802526);
    points << QPointF(780.4276047283932, 851.4983796996295);
    points << QPointF(793.250306920113, 851.2897382511853);
    points << QPointF(802.0871811023624, 850.6707401574804);

    QPointF point(652.5169278885382, 462.6106569368444);

    // See file collection/bug/pointOnCurve.val
    QTest::newRow("Point on curve") << points << point << true;

    points.clear();
    points << QPointF(9448.818897637795, 0.0);
    points << QPointF(9143.208661417324, -12.224409448818896);
    points << QPointF(8762.026328740158, -26.973425196850393);
    points << QPointF(8540.29281496063, -34.94586614173228);
    points << QPointF(8340.981791338583, -41.32381889763779);
    points << QPointF(8159.110482283465, -46.107283464566926);
    points << QPointF(7989.696112204724, -49.29625984251968);
    points << QPointF(7867.929533710629, -50.59178149606299);
    points << QPointF(7787.893700787402, -50.99040354330708);
    points << QPointF(7708.169291338582, -50.99040354330708);
    points << QPointF(7628.133458415356, -50.59178149606299);
    points << QPointF(7506.36687992126, -49.29625984251968);
    points << QPointF(7336.952509842519, -46.107283464566926);
    points << QPointF(7155.081200787401, -41.32381889763779);
    points << QPointF(6955.770177165354, -34.94586614173228);
    points << QPointF(6734.036663385827, -26.973425196850393);
    points << QPointF(6352.854330708662, -12.224409448818896);
    points << QPointF(6047.244094488189, 0.0);

    point = QPointF(7748.031496062632, -51.02362204724411);

    // See file <root>/src/app/share/collection/bugs/Issue_#626.val
    QTest::newRow("Issue #626") << points << point << true;

    points.clear();
    points << QPointF(-11.267867716535433, -621.9401574803151);
    points << QPointF(28.929472497969787, -621.860784520479);
    points << QPointF(111.02416803427747, -621.0204611542031);
    points << QPointF(192.81661180303666, -619.5641187251658);
    points << QPointF(271.70428169724175, -617.7160615368814);
    points << QPointF(380.0727033074438, -614.6790454948614);
    points << QPointF(489.1891345644603, -611.3242942049818);
    points << QPointF(520.2546519685039, -610.6015748031497);
    points << QPointF(520.2546519685039, -610.6015748031497);
    points << QPointF(533.2501095340014, -610.4104061683322);
    points << QPointF(558.2116665398979, -610.3912169130674);
    points << QPointF(581.9682339950501, -610.7865797593487);
    points << QPointF(604.5534121209836, -611.572910800845);
    points << QPointF(626.0008011392239, -612.726626131226);
    points << QPointF(646.344001271297, -614.2241418441603);
    points << QPointF(665.6166127387285, -616.0418740333172);
    points << QPointF(683.852235763044, -618.1562387923659);
    points << QPointF(701.0844705657695, -620.5436522149755);
    points << QPointF(717.3469173684304, -623.1805303948154);
    points << QPointF(740.1064589876694, -627.5281911653576);
    points << QPointF(767.2158272509128, -634.0157291924479);
    points << QPointF(791.050847501361, -641.0766159345035);
    points << QPointF(811.8803215112196, -648.5221801408777);
    points << QPointF(829.9730510526945, -656.1637505609234);
    points << QPointF(845.5978378979912, -663.8126559439941);
    points << QPointF(859.0234838193154, -671.2802250394427);
    points << QPointF(875.8174594072352, -681.8104815843149);
    points << QPointF(892.6995269725167, -693.39354704172);
    points << QPointF(899.4017385826772, -697.5307086614174);

    point = QPointF(756.052743609466, -631.2251813422373);

    // Real case. Issue #458. https://bitbucket.org/dismine/valentina/issues/458/issue-with-segment-of-curve
    // See file <root>/src/app/share/collection/bugs/Issue_#458.val
    QTest::newRow("Issue #458") << points << point << true;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractCurve::IsPointOnCurve() const
{
    QFETCH(QVector<QPointF>, points);
    QFETCH(QPointF, point);
    QFETCH(bool, expectedResult);

    bool result = VAbstractCurve::IsPointOnCurve(points, point);
    QCOMPARE(result, expectedResult);
}
