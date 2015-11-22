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

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

#include <QDebug>

//---------------------------------------------------------------------------------------------------------------------
VLayoutDetail::VLayoutDetail()
    :VAbstractDetail(), d(new VLayoutDetailData)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutDetail::VLayoutDetail(const VLayoutDetail &detail)
    :VAbstractDetail(detail), d (detail.d)
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
QRectF VLayoutDetail::BoundingRect() const
{
    QVector<QPointF> points = GetLayoutAllowencePoints();
    points.append(points.first());
    QRectF rec =  QPolygonF(points).boundingRect();
    return rec;
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
