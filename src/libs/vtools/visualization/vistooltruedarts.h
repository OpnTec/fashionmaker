/************************************************************************
 **
 **  @file   vistooltruedarts.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 6, 2015
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

#ifndef VISTOOLTRUEDARTS_H
#define VISTOOLTRUEDARTS_H

#include "visline.h"

class VisToolTrueDarts :public VisLine
{
    Q_OBJECT
public:
    explicit VisToolTrueDarts(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolTrueDarts() Q_DECL_OVERRIDE;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;

    void setPoint2Id(const quint32 &value);
    void setD1PointId(const quint32 &value);
    void setD2PointId(const quint32 &value);
    void setD3PointId(const quint32 &value);

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolTrueDarts)};
private:
    Q_DISABLE_COPY(VisToolTrueDarts)
    quint32 baseLineP2Id;
    quint32 dartP1Id;
    quint32 dartP2Id;
    quint32 dartP3Id;

    QGraphicsEllipseItem *point1;
    QGraphicsEllipseItem *point2;
    QGraphicsEllipseItem *baseLineP1;
    QGraphicsEllipseItem *baseLineP2;
    QGraphicsEllipseItem *dartP1;
    QGraphicsEllipseItem *dartP2;
    QGraphicsEllipseItem *dartP3;

    QGraphicsLineItem *lineblP1P1;
    QGraphicsLineItem *lineblP2P2;
    QGraphicsLineItem *p1d2;
    QGraphicsLineItem *d2p2;
};

#endif // VISTOOLTRUEDARTS_H
