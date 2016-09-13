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

#include "vistoolflippingbyline.h"
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
VisToolFlippingByLine::VisToolFlippingByLine(const VContainer *data, QGraphicsItem *parent)
    : VisOperation(data, parent),
      object2Id(NULL_ID),
      point1(nullptr),
      point2(nullptr)
{
    point1 = InitPoint(supportColor2, this);
    point2 = InitPoint(supportColor2, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolFlippingByLine::~VisToolFlippingByLine()
{
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
void VisToolFlippingByLine::RefreshGeometry()
{
    if (objects.isEmpty())
    {
        return;
    }

    QPointF firstPoint;
    QPointF secondPoint;

    if (object1Id != NULL_ID)
    {
        firstPoint = *Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(point1, firstPoint, supportColor2);

        if (object2Id == NULL_ID)
        {
            secondPoint = Visualization::scenePos;
        }
        else
        {
            secondPoint = *Visualization::data->GeometricObject<VPointF>(object2Id);
            DrawPoint(point2, secondPoint, supportColor2);
        }

        DrawLine(this, QLineF(firstPoint, secondPoint), supportColor2, Qt::DashLine);
    }

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
                QGraphicsEllipseItem *point = GetPoint(iPoint, supportColor2);
                DrawPoint(point, *p, supportColor2);

                ++iPoint;
                point = GetPoint(iPoint, supportColor);

                if (object1Id != NULL_ID)
                {
                    DrawPoint(point, p->Flip(QLineF(firstPoint, secondPoint)), supportColor);
                }
                break;
            }
            case GOType::Arc:
            {
                iCurve = AddCurve<VArc>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::EllipticalArc:
            {
                iCurve = AddCurve<VEllipticalArc>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::Spline:
            {
                iCurve = AddCurve<VSpline>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::SplinePath:
            {
                iCurve = AddCurve<VSplinePath>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::CubicBezier:
            {
                iCurve = AddCurve<VCubicBezier>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::CubicBezierPath:
            {
                iCurve = AddCurve<VCubicBezierPath>(firstPoint, secondPoint, id, iCurve);
                break;
            }
            case GOType::Unknown:
                break;
        }
    }
}
QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
void VisToolFlippingByLine::SetFirstLinePointId(quint32 value)
{
    object1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolFlippingByLine::SetSecondLinePointId(quint32 value)
{
    object2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
int VisToolFlippingByLine::AddCurve(const QPointF &firstPoint, const QPointF &secondPoint, quint32 id, int i)
{
    const QSharedPointer<Item> curve = Visualization::data->template GeometricObject<Item>(id);

    ++i;
    QGraphicsPathItem *path = GetCurve(i, supportColor2);
    DrawPath(path, curve->GetPath(PathDirection::Show), supportColor2, Qt::SolidLine, Qt::RoundCap);

    ++i;
    path = GetCurve(i, supportColor);
    if (object1Id != NULL_ID)
    {
        const Item flipped = curve->Flip(QLineF(firstPoint, secondPoint));
        DrawPath(path, flipped.GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);
    }

    return i;
}
