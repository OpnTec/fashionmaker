/************************************************************************
 **
 **  @file   vtoolrecord.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VTOOLRECORD_H
#define VTOOLRECORD_H

#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"

/**
 * @brief The VToolRecord class record about tool in history.
 */
class VToolRecord
{
public:
    VToolRecord();
    VToolRecord(const quint32 &id, const Tool &typeTool, const QString &nameDraw);
    bool operator==(const VToolRecord &record) const;
    VToolRecord &operator=(const VToolRecord &record);
    VToolRecord(const VToolRecord &record);
    quint32 getId() const;
    void    setId(const quint32 &value);
    Tool    getTypeTool() const;
    void    setTypeTool(const Tool &value);
    QString getNameDraw() const;
    void    setNameDraw(const QString &value);
    ~VToolRecord() Q_DECL_EQ_DEFAULT;
private:
    /** @brief id tool id. */
    quint32 id;

    /** @brief typeTool tool type. */
    Tool    typeTool;

    /** @brief nameDraw pattern peace name. */
    QString nameDraw;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getId return tool id.
 * @return id.
 */
inline quint32 VToolRecord::getId() const
{
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setId set tool id.
 * @param value id.
 */
inline void VToolRecord::setId(const quint32 &value)
{
    id = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTypeTool return tool type.
 * @return tool type.
 */
inline Tool VToolRecord::getTypeTool() const
{
    return typeTool;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setTypeTool set tool type.
 * @param value tool type.
 */
inline void VToolRecord::setTypeTool(const Tool &value)
{
    typeTool = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getNameDraw return pattern peace name.
 * @return pattern peace name.
 */
inline QString VToolRecord::getNameDraw() const
{
    return nameDraw;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setNameDraw set pattern peace name.
 * @param value pattern peace name.
 */
inline void VToolRecord::setNameDraw(const QString &value)
{
    nameDraw = value;
}

//---------------------------------------------------------------------------------------------------------------------
inline bool VToolRecord::operator==(const VToolRecord &record) const
{
    // Id should be enough
    return id == record.getId()/* && typeTool == record.getTypeTool() && nameDraw == record.getNameDraw()*/;
}

#endif // VTOOLRECORD_H
