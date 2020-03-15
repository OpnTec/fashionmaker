/************************************************************************
 **
 **  @file   vlayoutdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include "vlayoutpiece.h"

#include <QBrush>
#include <QFlags>
#include <QFont>
#include <QFontMetrics>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QGraphicsPathItem>
#include <QList>
#include <QMatrix>
#include <QMessageLogger>
#include <QPainterPath>
#include <QPoint>
#include <QPolygonF>
#include <QTransform>
#include <Qt>
#include <QtDebug>

#include "../vpatterndb/floatItemData/vpatternlabeldata.h"
#include "../vpatterndb/floatItemData/vpiecelabeldata.h"
#include "../vmisc/vmath.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/compatibility.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/vpassmark.h"
#include "../vpatterndb/vpiecenode.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vplacelabelitem.h"
#include "vlayoutdef.h"
#include "vlayoutpiece_p.h"
#include "vtextmanager.h"
#include "vgraphicsfillitem.h"

const quint32 VLayoutPieceData::streamHeader = 0x80D7D009; // CRC-32Q string "VLayoutPieceData"
const quint16 VLayoutPieceData::classVersion = 2;

namespace
{
//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPiecePath> ConvertInternalPaths(const VPiece &piece, const VContainer *pattern)
{
    SCASSERT(pattern != nullptr)

    QVector<VLayoutPiecePath> paths;
    const QVector<quint32> pathsId = piece.GetInternalPaths();
    const QVector<QPointF> cuttingPath = piece.CuttingPathPoints(pattern);
    paths.reserve(pathsId.size());
    for (auto id : pathsId)
    {
        const VPiecePath path = pattern->GetPiecePath(id);
        if (path.GetType() == PiecePathType::InternalPath && path.IsVisible(pattern->DataVariables()))
        {
            VLayoutPiecePath convertedPath = VLayoutPiecePath(path.PathPoints(pattern, cuttingPath));
            convertedPath.SetCutPath(path.IsCutPath());
            convertedPath.SetPenStyle(path.GetPenType());
            paths.append(convertedPath);
        }
    }
    return paths;
}

//---------------------------------------------------------------------------------------------------------------------
bool FindLabelGeometry(const VPatternLabelData &labelData, const VContainer *pattern, qreal &rotationAngle,
                       qreal &labelWidth, qreal &labelHeight, QPointF &pos)
{
    SCASSERT(pattern != nullptr)

    try
    {
        Calculator cal1;
        rotationAngle = cal1.EvalFormula(pattern->DataVariables(), labelData.GetRotation());
    }
    catch(qmu::QmuParserError &e)
    {
        Q_UNUSED(e);
        return false;
    }

    const quint32 topLeftPin = labelData.TopLeftPin();
    const quint32 bottomRightPin = labelData.BottomRightPin();

    if (topLeftPin != NULL_ID && bottomRightPin != NULL_ID)
    {
        try
        {
            const auto topLeftPinPoint = pattern->GeometricObject<VPointF>(topLeftPin);
            const auto bottomRightPinPoint = pattern->GeometricObject<VPointF>(bottomRightPin);

            const QRectF labelRect = QRectF(static_cast<QPointF>(*topLeftPinPoint),
                                            static_cast<QPointF>(*bottomRightPinPoint));
            labelWidth = qAbs(labelRect.width());
            labelHeight = qAbs(labelRect.height());

            pos = labelRect.topLeft();

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
        labelWidth = cal1.EvalFormula(pattern->DataVariables(), labelData.GetLabelWidth());

        Calculator cal2;
        labelHeight = cal2.EvalFormula(pattern->DataVariables(), labelData.GetLabelHeight());
    }
    catch(qmu::QmuParserError &e)
    {
        Q_UNUSED(e);
        return false;
    }

    const quint32 centerPin = labelData.CenterPin();
    if (centerPin != NULL_ID)
    {
        try
        {
            const auto centerPinPoint = pattern->GeometricObject<VPointF>(centerPin);

            const qreal lWidth = ToPixel(labelWidth, *pattern->GetPatternUnit());
            const qreal lHeight = ToPixel(labelHeight, *pattern->GetPatternUnit());

            pos = static_cast<QPointF>(*centerPinPoint) - QRectF(0, 0, lWidth, lHeight).center();
        }
        catch(const VExceptionBadId &)
        {
            pos = labelData.GetPos();
        }
    }
    else
    {
        pos = labelData.GetPos();
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> PrepareAllowance(const QVector<QPointF> &points)
{
    QVector<VSAPoint> allowancePoints;
    allowancePoints.reserve(points.size());
    for(auto point : points)
    {
        allowancePoints.append(VSAPoint(point));
    }
    return allowancePoints;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VLayoutDetail::RotatePoint rotates a point around the center for given angle
 * @param ptCenter center around which the point is rotated
 * @param pt point, which is rotated around the center
 * @param dAng angle of rotation
 * @return position of point pt after rotating it around the center for dAng radians
 */
