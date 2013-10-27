/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VDATATOOL_H
#define VDATATOOL_H

#include <container/vcontainer.h>

//We need QObject class because we use qobject_cast.
class VDataTool : public QObject{
    Q_OBJECT
public:
    explicit              VDataTool(VContainer *data, QObject *parent = 0): QObject(parent), data(*data), _referens(1){}
    virtual               ~VDataTool(){}
    VDataTool             &operator= (const VDataTool &tool);
    inline VContainer     getData() const { return data; }
    inline void           setData(const VContainer *value) {data = *value;}
    virtual inline qint64 referens() const {return _referens;}
    virtual inline void   incrementReferens(){++_referens;}
    virtual void          decrementReferens();
protected:
    VContainer            data;
    qint64                _referens;
};

#endif // VDATATOOL_H
