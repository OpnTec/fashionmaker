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
void TST_VSpline::CompareSplines(const VSpline &spl1, const VSpline &spl2) const
{
    QCOMPARE(spl1.GetP1().toQPointF().toPoint(), spl2.GetP1().toQPointF().toPoint());
    QCOMPARE(spl1.GetP2().toPoint(), spl2.GetP2().toPoint());
    QCOMPARE(spl1.GetP3().toPoint(), spl2.GetP3().toPoint());
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
