/************************************************************************
 **
 **  @file   tst_vdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 1, 2016
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

#include "tst_vdetail.h"
#include "../vpatterndb/vcontainer.h"
#include "../vgeometry/vsplinepath.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VDetail::TST_VDetail(QObject *parent)
    :AbstractTest(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VDetail::ClearLoop()
{
    // Input data taken from real case
    // See file <root>/src/app/share/collection/jacketМ6_30-110.val
    // Check correct seam allowance
    const Unit unit = Unit::Mm;
    VContainer *data = new VContainer(nullptr, &unit);

    data->UpdateGObject(304, new VPointF(61.866708661417327, 446.92270866141735, "Ф1", 5.0000125984251973,
                                         9.9999874015748045));
    data->UpdateGObject(307, new VPointF(642.96276692900597, 581.21895343695326, "С1", 88.99993700787401,
                                         50.000125984251973));

    data->UpdateGObject(56, new VPointF(802.08718110236236, 850.6707401574804, "Г6", 20.733316535433072,
                                        18.132850393700789));
    data->UpdateGObject(57, new VPointF(690.47666217505162, 804.29700711628709, "З", -11.505637795275591,
                                        31.221543307086616));
    data->UpdateGObject(203, new VPointF(642.96276692900597, 581.21895343695326, "С1", 88.99993700787401,
                                         50.000125984251973));

    VSplinePath *path = new VSplinePath();

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(203);
        VSplinePoint p(*point.data(), 0.79455646129695412, 449.62747641208136, 1.6867283804609809, 269.62747641208136);
        path->append(p);
    }

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(57);
        VSplinePoint p(*point.data(), 0.4456850846354396, 120.24000000000034, 1.0255399999999999, 300.24000000000035);
        path->append(p);
    }

    {
        const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(56);
        VSplinePoint p(*point.data(), 1.0085299999999999, 184.58891, 1, 4.5889100000000003);
        path->append(p);
    }

    data->UpdateGObject(308, path);

    data->UpdateGObject(309, new VPointF(799.45989815267649, 850.6707401574804, "Г8", -30.431206299212597,
                                         29.487155905511813));
    data->UpdateGObject(310, new VPointF(802.08718110236236, 1653.9337322834645, "Н5", 5.0000125984251973,
                                         9.9999874015748045));

    VDetail detail;
    detail.setSeamAllowance(true);
    detail.setWidth(7);
    detail.setClosed(false);
    detail.append(VNodeDetail(304, Tool::NodePoint, NodeDetail::Contour));
    detail.append(VNodeDetail(307, Tool::NodePoint, NodeDetail::Contour));
    detail.append(VNodeDetail(308, Tool::NodeSplinePath, NodeDetail::Contour));
    detail.append(VNodeDetail(309, Tool::NodePoint, NodeDetail::Contour));
    detail.append(VNodeDetail(310, Tool::NodePoint, NodeDetail::Contour));

    const QVector<QPointF> pointsEkv = detail.SeamAllowancePoints(data);

    QVector<QPointF> origPoints;
    origPoints.append(QPointF(42.464056596, 415.284547056));
    origPoints.append(QPointF(669.471111282, 560.191213853));
    origPoints.append(QPointF(669.386058691, 594.870268822));
    origPoints.append(QPointF(669.853724171, 619.849997563));
    origPoints.append(QPointF(670.904422406, 642.3178846));
    origPoints.append(QPointF(672.476094621, 662.479332552));
    origPoints.append(QPointF(674.504307528, 680.4882883));
    origPoints.append(QPointF(676.923618554, 696.502389941));
    origPoints.append(QPointF(679.668504965, 710.685043438));
    origPoints.append(QPointF(682.675134578, 723.207854677));
    origPoints.append(QPointF(685.884182534, 734.253021932));
    origPoints.append(QPointF(689.244614632, 744.014989124));
    origPoints.append(QPointF(692.717799245, 752.700488615));
    origPoints.append(QPointF(696.244854868, 760.447827851));
    origPoints.append(QPointF(701.80053872, 771.230129596));
    origPoints.append(QPointF(709.490850269, 784.462136001));
    origPoints.append(QPointF(713.209035073, 790.761640932));
    origPoints.append(QPointF(715.012191536, 793.76372792));
    origPoints.append(QPointF(718.780883478, 799.17428152));
    origPoints.append(QPointF(722.57235226, 803.731752263));
    origPoints.append(QPointF(726.490081061, 807.667595608));
    origPoints.append(QPointF(730.558043385, 811.069205493));
    origPoints.append(QPointF(734.817246318, 814.013788881));
    origPoints.append(QPointF(739.318992666, 816.561622842));
    origPoints.append(QPointF(744.115969332, 818.753220198));
    origPoints.append(QPointF(749.253944798, 820.61090345));
    origPoints.append(QPointF(754.766262359, 822.143554621));
    origPoints.append(QPointF(760.671847372, 823.352504448));
    origPoints.append(QPointF(766.976111339, 824.236813134));
    origPoints.append(QPointF(773.673526571, 824.797038187));
    origPoints.append(QPointF(780.661572758, 825.034345703));
    origPoints.append(QPointF(791.720374143, 824.854420471));
    origPoints.append(QPointF(825.821175407, 821.455180638));
    origPoints.append(QPointF(828.685875399, 1697.30583347));

    // Begin comparison
    Comparison(pointsEkv, origPoints);
}
