/************************************************************************
 **
 **  @file   vtoolrecord.h
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

#ifndef VTOOLRECORD_H
#define VTOOLRECORD_H

#include "../options.h"

#include <QString>

/**
 * @brief The VToolRecord class record about tool in history.
 */
class VToolRecord
{
public:
                       /**
                        * @brief VToolRecord default constructor.
                        */
                       VToolRecord();
                       /**
                        * @brief VToolRecord constructor.
                        * @param id tool id.
                        * @param typeTool tool type.
                        * @param nameDraw pattern peace name.
                        */
                       VToolRecord(const quint32 &id, const Tool::Tools &typeTool, const QString &nameDraw);
    /**
     * @brief getId return tool id.
     * @return id.
     */
    inline quint32      getId() const {return id;}
    /**
     * @brief setId set tool id.
     * @param value id.
     */
    inline void        setId(const quint32 &value) {id = value;}
    /**
     * @brief getTypeTool return tool type.
     * @return tool type.
     */
    inline Tool::Tools getTypeTool() const {return typeTool;}
    /**
     * @brief setTypeTool set tool type.
     * @param value tool type.
     */
    inline void        setTypeTool(const Tool::Tools &value) {typeTool = value;}
    /**
     * @brief getNameDraw return pattern peace name.
     * @return pattern peace name.
     */
    inline QString     getNameDraw() const {return nameDraw;}
    /**
     * @brief setNameDraw set pattern peace name.
     * @param value pattern peace name.
     */
    inline void        setNameDraw(const QString &value) {nameDraw = value;}
private:
    /**
     * @brief id tool id.
     */
    quint32             id;
    /**
     * @brief typeTool tool type.
     */
    Tool::Tools        typeTool;
    /**
     * @brief nameDraw pattern peace name.
     */
    QString            nameDraw;
};

#endif // VTOOLRECORD_H
