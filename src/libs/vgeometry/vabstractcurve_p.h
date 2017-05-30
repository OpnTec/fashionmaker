/************************************************************************
 **
 **  @file   vabstractcurve_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 2, 2016
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

#ifndef VABSTRACTCURVE_P_H
#define VABSTRACTCURVE_P_H

#include <QSharedData>

#include "../ifc/ifcdef.h"
#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VAbstractCurveData : public QSharedData
{
public:

    VAbstractCurveData ()
        : duplicate(0),
          color(ColorBlack),
          penStyle(TypeLineLine)
    {}

    VAbstractCurveData(const VAbstractCurveData &curve)
        : QSharedData(curve),
          duplicate(curve.duplicate),
          color(curve.color),
          penStyle(curve.penStyle)
    {}

    virtual ~VAbstractCurveData();

    /** @brief duplicate helps create unique name for curves that connects the same start and finish points. */
    quint32 duplicate;

    QString color;
    QString penStyle;

private:
    VAbstractCurveData &operator=(const VAbstractCurveData &) Q_DECL_EQ_DELETE;
};

VAbstractCurveData::~VAbstractCurveData()
{}

QT_WARNING_POP

#endif // VABSTRACTCURVE_P_H
