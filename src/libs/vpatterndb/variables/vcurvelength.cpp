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
 **  Copyright (C) 2013-2015 Valentina project
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

#include <QLatin1String>
#include <QMessageLogger>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vspline.h"
#include "vcurvevariable.h"

//---------------------------------------------------------------------------------------------------------------------
VCurveLength::VCurveLength()
    :VCurveVariable()
{
    SetType(VarType::CurveLength);
}

//---------------------------------------------------------------------------------------------------------------------
VCurveLength::VCurveLength(const quint32 &id, const quint32 &parentId, const VAbstractCurve *curve, Unit patternUnit)
    :VCurveVariable(id, parentId)
{
    SetType(VarType::CurveLength);
    SCASSERT(curve != nullptr);
    SetName(curve->name());
    SetValue(FromPixel(curve->GetLength(), patternUnit));
}

//---------------------------------------------------------------------------------------------------------------------
VCurveLength::VCurveLength(const quint32 &id, const quint32 &parentId, const QString &baseCurveName, const VSpline &spl,
                           Unit patternUnit, qint32 segment)
    :VCurveVariable(id, parentId)
{
    SCASSERT(not baseCurveName.isEmpty())

    SetType(VarType::CurveLength);
    SetName(baseCurveName + QLatin1String("_") + seg_ + QString().setNum(segment));
    SetValue(FromPixel(spl.GetLength(), patternUnit));
}

//---------------------------------------------------------------------------------------------------------------------
VCurveLength::VCurveLength(const VCurveLength &var)
    :VCurveVariable(var)
{}

//---------------------------------------------------------------------------------------------------------------------
VCurveLength &VCurveLength::operator=(const VCurveLength &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VCurveVariable::operator=(var);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VCurveLength::~VCurveLength()
{}
