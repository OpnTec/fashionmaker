/************************************************************************
 **
 **  @file   vcurvelength.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 8, 2014
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

#include "vcurvelength.h"
#include "vcurvelength_p.h"
#include "../widgets/vapplication.h"
#include "../geometry/vabstractcurve.h"

//---------------------------------------------------------------------------------------------------------------------
VCurveLength::VCurveLength()
    :VInternalVariable(), d(new VCurveLengthData)
{
    SetType(VarType::Unknown);
}

//---------------------------------------------------------------------------------------------------------------------
VCurveLength::VCurveLength(const quint32 &id, const quint32 &parentId, const VAbstractCurve *curve)
    :VInternalVariable(), d(new VCurveLengthData(id, parentId))
{
    SetType(VarType::Unknown);
    SCASSERT(curve != nullptr);
    SetName(curve->name());
    SetValue(qApp->fromPixel(curve->GetLength()));
}

//---------------------------------------------------------------------------------------------------------------------
VCurveLength::VCurveLength(const VCurveLength &var)
    :VInternalVariable(var), d(var.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VCurveLength &VCurveLength::operator=(const VCurveLength &var)
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
VCurveLength::~VCurveLength()
{}

//---------------------------------------------------------------------------------------------------------------------
bool VCurveLength::Filter(quint32 id)
{
    if (d->parentId != 0)//Do not check if value zero
    {// Not all curves have parents. Only those who was created after cutting the parent curve.
        return d->id == id || d->parentId == id;
    }
    else
    {
        return d->id == id;
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VCurveLength::GetId() const
{
    return d->id;
}

//---------------------------------------------------------------------------------------------------------------------
void VCurveLength::SetId(const quint32 &id)
{
    d->id = id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VCurveLength::GetParentId() const
{
    return d->parentId;
}

//---------------------------------------------------------------------------------------------------------------------
void VCurveLength::SetParentId(const quint32 &value)
{
    d->parentId = value;
}