QPointF RotatePoint(const QPointF &ptCenter, const QPointF& pt, qreal dAng)
{
    QPointF ptDest;
    QPointF ptRel = pt - ptCenter;
    ptDest.setX(cos(dAng)*ptRel.x() - sin(dAng)*ptRel.y());
    ptDest.setY(sin(dAng)*ptRel.x() + cos(dAng)*ptRel.y());

    return ptDest + ptCenter;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList PieceLabelText(const QVector<QPointF> &labelShape, const VTextManager &tm)
{
    QStringList text;
    if (labelShape.count() > 2)
    {
        text.reserve(tm.GetSourceLinesCount());
        for (int i = 0; i < tm.GetSourceLinesCount(); ++i)
        {
            text.append(tm.GetSourceLine(i).m_qsText);
        }
    }
    return text;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPlaceLabel> ConvertPlaceLabels(const VPiece &piece, const VContainer *pattern)
{
    QVector<VLayoutPlaceLabel> labels;
    const QVector<quint32> placeLabels = piece.GetPlaceLabels();
    labels.reserve(placeLabels.size());
    for(auto placeLabel : placeLabels)
    {
        const auto label = pattern->GeometricObject<VPlaceLabelItem>(placeLabel);
        if (label->IsVisible())
        {
            QT_WARNING_PUSH
            QT_WARNING_DISABLE_GCC("-Wnoexcept")
            // noexcept-expression evaluates to 'false' because of a call to 'constexpr QPointF::QPointF()'

            VLayoutPlaceLabel layoutLabel;
            layoutLabel.shape = label->LabelShape();
            layoutLabel.rotationMatrix = label->RotationMatrix();
            layoutLabel.box = label->Box();
            layoutLabel.center = label->toQPointF();
            layoutLabel.type = label->GetLabelType();
            labels.append(layoutLabel);

            QT_WARNING_POP
        }
    }
    return labels;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPassmark> ConvertPassmarks(const VPiece &piece, const VContainer *pattern)
{
    const QVector<VPassmark> passmarks = piece.Passmarks(pattern);
    QVector<VLayoutPassmark> layoutPassmarks;
    for(auto &passmark : passmarks)
    {
        if (not passmark.IsNull())
        {
            VPiecePassmarkData pData = passmark.Data();

            auto PreapreBuiltInSAPassmark = [pData, passmark, piece, &layoutPassmarks, pattern]()
            {
                VLayoutPassmark layoutPassmark;

                const QVector<VPieceNode> path = piece.GetUnitedPath(pattern);
                const int nodeIndex = VPiecePath::indexOfNode(path, pData.id);
                if (nodeIndex != -1)
                {
                    layoutPassmark.lines = passmark.BuiltInSAPassmark(piece, pattern);
                    layoutPassmark.baseLine = ConstFirst (passmark.BuiltInSAPassmarkBaseLine(piece));
                    layoutPassmark.type = pData.passmarkLineType;
                    layoutPassmark.isBuiltIn = true;

                    layoutPassmarks.append(layoutPassmark);
                }
                else
                {
                    const QString errorMsg =
                            QObject::tr("Passmark '%1' is not part of piece '%2'.")
                            .arg(pData.nodeName, piece.GetName());
                    qApp->IsPedantic() ? throw VException(errorMsg) :
                                         qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
                }
            };

            auto PrepareSAPassmark = [pData, passmark, piece, &layoutPassmarks, pattern](PassmarkSide side)
            {
                QT_WARNING_PUSH
                QT_WARNING_DISABLE_GCC("-Wnoexcept")
                // noexcept-expression evaluates to 'false' because of a call to 'constexpr QPointF::QPointF()'

                VLayoutPassmark layoutPassmark;

                QT_WARNING_POP

                const QVector<VPieceNode> path = piece.GetUnitedPath(pattern);
                const int nodeIndex = VPiecePath::indexOfNode(path, pData.id);
                if (nodeIndex != -1)
                {
                    QVector<QLineF> lines =
                            passmark.SAPassmarkBaseLine(piece, pattern, static_cast<PassmarkSide>(side));

                    if (side == PassmarkSide::All || side == PassmarkSide::Right)
                    {
                        layoutPassmark.baseLine = lines.first();
                    }
                    else if (side == PassmarkSide::Right)
                    {
                        layoutPassmark.baseLine = lines.last();
                    }
                    layoutPassmark.lines = passmark.SAPassmark(piece, pattern, side);
                    layoutPassmark.type = pData.passmarkLineType;
                    layoutPassmark.isBuiltIn = false;

                    layoutPassmarks.append(layoutPassmark);
                }
                else
                {
                    const QString errorMsg =
                            QObject::tr("Passmark '%1' is not part of piece '%2'.")
                            .arg(pData.nodeName, piece.GetName());
                    qApp->IsPedantic() ? throw VException(errorMsg) :
                                         qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
                }
            };

            if (not piece.IsSeamAllowanceBuiltIn())
            {
                if (pData.passmarkAngleType == PassmarkAngleType::Straightforward
                        || pData.passmarkAngleType == PassmarkAngleType::Bisector)
                {
                    PrepareSAPassmark(PassmarkSide::All);
                }
                else if (pData.passmarkAngleType == PassmarkAngleType::Intersection
                         || pData.passmarkAngleType == PassmarkAngleType::IntersectionOnlyLeft
                         || pData.passmarkAngleType == PassmarkAngleType::IntersectionOnlyRight
                         || pData.passmarkAngleType == PassmarkAngleType::Intersection2
                         || pData.passmarkAngleType == PassmarkAngleType::Intersection2OnlyLeft
                         || pData.passmarkAngleType == PassmarkAngleType::Intersection2OnlyRight)
                {
                    if (pData.passmarkAngleType == PassmarkAngleType::Intersection ||
                            pData.passmarkAngleType == PassmarkAngleType::Intersection2)
                    {
                        PrepareSAPassmark(PassmarkSide::Left);
                        PrepareSAPassmark(PassmarkSide::Right);
                    }
                    else if (pData.passmarkAngleType == PassmarkAngleType::IntersectionOnlyLeft ||
                             pData.passmarkAngleType == PassmarkAngleType::Intersection2OnlyLeft)
                    {
                        PrepareSAPassmark(PassmarkSide::Left);
                    }
                    else if (pData.passmarkAngleType == PassmarkAngleType::IntersectionOnlyRight ||
                             pData.passmarkAngleType == PassmarkAngleType::Intersection2OnlyRight)
                    {
                        PrepareSAPassmark(PassmarkSide::Right);
                    }
                }

                if (qApp->Settings()->IsDoublePassmark()
                        && (qApp->Settings()->IsPieceShowMainPath() || not piece.IsHideMainPath())
                        && pData.isMainPathNode
                        && pData.passmarkAngleType != PassmarkAngleType::Intersection
                        && pData.passmarkAngleType != PassmarkAngleType::IntersectionOnlyLeft
                        && pData.passmarkAngleType != PassmarkAngleType::IntersectionOnlyRight
                        && pData.passmarkAngleType != PassmarkAngleType::Intersection2
                        && pData.passmarkAngleType != PassmarkAngleType::Intersection2OnlyLeft
                        && pData.passmarkAngleType != PassmarkAngleType::Intersection2OnlyRight
                        && pData.isShowSecondPassmark)
                {
                    PreapreBuiltInSAPassmark();
                }
            }
            else
            {
                PreapreBuiltInSAPassmark();
            }
        }
    }

    return layoutPassmarks;
}
}

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &dataStream, const VLayoutPiece &piece)
{
    dataStream << static_cast<VAbstractPiece>(piece);
    dataStream << *piece.d;
    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &dataStream, VLayoutPiece &piece)
{
    dataStream >> static_cast<VAbstractPiece &>(piece);
    dataStream >> *piece.d;
    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece::VLayoutPiece()
    :VAbstractPiece(), d(new VLayoutPieceData)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece::VLayoutPiece(const VLayoutPiece &detail)
    :VAbstractPiece(detail), d(detail.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece &VLayoutPiece::operator=(const VLayoutPiece &detail)
{
    if ( &detail == this )
    {
        return *this;
    }
    VAbstractPiece::operator=(detail);
    d = detail.d;
    return *this;
}

#ifdef Q_COMPILER_RVALUE_REFS
//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece::VLayoutPiece(const VLayoutPiece &&detail) Q_DECL_NOTHROW
    :VAbstractPiece(detail), d(detail.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece &VLayoutPiece::operator=(VLayoutPiece &&detail) Q_DECL_NOTHROW
{
    VAbstractPiece::operator=(detail);
    std::swap(d, detail.d);
    return *this;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece::~VLayoutPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece VLayoutPiece::Create(const VPiece &piece, vidtype id, const VContainer *pattern)
{
    QFuture<QVector<QPointF> > futureSeamAllowance = QtConcurrent::run(piece, &VPiece::SeamAllowancePoints, pattern);
    QFuture<bool> futureSeamAllowanceValid = QtConcurrent::run(piece, &VPiece::IsSeamAllowanceValid, pattern);
    QFuture<QVector<QPointF> > futureMainPath = QtConcurrent::run(piece, &VPiece::MainPathPoints, pattern);
    QFuture<QVector<VLayoutPiecePath> > futureInternalPaths = QtConcurrent::run(ConvertInternalPaths, piece, pattern);
    QFuture<QVector<VLayoutPassmark> > futurePassmarks = QtConcurrent::run(ConvertPassmarks, piece, pattern);
    QFuture<QVector<VLayoutPlaceLabel> > futurePlaceLabels = QtConcurrent::run(ConvertPlaceLabels, piece, pattern);

    VLayoutPiece det;

    det.SetMx(piece.GetMx());
    det.SetMy(piece.GetMy());

    det.SetName(piece.GetName());

    det.SetSAWidth(qApp->toPixel(piece.GetSAWidth()));
    det.SetForbidFlipping(piece.IsForbidFlipping());
    det.SetForceFlipping(piece.IsForceFlipping());
    det.SetId(id);

    if (not futureSeamAllowanceValid.result())
    {
        const QString errorMsg = QObject::tr("Piece '%1'. Seam allowance is not valid.")
                .arg(piece.GetName());
        qApp->IsPedantic() ? throw VException(errorMsg) :
                             qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
    }

    det.SetCountourPoints(futureMainPath.result(),
                          qApp->Settings()->IsPieceShowMainPath() ? false : piece.IsHideMainPath());
    det.SetSeamAllowancePoints(futureSeamAllowance.result(), piece.IsSeamAllowance(), piece.IsSeamAllowanceBuiltIn());
    det.SetInternalPaths(futureInternalPaths.result());
    det.SetPassmarks(futurePassmarks.result());
    det.SetPlaceLabels(futurePlaceLabels.result());
    det.SetPriority(piece.GetPriority());

    // Very important to set main path first!
    if (det.ContourPath().isEmpty())
    {
        throw VException (tr("Piece %1 doesn't have shape.").arg(piece.GetName()));
    }

    const VPieceLabelData& data = piece.GetPatternPieceData();
    det.SetQuantity(data.GetQuantity());
    if (data.IsVisible())
    {
        det.SetPieceText(piece.GetName(), data, qApp->Settings()->GetLabelFont(), pattern);
    }

    const VPatternLabelData& geom = piece.GetPatternInfo();
    if (geom.IsVisible())
    {
        VAbstractPattern* pDoc = qApp->getCurrentDocument();
        det.SetPatternInfo(pDoc, geom, qApp->Settings()->GetLabelFont(), pattern);
    }

    const VGrainlineData& grainlineGeom = piece.GetGrainlineGeometry();
    if (grainlineGeom.IsVisible() == true)
    {
        det.SetGrainline(grainlineGeom, pattern);
    }

    return det;
}

//---------------------------------------------------------------------------------------------------------------------
template <class T>
QVector<T> VLayoutPiece::Map(QVector<T> points) const
{
    for (int i = 0; i < points.size(); ++i)
    {
        points[i] = d->matrix.map(points.at(i));
    }

    if (d->mirror)
    {
        QList<T> list = ConvertToList(points);

        for (int k=0, s=list.size(), max=(s/2); k<max; k++)
        {
            SwapItemsAt(list, k, s-(1+k));
        }
        points = ConvertToVector(list);
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
template <>
QVector<VLayoutPlaceLabel> VLayoutPiece::Map<VLayoutPlaceLabel>(QVector<VLayoutPlaceLabel> points) const
{
    for (int i = 0; i < points.size(); ++i)
    {
        points[i].shape = Map(points.at(i).shape);
    }

    return points;
}

//---------------------------------------------------------------------------------------------------------------------
template <>
QVector<VLayoutPassmark> VLayoutPiece::Map<VLayoutPassmark>(QVector<VLayoutPassmark> passmarks) const
{
    for (int i = 0; i < passmarks.size(); ++i)
    {
        passmarks[i].lines = Map(passmarks.at(i).lines);
        passmarks[i].baseLine = d->matrix.map(passmarks.at(i).baseLine);
    }

    return passmarks;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QVector<QPointF> VLayoutPiece::GetMappedContourPoints() const
{
    return Map(d->contour);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPiece::GetContourPoints() const
{
    return d->contour;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetCountourPoints(const QVector<QPointF> &points, bool hideMainPath)
{
    d->contour = RemoveDublicates(points, false);
    SetHideMainPath(hideMainPath);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QVector<QPointF> VLayoutPiece::GetMappedSeamAllowancePoints() const
{
    return Map(d->seamAllowance);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPiece::GetSeamAllowancePoints() const
{
    return d->seamAllowance;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetSeamAllowancePoints(const QVector<QPointF> &points, bool seamAllowance, bool seamAllowanceBuiltIn)
{
    if (seamAllowance)
    {
        SetSeamAllowance(seamAllowance);
        SetSeamAllowanceBuiltIn(seamAllowanceBuiltIn);
        d->seamAllowance = points;
        if (not d->seamAllowance.isEmpty())
        {
            d->seamAllowance = RemoveDublicates(d->seamAllowance, false);
        }
        else if (not IsSeamAllowanceBuiltIn())
        {
            qWarning()<<"Seam allowance is empty.";
            SetSeamAllowance(false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPiece::GetLayoutAllowancePoints() const
{
    return Map(d->layoutAllowance);
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VLayoutPiece::GetPieceTextPosition() const
{
    if (d->detailLabel.count() > 2)
    {
        return d->matrix.map(d->detailLabel.first());
    }
    else
    {
        return QPointF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VLayoutPiece::GetPieceText() const
{
    return PieceLabelText(d->detailLabel, d->m_tmDetail);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetPieceText(const QString& qsName, const VPieceLabelData& data, const QFont &font,
                                const VContainer *pattern)
{
    QPointF ptPos;
    qreal labelWidth = 0;
    qreal labelHeight = 0;
    qreal labelAngle = 0;
    if (not FindLabelGeometry(data, pattern, labelAngle, labelWidth, labelHeight, ptPos))
    {
        return;
    }

    labelWidth = ToPixel(labelWidth, *pattern->GetPatternUnit());
    labelHeight = ToPixel(labelHeight, *pattern->GetPatternUnit());

    QVector<QPointF> v;
    v << ptPos
      << QPointF(ptPos.x() + labelWidth, ptPos.y())
      << QPointF(ptPos.x() + labelWidth, ptPos.y() + labelHeight)
      << QPointF(ptPos.x(), ptPos.y() + labelHeight);

    const qreal dAng = qDegreesToRadians(-labelAngle);
    const QPointF ptCenter(ptPos.x() + labelWidth/2, ptPos.y() + labelHeight/2);

    for (int i = 0; i < v.count(); ++i)
    {
        v[i] = RotatePoint(ptCenter, v.at(i), dAng);
    }

    QScopedPointer<QGraphicsItem> item(GetMainPathItem());
    d->detailLabel = CorrectPosition(item->boundingRect(), v);

    // generate text
    d->m_tmDetail.SetFont(font);
    d->m_tmDetail.SetFontSize(data.GetFontSize());
    d->m_tmDetail.Update(qsName, data);
    // this will generate the lines of text
    d->m_tmDetail.SetFontSize(data.GetFontSize());
    d->m_tmDetail.FitFontSize(labelWidth, labelHeight);
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VLayoutPiece::GetPatternTextPosition() const
{
    if (d->patternInfo.count() > 2)
    {
        return d->matrix.map(d->patternInfo.first());
    }
    else
    {
        return QPointF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VLayoutPiece::GetPatternText() const
{
    return PieceLabelText(d->patternInfo, d->m_tmPattern);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetPatternInfo(VAbstractPattern* pDoc, const VPatternLabelData& geom, const QFont &font,
                                  const VContainer *pattern)
{
    QPointF ptPos;
    qreal labelWidth = 0;
    qreal labelHeight = 0;
    qreal labelAngle = 0;
    if (not FindLabelGeometry(geom, pattern, labelAngle, labelWidth, labelHeight, ptPos))
    {
        return;
    }

    labelWidth = ToPixel(labelWidth, *pattern->GetPatternUnit());
    labelHeight = ToPixel(labelHeight, *pattern->GetPatternUnit());

    QVector<QPointF> v;
    v << ptPos
      << QPointF(ptPos.x() + labelWidth, ptPos.y())
      << QPointF(ptPos.x() + labelWidth, ptPos.y() + labelHeight)
      << QPointF(ptPos.x(), ptPos.y() + labelHeight);

    const qreal dAng = qDegreesToRadians(-labelAngle);
    const QPointF ptCenter(ptPos.x() + labelWidth/2, ptPos.y() + labelHeight/2);
    for (int i = 0; i < v.count(); ++i)
    {
        v[i] = RotatePoint(ptCenter, v.at(i), dAng);
    }
    QScopedPointer<QGraphicsItem> item(GetMainPathItem());
    d->patternInfo = CorrectPosition(item->boundingRect(), v);

    // Generate text
    d->m_tmPattern.SetFont(font);
    d->m_tmPattern.SetFontSize(geom.GetFontSize());

    d->m_tmPattern.Update(pDoc);

    // generate lines of text
    d->m_tmPattern.SetFontSize(geom.GetFontSize());
    d->m_tmPattern.FitFontSize(labelWidth, labelHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetGrainline(const VGrainlineData& geom, const VContainer* pattern)
{
    qreal dAng = 0;

    QScopedPointer<QGraphicsItem> item(GetMainPathItem());
    const QVector<QPointF> v = GrainlinePoints(geom, pattern, item->boundingRect(), dAng);

    if (v.isEmpty())
    {
        return;
    }

    d->grainlineEnabled = true;
    d->grainlineArrowType = geom.GetArrowType();
    d->grainlineAngle = qRadiansToDegrees(dAng);
    d->grainlinePoints = v;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPiece::GetGrainline() const
{
    return Map(d->grainlinePoints);
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPiece::IsGrainlineEnabled() const
{
    return d->grainlineEnabled;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutPiece::GrainlineAngle() const
{
    return d->grainlineAngle;
}

//---------------------------------------------------------------------------------------------------------------------
GrainlineArrowDirection VLayoutPiece::GrainlineArrowType() const
{
    return d->grainlineArrowType;
}

//---------------------------------------------------------------------------------------------------------------------
QTransform VLayoutPiece::GetMatrix() const
{
    return d->matrix;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetMatrix(const QTransform &matrix)
{
    d->matrix = matrix;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutPiece::GetLayoutWidth() const
{
    return d->layoutWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetLayoutWidth(qreal value)
{
    d->layoutWidth = value;
}

//---------------------------------------------------------------------------------------------------------------------
quint16 VLayoutPiece::GetQuantity() const
{
    return d->m_quantity;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetQuantity(quint16 value)
{
    d->m_quantity = qMax(static_cast<quint16>(1), value);
}

//---------------------------------------------------------------------------------------------------------------------
vidtype VLayoutPiece::GetId() const
{
    return d->m_id;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetId(vidtype id)
{
    d->m_id = id;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::Translate(qreal dx, qreal dy)
{
    QTransform m;
    m.translate(dx, dy);
    d->matrix *= m;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::Rotate(const QPointF &originPoint, qreal degrees)
{
    QTransform m;
    m.translate(originPoint.x(), originPoint.y());
    m.rotate(-degrees);
    m.translate(-originPoint.x(), -originPoint.y());
    d->matrix *= m;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::Mirror(const QLineF &edge)
{
    if (edge.isNull())
    {
        return;
    }

    const QLineF axis = QLineF(edge.x2(), edge.y2(), edge.x2() + 100, edge.y2()); // Ox axis

    const qreal angle = edge.angleTo(axis);
    const QPointF p2 = edge.p2();
    QTransform m;
    m.translate(p2.x(), p2.y());
    m.rotate(-angle);
    m.translate(-p2.x(), -p2.y());
    d->matrix *= m;

    m.reset();
    m.translate(p2.x(), p2.y());
    m.scale(m.m11(), m.m22()*-1);
    m.translate(-p2.x(), -p2.y());
    d->matrix *= m;

    m.reset();
    m.translate(p2.x(), p2.y());
    m.rotate(-(360-angle));
    m.translate(-p2.x(), -p2.y());
    d->matrix *= m;

    d->mirror = !d->mirror;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::Mirror()
{
    QTransform m;
    m.scale(-1, 1);
    d->matrix *= m;
    d->mirror = !d->mirror;
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPiece::DetailEdgesCount() const
{
    return DetailPath().count();
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPiece::LayoutEdgesCount() const
{
    const int count = d->layoutAllowance.count();
    return count > 2 ? count : 0;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VLayoutPiece::LayoutEdge(int i) const
{
    return Edge(d->layoutAllowance, i);
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPiece::LayoutEdgeByPoint(const QPointF &p1) const
{
    return EdgeByPoint(d->layoutAllowance, p1);
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VLayoutPiece::DetailBoundingRect() const
{
    return IsSeamAllowance() && not IsSeamAllowanceBuiltIn() ? BoundingRect(GetMappedSeamAllowancePoints()) :
                                                               BoundingRect(GetMappedContourPoints());
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VLayoutPiece::LayoutBoundingRect() const
{
    return BoundingRect(GetLayoutAllowancePoints());
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutPiece::Diagonal() const
{
    const QRectF rec = LayoutBoundingRect();
    return qSqrt(pow(rec.height(), 2) + pow(rec.width(), 2));
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPiece::isNull() const
{
    if (d->contour.isEmpty() == false && d->layoutWidth > 0)
    {
        if (IsSeamAllowance() && not IsSeamAllowanceBuiltIn() && d->seamAllowance.isEmpty() == false)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qint64 VLayoutPiece::Square() const
{
    return d->m_square;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetLayoutAllowancePoints()
{
    d->m_square = 0;

    if (d->layoutWidth > 0)
    {
        if (IsSeamAllowance() && not IsSeamAllowanceBuiltIn())
        {
            d->layoutAllowance = Equidistant(PrepareAllowance(GetMappedSeamAllowancePoints()), d->layoutWidth,
                                             GetName());
            if (not d->layoutAllowance.isEmpty())
            {
                d->layoutAllowance.removeLast();

                d->m_square = qFloor(qAbs(SumTrapezoids(GetSeamAllowancePoints())/2.0));
            }
        }
        else
        {
            d->layoutAllowance = Equidistant(PrepareAllowance(GetMappedContourPoints()), d->layoutWidth, GetName());
            if (not d->layoutAllowance.isEmpty())
            {
                d->layoutAllowance.removeLast();

                d->m_square = qFloor(qAbs(SumTrapezoids(GetContourPoints())/2.0));
            }
        }
    }
    else
    {
        d->layoutAllowance.clear();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPassmark> VLayoutPiece::GetPassmarks() const
{
    return Map(d->passmarks);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetPassmarks(const QVector<VLayoutPassmark> &passmarks)
{
    if (IsSeamAllowance())
    {
        d->passmarks = passmarks;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPlaceLabel> VLayoutPiece::GetPlaceLabels() const
{
    return Map(d->m_placeLabels);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetPlaceLabels(const QVector<VLayoutPlaceLabel> &labels)
{
    d->m_placeLabels = labels;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QVector<QPointF> > VLayoutPiece::InternalPathsForCut(bool cut) const
{
    QVector<QVector<QPointF> > paths;

    for (auto &path : d->m_internalPaths)
    {
        if (path.IsCutPath() == cut)
        {
            paths.append(Map(path.Points()));
        }
    }

    return paths;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPiecePath> VLayoutPiece::GetInternalPaths() const
{
    return d->m_internalPaths;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetInternalPaths(const QVector<VLayoutPiecePath> &internalPaths)
{
    d->m_internalPaths = internalPaths;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPiece::ContourPath() const
{
    QPainterPath path;

    // contour
    if (not IsHideMainPath() || not IsSeamAllowance() || IsSeamAllowanceBuiltIn())
    {
        path = PainterPath(GetMappedContourPoints());
    }

    // seam allowance
    if (IsSeamAllowance())
    {
        if (not IsSeamAllowanceBuiltIn())
        {
            // Draw seam allowance
            QVector<QPointF>points = GetMappedSeamAllowancePoints();

            if (points.last().toPoint() != points.first().toPoint())
            {
                points.append(points.at(0));// Should be always closed
            }

            QPainterPath ekv;
            ekv.moveTo(points.at(0));
            for (qint32 i = 1; i < points.count(); ++i)
            {
                ekv.lineTo(points.at(i));
            }

            path.addPath(ekv);
        }

        // Draw passmarks
        QPainterPath passmaksPath;
        const QVector<VLayoutPassmark> passmarks = GetPassmarks();
        for(auto &passmark : passmarks)
        {
            for (auto &line : passmark.lines)
            {
                passmaksPath.moveTo(line.p1());
                passmaksPath.lineTo(line.p2());
            }
        }

        path.addPath(passmaksPath);
        path.setFillRule(Qt::WindingFill);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPiece::LayoutAllowancePath() const
{
    return PainterPath(GetLayoutAllowancePoints());
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPiece::PainterPath(const QVector<QPointF> &points)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    path.moveTo(points.at(0));
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points.at(i));
    }
    path.lineTo(points.at(0));

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsItem *VLayoutPiece::GetItem(bool textAsPaths) const
{
    QGraphicsPathItem *item = GetMainItem();

    for (auto &path : d->m_internalPaths)
    {
        QGraphicsPathItem* pathItem = new QGraphicsPathItem(item);
        pathItem->setPath(d->matrix.map(path.GetPainterPath()));

        QPen pen = pathItem->pen();
        pen.setStyle(path.PenStyle());
        pathItem->setPen(pen);
    }

    for (auto &label : d->m_placeLabels)
    {
        QGraphicsPathItem* pathItem = new QGraphicsPathItem(item);
        pathItem->setPath(d->matrix.map(VPlaceLabelItem::LabelShapePath(label.shape)));
    }

    CreateLabelStrings(item, d->detailLabel, d->m_tmDetail, textAsPaths);
    CreateLabelStrings(item, d->patternInfo, d->m_tmPattern, textAsPaths);
    CreateGrainlineItem(item);

    return item;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPiece::IsLayoutAllowanceValid() const
{
    QVector<QPointF> base = (IsSeamAllowance() && not IsSeamAllowanceBuiltIn()) ? d->seamAllowance : d->contour;
    return VAbstractPiece::IsAllowanceValid(base, d->layoutAllowance);
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutPiece::BiggestEdge() const
{
    qreal edge = 0;

    if (LayoutEdgesCount() < 1)
    {
        return edge;
    }

    for (int i = 1; i < LayoutEdgesCount(); ++i)
    {
        const qreal length = LayoutEdge(i).length();
        if (length > edge)
        {
            edge = length;
        }
    }

    return edge;
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VLayoutPiece::BoundingRect(QVector<QPointF> points)
{
    points.append(points.first());
    return QPolygonF(points).boundingRect();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::CreateLabelStrings(QGraphicsItem *parent, const QVector<QPointF> &labelShape,
                                      const VTextManager &tm, bool textAsPaths) const
{
    SCASSERT(parent != nullptr)

    if (labelShape.count() > 2)
    {
        const qreal dW = QLineF(labelShape.at(0), labelShape.at(1)).length();
        const qreal dH = QLineF(labelShape.at(1), labelShape.at(2)).length();
        const qreal angle = - QLineF(labelShape.at(0), labelShape.at(1)).angle();
        qreal dY = 0;
        qreal dX = 0;

        for (int i = 0; i < tm.GetSourceLinesCount(); ++i)
        {
            const TextLine& tl = tm.GetSourceLine(i);
            QFont fnt = tm.GetFont();
            fnt.setPixelSize(tm.GetFont().pixelSize() + tl.m_iFontSize);
            fnt.setBold(tl.m_bold);
            fnt.setItalic(tl.m_italic);

            QFontMetrics fm(fnt);

            if (textAsPaths)
            {
                dY += fm.height();
            }

            if (dY > dH)
            {
                break;
            }

            QString qsText = tl.m_qsText;
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
            if (fm.horizontalAdvance(qsText) > dW)
#else
            if (fm.width(qsText) > dW)
#endif
            {
                qsText = fm.elidedText(qsText, Qt::ElideMiddle, static_cast<int>(dW));
            }
            if ((tl.m_eAlign & Qt::AlignLeft) > 0)
            {
                dX = 0;
            }
            else if ((tl.m_eAlign & Qt::AlignHCenter) > 0)
            {
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
                dX = (dW - fm.horizontalAdvance(qsText))/2;
#else
                dX = (dW - fm.width(qsText))/2;
#endif
            }
            else
            {
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
                dX = dW - fm.horizontalAdvance(qsText);
#else
                dX = dW - fm.width(qsText);
#endif
            }

            // set up the rotation around top-left corner matrix
            QTransform labelMatrix;
            labelMatrix.translate(labelShape.at(0).x(), labelShape.at(0).y());
            if (d->mirror)
            {
                labelMatrix.scale(-1, 1);
                labelMatrix.rotate(-angle);
                labelMatrix.translate(-dW, 0);
                labelMatrix.translate(dX, dY); // Each string has own position
            }
            else
            {
                labelMatrix.rotate(angle);
                labelMatrix.translate(dX, dY); // Each string has own position
            }

            labelMatrix *= d->matrix;

            if (textAsPaths)
            {
                QPainterPath path;
                path.addText(0, - static_cast<qreal>(fm.ascent())/6., fnt, qsText);

                QGraphicsPathItem* item = new QGraphicsPathItem(parent);
                item->setPath(path);
                item->setBrush(QBrush(Qt::black));
                item->setTransform(labelMatrix);

                dY += tm.GetSpacing();
            }
            else
            {
                QGraphicsSimpleTextItem* item = new QGraphicsSimpleTextItem(parent);
                item->setFont(fnt);
                item->setText(qsText);
                item->setTransform(labelMatrix);

                dY += (fm.height() + tm.GetSpacing());
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::CreateGrainlineItem(QGraphicsItem *parent) const
{
    SCASSERT(parent != nullptr)

    if (not d->grainlineEnabled || d->grainlinePoints.count() < 2)
    {
        return;
    }
    VGraphicsFillItem* item = new VGraphicsFillItem(parent);

    QPainterPath path;

    QVector<QPointF> gPoints = GetGrainline();
    path.moveTo(gPoints.at(0));
    for (auto p : gPoints)
    {
        path.lineTo(p);
    }
    item->setPath(path);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPiece::DetailPath() const
{
    if (IsSeamAllowance() && not IsSeamAllowanceBuiltIn())
    {
        return d->seamAllowance;
    }
    else
    {
        return d->contour;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsPathItem *VLayoutPiece::GetMainItem() const
{
    QGraphicsPathItem *item = new QGraphicsPathItem();
    item->setPath(ContourPath());
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsPathItem *VLayoutPiece::GetMainPathItem() const
{
    QGraphicsPathItem *item = new QGraphicsPathItem();

    QPainterPath path;

    // contour
    QVector<QPointF> points = GetMappedContourPoints();

    path.moveTo(points.at(0));
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points.at(i));
    }
    path.lineTo(points.at(0));

    item->setPath(path);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPiece::IsMirror() const
{
    return d->mirror;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetMirror(bool value)
{
    d->mirror = value;
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VLayoutPiece::Edge(const QVector<QPointF> &path, int i) const
{
    if (i < 1)
    { // Doesn't exist such edge
        return QLineF();
    }

    int i1, i2;
    if (i < path.count())
    {
        i1 = i-1;
        i2 = i;
    }
    else
    {
        i1 = path.count()-1;
        i2 = 0;
    }

    if (d->mirror)
    {
        QVector<QPointF> newPath = Map(path);
        return QLineF(newPath.at(i1), newPath.at(i2));
    }
    else
    {
        return QLineF(d->matrix.map(path.at(i1)), d->matrix.map(path.at(i2)));
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPiece::EdgeByPoint(const QVector<QPointF> &path, const QPointF &p1) const
{
    if (p1.isNull())
    {
        return 0;
    }

    if (path.count() < 3)
    {
        return 0;
    }

    const QVector<QPointF> points = Map(path);
    for (int i=0; i < points.size(); i++)
    {
        if (VFuzzyComparePoints(points.at(i), p1))
        {
            int pos = i+1;
            if (pos > points.size())
            {
                pos = 1;
            }
            return pos;
        }
    }
    return 0; // Did not find edge
}
