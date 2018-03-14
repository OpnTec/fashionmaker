/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 10, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "../vgeometry/varc.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "visoperation.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolMove::VisToolMove(const VContainer *data, QGraphicsItem *parent)
    : VisOperation(data, parent),
      angle(0),
      rotationAngle(INT_MIN),
      length(0),
      pointOrigin(nullptr),
      pointRotationOrigin(nullptr),
      pointFinish(nullptr),
      angleArc(nullptr),
      rotationLine(nullptr),
      xAxis(nullptr)
{
    pointOrigin = InitPoint(supportColor2, this);
    pointRotationOrigin = InitPoint(supportColor2, this);
    pointFinish = InitPoint(supportColor, this);
    angleArc = InitItem<VCurvePathItem>(supportColor3, this);
    rotationLine = InitItem<VScaledLine>(supportColor3, this);
    xAxis = InitItem<VScaledLine>(supportColor3, this);
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

    QPointF origin = GetOriginPoint(originObjects);
    DrawPoint(pointOrigin, origin, supportColor2);

    qreal tempAngle = 0;
    qreal tempLength = 0;
    qreal tempRoationAngle = 0;

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

        if (object1Id != NULL_ID)
        {
            origin = Visualization::data->GeometricObject<VPointF>(object1Id)->toQPointF();
            DrawPoint(pointRotationOrigin, origin, supportColor2);
        }
        else
        {
            origin = line.p2();
            pointRotationOrigin->setVisible(false);
        }

        QLineF rLine;
        if (VFuzzyComparePossibleNulls(rotationAngle, INT_MIN))
        {
            rLine = QLineF(origin, Visualization::scenePos);

            if (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
            {
                rLine.setAngle(CorrectAngle(rLine.angle()));
            }

            qreal cursorLength = rLine.length();
            rLine.setP2(Ray(origin, rLine.angle()));
            //Radius of point circle, but little bigger. Need handle with hover sizes.
            qreal minL = ScaledRadius(SceneScale(qApp->getCurrentScene()))*1.5;
            if (cursorLength > minL)
            {
               tempRoationAngle = rLine.angle();
            }
            else
            {
                rLine.setAngle(0);
            }
        }
        else
        {
            rLine = QLineF(origin, Ray(origin, rotationAngle));
            tempRoationAngle = rotationAngle;
        }

        DrawLine(rotationLine, rLine, supportColor3, Qt::DashLine);
        DrawLine(xAxis, QLineF(origin, Ray(origin, 0)), supportColor3, Qt::DashLine);

        VArc arc(VPointF(origin), ScaledRadius(SceneScale(qApp->getCurrentScene()))*2, 0, tempRoationAngle);
        DrawPath(angleArc, arc.GetPath(), supportColor3, Qt::SolidLine, Qt::RoundCap);
    }
    DrawLine(this, line, supportColor2, Qt::DashLine);
    DrawPoint(pointFinish, line.p2(), supportColor);

    static const QString prefix = UnitsToStr(qApp->patternUnit(), true);
    if (qFuzzyIsNull(length))
    {
        Visualization::toolTip = tr("Length = %1%2, angle = %3°, <b>Shift</b> - sticking angle, "
                                    "<b>Mouse click</b> - finish selecting a position")
                .arg(qApp->TrVars()->FormulaToUser(QString::number(qApp->fromPixel(tempLength)),
                                                   qApp->Settings()->GetOsSeparator()), prefix)
                .arg(tempAngle);
    }
    else
    {
        Visualization::toolTip = tr("Length = %1%2, angle = %3°, rotation angle = %4°, <b>Shift</b> - sticking angle, "
                                    "<b>Ctrl</b> - change rotation origin point, <b>Mouse click</b> - finish creating")
                .arg(qApp->TrVars()->FormulaToUser(QString::number(qApp->fromPixel(tempLength)),
                                                   qApp->Settings()->GetOsSeparator()), prefix)
                .arg(tempAngle)
                .arg(tempRoationAngle);
    }

    CreateMovedRotatedObjects(iPoint, iCurve, tempLength, tempAngle, tempRoationAngle, origin);
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolMove::Angle() const
{
    return QString::number(line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolMove::SetAngle(const QString &expression)
{
    angle = FindValFromUser(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolMove::RotationAngle() const
{
    return QString::number(rotationLine->line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolMove::SetRotationAngle(const QString &expression)
{
    rotationAngle = FindValFromUser(expression, Visualization::data->DataVariables());
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
    length = FindLengthFromUser(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolMove::SetRotationOriginPointId(quint32 value)
{
    object1Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
QGraphicsPathItem *VisToolMove::AddOriginCurve(quint32 id, int &i)
{
    const QSharedPointer<Item> curve = Visualization::data->template GeometricObject<Item>(id);

    ++i;
    VCurvePathItem *path = GetCurve(static_cast<quint32>(i), supportColor2);
    DrawPath(path, curve->GetPath(), curve->DirectionArrows(), supportColor2, Qt::SolidLine, Qt::RoundCap);

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
int VisToolMove::AddMovedRotatedCurve(qreal angle, qreal length, quint32 id, int i, qreal rotationAngle,
                                      const QPointF &rotationOrigin)
{
    const QSharedPointer<Item> curve = Visualization::data->template GeometricObject<Item>(id);

    ++i;
    VCurvePathItem *path = GetCurve(static_cast<quint32>(i), supportColor);
    const Item moved = curve->Move(length, angle).Rotate(rotationOrigin, rotationAngle);
    DrawPath(path, moved.GetPath(), moved.DirectionArrows(), supportColor, Qt::SolidLine, Qt::RoundCap);

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
        Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 8, "Not all objects were handled.");

        switch(static_cast<GOType>(obj->getType()))
        {
            case GOType::Point:
            {
                const QSharedPointer<VPointF> p = Visualization::data->GeometricObject<VPointF>(id);

                ++iPoint;
                VScaledEllipse *point = GetPoint(static_cast<quint32>(iPoint), supportColor2);
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
            case GOType::PlaceLabel:
                Q_UNREACHABLE();
                break;
        }
    }

    return originObjects;
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")

void VisToolMove::CreateMovedRotatedObjects(int &iPoint, int &iCurve, qreal length, qreal angle, qreal rotationAngle,
                                            const QPointF &rotationOrigin)
{
    for (int i = 0; i < objects.size(); ++i)
    {
        const quint32 id = objects.at(i);
        const QSharedPointer<VGObject> obj = Visualization::data->GetGObject(id);

        // This check helps to find missed objects in the switch
        Q_STATIC_ASSERT_X(static_cast<int>(GOType::Unknown) == 8, "Not all objects was handled.");

        switch(static_cast<GOType>(obj->getType()))
        {
            case GOType::Point:
            {
                const QSharedPointer<VPointF> p = Visualization::data->GeometricObject<VPointF>(id);

                ++iPoint;
                VScaledEllipse *point = GetPoint(static_cast<quint32>(iPoint), supportColor);
                DrawPoint(point, static_cast<QPointF>(p->Move(length, angle).Rotate(rotationOrigin, rotationAngle)),
                          supportColor);
                break;
            }
            case GOType::Arc:
                iCurve = AddMovedRotatedCurve<VArc>(angle, length, id, iCurve, rotationAngle, rotationOrigin);
                break;
            case GOType::EllipticalArc:
                iCurve = AddMovedRotatedCurve<VEllipticalArc>(angle, length, id, iCurve, rotationAngle, rotationOrigin);
                break;
            case GOType::Spline:
                iCurve = AddMovedRotatedCurve<VSpline>(angle, length, id, iCurve, rotationAngle, rotationOrigin);
                break;
            case GOType::SplinePath:
                iCurve = AddMovedRotatedCurve<VSplinePath>(angle, length, id, iCurve, rotationAngle, rotationOrigin);
                break;
            case GOType::CubicBezier:
                iCurve = AddMovedRotatedCurve<VCubicBezier>(angle, length, id, iCurve, rotationAngle, rotationOrigin);
                break;
            case GOType::CubicBezierPath:
                iCurve = AddMovedRotatedCurve<VCubicBezierPath>(angle, length, id, iCurve, rotationAngle,
                                                                rotationOrigin);
                break;
            case GOType::Unknown:
            case GOType::PlaceLabel:
                Q_UNREACHABLE();
                break;
        }
    }
}

QT_WARNING_POP
