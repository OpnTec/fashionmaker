/************************************************************************
 **
 **  @file   vabstractcubicbezierpath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 3, 2016
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

#include "vabstractcubicbezierpath.h"
#include "../ifc/exception/vexception.h"
#include "vspline.h"

#include <QPainterPath>

//---------------------------------------------------------------------------------------------------------------------
VAbstractCubicBezierPath::VAbstractCubicBezierPath(const GOType &type, const quint32 &idObject, const Draw &mode)
    : VAbstractCurve(type, idObject, mode)
{
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCubicBezierPath::VAbstractCubicBezierPath(const VAbstractCubicBezierPath &curve)
    : VAbstractCurve(curve)
{
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCubicBezierPath &VAbstractCubicBezierPath::operator=(const VAbstractCubicBezierPath &curve)
{
    if ( &curve == this )
    {
        return *this;
    }
    VAbstractCurve::operator=(curve);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCubicBezierPath::~VAbstractCubicBezierPath()
{
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPath return QPainterPath which reprezent spline path.
 * @return path.
 */
QPainterPath VAbstractCubicBezierPath::GetPath(PathDirection direction) const
{
    QPainterPath painterPath;
    for (qint32 i = 1; i <= CountSubSpl(); ++i)
    {
        painterPath.addPath(GetSpline(i).GetPath(direction));
    }
    return painterPath;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPathPoints return list of points what located on path.
 * @return list.
 */
QVector<QPointF> VAbstractCubicBezierPath::GetPoints() const
{
    QVector<QPointF> pathPoints;
    for (qint32 i = 1; i <= CountSubSpl(); ++i)
    {
        pathPoints += GetSpline(i).GetPoints();
    }
    return pathPoints;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLength return length of spline path.
 * @return length.
 */
qreal VAbstractCubicBezierPath::GetLength() const
{
    qreal length = 0;
    for (qint32 i = 1; i <= CountSubSpl(); ++i)
    {
        length += GetSpline(i).GetLength();
    }
    return length;
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractCubicBezierPath::Segment(const QPointF &p) const
{
    int index = -1;
    for (qint32 i = 1; i <= CountSubSpl(); ++i)
    {
        const qreal t = GetSpline(i).ParamT(p);
        if (not qFuzzyIsNull(t) && qFuzzyCompare(t, -1))
        {
            continue;
        }
        else
        {
            index = i;
            break;
        }
    }
    return index;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CutSplinePath cut spline path into two. This method don't return two spline path. You must create spline
 * paths by yourself.
 * Example:
 * QPointF spl1p2, spl1p3, spl2p2, spl2p3;
 * qint32 p1 = 0, p2 = 0;
 * QPointF point = splPath->CutSplinePath(length, p1, p2, spl1p2, spl1p3, spl2p2, spl2p3);
 *
 * VSplinePoint splP1 = splPath->at(p1);
 * VSplinePoint splP2 = splPath->at(p2);
 * VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, *p, splPath->GetKCurve());
 * VSpline spl2 = VSpline(*p, spl2p2, spl2p3, splP2.P(), splPath->GetKCurve());
 * @param length length first spline path.
 * @param p1 index first spline point in list.
 * @param p2 index second spline point in list.
 * @param spl1p2 first control point first spline.
 * @param spl1p3 second control point first spline.
 * @param spl2p2 first control point second spline.
 * @param spl2p3 second control point second spline.
 * @return cutting point.
 */
QPointF VAbstractCubicBezierPath::CutSplinePath(qreal length, qint32 &p1, qint32 &p2, QPointF &spl1p2, QPointF &spl1p3,
                                                QPointF &spl2p2, QPointF &spl2p3) const
{
    if (CountSubSpl() < 1)
    {
        throw VException(tr("Can't cut this spline"));
    }

    //Always need return two spline paths, so we must correct wrong length.
    qreal fullLength = GetLength();
    if (length < fullLength * 0.02)
    {
        length = fullLength * 0.02;
    }
    else if ( length > fullLength * 0.98)
    {
        length = fullLength * 0.98;
    }

    fullLength = 0;
    for (qint32 i = 1; i <= CountSubSpl(); ++i)
    {
        const VSpline spl = GetSpline(i);
        fullLength += spl.GetLength();
        if (fullLength > length)
        {
            p1 = i-1;
            p2 = i;
            return spl.CutSpline(length - (fullLength - spl.GetLength()), spl1p2, spl1p3, spl2p2, spl2p3);
        }
    }
    return QPointF();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NameForHistory helps to create name for dialog History.
 * @param toolName first part of name. Like 'Spline path' or 'Cubic Bezier path'.
 * @return name of curve for history records.
 */
QString VAbstractCubicBezierPath::NameForHistory(const QString &toolName) const
{
    QString name = toolName;
    if (CountPoints() > 0)
    {
        name += QString(" %1").arg(FirstPoint().name());
        if (CountSubSpl() >= 1)
        {
            name += QString("_%1").arg(LastPoint().name());
        }

        if (GetDuplicate() > 0)
        {
            name += QString("_%1").arg(GetDuplicate());
        }
    }
    return name;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractCubicBezierPath::CreateName()
{
    QString name;
    if (CountPoints() > 0)
    {
        name = splPath;
        name.append(QString("_%1").arg(FirstPoint().name()));
        if (CountSubSpl() >= 1)
        {
            name.append(QString("_%1").arg(LastPoint().name()));
        }

        if (GetDuplicate() > 0)
        {
            name += QString("_%1").arg(GetDuplicate());
        }
    }
    setName(name);
}
