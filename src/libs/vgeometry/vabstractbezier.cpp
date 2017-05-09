/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vabstractbezier.h"

//---------------------------------------------------------------------------------------------------------------------
VAbstractBezier::VAbstractBezier(const GOType &type, const quint32 &idObject, const Draw &mode)
    : VAbstractCurve(type, idObject, mode)
{
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractBezier::VAbstractBezier(const VAbstractBezier &curve)
    : VAbstractCurve(curve)
{
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractBezier &VAbstractBezier::operator=(const VAbstractBezier &curve)
{
    if ( &curve == this )
    {
        return *this;
    }
    VAbstractCurve::operator=(curve);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractBezier::~VAbstractBezier()
{
}
