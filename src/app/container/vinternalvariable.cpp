/************************************************************************
 **
 **  @file   vinternalvariable.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 7, 2014
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

#include "vinternalvariable.h"
#include "vinternalvariable_p.h"

//---------------------------------------------------------------------------------------------------------------------
VInternalVariable::VInternalVariable()
    :d(new VInternalVariableData)
{}

//---------------------------------------------------------------------------------------------------------------------
VInternalVariable::VInternalVariable(const VInternalVariable &var)
    :d(var.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VInternalVariable &VInternalVariable::operator=(const VInternalVariable &var)
{
    if ( &var == this )
    {
        return *this;
    }
    d = var.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VInternalVariable::~VInternalVariable()
{}

//---------------------------------------------------------------------------------------------------------------------
bool VInternalVariable::Filter(quint32 id)
{
    Q_UNUSED(id);
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VInternalVariable::GetValue() const
{
    return d->value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal *VInternalVariable::GetValue()
{
    return &d->value;
}

void VInternalVariable::SetValue(const qreal &value)
{
    d->value = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VInternalVariable::GetName() const
{
    return d->name;
}

//---------------------------------------------------------------------------------------------------------------------
void VInternalVariable::SetName(const QString &name)
{
    d->name = name;
}

//---------------------------------------------------------------------------------------------------------------------
VarType VInternalVariable::GetType() const
{
    return d->type;
}


//---------------------------------------------------------------------------------------------------------------------
void VInternalVariable::SetType(const VarType &type)
{
    d->type = type;
}
