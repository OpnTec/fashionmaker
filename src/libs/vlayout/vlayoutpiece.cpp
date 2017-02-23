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

#include "../vpatterndb/vpatterninfogeometry.h"
#include "../vpatterndb/vpiecelabeldata.h"
#include "../vmisc/vmath.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/calculator.h"
#include "vlayoutdef.h"
#include "vlayoutpiece_p.h"
#include "vtextmanager.h"
#include "vgraphicsfillitem.h"

namespace
{
QVector<VLayoutPiecePath> ConvertInternalPaths(const VPiece &piece, const VContainer *pattern)
{
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
    det.SetCountourPoints(piece.MainPathPoints(pattern));
    det.SetSeamAllowancePoints(piece.SeamAllowancePoints(pattern), piece.IsSeamAllowance());
    det.SetInternalPaths(ConvertInternalPaths(piece, pattern));

    det.SetName(piece.GetName());
    const VPieceLabelData& data = piece.GetPatternPieceData();
    if (data.IsVisible() == true)
    {
        det.SetDetail(piece.GetName(), data, qApp->font());
    }
    const VPatternInfoGeometry& geom = piece.GetPatternInfo();
    if (geom.IsVisible() == true)
    {
        VAbstractPattern* pDoc = qApp->getCurrentDocument();
        det.SetPatternInfo(pDoc, geom, qApp->font(), pattern->size(), pattern->height());
    }
    const VGrainlineGeometry& grainlineGeom = piece.GetGrainlineGeometry();
    if (grainlineGeom.IsVisible() == true)
    {
        det.SetGrainline(grainlineGeom, *pattern);
    }
    det.SetSAWidth(qApp->toPixel(piece.GetSAWidth()));
    det.CreateTextItems();
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
void VLayoutPiece::SetCountourPoints(const QVector<QPointF> &points)
{
    d->contour = RemoveDublicates(RoundPoints(points), false);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QVector<QPointF> VLayoutPiece::GetSeamAllowancePoints() const
{
    return Map(d->seamAllowance);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetSeamAllowancePoints(const QVector<QPointF> &points, bool seamAllowance)
{
    if (seamAllowance)
    {
        SetSeamAllowance(seamAllowance);
        d->seamAllowance = points;
        if (not d->seamAllowance.isEmpty())
        {
            d->seamAllowance = RemoveDublicates(RoundPoints(d->seamAllowance), false);
        }
        else
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
void VLayoutPiece::SetDetail(const QString& qsName, const VPieceLabelData& data, const QFont &font)
{
    d->detailData = data;
    qreal dAng = qDegreesToRadians(data.GetRotation());
    QPointF ptCenter(data.GetPos().x() + data.GetLabelWidth()/2, data.GetPos().y() + data.GetLabelHeight()/2);
    QPointF ptPos = data.GetPos();
    QVector<QPointF> v;
    v << ptPos << QPointF(ptPos.x() + data.GetLabelWidth(), ptPos.y())
      << QPointF(ptPos.x() + data.GetLabelWidth(), ptPos.y() + data.GetLabelHeight())
         << QPointF(ptPos.x(), ptPos.y() + data.GetLabelHeight());
    for (int i = 0; i < v.count(); ++i)
    {
        v[i] = RotatePoint(ptCenter, v.at(i), dAng);
    }
    d->detailLabel = RoundPoints(v);

    // generate text
    d->m_tmDetail.SetFont(font);
    d->m_tmDetail.SetFontSize(data.GetFontSize());
    d->m_tmDetail.Update(qsName, data);
    // this will generate the lines of text
    d->m_tmDetail.SetFontSize(data.GetFontSize());
    d->m_tmDetail.FitFontSize(data.GetLabelWidth(), data.GetLabelHeight());
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetPatternInfo(const VAbstractPattern* pDoc, const VPatternInfoGeometry& geom, const QFont &font,
                                   qreal dSize, qreal dHeight)
{
    d->patternGeom = geom;
    qreal dAng = qDegreesToRadians(geom.GetRotation());
    QPointF ptCenter(geom.GetPos().x() + geom.GetLabelWidth()/2, geom.GetPos().y() + geom.GetLabelHeight()/2);
    QPointF ptPos = geom.GetPos();
    QVector<QPointF> v;
    v << ptPos << QPointF(ptPos.x() + geom.GetLabelWidth(), ptPos.y())
         << QPointF(ptPos.x() + geom.GetLabelWidth(), ptPos.y() + geom.GetLabelHeight())
            << QPointF(ptPos.x(), ptPos.y() + geom.GetLabelHeight());
    for (int i = 0; i < v.count(); ++i)
    {
        v[i] = RotatePoint(ptCenter, v.at(i), dAng);
    }
    d->patternInfo = RoundPoints(v);

    // Generate text
    d->m_tmPattern.SetFont(font);
    d->m_tmPattern.SetFontSize(geom.GetFontSize());

    d->m_tmPattern.Update(pDoc, dSize, dHeight);

    // generate lines of text
    d->m_tmPattern.SetFontSize(geom.GetFontSize());
    d->m_tmPattern.FitFontSize(geom.GetLabelWidth(), geom.GetLabelHeight());
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::SetGrainline(const VGrainlineGeometry& geom, const VContainer& rPattern)
{
    d->grainlineGeom = geom;
    qreal dAng;
    qreal dLen;

    try
    {
        QString qsFormula = geom.GetRotation().replace("\n", " ");
        qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
        Calculator cal1;
        dAng = cal1.EvalFormula(rPattern.PlainVariables(), qsFormula);
        dAng = qDegreesToRadians(dAng);

        qsFormula = geom.GetLength().replace("\n", " ");
        qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
        Calculator cal2;
        dLen = cal2.EvalFormula(rPattern.PlainVariables(), qsFormula);
        dLen = ToPixel(dLen, *rPattern.GetPatternUnit());
    }
    catch(qmu::QmuParserError &e)
    {
        Q_UNUSED(e);
        return;
    }

    QPointF pt1 = geom.GetPos();
    QPointF pt2;
    pt2.setX(pt1.x() + dLen * qCos(dAng));
    pt2.setY(pt1.y() - dLen * qSin(dAng));
    QVector<QPointF> v;
    QPointF pt;
    qreal dArrowLen = ToPixel(0.5, *rPattern.GetPatternUnit());
    qreal dArrowAng = M_PI/9;

    v << pt1;

    if (geom.GetArrowType() != VGrainlineGeometry::atRear) {
        pt.setX(pt1.x() + dArrowLen * qCos(dAng + dArrowAng));
        pt.setY(pt1.y() - dArrowLen * qSin(dAng + dArrowAng));
        v << pt;
        pt.setX(pt1.x() + dArrowLen * qCos(dAng - dArrowAng));
        pt.setY(pt1.y() - dArrowLen * qSin(dAng - dArrowAng));
        v << pt;

        v << pt1;
    }

    v << pt2;

    if (geom.GetArrowType() != VGrainlineGeometry::atFront)
    {
        dAng += M_PI;

        pt.setX(pt2.x() + dArrowLen * qCos(dAng + dArrowAng));
        pt.setY(pt2.y() - dArrowLen * qSin(dAng + dArrowAng));
        v << pt;
        pt.setX(pt2.x() + dArrowLen * qCos(dAng - dArrowAng));
        pt.setY(pt2.y() - dArrowLen * qSin(dAng - dArrowAng));
        v << pt;

        v << pt2;
    }

    d->grainlinePoints = RoundPoints(v);
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
    if (IsSeamAllowance())
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
        if (IsSeamAllowance() && d->seamAllowance.isEmpty() == false)
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
        if (IsSeamAllowance())
        {
            d->layoutAllowance = Equidistant(PrepareAllowance(GetSeamAllowancePoints()), d->layoutWidth);
            if (d->layoutAllowance.isEmpty() == false)
            {
                #if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
                    d->layoutAllowance.remove(d->layoutAllowance.size() - 1);
                #else
                    d->layoutAllowance.removeLast();
                #endif
            }
        }
        else
        {
            d->layoutAllowance = Equidistant(PrepareAllowance(GetContourPoints()), d->layoutWidth);
            if (d->layoutAllowance.isEmpty() == false)
            {
            #if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
                d->layoutAllowance.remove(d->layoutAllowance.size() - 1);
            #else
                d->layoutAllowance.removeLast();
            #endif
            }
        }
    }
    else
    {
        d->layoutAllowance.clear();
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
QVector<QPointF> VLayoutPiece::Map(const QVector<QPointF> &points) const
{
    QVector<QPointF> p;
    for (int i = 0; i < points.size(); ++i)
    {
        p.append(d->matrix.map(points.at(i)));
    }

    if (d->mirror)
    {
        QList<QPointF> list = p.toList();
        for (int k=0, s=list.size(), max=(s/2); k<max; k++)
        {
            list.swap(k, s-(1+k));
        }
        p = list.toVector();
    }
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPiece::RoundPoints(const QVector<QPointF> &points)
{
    QVector<QPointF> p;
    for (int i=0; i < points.size(); ++i)
    {
        p.append(QPointF(qRound(points.at(i).x()), qRound(points.at(i).y())));
    }
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPiece::ContourPath() const
{
    QPainterPath path;

    // contour
    QVector<QPointF> points = GetContourPoints();
    path.moveTo(points.at(0));
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points.at(i));
    }
    path.lineTo(points.at(0));

    // seam allowance
    if (IsSeamAllowance() == true)
    {
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
        path.setFillRule(Qt::WindingFill);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::ClearTextItems()
{
    d->m_liPP.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiece::CreateTextItems()
{
    ClearTextItems();
    // first add detail texts
    if (d->detailLabel.count() > 0)
    {
        // get the mapped label vertices
        QVector<QPointF> points = Map(Mirror(d->detailLabel));
        // append the first point to obtain the closed rectangle
        points.push_back(points.at(0));
        // calculate the angle of rotation
        qreal dAng = qAtan2(points.at(1).y() - points.at(0).y(), points.at(1).x() - points.at(0).x());
        // calculate the label width and height
        qreal dW = GetDistance(points.at(0), points.at(1));
        qreal dH = GetDistance(points.at(1), points.at(2));
        qreal dY = 0;
        qreal dX;
        // set up the rotation around top-left corner matrix
        QMatrix mat;
        mat.translate(points.at(0).x(), points.at(0).y());
        mat.rotate(qRadiansToDegrees(dAng));

        for (int i = 0; i < d->m_tmDetail.GetSourceLinesCount(); ++i)
        {
            const TextLine& tl = d->m_tmDetail.GetSourceLine(i);
            QFont fnt = d->m_tmDetail.GetFont();
            fnt.setPixelSize(d->m_tmDetail.GetFont().pixelSize() + tl.m_iFontSize);
            fnt.setWeight(tl.m_eFontWeight);
            fnt.setStyle(tl.m_eStyle);

            QFontMetrics fm(fnt);

            dY += fm.height();
            // check if the next line will go out of bounds
            if (dY > dH)
            {
                break;
            }

            QString qsText = tl.m_qsText;
            if (fm.width(qsText) > dW)
            {
                qsText = fm.elidedText(qsText, Qt::ElideMiddle, static_cast<int>(dW));
            }
            // find the correct horizontal offset, depending on the alignment flag
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
            // create text path and add it to the list
            QPainterPath path;
            path.addText(dX, dY - (fm.height() - fm.ascent())/2, fnt, qsText);
            d->m_liPP << mat.map(path);
            dY += d->m_tmDetail.GetSpacing();
        }
    }
    // and then add pattern texts
    if (d->patternInfo.count() > 0)
    {
        // similar approach like for the detail label
        QVector<QPointF> points = Map(Mirror(d->patternInfo));
        points.push_back(points.at(0));
        qreal dAng = qAtan2(points.at(1).y() - points.at(0).y(), points.at(1).x() - points.at(0).x());
        qreal dW = GetDistance(points.at(0), points.at(1));
        qreal dH = GetDistance(points.at(1), points.at(2));
        qreal dY = 0;
        qreal dX;
        QMatrix mat;
        mat.translate(points.at(0).x(), points.at(0).y());
        mat.rotate(qRadiansToDegrees(dAng));

        for (int i = 0; i < d->m_tmPattern.GetSourceLinesCount(); ++i)
        {
            const TextLine& tl = d->m_tmPattern.GetSourceLine(i);
            QFont fnt = d->m_tmPattern.GetFont();
            fnt.setPixelSize(d->m_tmPattern.GetFont().pixelSize() + tl.m_iFontSize);
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
            d->m_liPP << mat.map(path);
            dY += d->m_tmPattern.GetSpacing();
        }
    }
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
/**
 * @brief CreateTextItem Creates the i-th text item
 * @param i index of the requested item
 * @param parent parent of this text item. Can't be null.
 */
void VLayoutPiece::CreateTextItem(int i, QGraphicsItem *parent) const
{
    SCASSERT(parent != nullptr)

    QGraphicsPathItem* item = new QGraphicsPathItem(parent);
    QPainterPath path = d->matrix.map(d->m_liPP.at(i));

    if (d->mirror == true)
    {
        QVector<QPointF> points;
        if (i < d->m_tmDetail.GetSourceLinesCount())
        {
            points = Map(Mirror(d->detailLabel));
        }
        else
        {
            points = Map(Mirror(d->patternInfo));
        }
        QPointF ptCenter = (points.at(1) + points.at(3))/2;
        qreal dRot = qRadiansToDegrees(qAtan2(points.at(1).y() - points.at(0).y(),
                                              points.at(1).x() - points.at(0).x()));

        // we need to move the center back to the origin, rotate it to align it with x axis,
        // then mirror it to obtain the proper text direction, rotate it and translate it back to original position.
        // The operations must be added in reverse order
        QTransform t;
        // move the label back to its original position
        t.translate(ptCenter.x(), ptCenter.y());
        // rotate the label back to original angle
        t.rotate(dRot);
        // mirror the label horizontally
        t.scale(-1, 1);
        // rotate the label to normal position
        t.rotate(-dRot);
        // move the label center into origin
        t.translate(-ptCenter.x(), -ptCenter.y());
        path = t.map(path);
    }

    item->setPath(path);
    item->setBrush(QBrush(Qt::black));
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
    QGraphicsPathItem *item = new QGraphicsPathItem();
    item->setPath(ContourPath());

    for (int i = 0; i < d->m_internalPaths.count(); ++i)
    {
        CreateInternalPathItem(i, item);
    }

    for (int i = 0; i < d->m_liPP.count(); ++i)
    {
        CreateTextItem(i, item);
    }

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
    QVector<QPointF> v = Map(d->grainlinePoints);
    path.moveTo(v.at(0));
    for (int i = 1; i < v.count(); ++i)
    {
        path.lineTo(v.at(i));
    }
    item->setPath(path);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPiece::DetailPath() const
{
    if (IsSeamAllowance())
    {
        return d->seamAllowance;
    }
    else
    {
        return d->contour;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VSAPoint> VLayoutPiece::PrepareAllowance(const QVector<QPointF> &points)
{
    QVector<VSAPoint> allowancePoints;
    for(int i = 0; i < points.size(); ++i)
    {
        allowancePoints.append(VSAPoint(points.at(i)));
    }
    return allowancePoints;
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
/**
 * @brief VLayoutDetail::RotatePoint rotates a point around the center for given angle
 * @param ptCenter center around which the point is rotated
 * @param pt point, which is rotated around the center
 * @param dAng angle of rotation
 * @return position of point pt after rotating it around the center for dAng radians
 */
QPointF VLayoutPiece::RotatePoint(const QPointF &ptCenter, const QPointF& pt, qreal dAng)
{
    QPointF ptDest;
    QPointF ptRel = pt - ptCenter;
    ptDest.setX(cos(dAng)*ptRel.x() - sin(dAng)*ptRel.y());
    ptDest.setY(sin(dAng)*ptRel.x() + cos(dAng)*ptRel.y());

    return ptDest + ptCenter;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VLayoutDetail::Mirror if the detail layout is rotated, this method will flip the
 *  label points over vertical axis, which goes through the center of the label
 * @param points list of 4 label vertices
 * @return list of flipped points
 */
QVector<QPointF> VLayoutPiece::Mirror(const QVector<QPointF> &points) const
{
    // should only call this method with rectangular shapes
    Q_ASSERT(points.count() == 4);
    if (d->mirror == false)
    {
        return points;
    }

    QVector<QPointF> v;
    v.resize(4);
    v[0] = points.at(2);
    v[1] = points.at(3);
    v[2] = points.at(0);
    v[3] = points.at(1);
    return v;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VLayoutDetail::GetDistance calculates the Euclidian distance between the points
 * @param pt1 first point
 * @param pt2 second point
 * @return Euclidian distance between the two points
 */
qreal VLayoutPiece::GetDistance(const QPointF &pt1, const QPointF &pt2)
{
    const qreal dX = pt1.x() - pt2.x();
    const qreal dY = pt1.y() - pt2.y();

    return qSqrt(dX*dX + dY*dY);
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
