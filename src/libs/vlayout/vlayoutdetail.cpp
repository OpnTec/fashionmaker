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
#include <QtMath>

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
QVector<QPointF> VLayoutDetail::GetContourPoints() const
{
    return d->contour;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetCountourPoints(const QVector<QPointF> &points)
{
    d->contour = points;
    // Contour can't be closed
    if (d->contour.first() == d->contour.last())
    {
        d->contour.removeLast();
    }

    d->contour = RemoveDublicates(RoundPoints(d->contour));
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutDetail::GetSeamAllowencePoints() const
{
    return d->seamAllowence;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutDetail::SetSeamAllowencePoints(const QVector<QPointF> &points)
{
    d->seamAllowence = points;
    // Seam allowence can't be closed
    if (d->seamAllowence.first() == d->seamAllowence.last())
    {
        d->seamAllowence.removeLast();
    }

    d->seamAllowence = RemoveDublicates(RoundPoints(d->seamAllowence));
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
    QTransform m;
    m.translate(edge.p2().x(), edge.p2().y());
    m.rotate(-angle);
    m.translate(-edge.p2().x(), -edge.p2().y());
    d->matrix *= m;

    m.reset();
    m.translate(edge.p2().x(), edge.p2().y());
    m.scale(m.m11(), m.m22()*-1);
    m.translate(-edge.p2().x(), -edge.p2().y());
    d->matrix *= m;

    m.reset();
    m.translate(edge.p2().x(), edge.p2().y());
    m.rotate(-(360-angle));
    m.translate(-edge.p2().x(), -edge.p2().y());
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
        i1 = (d->layoutAllowence.size()-1) - i2;
        i2 = (d->layoutAllowence.size()-1) - oldI1;
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
    if (d->layoutAllowence.isEmpty())
    {
        return 0;
    }

    const int n = d->layoutAllowence.count();
    qreal s, res = 0;
    qint64 sq = 0;

    QVector<qreal> x;
    QVector<qreal> y;

    for (int i=0; i < n; ++i)
    {
        x.append(d->layoutAllowence.at(i).x());
        y.append(d->layoutAllowence.at(i).y());
    }

    // Calculation a polygon area through the sum of the areas of trapezoids
    for (int i = 0; i < n; ++i)
    {
        if (i == 0)
        {
            s = x.at(i)*(y.at(n-1) - y.at(i+1)); //if i == 0, then y[i-1] replace on y[n-1]
            res += s;
        }
        else
        {
            if (i == n-1)
            {
                s = x.at(i)*(y.at(i-1) - y.at(0)); // if i == n-1, then y[i+1] replace on y[0]
                res += s;
            }
            else
            {
                s = x.at(i)*(y.at(i-1) - y.at(i+1));
                res += s;
            }
        }
    }
    sq = qFloor(qAbs(res/2.0));
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
                d->layoutAllowence.removeLast();
            }
        }
        else
        {
            d->layoutAllowence = Equidistant(d->contour, EquidistantType::CloseEquidistant, d->layoutWidth);
            if (d->layoutAllowence.isEmpty() == false)
            {
                d->layoutAllowence.removeLast();
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
QVector<QPointF> VLayoutDetail::RoundPoints(const QVector<QPointF> &points) const
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
