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

/**
 * @brief The VPointF class keep data of point.
 */
class VPointF
{
public:
    /**
     * @brief VPointF creat empty point
     */
    inline             VPointF ()
                       :_name(QString()), _mx(0), _my(0), _x(0), _y(0), idObject(0){}
    /**
     * @brief VPointF copy constructor
     * @param point
     */
    inline             VPointF (const VPointF &point )
                       :_name(point.name()), _mx(point.mx()), _my(point.my()), _x(point.x()), _y(point.y()),
                       idObject(point.getIdObject()){}
    /**
     * @brief VPointF create new point
     * @param x x coordinate
     * @param y y coordinate
     * @param name name of point
     * @param mx offset name respect to x
     * @param my offset name respect to y
     * @param idObject point modeling keep here id of parent point
     */
    inline             VPointF ( qreal x, qreal y, QString name, qreal mx, qreal my, qint64 idObject = 0)
                       :_name(name), _mx(mx), _my(my), _x(x), _y(y), idObject(idObject){}
    /**
     * @brief operator = assignment operator
     * @param point point
     * @return point
     */
    VPointF            &operator=(const VPointF &point);
                       ~VPointF(){}
    /**
     * @brief name return name of point
     * @return name
     */
    inline QString     name() const { return _name;}
    /**
     * @brief mx return offset name respect to x
     * @return offset
     */
    inline qreal       mx() const {return _mx;}
    /**
     * @brief my return offset name respect to y
     * @return offset
     */
    inline qreal       my() const {return _my;}
    /**
     * @brief setName set name of point
     * @param name name
     */
    inline void        setName(const QString &name) {_name = name;}
    /**
     * @brief setMx set offset name respect to x
     * @param mx offset
     */
    inline void        setMx(qreal mx) {_mx = mx;}
    /**
     * @brief setMy set offset name respect to y
     * @param my offset
     */
    inline void        setMy(qreal my) {_my = my;}
    /**
     * @brief toQPointF convert to QPointF
     * @return QPointF point
     */
    inline QPointF     toQPointF()const {return QPointF(_x, _y);}
    /**
     * @brief x return x coordinate
     * @return value
     */
    inline qreal       x() const {return _x;}
    /**
     * @brief setX set x coordinate
     * @param value x coordinate
     */
    inline void        setX(const qreal &value){_x = value;}
    /**
     * @brief y return y coordinate
     * @return value
     */
    inline qreal       y() const {return _y;}
    /**
     * @brief setY set y coordinate
     * @param value y coordinate
     */
    inline void        setY(const qreal &value){_y = value;}
    /**
     * @brief getIdObject return id of parrent.
     * @return id
     */
    inline qint64      getIdObject() const {return idObject;}
    /**
     * @brief setIdObject set id of parent
     * @param value id
     */
    inline void        setIdObject(const qint64 &value) {idObject = value;}
private:
    /**
     * @brief _name name of point
     */
    QString            _name;
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
    /**
     * @brief idObject id of parent. Only for point modeling. All another return 0.
     */
    qint64             idObject;
};

#endif // VPOINTF_H
