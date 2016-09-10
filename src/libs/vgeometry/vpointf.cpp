/************************************************************************
 **
 **  @file   vpointf.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "vpointf.h"
#include "vpointf_p.h"
#include <QLineF>
#include <QPointF>
#include <QString>
#include <QTransform>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VPointF creat empty point
 */
VPointF::VPointF()
    :VGObject(GOType::Point, 0, Draw::Calculation), d(new VPointFData)
{}

//---------------------------------------------------------------------------------------------------------------------
VPointF::VPointF(const VPointF &point)
    :VGObject(point), d(point.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPointF::VPointF(const QPointF &point)
    :VGObject(VPointF()), d(new VPointFData(point))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VPointF create new point
 * @param x x coordinate
 * @param y y coordinate
 * @param name point label
 * @param mx offset name respect to x
 * @param my offset name respect to y
 */
VPointF::VPointF(qreal x, qreal y, const QString &name, qreal mx, qreal my, quint32 idObject, const Draw &mode)
    :VGObject(GOType::Point, idObject, mode), d(new VPointFData(x, y, mx, my))
{
    setName(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VPointF create new point
 * @param point point
 * @param name point label
 * @param mx offset name respect to x
 * @param my offset name respect to y
 */
VPointF::VPointF(const QPointF &point, const QString &name, qreal mx, qreal my, quint32 idObject, const Draw &mode)
    :VGObject(GOType::Point, idObject, mode), d(new VPointFData(point, mx, my))
{
    setName(name);
}

//---------------------------------------------------------------------------------------------------------------------
VPointF::~VPointF()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator
 * @param point point
 * @return point
 */
VPointF &VPointF::operator =(const VPointF &point)
{
    if ( &point == this )
    {
        return *this;
    }
    VGObject::operator=(point);
    d = point.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF::operator QPointF() const
{
    return toQPointF();
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VPointF::Rotate(const QPointF &originPoint, qreal degrees, const QString &prefix) const
{
    const QPointF p = RotatePF(originPoint, toQPointF(), degrees);
    return VPointF(p, name() + prefix, mx(), my());
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VPointF::Flip(const QLineF &axis, const QString &prefix) const
{
    const QTransform matrix = FlippingMatrix(axis);
    const QPointF p = matrix.map(toQPointF());
    return VPointF(p, name() + prefix, mx(), my());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mx return offset name respect to x
 * @return offset
 */
qreal VPointF::mx() const
{
    return d->_mx;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief my return offset name respect to y
 * @return offset
 */
qreal VPointF::my() const
{
    return d->_my;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMx set offset name respect to x
 * @param mx offset
 */
void VPointF::setMx(qreal mx)
{
    d->_mx = mx;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMy set offset name respect to y
 * @param my offset
 */
void VPointF::setMy(qreal my)
{
    d->_my = my;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VPointF::toQPointF() const
{
    return QPointF(d->_x, d->_y);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief x return x coordinate
 * @return value
 */
qreal VPointF::x() const
{
    return d->_x;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setX set x coordinate
 * @param value x coordinate
 */
void VPointF::setX(const qreal &value)
{
    d->_x = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief y return y coordinate
 * @return value
 */
qreal VPointF::y() const
{
    return d->_y;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setY set y coordinate
 * @param value y coordinate
 */
void VPointF::setY(const qreal &value)
{
    d->_y = value;
}

//---------------------------------------------------------------------------------------------------------------------
QPointF VPointF::RotatePF(const QPointF &originPoint, const QPointF &point, qreal degrees)
{
    QLineF axis(originPoint, point);
    axis.setAngle(axis.angle() + degrees);
    return axis.p2();
}
