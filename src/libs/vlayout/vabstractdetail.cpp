/************************************************************************
 **
 **  @file   vabstractdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vabstractdetail.h"
#include "vabstractdetail_p.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractDetail default contructor. Create empty detail.
 */
VAbstractDetail::VAbstractDetail()
    :d(new VAbstractDetailData)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractDetail constructor.
 * @param name detail name.
 */
VAbstractDetail::VAbstractDetail(const QString &name)
    :d(new VAbstractDetailData(name))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractDetail copy constructor.
 * @param detail detail.
 */
VAbstractDetail::VAbstractDetail(const VAbstractDetail &detail)
    :d (detail.d)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator.
 * @param detail detail.
 * @return new detail.
 */
VAbstractDetail &VAbstractDetail::operator=(const VAbstractDetail &detail)
{
    if ( &detail == this )
    {
        return *this;
    }
    d = detail.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractDetail::~VAbstractDetail()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear detail full clear.
 */
void VAbstractDetail::Clear()
{
    d->name.clear();
    d->seamAllowance = false;
    d->closed = true;
    d->width = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getName return detail name.
 * @return name.
 */
QString VAbstractDetail::getName() const
{
    return d->name;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setName set detail name.
 * @param value new name.
 */
void VAbstractDetail::setName(const QString &value)
{
    d->name = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSeamAllowance keep status for seam allowance detail.
 * @return true - need seam allowance, false - no need seam allowance.
 */
bool VAbstractDetail::getSeamAllowance() const
{
    return d->seamAllowance;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSeamAllowance set status for seam allowance detail.
 * @param value true - need seam allowance, false - no need seam allowance.
 */
void VAbstractDetail::setSeamAllowance(bool value)
{
    d->seamAllowance = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getClosed keep close status for detail equdistant.
 * @return true - close equdistant, false - don't close equdistant.
 */
bool VAbstractDetail::getClosed() const
{
    return d->closed;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setClosed set close status for detail equdistant.
 * @param value true - close equdistant, false - don't close equdistant.
 */
void VAbstractDetail::setClosed(bool value)
{
    d->closed = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getWidth return value detail seam allowance.
 * @return value in mm.
 */
qreal VAbstractDetail::getWidth() const
{
    return d->width;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setWidth set value detail seam allowance.
 * @param value width in mm.
 */
void VAbstractDetail::setWidth(const qreal &value)
{
    d->width = value;
}
