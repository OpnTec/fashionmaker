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
#include "vvariable_p.h"
#include "../widgets/vapplication.h"
#include "../xml/vabstractmeasurements.h"

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable()
    :VInternalVariable(), d(new VVariableData)
{
    VInternalVariable::SetValue(d->base);
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const QString &name, const qreal &base, const qreal &ksize, const qreal &kheight,
                     const QString &description)
    :VInternalVariable(), d(new VVariableData(base, ksize, kheight, description))
{
    VInternalVariable::SetValue(d->base);
    SetName(name);
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const QString &name, const qreal &base, const QString &description)
    :d(new VVariableData(base, description))
{
    VInternalVariable::SetValue(base);
    SetName(name);
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const VVariable &var)
    :VInternalVariable(var), d(var.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VVariable &VVariable::operator=(const VVariable &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VInternalVariable::operator=(var);
    d = var.d;
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
    VInternalVariable::SetValue(d->base + k_size * d->ksize + k_height * d->kheight);
}

//---------------------------------------------------------------------------------------------------------------------
bool VVariable::IsNotUsed() const
{
    if (qFuzzyCompare(d->base+1, 0+1) && qFuzzyCompare(d->ksize+1, 0+1) && qFuzzyCompare(d->kheight+1, 0+1))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetBase return value in base size and height
 * @return value
 */
qreal VVariable::GetBase() const
{
    return d->base;
}

//---------------------------------------------------------------------------------------------------------------------
void VVariable::SetBase(const qreal &value)
{
    d->base = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKsize return increment in sizes
 * @return increment
 */
qreal VVariable::GetKsize() const
{
    return d->ksize;
}

//---------------------------------------------------------------------------------------------------------------------
void VVariable::SetKsize(const qreal &value)
{
    d->ksize = value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetKheight return increment in heights
 * @return increment
 */
qreal VVariable::GetKheight() const
{
    return d->kheight;
}


//---------------------------------------------------------------------------------------------------------------------
void VVariable::SetKheight(const qreal &value)
{
    d->kheight = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VVariable::GetDescription() const
{
    return d->description;
}


//---------------------------------------------------------------------------------------------------------------------
void VVariable::SetDescription(const QString &desc)
{
    d->description = desc;
}
