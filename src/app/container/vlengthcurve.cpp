/************************************************************************
 **
 **  @file   vlengthcurve.cpp
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

#include "vlengthcurve.h"
#include "../widgets/vapplication.h"
#include "../geometry/vabstractcurve.h"

//---------------------------------------------------------------------------------------------------------------------
VLengthCurve::VLengthCurve()
    :VInternalVariable(), id(0), parentId(0)
{
    type = VarType::Unknown;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthCurve::VLengthCurve(const quint32 &id, const quint32 &parentId, const VAbstractCurve *curve)
    :VInternalVariable(), id(id), parentId(parentId)
{
    type = VarType::Unknown;
    SCASSERT(curve != nullptr);
    name = curve->name();
    value = qApp->fromPixel(curve->GetLength());
}

//---------------------------------------------------------------------------------------------------------------------
VLengthCurve::VLengthCurve(const VLengthCurve &var)
    :VInternalVariable(var), id(var.GetId()), parentId(var.GetParentId())
{}

//---------------------------------------------------------------------------------------------------------------------
VLengthCurve &VLengthCurve::operator=(const VLengthCurve &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VInternalVariable::operator=(var);
    this->id = var.GetId();
    this->parentId = var.GetParentId();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthCurve::~VLengthCurve()
{}

//---------------------------------------------------------------------------------------------------------------------
bool VLengthCurve::Filter(quint32 id)
{
    if (parentId != 0)//Do not check if value zero
    {// Not all curves have parents. Only those who was created after cutting the parent curve.
        return this->id == id || parentId == id;
    }
    else
    {
        return this->id == id;
    }
}
