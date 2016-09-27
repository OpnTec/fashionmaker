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

#ifndef VABSTRACTBEZIER_H
#define VABSTRACTBEZIER_H

#include <QtGlobal>

#include "vabstractcurve.h"

class VAbstractBezier : public VAbstractCurve
{
public:
    VAbstractBezier(const GOType &type, const quint32 &idObject = NULL_ID, const Draw &mode = Draw::Calculation);
    VAbstractBezier(const VAbstractBezier &curve);
    VAbstractBezier& operator= (const VAbstractBezier &curve);
    virtual ~VAbstractBezier();

    virtual qreal GetC1Length() const =0;
    virtual qreal GetC2Length() const =0;
};

#endif // VABSTRACTBEZIER_H
