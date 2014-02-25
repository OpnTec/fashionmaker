/************************************************************************
 **
 **  @file   vpointf.cpp
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

#include "vpointf.h"

VPointF::VPointF(qreal x, qreal y, QString name, qreal mx, qreal my, quint32 idObject, Draw::Draws mode)
    :VGObject(GObject::Point, idObject, mode), _mx(mx), _my(my), _x(x), _y(y)
{
    this->_name = name;
}

VPointF &VPointF::operator =(const VPointF &point)
{
    VGObject::operator=(point);
    _mx = point.mx();
    _my = point.my();
    _x = point.x();
    _y = point.y();
    return *this;
}
