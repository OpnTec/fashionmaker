/************************************************************************
 **
 **  @file   vgobject.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   27 12, 2013
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

#include "vgobject.h"

VGObject::VGObject()
    :_id(0), type(GObject::Point), idObject(0), _name(QString()), mode(Draw::Calculation)
{
}

VGObject::VGObject(const GObject::Type &type, const quint32 &idObject, const Draw::Draws &mode)
    :_id(0), type(type), idObject(idObject), _name(QString()), mode(mode)
{
}

VGObject::VGObject(const VGObject &obj)
    :_id(obj.id()), type(obj.getType()), idObject(obj.getIdObject()), _name(obj.name()), mode(obj.getMode())
{
}

VGObject &VGObject::operator=(const VGObject &obj)
{
    this->_id      = obj.id();
    this->type     = obj.getType();
    this->idObject = obj.getIdObject();
    this->_name    = obj.name();
    this->mode     = obj.getMode();
    return *this;
}

quint32 VGObject::getIdObject() const
{
    return idObject;
}

void VGObject::setIdObject(const quint32 &value)
{
    idObject = value;
}

QString VGObject::name() const
{
    return _name;
}

void VGObject::setName(const QString &name)
{
    _name = name;
}

Draw::Draws VGObject::getMode() const
{
    return mode;
}

void VGObject::setMode(const Draw::Draws &value)
{
    mode = value;
}

GObject::Type VGObject::getType() const
{
    return type;
}

quint32 VGObject::id() const
{
    return _id;
}

void VGObject::setId(const quint32 &id)
{
    _id = id;
}
