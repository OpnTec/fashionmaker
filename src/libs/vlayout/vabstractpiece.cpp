/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
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

#include "vabstractpiece.h"
#include "vabstractpiece_p.h"
#include "../vmisc/vabstractapplication.h"
#include "../vgeometry/vpointf.h"
#include "../ifc/exception/vexception.h"

#include <QLineF>
#include <QSet>
#include <QVector>
#include <QPainterPath>
#include <QTemporaryFile>

const qreal maxL = 2.5;

namespace
{
// Do we create a point outside of a path?
inline bool IsOutsidePoint(QPointF p1, QPointF p2, QPointF px)
{
    return qAbs(QLineF(p1, p2).angle() - QLineF(p1, px).angle()) < 0.001;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> SubPath(const QVector<QPointF> &path, int startIndex, int endIndex)
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
bool Crossing(const QVector<QPointF> &sub1, const QVector<QPointF> &sub2)
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
bool CheckIntersection(const QVector<QPointF> &points, int i, int iNext, int j, int jNext, const QPointF &crossPoint)
{
    QVector<QPointF> sub1 = SubPath(points, iNext, j);
    sub1.append(crossPoint);
    sub1 = VAbstractPiece::CorrectEquidistantPoints(sub1, false);
    const qreal sub1Sum = VAbstractPiece::SumTrapezoids(sub1);

    QVector<QPointF> sub2 = SubPath(points, jNext, i);
    sub2.append(crossPoint);
    sub2 = VAbstractPiece::CorrectEquidistantPoints(sub2, false);
    const qreal sub2Sum = VAbstractPiece::SumTrapezoids(sub2);

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
bool ParallelCrossPoint(const QLineF &line1, const QLineF &line2, QPointF &point)
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
Q_DECL_CONSTEXPR qreal PointPosition(const QPointF &p, const QLineF &line)
{
    return (line.p2().x() - line.p1().x()) * (p.y() - line.p1().y()) -
           (line.p2().y() - line.p1().y()) * (p.x() - line.p1().x());
}

//---------------------------------------------------------------------------------------------------------------------
// Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
QVector<QPointF> RollbackSeamAllowance(QVector<QPointF> points, const QLineF &cuttingEdge, bool *success)
{
    *success = false;
    QVector<QPointF> clipped;
    clipped.reserve(points.count()+1);
    for (int i = points.count()-1; i > 0; --i)
    {
        QLineF segment(points.at(i), points.at(i-1));
        QPointF crosPoint;
        const QLineF::IntersectType type = cuttingEdge.intersect(segment, &crosPoint);
        if (type != QLineF::NoIntersection
                && VGObject::IsPointOnLineSegment(crosPoint, segment.p1(), segment.p2()))
        {
            clipped.append(crosPoint);
            for (int j=i-1; j>=0; --j)
            {
                clipped.append(points.at(j));
            }
            points = VGObject::GetReversePoints(clipped);
            *success = true;
        }
    }

    if (not *success && points.size() > 1)
    {
        QPointF crosPoint;
        QLineF secondLast(points.at(points.size()-2), points.at(points.size()-1));
        QLineF::IntersectType type = secondLast.intersect(cuttingEdge, &crosPoint);
        if (type != QLineF::NoIntersection && IsOutsidePoint(secondLast.p1(), secondLast.p2(), crosPoint))
        {
            points.append(crosPoint);
            *success = true;
        }
    }

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> AngleByLength(QVector<QPointF> points, QPointF p2, const QLineF &bigLine1, QPointF sp2,
                               const QLineF &bigLine2, const VSAPoint &p, qreal width)
{
    const QPointF sp1 = bigLine1.p1();
    const QPointF sp3 = bigLine2.p2();
    const qreal localWidth = p.MaxLocalSA(width);

    if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), sp2) && IsOutsidePoint(bigLine2.p2(), bigLine2.p1(), sp2) )
    {
        QLineF line(p2, sp2);
        const qreal length = line.length();
        if (length > localWidth*maxL)
        { // Cutting too long acut angle
            line.setLength(localWidth);
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
        {// The point just fine
            points.append(sp2);
        }
    }
    else
    {
        if (not IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), sp2))
        {
            if (p.GetAngleType() != PieceNodeAngle::ByLengthCurve)
            {
                bool success = false;
                points = RollbackSeamAllowance(points, bigLine2, &success);
            }
            else
            {
                points.append(sp2);
            }
        }
        else
        {
            if (p.GetAngleType() != PieceNodeAngle::ByLengthCurve)
            {
                // Need to create artificial loop
                QLineF loop1(sp2, sp1);
                loop1.setLength(loop1.length()*0.1);

                points.append(loop1.p2()); // Nedd for the main path rule

                loop1.setAngle(loop1.angle() + 180);
                loop1.setLength(localWidth);
                points.append(loop1.p2());
                points.append(bigLine2.p1());
            }
            else
            {
                points.append(sp2);
            }
        }
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> AngleByIntersection(const QVector<QPointF> &points, QPointF p1, QPointF p2, QPointF p3,
                                     const QLineF &bigLine1, QPointF sp2, const QLineF &bigLine2,
                                     const VSAPoint &p, qreal width)
{
    const qreal localWidth = p.MaxLocalSA(width);
    QVector<QPointF> pointsIntr = points;

    // First point
    QLineF edge2(p2, p3);

    QPointF px;
    QLineF::IntersectType type = edge2.intersect(bigLine1, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
    }

    if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), px))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
        }
        pointsIntr.append(px);
    }
    else
    {// Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
        bool success = false;
        pointsIntr = RollbackSeamAllowance(pointsIntr, edge2, &success);
    }

    // Second point
    QLineF edge1(p1, p2);

    type = edge1.intersect(bigLine2, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
    }

    if (IsOutsidePoint(bigLine2.p2(), bigLine2.p1(), px))
    {
        pointsIntr.append(px);
    }
    else
    {
        QLineF allowance(p2, px);
        pointsIntr.append(allowance.p2());
        allowance.setLength(allowance.length() + localWidth * 3.);
        pointsIntr.append(allowance.p2());
        pointsIntr.append(bigLine2.p2());
    }

    return pointsIntr;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> AngleByFirstSymmetry(const QVector<QPointF> &points, QPointF p1, QPointF p2,
                                      const QLineF &bigLine1, QPointF sp2, const QLineF &bigLine2,
                                      const VSAPoint &p, qreal width)
{
    const qreal localWidth = p.MaxLocalSA(width);
    QVector<QPointF> pointsIntr = points;

    QLineF sEdge(VPointF::FlipPF(bigLine2, p1), VPointF::FlipPF(bigLine2, p2));

    QPointF px;
    QLineF::IntersectType type = sEdge.intersect(bigLine1, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
    }

    if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), px))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
        }
        pointsIntr.append(px);
    }
    else
    {// Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
        bool success = false;
        pointsIntr = RollbackSeamAllowance(pointsIntr, bigLine2, &success);
    }

    type = sEdge.intersect(bigLine2, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
    }

    if (IsOutsidePoint(bigLine2.p2(), bigLine2.p1(), px))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
        }
        pointsIntr.append(px);
    }
    else
    {
        QLineF allowance(px, p2);
        allowance.setAngle(allowance.angle() + 90);
        pointsIntr.append(allowance.p2());
        pointsIntr.append(bigLine2.p1());
    }

    return pointsIntr;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> AngleBySecondSymmetry(const QVector<QPointF> &points, QPointF p2, QPointF p3,
                                       const QLineF &bigLine1, QPointF sp2, const QLineF &bigLine2,
                                       const VSAPoint &p, qreal width)
{
    const qreal localWidth = p.MaxLocalSA(width);
    QVector<QPointF> pointsIntr = points;

    QLineF sEdge(VPointF::FlipPF(bigLine1, p2), VPointF::FlipPF(bigLine1, p3));

    QPointF px;
    QLineF::IntersectType type = sEdge.intersect(bigLine1, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
    }

    if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), px))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
        }
        pointsIntr.append(px);
    }
    else
    {// Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
        bool success = false;
        pointsIntr = RollbackSeamAllowance(pointsIntr, bigLine2, &success);
    }

    type = sEdge.intersect(bigLine2, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
    }

    if (IsOutsidePoint(bigLine2.p2(), bigLine2.p1(), px))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
        }
        pointsIntr.append(px);
    }
    else
    {
        QLineF allowance(p2, px);
        allowance.setLength(p.GetSAAfter(width)*0.98);
        pointsIntr.append(allowance.p2());
        allowance.setLength(allowance.length() + localWidth * 3.);
        pointsIntr.append(allowance.p2());
        pointsIntr.append(bigLine2.p2());
    }

    return pointsIntr;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> AngleByFirstRightAngle(const QVector<QPointF> &points, QPointF p1, QPointF p2,
                                        const QLineF &bigLine1, QPointF sp2, const QLineF &bigLine2,
                                        const VSAPoint &p, qreal width)
{
    const qreal localWidth = p.MaxLocalSA(width);
    QVector<QPointF> pointsRA = points;
    QLineF edge(p1, p2);

    QPointF px;
    QLineF::IntersectType type = edge.intersect(bigLine2, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
    }

    QLineF seam(px, p1);
    seam.setAngle(seam.angle()-90);
    seam.setLength(p.GetSABefore(width));

    pointsRA.append(seam.p2());

    if (IsOutsidePoint(bigLine2.p2(), bigLine2.p1(), seam.p1()))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
        }
        pointsRA.append(seam.p1());
    }
    else
    {
        QLineF loopLine(px, sp2);
        const qreal length = loopLine.length()*0.98;
        loopLine.setLength(length);

        QLineF tmp(seam.p2(), seam.p1());
        tmp.setLength(tmp.length()+length);

        pointsRA.append(tmp.p2());
        pointsRA.append(loopLine.p2());
    }

    return pointsRA;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> AngleBySecondRightAngle(QVector<QPointF> points, QPointF p2, QPointF p3,
                                         const QLineF &bigLine1,  QPointF sp2, const QLineF &bigLine2,
                                         const VSAPoint &p, qreal width)
{
    const qreal localWidth = p.MaxLocalSA(width);
    QLineF edge(p2, p3);

    QPointF px;
    QLineF::IntersectType type = edge.intersect(bigLine1, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
    }



    if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), px))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p2, bigLine1, sp2, bigLine2, p, width);
        }
        points.append(px);
    }
    else
    {
        // Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
        bool success = false;
        points = RollbackSeamAllowance(points, edge, &success);
        if (success)
        {
            px = points.last();
        }
    }

    QLineF seam(px, p3);
    seam.setAngle(seam.angle()+90);
    seam.setLength(p.GetSAAfter(width));
    points.append(seam.p2());

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF SingleParallelPoint(const QPointF &p1, const QPointF &p2, qreal angle, qreal width)
{
    QLineF pLine(p1, p2);
    pLine.setAngle( pLine.angle() + angle );
    pLine.setLength( width );
    return pLine.p2();
}

