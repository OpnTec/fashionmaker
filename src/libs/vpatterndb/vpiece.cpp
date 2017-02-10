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
QVector<VPointF> VPiece::MainPathNodePoints(const VContainer *data) const
{
    return GetPath().PathNodePoints(data);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VPiece::SeamAllowancePoints(const VContainer *data) const
{
    SCASSERT(data != nullptr);


    if (not IsSeamAllowance())
    {
        return QVector<QPointF>();
    }

    const QVector<CustomSARecord> records = GetValidRecords();
    int recordIndex = -1;
    bool insertingCSA = false;
    const qreal width = ToPixel(GetSAWidth(), *data->GetPatternUnit());

    QVector<VSAPoint> pointsEkv;
    for (int i = 0; i< d->m_path.CountNodes(); ++i)
    {
        const VPieceNode &node = d->m_path.at(i);
        switch (node.GetTypeTool())
        {
            case (Tool::NodePoint):
            {
                if (not insertingCSA)
                {
                    pointsEkv.append(VPiecePath::PreparePointEkv(node, data));

                    recordIndex = IsCSAStart(records, node.GetId());
                    if (recordIndex != -1)
                    {
                        insertingCSA = true;

                        const VPiecePath path = data->GetPiecePath(records.at(recordIndex).path);
                        QVector<VSAPoint> r = path.SeamAllowancePoints(data, width, records.at(recordIndex).reverse);

                        if (records.at(recordIndex).includeType == PiecePathIncludeType::AsCustomSA)
                        {
                            for (int j = 0; j < r.size(); ++j)
                            {
                                r[j].SetAngleType(PieceNodeAngle::ByLength);
                                r[j].SetSABefore(0);
                                r[j].SetSAAfter(0);
                            }
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

                    pointsEkv += VPiecePath::CurveSeamAllowanceSegment(data, d->m_path.GetNodes(), curve, i,
                                                                       node.GetReverse(), width);
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
void VPiece::SetInternalPaths(const QVector<quint32> &iPaths)
{
    d->m_internalPaths = iPaths;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::AppendInternalPath(quint32 path)
{
    d->m_internalPaths.append(path);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<CustomSARecord> VPiece::GetCustomSARecords() const
{
    return d->m_customSARecords;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetCustomSARecords(const QVector<CustomSARecord> &records)
{
    d->m_customSARecords = records;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::AppendCustomSARecord(const CustomSARecord &record)
{
    d->m_customSARecords.append(record);
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
    const QVector<CustomSARecord> detRecords = det.GetCustomSARecords();
    if (d->m_customSARecords.size() == detRecords.size()) //-V807
    {
        return QVector<quint32>();
    }

    QSet<quint32> set1;
    for (qint32 i = 0; i < d->m_customSARecords.size(); ++i)
    {
        set1.insert(d->m_customSARecords.at(i).path);
    }

    QSet<quint32> set2;
    for (qint32 j = 0; j < detRecords.size(); ++j)
    {
        set2.insert(detRecords.at(j).path);
    }

    const QList<quint32> set3 = set1.subtract(set2).toList();
    QVector<quint32> r;
    for (qint32 i = 0; i < set3.size(); ++i)
    {
        r.append(set3.at(i));
    }

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VPiece::MissingInternalPaths(const VPiece &det) const
{
    const QVector<quint32> detRecords = det.GetInternalPaths();
    if (d->m_internalPaths.size() == detRecords.size()) //-V807
    {
        return QVector<quint32>();
    }

    QSet<quint32> set1;
    for (qint32 i = 0; i < d->m_internalPaths.size(); ++i)
    {
        set1.insert(d->m_internalPaths.at(i));
    }

    QSet<quint32> set2;
    for (qint32 j = 0; j < detRecords.size(); ++j)
    {
        set2.insert(detRecords.at(j));
    }

    const QList<quint32> set3 = set1.subtract(set2).toList();
    QVector<quint32> r;
    for (qint32 i = 0; i < set3.size(); ++i)
    {
        r.append(set3.at(i));
    }

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetPatternPieceData(const VPatternPieceData &data)
{
    d->m_ppData = data;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns full access to the pattern piece data object
 * @return pattern piece data object
 */
VPatternPieceData &VPiece::GetPatternPieceData()
{
    return d->m_ppData;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the read only reference to the pattern piece data object
 * @return pattern piece data object
 */
const VPatternPieceData &VPiece::GetPatternPieceData() const
{
    return d->m_ppData;
}

//---------------------------------------------------------------------------------------------------------------------
void VPiece::SetPatternInfo(const VPatternInfoGeometry &info)
{
    d->m_piPatternInfo = info;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns full access to the pattern info geometry object
 * @return pattern info geometry object
 */
VPatternInfoGeometry &VPiece::GetPatternInfo()
{
    return d->m_piPatternInfo;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Returns the read only reference to the pattern info geometry object
 * @return pattern info geometry object
 */
const VPatternInfoGeometry &VPiece::GetPatternInfo() const
{
    return d->m_piPatternInfo;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDetail::GetGrainlineGeometry full access to the grainline geometry object
 * @return reference to grainline geometry object
 */
VGrainlineGeometry &VPiece::GetGrainlineGeometry()
{
    return d->m_glGrainline;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDetail::GetGrainlineGeometry returns the read-only reference to the grainline geometry object
 * @return reference to grainline geometry object
 */
const VGrainlineGeometry &VPiece::GetGrainlineGeometry() const
{
    return d->m_glGrainline;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<CustomSARecord> VPiece::GetValidRecords() const
{
    QVector<CustomSARecord> records;
    for (int i = 0; i < d->m_customSARecords.size(); ++i)
    {
        const CustomSARecord &record = d->m_customSARecords.at(i);

        if (record.startPoint > NULL_ID
                && record.path > NULL_ID
                && record.endPoint > NULL_ID
                && d->m_path.indexOfNode(record.startPoint) != -1
                && d->m_path.indexOfNode(record.endPoint) != -1)
        {
            records.append(record);
        }
    }
    return records;
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
