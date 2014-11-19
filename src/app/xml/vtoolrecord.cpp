/************************************************************************
 **
 **  @file   vtoolrecord.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "vtoolrecord.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolRecord default constructor.
 */
VToolRecord::VToolRecord()
    :id(NULL_ID), typeTool(Tool::Arrow), nameDraw(QString())
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolRecord constructor.
 * @param id tool id.
 * @param typeTool tool type.
 * @param nameDraw pattern peace name.
 */
VToolRecord::VToolRecord(const quint32 &id, const Tool &typeTool, const QString &nameDraw)
    :id(id), typeTool(typeTool), nameDraw(nameDraw)
{}

//---------------------------------------------------------------------------------------------------------------------
bool VToolRecord::operator==(const VToolRecord &record) const
{
    bool isEqual = false;
    if (id == record.getId() && typeTool == record.getTypeTool() && nameDraw == record.getNameDraw())
    {
        isEqual = true;
    }
    return isEqual;
}

//---------------------------------------------------------------------------------------------------------------------
VToolRecord &VToolRecord::operator=(const VToolRecord &record)
{
    this->id = record.getId();
    this->typeTool = record.getTypeTool();
    this->nameDraw = record.getNameDraw();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VToolRecord::VToolRecord(const VToolRecord &record)
    :id(record.getId()), typeTool(record.getTypeTool()), nameDraw(record.getNameDraw())
{}

//---------------------------------------------------------------------------------------------------------------------
VToolRecord::~VToolRecord()
{}
