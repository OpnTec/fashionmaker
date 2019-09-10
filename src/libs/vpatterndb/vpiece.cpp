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
#include "vpassmark.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vplacelabelitem.h"
#include "../vgeometry/varc.h"
#include "vcontainer.h"
#include "../vmisc/vabstractapplication.h"
#include "../ifc/exception/vexceptioninvalidnotch.h"
#include "../vlayout/testpath.h"

#include <QSharedPointer>
#include <QDebug>
#include <QPainterPath>
#include <QTemporaryFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

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
    r.reserve(set3.size());
    for (qint32 i = 0; i < set3.size(); ++i)
    {
        r.append(set3.at(i));
    }

    return r;
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
QVector<VPieceNode> RotatePath(const QVector<VPieceNode> &path, int index)
{
    if (index < 0 || index >= path.size())
    {
        return path;
    }

    return path.mid(index) + path.mid(0, index);
}
} // anonymous namespace

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
//    DumpPiece(*this, data);  // Uncomment for dumping test data

    QVector<QPointF> points = GetPath().PathPoints(data);
    points = CheckLoops(CorrectEquidistantPoints(points));//A path can contains loops

//    DumpVector(points); // Uncomment for dumping test data
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VPiece::UniteMainPathPoints(const VContainer *data) const
{
    QVector<QPointF> points = VPiecePath::NodesToPoints(data, GetUnitedPath(data));
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
    return SeamAllowancePointsWithRotation(data, -1);
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
QVector<QLineF> VPiece::PassmarksLines(const VContainer *data) const
{
    QVector<VPassmark> passmarks = Passmarks(data);
    QVector<QLineF> lines;
    for(auto &passmark : passmarks)
    {
        if (not passmark.IsNull())
        {
            lines += passmark.FullPassmark(*this, data);
        }
    }

    return lines;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPassmark> VPiece::Passmarks(const VContainer *data) const
{
    const QVector<VPieceNode> unitedPath = GetUnitedPath(data);
    if (not IsSeamAllowance() || not IsPassmarksPossible(unitedPath))
    {
        return QVector<VPassmark>();
    }

    QVector<VPassmark> passmarks;

    for (int i = 0; i< unitedPath.size(); ++i)
    {
        const VPieceNode &node = unitedPath.at(i);
        if (node.IsExcluded() || not node.IsPassmark())
        {
            continue;// skip node
        }

        const int previousIndex = VPiecePath::FindInLoopNotExcludedUp(i, unitedPath);
        const int nextIndex = VPiecePath::FindInLoopNotExcludedDown(i, unitedPath);

        passmarks += CreatePassmark(unitedPath, previousIndex, i, nextIndex, data);
    }

    return passmarks;
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

#if !defined(V_NO_ASSERT)
            // uncomment for debug
//            for (qint32 i = 0; i < points.count(); ++i)
//            {
//                ekv.addEllipse(points.at(i).x()-accuracyPointOnLine, points.at(i).y()-accuracyPointOnLine,
//                               accuracyPointOnLine*2., accuracyPointOnLine*2.);
//            }
#endif

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
QPainterPath VPiece::PlaceLabelPath(const VContainer *data) const
{
    QPainterPath path;
    for(auto placeLabel : d->m_placeLabels)
    {
        try
        {
            const auto label = data->GeometricObject<VPlaceLabelItem>(placeLabel);
            if (label->IsVisible())
            {
                path.addPath(label->LabelShapePath());
            }
        }
        catch (const VExceptionBadId &e)
        {
            qWarning() << e.ErrorMessage();
        }
    }
    path.setFillRule(Qt::WindingFill);
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPiece::IsSeamAllowanceValid(const VContainer *data) const
{
    if (IsSeamAllowance() && not IsSeamAllowanceBuiltIn())
    {
        return VAbstractPiece::IsAllowanceValid(UniteMainPathPoints(data), SeamAllowancePoints(data));
    }
    else
    {
        return true;
    }
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
QVector<QPointF> VPiece::SeamAllowancePointsWithRotation(const VContainer *data, int makeFirst) const
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
    const QVector<VPieceNode> unitedPath = makeFirst > 0 ? RotatePath(GetUnitedPath(data), makeFirst)
                                                         : GetUnitedPath(data);

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
                            r[j].SetAngleType(PieceNodeAngle::ByLengthCurve);
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

    return Equidistant(pointsEkv, width, GetName());
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

            QVector<VPieceNode> midBefore;
            QVector<VPieceNode> midAfter;
            if (indexStartPoint <= indexEndPoint)
            {
                midBefore = united.mid(0, indexStartPoint+1);
                midAfter = united.mid(indexEndPoint, united.size() - midBefore.size());
            }
            else
            {
                midBefore = united.mid(indexEndPoint, indexStartPoint+1);
            }

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

    QVector<VPieceNode> path = d->m_path.GetNodes();
    QVector<CustomSARecord> filteredRecords;
    for (auto record : qAsConst(records))
    {
        const int indexStartPoint = VPiecePath::indexOfNode(path, record.startPoint);
        const int indexEndPoint = VPiecePath::indexOfNode(path, record.endPoint);

        if (indexStartPoint == -1 || indexEndPoint == -1)
        {
            continue;
        }

        QVector<VPieceNode> midBefore;
        QVector<VPieceNode> midAfter;
        if (indexStartPoint <= indexEndPoint)
        {
            midBefore = path.mid(0, indexStartPoint+1);
            midAfter = path.mid(indexEndPoint, path.size() - midBefore.size());
        }
        else
        {
            midBefore = path.mid(indexEndPoint, indexStartPoint+1);
        }

        path = midBefore + midAfter;
        filteredRecords.append(record);
    }

    return filteredRecords;
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
                                         const VContainer *data, VSAPoint &point, int passmarkIndex) const
{
    SCASSERT(data != nullptr)

    const QVector<VSAPoint> points = GetNodeSAPoints(path, index, data);

    if (points.isEmpty())
    {
        const QString errorMsg = tr("Cannot calculate a notch for point '%1' in piece '%2'.")
                .arg(VPiecePath::NodeName(path, passmarkIndex, data), GetName());
        qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
        return false; // Something wrong
    }

    bool found = false;
    int nodeIndex = points.size()-1;
    do
    {
        const VSAPoint previous = points.at(nodeIndex);
        QLineF line(passmarkSAPoint, previous);
        if (line.length() >= ToPixel(1, Unit::Mm))
        {
            point = previous;
            found = true;
        }
        --nodeIndex;
    } while (nodeIndex >= 0 && not found);

    if (not found)
    {
        // No warning here because of valid case of passmark collapse
        return false; // Something wrong
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPiece::GetPassmarkNextSAPoints(const QVector<VPieceNode> &path, int index, const VSAPoint &passmarkSAPoint,
                                    const VContainer *data, VSAPoint &point, int passmarkIndex) const
{
    SCASSERT(data != nullptr)

    const QVector<VSAPoint> points = GetNodeSAPoints(path, index, data);

    if (points.isEmpty())
    {
        const QString errorMsg = tr("Cannot calculate a notch for point '%1' in piece '%2'.")
                .arg(VPiecePath::NodeName(path, passmarkIndex, data), GetName());
        qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
        return false; // Something wrong
    }

    bool found = false;
    int nodeIndex = 0;
    do
    {
        const VSAPoint next = points.at(nodeIndex);
        QLineF line(passmarkSAPoint, next);
        if (line.length() >= ToPixel(1, Unit::Mm))
        {
            point = next;
            found = true;
        }
        ++nodeIndex;

    } while (nodeIndex < points.size() && not found);

    if (not found)
    {
        // No warning here because of valid case of passmark collapse
        return false; // Something wrong
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
VPassmark VPiece::CreatePassmark(const QVector<VPieceNode> &path, int previousIndex, int passmarkIndex, int nextIndex,
                                 const VContainer *data) const
{
    SCASSERT(data != nullptr);

    if (not IsPassmarkVisible(path, passmarkIndex))
    {
        return VPassmark();
    }

    VSAPoint passmarkSAPoint;
    if (not GetPassmarkSAPoint(path, passmarkIndex, data, passmarkSAPoint))
    {
        const QString errorMsg = tr("Cannot calculate a notch for point '%1' in piece '%2'.")
                .arg(VPiecePath::NodeName(path, passmarkIndex, data), GetName());
        qApp->IsPedantic() ? throw VExceptionInvalidNotch(errorMsg) : qWarning() << errorMsg;
        return VPassmark();
    }

    VSAPoint previousSAPoint;
    if (not GetPassmarkPreviousSAPoints(path, previousIndex, passmarkSAPoint, data, previousSAPoint, passmarkIndex))
    {
        // No check here because it will cover valid cases
        return VPassmark(); // Something wrong
    }

    VSAPoint nextSAPoint;
    if (not GetPassmarkNextSAPoints(path, nextIndex, passmarkSAPoint, data, nextSAPoint, passmarkIndex))
    {
        // No check here because it will cover valid cases
        return VPassmark(); // Something wrong
    }

    if (passmarkSAPoint.IsManualPasskmarkLength() && passmarkSAPoint.GetPasskmarkLength() <= 0)
    {
        return VPassmark();
    }

    VPiecePassmarkData passmarkData;
    passmarkData.previousSAPoint = previousSAPoint;
    passmarkData.passmarkSAPoint = passmarkSAPoint;
    passmarkData.nextSAPoint = nextSAPoint;
    passmarkData.saWidth = ToPixel(GetSAWidth(), *data->GetPatternUnit());
    passmarkData.nodeName = VPiecePath::NodeName(path, passmarkIndex, data);
    passmarkData.pieceName = GetName();
    passmarkData.passmarkLineType = path.at(passmarkIndex).GetPassmarkLineType();
    passmarkData.passmarkAngleType = path.at(passmarkIndex).GetPassmarkAngleType();
    passmarkData.isMainPathNode = path.at(passmarkIndex).IsMainPathNode();
    passmarkData.isShowSecondPassmark = path.at(passmarkIndex).IsShowSecondPassmark();
    passmarkData.passmarkIndex = passmarkIndex;
    passmarkData.id = path.at(passmarkIndex).GetId();

    return VPassmark(passmarkData);
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

//---------------------------------------------------------------------------------------------------------------------
#if !defined(V_NO_ASSERT)
// Use for writing tests
//---------------------------------------------------------------------------------------------------------------------
QJsonObject VPiece::MainPathToJson() const
{
    QJsonObject pieceObject
    {
        {"seamAllowance", IsSeamAllowance()},
        {"saWidth", GetSAWidth()},
    };

    QJsonArray nodesArray;
    for (qint32 i = 0; i < d->m_path.CountNodes(); ++i)
    {
        QJsonObject nodeObject
        {
            {"id", static_cast<qint64>(d->m_path.at(i).GetId())},
            {"type", static_cast<int>(d->m_path.at(i).GetTypeTool())},
            {"reverse", d->m_path.at(i).GetReverse()},
        };

        nodesArray.append(nodeObject);
    }
    pieceObject[QLatin1String("nodes")] = nodesArray;

    return pieceObject;
}

//---------------------------------------------------------------------------------------------------------------------
QJsonObject VPiece::DBToJson(const VContainer *data) const
{
    QJsonArray itemsArray;
    for (qint32 i = 0; i < d->m_path.CountNodes(); ++i)
    {
        itemsArray.append(data->GetGObject(d->m_path.at(i).GetId())->ToJson());
    }

    QJsonObject dbObject
    {
        {"items", itemsArray}
    };

    return dbObject;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::DumpPiece(const VPiece &piece, const VContainer *data)
{
    SCASSERT(data != nullptr)
    QTemporaryFile temp; // Go to tmp folder to find dump
    temp.setAutoRemove(false); // Remove dump manually
    if (temp.open())
    {
        QJsonObject testCase
        {
            {"bd", piece.DBToJson(data)},
            {"piece", piece.MainPathToJson()},
        };

        QJsonObject json
        {
            {"testCase", testCase},
        };

        QJsonDocument document(json);

        QTextStream out(&temp);
        out << document.toJson();
        out.flush();
    }
}
#endif // !defined(V_NO_ASSERT)
