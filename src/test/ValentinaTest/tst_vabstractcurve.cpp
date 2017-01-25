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
void TST_VAbstractCurve::GetSegmentPoints_issue458()
{
    // Real case. Issue #458. https://bitbucket.org/dismine/valentina/issues/458/issue-with-segment-of-curve
    // See file AnalyzeStrangeIssueBug_isotated.val

    QVector<QPointF> curve;
    curve.append(QPointF(-11.267867716535433, -621.9401574803151));
    curve.append(QPointF(28.929492484925593, -621.8607845421614));
    curve.append(QPointF(111.02420124017556, -621.0204613872887));
    curve.append(QPointF(192.81663506084445, -619.5641192834869));
    curve.append(QPointF(271.7042817720716, -617.7160624366999));
    curve.append(QPointF(380.07266406952584, -614.6790467524394));
    curve.append(QPointF(489.18908873700275, -611.324295072277));
    curve.append(QPointF(520.2546519685039, -610.6015748031497));
    curve.append(QPointF(520.2546519685039, -610.6015748031497));
    curve.append(QPointF(533.2501074340664, -610.4104062194663));
    curve.append(QPointF(558.2116609090922, -610.3912171914401));
    curve.append(QPointF(581.9682255886432, -610.7865804010944));
    curve.append(QPointF(604.5534016414362, -611.572911926731));
    curve.append(QPointF(626.0007892361873, -612.7266278466516));
    curve.append(QPointF(646.343988541613, -614.224144239158));
    curve.append(QPointF(665.6165997264297, -616.0418771825518));
    curve.append(QPointF(683.8522229593539, -618.1562427551349));
    curve.append(QPointF(701.0844584091014, -620.543657035209));
    curve.append(QPointF(717.3469062443893, -623.1805361010756));
    curve.append(QPointF(740.1064499883596, -627.5281982218287));
    curve.append(QPointF(767.2158219967725, -634.0157380079388));
    curve.append(QPointF(791.0508465842408, -641.0766263466508));
    curve.append(QPointF(811.8803251004954, -648.5221918643786));
    curve.append(QPointF(829.9730588952673, -656.1637631875356));
    curve.append(QPointF(845.5978493182879, -663.8126689425358));
    curve.append(QPointF(859.0234977192879, -671.2802377557932));
    curve.append(QPointF(875.8174743129977, -681.8104925021551));
    curve.append(QPointF(892.6995351796473, -693.3935519951781));
    curve.append(QPointF(899.4017385826772, -697.5307086614174));

    QPointF begin (541.621890489816, -610.374541985993);
    QPointF end	(660.2625170532651, -735.7793605757131);

    const QVector<QPointF> points = VAbstractCurve::GetSegmentPoints(curve, begin, end, true);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(899.4017385826772, -697.5307086614174));
    origPoints.append(QPointF(892.6995351796473, -693.3935519951781));
    origPoints.append(QPointF(875.8174743129977, -681.8104925021551));
    origPoints.append(QPointF(859.0234977192879, -671.2802377557932));
    origPoints.append(QPointF(845.5978493182879, -663.8126689425358));
    origPoints.append(QPointF(829.9730588952673, -656.1637631875356));
    origPoints.append(QPointF(811.8803251004954, -648.5221918643786));
    origPoints.append(QPointF(791.0508465842408, -641.0766263466508));
    origPoints.append(QPointF(767.2158219967725, -634.0157380079388));
    origPoints.append(QPointF(740.1064499883596, -627.5281982218287));
    origPoints.append(QPointF(717.3469062443893, -623.1805361010756));
    origPoints.append(QPointF(701.0844584091014, -620.543657035209));
    origPoints.append(QPointF(683.8522229593539, -618.1562427551349));
    origPoints.append(QPointF(665.6165997264297, -616.0418771825518));
    origPoints.append(QPointF(646.343988541613, -614.224144239158));
    origPoints.append(QPointF(626.0007892361873, -612.7266278466516));
    origPoints.append(QPointF(604.5534016414362, -611.572911926731));
    origPoints.append(QPointF(581.9682255886432, -610.7865804010944));
    origPoints.append(QPointF(558.2116609090922, -610.3912171914401));
    origPoints.append(QPointF(533.2501074340664, -610.4104062194663));
    origPoints.append(QPointF(520.2546519685039, -610.6015748031497));
    origPoints.append(QPointF(520.2546519685039, -610.6015748031497));
    origPoints.append(QPointF(489.18908873700275, -611.324295072277));
    origPoints.append(QPointF(380.07266406952584, -614.6790467524394));
    origPoints.append(QPointF(271.7042817720716, -617.7160624366999));
    origPoints.append(QPointF(192.81663506084445, -619.5641192834869));
    origPoints.append(QPointF(111.02420124017556, -621.0204613872887));
    origPoints.append(QPointF(28.929492484925593, -621.8607845421614));
    origPoints.append(QPointF(-11.267867716535433, -621.9401574803151));

    // Begin comparison
    Comparison(points, origPoints);
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
