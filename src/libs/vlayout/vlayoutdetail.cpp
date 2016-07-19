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
#include "vlayoutdetail_p.h"

#include <QGraphicsItem>
#include <QPainterPath>
#include <QFont>
#include <QFontMetrics>
#include <QApplication>
#include <QBrush>

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

#include <QDebug>

//---------------------------------------------------------------------------------------------------------------------
VLayoutDetail::VLayoutDetail()
    :VAbstractDetail(), d(new VLayoutDetailData), m_tmDetail(), m_liPP()
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutDetail::VLayoutDetail(const VLayoutDetail &detail)
    :VAbstractDetail(detail), d(detail.d), m_tmDetail(detail.m_tmDetail), m_liPP(detail.m_liPP)
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
    m_tmDetail = detail.m_tmDetail;
    m_liPP = detail.m_liPP;
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
void VLayoutDetail::SetDetail(const QString& qsName, const VPatternPieceData& data)
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
        v[i] = RotatePoint(ptCenter, v[i], dAng);
    d->detailLabel = RoundPoints(v);

    // generate text
    QFont fnt = QApplication::font();
    m_tmDetail.SetFont(fnt);
    m_tmDetail.SetFontSize(data.GetFontSize());
    m_tmDetail.Clear();
    TextLine tl;
    // letter
    tl.m_qsText = data.GetLetter();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eAlign = Qt::AlignCenter;
        tl.m_eFontWeight = QFont::Bold;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 6;
        m_tmDetail.AddLine(tl);
    }
    // name
    if (qsName.isEmpty() == false)
    {
        tl.m_qsText = qsName;
        tl.m_eFontWeight = QFont::DemiBold;
        tl.m_iFontSize = 2;
        m_tmDetail.AddLine(tl);
    }

    // MCP
    QString qsText = "Cut %1 of %2%3";
    QStringList qslPlace;
    qslPlace << "" << " on Fold";
    tl.m_eAlign = Qt::AlignLeft | Qt::AlignVCenter;
    tl.m_eFontWeight = QFont::Normal;
    tl.m_iFontSize = 0;
    for (int i = 0; i < data.GetMCPCount(); ++i)
    {
        MaterialCutPlacement mcp = data.GetMCP(i);
        if (mcp.m_iCutNumber > 0)
        {
            tl.m_qsText = qsText.arg(mcp.m_iCutNumber).arg(mcp.m_qsMaterialUserDef).arg(qslPlace[int(mcp.m_ePlacement)]);
            m_tmDetail.AddLine(tl);
        }
    }
    // will generate the lines of text
    m_tmDetail.IsBigEnough(data.GetLabelWidth(), data.GetLabelHeight(), data.GetFontSize());
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutDetail::GetPatternInfoPoints() const
{
    return d->patternInfo;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetPatternInfoPoints(const QPointF& ptPos, qreal dWidth, qreal dHeight, qreal dRot)
{
    qreal dAng = qDegreesToRadians(dRot);
    QPointF ptCenter(ptPos.x() + dWidth/2, ptPos.y() + dHeight/2);
    QVector<QPointF> v;
    v << ptPos << QPointF(ptPos.x() + dWidth, ptPos.y()) << QPointF(ptPos.x() + dWidth, ptPos.y() + dHeight)
         << QPointF(ptPos.x(), ptPos.y() + dHeight);
    for (int i = 0; i < v.count(); ++i)
        v[i] = RotatePoint(ptCenter, v[i], dAng);
    d->patternInfo = RoundPoints(v);
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

    if (d->detailLabel.count() > 0)
    {
        points = Map(Mirror(d->detailLabel));
        points.push_back(points.at(0));

        QPainterPath pathDet;
        pathDet.moveTo(points.at(0));
        for (qint32 i = 1; i < points.count(); ++i)
        {
            pathDet.lineTo(points.at(i));
        }

        path.addPath(pathDet);
    }

    if (d->patternInfo.count() > 0)
    {
        points = Map(Mirror(d->patternInfo));
        points.push_back(points.at(0));

        QPainterPath pathDet;
        pathDet.moveTo(points.at(0));
        for (qint32 i = 1; i < points.count(); ++i)
        {
            pathDet.lineTo(points.at(i));
        }

        path.addPath(pathDet);
    }

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::ClearTextItems()
{
    m_liPP.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::CreateTextItems()
{
    ClearTextItems();
    if (d->detailLabel.count() > 0)
    {
        QVector<QPointF> points = Map(Mirror(d->detailLabel));
        points.push_back(points.at(0));
        qreal dAng = qAtan2(points.at(1).y() - points.at(0).y(), points.at(1).x() - points.at(0).x());
        qreal dW = GetDistance(points.at(0), points.at(1));
        qreal dY = 0;
        qreal dX;
        for (int i = 0; i < m_tmDetail.GetCount(); ++i)
        {
            const TextLine& tl = m_tmDetail.GetLine(i);
            QFont fnt = m_tmDetail.GetFont();
            fnt.setPixelSize(m_tmDetail.GetFont().pixelSize() + tl.m_iFontSize);
            fnt.setWeight(tl.m_eFontWeight);
            fnt.setStyle(tl.m_eStyle);
            dY += tl.m_iHeight;
            QMatrix mat;
            mat.translate(points.at(0).x(), points.at(0).y());
            mat.rotate(qRadiansToDegrees(dAng));

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
            m_liPP << mat.map(path);
            dY += m_tmDetail.GetSpacing();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutDetail::GetTextItemsCount() const
{
    return m_liPP.count();
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsItem* VLayoutDetail::GetTextItem(int i) const
{
    QGraphicsPathItem* item = new QGraphicsPathItem();
    QTransform transform = d->matrix;

    QPainterPath path = transform.map(m_liPP[i]);

    if (d->mirror == true)
    {
        QVector<QPointF> points = Map(Mirror(d->detailLabel));
        QPointF ptCenter = (points.at(1) + points.at(3))/2;
        qreal dRot = qRadiansToDegrees(qAtan2(points.at(1).y() - points.at(0).y(), points.at(1).x() - points.at(0).x()));

        // we need to move the center back to the origin, rotate it to align it with x axis,
        // then mirror it to obtain the proper text direction, rotate it and translate it back to original position
        QTransform t;
        t.translate(ptCenter.x(), ptCenter.y());
        t.rotate(dRot);
        t.scale(-1, 1);
        t.rotate(-dRot);
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
QPointF VLayoutDetail::RotatePoint(const QPointF &ptCenter, const QPointF& pt, qreal dAng) const
{
    QPointF ptDest;
    QPointF ptRel = pt - ptCenter;
    ptDest.setX(cos(dAng)*ptRel.x() - sin(dAng)*ptRel.y());
    ptDest.setY(sin(dAng)*ptRel.x() + cos(dAng)*ptRel.y());

    return ptDest + ptCenter;
}

//---------------------------------------------------------------------------------------------------------------------
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
qreal VLayoutDetail::GetDistance(const QPointF &pt1, const QPointF &pt2) const
{
    qreal dX = pt1.x() - pt2.x();
    qreal dY = pt1.y() - pt2.y();

    return qSqrt(dX*dX + dY*dY);
}
