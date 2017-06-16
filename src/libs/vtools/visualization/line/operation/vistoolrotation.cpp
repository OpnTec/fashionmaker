/************************************************************************
 **
 **  @file   vistoolrotation.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2016
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

#include "vistoolrotation.h"

#include <limits.h>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QGuiApplication>
#include <QLineF>
#include <QPainterPath>
#include <QSharedPointer>
#include <Qt>
#include <QtAlgorithms>
#include <new>

#include "../vmisc/diagnostic.h"
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
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "visoperation.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolRotation::VisToolRotation(const VContainer *data, QGraphicsItem *parent)
    : VisOperation(data, parent),
      angle(INT_MIN),
      point(nullptr),
      angleArc(nullptr),
      xAxis(nullptr)
{
    point = InitPoint(supportColor2, this);
    angleArc = InitItem<VCurvePathItem>(supportColor2, this);
    xAxis = InitItem<QGraphicsLineItem>(supportColor2, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolRotation::~VisToolRotation()
{
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")

void VisToolRotation::RefreshGeometry()
{
    if (objects.isEmpty())
    {
        return;
    }

    QSharedPointer<VPointF> origin = QSharedPointer<VPointF>(new VPointF());

    qreal tempAngle = 0;

    if (object1Id != NULL_ID)
    {
        origin = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(point, static_cast<QPointF>(*origin), supportColor2);

        QLineF rLine;
        if (VFuzzyComparePossibleNulls(angle, INT_MIN))
        {
            rLine = QLineF(static_cast<QPointF>(*origin), Visualization::scenePos);

            if (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
            {
                rLine.setAngle(CorrectAngle(rLine.angle()));
            }

            rLine.setP2(Ray(static_cast<QPointF>(*origin), rLine.angle()));
            tempAngle = rLine.angle();
        }
        else
        {
            rLine = QLineF(static_cast<QPointF>(*origin), Ray(static_cast<QPointF>(*origin), angle));
            tempAngle = angle;
        }

        DrawLine(this, rLine, supportColor2, Qt::DashLine);
        DrawLine(xAxis, QLineF(static_cast<QPointF>(*origin), Ray(static_cast<QPointF>(*origin), 0)), supportColor2,
                 Qt::DashLine);

        VArc arc(*origin, defPointRadiusPixel*2, 0, tempAngle);
        DrawPath(angleArc, arc.GetPath(PathDirection::Hide), supportColor2, Qt::SolidLine, Qt::RoundCap);

        Visualization::toolTip = tr("Rotating angle = %1°, <b>Shift</b> - sticking angle, "
                                    "<b>Mouse click</b> - finish creation").arg(tempAngle);
    }

    int iPoint = -1;
    int iCurve = -1;
    for (int i = 0; i < objects.size(); ++i)
    {
        const quint32 id = objects.at(i);
        const QSharedPointer<VGObject> obj = Visualization::data->GetGObject(id);

        // This check helps to find missed objects in the switch
        Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 7, "Not all objects was handled.");

        switch(static_cast<GOType>(obj->getType()))
        {
            case GOType::Point:
            {
                const QSharedPointer<VPointF> p = Visualization::data->GeometricObject<VPointF>(id);

                ++iPoint;
                QGraphicsEllipseItem *point = GetPoint(static_cast<quint32>(iPoint), supportColor2);
                DrawPoint(point, static_cast<QPointF>(*p), supportColor2);

                ++iPoint;
                point = GetPoint(static_cast<quint32>(iPoint), supportColor);

                if (object1Id != NULL_ID)
                {
                    DrawPoint(point, static_cast<QPointF>(p->Rotate(static_cast<QPointF>(*origin), tempAngle)),
                              supportColor);
                }
                break;
            }
            case GOType::Arc:
            {
                iCurve = AddCurve<VArc>(tempAngle, static_cast<QPointF>(*origin), id, iCurve);
                break;
            }
            case GOType::EllipticalArc:
            {
                iCurve = AddCurve<VEllipticalArc>(tempAngle, static_cast<QPointF>(*origin), id, iCurve);
                break;
            }
            case GOType::Spline:
            {
                iCurve = AddCurve<VSpline>(tempAngle, static_cast<QPointF>(*origin), id, iCurve);
                break;
            }
            case GOType::SplinePath:
            {
                iCurve = AddCurve<VSplinePath>(tempAngle, static_cast<QPointF>(*origin), id, iCurve);
                break;
            }
            case GOType::CubicBezier:
            {
                iCurve = AddCurve<VCubicBezier>(tempAngle, static_cast<QPointF>(*origin), id, iCurve);
                break;
            }
            case GOType::CubicBezierPath:
            {
                iCurve = AddCurve<VCubicBezierPath>(tempAngle, static_cast<QPointF>(*origin), id, iCurve);
                break;
            }
            case GOType::Unknown:
                break;
        }
    }
}
QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
void VisToolRotation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    ScalePoint(point, scale);
    ScalePenWidth(angleArc, scale);
    ScalePenWidth(xAxis, scale);

    VisOperation::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolRotation::SetOriginPointId(quint32 value)
{
    object1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolRotation::Angle() const
{
    return QString::number(line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolRotation::SetAngle(const QString &expression)
{
    angle = FindVal(expression, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
int VisToolRotation::AddCurve(qreal angle, const QPointF &origin, quint32 id, int i)
{
    const QSharedPointer<Item> curve = Visualization::data->template GeometricObject<Item>(id);

    ++i;
    VCurvePathItem *path = GetCurve(static_cast<quint32>(i), supportColor2);
    DrawPath(path, curve->GetPath(PathDirection::Show), supportColor2, Qt::SolidLine, Qt::RoundCap);

    ++i;
    path = GetCurve(static_cast<quint32>(i), supportColor);
    if (object1Id != NULL_ID)
    {
        const Item rotated = curve->Rotate(origin, angle);
        DrawPath(path, rotated.GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);
    }

    return i;
}
