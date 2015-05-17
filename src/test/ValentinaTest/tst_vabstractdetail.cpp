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
#include "../../libs/vlayout/vabstractdetail.h"

#include <QPointF>
#include <QVector>

//---------------------------------------------------------------------------------------------------------------------
TST_VAbstractDetail::TST_VAbstractDetail(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::EquidistantRemoveLoop() const
{
    // These are two real cases where equdistant has loop.
    // See issue #298. Segmented Curve isn't selected in Seam Allowance tool.
    // https://bitbucket.org/dismine/valentina/issue/298/segmented-curve-isnt-selected-in-seam
    // Code should clean loops in path.
    Case1();
    Case2();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::Case1() const
{
    const QVector<QPointF> points = InputPointsCase1(); // Input points.
    const EquidistantType eqv = EquidistantType::CloseEquidistant; // Closed path
    const qreal width = 75.5906; // seam allowance width

    const QVector<QPointF> ekv = VAbstractDetail::Equidistant(points, eqv, width);// Take result
    const QVector<QPointF> ekvOrig = OutputPointsCase1(); // Expected result

    // Begin comparison
    Comparison(ekv, ekvOrig);
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VAbstractDetail::Case2() const
{
    const QVector<QPointF> points = InputPointsCase2(); // Input points.
    const EquidistantType eqv = EquidistantType::CloseEquidistant; // Closed path
    const qreal width = 37.7953; // seam allowance width

    const QVector<QPointF> ekv = VAbstractDetail::Equidistant(points, eqv, width);// Take result
    const QVector<QPointF> ekvOrig = OutputPointsCase2(); // Expected result

    // Begin comparison
    Comparison(ekv, ekvOrig);
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

    points += QPointF(-52.3725, -35.5907);
    points += QPointF(487.711, -35.5907);
    points += QPointF(493.343, 473.326);
    points += QPointF(385.981, 506.845);
    points += QPointF(345.647, 447.143);
    points += QPointF(326.825, 417.766);
    points += QPointF(297.492, 369.749);
    points += QPointF(280.344, 340.638);
    points += QPointF(268.238, 345.564);
    points += QPointF(254.389, 348.788);
    points += QPointF(240.894, 350.022);
    points += QPointF(224.296, 349.294);
    points += QPointF(205.506, 345.316);
    points += QPointF(188.726, 339.383);
    points += QPointF(173.487, 332.257);
    points += QPointF(159.093, 324.152);
    points += QPointF(145.156, 315.146);
    points += QPointF(131.469, 305.281);
    points += QPointF(117.936, 294.59);
    points += QPointF(104.527, 283.112);
    points += QPointF(91.2504, 270.888);
    points += QPointF(78.1433, 257.963);
    points += QPointF(65.2575, 244.383);
    points += QPointF(52.6567, 230.194);
    points += QPointF(40.4121, 215.44);
    points += QPointF(28.6008, 200.16);
    points += QPointF(17.3047, 184.387);
    points += QPointF(6.61028, 168.141);
    points += QPointF(-3.38943, 151.431);
    points += QPointF(-12.5922, 134.245);
    points += QPointF(-20.8806, 116.548);
    points += QPointF(-28.1111, 98.2771);
    points += QPointF(-34.0981, 79.3367);
    points += QPointF(-38.4416, 60.2487);
    points += QPointF(-52.3725, -35.5907);

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

    points += QPointF(-2.79526, 4.83848);
    points += QPointF(67.3445, -0.232496);
    points += QPointF(73.1172, 39.482);
    points += QPointF(75.4242, 49.6203);
    points += QPointF(78.7941, 60.2814);
    points += QPointF(83.273, 71.5993);
    points += QPointF(88.7999, 83.3999);
    points += QPointF(95.2925, 95.5245);
    points += QPointF(102.656, 107.829);
    points += QPointF(110.787, 120.18);
    points += QPointF(119.578, 132.456);
    points += QPointF(128.919, 144.54);
    points += QPointF(138.697, 156.322);
    points += QPointF(148.796, 167.694);
    points += QPointF(159.098, 178.552);
    points += QPointF(169.482, 188.791);
    points += QPointF(179.818, 198.308);
    points += QPointF(189.972, 207);
    points += QPointF(199.794, 214.759);
    points += QPointF(209.115, 221.477);
    points += QPointF(216.034, 225.948);
    points += QPointF(215.311, 223.266);
    points += QPointF(212.945, 216.109);
    points += QPointF(205.759, 194.78);
    points += QPointF(197.889, 169.298);
    points += QPointF(193.975, 155.081);
    points += QPointF(191.164, 142.904);
    points += QPointF(189.364, 131.496);
    points += QPointF(189.144, 117.752);
    points += QPointF(194.429, 100.985);
    points += QPointF(210.02, 85.4196);
    points += QPointF(231.367, 81.4782);
    points += QPointF(246.493, 85.8976);
    points += QPointF(256.602, 92.4352);
    points += QPointF(264.477, 100.216);
    points += QPointF(270.989, 108.11);
    points += QPointF(280.35, 121.002);
    points += QPointF(294.425, 142.543);
    points += QPointF(318.564, 182.007);
    points += QPointF(355.648, 245.077);
    points += QPointF(394.726, 311.416);
    points += QPointF(422.951, 357.621);
    points += QPointF(440.373, 384.812);
    points += QPointF(488.279, 455.72);
    points += QPointF(-2.79526, 455.75);
    points += QPointF(-2.79526, 4.83848);

    return points;
}
