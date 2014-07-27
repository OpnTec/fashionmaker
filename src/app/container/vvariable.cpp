/************************************************************************
 **
 **  @file   vvariable.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vvariable.h"
#include "../widgets/vapplication.h"
#include "../xml/vabstractmeasurements.h"

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable()
    :base(0), ksize(0), kheight(0), description(QString())
{
    Init();
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const qreal &base, const qreal &ksize, const qreal &kheight, const QString &description)
    :base(base), ksize(ksize), kheight(kheight), description(description)
{}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const qreal &base, const QString &description)
    :base(base), ksize(0), kheight(0), description(description)
{
    Init();
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const VVariable &var)
    :base(var.GetBase()), ksize(var.GetKsize()), kheight(var.GetKheight()), description(var.GetDescription())
{}

//---------------------------------------------------------------------------------------------------------------------
VVariable &VVariable::operator=(const VVariable &var)
{
    if ( &var == this )
    {
        return *this;
    }
    this->base = var.GetBase();
    this->ksize = var.GetKsize();
    this->kheight = var.GetKheight();
    this->description = var.GetDescription();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::~VVariable()
{}

//---------------------------------------------------------------------------------------------------------------------
qreal VVariable::GetValue(const qreal &size, const qreal &height) const
{
    if (qApp->patternUnit() != Unit::Inch)
    {
        const qreal baseSize = VAbstractMeasurements::UnitConvertor(50.0, Unit::Cm, qApp->patternUnit());
        const qreal baseHeight = VAbstractMeasurements::UnitConvertor(176.0, Unit::Cm, qApp->patternUnit());
        const qreal sizeIncrement = VAbstractMeasurements::UnitConvertor(2.0, Unit::Cm, qApp->patternUnit());
        const qreal heightIncrement = VAbstractMeasurements::UnitConvertor(6.0, Unit::Cm, qApp->patternUnit());

        // Formula for calculation gradation
        const qreal k_size    = ( size - baseSize ) / sizeIncrement;
        const qreal k_height  = ( height - baseHeight ) / heightIncrement;
        return base + k_size * ksize + k_height * kheight;
    }
    else// Must not be reached!!!!
    {
        return base;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VVariable::Init()
{
    if (qApp->patternUnit() != Unit::Inch)
    {
        ksize = VAbstractMeasurements::UnitConvertor(50.0, Unit::Cm, qApp->patternUnit());
        kheight = VAbstractMeasurements::UnitConvertor(176.0, Unit::Cm, qApp->patternUnit());
    }
}
