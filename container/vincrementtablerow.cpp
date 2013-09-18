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

#include "vincrementtablerow.h"

VIncrementTableRow::VIncrementTableRow():id(0), base(0), ksize(0), kgrowth(0), description(QString()){
}

VIncrementTableRow::VIncrementTableRow(qint64 id, qreal base, qreal ksize, qreal kgrowth,
                                         QString description):id(id), base(base), ksize(ksize),
    kgrowth(kgrowth), description(description){
}

QString VIncrementTableRow::getDescription() const{
    return description;
}

void VIncrementTableRow::setDescription(const QString &value){
    description = value;
}

qreal VIncrementTableRow::getKgrowth() const{
    return kgrowth;
}

void VIncrementTableRow::setKgrowth(const qreal &value){
    kgrowth = value;
}

qreal VIncrementTableRow::getKsize() const{
    return ksize;
}

void VIncrementTableRow::setKsize(const qreal &value){
    ksize = value;
}

qreal VIncrementTableRow::getBase() const{
    return base;
}

void VIncrementTableRow::setBase(const qreal &value){
    base = value;
}

qint64 VIncrementTableRow::getId() const{
    return id;
}

void VIncrementTableRow::setId(const qint64 &value){
    id = value;
}
