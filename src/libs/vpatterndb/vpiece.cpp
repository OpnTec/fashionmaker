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

#include "vpiece.h"
#include "vpiece_p.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vplacelabelitem.h"
#include "vcontainer.h"
#include "../vmisc/vabstractapplication.h"

#include <QSharedPointer>
#include <QDebug>
#include <QPainterPath>

const qreal passmarkFactor = 0.5;
const qreal maxPassmarkLength = (10/*mm*/ / 25.4) * PrintDPI;

namespace
{
QVector<quint32> PieceMissingNodes(const QVector<quint32> &d1Nodes, const QVector<quint32> &d2Nodes)
{
    if (d1Nodes.size() == d2Nodes.size()) //-V807
    {
        return QVector<quint32>();
    }

    QSet<quint32> set1;
    for (qint32 i = 0; i < d1Nodes.size(); ++i)
    {
        set1.insert(d1Nodes.at(i));
    }

    QSet<quint32> set2;
    for (qint32 j = 0; j < d2Nodes.size(); ++j)
    {
        set2.insert(d2Nodes.at(j));
    }

    const QList<quint32> set3 = set1.subtract(set2).toList();
    QVector<quint32> r;
    for (qint32 i = 0; i < set3.size(); ++i)
    {
        r.append(set3.at(i));
    }

    return r;
}

const qreal passmarkGap = (1.5/*mm*/ / 25.4) * PrintDPI;

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateTwoPassmarkLines(const QLineF &line)
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
    lines.append(QLineF(l1p1, l1p2));
    lines.append(QLineF(l2p1, l2p2));
    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> CreateThreePassmarkLines(const QLineF &line)
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
    lines.append(QLineF(l1p1, l1p2));
    lines.append(line);
    lines.append(QLineF(l2p1, l2p2));
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
QVector<QLineF> CreatePassmarkLines(PassmarkLineType lineType, PassmarkAngleType angleType, const QLineF &line)
{
    QVector<QLineF> passmarksLines;

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
                passmarksLines += CreateTwoPassmarkLines(line);
                break;
            case PassmarkLineType::ThreeLines:
                passmarksLines += CreateThreePassmarkLines(line);
                break;
            case PassmarkLineType::TMark:
                passmarksLines += CreateTMarkPassmark(line);
                break;
            case PassmarkLineType::VMark:
                passmarksLines += CreateVMarkPassmark(line);
                break;
            case PassmarkLineType::OneLine:
            default:
                passmarksLines.append(line);
                break;
        }
    }
    else
    {
        switch (lineType)
        {
            case PassmarkLineType::TMark:
                passmarksLines += CreateTMarkPassmark(line);
                break;
            case PassmarkLineType::OneLine:
            case PassmarkLineType::TwoLines:
            case PassmarkLineType::ThreeLines:
            case PassmarkLineType::VMark:
            default:
                passmarksLines.append(line);
                break;
        }
    }

    return passmarksLines;
}

//---------------------------------------------------------------------------------------------------------------------
bool IsPassmarksPossible(const QVector<VPieceNode> &path)
{
    int countPointNodes = 0;
    int countOthers = 0;

    for (auto &node : path)
    {
        if (node.IsExcluded())
        {
            continue;// skip node
        }

        node.GetTypeTool() == Tool::NodePoint ? ++countPointNodes : ++countOthers;
    }

    return countPointNodes >= 3 || (countPointNodes >= 1 && countOthers >= 1);
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

    return fixed;
}
}

//---------------------------------------------------------------------------------------------------------------------
VPiece::VPiece()
    : VAbstractPiece(), d(new VPieceData(PiecePathType::PiecePath))
{}

