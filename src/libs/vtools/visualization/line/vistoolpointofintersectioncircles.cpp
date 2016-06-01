/************************************************************************
 **
 **  @file   vistoolpointofintersectioncircles.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2015
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

#include "vistoolpointofintersectioncircles.h"
#include "../vpatterndb/vcontainer.h"
#include "../../tools/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectioncircles.h"
#include "../vgeometry/vpointf.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfIntersectionCircles::VisToolPointOfIntersectionCircles(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), object2Id(NULL_ID), c1Radius(0), c2Radius(0),
      crossPoint(CrossCirclesPoint::FirstPoint), point(nullptr), c1Center(nullptr), c2Center(nullptr), c1Path(nullptr),
      c2Path(nullptr)
{
    this->setPen(QPen(Qt::NoPen)); // don't use parent this time

    c1Path = InitItem<QGraphicsEllipseItem>(Qt::darkGreen, this);
    c2Path = InitItem<QGraphicsEllipseItem>(Qt::darkRed, this);
    point = InitPoint(mainColor, this);
    c1Center = InitPoint(supportColor, this);
    c2Center = InitPoint(supportColor, this);  //-V656
}

//---------------------------------------------------------------------------------------------------------------------
VisToolPointOfIntersectionCircles::~VisToolPointOfIntersectionCircles()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCircles::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(c1Center, *first, supportColor);

        if (object2Id > NULL_ID)
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(object2Id);
            DrawPoint(c2Center, *second, supportColor);

            if (c1Radius > 0 && c2Radius > 0)
            {
                c1Path->setRect(PointRect(c1Radius));
                DrawPoint(c1Path, *first, Qt::darkGreen, Qt::DashLine);

                c2Path->setRect(PointRect(c2Radius));
                DrawPoint(c2Path, *second, Qt::darkRed, Qt::DashLine);

                const QPointF fPoint = VToolPointOfIntersectionCircles::FindPoint(*first, *second, c1Radius,
                                                                                  c2Radius, crossPoint);
                DrawPoint(point, fPoint, mainColor);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCircles::VisualMode(const quint32 &id)
{
    VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr);

    this->object1Id = id;
    Visualization::scenePos = scene->getScenePos();
    RefreshGeometry();

    AddOnScene();
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCircles::setObject2Id(const quint32 &value)
{
    object2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCircles::setC1Radius(const QString &value)
{
    c1Radius = FindLength(value, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCircles::setC2Radius(const QString &value)
{
    c2Radius = FindLength(value, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPointOfIntersectionCircles::setCrossPoint(const CrossCirclesPoint &value)
{
    crossPoint = value;
}
