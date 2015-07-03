/************************************************************************
 **
 **  @file   vistoolpointfromarcandtangent.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 6, 2015
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

#ifndef VISTOOLPOINTFROMARCANDTANGENT_H
#define VISTOOLPOINTFROMARCANDTANGENT_H

#include "visline.h"
#include "../ifc/xml/vabstractpattern.h"

class VisToolPointFromArcAndTangent : public VisLine
{
    Q_OBJECT
public:
    VisToolPointFromArcAndTangent(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolPointFromArcAndTangent() Q_DECL_OVERRIDE;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;

    void         setArcId(const quint32 &value);
    void         setCrossPoint(const CrossCirclesPoint &value);

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPointFromArcAndTangent)};
private:
    Q_DISABLE_COPY(VisToolPointFromArcAndTangent)
    quint32              arcId;
    CrossCirclesPoint    crossPoint;
    QGraphicsEllipseItem *point;
    QGraphicsEllipseItem *tangent;
    QGraphicsPathItem    *arcPath;
    QGraphicsLineItem    *tangentLine2;

    void FindRays(const QPointF &p, const VArc *arc);
};

#endif // VISTOOLPOINTFROMARCANDTANGENT_H
