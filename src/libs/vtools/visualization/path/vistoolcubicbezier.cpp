/************************************************************************
 **
 **  @file   vistoolcubicbezier.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 3, 2016
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

#include "vistoolcubicbezier.h"
#include "../vgeometry/vcubicbezier.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolCubicBezier::VisToolCubicBezier(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent),
      object2Id(NULL_ID),
      object3Id(NULL_ID),
      object4Id(NULL_ID),
      point1(nullptr),
      point2(nullptr),
      point3(nullptr),
      point4(nullptr),
      helpLine1(nullptr),
      helpLine2(nullptr)
{
    helpLine1 = InitItem<QGraphicsLineItem>(mainColor, this);
    helpLine2 = InitItem<QGraphicsLineItem>(mainColor, this);

    point1 = InitPoint(supportColor, this);
    point2 = InitPoint(supportColor, this); //-V656
    point3 = InitPoint(supportColor, this); //-V656
    point4 = InitPoint(supportColor, this); //-V656
}

//---------------------------------------------------------------------------------------------------------------------
VisToolCubicBezier::~VisToolCubicBezier()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezier::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const auto first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(point1, *first, Qt::DashLine);

        if (object2Id <= NULL_ID)
        {
            DrawLine(helpLine1, QLineF(*first, Visualization::scenePos), mainColor, Qt::DashLine);
        }
        else
        {
            const auto second = Visualization::data->GeometricObject<VPointF>(object2Id);
            DrawPoint(point2, *second, supportColor);
            DrawLine(helpLine1, QLineF(*first, *second), mainColor, Qt::DashLine);

            if (object3Id <= NULL_ID)
            {
                VCubicBezier spline(*first, *second, VPointF(Visualization::scenePos),
                                    VPointF(Visualization::scenePos));
                DrawPath(this, spline.GetPath(PathDirection::Hide), mainColor, Qt::SolidLine, Qt::RoundCap);
            }
            else
            {
                const auto third = Visualization::data->GeometricObject<VPointF>(object3Id);
                DrawPoint(point3, *third, supportColor);

                if (object4Id <= NULL_ID)
                {
                    VCubicBezier spline(*first, *second, *third,  VPointF(Visualization::scenePos));
                    DrawPath(this, spline.GetPath(PathDirection::Hide), mainColor, Qt::SolidLine, Qt::RoundCap);
                    DrawLine(helpLine2, QLineF(*third, Visualization::scenePos), mainColor, Qt::DashLine);
                }
                else
                {
                    const auto fourth = Visualization::data->GeometricObject<VPointF>(object4Id);
                    DrawPoint(point4, *fourth, supportColor);
                    DrawLine(helpLine2, QLineF(*fourth, *third), mainColor, Qt::DashLine);

                    VCubicBezier spline(*first, *second, *third,  *fourth);
                    DrawPath(this, spline.GetPath(PathDirection::Show), mainColor, Qt::SolidLine, Qt::RoundCap);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezier::setObject2Id(const quint32 &value)
{
    object2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezier::setObject3Id(const quint32 &value)
{
    object3Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezier::setObject4Id(const quint32 &value)
{
    object4Id = value;
}
