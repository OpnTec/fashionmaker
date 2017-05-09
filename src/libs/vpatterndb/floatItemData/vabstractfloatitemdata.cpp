/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 2, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "vabstractfloatitemdata.h"
#include "vabstractfloatitemdata_p.h"

//---------------------------------------------------------------------------------------------------------------------
VAbstractFloatItemData::VAbstractFloatItemData()
    : d(new VAbstractFloatItemDataPrivate())
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractFloatItemData::VAbstractFloatItemData(const VAbstractFloatItemData &data)
    : d (data.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractFloatItemData &VAbstractFloatItemData::operator=(const VAbstractFloatItemData &data)
{
    if ( &data == this )
    {
        return *this;
    }
    d = data.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractFloatItemData::~VAbstractFloatItemData()
{}

//---------------------------------------------------------------------------------------------------------------------
QPointF VAbstractFloatItemData::GetPos() const
{
    return d->m_ptPos;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractFloatItemData::SetPos(const QPointF &ptPos)
{
    d->m_ptPos = ptPos;
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractFloatItemData::IsVisible() const
{
    return d->m_bVisible;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractFloatItemData::SetVisible(bool bVisible)
{
    d->m_bVisible = bVisible;
}
