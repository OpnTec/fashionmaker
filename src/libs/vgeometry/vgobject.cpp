/************************************************************************
 **
 **  @file   vgobject.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   27 12, 2013
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

#include "vgobject.h"
#include "vgobject_p.h"

#include <QLineF>
#include <QPointF>
#include <QRectF>
#include <QtCore/qmath.h>
#include <climits>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGObject default constructor.
 */
VGObject::VGObject()
    :d(new VGObjectData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGObject constructor.
 * @param type type graphical object.
 * @param idObject id parent object.
 * @param mode mode creation. Used in modeling mode.
 */
VGObject::VGObject(const GOType &type, const quint32 &idObject, const Draw &mode)
    :d(new VGObjectData(type, idObject, mode))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGObject copy constructor.
 * @param obj object.
 */
VGObject::VGObject(const VGObject &obj)
    :d (obj.d)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator.
 * @param obj object
 * @return object
 */
VGObject &VGObject::operator=(const VGObject &obj)
{
    if ( &obj == this )
    {
        return *this;
    }
    d = obj.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VGObject::~VGObject()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getIdObject return parent id.
 * @return parent id or 0 if object don't have parent.
 */
quint32 VGObject::getIdObject() const
{
    return d->idObject;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setIdObject set parent id.
 * @param value parent id.
 */
void VGObject::setIdObject(const quint32 &value)
{
    d->idObject = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief name return name graphical object.
 * @return name
 */
QString VGObject::name() const
{
    return d->_name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setName set name graphical object.
 * @param name name graphical object.
 */
void VGObject::setName(const QString &name)
{
    d->_name = name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getMode return mode creation.
 * @return mode.
 */
Draw VGObject::getMode() const
{
    return d->mode;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMode set mode creation.
 * @param value mode.
 */
void VGObject::setMode(const Draw &value)
{
    d->mode = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getType return object type.
 * @return type.
 */
GOType VGObject::getType() const
{
    return d->type;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VGObject::setType(const GOType &type)
{
    d->type = type;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief id return id object.
 * @return id
 */
quint32 VGObject::id() const
{
    return d->_id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setId set id object.
 * @param id id.
 */
void VGObject::setId(const quint32 &id)
{
    d->_id = id;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VGObject::BuildLine(const QPointF &p1, const qreal &length, const qreal &angle)
{
    QLineF line = QLineF();
    line.setP1(p1);
    line.setAngle(angle);// First set angle than length. Length can have negative value.
    line.setLength(length);
    return line;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VGObject::BuildRay(const QPointF &firstPoint, const qreal &angle, const QRectF &scRect)
{
    QRectF rect = scRect;
    if (rect.isValid() == false)
    {
        rect = QRectF(0, 0, 1200, 700);
    }
    if (rect.contains(firstPoint) == false)
    {
        // If point outside of scene rect create one around point and unite two rects.
        QRectF rectangle(firstPoint.x()-rect.width()/2, firstPoint.y()-rect.height()/2, rect.width(), rect.height());
        rect = rect.united(rectangle);
    }
    const qreal diagonal = qSqrt(pow(rect.height(), 2) + pow(rect.width(), 2));
    const QLineF line = BuildLine(firstPoint, diagonal, angle);

    return LineIntersectRect(rect, line);
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VGObject::BuildAxis(const QPointF &p, const qreal &angle, const QRectF &scRect)
{
    const QPointF endP1 = BuildRay(p, angle+180, scRect);
    const QPointF endP2 = BuildRay(p, angle, scRect);
    return QLineF(endP1, endP2);
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VGObject::BuildAxis(const QPointF &p1, const QPointF &p2, const QRectF &scRect)
{
    QLineF line(p1, p2);
    return BuildAxis(p1, line.angle(), scRect);
}

//---------------------------------------------------------------------------------------------------------------------
int VGObject::ContactPoints(const QPointF &p, const QPointF &center, qreal radius, QPointF &p1, QPointF &p2)
{
    const int flag = PointInCircle(p, center, radius);

    if (flag == 0)
    {
        return 0;
    }

    if (flag == 1)
    {
        p1 = p;
        return 1;
    }

    const double d = QLineF (p, center).length();
    const double k = sqrt (d * d - radius * radius);
    return IntersectionCircles(p, k, center, radius, p1, p2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LineIntersectRect find point intersection line and rect.
 * @param rec rect.
 * @param line line.
 * @return point intersection.
 */
QPointF VGObject::LineIntersectRect(const QRectF &rec, const QLineF &line)
{
    qreal x1, y1, x2, y2;
    rec.getCoords(&x1, &y1, &x2, &y2);
    QPointF point;
    QLineF::IntersectType type = line.intersect(QLineF(QPointF(x1, y1), QPointF(x1, y2)), &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    type = line.intersect(QLineF(QPointF(x1, y1), QPointF(x2, y1)), &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    type = line.intersect(QLineF(QPointF(x1, y2), QPointF(x2, y2)), &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    type = line.intersect(QLineF(QPointF(x2, y1), QPointF(x2, y2)), &point);
    if ( type == QLineF::BoundedIntersection )
    {
        return point;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
int VGObject::IntersectionCircles(const QPointF &c1, double r1, const QPointF &c2, double r2, QPointF &p1, QPointF &p2)
{
    if (qFuzzyCompare(c1.x(), c2.x()) && qFuzzyCompare(c1.y(), c2.y()) && qFuzzyCompare(r1, r2))
    {
        return 3;// Circles are equal
    }
    const double a = - 2.0 * (c2.x() - c1.x());
    const double b = - 2.0 * (c2.y() - c1.y());
    const double c = (c2.x() - c1.x())* (c2.x() - c1.x()) + (c2.y() - c1.y()) * (c2.y() - c1.y()) + r1 * r1 - r2 * r2;

    const double x0 = -a*c/(a*a+b*b);
    const double y0 = -b*c/(a*a+b*b);

    if (c*c > r1*r1*(a*a+b*b))
    {
        return 0;
    }
    else if (qFuzzyCompare(c*c, r1*r1*(a*a+b*b)))
    {
        p1 = QPointF(x0 + c1.x(), y0  + c1.y());
        return 1;
    }
    else
    {
        const double d = r1*r1 - c*c/(a*a+b*b);
        const double mult = sqrt (d / (a*a+b*b));

        const double ax = x0 + b * mult;
        const double bx = x0 - b * mult;
        const double ay = y0 - a * mult;
        const double by = y0 + a * mult;

        p1 = QPointF(ax + c1.x(), ay + c1.y());
        p2 = QPointF(bx + c1.x(), by + c1.y());
        return 2;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LineIntersectCircle find point intersection line and circle.
 * @param center arc center.
 * @param radius arc radius.
 * @param line line
 * @param p1 first intersection point.
 * @param p2 second intersection point.
 * @return 0 - intersection doesn't exist, 1 - one intersection point, 2 - two intersection points.
 */
qint32 VGObject::LineIntersectCircle(const QPointF &center, qreal radius, const QLineF &line, QPointF &p1, QPointF &p2)
{
    //coefficient for equation of segment
    qreal a = 0, b = 0, c = 0;
    LineCoefficients(line, &a, &b, &c);
    // projection center of circle on to line
    const QPointF p = ClosestPoint (line, center);
    // how many solutions?
    qint32 flag = 0;
    const qreal d = QLineF (center, p).length();
    if (qFuzzyCompare(d, radius))
    {
        flag = 1;
    }
    else
    {
        if (radius > d)
        {
            flag = 2;
        }
        else
        {
            return 0;
        }
    }
    // find distance from projection to points of intersection
    const qreal k = qSqrt (qAbs(radius * radius - d * d));
    const qreal t = QLineF (QPointF (0, 0), QPointF (b, - a)).length();
    // add to projection a vectors aimed to points of intersection
    p1 = addVector (p, QPointF (0, 0), QPointF (- b, a), k / t);
    p2 = addVector (p, QPointF (0, 0), QPointF (b, - a), k / t);
    return flag;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosestPoint find point projection of point onto line.
 * @param line line.
 * @return point on line or extended line if origin size too small.
 */
QPointF VGObject::ClosestPoint(const QLineF &line, const QPointF &point)
{
    qreal a = 0, b = 0, c = 0;
    LineCoefficients(line, &a, &b, &c);
    qreal x = point.x() + a;
    qreal y = b + point.y();
    QLineF lin (point, QPointF(x, y));
    QPointF p;
    QLineF::IntersectType intersect = line.intersect(lin, &p);
    if (intersect == QLineF::UnboundedIntersection || intersect == QLineF::BoundedIntersection)
    {
        return p;
    }
    else
    {
        return QPointF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VGObject::addVector(const QPointF &p, const QPointF &p1, const QPointF &p2, qreal k)
{
    return QPointF (p.x() + (p2.x() - p1.x()) * k, p.y() + (p2.y() - p1.y()) * k);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LineCoefficients coefficient for equation of segment. Segment equestion ax+by+c=0.
 * @param line line
 * @param a a value
 * @param b b value
 * @param c c value
 */
void VGObject::LineCoefficients(const QLineF &line, qreal *a, qreal *b, qreal *c)
{
    //coefficient for equation of segment
    QPointF p1 = line.p1();
    *a = line.p2().y() - p1.y();
    *b = p1.x() - line.p2().x();
    *c = - *a * p1.x() - *b * p1.y();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IsPointOnLineSegment Check if the point is on the line segment.
 *
 * Original idea http://www.sunshine2k.de/coding/java/PointOnLine/PointOnLine.html
 */
bool VGObject::IsPointOnLineSegment(const QPointF &t, const QPointF &p1, const QPointF &p2)
{
    // The test point must lie inside the bounding box spanned by the two line points.
    if (not ( (p1.x() <= t.x() && t.x() <= p2.x()) || (p2.x() <= t.x() && t.x() <= p1.x()) ))
    {
        // test point not in x-range
        return false;
    }

    if (not ( (p1.y() <= t.y() && t.y() <= p2.y()) || (p2.y() <= t.y() && t.y() <= p1.y()) ))
    {
        // test point not in y-range
        return false;
    }

    // Test via the perp dot product (PDP)
    return IsPointOnLineviaPDP(t, p1, p2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IsPointOnLineviaPDP use the perp dot product (PDP) way.
 *
 *  The pdp is zero only if the t lies on the line e1 = vector from p1 to p2.
 */
bool VGObject::IsPointOnLineviaPDP(const QPointF &t, const QPointF &p1, const QPointF &p2)
{
    return ( qAbs(PerpDotProduct(p1, p2, t) < GetEpsilon(p1, p2)) );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PerpDotProduct Calculates the area of the parallelogram of the three points.
 * This is actually the same as the area of the triangle defined by the three points, multiplied by 2.
 * @return 2 * triangleArea(a,b,c)
 */
double VGObject::PerpDotProduct(const QPointF &t, const QPointF &p1, const QPointF &p2)
{
    return (p1.x() - t.x()) * (p2.y() - t.y()) - (p1.y() - t.y()) * (p2.x() - t.x());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetEpsilon solve the floating-point accuraccy problem.
 *
 * There is the floating-point accuraccy problem, so instead of checking against zero, some epsilon value has to be
 * used. Because the size of the pdp value depends on the length of the vectors, no static value can be used. One
 * approach is to compare the pdp/area value to the fraction of another area which also depends on the length of the
 * line e1=(p1, p2), e.g. the area of the square with side e1 which is computed below
 */
double VGObject::GetEpsilon(const QPointF &p1, const QPointF &p2)
{
    const int dx1 = p2.toPoint().x() - p1.toPoint().x();
    const int dy1 = p2.toPoint().y() - p1.toPoint().y();
    const double epsilon = 0.003 * (dx1 * dx1 + dy1 * dy1);
    return epsilon;
}

//---------------------------------------------------------------------------------------------------------------------
int VGObject::PointInCircle(const QPointF &p, const QPointF &center, qreal radius)
{
    const double d = QLineF (p, center).length();
    if (qFuzzyCompare(radius, d))
    {
        return 1; // on circle
    }
    if (radius > d)
    {
        return 0; // outside circle
    }
    return 2; // inside circle
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetReversePoint return revers container of points.
 * @param points container with points.
 * @return reverced points.
 */
QVector<QPointF> VGObject::GetReversePoints(const QVector<QPointF> &points)
{
    if (points.isEmpty())
    {
        return points;
    }
    QVector<QPointF> reversePoints;
    for (qint32 i = points.size() - 1; i >= 0; --i)
    {
        reversePoints.append(points.at(i));
    }
    return reversePoints;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetLengthContour return length of contour.
 * @param contour container with points of contour.
 * @param newPoints point whos we try to add to contour.
 * @return length length of contour.
 */
// cppcheck-suppress unusedFunction
int VGObject::GetLengthContour(const QVector<QPointF> &contour, const QVector<QPointF> &newPoints)
{
    qreal length = 0;
    QVector<QPointF> points;
    points << contour << newPoints;
    for (qint32 i = 0; i < points.size()-1; ++i)
    {
        QLineF line(points.at(i), points.at(i+1));
        length += line.length();
    }
    return qFloor(length);
}
