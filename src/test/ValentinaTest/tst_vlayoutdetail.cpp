/************************************************************************
 **
 **  @file   tst_vlayoutdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 5, 2015
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

#include "tst_vlayoutdetail.h"
#include "../../libs/vlayout/vlayoutdetail.h"

#include <QtDebug>

//---------------------------------------------------------------------------------------------------------------------
TST_VLayoutDetail::TST_VLayoutDetail(QObject *parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VLayoutDetail::RemoveDublicates() const
{
    qDebug()<<"Case 1.";
    Case1();

    qDebug()<<"Case 2.";
    Case2();

    qDebug()<<"Case 3.";
    Case3();
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VLayoutDetail::Case1() const
{
    // See issue #304. Layout appears different than my pattern.
    // https://bitbucket.org/dismine/valentina/issue/304/layout-appears-different-than-my-pattern

    VLayoutDetail det = VLayoutDetail();
    det.SetCountourPoints(InputPointsCase1());

    // Begin comparison
    Comparison(det.GetContourPoints(), OutputPointsCase1());
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VLayoutDetail::InputPointsCase1() const
{
    QVector<QPointF> points;

    points += QPointF(557.0, -94.0);
    points += QPointF(760.0, -53.0);
    points += QPointF(661.0, 411.0);
    points += QPointF(708.0, 467.0);
    points += QPointF(366.0, 845.0);
    points += QPointF(750.0, 509.0);
    points += QPointF(957.0, 556.0);
    points += QPointF(933.0, 787.0);
    points += QPointF(366.0, 845.0);
    points += QPointF(921.0, 901.0);
    points += QPointF(883.0, 1158.0);
    points += QPointF(866.0, 1446.0);
    points += QPointF(396.0, 1446.0);
    points += QPointF(400.0, 1093.0);
    points += QPointF(366.0, 917.0);
    points += QPointF(366.0, 845.0);
    points += QPointF(466.0, 411.0);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VLayoutDetail::OutputPointsCase1() const
{
    QVector<QPointF> points;

    points += QPointF(557.0, -94.0);
    points += QPointF(760.0, -53.0);
    points += QPointF(661.0, 411.0);
    points += QPointF(708.0, 467.0);
    points += QPointF(366.0, 845.0);
    points += QPointF(750.0, 509.0);
    points += QPointF(957.0, 556.0);
    points += QPointF(933.0, 787.0);
    points += QPointF(366.0, 845.0);
    points += QPointF(921.0, 901.0);
    points += QPointF(883.0, 1158.0);
    points += QPointF(866.0, 1446.0);
    points += QPointF(396.0, 1446.0);
    points += QPointF(400.0, 1093.0);
    points += QPointF(366.0, 917.0);
    points += QPointF(366.0, 845.0);
    points += QPointF(466.0, 411.0);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VLayoutDetail::Case2() const
{
    VLayoutDetail det = VLayoutDetail();
    det.SetCountourPoints(InputPointsCase2());

    // Begin comparison
    Comparison(det.GetContourPoints(), OutputPointsCase2());
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VLayoutDetail::InputPointsCase2() const
{
    QVector<QPointF> points;

    points += QPointF(557.0, -94.0);
    points += QPointF(760.0, -53.0);
    points += QPointF(760.0, -53.0);// Need delete this part
    points += QPointF(760.0, -53.0);// Need delete this part
    points += QPointF(760.0, -53.0);// Need delete this part
    points += QPointF(661.0, 411.0);
    points += QPointF(708.0, 467.0);
    points += QPointF(366.0, 845.0);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VLayoutDetail::OutputPointsCase2() const
{
    QVector<QPointF> points;

    points += QPointF(557.0, -94.0);
    points += QPointF(760.0, -53.0);
    points += QPointF(661.0, 411.0);
    points += QPointF(708.0, 467.0);
    points += QPointF(366.0, 845.0);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VLayoutDetail::Case3() const
{
    VLayoutDetail det = VLayoutDetail();
    det.SetCountourPoints(InputPointsCase3());

    // Begin comparison
    Comparison(det.GetContourPoints(), OutputPointsCase3());
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VLayoutDetail::InputPointsCase3() const
{
    QVector<QPointF> points;

    points += QPointF(557.0, -94.0);// Only one point

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> TST_VLayoutDetail::OutputPointsCase3() const
{
    QVector<QPointF> points;

    points += QPointF(557.0, -94.0);// Only one point

    return points;
}
