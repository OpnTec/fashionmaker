/************************************************************************
 **
 **  @file   vistoolpointofintersectioncurves.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 1, 2016
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

#include "vistoolpointofintersectioncurves.h"
#include "../../tools/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectioncurves.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfIntersectionCurves::VisToolPointOfIntersectionCurves(const VContainer *data, QGraphicsItem *parent)
    :VisPath(data, parent),
      object2Id(NULL_ID),
      vCrossPoint(VCrossCurvesPoint::HighestPoint),
      hCrossPoint(HCrossCurvesPoint::LeftmostPoint),
      point(nullptr),
      visCurve2(nullptr)
{
    visCurve2 = InitItem<QGraphicsPathItem>(supportColor, this);
    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCurves::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        auto curve1 = Visualization::data->GeometricObject<VAbstractCurve>(object1Id);
        DrawPath(this, curve1->GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);

        if (object2Id > NULL_ID)
        {
            auto curve2 = Visualization::data->GeometricObject<VAbstractCurve>(object2Id);
            DrawPath(visCurve2, curve2->GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);

            auto p = VToolPointOfIntersectionCurves::FindPoint(curve1->GetPoints(), curve2->GetPoints(), vCrossPoint,
                                                               hCrossPoint);
            DrawPoint(point, p, mainColor);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCurves::VisualMode(const quint32 &id)
{
    auto scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr);

    this->object1Id = id;
    Visualization::scenePos = scene->getScenePos();
    RefreshGeometry();

    AddOnScene();
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCurves::setObject2Id(const quint32 &value)
{
    object2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCurves::setVCrossPoint(const VCrossCurvesPoint &value)
{
    vCrossPoint = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCurves::setHCrossPoint(const HCrossCurvesPoint &value)
{
    hCrossPoint = value;
}
