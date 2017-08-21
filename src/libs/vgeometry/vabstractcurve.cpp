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

#include <QLine>
#include <QLineF>
#include <QMessageLogger>
#include <QPainterPath>
#include <QPoint>
#include <QtDebug>

#include "vabstractcurve_p.h"

const qreal VAbstractCurve::lengthCurveDirectionArrow = 14;

VAbstractCurve::VAbstractCurve(const GOType &type, const quint32 &idObject, const Draw &mode)
    :VGObject(type, idObject, mode), d (new VAbstractCurveData())
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCurve::VAbstractCurve(const VAbstractCurve &curve)
    :VGObject(curve), d (curve.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCurve &VAbstractCurve::operator=(const VAbstractCurve &curve)
{
    if ( &curve == this )
    {
        return *this;
    }
    VGObject::operator=(curve);
    d = curve.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractCurve::~VAbstractCurve()
{}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractCurve::GetSegmentPoints(const QVector<QPointF> &points, const QPointF &begin,
                                                  const QPointF &end, bool reverse)
{
    QVector<QPointF> segment = points;
    if (reverse)
    {
        segment = GetReversePoints(segment);
    }

    QPointF start = begin;
    QPointF finish = end;

    if (begin == end)
    {
        start = segment.first();
        finish = segment.last();
    }

    segment = FromBegin(segment, start);
    segment = ToEnd(segment, finish);
    return segment;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractCurve::GetSegmentPoints(const QPointF &begin, const QPointF &end, bool reverse) const
{
    return GetSegmentPoints(GetPoints(), begin, end, reverse);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractCurve::FromBegin(const QVector<QPointF> &points, const QPointF &begin, bool *ok)
{
    if (points.count() >= 2)
    {
        if (points.first().toPoint() == begin.toPoint())
        {
            if (ok != nullptr)
            {
                *ok = true;
            }
            return points;
        }

        QVector<QPointF> segment;
        bool theBegin = false;
        for (qint32 i = 0; i < points.count()-1; ++i)
        {
            if (theBegin == false)
            {
                if (IsPointOnLineSegment(begin, points.at(i), points.at(i+1)))
                {
                    theBegin = true;

                    if (begin != points.at(i+1))
                    {
                        segment.append(begin);
                    }

                    if (i == points.count()-2)
                    {
                         segment.append(points.at(i+1));
                    }
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
            if (ok != nullptr)
            {
                *ok = false;
            }
            return points;
        }
        else
        {
            if (ok != nullptr)
            {
                *ok = true;
            }
            return segment;
        }
    }
    else
    {
        if (ok != nullptr)
        {
            *ok = false;
        }
        return points;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractCurve::ToEnd(const QVector<QPointF> &points, const QPointF &end, bool *ok)
{
    QVector<QPointF> reversed = GetReversePoints(points);
    reversed = FromBegin(reversed, end, ok);
    return GetReversePoints(reversed);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VAbstractCurve::GetPath() const
{
    QPainterPath path;

    const QVector<QPointF> points = GetPoints();
    if (points.count() >= 2)
    {
        path.addPolygon(QPolygonF(points));
    }
    else
    {
        qDebug()<<"points.count() < 2"<<Q_FUNC_INFO;
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractCurve::GetLengthByPoint(const QPointF &point) const
{
    const QVector<QPointF> points = GetPoints();
    if (points.size() < 2)
    {
        return -1;
    }

    if (points.first().toPoint() == point.toPoint())
    {
        return 0;
    }

    bool ok = false;
    const QVector<QPointF> segment = ToEnd(points, point, &ok);
    if (not ok)
    {
        return -1;
    }
    return PathLength(segment);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IntersectLine return list of points for real intersection with line
 * @param line line that intersect with curve
 * @return list of intersection points
 */
QVector<QPointF> VAbstractCurve::IntersectLine(const QLineF &line) const
{
    return CurveIntersectLine(this->GetPoints(), line);
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractCurve::IsIntersectLine(const QLineF &line) const
{
    const QVector<QPointF> points = IntersectLine(line);
    return not points.isEmpty();
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractCurve::IsPointOnCurve(const QVector<QPointF> &points, const QPointF &p)
{
    if (points.isEmpty())
    {
        return false;
    }
    else if (points.size() < 2)
    {
        return points.at(0) == p;
    }
    else
    {
        for (qint32 i = 0; i < points.count()-1; ++i)
        {
            if (IsPointOnLineSegment(p, points.at(i), points.at(i+1)))
            {
                return true;
            }
        }
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractCurve::IsPointOnCurve(const QPointF &p) const
{
    return IsPointOnCurve(GetPoints(), p);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VAbstractCurve::GetDuplicate() const
{
    return d->duplicate;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractCurve::SetDuplicate(quint32 number)
{
    d->duplicate = number;
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractCurve::GetColor() const
{
    return d->color;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractCurve::SetColor(const QString &color)
{
    d->color = color;
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractCurve::GetPenStyle() const
{
    return d->penStyle;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractCurve::SetPenStyle(const QString &penStyle)
{
    d->penStyle = penStyle;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractCurve::CurveIntersectLine(const QVector<QPointF> &points, const QLineF &line)
{
    QVector<QPointF> intersections;
    for ( auto i = 0; i < points.count()-1; ++i )
    {
        QPointF crosPoint;
        const auto type = line.intersect(QLineF(points.at(i), points.at(i+1)), &crosPoint);
        if ( type == QLineF::BoundedIntersection )
        {
            intersections.append(crosPoint);
        }
    }
    return intersections;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<DirectionArrow> VAbstractCurve::DirectionArrows() const
{
    QVector<DirectionArrow> arrows;

    const QVector<QPointF> points = GetPoints();
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
        arrow.setLength(lengthCurveDirectionArrow);//arrow length in pixels

        DirectionArrow dArrow;

        arrow.setAngle(angle-35);
        dArrow.first = arrow;

        arrow.setAngle(angle+35);
        dArrow.second = arrow;

        arrows.append(dArrow);
    }
    return arrows;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VAbstractCurve::ShowDirection(const QVector<DirectionArrow> &arrows, qreal width)
{
    QPainterPath path;

    for (int i = 0; i < arrows.size(); ++i)
    {
        const DirectionArrow arrow = arrows.at(i);
        if (not arrow.first.isNull() && not arrow.second.isNull())
        {
            QPainterPath arrowPath;

            QLineF line = arrow.first;
            line.setLength(width);
            arrowPath.moveTo(line.p1());
            arrowPath.lineTo(line.p2());

            line = arrow.second;
            line.setLength(width);
            arrowPath.moveTo(line.p1());
            arrowPath.lineTo(line.p2());

            path.addPath(arrowPath);
        }
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractCurve::PathLength(const QVector<QPointF> &path)
{
    if (path.size() < 2)
    {
        return 0;
    }

    QPainterPath splinePath;
    splinePath.moveTo(path.at(0));
    for (qint32 i = 1; i < path.count(); ++i)
    {
        splinePath.lineTo(path.at(i));
    }
    return splinePath.length();
}
