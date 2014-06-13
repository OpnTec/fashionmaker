/************************************************************************
 **
 **  @file   vdatatool.cpp
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

#include "vdatatool.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDataTool constructor.
 * @param data container with variables
 * @param parent parent object
 */
VDataTool::VDataTool(VContainer *data, QObject *parent): QObject(parent), data(*data), _referens(1)
{
    SCASSERT(data != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator.
 * @param tool tool
 * @return tool
 */
VDataTool &VDataTool::operator =(const VDataTool &tool)
{
    data = tool.getData();
    _referens = tool.referens();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief decrementReferens decrement referens.
 */
void VDataTool::decrementReferens()
{
    if (_referens > 0)
    {
        --_referens;
    }
}
