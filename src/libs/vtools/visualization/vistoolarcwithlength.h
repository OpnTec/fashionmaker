/************************************************************************
 **
 **  @file   vistoolarcwithlength.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2015
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

#ifndef VISTOOLARCWITHLENGTH_H
#define VISTOOLARCWITHLENGTH_H

#include "vispath.h"

class VisToolArcWithLength : public VisPath
{
    Q_OBJECT
public:
    explicit VisToolArcWithLength(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolArcWithLength() Q_DECL_OVERRIDE;

    virtual void RefreshGeometry() Q_DECL_OVERRIDE;
    void         setRadius(const QString &expression);
    void         setF1(const QString &expression);
    void         setLength(const QString &expression);
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolArcWithLength)};
private:
    Q_DISABLE_COPY(VisToolArcWithLength)
    QGraphicsEllipseItem *arcCenter;
    qreal                radius;
    qreal                f1;
    qreal                length;
};

#endif // VISTOOLARCWITHLENGTH_H
