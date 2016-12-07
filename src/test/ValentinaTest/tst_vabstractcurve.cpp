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
