/************************************************************************
 **
 **  @file   vabstractdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
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

#include "vabstractdetail.h"
#include "vabstractdetail_p.h"

#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QDebug>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractDetail default contructor. Create empty detail.
 */
VAbstractDetail::VAbstractDetail()
    :d(new VAbstractDetailData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractDetail constructor.
 * @param name detail name.
 */
VAbstractDetail::VAbstractDetail(const QString &name)
    :d(new VAbstractDetailData(name))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractDetail copy constructor.
 * @param detail detail.
 */
VAbstractDetail::VAbstractDetail(const VAbstractDetail &detail)
    :d (detail.d)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator.
 * @param detail detail.
 * @return new detail.
 */
VAbstractDetail &VAbstractDetail::operator=(const VAbstractDetail &detail)
{
    if ( &detail == this )
    {
        return *this;
    }
    d = detail.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractDetail::~VAbstractDetail()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear detail full clear.
 */
void VAbstractDetail::Clear()
{
    d->name.clear();
    d->seamAllowance = false;
    d->closed = true;
    d->width = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getName return detail name.
 * @return name.
 */
QString VAbstractDetail::getName() const
{
    return d->name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setName set detail name.
 * @param value new name.
 */
void VAbstractDetail::setName(const QString &value)
{
    d->name = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSeamAllowance keep status for seam allowance detail.
 * @return true - need seam allowance, false - no need seam allowance.
 */
bool VAbstractDetail::getSeamAllowance() const
{
    return d->seamAllowance;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSeamAllowance set status for seam allowance detail.
 * @param value true - need seam allowance, false - no need seam allowance.
 */
void VAbstractDetail::setSeamAllowance(bool value)
{
    d->seamAllowance = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getClosed keep close status for detail equdistant.
 * @return true - close equdistant, false - don't close equdistant.
 */
bool VAbstractDetail::getClosed() const
{
    return d->closed;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setClosed set close status for detail equdistant.
 * @param value true - close equdistant, false - don't close equdistant.
 */
void VAbstractDetail::setClosed(bool value)
{
    d->closed = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getWidth return value detail seam allowance.
 * @return value in mm.
 */
qreal VAbstractDetail::getWidth() const
{
    return d->width;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setWidth set value detail seam allowance.
 * @param value width in mm.
 */
void VAbstractDetail::setWidth(const qreal &value)
{
    d->width = value;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractDetail::Equidistant(const QVector<QPointF> &points, const EquidistantType &eqv, qreal width)
{
    QVector<QPointF> ekvPoints;

    if (width <= 0)
    {
        qDebug()<<"Width <= 0.";
        return QVector<QPointF>();
    }

    QVector<QPointF> p = CorrectEquidistantPoints(points);
    if ( p.size() < 3 )
    {
        qDebug()<<"Not enough points for building the equidistant.";
        return QVector<QPointF>();
    }

    if (eqv == EquidistantType::CloseEquidistant)
    {
        p.append(p.at(0));
    }
    for (qint32 i = 0; i < p.size(); ++i )
    {
        if ( i == 0 && eqv == EquidistantType::CloseEquidistant)
        {//first point, polyline closed
            ekvPoints<<EkvPoint(QLineF(p.at(p.size()-2), p.at(p.size()-1)), QLineF(p.at(1), p.at(0)), width);
            continue;
        }
        else if (i == 0 && eqv == EquidistantType::OpenEquidistant)
        {//first point, polyline doesn't closed
            ekvPoints.append(UnclosedEkvPoint(QLineF(p.at(0), p.at(1)), QLineF(p.at(0), p.at(p.size()-1)), width));
            continue;
        }
        if (i == p.size()-1 && eqv == EquidistantType::CloseEquidistant)
        {//last point, polyline closed
            ekvPoints.append(ekvPoints.at(0));
            continue;
        }
        else if (i == p.size()-1 && eqv == EquidistantType::OpenEquidistant)
        {//last point, polyline doesn't closed
            ekvPoints.append(UnclosedEkvPoint(QLineF(p.at(p.size()-2), p.at(p.size()-1)),
                                              QLineF(p.at(0), p.at(p.size()-1)), width));
            continue;
        }
        //points in the middle of polyline
        ekvPoints<<EkvPoint(QLineF(p.at(i-1), p.at(i)), QLineF(p.at(i+1), p.at(i)), width);
    }
    ekvPoints = CheckLoops(ekvPoints);//Result path can contain loops
    return ekvPoints;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractDetail::RemoveDublicates(const QVector<QPointF> &points)
{
    QVector<QPointF> p = points;

    if (not p.isEmpty() && p.size() > 1)
    {
        // Path can't be closed
        if (p.first() == p.last())
        {
        #if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
            p.remove(p.size() - 1);
        #else
            p.removeLast();
        #endif
        }
    }

    for (int i = 0; i < p.size()-1; ++i)
    {
        if (p.at(i) == p.at(i+1))
        {
            p.erase(p.begin() + i + 1);
            --i;
            continue;
        }
    }

    return p;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CorrectEquidistantPoints clear equivalent points and remove point on line from equdistant.
 * @param points list of points equdistant.
 * @return corrected list.
 */
QVector<QPointF> VAbstractDetail::CorrectEquidistantPoints(const QVector<QPointF> &points)
{
    QVector<QPointF> correctPoints;
    if (points.size()<4)//Better don't check if only three points. We can destroy equidistant.
    {
        qDebug()<<"Only three points.";
        return points;
    }

    //Clear equivalent points
    correctPoints = RemoveDublicates(points);

    if (correctPoints.size()<3)
    {
        return correctPoints;
    }
    //Remove point on line
    QPointF point;
    for (qint32 i = 1; i <correctPoints.size()-1; ++i)
    {
        QLineF l1(correctPoints.at(i-1), correctPoints.at(i));
        QLineF l2(correctPoints.at(i), correctPoints.at(i+1));
        QLineF::IntersectType intersect = l1.intersect(l2, &point);
        if (intersect == QLineF::NoIntersection)
        {
            correctPoints.remove(i);
        }
    }
    return correctPoints;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckLoops seek and delete loops in equidistant.
 * @param points vector of points of equidistant.
 * @return vector of points of equidistant.
 */
QVector<QPointF> VAbstractDetail::CheckLoops(const QVector<QPointF> &points)
{
    const int count = points.size();
    /*If we got less than 4 points no need seek loops.*/
    if (count < 4)
    {
        qDebug()<<"Less then 4 points. Doesn't need check for loops.";
        return points;
    }

    bool closed = false;
    if (points.first() == points.last())
    {
        closed = true;
    }

    QVector<QPointF> ekvPoints;

    qint32 i, j;
    for (i = 0; i < count; ++i)
    {
        /*Last three points no need check.*/
        /*Triangle has not contain loops*/
        if (i >= count-3)
        {
            ekvPoints.append(points.at(i));
            continue;
        }

        QPointF crosPoint;
        QLineF::IntersectType intersect = QLineF::NoIntersection;
        const QLineF line1(points.at(i), points.at(i+1));
        // Because a path can contains several loops we will seek the last and only then remove the loop(s)
        // That's why we parse from the end
        for (j = count-2; j >= i+2; --j)
        {
            const QLineF line2(points.at(j), points.at(j+1));
            intersect = line1.intersect(line2, &crosPoint);
            if (intersect == QLineF::BoundedIntersection && not (i == 0 && j+1 == count-1 && closed))
            { // Break, but not if intersects the first edge and the last edge in closed path
                if (line1.p1() != crosPoint && line1.p2() != crosPoint &&
                    line2.p1() != crosPoint && line2.p2() != crosPoint)
                { // Break, but not if loop creates crosPoint when it is first or last point of lines
                    break;
                }
            }
            intersect = QLineF::NoIntersection;
        }

        if (intersect == QLineF::BoundedIntersection)
        {
            /*We have found loop.*/
            ekvPoints.append(points.at(i));
            ekvPoints.append(crosPoint);
            i = j;
        }
        else
        {
            /*We have not found loop.*/
            ekvPoints.append(points.at(i));
        }
    }
    return ekvPoints;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EkvPoint return vector of points of equidistant two lines. Last point of two lines must be equal.
 * @param line1 first line.
 * @param line2 second line.
 * @param width width of equidistant.
 * @return vector of points.
 */
QVector<QPointF> VAbstractDetail::EkvPoint(const QLineF &line1, const QLineF &line2, const qreal &width)
{
    if (width <= 0)
    {
        return QVector<QPointF>();
    }
    QVector<QPointF> points;
    if (line1.p2() != line2.p2())
    {
        qDebug()<<"Last points of two lines must be equal.";
        return QVector<QPointF>();
    }
    QPointF CrosPoint;
    QLineF bigLine1 = ParallelLine(line1, width );
    QLineF bigLine2 = ParallelLine(QLineF(line2.p2(), line2.p1()), width );
    QLineF::IntersectType type = bigLine1.intersect( bigLine2, &CrosPoint );
    switch (type)
    {
        case (QLineF::BoundedIntersection):
            points.append(CrosPoint);
            return points;
            break;
        case (QLineF::UnboundedIntersection):
        {
                QLineF line( line1.p2(), CrosPoint );
                const qreal length = line.length();
                if (length > width*2.4)
                { // Cutting too long an acute angle
                    line.setLength(width); // Not sure about width value here
                    QLineF cutLine(line.p2(), CrosPoint); // Cut line is a perpendicular
                    cutLine.setLength(length); // Decided take this length

                    // We do not check intersection type because intersection must alwayse exist
                    QPointF px;
                    cutLine.setAngle(cutLine.angle()+90);
                    QLineF::IntersectType type = bigLine1.intersect( cutLine, &px );
                    if (type == QLineF::NoIntersection)
                    {
                        qDebug()<<"Couldn't find intersection with cut line.";
                    }
                    points.append(px);

                    cutLine.setAngle(cutLine.angle()-180);
                    type = bigLine2.intersect( cutLine, &px );
                    if (type == QLineF::NoIntersection)
                    {
                        qDebug()<<"Couldn't find intersection with cut line.";
                    }
                    points.append(px);
                }
                else
                {
                    points.append(CrosPoint);
                    return points;
                }
            break;
        }
        case (QLineF::NoIntersection):
            /*If we have correct lines this means lines lie on a line.*/
            points.append(bigLine1.p2());
            return points;
            break;
        default:
            break;
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UnclosedEkvPoint helps find point of an unclosed seam allowance. One side of two lines should be equal.
 *
 * In case of the first seam allowance point equal should be the first point of the two lines. In case the last point -
 * the last point of the two lines.
 *
 * @param line line of a seam allowance
 * @param helpLine help line of the main path that cut unclosed seam allowance
 * @param width seam allowance width
 * @return seam allowance point
 */
QPointF VAbstractDetail::UnclosedEkvPoint(const QLineF &line, const QLineF &helpLine, const qreal &width)
{
    if (width <= 0)
    {
        return QPointF();
    }

    if (not (line.p2() == helpLine.p2() || line.p1() == helpLine.p1()))
    {
        qDebug()<<"Two points of two lines must be equal.";
        return QPointF();
    }

    QPointF CrosPoint;
    const QLineF bigLine = ParallelLine(line, width );
    QLineF::IntersectType type = bigLine.intersect( helpLine, &CrosPoint );
    switch (type)
    {
        case (QLineF::BoundedIntersection):
        case (QLineF::UnboundedIntersection):
            return CrosPoint;
            break;
        case (QLineF::NoIntersection):
            /*If we have correct lines this means lines lie on a line.*/
            return bigLine.p2();
            break;
        default:
            break;
    }
    return QPointF();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ParallelLine create parallel line.
 * @param line starting line.
 * @param width width to parallel line.
 * @return parallel line.
 */
QLineF VAbstractDetail::ParallelLine(const QLineF &line, qreal width)
{
    QLineF paralel = QLineF (SingleParallelPoint(line, 90, width), SingleParallelPoint(QLineF(line.p2(), line.p1()),
                                                                                       -90, width));
    return paralel;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SingleParallelPoint return point of parallel line.
 * @param line starting line.
 * @param angle angle in degree.
 * @param width width to parallel line.
 * @return point of parallel line.
 */
QPointF VAbstractDetail::SingleParallelPoint(const QLineF &line, const qreal &angle, const qreal &width)
{
    QLineF pLine = line;
    pLine.setAngle( pLine.angle() + angle );
    pLine.setLength( width );
    return pLine.p2();
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractDetail::SumTrapezoids(const QVector<QPointF> &points)
{
    // Calculation a polygon area through the sum of the areas of trapezoids
    qreal s, res = 0;
    const int n = points.size();

    if(n > 2)
    {
        for (int i = 0; i < n; ++i)
        {
            if (i == 0)
            {
                s = points.at(i).x()*(points.at(n-1).y() - points.at(i+1).y()); //if i == 0, then y[i-1] replace on y[n-1]
                res += s;
            }
            else
            {
                if (i == n-1)
                {
                    s = points.at(i).x()*(points.at(i-1).y() - points.at(0).y()); // if i == n-1, then y[i+1] replace on y[0]
                    res += s;
                }
                else
                {
                    s = points.at(i).x()*(points.at(i-1).y() - points.at(i+1).y());
                    res += s;
                }
            }
        }
    }
    return res;
}
