/************************************************************************
 **
 **  @file   vistoolpointofintersection.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 8, 2014
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

#ifndef VISTOOLPOINTOFINTERSECTION_H
#define VISTOOLPOINTOFINTERSECTION_H

#include "visline.h"

class VisToolPointOfIntersection : public VisLine
{
    Q_OBJECT
public:
    VisToolPointOfIntersection(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolPointOfIntersection();

    virtual void RefreshGeometry();

    void         setPoint2Id(const quint32 &value);
private:
    Q_DISABLE_COPY(VisToolPointOfIntersection)
    quint32              point2Id;
    QGraphicsEllipseItem *point;
    QGraphicsEllipseItem *axisP1;//axis1 is class themself
    QGraphicsEllipseItem *axisP2;
    QGraphicsLineItem    *axis2;//axis1 is class themself

    QLineF       Axis(const QPointF &p, const qreal &angle) const;
    void         ShowIntersection(const QLineF &axis1, const QLineF &axis2);
};

#endif // VISTOOLPOINTOFINTERSECTION_H
