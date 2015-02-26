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
 **  Copyright (C) 2015 Valentina project
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
            ekvPoints.append(SingleParallelPoint(QLineF(p.at(0), p.at(1)), 90, width));
            continue;
        }
        if (i == p.size()-1 && eqv == EquidistantType::CloseEquidistant)
        {//last point, polyline closed
            ekvPoints.append(ekvPoints.at(0));
            continue;
        }
        else if (i == p.size()-1 && eqv == EquidistantType::OpenEquidistant)
        {//last point, polyline doesn't closed
                ekvPoints.append(SingleParallelPoint(QLineF(p.at(p.size()-1), p.at(p.size()-2)), -90, width));
                continue;
        }
        //points in the middle of polyline
        ekvPoints<<EkvPoint(QLineF(p.at(i-1), p.at(i)), QLineF(p.at(i+1), p.at(i)), width);
    }
    return ekvPoints;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractDetail::RemoveDublicates(const QVector<QPointF> &points)
{
    QVector<QPointF> p = points;
    for(int i = 0; i < p.size(); i++)
    {
        QPointF current = p.at(i);

        for(int j = i; j < p.size(); j++)
        {
            if(j == i)
            {
                continue;
            }
            else
            {
                QPointF temp = p.at(j);
                if(current == temp)
                {
                    QVector<QPointF>::iterator iter = p.begin() + j;
                    p.erase(iter);
                    j--;
                }
            }
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
    correctPoints = CheckLoops(correctPoints);
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
    QVector<QPointF> ekvPoints;
    /*If we got less than 4 points no need seek loops.*/
    if (points.size() < 4)
    {
        qDebug()<<"Less then 4 points. Doesn't need check for loops.";
        return points;
    }
    bool closed = false;
    if (points.at(0) == points.at(points.size()-1))
    {
        closed = true;
    }
    qint32 i, j;
    for (i = 0; i < points.size(); ++i)
    {
        /*Last three points no need check.*/
        if (i >= points.size()-3)
        {
            ekvPoints.append(points.at(i));
            continue;
        }
        QPointF crosPoint;
        QLineF::IntersectType intersect = QLineF::NoIntersection;
        QLineF line1(points.at(i), points.at(i+1));
        for (j = i+2; j < points.size()-1; ++j)
        {
            QLineF line2(points.at(j), points.at(j+1));
            intersect = line1.intersect(line2, &crosPoint);
            if (intersect == QLineF::BoundedIntersection)
            {
                break;
            }
        }
        if (intersect == QLineF::BoundedIntersection)
        {
            if (i == 0 && j+1 == points.size()-1 && closed)
            {
                /*We got closed contour.*/
                ekvPoints.append(points.at(i));
            }
            else
            {
                /*We found loop.*/
                ekvPoints.append(points.at(i));
                ekvPoints.append(crosPoint);
                i = j;
            }
        }
        else
        {
            /*We did not found loop.*/
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
