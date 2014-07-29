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
    :VInternalVariable(), base(0), ksize(0), kheight(0), description(QString())
{
    Init();
    value = base;
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const QString &name, const qreal &base, const qreal &ksize, const qreal &kheight,
                     const QString &description)
    :VInternalVariable(), base(base), ksize(ksize), kheight(kheight), description(description)
{
    value = base;
    this->name = name;
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const QString &name, const qreal &base, const QString &description)
    :base(base), ksize(0), kheight(0), description(description)
{
    Init();
    value = base;
    this->name = name;
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const VVariable &var)
    :VInternalVariable(var), base(var.GetBase()), ksize(var.GetKsize()), kheight(var.GetKheight()),
      description(var.GetDescription())
{}

//---------------------------------------------------------------------------------------------------------------------
VVariable &VVariable::operator=(const VVariable &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VInternalVariable::operator=(var);
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
void VVariable::SetValue(const qreal &size, const qreal &height)
{
    if (qApp->patternUnit() == Unit::Inch)
    {
        qWarning("Gradation doesn't support inches");
        return;
    }
    const qreal baseSize = VAbstractMeasurements::UnitConvertor(50.0, Unit::Cm, qApp->patternUnit());
    const qreal baseHeight = VAbstractMeasurements::UnitConvertor(176.0, Unit::Cm, qApp->patternUnit());
    const qreal sizeIncrement = VAbstractMeasurements::UnitConvertor(2.0, Unit::Cm, qApp->patternUnit());
    const qreal heightIncrement = VAbstractMeasurements::UnitConvertor(6.0, Unit::Cm, qApp->patternUnit());

    // Formula for calculation gradation
    const qreal k_size    = ( size - baseSize ) / sizeIncrement;
    const qreal k_height  = ( height - baseHeight ) / heightIncrement;
    value = base + k_size * ksize + k_height * kheight;
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
