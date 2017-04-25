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
#include "../vmisc/vabstractapplication.h"
#include "../vgeometry/vpointf.h"

#include <QLineF>
#include <QSet>
#include <QVector>
#include <QPainterPath>

const qreal maxL = 2.4;

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
bool VAbstractPiece::IsSeamAllowanceBuiltIn() const
{
    return d->m_seamAllowanceBuiltIn;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetSeamAllowanceBuiltIn(bool value)
{
    d->m_seamAllowanceBuiltIn = value;
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
QVector<QPointF> VAbstractPiece::Equidistant(const QVector<VSAPoint> &points, qreal width)
{
    if (width < 0)
    {
        qDebug()<<"Width < 0.";
        return QVector<QPointF>();
    }

    QVector<VSAPoint> p = CorrectEquidistantPoints(points);
    if ( p.size() < 3 )
    {
        qDebug()<<"Not enough points for building the equidistant.";
        return QVector<QPointF>();
    }

    if (p.last().toPoint() != p.first().toPoint())
    {
        p.append(p.at(0));// Should be always closed
    }

    QVector<QPointF> ekvPoints;
    for (qint32 i = 0; i < p.size(); ++i )
    {
        if ( i == 0)
        {//first point
            ekvPoints << EkvPoint(p.at(p.size()-2), p.at(p.size()-1),
                                  p.at(1), p.at(0), width);
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
        ekvPoints << EkvPoint(p.at(i-1), p.at(i),
                              p.at(i+1), p.at(i), width);
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
    int count = points.size();
    /*If we got less than 4 points no need seek loops.*/
    if (count < 4)
    {
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
                    && uniqueVertices.size() == 4)
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
                        QPointF cPoint;
                        const bool caseFlag = ParallelCrossPoint(line1, line2, cPoint);
                        if (not caseFlag || CheckIntersection(points, i, i+1, j, jNext, cPoint))
                        {
                            status = ParallelIntersection;
                            break;
                        }
                    }
                }
            }
            else if (intersect == QLineF::BoundedIntersection)
            {
                if (uniqueVertices.size() == 4)
                { // Break, but not if lines are neighbors
                    if ((line1.p1() != crosPoint
                        && line1.p2() != crosPoint
                        && line2.p1() != crosPoint
                        && line2.p2() != crosPoint) || CheckIntersection(points, i, i+1, j, jNext, crosPoint))
                    {
                        status = BoundedIntersection;
                        break;
                    }
                }
            }
            status = NoIntersection;
        }

        switch (status)
        {
            case ParallelIntersection:
                /*We have found a loop.*/
                ekvPoints.append(points.at(i));
                ekvPoints.append(points.at(jNext));
                jNext > j ? i = jNext : i = j; // Skip a loop
                break;
            case BoundedIntersection:
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
Q_DECL_CONSTEXPR qreal VAbstractPiece::PointPosition(const QPointF &p, const QLineF &line)
{
    return (line.p2().x() - line.p1().x()) * (p.y() - line.p1().y()) -
           (line.p2().y() - line.p1().y()) * (p.x() - line.p1().x());
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractPiece::MaxLocalSA(const VSAPoint &p, qreal width)
{
    qreal w1 = p.GetSAAfter();
    if (w1 < 0)
    {
        w1 = width;
    }

    qreal w2 = p.GetSABefore();
    if (w2 < 0)
    {
        w2 = width;
    }

    return qMax(w1, w2);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EkvPoint return seam aloowance points in place of intersection two edges. Last points of two edges should be
 * equal.
 * @param width global seam allowance width.
 * @return seam aloowance points.
 */
QVector<QPointF> VAbstractPiece::EkvPoint(const VSAPoint &p1Line1, const VSAPoint &p2Line1,
                                          const VSAPoint &p1Line2, const VSAPoint &p2Line2, qreal width)
{
    if (width < 0)
    { // width can't be < 0
        return QVector<QPointF>();
    }

    QVector<QPointF> points;
    if (p2Line1 != p2Line2)
    {
        qDebug()<<"Last points of two lines must be equal.";
        return QVector<QPointF>(); // Wrong edges
    }

    const QLineF bigLine1 = ParallelLine(p1Line1, p2Line1, width );
    const QLineF bigLine2 = ParallelLine(p2Line2, p1Line2, width );
    QPointF CrosPoint;
    const QLineF::IntersectType type = bigLine1.intersect( bigLine2, &CrosPoint );
    switch (type)
    {// There are at least three big cases
        case (QLineF::BoundedIntersection):
            // The easiest, real intersection
            points.append(CrosPoint);
            return points;
        case (QLineF::UnboundedIntersection):
        { // Most common case
            const qreal localWidth = MaxLocalSA(p2Line1, width);
            QLineF line( p2Line1, CrosPoint );

            // Checking two subcases
            const QLineF b1 = BisectorLine(p1Line1, p2Line1, p1Line2);
            const QLineF b2 = BisectorLine(bigLine1.p1(), CrosPoint, bigLine2.p2());

            const qreal angle = AngleBetweenBisectors(b1, b2);

            // Comparison bisector angles helps to find direction
            if (angle < 90
                || VFuzzyComparePossibleNulls(angle, 90.0))// Go in a same direction
            {//Regular equdistant case
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
                switch (p2Line1.GetAngleType())
                {
                    case PieceNodeAngle::ByLength:
                        return AngleByLength(p2Line1, bigLine1.p1(), CrosPoint, bigLine2.p2(), localWidth);
                    case PieceNodeAngle::ByPointsIntersection:
                        return AngleByIntersection(p1Line1, p2Line1, p1Line2, bigLine1.p1(), CrosPoint, bigLine2.p2(),
                                                   localWidth);
                    case PieceNodeAngle::ByFirstEdgeSymmetry:
                        return AngleByFirstSymmetry(p1Line1, p2Line1, bigLine1.p1(), CrosPoint, bigLine2.p2(),
                                                    localWidth);
                    case PieceNodeAngle::BySecondEdgeSymmetry:
                        return AngleBySecondSymmetry(p2Line1, p1Line2, bigLine1.p1(), CrosPoint,bigLine2.p2(),
                                                     localWidth);
                    case PieceNodeAngle::ByFirstEdgeRightAngle:
                        return AngleByFirstRightAngle(p1Line1, p2Line1, bigLine1.p1(), CrosPoint, bigLine2.p2(),
                                                      localWidth);
                    case PieceNodeAngle::BySecondEdgeRightAngle:
                        return AngleBySecondRightAngle(p2Line1, p1Line2, bigLine1.p1(), CrosPoint, bigLine2.p2(),
                                                       localWidth);
                }
QT_WARNING_POP
            }
            else
            { // Different directions
                QLineF bisector(p2Line1, p1Line1);
                bisector.setAngle(b1.angle());

                const qreal result1 = PointPosition(bisector.p2(), QLineF(p1Line1, p2Line1));
                const qreal result2 = PointPosition(bisector.p2(), QLineF(p2Line2, p1Line2));

                if ((result1 < 0 || qFuzzyIsNull(result1)) && (result2 < 0 || qFuzzyIsNull(result2)))
                {// Dart case. A bisector watch outside. In some cases a point still valid, but ignore if going
                 // outside of an equdistant.

                    const QLineF bigEdge = ParallelLine(p1Line1, p1Line2, localWidth );
                    QPointF px;
                    const QLineF::IntersectType type = bigEdge.intersect(line, &px);
                    if (type != QLineF::BoundedIntersection)
                    {
                        if (line.length() < QLineF(p2Line1, px).length())
                        {
                            points.append(CrosPoint);
                            return points;
                        }
                    }
                }
                else
                { // New subcase. This is not a dart. An angle is acute and bisector watch inside.
                    const qreal result1 = PointPosition(CrosPoint, QLineF(p1Line1, p2Line1));
                    const qreal result2 = PointPosition(CrosPoint, QLineF(p2Line2, p1Line2));

                    if ((result1 < 0 || qFuzzyIsNull(result1)) && (result2 < 0 || qFuzzyIsNull(result2)))
                    {// The cross point is still outside of a piece
                        if (line.length() >= localWidth)
                        {
                            points.append(CrosPoint);
                            return points;
                        }
                        else
                        {// but not enough far, fix it
                            line.setLength(localWidth);
                            points.append(line.p2());
                            return points;
                        }
                    }
                    else
                    {// Wrong cross point, probably inside of a piece. Manually creating correct seam allowance
                        const QLineF bigEdge = ParallelLine(bigLine1.p2(), bigLine2.p1(), localWidth );
                        points.append(bigEdge.p1());
                        points.append(bigEdge.p2());
                        return points;
                    }
                }
            }
            break;
        }
        case (QLineF::NoIntersection):
            /*If we have correct lines this means lines lie on a line.*/
            points.append(bigLine1.p2());
            return points;
        default:
            break;
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::AngleByLength(const QPointF &p2, const QPointF &sp1, const QPointF &sp2,
                                               const QPointF &sp3, qreal width)
{
    QVector<QPointF> points;

    QLineF line(p2, sp2);
    const qreal length = line.length();
    if (length > width*maxL)
    { // Cutting too long a cut angle
        line.setLength(width);
        QLineF cutLine(line.p2(), sp2); // Cut line is a perpendicular
        cutLine.setLength(length); // Decided take this length

        // We do not check intersection type because intersection must alwayse exist
        QPointF px;
        cutLine.setAngle(cutLine.angle()+90);
        QLineF::IntersectType type = QLineF(sp1, sp2).intersect(cutLine, &px);
        if (type == QLineF::NoIntersection)
        {
            qDebug()<<"Couldn't find intersection with cut line.";
        }
        points.append(px);

        cutLine.setAngle(cutLine.angle()-180);
        type = QLineF(sp2, sp3).intersect(cutLine, &px);
        if (type == QLineF::NoIntersection)
        {
            qDebug()<<"Couldn't find intersection with cut line.";
        }
        points.append(px);
    }
    else
    { // The point just fine
        points.append(sp2);
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::AngleByIntersection(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                                     const QPointF &sp1, const QPointF &sp2, const QPointF &sp3,
                                                     qreal width)
{
    QVector<QPointF> points;

    QLineF edge2(p2, p3);
    QLineF sEdge1(sp1, sp2);

    QPointF px;
    QLineF::IntersectType type = edge2.intersect(sEdge1, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    QLineF edge1(p1, p2);
    QLineF sEdge2(sp2, sp3);

    type = edge1.intersect(sEdge2, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::AngleByFirstSymmetry(const QPointF &p1, const QPointF &p2,
                                                      const QPointF &sp1, const QPointF &sp2, const QPointF &sp3,
                                                      qreal width)
{
    QVector<QPointF> points;

    QLineF sEdge2(sp2, sp3);
    QPointF fp1 = VPointF::FlipPF(sEdge2, p1);
    QPointF fp2 = VPointF::FlipPF(sEdge2, p2);
    QLineF fEdge(fp1, fp2);

    QPointF px;
    QLineF sEdge1(sp1, sp2);
    QLineF::IntersectType type = fEdge.intersect(sEdge1, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    type = fEdge.intersect(sEdge2, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::AngleBySecondSymmetry(const QPointF &p2, const QPointF &p3,
                                                       const QPointF &sp1, const QPointF &sp2, const QPointF &sp3,
                                                       qreal width)
{
    QVector<QPointF> points;

    QLineF sEdge1(sp1, sp2);
    QPointF fp2 = VPointF::FlipPF(sEdge1, p2);
    QPointF fp3 = VPointF::FlipPF(sEdge1, p3);
    QLineF fEdge(fp2, fp3);

    QPointF px;
    QLineF::IntersectType type = fEdge.intersect(sEdge1, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    QLineF sEdge2(sp2, sp3);
    type = fEdge.intersect(sEdge2, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::AngleByFirstRightAngle(const QPointF &p1, const QPointF &p2,
                                                        const QPointF &sp1, const QPointF &sp2, const QPointF &sp3,
                                                        qreal width)
{
    QVector<QPointF> points;

    QLineF edge1(p2, p1);
    edge1.setAngle(edge1.angle()-90);

    QPointF px;
    QLineF::IntersectType type = edge1.intersect(QLineF(sp1, sp2), &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    type = edge1.intersect(QLineF(sp2, sp3), &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::AngleBySecondRightAngle(const QPointF &p2, const QPointF &p3,
                                                         const QPointF &sp1, const QPointF &sp2, const QPointF &sp3,
                                                         qreal width)
{
    QVector<QPointF> points;

    QLineF edge2(p2, p3);
    edge2.setAngle(edge2.angle()+90);

    QPointF px;
    QLineF::IntersectType type = edge2.intersect(QLineF(sp1, sp2), &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    type = edge2.intersect(QLineF(sp2, sp3), &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }

    if (QLineF(p2, px).length() > width*maxL)
    {
        return AngleByLength(p2, sp1, sp2, sp3, width);
    }
    points.append(px);

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VAbstractPiece::ParallelLine(const VSAPoint &p1, const VSAPoint &p2, qreal width)
{
    qreal w1 = p1.GetSAAfter();
    if (w1 < 0)
    {
        w1 = width;
    }

    qreal w2 = p2.GetSABefore();
    if (w2 < 0)
    {
        w2 = width;
    }

    const QLineF paralel = QLineF(SingleParallelPoint(p1, p2, 90, w1),
                                  SingleParallelPoint(p2, p1, -90, w2));
    return paralel;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::ParallelCurve(const QVector<VSAPoint> &points, qreal width)
{
    QVector<QPointF> curvePoints;

    bool removeFirstAndLast = false;
    const QVector<VSAPoint> p = CorrectEquidistantPoints(points, removeFirstAndLast);

    if (p.size() < 2)
    {
        return QVector<QPointF>();
    }
    else if (p.size() < 3)
    {
         const QLineF line = ParallelLine(p.at(0), p.at(1), width);
         curvePoints << line.p1();
         curvePoints << line.p2();
    }
    else
    {
        for (qint32 i = 0; i < p.size(); ++i)
        {
            if ( i == 0)
            {//first point
                curvePoints << ParallelLine(p.at(i), p.at(i+1), width).p1();
                continue;
            }

            if (i == p.size()-1)
            {//last point
                if (not curvePoints.isEmpty())
                {
                    curvePoints << ParallelLine(p.at(i-1), p.at(i), width).p2();
                }
                continue;
            }
            //points in the middle of polyline
            curvePoints << EkvPoint(p.at(i-1), p.at(i), p.at(i+1), p.at(i), width);
        }
    }

    curvePoints = CheckLoops(CorrectEquidistantPoints(curvePoints, removeFirstAndLast));//Result path can contain loops
    return curvePoints;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VAbstractPiece::ParallelLine(const QPointF &p1, const QPointF &p2, qreal width)
{
    const QLineF paralel = QLineF(SingleParallelPoint(p1, p2, 90, width),
                                  SingleParallelPoint(p2, p1, -90, width));
    return paralel;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VAbstractPiece::SingleParallelPoint(const QPointF &p1, const QPointF &p2, qreal angle, qreal width)
{
    QLineF pLine(p1, p2);
    pLine.setAngle( pLine.angle() + angle );
    pLine.setLength( width );
    return pLine.p2();
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VAbstractPiece::BisectorLine(const QPointF &p1, const QPointF &p2, const QPointF &p3)
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

    return bLine;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractPiece::AngleBetweenBisectors(const QLineF &b1, const QLineF &b2)
{
    const QLineF newB2 = b2.translated(-(b2.p1().x() - b1.p1().x()), -(b2.p1().y() - b1.p1().y()));

    qreal angle1 = newB2.angleTo(b1);
    if (VFuzzyComparePossibleNulls(angle1, 360))
    {
        angle1 = 0;
    }

    qreal angle2 = b1.angleTo(newB2);
    if (VFuzzyComparePossibleNulls(angle2, 360))
    {
        angle2 = 0;
    }

    if (angle1 <= angle2)
    {
        return angle1;
    }
    else
    {
        return angle2;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::CheckIntersection(const QVector<QPointF> &points, int i, int iNext, int j, int jNext,
                                        const QPointF &crossPoint)
{
    QVector<QPointF> sub1 = SubPath(points, iNext, j);
    sub1.append(crossPoint);
    sub1 = CheckLoops(CorrectEquidistantPoints(sub1, false));
    const qreal sub1Sum = SumTrapezoids(sub1);

    QVector<QPointF> sub2 = SubPath(points, jNext, i);
    sub2.append(crossPoint);
    sub2 = CheckLoops(CorrectEquidistantPoints(sub2, false));
    const qreal sub2Sum = SumTrapezoids(sub2);

    if (sub1Sum < 0 && sub2Sum < 0)
    {
        if (Crossing(sub1, sub2))
        {
            return true;
        }
    }
    else
    {
        if (not Crossing(sub1, sub2))
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::ParallelCrossPoint(const QLineF &line1, const QLineF &line2, QPointF &point)
{
    const bool l1p1el2p1 = (line1.p1() == line2.p1());
    const bool l1p2el2p2 = (line1.p2() == line2.p2());
    const bool l1p1el2p2 = (line1.p1() == line2.p2());
    const bool l1p2el2p1 = (line1.p2() == line2.p1());

    if (l1p2el2p2 || l1p2el2p1)
    {
        point = line1.p2();
        return true;
    }
    else if (l1p1el2p1 || l1p1el2p2)
    {
        point = line1.p1();
        return true;
    }
    else
    {
        point = QPointF();
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::Crossing(const QVector<QPointF> &sub1, const QVector<QPointF> &sub2)
{
    if (sub1.isEmpty() || sub2.isEmpty())
    {
        return false;
    }

    const QRectF sub1Rect = QPolygonF(sub1).boundingRect();
    const QRectF sub2Rect = QPolygonF(sub2).boundingRect();
    if (not sub1Rect.intersects(sub2Rect))
    {
        return false;
    }

    QPainterPath sub1Path;
    sub1Path.setFillRule(Qt::WindingFill);
    sub1Path.moveTo(sub1.at(0));
    for (qint32 i = 1; i < sub1.count(); ++i)
    {
        sub1Path.lineTo(sub1.at(i));
    }
    sub1Path.lineTo(sub1.at(0));

    QPainterPath sub2Path;
    sub2Path.setFillRule(Qt::WindingFill);
    sub2Path.moveTo(sub2.at(0));
    for (qint32 i = 1; i < sub2.count(); ++i)
    {
        sub2Path.lineTo(sub2.at(i));
    }
    sub2Path.lineTo(sub2.at(0));

    if (not sub1Path.intersects(sub2Path))
    {
        return false;
    }
    else
    {
        return true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::SubPath(const QVector<QPointF> &path, int startIndex, int endIndex)
{
    if (path.isEmpty()
       || startIndex < 0 || startIndex >= path.size()
       || endIndex < 0 || endIndex >= path.size()
       || startIndex == endIndex)
    {
        return path;
    }

    QVector<QPointF> subPath;
    int i = startIndex - 1;
    do
    {
        ++i;
        if (i >= path.size())
        {
            i = 0;
        }
        subPath.append(path.at(i));
    } while (i != endIndex);

    return subPath;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::IsEkvPointOnLine(const QPointF &iPoint, const QPointF &prevPoint, const QPointF &nextPoint)
{
    return VGObject::IsPointOnLineviaPDP(iPoint, prevPoint, nextPoint);
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::IsEkvPointOnLine(const VSAPoint &iPoint, const VSAPoint &prevPoint, const VSAPoint &nextPoint)
{
    // See bug #646
    bool ekvPointOnLine = false;

    if (VFuzzyComparePossibleNulls(prevPoint.GetSAAfter(), iPoint.GetSABefore())
            && VFuzzyComparePossibleNulls(iPoint.GetSAAfter(), nextPoint.GetSABefore()))
    {
        if (VFuzzyComparePossibleNulls(prevPoint.GetSAAfter(), nextPoint.GetSABefore()))
        {
            ekvPointOnLine = true;
        }
    }
    return (VGObject::IsPointOnLineviaPDP(iPoint, prevPoint, nextPoint) && ekvPointOnLine);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSAPoint::GetSABefore(qreal width) const
{
    if (m_before < 0)
    {
        return width;
    }
    return m_before;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSAPoint::GetSAAfter(qreal width) const
{
    if (m_after < 0)
    {
        return width;
    }
    return m_after;
}
