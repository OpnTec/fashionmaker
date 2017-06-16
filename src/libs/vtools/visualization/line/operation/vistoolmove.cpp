/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 10, 2016
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

#include "vistoolmove.h"

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

#include "../ifc/xml/vdomdocument.h"
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
VisToolMove::VisToolMove(const VContainer *data, QGraphicsItem *parent)
    : VisOperation(data, parent),
      angle(0),
      length(0),
      pointOrigin(nullptr),
      pointFinish(nullptr)
{
    pointOrigin = InitPoint(supportColor2, this);
    pointFinish = InitPoint(supportColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolMove::~VisToolMove()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolMove::RefreshGeometry()
{
    if (objects.isEmpty())
    {
        return;
    }

    int iPoint = -1;
    int iCurve = -1;

    const QVector<QGraphicsItem *> originObjects = CreateOriginObjects(iPoint, iCurve);

    const QPointF origin = GetOriginPoint(originObjects);
    DrawPoint(pointOrigin, origin, supportColor2);

    qreal tempAngle = 0;
    qreal tempLength = 0;

    QLineF line;
    if (qFuzzyIsNull(length))
    {
        if (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
        {
            line = QLineF(origin, Visualization::scenePos);
            line.setAngle(CorrectAngle(line.angle()));
        }
        else
        {
            line = QLineF(origin, Visualization::scenePos);
        }

        tempAngle = line.angle();
        tempLength = line.length();
    }
    else
    {
        line = VGObject::BuildLine(origin, length, angle);
        tempAngle = angle;
        tempLength = length;
    }
    DrawLine(this, line, supportColor2, Qt::DashLine);
    DrawPoint(pointFinish, line.p2(), supportColor);

    static const QString prefix = VDomDocument::UnitsToStr(qApp->patternUnit(), true);
    Visualization::toolTip = tr("Length = %1%2, angle = %3°, <b>Shift</b> - sticking angle, "
                                "<b>Mouse click</b> - finish creation")
            .arg(qApp->TrVars()->FormulaToUser(QString::number(qApp->fromPixel(tempLength)),
                                               qApp->Settings()->GetOsSeparator()))
            .arg(prefix)
            .arg(tempAngle);

    CreateMovedObjects(iPoint, iCurve, tempLength, tempAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolMove::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal scale = SceneScale(scene());

    ScalePoint(pointOrigin, scale);
    ScalePoint(pointFinish, scale);

    VisOperation::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VisToolMove::boundingRect() const
{
    QRectF rect = VisOperation::boundingRect();
    rect = rect.united(pointOrigin->boundingRect());
    rect = rect.united(pointFinish->boundingRect());
    return rect;
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolMove::Angle() const
{
    return QString::number(line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolMove::SetAngle(const QString &expression)
{
    angle = FindVal(expression, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolMove::Length() const
{
    return QString::number(LengthValue());
}

//---------------------------------------------------------------------------------------------------------------------
qreal VisToolMove::LengthValue() const
{
    return qApp->fromPixel(line().length());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolMove::SetLength(const QString &expression)
{
    length = FindLength(expression, Visualization::data->PlainVariables());
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
QGraphicsPathItem *VisToolMove::AddOriginCurve(quint32 id, int &i)
{
    const QSharedPointer<Item> curve = Visualization::data->template GeometricObject<Item>(id);

    ++i;
    VCurvePathItem *path = GetCurve(static_cast<quint32>(i), supportColor2);
    DrawPath(path, curve->GetPath(PathDirection::Show), supportColor2, Qt::SolidLine, Qt::RoundCap);

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
int VisToolMove::AddMovedCurve(qreal angle, qreal length, quint32 id, int i)
{
    const QSharedPointer<Item> curve = Visualization::data->template GeometricObject<Item>(id);

    ++i;
    VCurvePathItem *path = GetCurve(static_cast<quint32>(i), supportColor);
    const Item moved = curve->Move(length, angle);
    DrawPath(path, moved.GetPath(PathDirection::Show), supportColor, Qt::SolidLine, Qt::RoundCap);

    return i;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VisToolMove::GetOriginPoint(const QVector<QGraphicsItem *> &objects)
{
    QRectF boundingRect;
    for (int i=0; i < objects.size(); ++i)
    {
        QGraphicsItem *object = objects.at(i);
        if (object)
        {
            QRectF childrenRect = object->childrenBoundingRect();
            //map to scene coordinate.
            childrenRect.translate(object->scenePos());

            QRectF itemRect = object->boundingRect();
            //map to scene coordinate.
            itemRect.translate(object->scenePos());

            boundingRect = boundingRect.united(itemRect);
            boundingRect = boundingRect.united(childrenRect);
        }
    }

    return boundingRect.center();
}

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")

QVector<QGraphicsItem *> VisToolMove::CreateOriginObjects(int &iPoint, int &iCurve)
{
    QVector<QGraphicsItem *> originObjects;

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
                DrawPoint(point, static_cast<QPointF>(*p), supportColor2);
                originObjects.append(point);

                break;
            }
            case GOType::Arc:
                originObjects.append(AddOriginCurve<VArc>(id, iCurve));
                break;
            case GOType::EllipticalArc:
                originObjects.append(AddOriginCurve<VEllipticalArc>(id, iCurve));
                break;
            case GOType::Spline:
                originObjects.append(AddOriginCurve<VSpline>(id, iCurve));
                break;
            case GOType::SplinePath:
                originObjects.append(AddOriginCurve<VSplinePath>(id, iCurve));
                break;
            case GOType::CubicBezier:
                originObjects.append(AddOriginCurve<VCubicBezier>(id, iCurve));
                break;
            case GOType::CubicBezierPath:
                originObjects.append(AddOriginCurve<VCubicBezierPath>(id, iCurve));
                break;
            case GOType::Unknown:
                break;
        }
    }

    return originObjects;
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")

void VisToolMove::CreateMovedObjects(int &iPoint, int &iCurve, qreal length, qreal angle)
{
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
                QGraphicsEllipseItem *point = GetPoint(static_cast<quint32>(iPoint), supportColor);
                DrawPoint(point, static_cast<QPointF>(p->Move(length, angle)), supportColor);
                break;
            }
            case GOType::Arc:
                iCurve = AddMovedCurve<VArc>(angle, length, id, iCurve);
                break;
            case GOType::EllipticalArc:
                iCurve = AddMovedCurve<VEllipticalArc>(angle, length, id, iCurve);
                break;
            case GOType::Spline:
                iCurve = AddMovedCurve<VSpline>(angle, length, id, iCurve);
                break;
            case GOType::SplinePath:
                iCurve = AddMovedCurve<VSplinePath>(angle, length, id, iCurve);
                break;
            case GOType::CubicBezier:
                iCurve = AddMovedCurve<VCubicBezier>(angle, length, id, iCurve);
                break;
            case GOType::CubicBezierPath:
                iCurve = AddMovedCurve<VCubicBezierPath>(angle, length, id, iCurve);
                break;
            case GOType::Unknown:
                break;
        }
    }
}

QT_WARNING_POP
