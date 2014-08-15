/************************************************************************
 **
 **  @file   vlengthcurve.h
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

#ifndef VLENGTHCURVE_H
#define VLENGTHCURVE_H

#include "vinternalvariable.h"

class VAbstractCurve;

class VLengthCurve : public VInternalVariable
{
public:
    VLengthCurve();
    VLengthCurve(const quint32 &id, const quint32 &parentId, const VAbstractCurve *curve);
    VLengthCurve(const VLengthCurve &var);
    VLengthCurve &operator=(const VLengthCurve &var);
    virtual ~VLengthCurve();

    virtual bool Filter(quint32 id);
    quint32      GetId() const;
    quint32      GetParentId() const;
protected:
    quint32 id;
    quint32 parentId;
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 VLengthCurve::GetId() const
{
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
inline quint32 VLengthCurve::GetParentId() const
{
    return parentId;
}

#endif // VLENGTHCURVE_H
