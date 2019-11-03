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
#include "../vmisc/vmath.h"
#include "../vpatterndb/floatItemData/vgrainlinedata.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/calculator.h"
#include "testpath.h"
#include "vrawsapoint.h"

#include <QLineF>
#include <QSet>
#include <QVector>
#include <QPainterPath>
#include <QTemporaryFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

const quint32 VAbstractPieceData::streamHeader = 0x05CDD73A; // CRC-32Q string "VAbstractPieceData"
const quint16 VAbstractPieceData::classVersion = 2;

const qreal maxL = 2.5;

const qreal VSAPoint::passmarkFactor = 0.5;
const qreal VSAPoint::maxPassmarkLength = (10/*mm*/ / 25.4) * PrintDPI;
const qreal VSAPoint::minSAWidth = ToPixel(0.01, Unit::Mm);

namespace
{
//---------------------------------------------------------------------------------------------------------------------
inline bool IsSameDirection(QPointF p1, QPointF p2, QPointF px)
{
    return qAbs(QLineF(p1, p2).angle() - QLineF(p1, px).angle()) < 0.001;
}

//---------------------------------------------------------------------------------------------------------------------
// Do we create a point outside of a path?
inline bool IsOutsidePoint(QPointF p1, QPointF p2, QPointF px)
{
    QLineF seg1(p1, p2);
    QLineF seg2(p1, px);

    return IsSameDirection(p1, p2, px) && seg2.length() >= seg1.length();
}

//---------------------------------------------------------------------------------------------------------------------
Q_DECL_CONSTEXPR qreal PointPosition(const QPointF &p, const QLineF &line)
{
    return (line.p2().x() - line.p1().x()) * (p.y() - line.p1().y()) -
           (line.p2().y() - line.p1().y()) * (p.x() - line.p1().x());
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VRawSAPoint> AngleByLength(QVector<VRawSAPoint> points, QPointF p1, QPointF p2, QPointF p3,
                                   const QLineF &bigLine1, QPointF sp2, const QLineF &bigLine2, const VSAPoint &p,
                                   qreal width, bool *needRollback = nullptr)
{
    if (needRollback != nullptr)
    {
        *needRollback = false;
    }

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
            cutLine.setLength(length); // Decided to take this length

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
        QLineF edge1(p2, p1);
        QLineF edge2(p2, p3);
        const qreal angle = edge1.angleTo(edge2);

        if (angle > 268)
        {
            QLineF loop(sp2, bigLine1.p1());
            loop.setLength(accuracyPointOnLine*2.);
            points.append(loop.p2());
            points.append(sp2);

            loop = QLineF(bigLine1.p1(), sp2);
            loop.setLength(loop.length() + localWidth*3.);
            points.append(loop.p2());
        }
        else
        {
            if (not IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), sp2))
            {
                if (p.GetAngleType() != PieceNodeAngle::ByLengthCurve)
                {
                    bool success = false;
                    QVector<VRawSAPoint> temp = points;
                    temp.append(bigLine1.p2());
                    temp = VAbstractPiece::RollbackSeamAllowance(temp, bigLine2, &success);

                    if (success)
                    {
                        points = temp;
                    }

                    if (needRollback != nullptr)
                    {
                        *needRollback = not success;
                    }
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
                    loop1.setLength(loop1.length()*0.2);

                    points.append(loop1.p2()); // Need for the main path rule

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
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VRawSAPoint> AngleByIntersection(const QVector<VRawSAPoint> &points, QPointF p1, QPointF p2, QPointF p3,
                                         const QLineF &bigLine1, QPointF sp2, const QLineF &bigLine2,
                                         const VSAPoint &p, qreal width, bool *needRollback = nullptr)
{
    {
        QLineF edge1(p2, p1);
        QLineF edge2(p2, p3);
        const qreal angle = edge1.angleTo(edge2);

        if (angle > 180)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
    }

    if (needRollback != nullptr)
    {
        *needRollback = false;
    }

    const qreal localWidth = p.MaxLocalSA(width);
    QVector<VRawSAPoint> pointsIntr = points;

    // First point
    QLineF edge2(p2, p3);

    QPointF px;
    QLineF::IntersectType type = edge2.intersect(bigLine1, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
    }

    if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), px))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
        pointsIntr.append(px);
    }
    else
    {// Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
        bool success = false;
        QVector<VRawSAPoint> temp = pointsIntr;
        temp.append(bigLine1.p2());
        temp = VAbstractPiece::RollbackSeamAllowance(temp, edge2, &success);

        if (success)
        {
            pointsIntr = temp;
        }

        if (needRollback != nullptr)
        {
            *needRollback = not success;
        }
    }

    // Second point
    QLineF edge1(p1, p2);

    type = edge1.intersect(bigLine2, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
    }

    if (IsOutsidePoint(bigLine2.p2(), bigLine2.p1(), px))
    {
        pointsIntr.append(px);
    }
    else
    {
        pointsIntr.append(px);

        QLineF allowance(p2, px);
        allowance.setLength(allowance.length() + localWidth * 3.);
        pointsIntr.append(allowance.p2());
        pointsIntr.append(bigLine2.p1());
    }

