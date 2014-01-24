/************************************************************************
 **
 **  @file   vdatatool.h
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

#ifndef VDATATOOL_H
#define VDATATOOL_H

#include "../container/vcontainer.h"

//We need QObject class because we use qobject_cast.
/**
 * @brief The VDataTool class
 */
class VDataTool : public QObject
{
    Q_OBJECT
public:
                          /**
                           * @brief VDataTool
                           * @param data container with variables
                           * @param parent parent object
                           */
                          VDataTool(VContainer *data, QObject *parent = 0);
    virtual               ~VDataTool(){}
    /**
     * @brief operator =
     * @param tool
     * @return
     */
    VDataTool             &operator= (const VDataTool &tool);
    /**
     * @brief getData
     * @return
     */
    inline VContainer     getData() const { return data; }
    /**
     * @brief setData
     * @param value
     */
    inline void           setData(const VContainer *value){data = *value;}
    /**
     * @brief referens
     * @return
     */
    virtual inline qint64 referens() const {return _referens;}
    /**
     * @brief incrementReferens
     */
    virtual inline void   incrementReferens(){++_referens;}
    /**
     * @brief decrementReferens
     */
    virtual void          decrementReferens();
protected:
    /**
     * @brief data container with data
     */
    VContainer            data;
    /**
     * @brief _referens
     */
    qint64                _referens;
};

#endif // VDATATOOL_H
