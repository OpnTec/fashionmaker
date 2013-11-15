/************************************************************************
 **
 **  @file vpointf.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
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

class VPointF
{
public:
    inline             VPointF ()
                       :_name(QString()), _mx(0), _my(0), _x(0), _y(0), mode(Draw::Calculation), idObject(0){}
    inline             VPointF (const VPointF &point )
                       :_name(point.name()), _mx(point.mx()), _my(point.my()), _x(point.x()), _y(point.y()),
                       mode(point.getMode()), idObject(point.getIdObject()){}
    inline             VPointF ( qreal x, qreal y, QString name, qreal mx, qreal my,
                                 Draw::Draws mode = Draw::Calculation, qint64 idObject = 0)
                       :_name(name), _mx(mx), _my(my), _x(x), _y(y), mode(mode), idObject(idObject){}
    VPointF            &operator=(const VPointF &point);
                       ~VPointF(){}
    inline QString     name() const { return _name;}
    inline qreal       mx() const {return _mx;}
    inline qreal       my() const {return _my;}
    inline void        setName(const QString &name) {_name = name;}
    inline void        setMx(qreal mx) {_mx = mx;}
    inline void        setMy(qreal my) {_my = my;}
    inline QPointF     toQPointF()const {return QPointF(_x, _y);}
    inline qreal       x() const {return _x;}
    inline void        setX(const qreal &value){_x = value;}
    inline qreal       y() const {return _y;}
    inline void        setY(const qreal &value){_y = value;}
    inline Draw::Draws getMode() const{return mode;}
    inline void        setMode(const Draw::Draws &value) {mode = value;}
    inline qint64      getIdObject() const {return idObject;}
    inline void        setIdObject(const qint64 &value) {idObject = value;}
private:
    QString            _name;
    qreal              _mx;
    qreal              _my;
    qreal              _x;
    qreal              _y;
    Draw::Draws        mode;
    qint64             idObject;
};

#endif // VPOINTF_H
