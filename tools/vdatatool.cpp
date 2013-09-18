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

#include "vdatatool.h"

VDataTool::VDataTool(VContainer *data, QObject *parent) :
    QObject(parent), data(*data){
}

VDataTool::~VDataTool(){
}

VDataTool &VDataTool::operator =(const VDataTool &tool){
   data = tool.getData();
   return *this;
}

VContainer VDataTool::getData() const{
    return data;
}

void VDataTool::setData(const VContainer *value){
    data = *value;
}
