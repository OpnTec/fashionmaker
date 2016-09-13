/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
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

#ifndef VISTOOLFLIPPINGBYLINE_H
#define VISTOOLFLIPPINGBYLINE_H

#include <QtGlobal>

#include "visoperation.h"

class VisToolFlippingByLine : public VisOperation
{
    Q_OBJECT
public:
    explicit VisToolFlippingByLine(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolFlippingByLine();

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;

    void SetFirstLinePointId(quint32 value);
    void SetSecondLinePointId(quint32 value);

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolFlippingByLine)};
private:
    Q_DISABLE_COPY(VisToolFlippingByLine)
    quint32               object2Id;
    QGraphicsEllipseItem *point1;
    QGraphicsEllipseItem *point2;

    template <class Item>
    int AddCurve(const QPointF &firstPoint, const QPointF &secondPoint, quint32 id, int i);
};

#endif // VISTOOLFLIPPINGBYLINE_H
