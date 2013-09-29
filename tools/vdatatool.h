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

#include "container/vcontainer.h"

class VDataTool : public QObject{
    Q_OBJECT
public:
    explicit VDataTool(VContainer *data, QObject *parent = 0);
    virtual ~VDataTool();
    VDataTool& operator= (const VDataTool &tool);
    VContainer getData() const { return data; }
    void setData(const VContainer *value);
protected:
    VContainer   data;
};

#endif // VDATATOOL_H
