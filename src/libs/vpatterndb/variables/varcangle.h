/************************************************************************
 **
 **  @file   varcangle.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef VARCANGLE_H
#define VARCANGLE_H

#include "vcurveangle.h"

class VAbstractCurve;

class VArcAngle :public VCurveAngle
{
public:
    VArcAngle();
    VArcAngle(const quint32 &id, const quint32 &parentId, const VAbstractCurve *curve, CurveAngle angle);
    VArcAngle(const VArcAngle &var);
    VArcAngle &operator=(const VArcAngle &var);
    virtual ~VArcAngle() Q_DECL_OVERRIDE;
};

#endif // VARCANGLE_H
