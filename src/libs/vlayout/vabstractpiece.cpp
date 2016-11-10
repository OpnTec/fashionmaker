/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
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

#include "vabstractpiece.h"
#include "vabstractpiece_p.h"

#include <QLineF>
#include <QSet>
#include <QVector>
#include <QDebug>

#include "../vgeometry/vgobject.h"

//---------------------------------------------------------------------------------------------------------------------
VAbstractPiece::VAbstractPiece()
    : d(new VAbstractPieceData)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractPiece::VAbstractPiece(const VAbstractPiece &piece)
    :d (piece.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractPiece &VAbstractPiece::operator=(const VAbstractPiece &piece)
{
    if ( &piece == this )
    {
        return *this;
    }
    d = piece.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractPiece::~VAbstractPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPiece::GetName() const
{
    return d->m_name;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetName(const QString &value)
{
    d->m_name = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::IsForbidFlipping() const
{
    return d->m_forbidFlipping;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetForbidFlipping(bool value)
{
    d->m_forbidFlipping = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::IsSeamAllowance() const
{
    return d->m_seamAllowance;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetSeamAllowance(bool value)
{
    d->m_seamAllowance = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractPiece::GetSAWidth() const
{
    return d->m_width;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetSAWidth(qreal value)
{
    value >= 0 ? d->m_width = value : d->m_width = 0;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::Equidistant(const QVector<QPointF> &points, qreal width)
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

    if (p.last().toPoint() != p.first().toPoint())
    {
        p.append(p.at(0));// Should be always closed
    }

    for (qint32 i = 0; i < p.size(); ++i )
    {
        if ( i == 0)
        {//first point
            ekvPoints << EkvPoint(QLineF(p.at(p.size()-2), p.at(p.size()-1)), QLineF(p.at(1), p.at(0)), width);
            continue;
        }

        if (i == p.size()-1)
        {//last point
            if (not ekvPoints.isEmpty())
            {
                ekvPoints.append(ekvPoints.at(0));
            }
            continue;
        }
        //points in the middle of polyline
        ekvPoints << EkvPoint(QLineF(p.at(i-1), p.at(i)), QLineF(p.at(i+1), p.at(i)), width);
    }

    const bool removeFirstAndLast = false;
    ekvPoints = CheckLoops(CorrectEquidistantPoints(ekvPoints, removeFirstAndLast));//Result path can contain loops
    return ekvPoints;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractPiece::SumTrapezoids(const QVector<QPointF> &points)
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
                //if i == 0, then y[i-1] replace on y[n-1]
                s = points.at(i).x()*(points.at(n-1).y() - points.at(i+1).y());
                res += s;
            }
            else
            {
                if (i == n-1)
                {
                    // if i == n-1, then y[i+1] replace on y[0]
                    s = points.at(i).x()*(points.at(i-1).y() - points.at(0).y());
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckLoops seek and delete loops in equidistant.
 * @param points vector of points of equidistant.
 * @return vector of points of equidistant.
 */
QVector<QPointF> VAbstractPiece::CheckLoops(const QVector<QPointF> &points)
{
    const int count = points.size();
    /*If we got less than 4 points no need seek loops.*/
    if (count < 4)
    {
        qDebug()<<"Less then 4 points. Doesn't need check for loops.";
        return points;
    }

    const bool pathClosed = (points.first() == points.last());

    QVector<QPointF> ekvPoints;

    qint32 i, j, jNext = 0;
    for (i = 0; i < count; ++i)
    {
        /*Last three points no need check.*/
        /*Triangle has not contain loops*/
        if (i > count-3)
        {
            ekvPoints.append(points.at(i));
            continue;
        }

        enum LoopIntersectType { NoIntersection, BoundedIntersection, ParallelIntersection };

        QPointF crosPoint;
        LoopIntersectType status = NoIntersection;
        const QLineF line1(points.at(i), points.at(i+1));
        // Because a path can contains several loops we will seek the last and only then remove the loop(s)
        // That's why we parse from the end
        for (j = count-1; j >= i+2; --j)
        {

            j == count-1 ? jNext = 0 : jNext = j+1;
            QLineF line2(points.at(j), points.at(jNext));

            if(qFuzzyIsNull(line2.length()))
            {//If a path is closed the edge (count-1;0) length will be 0
                continue;
            }

            QSet<qint32> uniqueVertices;
            uniqueVertices << i << i+1 << j;

            // For closed path last point is equal to first. Using index of the first.
            pathClosed && jNext == count-1 ? uniqueVertices << 0 : uniqueVertices << jNext;

            const QLineF::IntersectType intersect = line1.intersect(line2, &crosPoint);
            if (intersect == QLineF::NoIntersection)
            { // According to the documentation QLineF::NoIntersection indicates that the lines do not intersect;
              // i.e. they are parallel. But parallel also mean they can be on the same line.
              // Method IsPointOnLineviaPDP will check it.
                if (VGObject::IsPointOnLineviaPDP(points.at(j), points.at(i), points.at(i+1))
                    // Lines are not neighbors
                    && uniqueVertices.size() == 4
                    && line1.p2() != line2.p2()
                    && line1.p1() != line2.p1()
                    && line1.p2() != line2.p1()
                    && line1.p1() != line2.p2())
                {
                    // Left to catch case where segments are on the same line, but do not have real intersections.
                    QLineF tmpLine1 = line1;
                    QLineF tmpLine2 = line2;

                    tmpLine1.setAngle(tmpLine1.angle()+90);

                    QPointF tmpCrosPoint;
                    const QLineF::IntersectType tmpIntrs1 = tmpLine1.intersect(tmpLine2, &tmpCrosPoint);

                    tmpLine1 = line1;
                    tmpLine2.setAngle(tmpLine2.angle()+90);

                    const QLineF::IntersectType tmpIntrs2 = tmpLine1.intersect(tmpLine2, &tmpCrosPoint);

                    if (tmpIntrs1 == QLineF::BoundedIntersection || tmpIntrs2 == QLineF::BoundedIntersection)
                    { // Now we really sure that lines are on the same lines and have real intersections.
                        status = ParallelIntersection;
                        break;
                    }
                }
            }
            else if (intersect == QLineF::BoundedIntersection)
            {
                if (uniqueVertices.size() == 4
                    && line1.p1() != crosPoint
                    && line1.p2() != crosPoint
                    && line2.p1() != crosPoint
                    && line2.p2() != crosPoint)
                { // Break, but not if lines are neighbors
                    status = BoundedIntersection;
                    break;
                }
            }
            status = NoIntersection;
        }

        switch (status)
        {
            case ParallelIntersection:
                /*We have found a loop.*/
                // Theoretically there is no big difference which point j or jNext to select.
                // In the end we will draw a line in any case.
                ekvPoints.append(points.at(i));
                ekvPoints.append(points.at(jNext));
                i = j;
                break;
            case BoundedIntersection:
                /*We have found a loop.*/
                ekvPoints.append(points.at(i));
                ekvPoints.append(crosPoint);
                i = j;
                break;
            case NoIntersection:
                /*We have not found loop.*/
                ekvPoints.append(points.at(i));
                break;
            default:
                break;
        }
    }
    return ekvPoints;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CorrectEquidistantPoints clear equivalent points and remove point on line from equdistant.
 * @param points list of points equdistant.
 * @return corrected list.
 */
QVector<QPointF> VAbstractPiece::CorrectEquidistantPoints(const QVector<QPointF> &points, bool removeFirstAndLast)
{
    if (points.size()<4)//Better don't check if only three points. We can destroy equidistant.
    {
        qDebug()<<"Only three points.";
        return points;
    }

    //Clear equivalent points
    QVector<QPointF> correctPoints = RemoveDublicates(points, removeFirstAndLast);

    if (correctPoints.size()<3)
    {
        return correctPoints;
    }

    //Remove point on line
    for (qint32 i = 1; i <correctPoints.size()-1; ++i)
    {// In this case we alwayse will have bounded intersection, so all is need is to check if point i is on line.
     // Unfortunatelly QLineF::intersect can't be used in this case because of the floating-point accuraccy problem.
        if (VGObject::IsPointOnLineviaPDP(correctPoints.at(i), correctPoints.at(i-1), correctPoints.at(i+1)))
        {
            correctPoints.remove(i);
        }
    }

    return correctPoints;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::RemoveDublicates(const QVector<QPointF> &points, bool removeFirstAndLast)
{
    QVector<QPointF> p = points;

    if (removeFirstAndLast)
    {
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
    }

    for (int i = 0; i < p.size()-1; ++i)
    {
        if (p.at(i) == p.at(i+1))
        {
            if (not removeFirstAndLast && (i == p.size()-1))
            {
                continue;
            }

            p.erase(p.begin() + i + 1);
            --i;
            continue;
        }
    }

    return p;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EkvPoint return vector of points of equidistant two lines. Last point of two lines must be equal.
 * @param line1 first line.
 * @param line2 second line.
 * @param width width of equidistant.
 * @return vector of points.
 */
QVector<QPointF> VAbstractPiece::EkvPoint(const QLineF &line1, const QLineF &line2, qreal width)
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
    const QLineF bigLine1 = ParallelLine(line1, width );
    const QLineF bigLine2 = ParallelLine(QLineF(line2.p2(), line2.p1()), width );
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

            const int angle1 = BisectorAngle(line1.p1(), line1.p2(), line2.p1());
            const int angle2 = BisectorAngle(bigLine1.p1(), CrosPoint, bigLine2.p2());

            if (angle1 == angle2)
            {//Regular equdistant case
                const qreal length = line.length();
                if (length > width*2.4)
                { // Cutting too long a cut angle
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
            }
            else
            {// Dart. Ignore if going outside of equdistant
                const QLineF bigEdge = ParallelLine(QLineF(line1.p1(), line2.p1()), width );
                QPointF px;
                const QLineF::IntersectType type = bigEdge.intersect(line, &px);
                if (type != QLineF::BoundedIntersection)
                {
                    points.append(CrosPoint);
                    return points;
                }
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
QLineF VAbstractPiece::ParallelLine(const QLineF &line, qreal width)
{
    const QLineF paralel = QLineF(SingleParallelPoint(line, 90, width),
                                  SingleParallelPoint(QLineF(line.p2(), line.p1()), -90, width));
    return paralel;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VAbstractPiece::SingleParallelPoint(const QLineF &line, qreal angle, qreal width)
{
    QLineF pLine = line;
    pLine.setAngle( pLine.angle() + angle );
    pLine.setLength( width );
    return pLine.p2();
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractPiece::BisectorAngle(const QPointF &p1, const QPointF &p2, const QPointF &p3)
{
    QLineF line1(p2, p1);
    QLineF line2(p2, p3);
    QLineF bLine;

    const qreal angle1 = line1.angleTo(line2);
    const qreal angle2 = line2.angleTo(line1);

    if (angle1 <= angle2)
    {
        bLine = line1;
        bLine.setAngle(bLine.angle() + angle1/2.0);
    }
    else
    {
        bLine = line2;
        bLine.setAngle(bLine.angle() + angle2/2.0);
    }

    return qRound(bLine.angle());
}
