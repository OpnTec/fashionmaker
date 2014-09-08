/************************************************************************
 **
 **  @file   vistoolcutsplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 9, 2014
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

#ifndef VISTOOLCUTSPLINEPATH_H
#define VISTOOLCUTSPLINEPATH_H

#include "vispath.h"

class VisToolCutSplinePath : public VisPath
{
    Q_OBJECT
public:
    VisToolCutSplinePath(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolCutSplinePath();

    virtual void RefreshGeometry();
    void         setLength(const QString &expression);
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolCutSpline)};
protected:
    Q_DISABLE_COPY(VisToolCutSplinePath)
    QGraphicsEllipseItem *point;
    QGraphicsPathItem    *splPath1;
    QGraphicsPathItem    *splPath2;
    qreal                length;
};

#endif // VISTOOLCUTSPLINEPATH_H
