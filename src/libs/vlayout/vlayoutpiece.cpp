/************************************************************************
 **
 **  @file   vlayoutdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vlayoutpiece.h"

#include <QBrush>
#include <QFlags>
#include <QFont>
#include <QFontMetrics>
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
#include "../vpatterndb/calculator.h"
#include "../vgeometry/vpointf.h"
#include "vlayoutdef.h"
#include "vlayoutpiece_p.h"
#include "vtextmanager.h"
#include "vgraphicsfillitem.h"

namespace
{
//---------------------------------------------------------------------------------------------------------------------
QVector<VLayoutPiecePath> ConvertInternalPaths(const VPiece &piece, const VContainer *pattern)
{
    SCASSERT(pattern != nullptr)

    QVector<VLayoutPiecePath> paths;
    const QVector<quint32> pathsId = piece.GetInternalPaths();
    for (int i = 0; i < pathsId.size(); ++i)
    {
        const VPiecePath path = pattern->GetPiecePath(pathsId.at(i));
        if (path.GetType() == PiecePathType::InternalPath)
        {
            paths.append(VLayoutPiecePath(path.PathPoints(pattern), path.GetPenType()));
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
        rotationAngle = cal1.EvalFormula(pattern->PlainVariables(), labelData.GetRotation());
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
        labelWidth = cal1.EvalFormula(pattern->PlainVariables(), labelData.GetLabelWidth());

        Calculator cal2;
        labelHeight = cal2.EvalFormula(pattern->PlainVariables(), labelData.GetLabelHeight());
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
bool FindGrainlineGeometry(const VGrainlineData& geom, const VContainer *pattern, qreal &length, qreal &rotationAngle,
                           QPointF &pos)
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
        rotationAngle = cal1.EvalFormula(pattern->PlainVariables(), geom.GetRotation());
        rotationAngle = qDegreesToRadians(rotationAngle);

        Calculator cal2;
        length = cal2.EvalFormula(pattern->PlainVariables(), geom.GetLength());
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
bool IsItemContained(const QRectF &parentBoundingRect, const QVector<QPointF> &shape, qreal &dX, qreal &dY)
{
    dX = 0;
    dY = 0;
    // single point differences
    bool bInside = true;

    for (int i = 0; i < shape.size(); ++i)
    {
        qreal dPtX = 0;
        qreal dPtY = 0;
        if (not parentBoundingRect.contains(shape.at(i)))
        {
            if (shape.at(i).x() < parentBoundingRect.left())
            {
                dPtX = parentBoundingRect.left() - shape.at(i).x();
            }
            else if (shape.at(i).x() > parentBoundingRect.right())
            {
                dPtX = parentBoundingRect.right() - shape.at(i).x();
            }

            if (shape.at(i).y() < parentBoundingRect.top())
            {
                dPtY = parentBoundingRect.top() - shape.at(i).y();
            }
            else if (shape.at(i).y() > parentBoundingRect.bottom())
            {
                dPtY = parentBoundingRect.bottom() - shape.at(i).y();
            }

            if (fabs(dPtX) > fabs(dX))
            {
                dX = dPtX;
            }

            if (fabs(dPtY) > fabs(dY))
            {
                dY = dPtY;
            }

            bInside = false;
        }
    }
    return bInside;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> CorrectPosition(const QRectF &parentBoundingRect, QVector<QPointF> points)
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
QVector<QPointF> RoundPoints(QVector<QPointF> points)
{
    for (int i=0; i < points.size(); ++i)
    {
        points[i] = QPointF(qRound(points.at(i).x()), qRound(points.at(i).y()));
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> PrepareAllowance(const QVector<QPointF> &points)
{
    QVector<VSAPoint> allowancePoints;
    for(int i = 0; i < points.size(); ++i)
    {
        allowancePoints.append(VSAPoint(points.at(i)));
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
void CreateLabel(QGraphicsItem *parent, const QPainterPath &path)
{
    SCASSERT(parent != nullptr)

    if (not path.isEmpty())
    {
        QGraphicsPathItem* item = new QGraphicsPathItem(parent);
        item->setPath(path);
        item->setBrush(QBrush(Qt::black));
    }
}
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

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece::~VLayoutPiece()
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece VLayoutPiece::Create(const VPiece &piece, const VContainer *pattern)
{
    VLayoutPiece det;
    det.SetCountourPoints(piece.MainPathPoints(pattern), piece.IsHideMainPath());
    det.SetSeamAllowancePoints(piece.SeamAllowancePoints(pattern), piece.IsSeamAllowance(),
                               piece.IsSeamAllowanceBuiltIn());
    det.SetInternalPaths(ConvertInternalPaths(piece, pattern));
    det.SetPassmarks(piece.PassmarksLines(pattern));

    det.SetName(piece.GetName());

    // Very important to set main path first!
    if (det.ContourPath().isEmpty())
    {
        throw VException (tr("Piece %1 doesn't have shape.").arg(piece.GetName()));
    }

    const VPieceLabelData& data = piece.GetPatternPieceData();
    if (data.IsVisible() == true)
    {
        det.SetDetail(piece.GetName(), data, QApplication::font(), pattern);
    }

    const VPatternLabelData& geom = piece.GetPatternInfo();
    if (geom.IsVisible() == true)
    {
        VAbstractPattern* pDoc = qApp->getCurrentDocument();
        det.SetPatternInfo(pDoc, geom, QApplication::font(), VContainer::size(), VContainer::height(), pattern);
    }

    const VGrainlineData& grainlineGeom = piece.GetGrainlineGeometry();
    if (grainlineGeom.IsVisible() == true)
    {
        det.SetGrainline(grainlineGeom, pattern);
    }

    det.SetSAWidth(qApp->toPixel(piece.GetSAWidth()));
    det.SetForbidFlipping(piece.IsForbidFlipping());

    return det;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QVector<QPointF> VLayoutPiece::GetContourPoints() const
{
    return Map(d->contour);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetCountourPoints(const QVector<QPointF> &points, bool hideMainPath)
{
    d->contour = RemoveDublicates(RoundPoints(points), false);
    SetHideMainPath(hideMainPath);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QVector<QPointF> VLayoutPiece::GetSeamAllowancePoints() const
{
    return Map(d->seamAllowance);
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
            d->seamAllowance = RemoveDublicates(RoundPoints(d->seamAllowance), false);
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
void VLayoutPiece::SetDetail(const QString& qsName, const VPieceLabelData& data, const QFont &font,
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

    const qreal dAng = qDegreesToRadians(labelAngle);
    const QPointF ptCenter(ptPos.x() + labelWidth/2, ptPos.y() + labelHeight/2);
    for (int i = 0; i < v.count(); ++i)
    {
        v[i] = RotatePoint(ptCenter, v.at(i), dAng);
    }

    QScopedPointer<QGraphicsItem> item(GetMainItem());
    d->detailLabel = CorrectPosition(item->boundingRect(), RoundPoints(v));

    // generate text
    d->m_tmDetail.SetFont(font);
    d->m_tmDetail.SetFontSize(data.GetFontSize());
    d->m_tmDetail.Update(qsName, data);
    // this will generate the lines of text
    d->m_tmDetail.SetFontSize(data.GetFontSize());
    d->m_tmDetail.FitFontSize(labelWidth, labelHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetPatternInfo(const VAbstractPattern* pDoc, const VPatternLabelData& geom, const QFont &font,
                                   qreal dSize, qreal dHeight, const VContainer *pattern)
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

    const qreal dAng = qDegreesToRadians(labelAngle);
    const QPointF ptCenter(ptPos.x() + labelWidth/2, ptPos.y() + labelHeight/2);
    for (int i = 0; i < v.count(); ++i)
    {
        v[i] = RotatePoint(ptCenter, v.at(i), dAng);
    }
    QScopedPointer<QGraphicsItem> item(GetMainItem());
    d->patternInfo = CorrectPosition(item->boundingRect(), RoundPoints(v));

    // Generate text
    d->m_tmPattern.SetFont(font);
    d->m_tmPattern.SetFontSize(geom.GetFontSize());

    d->m_tmPattern.Update(pDoc, dSize, dHeight);

    // generate lines of text
    d->m_tmPattern.SetFontSize(geom.GetFontSize());
    d->m_tmPattern.FitFontSize(labelWidth, labelHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetGrainline(const VGrainlineData& geom, const VContainer* pattern)
{
    SCASSERT(pattern != nullptr)

    QPointF pt1;
    qreal dAng = 0;
    qreal dLen = 0;
    if ( not FindGrainlineGeometry(geom, pattern, dLen, dAng, pt1))
    {
        return;
    }

    QPointF pt2(pt1.x() + dLen * qCos(dAng), pt1.y() - dLen * qSin(dAng));

    const qreal dArrowLen = ToPixel(0.5, *pattern->GetPatternUnit());
    const qreal dArrowAng = M_PI/9;

    QVector<QPointF> v;
    v << pt1;

    if (geom.GetArrowType() != ArrowType::atFront)
    {
        v << QPointF(pt1.x() + dArrowLen * qCos(dAng + dArrowAng), pt1.y() - dArrowLen * qSin(dAng + dArrowAng));
        v << QPointF(pt1.x() + dArrowLen * qCos(dAng - dArrowAng), pt1.y() - dArrowLen * qSin(dAng - dArrowAng));
        v << pt1;
    }

    v << pt2;

    if (geom.GetArrowType() != ArrowType::atRear)
    {
        dAng += M_PI;

        v << QPointF(pt2.x() + dArrowLen * qCos(dAng + dArrowAng), pt2.y() - dArrowLen * qSin(dAng + dArrowAng));
        v << QPointF(pt2.x() + dArrowLen * qCos(dAng - dArrowAng), pt2.y() - dArrowLen * qSin(dAng - dArrowAng));
        v << pt2;
    }

    QScopedPointer<QGraphicsItem> item(GetMainItem());
    d->grainlinePoints = CorrectPosition(item->boundingRect(), RoundPoints(v));
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
void VLayoutPiece::SetLayoutWidth(const qreal &value)
{
    d->layoutWidth = value;
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
int VLayoutPiece::DetailEdgesCount() const
{
    return DetailPath().count();
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPiece::LayoutEdgesCount() const
{
    return d->layoutAllowance.count();
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VLayoutPiece::DetailEdge(int i) const
{
    return Edge(DetailPath(), i);
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VLayoutPiece::LayoutEdge(int i) const
{
    return Edge(d->layoutAllowance, i);
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPiece::DetailEdgeByPoint(const QPointF &p1) const
{
    return EdgeByPoint(DetailPath(), p1);
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPiece::LayoutEdgeByPoint(const QPointF &p1) const
{
    return EdgeByPoint(d->layoutAllowance, p1);
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VLayoutPiece::DetailBoundingRect() const
{
    QVector<QPointF> points;
    if (IsSeamAllowance() && not IsSeamAllowanceBuiltIn())
    {
        points = GetSeamAllowancePoints();
    }
    else
    {
        points = GetContourPoints();
    }

    points.append(points.first());
    return QPolygonF(points).boundingRect();
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VLayoutPiece::LayoutBoundingRect() const
{
    QVector<QPointF> points = GetLayoutAllowancePoints();
    points.append(points.first());
    return QPolygonF(points).boundingRect();
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
    if (d->layoutAllowance.isEmpty()) //-V807
    {
        return 0;
    }

    const qreal res = SumTrapezoids(d->layoutAllowance);

    const qint64 sq = qFloor(qAbs(res/2.0));
    return sq;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetLayoutAllowancePoints()
{
    if (d->layoutWidth > 0)
    {
        if (IsSeamAllowance() && not IsSeamAllowanceBuiltIn())
        {
            d->layoutAllowance = Equidistant(PrepareAllowance(GetSeamAllowancePoints()), d->layoutWidth);
            if (d->layoutAllowance.isEmpty() == false)
            {
                d->layoutAllowance.removeLast();
            }
        }
        else
        {
            d->layoutAllowance = Equidistant(PrepareAllowance(GetContourPoints()), d->layoutWidth);
            if (d->layoutAllowance.isEmpty() == false)
            {
                d->layoutAllowance.removeLast();
            }
        }
    }
    else
    {
        d->layoutAllowance.clear();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QLineF> VLayoutPiece::GetPassmarks() const
{
    return Map(d->passmarks);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetPassmarks(const QVector<QLineF> &passmarks)
{
    if (IsSeamAllowance())
    {
        d->passmarks = passmarks;
    }
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
template <class T>
QVector<T> VLayoutPiece::Map(const QVector<T> &points) const
{
    QVector<T> p;
    for (int i = 0; i < points.size(); ++i)
    {
        p.append(d->matrix.map(points.at(i)));
    }

    if (d->mirror)
    {
        QList<T> list = p.toList();
        for (int k=0, s=list.size(), max=(s/2); k<max; k++)
        {
            list.swap(k, s-(1+k));
        }
        p = list.toVector();
    }
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPiece::ContourPath() const
{
    QPainterPath path;

    // contour
    QVector<QPointF> points = GetContourPoints();

    if (not IsHideMainPath() || not IsSeamAllowance() || IsSeamAllowanceBuiltIn())
    {
        path.moveTo(points.at(0));
        for (qint32 i = 1; i < points.count(); ++i)
        {
            path.lineTo(points.at(i));
        }
        path.lineTo(points.at(0));
    }

    // seam allowance
    if (IsSeamAllowance())
    {
        if (not IsSeamAllowanceBuiltIn())
        {
            // Draw seam allowance
            points = GetSeamAllowancePoints();

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
        const QVector<QLineF> passmarks = GetPassmarks();
        QPainterPath passmaksPath;
        for (qint32 i = 0; i < passmarks.count(); ++i)
        {
            passmaksPath.moveTo(passmarks.at(i).p1());
            passmaksPath.lineTo(passmarks.at(i).p2());
        }

        path.addPath(passmaksPath);
        path.setFillRule(Qt::WindingFill);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::CreateInternalPathItem(int i, QGraphicsItem *parent) const
{
    SCASSERT(parent != nullptr)
    QGraphicsPathItem* item = new QGraphicsPathItem(parent);
    item->setPath(d->matrix.map(d->m_internalPaths.at(i).GetPainterPath()));

    QPen pen = item->pen();
    pen.setStyle(d->m_internalPaths.at(i).PenStyle());
    item->setPen(pen);
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPiece::LayoutAllowancePath() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    const QVector<QPointF> points = GetLayoutAllowancePoints();
    path.moveTo(points.at(0));
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points.at(i));
    }
    path.lineTo(points.at(0));

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsItem *VLayoutPiece::GetItem() const
{
    QGraphicsPathItem *item = GetMainItem();

    for (int i = 0; i < d->m_internalPaths.count(); ++i)
    {
        CreateInternalPathItem(i, item);
    }

    CreateLabel(item, CreateLabelText(d->detailLabel, d->m_tmDetail));
    CreateLabel(item, CreateLabelText(d->patternInfo, d->m_tmPattern));
    CreateGrainlineItem(item);

    return item;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::CreateGrainlineItem(QGraphicsItem *parent) const
{
    SCASSERT(parent != nullptr)

    if (d->grainlinePoints.count() < 2)
    {
        return;
    }
    VGraphicsFillItem* item = new VGraphicsFillItem(parent);

    QPainterPath path;

    QVector<QPointF> gPoints = Map(d->grainlinePoints);
    path.moveTo(gPoints.at(0));
    for (int i = 1; i < gPoints.count(); ++i)
    {
        path.lineTo(gPoints.at(i));
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
    if (i < 1 || i > path.count())
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
        const int oldI1 = i1;
        const int size = path.size()-1; //-V807
        i1 = size - i2;
        i2 = size - oldI1;
        return QLineF(d->matrix.map(path.at(i2)), d->matrix.map(path.at(i1)));
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
        if (points.at(i) == p1)
        {
            return i+1;
        }
    }
    return 0; // Did not find edge
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPiece::CreateLabelText(const QVector<QPointF> &labelShape, const VTextManager &tm) const
{
    QPainterPath textpath;
    if (labelShape.count() > 2)
    {
        const qreal dW = QLineF(labelShape.at(0), labelShape.at(1)).length();
        const qreal dH = QLineF(labelShape.at(1), labelShape.at(2)).length();
        const qreal angle = QLineF(labelShape.at(0), labelShape.at(1)).angle();
        qreal dY = 0;
        qreal dX;

        // set up the rotation around top-left corner matrix
        QTransform mat;
        mat.translate(labelShape.at(0).x(), labelShape.at(0).y());
        if (d->mirror)
        {
            mat.scale(-1, 1);
            mat.rotate(angle);
            mat.translate(-dW, 0);
        }
        else
        {
            mat.rotate(angle);
        }

        mat *= d->matrix;

        for (int i = 0; i < tm.GetSourceLinesCount(); ++i)
        {
            const TextLine& tl = tm.GetSourceLine(i);
            QFont fnt = tm.GetFont();
            fnt.setPixelSize(tm.GetFont().pixelSize() + tl.m_iFontSize);
            fnt.setWeight(tl.m_eFontWeight);
            fnt.setStyle(tl.m_eStyle);

            QFontMetrics fm(fnt);

            dY += fm.height();
            if (dY > dH)
            {
                break;
            }

            QString qsText = tl.m_qsText;
            if (fm.width(qsText) > dW)
            {
                qsText = fm.elidedText(qsText, Qt::ElideMiddle, static_cast<int>(dW));
            }
            if ((tl.m_eAlign & Qt::AlignLeft) > 0)
            {
                dX = 0;
            }
            else if ((tl.m_eAlign & Qt::AlignHCenter) > 0)
            {
                dX = (dW - fm.width(qsText))/2;
            }
            else
            {
                dX = dW - fm.width(qsText);
            }
            QPainterPath path;
            path.addText(dX, dY - (fm.height() - fm.ascent())/2, fnt, qsText);
            textpath.addPath(mat.map(path));
            dY += tm.GetSpacing();
        }
    }

    return textpath;
}
