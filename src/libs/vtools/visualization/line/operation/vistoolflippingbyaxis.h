/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef VISTOOLFLIPPINGBYAXIS_H
#define VISTOOLFLIPPINGBYAXIS_H

#include <QtGlobal>

#include "visoperation.h"
#include "../ifc/xml/vabstractpattern.h"

class VisToolFlippingByAxis : public VisOperation
{
    Q_OBJECT
public:
    explicit VisToolFlippingByAxis(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolFlippingByAxis() = default;

    virtual void RefreshGeometry() override;

    void SetOriginPointId(quint32 value);
    void SetAxisType(AxisType value);

    virtual int type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolFlippingByAxis)};
private:
    Q_DISABLE_COPY(VisToolFlippingByAxis)

    AxisType m_axisType;

    VScaledEllipse *point1;
};

#endif // VISTOOLFLIPPINGBYAXIS_H
