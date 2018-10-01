/************************************************************************
 **
 **  @file   vcurveangle.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vcurveangle.h"

#include <QLatin1String>
#include <QMessageLogger>

#include "../vmisc/def.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vellipticalarc.h"
#include "vcurvevariable.h"

//---------------------------------------------------------------------------------------------------------------------
VCurveAngle::VCurveAngle()
    :VCurveVariable()
{
    SetType(VarType::CurveAngle);
}

//---------------------------------------------------------------------------------------------------------------------
VCurveAngle::VCurveAngle(const quint32 &id, const quint32 &parentId)
    :VCurveVariable(id, parentId)
{
    SetType(VarType::CurveAngle);
}

//---------------------------------------------------------------------------------------------------------------------
VCurveAngle::VCurveAngle(const quint32 &id, const quint32 &parentId, const VAbstractCurve *curve, CurveAngle angle)
    :VCurveVariable(id, parentId)
{
    SetType(VarType::CurveAngle);
    SCASSERT(curve != nullptr)
    if (angle == CurveAngle::StartAngle)
    {
        SetValue(curve->GetStartAngle());
        SetName(angle1_V + curve->name());
    }
    else
    {
        SetValue(curve->GetEndAngle());
        SetName(angle2_V + curve->name());
    }
}

//---------------------------------------------------------------------------------------------------------------------
VCurveAngle::VCurveAngle(const quint32 &id, const quint32 &parentId, const QString &baseCurveName, const VSpline &spl,
                         CurveAngle angle, qint32 segment)
    :VCurveVariable(id, parentId)
{
    SetType(VarType::CurveAngle);
    if (angle == CurveAngle::StartAngle)
    {
        SetValue(spl.GetStartAngle());
        SetName(angle1_V + baseCurveName + QLatin1String("_") + seg_ + QString().setNum(segment));
    }
    else
    {
        SetValue(spl.GetEndAngle());
        SetName(angle2_V + baseCurveName + QLatin1String("_") + seg_ + QString().setNum(segment));
    }
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcRotation::VEllipticalArcRotation()
    : VCurveAngle()
{
    SetType(VarType::CurveAngle);
}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcRotation::VEllipticalArcRotation(const quint32 &id, const quint32 &parentId, const VEllipticalArc *elArc)
    : VCurveAngle(id, parentId)
{
    SetType(VarType::CurveAngle);
    SCASSERT(elArc != nullptr)
    SetValue(elArc->GetRotationAngle());
    SetName(rotation_V + elArc->name());
}
