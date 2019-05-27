/************************************************************************
 **
 **  @file   vpassmark.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 5, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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

#include "vpassmark.h"
#include "../vmisc/vabstractapplication.h"
#include "../ifc/exception/vexceptioninvalidnotch.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"

const qreal VPassmark::passmarkRadiusFactor = 0.45;

namespace
{
//---------------------------------------------------------------------------------------------------------------------
PassmarkStatus GetSeamPassmarkSAPoint(const VPiecePassmarkData &passmarkData, const QVector<QPointF> &seamAllowance,
                                      QPointF &point)
{
    bool needRollback = false; // no need for rollback
    QVector<QPointF> ekvPoints;
    ekvPoints = VAbstractPiece::EkvPoint(ekvPoints, passmarkData.previousSAPoint, passmarkData.passmarkSAPoint,
                                         passmarkData.nextSAPoint, passmarkData.passmarkSAPoint, passmarkData.saWidth,
                                         &needRollback);

    if (needRollback && not seamAllowance.isEmpty())
    {
        ekvPoints.clear();
        ekvPoints += seamAllowance.at(seamAllowance.size()-2);
    }

    if (ekvPoints.isEmpty())
    { // Just in case
        return PassmarkStatus::Error; // Something wrong
    }

    if (ekvPoints.size() == 1 || ekvPoints.size() > 2)
    {
        point = ekvPoints.first();
    }
    else if (ekvPoints.size() == 2)
    {
        if(passmarkData.passmarkSAPoint.GetAngleType() == PieceNodeAngle::ByFirstEdgeSymmetry ||
                passmarkData.passmarkSAPoint.GetAngleType() == PieceNodeAngle::ByFirstEdgeRightAngle)
        {
            point = ekvPoints.first();
        }
        else
        {
            QLineF line = QLineF(ekvPoints.at(0), ekvPoints.at(1));
            line.setLength(line.length()/2.);
            point = line.p2();
        }
    }
    return needRollback ? PassmarkStatus::Rollback : PassmarkStatus::Common;
}

//---------------------------------------------------------------------------------------------------------------------
bool FixNotchPoint(const QVector<QPointF> &seamAllowance, const QPointF &notchBase, QPointF *notch)
{
    bool fixed = true;
    if (not VAbstractCurve::IsPointOnCurve(seamAllowance, *notch))
    {
        fixed = false;
        QLineF axis = QLineF(notchBase, *notch);
        axis.setLength(ToPixel(50, Unit::Cm));
        const QVector<QPointF> points = VAbstractCurve::CurveIntersectLine(seamAllowance, axis);

        if (points.size() > 0)
        {
            if (points.size() == 1)
            {
                *notch = points.at(0);
                fixed = true;
            }
            else
            {
                QMap<qreal, int> forward;

                for ( qint32 i = 0; i < points.size(); ++i )
                {
                    if (points.at(i) == notchBase)
                    { // Always seek unique intersection
                        continue;
                    }

                    const QLineF length(notchBase, points.at(i));
                    if (qAbs(length.angle() - axis.angle()) < 0.1)
                    {
                        forward.insert(length.length(), i);
                    }
                }


                // Closest point is not always want we need. First return point in forward direction if exists.
                if (not forward.isEmpty())
                {
                    *notch = points.at(forward.first());
                    fixed = true;
                }
            }
        }
    }
    else
    { // Fixing distortion
        QLineF axis = QLineF(notchBase, *notch);
        axis.setLength(axis.length() + accuracyPointOnLine * 10);
        const QVector<QPointF> points = VAbstractCurve::CurveIntersectLine(seamAllowance, axis);
        if (points.size() == 1)
        {
            *notch = points.first();
        }
    }

    return fixed;
}
const qreal passmarkGap = (1.5/*mm*/ / 25.4) * PrintDPI;

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateOnePassmarkLines(const QLineF &line)
{
    return QVector<QLineF>({line});
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateTwoPassmarkLines(const QLineF &line, const QVector<QPointF> &seamAllowance)
{
    QPointF l1p1;
    {
        QLineF line1 = line;
        line1.setAngle(line1.angle() + 90);
        line1.setLength(passmarkGap/2.);
        l1p1 = line1.p2();
    }

    QPointF l2p1;
    {
        QLineF line2 = line;
        line2.setAngle(line2.angle() - 90);
        line2.setLength(passmarkGap/2.);
        l2p1 = line2.p2();
    }

    QPointF l1p2;
    {
        QLineF line1 = QLineF(line.p2(), line.p1());
        line1.setAngle(line1.angle() - 90);
        line1.setLength(passmarkGap/2.);
        l1p2 = line1.p2();
    }

    QPointF l2p2;
    {
        QLineF line2 = QLineF(line.p2(), line.p1());
        line2.setAngle(line2.angle() + 90);
        line2.setLength(passmarkGap/2.);
        l2p2 = line2.p2();
    }

    QVector<QLineF> lines;
    QLineF seg = VPassmark::FindIntersection(QLineF(l1p2, l1p1), seamAllowance);
    lines.append(QLineF(seg.p2(), seg.p1()));

    seg = VPassmark::FindIntersection(QLineF(l2p2, l2p1), seamAllowance);
    lines.append(QLineF(seg.p2(), seg.p1()));
    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateThreePassmarkLines(const QLineF &line, const QVector<QPointF> &seamAllowance)
{
    QPointF l1p1;
    {
        QLineF line1 = line;
        line1.setAngle(line1.angle() + 90);
        line1.setLength(passmarkGap);
        l1p1 = line1.p2();
    }

    QPointF l2p1;
    {
        QLineF line2 = line;
        line2.setAngle(line2.angle() - 90);
        line2.setLength(passmarkGap);
        l2p1 = line2.p2();
    }

    QPointF l1p2;
    {
        QLineF line1 = QLineF(line.p2(), line.p1());
        line1.setAngle(line1.angle() - 90);
        line1.setLength(passmarkGap);
        l1p2 = line1.p2();
    }

    QPointF l2p2;
    {
        QLineF line2 = QLineF(line.p2(), line.p1());
        line2.setAngle(line2.angle() + 90);
        line2.setLength(passmarkGap);
        l2p2 = line2.p2();
    }

    QVector<QLineF> lines;
    QLineF seg = VPassmark::FindIntersection(QLineF(l1p2, l1p1), seamAllowance);
    lines.append(QLineF(seg.p2(), seg.p1()));

    lines.append(line);

    seg = VPassmark::FindIntersection(QLineF(l2p2, l2p1), seamAllowance);
    lines.append(QLineF(seg.p2(), seg.p1()));
    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateTMarkPassmark(const QLineF &line)
{
    QPointF p1;
    {
        QLineF tmpLine = QLineF(line.p2(), line.p1());
        tmpLine.setAngle(tmpLine.angle() - 90);
        tmpLine.setLength(line.length() * 0.75 / 2);
        p1 = tmpLine.p2();
    }

    QPointF p2;
    {
        QLineF tmpLine = QLineF(line.p2(), line.p1());
        tmpLine.setAngle(tmpLine.angle() + 90);
        tmpLine.setLength(line.length() * 0.75 / 2);
        p2 = tmpLine.p2();
    }

    QVector<QLineF> lines;
    lines.append(line);
    lines.append(QLineF(p1, p2));
    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateVMarkPassmark(const QLineF &line)
{
    QLineF l1 = line;
    l1.setAngle(l1.angle() - 35);

    QLineF l2 = line;
    l2.setAngle(l2.angle() + 35);

    QVector<QLineF> lines;
    lines.append(l1);
    lines.append(l2);
    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateVMark2Passmark(const QLineF &line, const QVector<QPointF> &seamAllowance)
{
    QLineF l1 = QLineF(line.p2(), line.p1());
    l1.setAngle(l1.angle() + 35);

    QLineF l2 = QLineF(line.p2(), line.p1());
    l2.setAngle(l2.angle() - 35);

    QVector<QLineF> lines;
    lines.append(VPassmark::FindIntersection(l1, seamAllowance));
    lines.append(VPassmark::FindIntersection(l2, seamAllowance));
    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> PointsToSegments(const QVector<QPointF> &points)
{
    QVector<QLineF> lines;
    if (points.size() >= 2)
    {
        for (int i=0; i < points.size()-1; ++i)
        {
            QLineF segment = QLineF(points.at(i), points.at(i+1));
            if (segment.length() > 0)
            {
                lines.append(segment);
            }
        }
    }
    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateUMarkPassmark(const QLineF &line, const QVector<QPointF> &seamAllowance)
{
    const qreal radius = line.length() * VPassmark::passmarkRadiusFactor;

    QLineF baseLine = line;
    baseLine.setLength(baseLine.length() - radius); // keep defined depth

    QPointF l1p1;
    {
        QLineF line1 = baseLine;
        line1.setAngle(line1.angle() + 90);
        line1.setLength(radius);
        l1p1 = line1.p2();
    }

    QPointF l2p1;
    {
        QLineF line2 = baseLine;
        line2.setAngle(line2.angle() - 90);
        line2.setLength(radius);
        l2p1 = line2.p2();
    }

    QPointF l1p2;
    {
        QLineF line1 = QLineF(baseLine.p2(), baseLine.p1());
        line1.setAngle(line1.angle() - 90);
        line1.setLength(radius);
        l1p2 = line1.p2();
    }

    QPointF l2p2;
    {
        QLineF line2 = QLineF(baseLine.p2(), baseLine.p1());
        line2.setAngle(line2.angle() + 90);
        line2.setLength(radius);
        l2p2 = line2.p2();
    }

    QLineF axis = QLineF(baseLine.p2(), baseLine.p1());
    axis.setLength(radius);

    QVector<QPointF> points;

    QLineF seg = VPassmark::FindIntersection(QLineF(l2p2, l2p1), seamAllowance);
    seg = QLineF(seg.p2(), seg.p1());
    points.append(seg.p1());
    points.append(seg.p2());

    VArc arc(VPointF(baseLine.p2()), radius, QLineF(baseLine.p2(), l2p2).angle(), QLineF(baseLine.p2(), l1p2).angle());
    arc.SetApproximationScale(10);
    points += arc.GetPoints();

    seg = VPassmark::FindIntersection(QLineF(l1p2, l1p1), seamAllowance);
    seg = QLineF(seg.p2(), seg.p1());
    points.append(seg.p2());
    points.append(seg.p1());

    return PointsToSegments(points);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateBoxMarkPassmark(const QLineF &line, const QVector<QPointF> &seamAllowance)
{
    const qreal radius = line.length() * VPassmark::passmarkRadiusFactor;

    QPointF l1p1;
    {
        QLineF line1 = line;
        line1.setAngle(line1.angle() + 90);
        line1.setLength(radius);
        l1p1 = line1.p2();
    }

    QPointF l2p1;
    {
        QLineF line2 = line;
        line2.setAngle(line2.angle() - 90);
        line2.setLength(radius);
        l2p1 = line2.p2();
    }

    QPointF l1p2;
    {
        QLineF line1 = QLineF(line.p2(), line.p1());
        line1.setAngle(line1.angle() - 90);
        line1.setLength(radius);
        l1p2 = line1.p2();
    }

    QPointF l2p2;
    {
        QLineF line2 = QLineF(line.p2(), line.p1());
        line2.setAngle(line2.angle() + 90);
        line2.setLength(radius);
        l2p2 = line2.p2();
    }

    QVector<QPointF> points;

    QLineF seg = VPassmark::FindIntersection(QLineF(l1p2, l1p1), seamAllowance);
    points.append(seg.p2());
    points.append(seg.p1());

    seg = VPassmark::FindIntersection(QLineF(l2p2, l2p1), seamAllowance);
    points.append(seg.p1());
    points.append(seg.p2());

    return PointsToSegments(points);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreatePassmarkLines(PassmarkLineType lineType, PassmarkAngleType angleType,
                                    const QVector<QLineF> &lines, const QVector<QPointF> &seamAllowance,
                                    PassmarkSide side)
{
    if (lines.isEmpty())
    {
        return QVector<QLineF>();
    }

    QVector<QLineF> passmarksLines;

    auto CreateLinesWithCorrection = [&passmarksLines, side, angleType, lines, seamAllowance]
            (QVector<QLineF> (*create)(const QLineF &, const QVector<QPointF> &))
    {
        if (angleType == PassmarkAngleType::Straightforward)
        {
            passmarksLines += (*create)(lines.first(), seamAllowance);
        }
        else
        {
            if (side == PassmarkSide::All || side == PassmarkSide::Left)
            {
                passmarksLines += (*create)(lines.first(), seamAllowance);
            }

            if (side == PassmarkSide::All || side == PassmarkSide::Right)
            {
                passmarksLines += (*create)(lines.last(), seamAllowance);
            }
        }
    };

    auto CreateLines = [&passmarksLines, side, angleType, lines](QVector<QLineF> (*create)(const QLineF &))
    {
        if (angleType == PassmarkAngleType::Straightforward)
        {
            passmarksLines += (*create)(lines.first());
        }
        else
        {
            if (side == PassmarkSide::All || side == PassmarkSide::Left)
            {
                passmarksLines += (*create)(lines.first());
            }

            if (side == PassmarkSide::All || side == PassmarkSide::Right)
            {
                passmarksLines += (*create)(lines.last());
            }
        }
    };

    if (angleType == PassmarkAngleType::Straightforward
            || angleType == PassmarkAngleType::Intersection
            || angleType == PassmarkAngleType::IntersectionOnlyLeft
            || angleType == PassmarkAngleType::IntersectionOnlyRight
            || angleType == PassmarkAngleType::Intersection2
            || angleType == PassmarkAngleType::Intersection2OnlyLeft
            || angleType == PassmarkAngleType::Intersection2OnlyRight)
    {
        switch (lineType)
        {
            case PassmarkLineType::TwoLines:
                CreateLinesWithCorrection(CreateTwoPassmarkLines);
                break;
            case PassmarkLineType::ThreeLines:
                CreateLinesWithCorrection(CreateThreePassmarkLines);
                break;
            case PassmarkLineType::TMark:
                CreateLines(CreateTMarkPassmark);
                break;
            case PassmarkLineType::VMark:
                CreateLines(CreateVMarkPassmark);
                break;
            case PassmarkLineType::VMark2:
                CreateLinesWithCorrection(CreateVMark2Passmark);
                break;
            case PassmarkLineType::UMark:
                CreateLinesWithCorrection(CreateUMarkPassmark);
                break;
            case PassmarkLineType::BoxMark:
                CreateLinesWithCorrection(CreateBoxMarkPassmark);
                break;
            case PassmarkLineType::OneLine:
            default:
                CreateLines(CreateOnePassmarkLines);
                break;
        }
    }
    else
    {
        switch (lineType)
        {
            case PassmarkLineType::TMark:
                passmarksLines += CreateTMarkPassmark(lines.first());
                break;
            case PassmarkLineType::OneLine:
            case PassmarkLineType::TwoLines:
            case PassmarkLineType::ThreeLines:
            case PassmarkLineType::VMark:
            case PassmarkLineType::VMark2:
            case PassmarkLineType::UMark:
            case PassmarkLineType::BoxMark:
            default:
                passmarksLines.append(lines.first());
                break;
        }
    }

    return passmarksLines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> PassmarkBisectorBaseLine(PassmarkStatus seamPassmarkType, const VPiecePassmarkData &passmarkData,
                                         const QPointF &seamPassmarkSAPoint, const QVector<QPointF> &seamAllowance)
{
    QLineF edge1;
    QLineF edge2;

    if (seamPassmarkType == PassmarkStatus::Common)
    {
        if (passmarkData.passmarkSAPoint.GetAngleType() == PieceNodeAngle::ByFirstEdgeSymmetry)
        {
            edge1 = QLineF(seamPassmarkSAPoint, seamAllowance.at(seamAllowance.size() - 2));
            edge2 = QLineF(seamPassmarkSAPoint, seamAllowance.at(1));
        }
        else
        {
            const QLineF bigLine1 = VAbstractPiece::ParallelLine(passmarkData.previousSAPoint,
                                                                 passmarkData.passmarkSAPoint, passmarkData.saWidth );
            const QLineF bigLine2 = VAbstractPiece::ParallelLine(passmarkData.passmarkSAPoint, passmarkData.nextSAPoint,
                                                                 passmarkData.saWidth );

            edge1 = QLineF(seamPassmarkSAPoint, bigLine1.p1());
            edge2 = QLineF(seamPassmarkSAPoint, bigLine2.p2());
        }
    }
    else if(seamPassmarkType == PassmarkStatus::Rollback)
    {
        edge1 = QLineF(seamPassmarkSAPoint, seamAllowance.at(seamAllowance.size() - 3));
        edge2 = QLineF(seamPassmarkSAPoint, seamAllowance.at(0));
    }
    else
    { // Should never happen
        return QVector<QLineF>();
    }

    const qreal length = passmarkData.passmarkSAPoint.PassmarkLength(passmarkData.saWidth);
    if (not passmarkData.passmarkSAPoint.IsManualPasskmarkLength() && length <= accuracyPointOnLine)
    {
        const QString errorMsg = QObject::tr("Found null notch for point '%1' in piece '%2'. Length is less "
                                             "than minimal allowed.")
                .arg(passmarkData.nodeName, passmarkData.pieceName);
        qApp->IsPedantic() ? throw VException(errorMsg) : qWarning() << errorMsg;
        return QVector<QLineF>();
    }

    edge1.setAngle(edge1.angle() + edge1.angleTo(edge2)/2.);
    edge1.setLength(length);

    return QVector<QLineF>({edge1});
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath PassmarkToPath(const QVector<QLineF> &passmark)
{
    QPainterPath path;
    if (not passmark.isEmpty())
    {
        for (qint32 i = 0; i < passmark.count(); ++i)
        {
            path.moveTo(passmark.at(i).p1());
            path.lineTo(passmark.at(i).p2());
        }

        path.setFillRule(Qt::WindingFill);
    }
    return path;
}
}

//---------------------------------------------------------------------------------------------------------------------
VPassmark::VPassmark()
{}

//---------------------------------------------------------------------------------------------------------------------
VPassmark::VPassmark(const VPiecePassmarkData &data)
    : m_data(data),
      m_null(false)
{
    // Correct distorsion
    if (VGObject::IsPointOnLineSegment(m_data.passmarkSAPoint, m_data.previousSAPoint,
                                       m_data.nextSAPoint))
    {
        const QPointF p = VGObject::CorrectDistortion(m_data.passmarkSAPoint, m_data.previousSAPoint,
                                                      m_data.nextSAPoint);
        m_data.passmarkSAPoint.setX(p.x());
        m_data.passmarkSAPoint.setY(p.y());
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPassmark::FullPassmark(const VPiece &piece, const VContainer *data) const
{
    if (m_null)
    {
        return QVector<QLineF>();
    }

    if (not piece.IsSeamAllowanceBuiltIn())
    {
        QVector<QLineF> lines;
        lines += SAPassmark(piece, data, PassmarkSide::All);
        if (qApp->Settings()->IsDoublePassmark()
                && not piece.IsHideMainPath()
                && m_data.isMainPathNode
                && m_data.passmarkAngleType != PassmarkAngleType::Intersection
                && m_data.passmarkAngleType != PassmarkAngleType::IntersectionOnlyLeft
                && m_data.passmarkAngleType != PassmarkAngleType::IntersectionOnlyRight
                && m_data.passmarkAngleType != PassmarkAngleType::Intersection2
                && m_data.passmarkAngleType != PassmarkAngleType::Intersection2OnlyLeft
                && m_data.passmarkAngleType != PassmarkAngleType::Intersection2OnlyRight
                && m_data.isShowSecondPassmark)
        {
            lines += BuiltInSAPassmark(piece, data);
        }
        return lines;
    }

    return BuiltInSAPassmark(piece, data);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPassmark::SAPassmark(const VPiece &piece, const VContainer *data, PassmarkSide side) const
{
    if (m_null)
    {
        return QVector<QLineF>();
    }

    if (not piece.IsSeamAllowanceBuiltIn())
    {
        // Because rollback cannot be calulated if passmark is not first point in main path we rotate it.
        return SAPassmark(piece.SeamAllowancePointsWithRotation(data, m_data.passmarkIndex), side);
    }

    return QVector<QLineF>();
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPassmark::SAPassmark(const QVector<QPointF> &seamAllowance, PassmarkSide side) const
{
    if (m_null)
    {
        return QVector<QLineF>();
    }

    // Because rollback @seamAllowance must be rotated here.
    return MakeSAPassmark(seamAllowance, side);
}

//---------------------------------------------------------------------------------------------------------------------
bool VPassmark::IsNull() const
{
    return m_null;
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePassmarkData VPassmark::Data() const
{
    return m_data;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VPassmark::FindIntersection(const QLineF &line, const QVector<QPointF> &seamAllowance)
{
    QLineF testLine = line;
    testLine.setLength(testLine.length()*10);
    QVector<QPointF> intersections = VAbstractCurve::CurveIntersectLine(seamAllowance, testLine);
    if (not intersections.isEmpty())
    {
        return QLineF(line.p1(), intersections.last());
    }

    return line;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPassmark::MakeSAPassmark(const QVector<QPointF> &seamAllowance, PassmarkSide side) const
{
    const QVector<QLineF> lines = SAPassmarkBaseLine(seamAllowance, side);
    if (lines.isEmpty())
    {
        return QVector<QLineF>();
    }

    return CreatePassmarkLines(m_data.passmarkLineType, m_data.passmarkAngleType, lines, seamAllowance, side);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPassmark::BuiltInSAPassmark(const VPiece &piece, const VContainer *data) const
{
    if (m_null)
    {
        return QVector<QLineF>();
    }

    const QVector<QLineF> lines = BuiltInSAPassmarkBaseLine(piece);
    if (lines.isEmpty())
    {
        return QVector<QLineF>();
    }

    return CreatePassmarkLines(m_data.passmarkLineType, m_data.passmarkAngleType, lines, piece.MainPathPoints(data),
                               PassmarkSide::All);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPassmark::BuiltInSAPassmarkBaseLine(const VPiece &piece) const
{
    if (m_null)
    {
        return QVector<QLineF>();
    }

    qreal length = 0;
    if (not piece.IsSeamAllowanceBuiltIn())
    {
        length = m_data.passmarkSAPoint.PassmarkLength(m_data.saWidth);
        if (not m_data.passmarkSAPoint.IsManualPasskmarkLength() && length <= accuracyPointOnLine)
        {
            const QString errorMsg = QObject::tr("Found null notch for point '%1' in piece '%2'. Length is less "
                                                 "than minimal allowed.")
                    .arg(m_data.nodeName, m_data.pieceName);
            qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
            return QVector<QLineF>();
        }
    }
    else
    {
        if (m_data.passmarkSAPoint.IsManualPasskmarkLength())
        {
            length = m_data.passmarkSAPoint.GetPasskmarkLength();
        }
        else
        {
            const QString errorMsg = QObject::tr("Cannot calculate a notch for point '%1' in piece '%2' with built in "
                                                 "seam allowance. User must manually provide length.")
                    .arg(m_data.nodeName, m_data.pieceName);
            qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
            return QVector<QLineF>();
        }
    }

    QLineF edge1 = QLineF(m_data.passmarkSAPoint, m_data.previousSAPoint);
    QLineF edge2 = QLineF(m_data.passmarkSAPoint, m_data.nextSAPoint);

    edge1.setAngle(edge1.angle() + edge1.angleTo(edge2)/2.);
    edge1.setLength(length);

    return QVector<QLineF>({edge1});
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPassmark::SAPassmarkBaseLine(const VPiece &piece, const VContainer *data, PassmarkSide side) const
{
    if (m_null)
    {
        return QVector<QLineF>();
    }

    if (not piece.IsSeamAllowanceBuiltIn())
    {
        // Because rollback cannot be calulated if passmark is not first point in main path we rotate it.
        return SAPassmarkBaseLine(piece.SeamAllowancePointsWithRotation(data, m_data.passmarkIndex), side);
    }

    return QVector<QLineF>();
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPassmark::SAPassmarkBaseLine(const QVector<QPointF> &seamAllowance, PassmarkSide side) const
{
    if (m_null)
    {
        return QVector<QLineF>();
    }

    if (seamAllowance.size() < 2)
    {
        const QString errorMsg = QObject::tr("Cannot calculate a notch for point '%1' in piece '%2'. Seam allowance is "
                                             "empty.").arg(m_data.nodeName, m_data.pieceName);
        qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
        return QVector<QLineF>(); // Something wrong
    }

    QPointF seamPassmarkSAPoint;
    const PassmarkStatus seamPassmarkType = GetSeamPassmarkSAPoint(m_data, seamAllowance, seamPassmarkSAPoint);
    if (seamPassmarkType == PassmarkStatus::Error)
    {
        const QString errorMsg = QObject::tr("Cannot calculate a notch for point '%1' in piece '%2'. Cannot find "
                                             "position for a notch.")
                .arg(m_data.nodeName, m_data.pieceName);
        qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
        return QVector<QLineF>(); // Something wrong
    }

    if (not FixNotchPoint(seamAllowance, m_data.passmarkSAPoint, &seamPassmarkSAPoint))
    {
        const QString errorMsg = QObject::tr("Cannot calculate a notch for point '%1' in piece '%2'. Unable to fix a "
                                             "notch position.")
                .arg(m_data.nodeName, m_data.pieceName);
        qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
    }

    auto PassmarkIntersection = [this, seamAllowance] (QLineF line, qreal width)
    {
        line.setLength(line.length()*100); // Hope 100 is enough

        const QVector<QPointF> intersections = VAbstractCurve::CurveIntersectLine(seamAllowance, line);
        if (not intersections.isEmpty())
        {
            if (intersections.last() != m_data.passmarkSAPoint)
            {
                line = QLineF(intersections.last(), m_data.passmarkSAPoint);
                if (not m_data.passmarkSAPoint.IsManualPasskmarkLength())
                {
                    const qreal length = qMin(width * VSAPoint::passmarkFactor, VSAPoint::maxPassmarkLength);
                    if (length <= accuracyPointOnLine)
                    {
                        const QString errorMsg = QObject::tr("Found null notch for point '%1' in piece '%2'. Length is "
                                                             "less than minimal allowed.")
                                .arg(m_data.nodeName, m_data.pieceName);
                        qApp->IsPedantic() ? throw VException(errorMsg) : qWarning() << errorMsg;
                        return QLineF();
                    }
                    line.setLength(length);
                }
                else
                {
                    line.setLength(m_data.passmarkSAPoint.GetPasskmarkLength());
                }
                return line;
            }
            else
            {
                const QString errorMsg = QObject::tr("Cannot calculate a notch for point '%1' in piece '%2'. Notch "
                                                     "collapse.")
                        .arg(m_data.nodeName, m_data.pieceName);
                qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
            }
        }
        else
        {
            const QString errorMsg = QObject::tr("Cannot calculate a notch for point '%1' in piece '%2'. Cannot find "
                                                 "intersection.")
                    .arg(m_data.nodeName, m_data.pieceName);
            qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
        }

        return QLineF();
    };

    if (m_data.passmarkAngleType == PassmarkAngleType::Straightforward)
    {
        const qreal length = m_data.passmarkSAPoint.PassmarkLength(m_data.saWidth);
        if (not m_data.passmarkSAPoint.IsManualPasskmarkLength() && length <= accuracyPointOnLine)
        {
            const QString errorMsg = QObject::tr("Found null notch for point '%1' in piece '%2'. Length is less "
                                                 "than minimal allowed.")
                    .arg(m_data.nodeName, m_data.pieceName);
            qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
        }
        else
        {
            QLineF line = QLineF(seamPassmarkSAPoint, m_data.passmarkSAPoint);
            line.setLength(length);
            return QVector<QLineF>({line});
        }
    }
    else if (m_data.passmarkAngleType == PassmarkAngleType::Bisector)
    {
        return PassmarkBisectorBaseLine(seamPassmarkType, m_data, seamPassmarkSAPoint, seamAllowance);
    }
    else if (m_data.passmarkAngleType == PassmarkAngleType::Intersection
             || m_data.passmarkAngleType == PassmarkAngleType::IntersectionOnlyLeft
             || m_data.passmarkAngleType == PassmarkAngleType::IntersectionOnlyRight)
    {
        QVector<QLineF> lines;
        if ((m_data.passmarkAngleType == PassmarkAngleType::Intersection
                || m_data.passmarkAngleType == PassmarkAngleType::IntersectionOnlyLeft)
                && (side == PassmarkSide::All || side == PassmarkSide::Left))
        {
            // first passmark
            lines += PassmarkIntersection(QLineF(m_data.nextSAPoint, m_data.passmarkSAPoint),
                                          m_data.passmarkSAPoint.GetSABefore(m_data.saWidth));
        }

        if ((m_data.passmarkAngleType == PassmarkAngleType::Intersection
                || m_data.passmarkAngleType == PassmarkAngleType::IntersectionOnlyRight)
                && (side == PassmarkSide::All || side == PassmarkSide::Right))
        {
            // second passmark
            lines += PassmarkIntersection(QLineF(m_data.previousSAPoint, m_data.passmarkSAPoint),
                                          m_data.passmarkSAPoint.GetSAAfter(m_data.saWidth));
        }

        return lines;
    }
    else if (m_data.passmarkAngleType == PassmarkAngleType::Intersection2
             || m_data.passmarkAngleType == PassmarkAngleType::Intersection2OnlyLeft
             || m_data.passmarkAngleType == PassmarkAngleType::Intersection2OnlyRight)
    {
        QVector<QLineF> lines;
        if ((m_data.passmarkAngleType == PassmarkAngleType::Intersection2
                || m_data.passmarkAngleType == PassmarkAngleType::Intersection2OnlyLeft)
                && (side == PassmarkSide::All || side == PassmarkSide::Left))
        {
            // first passmark
            QLineF line(m_data.passmarkSAPoint, m_data.previousSAPoint);
            line.setAngle(line.angle()-90);
            lines += PassmarkIntersection(line, m_data.passmarkSAPoint.GetSABefore(m_data.saWidth));
        }

        if ((m_data.passmarkAngleType == PassmarkAngleType::Intersection2
                || m_data.passmarkAngleType == PassmarkAngleType::Intersection2OnlyRight)
                && (side == PassmarkSide::All || side == PassmarkSide::Right))
        {
            // second passmark
            QLineF line(m_data.passmarkSAPoint, m_data.nextSAPoint);
            line.setAngle(line.angle()+90);
            lines += PassmarkIntersection(line, m_data.passmarkSAPoint.GetSAAfter(m_data.saWidth));
        }

        return lines;
    }

    return QVector<QLineF>();
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPassmark::SAPassmarkPath(const VPiece &piece, const VContainer *data, PassmarkSide side) const
{
    return PassmarkToPath(SAPassmark(piece, data, side));
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPassmark::BuiltInSAPassmarkPath(const VPiece &piece, const VContainer *data) const
{
    return PassmarkToPath(BuiltInSAPassmark(piece, data));
}
