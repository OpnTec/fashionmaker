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

#include "vcurveclength.h"

#include <QLatin1String>
#include <QMessageLogger>

#include "../vmisc/def.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vspline.h"
#include "vcurvevariable.h"

//---------------------------------------------------------------------------------------------------------------------
VCurveCLength::VCurveCLength()
    : VCurveVariable()
{
    SetType(VarType::CurveCLength);
}

//---------------------------------------------------------------------------------------------------------------------
VCurveCLength::VCurveCLength(const quint32 &id, const quint32 &parentId, const VAbstractBezier *curve,
                             CurveCLength cType, Unit patternUnit)
    : VCurveVariable(id, parentId)
{
    SetType(VarType::CurveCLength);
    SCASSERT(curve != nullptr)
    if (cType == CurveCLength::C1)
    {
        SetValue(FromPixel(curve->GetC1Length(), patternUnit));
        SetName(c1Length_V + curve->name());
    }
    else
    {
        SetValue(FromPixel(curve->GetC2Length(), patternUnit));
        SetName(c2Length_V + curve->name());
    }
}

//---------------------------------------------------------------------------------------------------------------------
VCurveCLength::VCurveCLength(const quint32 &id, const quint32 &parentId, const QString &baseCurveName,
                             const VSpline &spl, CurveCLength cType, Unit patternUnit, qint32 segment)
    : VCurveVariable(id, parentId)
{
    SetType(VarType::CurveCLength);
    if (cType == CurveCLength::C1)
    {
        SetValue(FromPixel(spl.GetC1Length(), patternUnit));
        SetName(c1Length_V + baseCurveName + QLatin1String("_") + seg_ + QString().setNum(segment));
    }
    else
    {
        SetValue(FromPixel(spl.GetC2Length(), patternUnit));
        SetName(c2Length_V + baseCurveName + QLatin1String("_") + seg_ + QString().setNum(segment));
    }
}

//---------------------------------------------------------------------------------------------------------------------
VCurveCLength::VCurveCLength(const VCurveCLength &var)
    : VCurveVariable(var)
{
}

//---------------------------------------------------------------------------------------------------------------------
VCurveCLength &VCurveCLength::operator=(const VCurveCLength &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VCurveVariable::operator=(var);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VCurveCLength::~VCurveCLength()
{
}

