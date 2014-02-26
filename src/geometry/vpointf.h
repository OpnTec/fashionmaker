/************************************************************************
 **
 **  @file   vpointf.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VPOINTF_H
#define VPOINTF_H

#include <QPointF>
#include <QString>
#include "../options.h"
#include "vgobject.h"

/**
 * @brief The VPointF class keep data of point.
 */
class VPointF:public VGObject
{
public:
    /**
     * @brief VPointF creat empty point
     */
    VPointF ();
    /**
     * @brief VPointF copy constructor
     * @param point
     */
    VPointF (const VPointF &point );
    VPointF (const QPointF &point );
    /**
     * @brief VPointF create new point
     * @param x x coordinate
     * @param y y coordinate
     * @param mx offset name respect to x
     * @param my offset name respect to y
     */
     VPointF ( qreal x, qreal y, QString name, qreal mx, qreal my, quint32 idObject = 0,
               Draw::Draws mode = Draw::Calculation);
     ~VPointF(){}
    /**
     * @brief operator = assignment operator
     * @param point point
     * @return point
     */
    VPointF &operator=(const VPointF &point);

    /**
     * @brief mx return offset name respect to x
     * @return offset
     */
    qreal       mx() const;
    /**
     * @brief my return offset name respect to y
     * @return offset
     */
    qreal       my() const;
    /**
     * @brief setMx set offset name respect to x
     * @param mx offset
     */
    void        setMx(qreal mx);
    /**
     * @brief setMy set offset name respect to y
     * @param my offset
     */
    void        setMy(qreal my);
    /**
     * @brief toQPointF convert to QPointF
     * @return QPointF point
     */
    QPointF     toQPointF()const;
    /**
     * @brief x return x coordinate
     * @return value
     */
    qreal       x() const;
    /**
     * @brief setX set x coordinate
     * @param value x coordinate
     */
    void        setX(const qreal &value);
    /**
     * @brief y return y coordinate
     * @return value
     */
    qreal       y() const;
    /**
     * @brief setY set y coordinate
     * @param value y coordinate
     */
    void        setY(const qreal &value);
    virtual QString name() const;
private:
    /**
     * @brief _mx offset name respect to x
     */
    qreal              _mx;
    /**
     * @brief _my offset name respect to y
     */
    qreal              _my;
    /**
     * @brief _x x coordinate
     */
    qreal              _x;
    /**
     * @brief _y y coordinate
     */
    qreal              _y;
};

inline qreal VPointF::mx() const
{
    return _mx;
}

inline qreal VPointF::my() const
{
    return _my;
}

inline void VPointF::setMx(qreal mx)
{
    _mx = mx;
}

inline void VPointF::setMy(qreal my)
{
    _my = my;
}

inline QPointF VPointF::toQPointF() const
{
    return QPointF(_x, _y);
}

inline qreal VPointF::x() const
{
    return _x;
}

inline void VPointF::setX(const qreal &value)
{
    _x = value;
}

inline qreal VPointF::y() const
{
    return _y;
}

inline void VPointF::setY(const qreal &value)
{
    _y = value;
}

inline QString VPointF::name() const
{
    return _name;
}

#endif // VPOINTF_H
