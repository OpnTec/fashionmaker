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

#include "vpiece.h"
#include "vpiece_p.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vabstractcurve.h"
#include "vcontainer.h"

#include <QSharedPointer>
#include <QDebug>
#include <QPainterPath>

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
bool IsPassmarksPossible(const QVector<VPieceNode> &path)
{
    int countPointNodes = 0;
    int countOthers = 0;

    for (int i = 0; i< path.size(); ++i)
    {
        const VPieceNode &node = path.at(i);
        if (node.IsExcluded())
        {
            continue;// skip node
        }

        node.GetTypeTool() == Tool::NodePoint ? ++countPointNodes : ++countOthers;
    }

    return countPointNodes >= 3 || (countPointNodes >= 1 && countOthers >= 1);
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


    if (not IsSeamAllowance())
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
QVector<QLineF> VPiece::PassmarksLines(const VContainer *data) const
{
    const QVector<VPieceNode> unitedPath = GetUnitedPath(data);
    if (not IsSeamAllowance() || not IsPassmarksPossible(unitedPath))
    {
        return QVector<QLineF>();
    }

    QVector<QLineF> passmarks;

    for (int i = 0; i< unitedPath.size(); ++i)
    {
        const VPieceNode &node = unitedPath.at(i);
        if (node.IsExcluded() || not node.IsPassmark())
        {
            continue;// skip node
        }

        int passmarkIndex = i;

        int previousIndex = 0;
        if (passmarkIndex == 0)
        {
            previousIndex = VPiecePath::FindInLoopNotExcludedUp(unitedPath.size()-1, unitedPath);
        }
        else
        {
            previousIndex = VPiecePath::FindInLoopNotExcludedUp(passmarkIndex-1, unitedPath);
        }

        int nextIndex = 0;
        if (passmarkIndex == unitedPath.size()-1)
        {
            nextIndex = VPiecePath::FindInLoopNotExcludedDown(0, unitedPath);
        }
        else
        {
            nextIndex = VPiecePath::FindInLoopNotExcludedDown(passmarkIndex+1, unitedPath);
        }

        passmarks += CreatePassmark(unitedPath, previousIndex, passmarkIndex, nextIndex, data);
    }

    return passmarks;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPiece::MainPathPath(const VContainer *data) const
{
    const QVector<QPointF> points = MainPathPoints(data);
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
    const QVector<QPointF> pointsEkv = SeamAllowancePoints(data);
    QPainterPath ekv;

    // seam allowence
    if (IsSeamAllowance())
    {
        if (not pointsEkv.isEmpty())
        {
            ekv.moveTo(pointsEkv.at(0));
            for (qint32 i = 1; i < pointsEkv.count(); ++i)
            {
                ekv.lineTo(pointsEkv.at(i));
            }

            ekv.setFillRule(Qt::WindingFill);
        }
    }

    return ekv;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPiece::PassmarksPath(const VContainer *data) const
{
    const QVector<QLineF> passmarks = PassmarksLines(data);
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
qreal VPiece::GetMx() const
{
    return d->m_mx;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetMx(qreal value)
{
    d->m_mx = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPiece::GetMy() const
{
    return d->m_my;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetMy(qreal value)
{
    d->m_my = value;
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
    width >= 0 ? d->m_formulaWidth = formula : d->m_formulaWidth = QLatin1String("0");
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
            const QVector<VPieceNode> midAfter = united.mid(indexEndPoint);

            QVector<VPieceNode> customNodes = data->GetPiecePath(records.at(i).path).GetNodes();
            if (records.at(i).reverse)
            {
                customNodes = VGObject::GetReversePoints(customNodes);

                // Additionally reverse all curves
                for (int j = 0; j < customNodes.size(); ++j)
                { // don't make a check because node point will ignore the change
                    customNodes[j].SetReverse(not customNodes.at(j).GetReverse());
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
    for (int i = 0; i < d->m_customSARecords.size(); ++i)
    {
        const CustomSARecord &record = d->m_customSARecords.at(i);
        const int indexStartPoint = d->m_path.indexOfNode(record.startPoint);
        const int indexEndPoint = d->m_path.indexOfNode(record.endPoint);

        if (record.startPoint > NULL_ID
                && record.path > NULL_ID
                && record.endPoint > NULL_ID
                && indexStartPoint != -1
                && not d->m_path.at(indexStartPoint).IsExcluded()
                && indexEndPoint != -1
                && not d->m_path.at(indexEndPoint).IsExcluded()
                && indexStartPoint < indexEndPoint)
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
            foundFilter = true;
        }
    }

    if (not foundFilter)
    {
        return records; // return as is
    }

    records.remove(startIndex);

    QVector<CustomSARecord> secondRound;
    for (int i = 0; i < records.size(); ++i)
    {
        const int indexStartPoint = d->m_path.indexOfNode(records.at(i).startPoint);
        const int indexEndPoint = d->m_path.indexOfNode(filter.endPoint);
        if (indexStartPoint > indexEndPoint)
        {
            secondRound.append(records.at(i));
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
bool VPiece::GetPassmarkPreviousSAPoint(const QVector<VPieceNode> &path, int index, const VSAPoint &passmarkSAPoint,
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
bool VPiece::GetPassmarkNextSAPoint(const QVector<VPieceNode> &path, int index, const VSAPoint &passmarkSAPoint,
                                    const VContainer *data, VSAPoint &point) const
{
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

    const qreal width = ToPixel(GetSAWidth(), *data->GetPatternUnit());
    const QVector<QPointF> ekvPoints = EkvPoint(previousSAPoint, passmarkSAPoint, nextSAPoint, passmarkSAPoint, width);

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

    const QVector<CustomSARecord> records = FilterRecords(GetValidRecords());
    if (records.isEmpty())
    {
        return true;
    }

    for (int i = 0; i < records.size(); ++i)
    {
        if (records.at(i).includeType == PiecePathIncludeType::AsCustomSA)
        {
            const int indexStartPoint = VPiecePath::indexOfNode(path, records.at(i).startPoint);
            const int indexEndPoint = VPiecePath::indexOfNode(path, records.at(i).endPoint);
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
                                       int nextIndex, const VContainer *data) const
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
    if (not GetPassmarkPreviousSAPoint(path, previousIndex, passmarkSAPoint, data, previousSAPoint))
    {
        return QVector<QLineF>(); // Something wrong
    }

    VSAPoint nextSAPoint;
    if (not GetPassmarkNextSAPoint(path, nextIndex, passmarkSAPoint, data, nextSAPoint))
    {
        return QVector<QLineF>(); // Something wrong
    }

    QPointF seamPassmarkSAPoint;
    if (not GetSeamPassmarkSAPoint(previousSAPoint, passmarkSAPoint, nextSAPoint, data, seamPassmarkSAPoint))
    {
        return QVector<QLineF>(); // Something wrong
    }

    const qreal width = ToPixel(GetSAWidth(), *data->GetPatternUnit());
    const QLineF bigLine1 = ParallelLine(previousSAPoint, passmarkSAPoint, width );
    const QLineF bigLine2 = ParallelLine(passmarkSAPoint, nextSAPoint, width );

    QVector<QLineF> passmarksLines;

    const qreal passmarkLength = VAbstractPiece::MaxLocalSA(passmarkSAPoint, width) * 0.5;
    const VPieceNode &node = path.at(passmarkIndex);
    if (node.GetPassmarkAngleType() == PassmarkAngleType::Straightforward)
    {
        QLineF line = QLineF(seamPassmarkSAPoint, passmarkSAPoint);
        line.setLength(passmarkLength);
        if (node.GetPassmarkLineType() == PassmarkLineType::TwoLines)
        {
            passmarksLines += CreateTwoPassmarkLines(line);
        }
        else if (node.GetPassmarkLineType() == PassmarkLineType::ThreeLines)
        {
            passmarksLines += CreateThreePassmarkLines(line);
        }
        else
        {
            passmarksLines.append(line);
        }
    }
    else
    {
        QLineF edge1 = QLineF(seamPassmarkSAPoint, bigLine1.p1());
        QLineF edge2 = QLineF(seamPassmarkSAPoint, bigLine2.p2());

        edge1.setAngle(edge1.angle() + edge1.angleTo(edge2)/2.);
        edge1.setLength(passmarkLength);
        passmarksLines.append(edge1);
    }

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
