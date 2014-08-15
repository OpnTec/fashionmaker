/************************************************************************
 **
 **  @file   vlengthsplines.cpp
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

#include "vlengthspline.h"
#include "../geometry/vabstractcurve.h"
#include "../widgets/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
VLengthSpline::VLengthSpline()
    :VLengthCurve()
{
    type = VarType::LengthSpline;
}

VLengthSpline::VLengthSpline(const quint32 &id, const quint32 &parentId, const QString &name, const qreal &value)
    :VLengthCurve()
{
    type = VarType::LengthSpline;
    this->name = name;
    this->value = value;
    this->id = id;
    this->parentId = parentId;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthSpline::VLengthSpline(const quint32 &id, const quint32 &parentId, const VAbstractCurve *path)
    :VLengthCurve(id, parentId, path)
{
    type = VarType::LengthSpline;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthSpline::VLengthSpline(const VLengthSpline &var)
    :VLengthCurve(var)
{}

//---------------------------------------------------------------------------------------------------------------------
VLengthSpline &VLengthSpline::operator=(const VLengthSpline &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VLengthCurve::operator=(var);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthSpline::~VLengthSpline()
{}
