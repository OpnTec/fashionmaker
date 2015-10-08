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
