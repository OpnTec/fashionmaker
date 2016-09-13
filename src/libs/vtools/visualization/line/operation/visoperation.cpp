/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
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

#include "visoperation.h"

//---------------------------------------------------------------------------------------------------------------------
VisOperation::VisOperation(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent),
      objects(),
      supportColor2(Qt::darkGreen),
      points(),
      curves()
{

}

//---------------------------------------------------------------------------------------------------------------------
VisOperation::~VisOperation()
{
    qDeleteAll(points);
    qDeleteAll(curves);
}

//---------------------------------------------------------------------------------------------------------------------
void VisOperation::SetObjects(QVector<quint32> objects)
{
    this->objects = objects;
}

//---------------------------------------------------------------------------------------------------------------------
void VisOperation::VisualMode(const quint32 &pointId)
{
    Q_UNUSED(pointId);
    VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr);

    Visualization::scenePos = scene->getScenePos();
    RefreshGeometry();

    AddOnScene();
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsEllipseItem *VisOperation::GetPoint(quint32 i, const QColor &color)
{
    if (not points.isEmpty() && static_cast<quint32>(points.size() - 1) >= i)
    {
        return points.at(static_cast<int>(i));
    }
    else
    {
        auto point = InitPoint(color, this);
        points.append(point);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsPathItem *VisOperation::GetCurve(quint32 i, const QColor &color)
{
    if (not curves.isEmpty() && static_cast<quint32>(curves.size() - 1) >= i)
    {
        return curves.at(static_cast<int>(i));
    }
    else
    {
        auto curve = InitItem<QGraphicsPathItem>(color, this);
        curves.append(curve);
        return curve;
    }
    return nullptr;
}

