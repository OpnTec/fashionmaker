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
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"

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
    Q_UNUSED(pointId)
    VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr)

    Visualization::scenePos = scene->getScenePos();
    RefreshGeometry();

    AddOnScene();
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsEllipseItem *VisOperation::GetPoint(quint32 i, const QColor &color)
{
    return GetPointItem(Visualization::data, factor, points, i, color, this);
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

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
void VisOperation::RefreshFlippedObjects(const QPointF &firstPoint, const QPointF &secondPoint)
{
    int iPoint = -1;
    int iCurve = -1;
    for (int i = 0; i < objects.size(); ++i)
    {
        const quint32 id = objects.at(i);
        const QSharedPointer<VGObject> obj = Visualization::data->GetGObject(id);

        // This check helps to find missed objects in the switch
        Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects were handled.");

        switch(static_cast<GOType>(obj->getType()))
        {
            case GOType::Point:
            {
                const QSharedPointer<VPointF> p = Visualization::data->GeometricObject<VPointF>(id);

                ++iPoint;
                QGraphicsEllipseItem *point = GetPoint(static_cast<quint32>(iPoint), supportColor2);
                DrawPoint(point, *p, supportColor2);

                ++iPoint;
                point = GetPoint(static_cast<quint32>(iPoint), supportColor);

                if (object1Id != NULL_ID)
                {
                    DrawPoint(point, p->Flip(QLineF(firstPoint, secondPoint)), supportColor);
                }
                break;
            }
            case GOType::Arc:
            {
                iCurve = AddFlippedCurve<VArc>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::EllipticalArc:
            {
                iCurve = AddFlippedCurve<VEllipticalArc>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::Spline:
            {
                iCurve = AddFlippedCurve<VSpline>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::SplinePath:
            {
                iCurve = AddFlippedCurve<VSplinePath>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::CubicBezier:
            {
                iCurve = AddFlippedCurve<VCubicBezier>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::CubicBezierPath:
            {
                iCurve = AddFlippedCurve<VCubicBezierPath>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::Unknown:
                break;
        }
    }
}
QT_WARNING_POP
