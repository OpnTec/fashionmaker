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

#include "vlayoutdetail.h"

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
#include "../vpatterndb/vpatternpiecedata.h"
#include "../vmisc/vmath.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/calculator.h"
#include "vlayoutdef.h"
#include "vlayoutdetail_p.h"
#include "vtextmanager.h"
#include "vgraphicsfillitem.h"

class QGraphicsPathItem;
class QLineF;
class VAbstractPattern;

//---------------------------------------------------------------------------------------------------------------------
VLayoutDetail::VLayoutDetail()
    :VAbstractDetail(), d(new VLayoutDetailData)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutDetail::VLayoutDetail(const VLayoutDetail &detail)
    :VAbstractDetail(detail), d(detail.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutDetail &VLayoutDetail::operator=(const VLayoutDetail &detail)
{
    if ( &detail == this )
    {
        return *this;
    }
    VAbstractDetail::operator=(detail);
    d = detail.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutDetail::~VLayoutDetail()
{}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QVector<QPointF> VLayoutDetail::GetContourPoints() const
{
    return d->contour;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetCountourPoints(const QVector<QPointF> &points)
{
    d->contour = RemoveDublicates(RoundPoints(points));
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QVector<QPointF> VLayoutDetail::GetSeamAllowencePoints() const
{
    return d->seamAllowence;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetSeamAllowencePoints(const QVector<QPointF> &points, bool seamAllowence, bool closed)
{
    if (seamAllowence)
    {
        setSeamAllowance(seamAllowence);
        setClosed(closed);
        d->seamAllowence = points;
        if (not d->seamAllowence.isEmpty())
        {
            d->seamAllowence = RemoveDublicates(RoundPoints(d->seamAllowence));
        }
        else
        {
            qWarning()<<"Seam allowence is empty.";
            setSeamAllowance(false);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutDetail::GetLayoutAllowencePoints() const
{
    return Map(d->layoutAllowence);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetDetail(const QString& qsName, const VPatternPieceData& data, const QFont &font)
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
void VLayoutDetail::SetPatternInfo(const VAbstractPattern* pDoc, const VPatternInfoGeometry& geom, const QFont &font,
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
void VLayoutDetail::SetGrainline(const VGrainlineGeometry& geom, const VContainer& rPattern)
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
    catch(...)
    {
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

    if (geom.HasFrontArrow() == true) {
        pt.setX(pt1.x() + dArrowLen * qCos(dAng + dArrowAng));
        pt.setY(pt1.y() - dArrowLen * qSin(dAng + dArrowAng));
        v << pt;
        pt.setX(pt1.x() + dArrowLen * qCos(dAng - dArrowAng));
        pt.setY(pt1.y() - dArrowLen * qSin(dAng - dArrowAng));
        v << pt;

        v << pt1;
    }

    v << pt2;

    if (geom.HasRearArrow() == true)
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
QTransform VLayoutDetail::GetMatrix() const
{
    return d->matrix;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetMatrix(const QTransform &matrix)
{
    d->matrix = matrix;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutDetail::GetLayoutWidth() const
{
    return d->layoutWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetLayoutWidth(const qreal &value)
{
    d->layoutWidth = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::Translate(qreal dx, qreal dy)
{
    QTransform m;
    m.translate(dx, dy);
    d->matrix *= m;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::Rotate(const QPointF &originPoint, qreal degrees)
{
    QTransform m;
    m.translate(originPoint.x(), originPoint.y());
    m.rotate(-degrees);
    m.translate(-originPoint.x(), -originPoint.y());
    d->matrix *= m;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::Mirror(const QLineF &edge)
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
int VLayoutDetail::EdgesCount() const
{
    return d->layoutAllowence.count();
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VLayoutDetail::Edge(int i) const
{
    if (i < 1 || i > EdgesCount())
    { // Doesn't exist such edge
        return QLineF();
    }

    int i1, i2;
    if (i < EdgesCount())
    {
        i1 = i-1;
        i2 = i;
    }
    else
    {
        i1 = EdgesCount()-1;
        i2 = 0;
    }

    if (d->mirror)
    {
        const int oldI1 = i1;
        const int size = d->layoutAllowence.size()-1; //-V807
        i1 = size - i2;
        i2 = size - oldI1;
        return QLineF(d->matrix.map(d->layoutAllowence.at(i2)), d->matrix.map(d->layoutAllowence.at(i1)));
    }
    else
    {
        return QLineF(d->matrix.map(d->layoutAllowence.at(i1)), d->matrix.map(d->layoutAllowence.at(i2)));
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutDetail::EdgeByPoint(const QPointF &p1) const
{
    if (p1.isNull())
    {
        return 0;
    }

    if (EdgesCount() < 3)
    {
        return 0;
    }

    const QVector<QPointF> points = GetLayoutAllowencePoints();
    for (int i=0; i< points.size(); i++)
    {
        if (points.at(i) == p1)
        {
            return i+1;
        }
    }
    return 0; // Did not find edge
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VLayoutDetail::DetailBoundingRect() const
{
    QVector<QPointF> points;
    if (getSeamAllowance())
    {
        points = GetSeamAllowencePoints();
    }
    else
    {
        points = GetContourPoints();
    }

    points.append(points.first());
    return QPolygonF(points).boundingRect();
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VLayoutDetail::LayoutBoundingRect() const
{
    QVector<QPointF> points = GetLayoutAllowencePoints();
    points.append(points.first());
    return QPolygonF(points).boundingRect();
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutDetail::Diagonal() const
{
    const QRectF rec = LayoutBoundingRect();
    return qSqrt(pow(rec.height(), 2) + pow(rec.width(), 2));
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutDetail::isNull() const
{
    if (d->contour.isEmpty() == false && d->layoutWidth > 0)
    {
        if (getSeamAllowance() && d->seamAllowence.isEmpty() == false)
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
qint64 VLayoutDetail::Square() const
{
    if (d->layoutAllowence.isEmpty()) //-V807
    {
        return 0;
    }

    const qreal res = SumTrapezoids(d->layoutAllowence);

    const qint64 sq = qFloor(qAbs(res/2.0));
    return sq;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetLayoutAllowencePoints()
{
    if (d->layoutWidth > 0)
    {
        if (getSeamAllowance())
        {
            d->layoutAllowence = Equidistant(d->seamAllowence, EquidistantType::CloseEquidistant, d->layoutWidth);
            if (d->layoutAllowence.isEmpty() == false)
            {
                #if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
                    d->layoutAllowence.remove(d->layoutAllowence.size() - 1);
                #else
                    d->layoutAllowence.removeLast();
                #endif
            }
        }
        else
        {
            d->layoutAllowence = Equidistant(d->contour, EquidistantType::CloseEquidistant, d->layoutWidth);
            if (d->layoutAllowence.isEmpty() == false)
            {
            #if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
                d->layoutAllowence.remove(d->layoutAllowence.size() - 1);
            #else
                d->layoutAllowence.removeLast();
            #endif
            }
        }
    }
    else
    {
        d->layoutAllowence.clear();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutDetail::Map(const QVector<QPointF> &points) const
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
QVector<QPointF> VLayoutDetail::RoundPoints(const QVector<QPointF> &points)
{
    QVector<QPointF> p;
    for (int i=0; i < points.size(); ++i)
    {
        p.append(QPointF(qRound(points.at(i).x()), qRound(points.at(i).y())));
    }
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutDetail::ContourPath() const
{
    QPainterPath path;

    // contour
    QVector<QPointF> points = Map(d->contour);
    path.moveTo(points.at(0));
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points.at(i));
    }
    path.lineTo(points.at(0));

    // seam allowence
    if (getSeamAllowance() == true)
    {
        points = Map(d->seamAllowence);

        if (getClosed() == true)
        {
            points.append(points.at(0));
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
void VLayoutDetail::ClearTextItems()
{
    d->m_liPP.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::CreateTextItems()
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
        // calculate the label width
        qreal dW = GetDistance(points.at(0), points.at(1));
        qreal dY = 0;
        qreal dX;
        // set up the rotation around top-left corner matrix
        QMatrix mat;
        mat.translate(points.at(0).x(), points.at(0).y());
        mat.rotate(qRadiansToDegrees(dAng));

        for (int i = 0; i < d->m_tmDetail.GetCount(); ++i)
        {
            const TextLine& tl = d->m_tmDetail.GetLine(i);
            QFont fnt = d->m_tmDetail.GetFont();
            fnt.setPixelSize(d->m_tmDetail.GetFont().pixelSize() + tl.m_iFontSize);
            fnt.setWeight(tl.m_eFontWeight);
            fnt.setStyle(tl.m_eStyle);
            dY += tl.m_iHeight;

            QFontMetrics fm(fnt);
            // find the correct horizontal offset, depending on the alignment flag
            if ((tl.m_eAlign & Qt::AlignLeft) > 0)
            {
                dX = 0;
            }
            else if ((tl.m_eAlign & Qt::AlignHCenter) > 0)
            {
                dX = (dW - fm.width(tl.m_qsText))/2;
            }
            else
            {
                dX = dW - fm.width(tl.m_qsText);
            }
            // create text path and add it to the list
            QPainterPath path;
            path.addText(dX, dY - (fm.height() - fm.ascent())/2, fnt, tl.m_qsText);
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
        qreal dY = 0;
        qreal dX;
        QMatrix mat;
        mat.translate(points.at(0).x(), points.at(0).y());
        mat.rotate(qRadiansToDegrees(dAng));

        for (int i = 0; i < d->m_tmPattern.GetCount(); ++i)
        {
            const TextLine& tl = d->m_tmPattern.GetLine(i);
            QFont fnt = d->m_tmPattern.GetFont();
            fnt.setPixelSize(d->m_tmPattern.GetFont().pixelSize() + tl.m_iFontSize);
            fnt.setWeight(tl.m_eFontWeight);
            fnt.setStyle(tl.m_eStyle);
            dY += tl.m_iHeight;

            QFontMetrics fm(fnt);
            if ((tl.m_eAlign & Qt::AlignLeft) > 0)
            {
                dX = 0;
            }
            else if ((tl.m_eAlign & Qt::AlignHCenter) > 0)
            {
                dX = (dW - fm.width(tl.m_qsText))/2;
            }
            else
            {
                dX = dW - fm.width(tl.m_qsText);
            }
            QPainterPath path;
            path.addText(dX, dY - (fm.height() - fm.ascent())/2, fnt, tl.m_qsText);
            d->m_liPP << mat.map(path);
            dY += d->m_tmPattern.GetSpacing();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutDetail::GetTextItemsCount() const
{
    return d->m_liPP.count();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VLayoutDetail::GetTextItem Creates and returns the i-th text item
 * @param i index of the requested item
 * @return pointer to the newly created item. The caller is responsible to delete it.
 */
QGraphicsItem* VLayoutDetail::GetTextItem(int i) const
{
    QGraphicsPathItem* item = new QGraphicsPathItem();
    QTransform transform = d->matrix;

    QPainterPath path = transform.map(d->m_liPP[i]);

    if (d->mirror == true)
    {
        QVector<QPointF> points;
        if (i < d->m_tmDetail.GetCount())
        {
            points = Map(Mirror(d->detailLabel));
        }
        else
        {
            points = Map(Mirror(d->patternInfo));
        }
        QPointF ptCenter = (points.at(1) + points.at(3))/2;
        qreal dRot = qRadiansToDegrees(qAtan2(points.at(1).y() - points.at(0).y(), points.at(1).x() - points.at(0).x()));

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
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutDetail::LayoutAllowencePath() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    const QVector<QPointF> points = GetLayoutAllowencePoints();
    path.moveTo(points.at(0));
    for (qint32 i = 1; i < points.count(); ++i)
    {
        path.lineTo(points.at(i));
    }
    path.lineTo(points.at(0));

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsItem *VLayoutDetail::GetItem() const
{
    QGraphicsPathItem *item = new QGraphicsPathItem();
    item->setPath(ContourPath());
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsItem* VLayoutDetail::GetGrainlineItem() const
{
    if (d->grainlinePoints.count() < 2)
    {
        return 0;
    }
    VGraphicsFillItem* item = new VGraphicsFillItem();
    QPainterPath path;
    QVector<QPointF> v = Map(d->grainlinePoints);
    path.moveTo(v.at(0));
    for (int i = 1; i < v.count(); ++i)
    {
        path.lineTo(v.at(i));
    }
    item->setPath(path);
    return item;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutDetail::IsMirror() const
{
    return d->mirror;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetMirror(bool value)
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
QPointF VLayoutDetail::RotatePoint(const QPointF &ptCenter, const QPointF& pt, qreal dAng)
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
 * @param list of 4 label vertices
 * @return list of flipped points
 */
QVector<QPointF> VLayoutDetail::Mirror(const QVector<QPointF> &points) const
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
qreal VLayoutDetail::GetDistance(const QPointF &pt1, const QPointF &pt2)
{
    const qreal dX = pt1.x() - pt2.x();
    const qreal dY = pt1.y() - pt2.y();

    return qSqrt(dX*dX + dY*dY);
}
