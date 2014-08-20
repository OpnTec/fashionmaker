/************************************************************************
 **
 **  @file   vgobject.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
#include "vgobject_p.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGObject default constructor.
 */
VGObject::VGObject()
    :d(new VGObjectData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGObject constructor.
 * @param type type graphical object.
 * @param idObject id parent object.
 * @param mode mode creation. Used in modeling mode.
 */
VGObject::VGObject(const GOType &type, const quint32 &idObject, const Draw &mode)
    :d(new VGObjectData(type, idObject, mode))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VGObject copy constructor.
 * @param obj object.
 */
VGObject::VGObject(const VGObject &obj)
    :d (obj.d)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator.
 * @param obj object
 * @return object
 */
VGObject &VGObject::operator=(const VGObject &obj)
{
    if ( &obj == this )
    {
        return *this;
    }
    d = obj.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VGObject::~VGObject()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getIdObject return parent id.
 * @return parent id or 0 if object don't have parent.
 */
quint32 VGObject::getIdObject() const
{
    return d->idObject;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setIdObject set parent id.
 * @param value parent id.
 */
void VGObject::setIdObject(const quint32 &value)
{
    d->idObject = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief name return name graphical object.
 * @return name
 */
QString VGObject::name() const
{
    return d->_name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setName set name graphical object.
 * @param name name graphical object.
 */
void VGObject::setName(const QString &name)
{
    d->_name = name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getMode return mode creation.
 * @return mode.
 */
Draw VGObject::getMode() const
{
    return d->mode;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMode set mode creation.
 * @param value mode.
 */
void VGObject::setMode(const Draw &value)
{
    d->mode = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getType return object type.
 * @return type.
 */
GOType VGObject::getType() const
{
    return d->type;
}

//---------------------------------------------------------------------------------------------------------------------
void VGObject::setType(const GOType &type)
{
    d->type = type;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief id return id object.
 * @return id
 */
quint32 VGObject::id() const
{
    return d->_id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setId set id object.
 * @param id id.
 */
void VGObject::setId(const quint32 &id)
{
    d->_id = id;
}
