/************************************************************************
 **
 **  @file   vlengtharc.cpp
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

#include "vlengtharc.h"
#include "../geometry/varc.h"
#include "../widgets/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
VLengthArc::VLengthArc()
    :VInternalVariable(), id(0)
{
    type = VarType::LengthArc;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthArc::VLengthArc(const quint32 &id, const VArc *arc)
    :VInternalVariable(), id(id)
{
    type = VarType::LengthArc;
    SCASSERT(arc != nullptr);
    name = arc->name();
    value = qApp->fromPixel(arc->GetLength());
}

//---------------------------------------------------------------------------------------------------------------------
VLengthArc::VLengthArc(const VLengthArc &var)
    :VInternalVariable(var), id(var.GetId())
{}

//---------------------------------------------------------------------------------------------------------------------
VLengthArc &VLengthArc::operator=(const VLengthArc &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VInternalVariable::operator=(var);
    this->id = var.GetId();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthArc::~VLengthArc()
{}

//---------------------------------------------------------------------------------------------------------------------
bool VLengthArc::Filter(quint32 id)
{
    return this->id == id;
}
