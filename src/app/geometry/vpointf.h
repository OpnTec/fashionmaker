/************************************************************************
 **
 **  @file   vpointf.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "vgobject.h"

class QPointF;
class QString;

/**
 * @brief The VPointF class keep data of point.
 */
class VPointF:public VGObject
{
public:
    VPointF ();
    VPointF (const VPointF &point );
    VPointF (const QPointF &point );
    VPointF ( qreal x, qreal y, QString name, qreal mx, qreal my, quint32 idObject = 0,
              Draw mode = Draw::Calculation);
    virtual ~VPointF(){}
    VPointF &operator=(const VPointF &point);
    qreal       mx() const;
    qreal       my() const;
    void        setMx(qreal mx);
    void        setMy(qreal my);
    QPointF     toQPointF()const;
    qreal       x() const;
    void        setX(const qreal &value);
    qreal       y() const;
    void        setY(const qreal &value);
    virtual QString name() const;
private:
    /** @brief _mx offset name respect to x */
    qreal              _mx;

    /** @brief _my offset name respect to y */
    qreal              _my;

    /** @brief _x x coordinate */
    qreal              _x;

    /** @brief _y y coordinate */
    qreal              _y;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mx return offset name respect to x
 * @return offset
 */
inline qreal VPointF::mx() const
{
    return _mx;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief my return offset name respect to y
 * @return offset
 */
inline qreal VPointF::my() const
{
    return _my;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMx set offset name respect to x
 * @param mx offset
 */
inline void VPointF::setMx(qreal mx)
{
    _mx = mx;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMy set offset name respect to y
 * @param my offset
 */
inline void VPointF::setMy(qreal my)
{
    _my = my;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief x return x coordinate
 * @return value
 */
inline qreal VPointF::x() const
{
    return _x;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setX set x coordinate
 * @param value x coordinate
 */
inline void VPointF::setX(const qreal &value)
{
    _x = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief y return y coordinate
 * @return value
 */
inline qreal VPointF::y() const
{
    return _y;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setY set y coordinate
 * @param value y coordinate
 */
inline void VPointF::setY(const qreal &value)
{
    _y = value;
}

#endif // VPOINTF_H
