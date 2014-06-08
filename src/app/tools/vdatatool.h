/************************************************************************
 **
 **  @file   vdatatool.h
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

#ifndef VDATATOOL_H
#define VDATATOOL_H

#include "../container/vcontainer.h"

class QObject;

//We need QObject class because we use qobject_cast.
/**
 * @brief The VDataTool class need for getting access to data container of tool.
 */
class VDataTool : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief VDataTool constructor.
     * @param data container with variables
     * @param parent parent object
     */
    VDataTool(VContainer *data, QObject *parent = nullptr);
    virtual ~VDataTool(){}
    /**
     * @brief operator = assignment operator.
     * @param tool tool
     * @return tool
     */
    VDataTool             &operator= (const VDataTool &tool);
    /**
     * @brief getData return data container.
     * @return container.
     */
    VContainer            getData() const;
    /**
     * @brief setData set data container.
     * @param value container.
     */
    void                  setData(const VContainer *value);
    /**
     * @brief referens return count of referens.
     * @return count count of referens.
     */
    virtual quint32       referens() const;
    /**
     * @brief incrementReferens increment referens.
     */
    virtual void          incrementReferens();
    /**
     * @brief decrementReferens decrement referens.
     */
    virtual void          decrementReferens();
protected:
    /**
     * @brief data container with data
     */
    VContainer            data;
    /**
     * @brief _referens keep count tools what use this tool. If value more than 1 you can't delete tool.
     */
    quint32                _referens;
};

inline VContainer VDataTool::getData() const
{
    return data;
}

inline void VDataTool::setData(const VContainer *value)
{
    data = *value;
}

inline quint32 VDataTool::referens() const
{
    return _referens;
}

inline void VDataTool::incrementReferens()
{
    ++_referens;
}

#endif // VDATATOOL_H
