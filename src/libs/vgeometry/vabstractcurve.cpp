/************************************************************************
 **
 **  @file   vabstractcurve.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vabstractcurve.h"

#include <QPainterPath>
#include <QDebug>

VAbstractCurve::VAbstractCurve(const GOType &type, const quint32 &idObject, const Draw &mode)
    :VGObject(type, idObject, mode)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCurve::VAbstractCurve(const VAbstractCurve &curve)
    :VGObject(curve)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCurve &VAbstractCurve::operator=(const VAbstractCurve &curve)
{
    if ( &curve == this )
    {
        return *this;
    }
    VGObject::operator=(curve);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractCurve::GetSegmentPoints(const QPointF &begin, const QPointF &end, bool reverse) const
{
    QVector<QPointF> points = GetPoints();
    if (reverse)
    {
        points = GetReversePoints(points);
    }
    points = FromBegin(points, begin);
    points = ToEnd(points, end);
    return points;
}


//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractCurve::FromBegin(const QVector<QPointF> &points, const QPointF &begin)
{
    if (points.count() >= 2)
    {
        QVector<QPointF> segment;
        bool theBegin = false;
        for (qint32 i = 0; i < points.count()-1; ++i)
        {
            if (theBegin == false)
            {
                if (IsPointOnLineSegment(begin, points.at(i), points.at(i+1)))
                {
                    theBegin = true;
                    segment.append(begin);
                    if (i == points.count()-2)
                    {
                         segment.append(points.at(i+1));
                    }
                    continue;
                }
            }
            else
            {
                segment.append(points.at(i));
                if (i == points.count()-2)
                {
                     segment.append(points.at(i+1));
                }
            }
        }

        if (segment.isEmpty())
        {
            return points;
        }
        else
        {
            return segment;
        }
    }
    else
    {
        return points;
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractCurve::ToEnd(const QVector<QPointF> &points, const QPointF &end)
{
    QVector<QPointF> reversed = GetReversePoints(points);
    reversed = FromBegin(reversed, end);
    return GetReversePoints(reversed);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VAbstractCurve::GetPath(PathDirection direction) const
{
    QPainterPath path;

    QVector<QPointF> points = GetPoints();
    if (points.count() >= 2)
    {
        for (qint32 i = 0; i < points.count()-1; ++i)
        {
            path.moveTo(points.at(i));
            path.lineTo(points.at(i+1));
        }

        if (direction == PathDirection::Show && points.count() >= 3)
        {
            path.addPath(ShowDirection(points));
        }
    }
    else
    {
        qDebug()<<"points.count() < 2"<<Q_FUNC_INFO;
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IntersectLine return list of points for real intersection with line
 * @param line line that intersect with curve
 * @return list of intersection points
 */
QVector<QPointF> VAbstractCurve::IntersectLine(const QLineF &line) const
{
    QVector<QPointF> points = this->GetPoints();
    QVector<QPointF> intersections;
    for ( qint32 i = 0; i < points.count()-1; ++i )
    {
        QPointF crosPoint;
        QLineF::IntersectType type = line.intersect(QLineF ( points.at(i), points.at(i+1)), &crosPoint);
        if ( type == QLineF::BoundedIntersection )
        {
            intersections.append(crosPoint);
        }
    }
    return intersections;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractCurve::IsIntersectLine(const QLineF &line) const
{
    const QVector<QPointF> points = IntersectLine(line);
    return not points.isEmpty();
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VAbstractCurve::ShowDirection(const QVector<QPointF> &points) const
{
    QPainterPath path;

    if (points.count() >= 2)
    {
        /*Need find coordinate midle of curve.
          Universal way is take all points and find sum.*/
        const qreal seek_length = qAbs(GetLength())/2.0;
        qreal found_length = 0;
        QLineF arrow;
        for (qint32 i = 1; i <= points.size()-1; ++i)
        {
            arrow = QLineF(points.at(i-1), points.at(i));
            found_length += arrow.length();//Length that we aready find

            if (seek_length <= found_length)// if have found more that need stop.
            {
                //subtract length in last line and you will find position of the middle point.
                arrow.setLength(arrow.length() - (found_length - seek_length));
                break;
            }
        }

        //Reverse line because we want start arrow from this point
        arrow = QLineF(arrow.p2(), arrow.p1());
        const qreal angle = arrow.angle();//we each time change line angle, better save original angle value
        arrow.setLength(14);//arrow length in pixels

        arrow.setAngle(angle-35);
        path.moveTo(arrow.p1());
        path.lineTo(arrow.p2());

        arrow.setAngle(angle+35);
        path.moveTo(arrow.p1());
        path.lineTo(arrow.p2());
    }
    return path;
}
