/************************************************************************
 **
 **  @file   vistoolcutspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 9, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vistoolcutspline.h"
#include "../geometry/vspline.h"
#include "../container/vcontainer.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolCutSpline::VisToolCutSpline(const VContainer *data, QGraphicsItem *parent)
    :VisPath(data, parent), point(nullptr), spl1(nullptr), spl2(nullptr), length(0)
{
    spl1 = InitItem<QGraphicsPathItem>(Qt::darkGreen, this);
    spl1->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
    spl2 = InitItem<QGraphicsPathItem>(Qt::darkRed, this);
    spl2->setFlag(QGraphicsItem::ItemStacksBehindParent, false);

    point = InitPoint(mainColor, this);
    point->setZValue(2);
    point->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolCutSpline::~VisToolCutSpline()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCutSpline::RefreshGeometry()
{
    if (point1Id > 0)
    {
        const QSharedPointer<VSpline> spl = Visualization::data->GeometricObject<VSpline>(point1Id);
        DrawPath(this, spl->GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);

        if (qFuzzyCompare(1 + length, 1 + 0) == false)
        {
            VSpline sp1;
            VSpline sp2;
            QPointF p = spl->CutSpline(length, sp1, sp2);
            DrawPoint(point, p, mainColor);

            DrawPath(spl1, sp1.GetPath(PathDirection::Show), Qt::darkGreen, Qt::SolidLine, Qt::RoundCap);
            DrawPath(spl2, sp2.GetPath(PathDirection::Show), Qt::darkRed, Qt::SolidLine, Qt::RoundCap);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCutSpline::setLength(const QString &expression)
{
    length = FindLength(expression);
}
