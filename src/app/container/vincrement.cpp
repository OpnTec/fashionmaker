/************************************************************************
 **
 **  @file   vincrementtablerow.cpp
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

#include "vincrement.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VIncrement create enpty increment
 */
VIncrement::VIncrement()
    :id(0), base(0), ksize(50.0), kheight(176.0), description(QString())
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VIncrementTableRow create increment
 * @param id id
 * @param base value in base size and growth
 * @param ksize increment in sizes
 * @param kheight increment in heights
 * @param description description of increment
 */
VIncrement::VIncrement(quint32 id, qreal base, qreal ksize, qreal kheight, QString description)
    :id(id), base(base), ksize(ksize), kheight(kheight), description(description)
{}

//---------------------------------------------------------------------------------------------------------------------
VIncrement::VIncrement(const VIncrement &incr)
    :id(incr.getId()), base(incr.getBase()), ksize(incr.getKsize()), kheight(incr.getKheight()),
      description(incr.getDescription())
{}

//---------------------------------------------------------------------------------------------------------------------
VIncrement &VIncrement::operator=(const VIncrement &incr)
{
    this->id = incr.getId();
    this->base = incr.getBase();
    this->ksize = incr.getKsize();
    this->kheight = incr.getKheight();
    this->description = incr.getDescription();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VIncrement::GetValue() const
{
    return base;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VIncrement::GetValue(const qreal &size, const qreal &height) const
{
    const qreal k_size    = ( size - 50.0 ) / 2.0;
    const qreal k_height  = ( height - 176.0 ) / 6.0;
    return base + k_size * ksize + k_height * kheight;
}