    return pointsIntr;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VRawSAPoint> AngleByFirstSymmetry(const QVector<VRawSAPoint> &points, QPointF p1, QPointF p2, QPointF p3,
                                          const QLineF &bigLine1, QPointF sp2, const QLineF &bigLine2,
                                          const VSAPoint &p, qreal width, bool *needRollback = nullptr)
{
    {
        QLineF edge1(p2, p1);
        QLineF edge2(p2, p3);
        const qreal angle = edge1.angleTo(edge2);

        if (angle > 180)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
    }

    if (needRollback != nullptr)
    {
        *needRollback = false;
    }

    const QLineF axis = QLineF(p1, p2);

    QLineF sEdge(VPointF::FlipPF(axis, bigLine2.p1()), VPointF::FlipPF(axis, bigLine2.p2()));

    QPointF px1;
    QLineF::IntersectType type = sEdge.intersect(bigLine1, &px1);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
    }

    QPointF px2;
    type = sEdge.intersect(bigLine2, &px2);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
    }

    QVector<VRawSAPoint> pointsIntr = points;

    if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), px1))
    {
        pointsIntr.append(px1);
    }
    else
    {// Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
        bool success = false;
        QVector<VRawSAPoint> temp = pointsIntr;
        temp.append(bigLine1.p2());
        temp = VAbstractPiece::RollbackSeamAllowance(temp, sEdge, &success);

        if (success)
        {
            pointsIntr = temp;
        }

        if (needRollback != nullptr)
        {
            *needRollback = not success;
        }
    }

    if (IsOutsidePoint(bigLine2.p2(), bigLine2.p1(), px2))
    {
        pointsIntr.append(px2);
    }
    else
    {
        QLineF allowance(px2, p2);
        allowance.setAngle(allowance.angle() + 90);
        pointsIntr.append(px2);
        pointsIntr.append(allowance.p2());
        pointsIntr.append(bigLine2.p1());
    }

    return pointsIntr;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VRawSAPoint> AngleBySecondSymmetry(const QVector<VRawSAPoint> &points, QPointF p1, QPointF p2, QPointF p3,
                                           const QLineF &bigLine1, QPointF sp2, const QLineF &bigLine2,
                                           const VSAPoint &p, qreal width, bool *needRollback = nullptr)
{
    {
        QLineF edge1(p2, p1);
        QLineF edge2(p2, p3);
        const qreal angle = edge1.angleTo(edge2);

        if (angle > 180)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
    }

    if (needRollback != nullptr)
    {
        *needRollback = false;
    }

    const QLineF axis = QLineF(p3, p2);

    QLineF sEdge(VPointF::FlipPF(axis, bigLine1.p1()), VPointF::FlipPF(axis, bigLine1.p2()));

    QPointF px1;
    QLineF::IntersectType type = sEdge.intersect(bigLine1, &px1);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
    }

    QPointF px2;
    type = sEdge.intersect(bigLine2, &px2);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
    }

    const qreal localWidth = p.MaxLocalSA(width);
    QVector<VRawSAPoint> pointsIntr = points;

    if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), px1))
    {
        pointsIntr.append(px1);
    }
    else
    {// Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
        bool success = false;
        QVector<VRawSAPoint> temp = pointsIntr;
        temp.append(bigLine1.p2());
        temp = VAbstractPiece::RollbackSeamAllowance(temp, sEdge, &success);

        if (success)
        {
            pointsIntr = temp;
        }

        if (needRollback != nullptr)
        {
            *needRollback = not success;
        }
    }

    if (IsOutsidePoint(bigLine2.p2(), bigLine2.p1(), px2))
    {
        pointsIntr.append(px2);
    }
    else
    {
        QLineF allowance(p2, px2);
        allowance.setLength(p.GetSAAfter(width)*0.98);
        pointsIntr.append(allowance.p2());
        allowance.setLength(allowance.length() + localWidth * 3.);
        pointsIntr.append(allowance.p2());
        pointsIntr.append(bigLine2.p1());
    }

    return pointsIntr;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VRawSAPoint> AngleByFirstRightAngle(const QVector<VRawSAPoint> &points, QPointF p1, QPointF p2, QPointF p3,
                                            const QLineF &bigLine1, QPointF sp2, const QLineF &bigLine2,
                                            const VSAPoint &p, qreal width, bool *needRollback = nullptr)
{
    {
        QLineF edge1(p2, p1);
        QLineF edge2(p2, p3);
        const qreal angle = edge1.angleTo(edge2);

        if (angle > 270)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
    }

    const qreal localWidth = p.MaxLocalSA(width);
    QVector<VRawSAPoint> pointsRA = points;
    QLineF edge(p1, p2);

    QPointF px;
    QLineF::IntersectType type = edge.intersect(bigLine2, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
    }

    QLineF seam(px, p1);
    seam.setAngle(seam.angle()-90);
    seam.setLength(p.GetSABefore(width));

    if (IsOutsidePoint(bigLine2.p2(), bigLine2.p1(), seam.p1()) && IsSameDirection(p1, p2, px))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
        pointsRA.append(seam.p2());
        pointsRA.append(seam.p1());
    }
    else
    {
        QLineF edge1(p2, p1);
        QLineF edge2(p2, p3);
        const qreal angle = edge1.angleTo(edge2);

        if (angle > 180)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
        else
        {
            pointsRA.append(seam.p2());

            QLineF loopLine(px, sp2);
            const qreal length = loopLine.length()*0.98;
            loopLine.setLength(length);

            QLineF tmp(seam.p2(), seam.p1());
            tmp.setLength(tmp.length()+length);

            pointsRA.append(tmp.p2());
            pointsRA.append(loopLine.p2());
        }
    }

    return pointsRA;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VRawSAPoint> AngleBySecondRightAngle(QVector<VRawSAPoint> points, QPointF p1, QPointF p2, QPointF p3,
                                             const QLineF &bigLine1,  QPointF sp2, const QLineF &bigLine2,
                                             const VSAPoint &p, qreal width, bool *needRollback = nullptr)
{
    {
        QLineF edge1(p2, p1);
        QLineF edge2(p2, p3);
        const qreal angle = edge1.angleTo(edge2);

        if (angle > 270)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
    }

    if (needRollback != nullptr)
    {
        *needRollback = false;
    }

    const qreal localWidth = p.MaxLocalSA(width);
    QLineF edge(p2, p3);

    QPointF px;
    QLineF::IntersectType type = edge.intersect(bigLine1, &px);
    if (type == QLineF::NoIntersection)
    {
        return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
    }

    if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), px) && IsSameDirection(p3, p2, px))
    {
        if (QLineF(p2, px).length() > localWidth*maxL)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
        points.append(px);

        QLineF seam(px, p3);
        seam.setAngle(seam.angle()+90);
        seam.setLength(p.GetSAAfter(width));
        points.append(seam.p2());

        if (needRollback != nullptr)
        {
            *needRollback = true;
        }
    }
    else
    {
        QLineF edge1(p2, p1);
        QLineF edge2(p2, p3);
        const qreal angle = edge1.angleTo(edge2);

        if (angle > 180)
        {
            return AngleByLength(points, p1, p2, p3, bigLine1, sp2, bigLine2, p, width, needRollback);
        }
        else
        {
            // Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
            bool success = false;
            const int countBefore = points.size();
            QVector<VRawSAPoint> temp = points;
            temp.append(bigLine1.p2());
            temp = VAbstractPiece::RollbackSeamAllowance(temp, edge, &success);

            if (success)
            {
                points = temp;
            }

            if (success)
            {
                px = points.last();
            }

            if (countBefore > 0)
            {
                QLineF seam(px, p3);
                seam.setAngle(seam.angle()+90);
                seam.setLength(p.GetSAAfter(width));
                points.append(seam.p2());
            }
            else
            {
                if (needRollback != nullptr)
                {
                    *needRollback = not success;
                }
            }
        }
    }

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
template<class T>
QVector<T> CorrectPathDistortion(QVector<T> path)
{
    if (path.size() < 3)
    {
        return path;
    }

    int prev = -1;
    for (qint32 i = 0; i < path.size(); ++i)
    {
        if (prev == -1)
        {
            i == 0 ? prev = path.size() - 1 : prev = i-1;
        }

        int next = i+1;
        if (i == path.size() - 1)
        {
            next = 0;
        }

        const QPointF &iPoint = path.at(i);
        const QPointF &prevPoint = path.at(prev);
        const QPointF &nextPoint = path.at(next);

        if (VGObject::IsPointOnLineSegment(iPoint, prevPoint, nextPoint))
        {
            const QPointF p = VGObject::CorrectDistortion(iPoint, prevPoint, nextPoint);
            path[i].setX(p.x());
            path[i].setY(p.y());
        }
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
bool Rollback(QVector<VRawSAPoint> &points, const QLineF &edge)
{
    bool success = false;
    if (not points.isEmpty())
    {
        points.removeLast();
        points = VAbstractPiece::RollbackSeamAllowance(points, edge, &success);

        if (not points.isEmpty())
        {
            if (points.last().toPoint() != points.first().toPoint())
            {
                points.append(points.first());// Should be always closed
            }
        }
    }
    return success;
}

//---------------------------------------------------------------------------------------------------------------------
void RollbackByLength(QVector<VRawSAPoint> &ekvPoints, const QVector<VSAPoint> &points, qreal width)
{
    const QLineF bigLine1 = VAbstractPiece::ParallelLine(points.at(points.size()-2), points.at(0), width);

    QVector<VRawSAPoint> temp = ekvPoints;
    temp.insert(ekvPoints.size()-1, bigLine1.p2());
    bool success = Rollback(temp, VAbstractPiece::ParallelLine(points.at(0), points.at(1), width));

    if (success)
    {
        ekvPoints = temp;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void RollbackBySecondEdgeSymmetry(QVector<VRawSAPoint> &ekvPoints, const QVector<VSAPoint> &points, qreal width)
{
    const QLineF axis = QLineF(points.at(points.size()-1), points.at(1));
    const QLineF bigLine1 = VAbstractPiece::ParallelLine(points.at(points.size()-2), points.at(0), width);
    QLineF sEdge(VPointF::FlipPF(axis, bigLine1.p1()), VPointF::FlipPF(axis, bigLine1.p2()));

    QVector<VRawSAPoint> temp = ekvPoints;
    temp.insert(ekvPoints.size()-1, bigLine1.p2());
    bool success = Rollback(temp, sEdge);

    if (success)
    {
        ekvPoints = temp;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void RollbackByFirstEdgeSymmetry(QVector<VRawSAPoint> &ekvPoints, const QVector<VSAPoint> &points, qreal width)
{
    const QLineF axis = QLineF(points.at(points.size()-2), points.at(points.size()-1));
    const QLineF bigLine2 = VAbstractPiece::ParallelLine(points.at(points.size()-1), points.at(1), width);
    QLineF sEdge(VPointF::FlipPF(axis, bigLine2.p1()), VPointF::FlipPF(axis, bigLine2.p2()));
    const QLineF bigLine1 = VAbstractPiece::ParallelLine(points.at(points.size()-2), points.at(0), width);

    QVector<VRawSAPoint> temp = ekvPoints;
    temp.insert(ekvPoints.size()-1, bigLine1.p2());
    bool success = Rollback(temp, sEdge);

    if (success)
    {
        ekvPoints = temp;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void RollbackByPointsIntersection(QVector<VRawSAPoint> &ekvPoints, const QVector<VSAPoint> &points, qreal width)
{
    const QLineF bigLine1 = VAbstractPiece::ParallelLine(points.at(points.size()-2), points.at(0), width);
    QVector<VRawSAPoint> temp = ekvPoints;
    temp.insert(ekvPoints.size()-1, bigLine1.p2());
    bool success = Rollback(temp, QLineF(points.last(), points.at(1)));

    if (success)
    {
        ekvPoints = temp;
    }

    if (ekvPoints.size() > 2)
    { // Fix for the rule of main path
        ekvPoints.removeAt(ekvPoints.size()-1);
        ekvPoints.prepend(ekvPoints.at(ekvPoints.size()-1));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void RollbackBySecondEdgeRightAngle(QVector<VRawSAPoint> &ekvPoints, const QVector<VSAPoint> &points, qreal width)
{
    if (not ekvPoints.isEmpty())
    {
        const QLineF edge(points.last(), points.at(1));
        const QLineF bigLine1 = VAbstractPiece::ParallelLine(points.at(points.size()-2), points.at(0), width);

        QPointF px;
        edge.intersect(bigLine1, &px);

        ekvPoints.removeLast();

        if (IsOutsidePoint(bigLine1.p1(), bigLine1.p2(), px))
        {
            if (ekvPoints.size() > 3)
            {
                const QLineF edge1(ekvPoints.at(ekvPoints.size()-2), ekvPoints.last());
                const QLineF edge2(ekvPoints.at(0), ekvPoints.at(1));

                QPointF crosPoint;
                const QLineF::IntersectType type = edge1.intersect(edge2, &crosPoint );
                if (type == QLineF::BoundedIntersection)
                {
                    ekvPoints.removeFirst();
                    ekvPoints.removeLast();

                    ekvPoints.append(crosPoint);
                }
            }
        }
        else
        {
            bool success = false;
            QVector<VRawSAPoint> temp = ekvPoints;
            temp.insert(ekvPoints.size()-1, bigLine1.p2());
            temp = VAbstractPiece::RollbackSeamAllowance(temp, edge, &success);

            if (success)
            {
                ekvPoints = temp;
                px = ekvPoints.last();
            }

            QLineF seam(px, points.at(1));
            seam.setAngle(seam.angle()+90);
            seam.setLength(points.at(0).GetSAAfter(width));
            ekvPoints.append(seam.p2());

            if (not ekvPoints.isEmpty())
            {
                ekvPoints.append(ekvPoints.first());
            }
        }

        if (not ekvPoints.isEmpty())
        {
            if (ekvPoints.last().toPoint() != ekvPoints.first().toPoint())
            {
                ekvPoints.append(ekvPoints.first());// Should be always closed
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> CleanLoopArtifacts(const QVector<VRawSAPoint> &points)
{
    QVector<QPointF> cleaned;
    cleaned.reserve(points.size());
    for (auto &point : points)
    {
        if (not point.LoopPoint())
        {
            cleaned.append(point);
        }
    }

    return cleaned;
}
}

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &dataStream, const VAbstractPiece &piece)
{
    dataStream << *piece.d;
    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &dataStream, VAbstractPiece &piece)
{
    dataStream >> *piece.d;
    return dataStream;
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
    width = qMax(width, VSAPoint::minSAWidth);

//    DumpVector(points, QStringLiteral("input.json.XXXXXX")); // Uncomment for dumping test data

    points = CorrectEquidistantPoints(points);
    if ( points.size() < 3 )
    {
        const QString errorMsg = tr("Piece '%1'. Not enough points to build seam allowance.").arg(name);
        qApp->IsPedantic() ? throw VException(errorMsg) :
                             qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
        return QVector<QPointF>();
    }

    // Fix distorsion
    points = CorrectPathDistortion(points);

    if (points.last().toPoint() != points.first().toPoint())
    {
        points.append(points.at(0));// Should be always closed
    }

    bool needRollback = false; // no need for rollback
    QVector<VRawSAPoint> ekvPoints;
    for (qint32 i = 0; i < points.size(); ++i )
    {
        if ( i == 0)
        {//first point
            ekvPoints = EkvPoint(ekvPoints, points.at(points.size()-2), points.at(points.size()-1), points.at(1),
                                 points.at(0), width, &needRollback);
            continue;
        }

        if (i == points.size()-1)
        {//last point
            if (not ekvPoints.isEmpty())
            {
                ekvPoints.append(ekvPoints.first());
            }
            continue;
        }
        //points in the middle of polyline
        ekvPoints = EkvPoint(ekvPoints, points.at(i-1), points.at(i), points.at(i+1), points.at(i), width);
    }

    if (needRollback)
    {
        QT_WARNING_PUSH
        QT_WARNING_DISABLE_GCC("-Wswitch-default")
        // This check helps to find missed angle types in the switch
        Q_STATIC_ASSERT_X(static_cast<int>(PieceNodeAngle::LAST_ONE_DO_NOT_USE) == 7, "Not all types were handled.");
        switch (points.last().GetAngleType())
        {
            case PieceNodeAngle::LAST_ONE_DO_NOT_USE:
            case PieceNodeAngle::ByFirstEdgeRightAngle:
                Q_UNREACHABLE(); //-V501
                break;
            case PieceNodeAngle::ByLength:
            case PieceNodeAngle::ByLengthCurve:
                RollbackByLength(ekvPoints, points, width);
                break;
            case PieceNodeAngle::ByFirstEdgeSymmetry:
                RollbackByFirstEdgeSymmetry(ekvPoints, points, width);
                break;
            case PieceNodeAngle::BySecondEdgeSymmetry:
                RollbackBySecondEdgeSymmetry(ekvPoints, points, width);
                break;
            case PieceNodeAngle::ByPointsIntersection:
                RollbackByPointsIntersection(ekvPoints, points, width);
                break;
            case PieceNodeAngle::BySecondEdgeRightAngle:
                RollbackBySecondEdgeRightAngle(ekvPoints, points, width);
                break;
        }
        QT_WARNING_POP
    }

//    Uncomment for debug
//    QVector<QPointF> cleaned;
//    cleaned.reserve(ekvPoints.size());
//    for (auto &point : ekvPoints)
//    {
//        cleaned.append(point);
//    }

    const bool removeFirstAndLast = false;
    ekvPoints = RemoveDublicates(ekvPoints, removeFirstAndLast);
    QVector<QPointF> cleaned = CheckLoops(ekvPoints);//Result path can contain loops
    cleaned = CorrectEquidistantPoints(cleaned, removeFirstAndLast);
    cleaned = CorrectPathDistortion(cleaned);
//    DumpVector(cleaned, QStringLiteral("output.json.XXXXXX")); // Uncomment for dumping test data
    return cleaned;
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
QVector<QPointF> VAbstractPiece::CheckLoops(const QVector<QPointF> &points)
{
    QVector<VRawSAPoint> rawPath;
    rawPath.reserve(points.size());
    for (auto &point : points)
    {
        rawPath.append(point);
    }

    return CheckLoops(rawPath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckLoops seek and delete loops in equidistant.
 * @param points vector of points of equidistant.
 * @return vector of points of equidistant.
 */
QVector<QPointF> VAbstractPiece::CheckLoops(const QVector<VRawSAPoint> &points)
{
//    DumpVector(points); // Uncomment for dumping test data

    int count = points.size();
    /*If we got less than 4 points no need seek loops.*/
    if (count < 4)
    {
        return CleanLoopArtifacts(points);
    }

    const bool pathClosed = (points.first() == points.last());

    QVector<VRawSAPoint> ekvPoints;
    ekvPoints.reserve(points.size());

    QVector<qint32> uniqueVertices;
    uniqueVertices.reserve(4);

    qint32 i, j, jNext = 0;
    for (i = 0; i < count; ++i)
    {
        /*Last three points no need to check.*/
        /*Triangle can not contain a loop*/
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

            if (uniqueVertices.size() == 4)
            {// Lines are not neighbors
                const QLineF::IntersectType intersect = line1.intersect(line2, &crosPoint);
                if (intersect == QLineF::NoIntersection)
                { // According to the documentation QLineF::NoIntersection indicates that the lines do not intersect;
                  // i.e. they are parallel. But parallel also mean they can be on the same line.
                  // Method IsLineSegmentOnLineSegment will check it.
                    if (VGObject::IsLineSegmentOnLineSegment(line1, line2))
                    {// Now we really sure that segments are on the same line and have real intersections.
                        status = ParallelIntersection;
                        break;
                    }
                }
                else if (intersect == QLineF::BoundedIntersection)
                {
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
    const QVector<QPointF> cleaned = CleanLoopArtifacts(ekvPoints);
//    DumpVector(cleaned); // Uncomment for dumping test data
    return cleaned;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EkvPoint return seam aloowance points in place of intersection two edges. Last points of two edges should be
 * equal.
 * @param width global seam allowance width.
 * @return seam aloowance points.
 */
QVector<VRawSAPoint> VAbstractPiece::EkvPoint(QVector<VRawSAPoint> points, const VSAPoint &p1Line1,
                                              const VSAPoint &p2Line1, const VSAPoint &p1Line2, const VSAPoint &p2Line2,
                                              qreal width, bool *needRollback)
{
    if (width < 0)
    { // width can't be < 0
        return QVector<VRawSAPoint>();
    }

    width = qMax(width, VSAPoint::minSAWidth);

    if (p2Line1 != p2Line2)
    {
        qDebug()<<"Last points of two lines must be equal.";
        return QVector<VRawSAPoint>(); // Wrong edges
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
            auto IsOnLine = [](const QPointF &base, const QPointF &sp1, const QPointF &sp2, qreal accuracy)
            {
                if (not VFuzzyComparePoints(base, sp1))
                {
                    return VGObject::IsPointOnLineviaPDP(sp2, base, sp1, accuracy);
                }

                if (not VFuzzyComparePoints(base, sp2))
                {
                    return VGObject::IsPointOnLineviaPDP(sp1, base, sp2, accuracy);
                }
                return true;
            };
            if (VGObject::IsPointOnLineSegment(p2Line1, p1Line1, p1Line2, ToPixel(1, Unit::Mm)) &&
                    IsOnLine(p2Line1, bigLine1.p2(), bigLine2.p1(), ToPixel(1, Unit::Mm)))
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
            if (angle < 135
                || VFuzzyComparePossibleNulls(angle, 135.0))// Go in a same direction
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
                        return AngleByLength(points, p1Line1, p2Line1, p1Line2, bigLine1, crosPoint, bigLine2, p2Line1,
                                             width, needRollback);
                    case PieceNodeAngle::ByPointsIntersection:
                        return AngleByIntersection(points, p1Line1, p2Line1, p1Line2, bigLine1, crosPoint, bigLine2,
                                                   p2Line1, width, needRollback);
                    case PieceNodeAngle::ByFirstEdgeSymmetry:
                        return AngleByFirstSymmetry(points, p1Line1, p2Line1, p1Line2, bigLine1, crosPoint, bigLine2,
                                                    p2Line1, width, needRollback);
                    case PieceNodeAngle::BySecondEdgeSymmetry:
                        return AngleBySecondSymmetry(points, p1Line1, p2Line1, p1Line2, bigLine1, crosPoint, bigLine2,
                                                     p2Line1, width, needRollback);
                    case PieceNodeAngle::ByFirstEdgeRightAngle:
                        return AngleByFirstRightAngle(points, p1Line1, p2Line1, p1Line2, bigLine1, crosPoint, bigLine2,
                                                      p2Line1, width, needRollback);
                    case PieceNodeAngle::BySecondEdgeRightAngle:
                        return AngleBySecondRightAngle(points, p1Line1, p2Line1, p1Line2, bigLine1, crosPoint, bigLine2,
                                                       p2Line1, width, needRollback);
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
    return QLineF(SingleParallelPoint(p1, p2, 90, p1.GetSAAfter(width)),
                  SingleParallelPoint(p2, p1, -90, p2.GetSABefore(width)));
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::IsAllowanceValid(const QVector<QPointF> &base, const QVector<QPointF> &allowance)
{
    if (base.size() < 3 || allowance.size() < 3)
    {
        return false; // Not enough data
    }

//    DumpVector(base); // Uncomment for dumping test data
//    DumpVector(allowance); // Uncomment for dumping test data

    const qreal baseDirection = VPiece::SumTrapezoids(base);
    const qreal allowanceDirection = VPiece::SumTrapezoids(allowance);

    if (baseDirection >= 0 || allowanceDirection >= 0)
    {
        return false; // Wrong direction
    }

    for (auto i = 0; i < base.count(); ++i)
    {
        int nextI = -1;
        if (i < base.count()-1)
        {
            nextI = i + 1;
        }
        else
        {
            nextI = 0;
        }

        QLineF baseSegment(base.at(i), base.at(nextI));
        if (baseSegment.isNull())
        {
            continue;
        }

        for (auto j = 0; j < allowance.count(); ++j)
        {
            int nextJ = -1;
            if (j < allowance.count()-1)
            {
                nextJ = j + 1;
            }
            else
            {
                nextJ = 0;
            }

            QLineF allowanceSegment(allowance.at(j), allowance.at(nextJ));
            if (allowanceSegment.isNull())
            {
                continue;
            }

            QPointF crosPoint;
            const auto type = baseSegment.intersect(allowanceSegment, &crosPoint);
            if (type == QLineF::BoundedIntersection
                    && not VFuzzyComparePoints(baseSegment.p1(), crosPoint)
                    && not VFuzzyComparePoints(baseSegment.p2(), crosPoint)
                    && not VGObject::IsPointOnLineviaPDP(allowanceSegment.p1(), baseSegment.p1(), baseSegment.p2())
                    && not VGObject::IsPointOnLineviaPDP(allowanceSegment.p2(), baseSegment.p1(), baseSegment.p2()))
            {
                return false;
            }
        }
    }

    return true;
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

    bool seamOnLine = VGObject::IsPointOnLineviaPDP(iPoint, prevPoint, nextPoint);
    bool sa1OnLine = VGObject::IsPointOnLineviaPDP(bigLine1.p2(), bigLine1.p1(), bigLine2.p2());
    bool sa2OnLine = VGObject::IsPointOnLineviaPDP(bigLine2.p1(), bigLine1.p1(), bigLine2.p2());
    bool saDiff = qAbs(prevPoint.GetSAAfter(tmpWidth) - nextPoint.GetSABefore(tmpWidth)) < accuracyPointOnLine;

    // left point that splits a curve
    bool curve = (prevPoint.GetAngleType() == PieceNodeAngle::ByLengthCurve &&
                  iPoint.GetAngleType() == PieceNodeAngle::ByLengthCurve) ||
                 (nextPoint.GetAngleType() == PieceNodeAngle::ByLengthCurve &&
                  iPoint.GetAngleType() == PieceNodeAngle::ByLengthCurve);

    return seamOnLine && sa1OnLine && sa2OnLine && saDiff && not curve;
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
uint VAbstractPiece::GetPriority() const
{
    return d->m_priority;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPiece::SetPriority(uint value)
{
    d->m_priority = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSAPoint::GetSABefore(qreal width) const
{
    if (m_before < 0)
    {
        return width;
    }
    return qMax(m_before, minSAWidth);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSAPoint::GetSAAfter(qreal width) const
{
    if (m_after < 0)
    {
        return width;
    }
    return qMax(m_after, minSAWidth);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSAPoint::MaxLocalSA(qreal width) const
{
    return qMax(GetSAAfter(width), GetSABefore(width));
}

//---------------------------------------------------------------------------------------------------------------------
qreal VSAPoint::PassmarkLength(qreal width) const
{
    if (not m_manualPassmarkLength)
    {
        qreal passmarkLength = MaxLocalSA(width) * passmarkFactor;
        passmarkLength = qMin(passmarkLength, maxPassmarkLength);
        return passmarkLength;
    }
    else
    {
        return m_passmarkLength;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QJsonObject VSAPoint::toJson() const
{
    QJsonObject pointObject;
    pointObject[QLatin1String("type")] = "VSAPoint";
    pointObject[QLatin1String("x")] = x();
    pointObject[QLatin1String("y")] = y();

    if (not VFuzzyComparePossibleNulls(m_before, -1))
    {
        pointObject[QLatin1String("saBefore")] = m_before;
    }

    if (not VFuzzyComparePossibleNulls(m_after, -1))
    {
        pointObject[QLatin1String("saAfter")] = m_after;
    }

    if (m_angle != PieceNodeAngle::ByLength)
    {
        pointObject[QLatin1String("angle")] = static_cast<int>(m_angle);
    }

    return pointObject;
}

//---------------------------------------------------------------------------------------------------------------------
// Because artificial loop can lead to wrong clipping we must rollback current seam allowance points
QVector<VRawSAPoint> VAbstractPiece::RollbackSeamAllowance(QVector<VRawSAPoint> points, const QLineF &cuttingEdge,
                                                           bool *success)
{
    *success = false;
    QVector<VRawSAPoint> clipped;
    clipped.reserve(points.count()+1);
    for (int i = points.count()-1; i > 0; --i)
    {
        QLineF segment(points.at(i), points.at(i-1));
        QPointF crosPoint;
        const QLineF::IntersectType type = cuttingEdge.intersect(segment, &crosPoint);
        if (type != QLineF::NoIntersection
                && VGObject::IsPointOnLineSegment(crosPoint, segment.p1(), segment.p2())
                && IsSameDirection(cuttingEdge.p2(), cuttingEdge.p1(), crosPoint))
        {
            clipped.append(crosPoint);
            for (int j=i-1; j>=0; --j)
            {
                clipped.append(points.at(j));
            }
            points = VGObject::GetReversePoints(clipped);
            *success = true;
            break;
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
bool VAbstractPiece::IsItemContained(const QRectF &parentBoundingRect, const QVector<QPointF> &shape, qreal &dX,
                                     qreal &dY)
{
    dX = 0;
    dY = 0;
    // single point differences
    bool bInside = true;

    for (auto p : shape)
    {
        qreal dPtX = 0;
        qreal dPtY = 0;
        if (not parentBoundingRect.contains(p))
        {
            if (p.x() < parentBoundingRect.left())
            {
                dPtX = parentBoundingRect.left() - p.x();
            }
            else if (p.x() > parentBoundingRect.right())
            {
                dPtX = parentBoundingRect.right() - p.x();
            }

            if (p.y() < parentBoundingRect.top())
            {
                dPtY = parentBoundingRect.top() - p.y();
            }
            else if (p.y() > parentBoundingRect.bottom())
            {
                dPtY = parentBoundingRect.bottom() - p.y();
            }

            if (qAbs(dPtX) > qAbs(dX))
            {
                dX = dPtX;
            }

            if (qAbs(dPtY) > qAbs(dY))
            {
                dY = dPtY;
            }

            bInside = false;
        }
    }
    return bInside;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::CorrectPosition(const QRectF &parentBoundingRect, QVector<QPointF> points)
{
    qreal dX = 0;
    qreal dY = 0;
    if (not IsItemContained(parentBoundingRect, points, dX, dY))
    {
        for (int i =0; i < points.size(); ++i)
        {
            points[i] = QPointF(points.at(i).x() + dX, points.at(i).y() + dY);
        }
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractPiece::FindGrainlineGeometry(const VGrainlineData& geom, const VContainer *pattern, qreal &length,
                                           qreal &rotationAngle, QPointF &pos)
{
    SCASSERT(pattern != nullptr)

    const quint32 topPin = geom.TopPin();
    const quint32 bottomPin = geom.BottomPin();

    if (topPin != NULL_ID && bottomPin != NULL_ID)
    {
        try
        {
            const auto topPinPoint = pattern->GeometricObject<VPointF>(topPin);
            const auto bottomPinPoint = pattern->GeometricObject<VPointF>(bottomPin);

            QLineF grainline(static_cast<QPointF>(*bottomPinPoint), static_cast<QPointF>(*topPinPoint));
            length = grainline.length();
            rotationAngle = grainline.angle();

            if (not VFuzzyComparePossibleNulls(rotationAngle, 0))
            {
                grainline.setAngle(0);
            }

            pos = grainline.p1();
            rotationAngle = qDegreesToRadians(rotationAngle);

            return true;
        }
        catch(const VExceptionBadId &)
        {
            // do nothing.
        }
    }

    try
    {
        Calculator cal1;
        rotationAngle = cal1.EvalFormula(pattern->DataVariables(), geom.GetRotation());
        rotationAngle = qDegreesToRadians(rotationAngle);

        Calculator cal2;
        length = cal2.EvalFormula(pattern->DataVariables(), geom.GetLength());
        length = ToPixel(length, *pattern->GetPatternUnit());
    }
    catch(qmu::QmuParserError &e)
    {
        Q_UNUSED(e);
        return false;
    }

    const quint32 centerPin = geom.CenterPin();
    if (centerPin != NULL_ID)
    {
        try
        {
            const auto centerPinPoint = pattern->GeometricObject<VPointF>(centerPin);

            QLineF grainline(centerPinPoint->x(), centerPinPoint->y(),
                             centerPinPoint->x() + length / 2.0, centerPinPoint->y());

            grainline.setAngle(qRadiansToDegrees(rotationAngle));
            grainline = QLineF(grainline.p2(), grainline.p1());
            grainline.setLength(length);

            pos = grainline.p2();
        }
        catch(const VExceptionBadId &)
        {
            pos = geom.GetPos();
        }
    }
    else
    {
        pos = geom.GetPos();
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VAbstractPiece::GrainlinePoints(const VGrainlineData &geom, const VContainer *pattern,
                                                 const QRectF &boundingRect, qreal &dAng)
{
    SCASSERT(pattern != nullptr)

    QPointF pt1;
    qreal dLen = 0;
    if ( not FindGrainlineGeometry(geom, pattern, dLen, dAng, pt1))
    {
        return QVector<QPointF>();
    }

    qreal rotation = dAng;

    QPointF pt2(pt1.x() + dLen * qCos(rotation), pt1.y() - dLen * qSin(rotation));

    const qreal dArrowLen = ToPixel(0.5, *pattern->GetPatternUnit());
    const qreal dArrowAng = M_PI/9;

    QVector<QPointF> v;
    v << pt1;

    if (geom.GetArrowType() != ArrowType::atFront)
    {
        v << QPointF(pt1.x() + dArrowLen * qCos(rotation + dArrowAng),
                     pt1.y() - dArrowLen * qSin(rotation + dArrowAng));
        v << QPointF(pt1.x() + dArrowLen * qCos(rotation - dArrowAng),
                     pt1.y() - dArrowLen * qSin(rotation - dArrowAng));
        v << pt1;
    }

    v << pt2;

    if (geom.GetArrowType() != ArrowType::atRear)
    {
        rotation += M_PI;

        v << QPointF(pt2.x() + dArrowLen * qCos(rotation + dArrowAng),
                     pt2.y() - dArrowLen * qSin(rotation + dArrowAng));
        v << QPointF(pt2.x() + dArrowLen * qCos(rotation - dArrowAng),
                     pt2.y() - dArrowLen * qSin(rotation - dArrowAng));
        v << pt2;
    }

    return CorrectPosition(boundingRect, v);
}
