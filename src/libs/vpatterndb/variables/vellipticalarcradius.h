/************************************************************************
 **
 **  @file   vellipticalarcradius.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   16 12, 2016
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
#ifndef VELLIPTICALARCRADIUS_H
#define VELLIPTICALARCRADIUS_H

#include <qcompilerdetection.h>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "../ifc/ifcdef.h"
#include "vcurvevariable.h"

class VEllipticalArc;

class VEllipticalArcRadius :public VCurveVariable
{
public:
    VEllipticalArcRadius();
    VEllipticalArcRadius(const quint32 &id, const quint32 &parentId, const VEllipticalArc *elArc,
                         const int numberRadius, Unit patternUnit);
    VEllipticalArcRadius(const VEllipticalArcRadius &var);
    VEllipticalArcRadius &operator=(const VEllipticalArcRadius &var);
    virtual ~VEllipticalArcRadius() Q_DECL_OVERRIDE;
};

#endif // VELLIPTICALARCRADIUS_H
