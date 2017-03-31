/************************************************************************
 **
 **  @file   vistooltruedarts.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 6, 2015
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

#include "vistooltruedarts.h"

#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/tooldoublepoint/vtooltruedarts.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolTrueDarts::VisToolTrueDarts(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent),
      baseLineP2Id(NULL_ID),
      dartP1Id(NULL_ID),
      dartP2Id(NULL_ID),
      dartP3Id(NULL_ID),
      point1(nullptr),
      point2(nullptr),
      baseLineP1(nullptr),
      baseLineP2(nullptr),
      dartP1(nullptr),
      dartP2(nullptr),
      dartP3(nullptr),
      lineblP1P1(nullptr),
      lineblP2P2(nullptr),
      p1d2(nullptr),
      d2p2(nullptr)
{
    baseLineP1 = InitPoint(supportColor, this);
    baseLineP2 = InitPoint(supportColor, this);
    dartP1 = InitPoint(supportColor, this);
    dartP2 = InitPoint(supportColor, this);
    dartP3 = InitPoint(supportColor, this);

    lineblP1P1 = InitItem<QGraphicsLineItem>(supportColor, this);
    lineblP2P2 = InitItem<QGraphicsLineItem>(supportColor, this);
    p1d2 = InitItem<QGraphicsLineItem>(supportColor, this);
    d2p2 = InitItem<QGraphicsLineItem>(supportColor, this);

    point1 = InitPoint(mainColor, this);
    point2 = InitPoint(mainColor, this); //-V656
}

//---------------------------------------------------------------------------------------------------------------------
VisToolTrueDarts::~VisToolTrueDarts()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTrueDarts::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> blP1 = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(baseLineP1, static_cast<QPointF>(*blP1), supportColor);

        if (baseLineP2Id <= NULL_ID)
        {
            DrawLine(this, QLineF(static_cast<QPointF>(*blP1), Visualization::scenePos), supportColor, Qt::DashLine);
        }
        else
        {
            const QSharedPointer<VPointF> blP2 = Visualization::data->GeometricObject<VPointF>(baseLineP2Id);
            DrawPoint(baseLineP2, static_cast<QPointF>(*blP2), supportColor);
            DrawLine(this, QLineF(static_cast<QPointF>(*blP1), static_cast<QPointF>(*blP2)), supportColor,
                     Qt::DashLine);

            if (dartP1Id > NULL_ID)
            {
                const QSharedPointer<VPointF> d1 = Visualization::data->GeometricObject<VPointF>(dartP1Id);
                DrawPoint(dartP1, static_cast<QPointF>(*d1), supportColor);

                if (dartP2Id <= NULL_ID)
                {
                    DrawLine(p1d2, QLineF(static_cast<QPointF>(*d1), Visualization::scenePos), supportColor);
                }
                else
                {
                    const QSharedPointer<VPointF> d2 = Visualization::data->GeometricObject<VPointF>(dartP2Id);
                    DrawPoint(dartP2, static_cast<QPointF>(*d2), supportColor);
                    DrawLine(p1d2, QLineF(static_cast<QPointF>(*d1), static_cast<QPointF>(*d2)), supportColor);

                    if (dartP3Id <= NULL_ID)
                    {
                        DrawLine(d2p2, QLineF(static_cast<QPointF>(*d2), Visualization::scenePos), supportColor);
                    }
                    else
                    {
                        const QSharedPointer<VPointF> d3 = Visualization::data->GeometricObject<VPointF>(dartP3Id);
                        DrawPoint(dartP3, static_cast<QPointF>(*d3), supportColor);
                        DrawLine(d2p2, QLineF(static_cast<QPointF>(*d2), static_cast<QPointF>(*d3)), supportColor);

                        QPointF p1;
                        QPointF p2;
                        VToolTrueDarts::FindPoint(static_cast<QPointF>(*blP1), static_cast<QPointF>(*blP2),
                                                  static_cast<QPointF>(*d1), static_cast<QPointF>(*d2),
                                                  static_cast<QPointF>(*d3), p1, p2);

                        DrawLine(lineblP1P1, QLineF(static_cast<QPointF>(*blP1), p1), supportColor);
                        DrawLine(lineblP2P2, QLineF(static_cast<QPointF>(*blP2), p2), supportColor);
                        DrawLine(p1d2, QLineF(p1, static_cast<QPointF>(*d2)), supportColor);
                        DrawLine(d2p2, QLineF(static_cast<QPointF>(*d2), p2), supportColor);

                        DrawPoint(point1, p1, mainColor);
                        DrawPoint(point2, p2, mainColor);
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTrueDarts::setObject2Id(const quint32 &value)
{
    baseLineP2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTrueDarts::setD1PointId(const quint32 &value)
{
    dartP1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTrueDarts::setD2PointId(const quint32 &value)
{
    dartP2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolTrueDarts::setD3PointId(const quint32 &value)
{
    dartP3Id = value;
}
