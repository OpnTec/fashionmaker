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

/**
 * @brief The VToolRecord class
 */
class VToolRecord
{
public:
                       /**
                        * @brief VToolRecord
                        */
                       VToolRecord();
                       /**
                        * @brief VToolRecord
                        * @param id
                        * @param typeTool
                        * @param nameDraw
                        */
                       VToolRecord(const qint64 &id, const Tool::Tools &typeTool, const QString &nameDraw);
    /**
     * @brief getId
     * @return
     */
    inline qint64      getId() const {return id;}
    /**
     * @brief setId
     * @param value
     */
    inline void        setId(const qint64 &value) {id = value;}
    /**
     * @brief getTypeTool
     * @return
     */
    inline Tool::Tools getTypeTool() const {return typeTool;}
    /**
     * @brief setTypeTool
     * @param value
     */
    inline void        setTypeTool(const Tool::Tools &value) {typeTool = value;}
    /**
     * @brief getNameDraw
     * @return
     */
    inline QString     getNameDraw() const {return nameDraw;}
    /**
     * @brief setNameDraw
     * @param value
     */
    inline void        setNameDraw(const QString &value) {nameDraw = value;}
private:
    /**
     * @brief id
     */
    qint64             id;
    /**
     * @brief typeTool
     */
    Tool::Tools        typeTool;
    /**
     * @brief nameDraw
     */
    QString            nameDraw;
};

#endif // VTOOLRECORD_H