//---------------------------------------------------------------------------------------------------------------------
QLineF SimpleParallelLine(const QPointF &p1, const QPointF &p2, qreal width)
{
    const QLineF paralel = QLineF(SingleParallelPoint(p1, p2, 90, width),
                                  SingleParallelPoint(p2, p1, -90, width));
    return paralel;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF BisectorLine(const QPointF &p1, const QPointF &p2, const QPointF &p3)
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
qreal AngleBetweenBisectors(const QLineF &b1, const QLineF &b2)
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

    return qMin(angle1, angle2);
}

//---------------------------------------------------------------------------------------------------------------------
#if !defined(V_NO_ASSERT)
// Use for writing tests
void DumpVector(const QVector<QPointF> &points)
{
    QTemporaryFile temp; // Go to tmp folder to find dump
    temp.setAutoRemove(false); // Remove dump manually
    if (temp.open())
    {
        QTextStream out(&temp);
        out << "QVector<QPointF> points;" << endl << endl;

        for(auto point : points)
        {
            out << QString("points += QPointF(%1, %2);").arg(point.x(), 0, 'f', 15).arg(point.y(), 0, 'f', 15) << endl;
        }

        out << endl << "return points;";
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Use for writing tests
void DumpVector(const QVector<VSAPoint> &points)
{
    QTemporaryFile temp; // Go to tmp folder to find dump
    temp.setAutoRemove(false); // Remove dump manually
    if (temp.open())
    {
        QTextStream out(&temp);

        out << "QVector<VSAPoint> points;" << endl;

        bool firstPoint = true;
        enum PointType {Unknown, Default, Custom};
        PointType type = Unknown;

        for(auto point : points)
        {
            if (VFuzzyComparePossibleNulls(point.GetSAAfter(), -1)
                    and VFuzzyComparePossibleNulls(point.GetSABefore(), -1)
                    and point.GetAngleType() == PieceNodeAngle::ByLength)
            {
                if (type != Default)
                {
                    out << endl;
                }
                type = Default;
                out << QString("points += VSAPoint(%1, %2);").arg(point.x(), 0, 'f', 15).arg(point.y(), 0, 'f', 15)
                    << endl;
            }
            else
            {
                out << endl;
                type = Custom;

                if (firstPoint)
                {
                    out << "VSAPoint ";
                    firstPoint = false;
                }
                out << QString("p = VSAPoint(%1, %2);").arg(point.x(), 0, 'f', 15).arg(point.y(), 0, 'f', 15) << endl;

                if (not VFuzzyComparePossibleNulls(point.GetSABefore(), -1))
                {
                    out << QString("p.SetSABefore(%1);").arg(point.GetSABefore()) << endl;
                }

                if (not VFuzzyComparePossibleNulls(point.GetSAAfter(), -1))
                {
                    out << QString("p.SetSAAfter(%1);").arg(point.GetSAAfter()) << endl;
                }

                if (point.GetAngleType() != PieceNodeAngle::ByLength)
                {
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
                    // This check helps to find missed angle types in the switch
                    Q_STATIC_ASSERT_X(static_cast<int>(PieceNodeAngle::LAST_ONE_DO_NOT_USE) == 7,
                                      "Not all types were handled.");

                    QString typeStr;
                    switch (point.GetAngleType())
                    {
                        case PieceNodeAngle::LAST_ONE_DO_NOT_USE:
                        case PieceNodeAngle::ByLength:
                            Q_UNREACHABLE(); //-V501
                            break;
                        case PieceNodeAngle::ByPointsIntersection:
                            typeStr = "ByPointsIntersection";
                            break;
                        case PieceNodeAngle::ByFirstEdgeSymmetry:
                            typeStr = "ByFirstEdgeSymmetry";
                            break;
                        case PieceNodeAngle::BySecondEdgeSymmetry:
                            typeStr = "BySecondEdgeSymmetry";
                            break;
                        case PieceNodeAngle::ByFirstEdgeRightAngle:
                            typeStr = "ByFirstEdgeRightAngle";
                            break;
                        case PieceNodeAngle::BySecondEdgeRightAngle:
                            typeStr = "BySecondEdgeRightAngle";
                            break;
                        case PieceNodeAngle::ByLengthCurve:
                            typeStr = "ByLengthCurve";
                            break;
                    }
QT_WARNING_POP

                    out << QString("p.SetAngleType(PieceNodeAngle::%1);").arg(typeStr) << endl;
                }

                out << "points += p;" << endl;
            }
        }

        out << endl << "return points;";
    }
}
#endif
}

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

    if (value)
    {
        SetForceFlipping(not value);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::IsForceFlipping() const
{
    return d->m_forceFlipping;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetForceFlipping(bool value)
{
    d->m_forceFlipping = value;

    if (value)
    {
        SetForbidFlipping(not value);
    }
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
bool VAbstractPiece::IsHideMainPath() const
{
    return d->m_hideMainPath;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetHideMainPath(bool value)
{
    d->m_hideMainPath = value;
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
QVector<QPointF> VAbstractPiece::Equidistant(QVector<VSAPoint> points, qreal width, const QString &name)
{
    if (width < 0)
    {
        qDebug()<<"Width < 0.";
        return QVector<QPointF>();
    }

//    DumpVector(points); // Uncomment for dumping test data

    points = CorrectEquidistantPoints(points);
    if ( points.size() < 3 )
    {
        const QString errorMsg = QObject::tr("Piece '%1'. Not enough points to build seam allowance.").arg(name);
        qApp->IsPedantic() ? throw VException(errorMsg) : qWarning() << errorMsg;
        return QVector<QPointF>();
    }

    // Fix distorsion
    points = CorrectPathDistortion(points);

    if (points.last().toPoint() != points.first().toPoint())
    {
        points.append(points.at(0));// Should be always closed
    }

    QVector<QPointF> ekvPoints;
    for (qint32 i = 0; i < points.size(); ++i )
    {
        if ( i == 0)
        {//first point
            ekvPoints = EkvPoint(ekvPoints, points.at(points.size()-2), points.at(points.size()-1), points.at(1),
                                 points.at(0), width);
            continue;
        }

        if (i == points.size()-1)
        {//last point
            if (not ekvPoints.isEmpty())
            {
                ekvPoints.append(ekvPoints.at(0));
            }
            continue;
        }
        //points in the middle of polyline
        ekvPoints = EkvPoint(ekvPoints, points.at(i-1), points.at(i), points.at(i+1), points.at(i), width);
    }

    const bool removeFirstAndLast = false;
    ekvPoints = CheckLoops(CorrectEquidistantPoints(ekvPoints, removeFirstAndLast));//Result path can contain loops
    ekvPoints = CheckLoops(CorrectEquidistantPoints(ekvPoints, removeFirstAndLast));//Result path can contain loops
//    DumpVector(ekvPoints); // Uncomment for dumping test data
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
    ekvPoints.reserve(points.size());

    QVector<qint32> uniqueVertices;
    uniqueVertices.reserve(4);

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

            uniqueVertices.clear();

            auto AddUniqueIndex = [&uniqueVertices](qint32 i)
            {
                if (not uniqueVertices.contains(i))
                {
                    uniqueVertices.append(i);
                }
            };

            AddUniqueIndex(i);
            AddUniqueIndex(i+1);
            AddUniqueIndex(j);

            // For closed path last point is equal to first. Using index of the first.
            pathClosed && jNext == count-1 ? AddUniqueIndex(0) : AddUniqueIndex(jNext);

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
/**
 * @brief EkvPoint return seam aloowance points in place of intersection two edges. Last points of two edges should be
 * equal.
 * @param width global seam allowance width.
 * @return seam aloowance points.
 */
QVector<QPointF> VAbstractPiece::EkvPoint(QVector<QPointF> points, const VSAPoint &p1Line1, VSAPoint p2Line1,
                                          const VSAPoint &p1Line2, VSAPoint p2Line2, qreal width)
{
    if (width < 0)
    { // width can't be < 0
        return QVector<QPointF>();
    }

    if (p2Line1 != p2Line2)
    {
        qDebug()<<"Last points of two lines must be equal.";
        return QVector<QPointF>(); // Wrong edges
    }

    const QLineF bigLine1 = ParallelLine(p1Line1, p2Line1, width );
    const QLineF bigLine2 = ParallelLine(p2Line2, p1Line2, width );

    if (VFuzzyComparePoints(bigLine1.p2(), bigLine2.p1()))
    {
        points.append(bigLine1.p2());
        return points;
    }

    QPointF crosPoint;
    const QLineF::IntersectType type = bigLine1.intersect( bigLine2, &crosPoint );
    switch (type)
    {// There are at least three big cases
        case (QLineF::BoundedIntersection):
            // The easiest, real intersection
            points.append(crosPoint);
            return points;
        case (QLineF::UnboundedIntersection):
        { // Most common case
            /* Case when a path has point on line (both segments lie on the same line) and seam allowance creates
             * prong. */
            auto IsOnLine = [](const QPointF &base, const QPointF &sp1, const QPointF &sp2)
            {
                if (not VFuzzyComparePoints(base, sp1))
                {
                    return VGObject::IsPointOnLineviaPDP(sp2, base, sp1);
                }

                if (not VFuzzyComparePoints(base, sp2))
                {
                    return VGObject::IsPointOnLineviaPDP(sp1, base, sp2);
                }
                return true;
            };
            if (VGObject::IsPointOnLineSegment(p2Line1, p1Line1, p1Line2)
                    && IsOnLine(p2Line1, bigLine1.p2(), bigLine2.p1()))
            {
                points.append(bigLine1.p2());
                points.append(bigLine2.p1());
                return points;
            }

            const qreal localWidth = p2Line1.MaxLocalSA(width);
            QLineF line( p2Line1, crosPoint );

            // Checking two subcases
            const QLineF b1 = BisectorLine(p1Line1, p2Line1, p1Line2);
            const QLineF b2 = BisectorLine(bigLine1.p1(), crosPoint, bigLine2.p2());

            const qreal angle = AngleBetweenBisectors(b1, b2);

            // Comparison bisector angles helps to find direction
            if (angle < 90
                || VFuzzyComparePossibleNulls(angle, 90.0))// Go in a same direction
            {//Regular equdistant case
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
                // This check helps to find missed angle types in the switch
                Q_STATIC_ASSERT_X(static_cast<int>(PieceNodeAngle::LAST_ONE_DO_NOT_USE) == 7,
                                  "Not all types were handled.");
                switch (p2Line1.GetAngleType())
                {
                    case PieceNodeAngle::LAST_ONE_DO_NOT_USE:
                        Q_UNREACHABLE(); //-V501
                        break;
                    case PieceNodeAngle::ByLength:
                    case PieceNodeAngle::ByLengthCurve:
                        return AngleByLength(points, p2Line1, bigLine1, crosPoint, bigLine2, p2Line1, width);
                    case PieceNodeAngle::ByPointsIntersection:
                        return AngleByIntersection(points, p1Line1, p2Line1, p1Line2, bigLine1, crosPoint, bigLine2,
                                                   p2Line1, width);
                    case PieceNodeAngle::ByFirstEdgeSymmetry:
                        return AngleByFirstSymmetry(points, p1Line1, p2Line1, bigLine1, crosPoint, bigLine2,
                                                    p2Line1, width);
                    case PieceNodeAngle::BySecondEdgeSymmetry:
                        return AngleBySecondSymmetry(points, p2Line1, p1Line2, bigLine1, crosPoint, bigLine2,
                                                     p2Line1, width);
                    case PieceNodeAngle::ByFirstEdgeRightAngle:
                        return AngleByFirstRightAngle(points, p1Line1, p2Line1, bigLine1, crosPoint, bigLine2,
                                                      p2Line1, width);
                    case PieceNodeAngle::BySecondEdgeRightAngle:
                        return AngleBySecondRightAngle(points, p2Line1, p1Line2, bigLine1, crosPoint, bigLine2,
                                                       p2Line1, width);
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
                            points.append(crosPoint);
                            return points;
                        }
                    }
                }
                else
                { // New subcase. This is not a dart. An angle is acute and bisector watch inside.
                    const qreal result1 = PointPosition(crosPoint, QLineF(p1Line1, p2Line1));
                    const qreal result2 = PointPosition(crosPoint, QLineF(p2Line2, p1Line2));

                    if ((result1 < 0 || qFuzzyIsNull(result1)) && (result2 < 0 || qFuzzyIsNull(result2)))
                    {// The cross point is still outside of a piece
                        if (line.length() >= localWidth)
                        {
                            points.append(crosPoint);
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
                        const QLineF bigEdge = SimpleParallelLine(bigLine1.p2(), bigLine2.p1(), localWidth );
                        points.append(bigEdge.p1());
                        points.append(bigEdge.p2());
                        return points;
                    }
                }
            }
            break;
        }
        case (QLineF::NoIntersection):
            /*If we have correct lines this means lines lie on a line or parallel.*/
            points.append(bigLine1.p2());
            points.append(bigLine2.p1()); // Second point for parallel line
            return points;
        default:
            break;
    }
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
bool VAbstractPiece::IsEkvPointOnLine(const QPointF &iPoint, const QPointF &prevPoint, const QPointF &nextPoint)
{
    return VGObject::IsPointOnLineviaPDP(iPoint, prevPoint, nextPoint);
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::IsEkvPointOnLine(const VSAPoint &iPoint, const VSAPoint &prevPoint, const VSAPoint &nextPoint)
{
    // See bug #671
    const qreal tmpWidth = 10;
    const QLineF bigLine1 = ParallelLine(prevPoint, iPoint, tmpWidth );
    const QLineF bigLine2 = ParallelLine(iPoint, nextPoint, tmpWidth );

    return (VGObject::IsPointOnLineviaPDP(iPoint, prevPoint, nextPoint)
            && VGObject::IsPointOnLineviaPDP(bigLine1.p2(), bigLine1.p1(), bigLine2.p2())
            && VGObject::IsPointOnLineviaPDP(bigLine2.p1(), bigLine1.p1(), bigLine2.p2())
            && qAbs(prevPoint.GetSAAfter(tmpWidth) - nextPoint.GetSABefore(tmpWidth)) < accuracyPointOnLine);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VAbstractPiece::PlaceLabelImgPath(const PlaceLabelImg &img)
{
    QPainterPath path;
    for (auto &p : img)
    {
        if (not p.isEmpty())
        {
            path.moveTo(p.first());
            path.addPolygon(p);
        }
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractPiece::GetMx() const
{
    return d->m_mx;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetMx(qreal value)
{
    d->m_mx = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VAbstractPiece::GetMy() const
{
    return d->m_my;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetMy(qreal value)
{
    d->m_my = value;
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

//---------------------------------------------------------------------------------------------------------------------
qreal VSAPoint::MaxLocalSA(qreal width) const
{
    qreal w1 = GetSAAfter();
    if (w1 < 0)
    {
        w1 = width;
    }

    qreal w2 = GetSABefore();
    if (w2 < 0)
    {
        w2 = width;
    }

    return qMax(w1, w2);
}
