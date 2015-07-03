/************************************************************************
 **
 **  @file   vistoolpointofcontact.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VISTOOLPOINTOFCONTACT_H
#define VISTOOLPOINTOFCONTACT_H

#include "visline.h"

class VisToolPointOfContact : public VisLine
{
    Q_OBJECT
public:
    VisToolPointOfContact(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolPointOfContact() Q_DECL_OVERRIDE;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;
    void         setLineP2Id(const quint32 &value);
    void         setRadiusId(const quint32 &value);
    void         setRadius(const QString &expression);
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPointOfContact)};
private:
    Q_DISABLE_COPY(VisToolPointOfContact)
    quint32              lineP2Id;
    quint32              radiusId;
    QGraphicsEllipseItem *point;
    QGraphicsEllipseItem *lineP1;
    QGraphicsEllipseItem *lineP2;
    QGraphicsEllipseItem *arc_point;
    QGraphicsEllipseItem *circle;
    qreal                radius;

};

#endif // VISTOOLPOINTOFCONTACT_H
