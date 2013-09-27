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

#include "vtoolrecord.h"

VToolRecord::VToolRecord():id(0), typeTool(Tool::ArrowTool), nameDraw(QString()){
}

VToolRecord::VToolRecord(const qint64 &id, const Tool::Tools &typeTool, const QString &nameDraw):id(id),
    typeTool(typeTool), nameDraw(nameDraw){
}

QString VToolRecord::getNameDraw() const{
    return nameDraw;
}

void VToolRecord::setNameDraw(const QString &value){
    nameDraw = value;
}

Tool::Tools VToolRecord::getTypeTool() const{
    return typeTool;
}

void VToolRecord::setTypeTool(const Tool::Tools &value){
    typeTool = value;
}

qint64 VToolRecord::getId() const{
    return id;
}

void VToolRecord::setId(const qint64 &value){
    id = value;
}