//---------------------------------------------------------------------------------------------------------------------
VPiece::VPiece(const VPiece &piece)
    : VAbstractPiece(piece), d (piece.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPiece &VPiece::operator=(const VPiece &piece)
{
    if ( &piece == this )
    {
        return *this;
    }
    VAbstractPiece::operator=(piece);
    d = piece.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPiece::~VPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath VPiece::GetPath() const
{
    return d->m_path;
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath &VPiece::GetPath()
{
    return d->m_path;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetPath(const VPiecePath &path)
{
    d->m_path = path;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VPiece::MainPathPoints(const VContainer *data) const
{
    QVector<QPointF> points = GetPath().PathPoints(data);
    points = CheckLoops(CorrectEquidistantPoints(points));//A path can contains loops
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPointF> VPiece::MainPathNodePoints(const VContainer *data, bool showExcluded) const
{
    return GetPath().PathNodePoints(data, showExcluded);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VPiece::SeamAllowancePoints(const VContainer *data) const
{
    SCASSERT(data != nullptr);


    if (not IsSeamAllowance() || IsSeamAllowanceBuiltIn())
    {
        return QVector<QPointF>();
    }

    const QVector<CustomSARecord> records = FilterRecords(GetValidRecords());
    int recordIndex = -1;
    bool insertingCSA = false;
    const qreal width = ToPixel(GetSAWidth(), *data->GetPatternUnit());
    const QVector<VPieceNode> unitedPath = GetUnitedPath(data);

    QVector<VSAPoint> pointsEkv;
    for (int i = 0; i< unitedPath.size(); ++i)
    {
        const VPieceNode &node = unitedPath.at(i);
        if (node.IsExcluded())
        {
            continue;// skip excluded node
        }

        switch (node.GetTypeTool())
        {
            case (Tool::NodePoint):
            {
                if (not insertingCSA)
                {
                    pointsEkv.append(VPiecePath::PreparePointEkv(node, data));

                    recordIndex = IsCSAStart(records, node.GetId());
                    if (recordIndex != -1 && records.at(recordIndex).includeType == PiecePathIncludeType::AsCustomSA)
                    {
                        insertingCSA = true;

                        const VPiecePath path = data->GetPiecePath(records.at(recordIndex).path);
                        QVector<VSAPoint> r = path.SeamAllowancePoints(data, width, records.at(recordIndex).reverse);

                        for (int j = 0; j < r.size(); ++j)
                        {
                            r[j].SetAngleType(PieceNodeAngle::ByLength);
                            r[j].SetSABefore(0);
                            r[j].SetSAAfter(0);
                        }

                        pointsEkv += r;
                    }
                }
                else
                {
                    if (records.at(recordIndex).endPoint == node.GetId())
                    {
                        insertingCSA = false;
                        recordIndex = -1;

                        pointsEkv.append(VPiecePath::PreparePointEkv(node, data));
                    }
                }
            }
            break;
            case (Tool::NodeArc):
            case (Tool::NodeElArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
            {
                if (not insertingCSA)
                {
                    const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(node.GetId());

                    pointsEkv += VPiecePath::CurveSeamAllowanceSegment(data, unitedPath, curve, i, node.GetReverse(),
                                                                       width);
                }
            }
            break;
            default:
                qDebug()<<"Get wrong tool type. Ignore."<< static_cast<char>(node.GetTypeTool());
                break;
        }
    }

    return Equidistant(pointsEkv, width);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VPiece::CuttingPathPoints(const VContainer *data) const
{
    if (IsSeamAllowance() and not IsSeamAllowanceBuiltIn())
    {
        return SeamAllowancePoints(data);
    }
    else
    {
        return MainPathPoints(data);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPiece::PassmarksLines(const VContainer *data, const QVector<QPointF> &seamAllowance) const
{
    const QVector<VPieceNode> unitedPath = GetUnitedPath(data);
    if (not IsSeamAllowance() || not IsPassmarksPossible(unitedPath))
    {
        return QVector<QLineF>();
    }

    QVector<QLineF> passmarks;

    QVector<QPointF> seamPoints;
    seamAllowance.isEmpty() && not IsSeamAllowanceBuiltIn() ? seamPoints = SeamAllowancePoints(data) :
            seamPoints = seamAllowance;

    for (int i = 0; i< unitedPath.size(); ++i)
    {
        const VPieceNode &node = unitedPath.at(i);
        if (node.IsExcluded() || not node.IsPassmark())
        {
            continue;// skip node
        }

        const int previousIndex = VPiecePath::FindInLoopNotExcludedUp(i, unitedPath);
        const int nextIndex = VPiecePath::FindInLoopNotExcludedDown(i, unitedPath);

        passmarks += CreatePassmark(unitedPath, previousIndex, i, nextIndex, data, seamPoints);
    }

    return passmarks;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<PlaceLabelImg> VPiece::PlaceLabelPoints(const VContainer *data) const
{
    QVector<PlaceLabelImg> points;
    for(auto placeLabel : d->m_placeLabels)
    {
        try
        {
            const auto label = data->GeometricObject<VPlaceLabelItem>(placeLabel);
            if (label->IsVisible())
            {
                points.append(label->LabelShape());
            }
        }
        catch (const VExceptionBadId &e)
        {
            qWarning() << e.ErrorMessage();
        }
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPainterPath> VPiece::CurvesPainterPath(const VContainer *data) const
{
    return GetPath().CurvesPainterPath(data);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPiece::MainPathPath(const VContainer *data) const
{
    return VPiece::MainPathPath(MainPathPoints(data));
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPiece::MainPathPath(const QVector<QPointF> &points)
{
    QPainterPath path;

    if (not points.isEmpty())
    {
        path.moveTo(points[0]);
        for (qint32 i = 1; i < points.count(); ++i)
        {
            path.lineTo(points.at(i));
        }
        path.lineTo(points.at(0));
        path.setFillRule(Qt::WindingFill);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPiece::SeamAllowancePath(const VContainer *data) const
{
    return SeamAllowancePath(SeamAllowancePoints(data));
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPiece::SeamAllowancePath(const QVector<QPointF> &points) const
{
    QPainterPath ekv;

    // seam allowence
    if (IsSeamAllowance() && not IsSeamAllowanceBuiltIn())
    {
        if (not points.isEmpty())
        {
            ekv.moveTo(points.at(0));
            for (qint32 i = 1; i < points.count(); ++i)
            {
                ekv.lineTo(points.at(i));
            }

            ekv.setFillRule(Qt::WindingFill);
        }
    }

    return ekv;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPiece::PassmarksPath(const VContainer *data, const QVector<QPointF> &seamAllowance) const
{
    const QVector<QLineF> passmarks = PassmarksLines(data, seamAllowance);
    QPainterPath path;

    // seam allowence
    if (IsSeamAllowance())
    {
        if (not passmarks.isEmpty())
        {
            for (qint32 i = 0; i < passmarks.count(); ++i)
            {
                path.moveTo(passmarks.at(i).p1());
                path.lineTo(passmarks.at(i).p2());
            }

            path.setFillRule(Qt::WindingFill);
        }
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPiece::PlaceLabelPath(const VContainer *data) const
{
    const QVector<PlaceLabelImg> points = PlaceLabelPoints(data);
    QPainterPath path;

    if (not points.isEmpty())
    {
        for (qint32 i = 0; i < points.count(); ++i)
        {
            path.addPath(PlaceLabelImgPath(points.at(i)));
        }

        path.setFillRule(Qt::WindingFill);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPiece::IsInLayout() const
{
    return d->m_inLayout;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetInLayout(bool inLayout)
{
    d->m_inLayout = inLayout;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPiece::IsUnited() const
{
    return d->m_united;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetUnited(bool united)
{
    d->m_united = united;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPiece::GetFormulaSAWidth() const
{
    return d->m_formulaWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetFormulaSAWidth(const QString &formula, qreal value)
{
    SetSAWidth(value);
    const qreal width = GetSAWidth();
    width >= 0 ? d->m_formulaWidth = formula : d->m_formulaWidth = QLatin1Char('0');
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPiece::GetInternalPaths() const
{
    return d->m_internalPaths;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> &VPiece::GetInternalPaths()
{
    return d->m_internalPaths;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetInternalPaths(const QVector<quint32> &iPaths)
{
    d->m_internalPaths = iPaths;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<CustomSARecord> VPiece::GetCustomSARecords() const
{
    return d->m_customSARecords;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<CustomSARecord> &VPiece::GetCustomSARecords()
{
    return d->m_customSARecords;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetCustomSARecords(const QVector<CustomSARecord> &records)
{
    d->m_customSARecords = records;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPiece::GetPins() const
{
    return d->m_pins;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> &VPiece::GetPins()
{
    return d->m_pins;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetPins(const QVector<quint32> &pins)
{
    d->m_pins = pins;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPiece::GetPlaceLabels() const
{
    return d->m_placeLabels;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> &VPiece::GetPlaceLabels()
{
    return d->m_placeLabels;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetPlaceLabels(const QVector<quint32> &labels)
{
    d->m_placeLabels = labels;
}

//---------------------------------------------------------------------------------------------------------------------
QList<quint32> VPiece::Dependencies() const
{
    QList<quint32> list = d->m_path.Dependencies();
    list.reserve(list.size() + d->m_customSARecords.size() + d->m_internalPaths.size() + d->m_pins.size() +
                 d->m_placeLabels.size());

    for (auto &record : d->m_customSARecords)
    {
        list.append(record.path);
    }

    for (auto &value : d->m_internalPaths)
    {
        list.append(value);
    }

    for (auto &value : d->m_pins)
    {
        list.append(value);
    }

    for (auto &value : d->m_placeLabels)
    {
        list.append(value);
    }

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MissingNodes find missing nodes in detail. When we deleted object in detail and return this detail need
 * understand, what nodes need make invisible.
 * @param det changed detail.
 * @return  list with missing nodes.
 */
QVector<quint32> VPiece::MissingNodes(const VPiece &det) const
{
    return d->m_path.MissingNodes(det.GetPath());
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPiece::MissingCSAPath(const VPiece &det) const
{
    QVector<quint32> oldCSARecords;
    for (qint32 i = 0; i < d->m_customSARecords.size(); ++i)
    {
        oldCSARecords.append(d->m_customSARecords.at(i).path);
    }

    QVector<quint32> newCSARecords;
    for (qint32 i = 0; i < det.GetCustomSARecords().size(); ++i)
    {
        newCSARecords.append(det.GetCustomSARecords().at(i).path);
    }

    return PieceMissingNodes(oldCSARecords, newCSARecords);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPiece::MissingInternalPaths(const VPiece &det) const
{
    return PieceMissingNodes(d->m_internalPaths, det.GetInternalPaths());
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPiece::MissingPins(const VPiece &det) const
{
    return PieceMissingNodes(d->m_pins, det.GetPins());
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPiece::MissingPlaceLabels(const VPiece &det) const
{
    return PieceMissingNodes(d->m_placeLabels, det.GetPlaceLabels());
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetPatternPieceData(const VPieceLabelData &data)
{
    d->m_ppData = data;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns full access to the pattern piece data object
 * @return pattern piece data object
 */
VPieceLabelData &VPiece::GetPatternPieceData()
{
    return d->m_ppData;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the read only reference to the pattern piece data object
 * @return pattern piece data object
 */
const VPieceLabelData &VPiece::GetPatternPieceData() const
{
    return d->m_ppData;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetPatternInfo(const VPatternLabelData &info)
{
    d->m_piPatternInfo = info;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns full access to the pattern info geometry object
 * @return pattern info geometry object
 */
VPatternLabelData &VPiece::GetPatternInfo()
{
    return d->m_piPatternInfo;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the read only reference to the pattern info geometry object
 * @return pattern info geometry object
 */
const VPatternLabelData &VPiece::GetPatternInfo() const
{
    return d->m_piPatternInfo;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetGrainlineGeometry(const VGrainlineData &data)
{
    d->m_glGrainline = data;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDetail::GetGrainlineGeometry full access to the grainline geometry object
 * @return reference to grainline geometry object
 */
VGrainlineData &VPiece::GetGrainlineGeometry()
{
    return d->m_glGrainline;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDetail::GetGrainlineGeometry returns the read-only reference to the grainline geometry object
 * @return reference to grainline geometry object
 */
const VGrainlineData &VPiece::GetGrainlineGeometry() const
{
    return d->m_glGrainline;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPieceNode> VPiece::GetUnitedPath(const VContainer *data) const
{
    SCASSERT(data != nullptr)

    QVector<VPieceNode> united = d->m_path.GetNodes();

    if (IsSeamAllowance() && IsSeamAllowanceBuiltIn())
    {
        return united;
    }

    const QVector<CustomSARecord> records = FilterRecords(GetValidRecords());

    for (int i = 0; i < records.size(); ++i)
    {
        if (records.at(i).includeType == PiecePathIncludeType::AsMainPath)
        {
            const int indexStartPoint = VPiecePath::indexOfNode(united, records.at(i).startPoint);
            const int indexEndPoint = VPiecePath::indexOfNode(united, records.at(i).endPoint);

            if (indexStartPoint == -1 || indexEndPoint == -1)
            {
                continue;
            }

            const QVector<VPieceNode> midBefore = united.mid(0, indexStartPoint+1);
            const QVector<VPieceNode> midAfter = united.mid(indexEndPoint, united.size() - midBefore.size());

            QVector<VPieceNode> customNodes = data->GetPiecePath(records.at(i).path).GetNodes();
            if (records.at(i).reverse)
            {
                customNodes = VGObject::GetReversePoints(customNodes);
            }

            for (int j = 0; j < customNodes.size(); ++j)
            {
                // Additionally reverse all curves
                if (records.at(i).reverse)
                {
                    // don't make a check because node point will ignore the change
                    customNodes[j].SetReverse(not customNodes.at(j).GetReverse());
                }

                // If seam allowance is built in main path user will not see a passmark provided by piece path
                if (IsSeamAllowanceBuiltIn())
                {
                    customNodes[j].SetPassmark(false);
                }
                else
                {
                    customNodes[j].SetMainPathNode(false);
                }
            }

            united = midBefore + customNodes + midAfter;
        }
    }
    return united;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<CustomSARecord> VPiece::GetValidRecords() const
{
    QVector<CustomSARecord> records;
    records.reserve(d->m_customSARecords.size());
    for (auto &record : d->m_customSARecords)
    {
        const int indexStartPoint = d->m_path.indexOfNode(record.startPoint);
        const int indexEndPoint = d->m_path.indexOfNode(record.endPoint);

        if (record.startPoint > NULL_ID
                && record.path > NULL_ID
                && record.endPoint > NULL_ID
                && indexStartPoint != -1
                && not d->m_path.at(indexStartPoint).IsExcluded()
                && indexEndPoint != -1
                && not d->m_path.at(indexEndPoint).IsExcluded())
        {
            records.append(record);
        }
    }
    return records;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<CustomSARecord> VPiece::FilterRecords(QVector<CustomSARecord> records) const
{
    if (records.size() < 2)
    {
        return records;
    }

    // cppcheck-suppress variableScope
    bool foundFilter = false;// Need in case "filter" will stay empty.
    CustomSARecord filter;
    int startIndex = d->m_path.CountNodes()-1;

    for (int i = 0; i < records.size(); ++i)
    {
        const int indexStartPoint = d->m_path.indexOfNode(records.at(i).startPoint);
        if (indexStartPoint < startIndex)
        {
            startIndex = i;
            filter = records.at(i);
            // cppcheck-suppress unreadVariable
            foundFilter = true;
        }
    }

    if (not foundFilter)
    {
        return records; // return as is
    }

    records.remove(startIndex);

    QVector<CustomSARecord> secondRound;
    secondRound.reserve(records.size());
    for (auto &record : records)
    {
        const int indexStartPoint = d->m_path.indexOfNode(record.startPoint);
        const int indexEndPoint = d->m_path.indexOfNode(filter.endPoint);
        if (indexStartPoint > indexEndPoint)
        {
            secondRound.append(record);
        }
    }

    QVector<CustomSARecord> filtered;
    filtered.append(filter);

    filtered += FilterRecords(secondRound);

    return filtered;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> VPiece::GetNodeSAPoints(const QVector<VPieceNode> &path, int index, const VContainer *data) const
{
    SCASSERT(data != nullptr)

    if (index < 0 || index >= path.size())
    {
        return QVector<VSAPoint>();
    }

    const VPieceNode &node = path.at(index);
    QVector<VSAPoint> points;

    if (node.GetTypeTool() == Tool::NodePoint)
    {
        points.append(VPiecePath::PreparePointEkv(node, data));
    }
    else
    {
        const QSharedPointer<VAbstractCurve> curve = data->GeometricObject<VAbstractCurve>(node.GetId());
        const qreal width = ToPixel(GetSAWidth(), *data->GetPatternUnit());

        points += VPiecePath::CurveSeamAllowanceSegment(data, path, curve, index, node.GetReverse(), width);
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPiece::GetPassmarkSAPoint(const QVector<VPieceNode> &path, int index, const VContainer *data,
                                VSAPoint &point) const
{
    SCASSERT(data != nullptr)

    const QVector<VSAPoint> points = GetNodeSAPoints(path, index, data);

    if (points.isEmpty() || points.size() > 1)
    {
        return false;
    }

    point = points.first();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPiece::GetPassmarkPreviousSAPoints(const QVector<VPieceNode> &path, int index, const VSAPoint &passmarkSAPoint,
                                         const VContainer *data, VSAPoint &point) const
{
    SCASSERT(data != nullptr)

    const QVector<VSAPoint> points = GetNodeSAPoints(path, index, data);

    if (points.isEmpty())
    {
        return false; // Something wrong
    }

    bool found = false;
    int nodeIndex = points.size()-1;
    do
    {
        const VSAPoint previous = points.at(nodeIndex);
        if (passmarkSAPoint.toPoint() != previous.toPoint())
        {
            point = previous;
            found = true;
        }
        --nodeIndex;
    } while (nodeIndex >= 0 && not found);

    if (not found)
    {
        return false; // Something wrong
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
int VPiece::GetPassmarkNextSAPoints(const QVector<VPieceNode> &path, int index, const VSAPoint &passmarkSAPoint,
                                    const VContainer *data, VSAPoint &point) const
{
    SCASSERT(data != nullptr)

    const QVector<VSAPoint> points = GetNodeSAPoints(path, index, data);

    if (points.isEmpty())
    {
        return false; // Something wrong
    }

    bool found = false;
    int nodeIndex = 0;
    do
    {
        const VSAPoint next = points.at(nodeIndex);
        if (passmarkSAPoint.toPoint() != next.toPoint())
        {
            point = next;
            found = true;
        }
        ++nodeIndex;

    } while (nodeIndex < points.size() && not found);

    if (not found)
    {
        return false; // Something wrong
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPiece::GetSeamPassmarkSAPoint(const VSAPoint &previousSAPoint, const VSAPoint &passmarkSAPoint,
                                    const VSAPoint &nextSAPoint, const VContainer *data, QPointF &point) const
{
    SCASSERT(data != nullptr)

    QVector<QPointF> ekvPoints;
    const qreal width = ToPixel(GetSAWidth(), *data->GetPatternUnit());

    /* Because method VAbstractPiece::EkvPoint has troubles with edges on a same line we should specially treat such
       cases.
       First check if two edges and seam alowance create paralell lines.
       Second case check if two edges are on a same line geometrically and a passmark point has equal SA width.*/
    if (IsEkvPointOnLine(passmarkSAPoint, previousSAPoint, nextSAPoint)// see issue #665
        || (IsEkvPointOnLine(static_cast<QPointF>(passmarkSAPoint), static_cast<QPointF>(previousSAPoint),
                             static_cast<QPointF>(nextSAPoint))
            && qAbs(passmarkSAPoint.GetSABefore(width) - passmarkSAPoint.GetSAAfter(width)) < accuracyPointOnLine))
    {
        QLineF line (passmarkSAPoint, nextSAPoint);
        line.setAngle(line.angle() + 90);
        line.setLength(passmarkSAPoint.MaxLocalSA(width));
        ekvPoints.append(line.p2());
    }
    else
    {
        ekvPoints = EkvPoint(ekvPoints, previousSAPoint, passmarkSAPoint, nextSAPoint, passmarkSAPoint, width);
    }

    if (ekvPoints.isEmpty())
    { // Just in case
        return false; // Something wrong
    }

    if (ekvPoints.size() == 1 || ekvPoints.size() > 2)
    {
        point = ekvPoints.first();
    }
    else if (ekvPoints.size() == 2)
    {
        QLineF line = QLineF(ekvPoints.at(0), ekvPoints.at(1));
        line.setLength(line.length()/2.);
        point = line.p2();
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPiece::IsPassmarkVisible(const QVector<VPieceNode> &path, int passmarkIndex) const
{
    if (passmarkIndex < 0 || passmarkIndex >= path.size())
    {
        return false;
    }

    const VPieceNode &node = path.at(passmarkIndex);
    if (node.GetTypeTool() != Tool::NodePoint || not node.IsPassmark() || node.IsExcluded())
    {
        return false;
    }

    if (IsSeamAllowance() && IsSeamAllowanceBuiltIn())
    {
        return true;
    }

    const QVector<CustomSARecord> records = FilterRecords(GetValidRecords());
    if (records.isEmpty())
    {
        return true;
    }

    for (auto &record : records)
    {
        if (record.includeType == PiecePathIncludeType::AsCustomSA)
        {
            const int indexStartPoint = VPiecePath::indexOfNode(path, record.startPoint);
            const int indexEndPoint = VPiecePath::indexOfNode(path, record.endPoint);
            if (passmarkIndex > indexStartPoint && passmarkIndex < indexEndPoint)
            {
                return false;
            }
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPiece::CreatePassmark(const QVector<VPieceNode> &path, int previousIndex, int passmarkIndex,
                                       int nextIndex, const VContainer *data,
                                       const QVector<QPointF> &seamAllowance) const
{
    SCASSERT(data != nullptr);

    if (not IsPassmarkVisible(path, passmarkIndex))
    {
        return QVector<QLineF>();
    }

    VSAPoint passmarkSAPoint;
    if (not GetPassmarkSAPoint(path, passmarkIndex, data, passmarkSAPoint))
    {
        return QVector<QLineF>(); // Something wrong
    }

    VSAPoint previousSAPoint;
    if (not GetPassmarkPreviousSAPoints(path, previousIndex, passmarkSAPoint, data,
                                        previousSAPoint))
    {
        return QVector<QLineF>(); // Something wrong
    }

    VSAPoint nextSAPoint;
    if (not GetPassmarkNextSAPoints(path, nextIndex, passmarkSAPoint, data, nextSAPoint))
    {
        return QVector<QLineF>(); // Something wrong
    }

    if (not IsSeamAllowanceBuiltIn())
    {
        QVector<QLineF> lines;
        lines += SAPassmark(path, previousSAPoint, passmarkSAPoint, nextSAPoint, data, passmarkIndex, seamAllowance);
        if (qApp->Settings()->IsDoublePassmark()
                && not IsHideMainPath()
                && path.at(passmarkIndex).IsMainPathNode()
                && path.at(passmarkIndex).GetPassmarkAngleType() != PassmarkAngleType::Intersection
                && path.at(passmarkIndex).GetPassmarkAngleType() != PassmarkAngleType::IntersectionOnlyLeft
                && path.at(passmarkIndex).GetPassmarkAngleType() != PassmarkAngleType::IntersectionOnlyRight
                && path.at(passmarkIndex).GetPassmarkAngleType() != PassmarkAngleType::Intersection2
                && path.at(passmarkIndex).GetPassmarkAngleType() != PassmarkAngleType::Intersection2OnlyLeft
                && path.at(passmarkIndex).GetPassmarkAngleType() != PassmarkAngleType::Intersection2OnlyRight
                && path.at(passmarkIndex).IsShowSecondPassmark())
        {
            lines += BuiltInSAPassmark(path, previousSAPoint, passmarkSAPoint, nextSAPoint, data, passmarkIndex);
        }
        return lines;
    }
    else
    {
        return BuiltInSAPassmark(path, previousSAPoint, passmarkSAPoint, nextSAPoint, data, passmarkIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPiece::SAPassmark(const QVector<VPieceNode> &path, VSAPoint &previousSAPoint,
                                   const VSAPoint &passmarkSAPoint, VSAPoint &nextSAPoint, const VContainer *data,
                                   int passmarkIndex, const QVector<QPointF> &seamAllowance) const
{
    if (seamAllowance.size() < 2)
    {
        return QVector<QLineF>(); // Something wrong
    }

    QPointF seamPassmarkSAPoint;
    if (not GetSeamPassmarkSAPoint(previousSAPoint, passmarkSAPoint, nextSAPoint, data, seamPassmarkSAPoint))
    {
        return QVector<QLineF>(); // Something wrong
    }

    if (not FixNotchPoint(seamAllowance, passmarkSAPoint, &seamPassmarkSAPoint))
    {
        // Show warning
    }

    const qreal width = ToPixel(GetSAWidth(), *data->GetPatternUnit());

    QVector<QLineF> passmarksLines;

    qreal passmarkLength = passmarkSAPoint.MaxLocalSA(width) * passmarkFactor;
    passmarkLength = qMin(passmarkLength, maxPassmarkLength);
    const VPieceNode &node = path.at(passmarkIndex);

    auto PassmarkIntersection = [&passmarksLines, passmarkSAPoint, node](QLineF line,
            const QVector<QPointF> &seamPoints, qreal width)
    {
        line.setLength(line.length()*100); // Hope 100 is enough

        const QVector<QPointF> intersections = VAbstractCurve::CurveIntersectLine(seamPoints, line);
        if (not intersections.isEmpty())
        {
            line = QLineF(intersections.last(), passmarkSAPoint);
            line.setLength(qMin(width * passmarkFactor, maxPassmarkLength));

            passmarksLines += CreatePassmarkLines(node.GetPassmarkLineType(), node.GetPassmarkAngleType(), line);
        }
    };

    if (node.GetPassmarkAngleType() == PassmarkAngleType::Straightforward)
    {
        QLineF line = QLineF(seamPassmarkSAPoint, passmarkSAPoint);
        line.setLength(passmarkLength);
        passmarksLines += CreatePassmarkLines(node.GetPassmarkLineType(), node.GetPassmarkAngleType(), line);
    }
    else if (node.GetPassmarkAngleType() == PassmarkAngleType::Bisector)
    {
        const QLineF bigLine1 = ParallelLine(previousSAPoint, passmarkSAPoint, width );
        const QLineF bigLine2 = ParallelLine(passmarkSAPoint, nextSAPoint, width );

        QLineF edge1 = QLineF(seamPassmarkSAPoint, bigLine1.p1());
        QLineF edge2 = QLineF(seamPassmarkSAPoint, bigLine2.p2());

        edge1.setAngle(edge1.angle() + edge1.angleTo(edge2)/2.);
        edge1.setLength(passmarkLength);

        passmarksLines += CreatePassmarkLines(node.GetPassmarkLineType(), node.GetPassmarkAngleType(), edge1);
    }
    else if (node.GetPassmarkAngleType() == PassmarkAngleType::Intersection
             || node.GetPassmarkAngleType() == PassmarkAngleType::IntersectionOnlyLeft
             || node.GetPassmarkAngleType() == PassmarkAngleType::IntersectionOnlyRight)
    {
        if (node.GetPassmarkAngleType() == PassmarkAngleType::Intersection
                || node.GetPassmarkAngleType() == PassmarkAngleType::IntersectionOnlyRight)
        {
            // first passmark
            PassmarkIntersection(QLineF(previousSAPoint, passmarkSAPoint), seamAllowance,
                                 passmarkSAPoint.GetSAAfter(width));
        }

        if (node.GetPassmarkAngleType() == PassmarkAngleType::Intersection
                || node.GetPassmarkAngleType() == PassmarkAngleType::IntersectionOnlyLeft)
        {
            // second passmark
            PassmarkIntersection(QLineF(nextSAPoint, passmarkSAPoint), seamAllowance,
                                 passmarkSAPoint.GetSABefore(width));
        }
    }
    else if (node.GetPassmarkAngleType() == PassmarkAngleType::Intersection2
             || node.GetPassmarkAngleType() == PassmarkAngleType::Intersection2OnlyLeft
             || node.GetPassmarkAngleType() == PassmarkAngleType::Intersection2OnlyRight)
    {
        if (node.GetPassmarkAngleType() == PassmarkAngleType::Intersection2
                || node.GetPassmarkAngleType() == PassmarkAngleType::Intersection2OnlyRight)
        {
            // first passmark
            QLineF line(passmarkSAPoint, nextSAPoint);
            line.setAngle(line.angle()+90);
            PassmarkIntersection(line, seamAllowance, passmarkSAPoint.GetSAAfter(width));
        }

        if (node.GetPassmarkAngleType() == PassmarkAngleType::Intersection2
                || node.GetPassmarkAngleType() == PassmarkAngleType::Intersection2OnlyLeft)
        {
            // second passmark
            QLineF line(passmarkSAPoint, previousSAPoint);
            line.setAngle(line.angle()-90);
            PassmarkIntersection(line, seamAllowance, passmarkSAPoint.GetSABefore(width));
        }
    }

    return passmarksLines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VPiece::BuiltInSAPassmark(const QVector<VPieceNode> &path, const VSAPoint &previousSAPoint,
                                          const VSAPoint &passmarkSAPoint, const VSAPoint &nextSAPoint,
                                          const VContainer *data, int passmarkIndex) const
{
    QVector<QLineF> passmarksLines;

    const qreal width = ToPixel(GetSAWidth(), *data->GetPatternUnit());
    qreal passmarkLength = passmarkSAPoint.MaxLocalSA(width) * passmarkFactor;
    passmarkLength = qMin(passmarkLength, maxPassmarkLength);

    QLineF edge1 = QLineF(passmarkSAPoint, previousSAPoint);
    QLineF edge2 = QLineF(passmarkSAPoint, nextSAPoint);

    edge1.setAngle(edge1.angle() + edge1.angleTo(edge2)/2.);
    edge1.setLength(passmarkLength);

    const VPieceNode &node = path.at(passmarkIndex);
    passmarksLines += CreatePassmarkLines(node.GetPassmarkLineType(), node.GetPassmarkAngleType(), edge1);

    return passmarksLines;
}

//---------------------------------------------------------------------------------------------------------------------
int VPiece::IsCSAStart(const QVector<CustomSARecord> &records, quint32 id)
{
    for (int i = 0; i < records.size(); ++i)
    {
        if (records.at(i).startPoint == id)
        {
            return i;
        }
    }

    return -1;
}
