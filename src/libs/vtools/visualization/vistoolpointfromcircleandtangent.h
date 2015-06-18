/************************************************************************
 **
 **  @file   vistoolpointfromcircleandtangent.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 6, 2015
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

#ifndef VISTOOLPOINTFROMCIRCLEANDTANGENT_H
#define VISTOOLPOINTFROMCIRCLEANDTANGENT_H

#include "visline.h"
#include "../ifc/xml/vabstractpattern.h"

class VisToolPointFromCircleAndTangent : public VisLine
{
    Q_OBJECT
public:
    VisToolPointFromCircleAndTangent(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolPointFromCircleAndTangent();

    virtual void RefreshGeometry();

    void         setPoint2Id(const quint32 &value);
    void         setCRadius(const QString &value);
    void         setCrossPoint(const CrossCirclesPoint &value);

    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPointFromCircleAndTangent)};
private:
    Q_DISABLE_COPY(VisToolPointFromCircleAndTangent)
    quint32              point2Id;
    qreal                cRadius;
    CrossCirclesPoint    crossPoint;
    QGraphicsEllipseItem *point;
    QGraphicsEllipseItem *tangent;
    QGraphicsEllipseItem *cCenter;
    QGraphicsEllipseItem *cPath;
    QGraphicsLineItem    *tangent2;

    void FindRays(const QPointF &p, const QPointF &center, qreal radius);
};

#endif // VISTOOLPOINTFROMCIRCLEANDTANGENT_H
