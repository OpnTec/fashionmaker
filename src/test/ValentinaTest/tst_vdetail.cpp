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
    origPoints.append(QPointF(42.46405659601934, 415.2845470563871));
    origPoints.append(QPointF(668.6348763356573, 559.9979528395143));
    origPoints.append(QPointF(672.5077340045727, 662.2773194632347));
    origPoints.append(QPointF(682.7797640244206, 723.1972585208631));
    origPoints.append(QPointF(696.5440854182214, 760.8937002484754));
    origPoints.append(QPointF(714.3917818918, 792.5896577365422));
    origPoints.append(QPointF(730.8871996023822, 810.8059842268941));
    origPoints.append(QPointF(749.7001857069857, 820.4573142312262));
    origPoints.append(QPointF(774.1450140007969, 824.7139100366174));
    origPoints.append(QPointF(825.8284608555382, 823.6826266774736));
    origPoints.append(QPointF(828.6858753986579, 1697.305833468011));

    // Begin comparison
    Comparison(pointsEkv, origPoints);
}
