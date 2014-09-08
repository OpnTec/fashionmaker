/************************************************************************
 **
 **  @file   vistoolalongline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 7, 2014
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

#ifndef VISTOOLALONGLINE_H
#define VISTOOLALONGLINE_H

#include "visline.h"

class VContainer;

class VisToolAlongLine :public VisLine
{
    Q_OBJECT
public:
    VisToolAlongLine(const VContainer *data, QGraphicsItem *parent = 0);
    virtual ~VisToolAlongLine();

    virtual void RefreshGeometry();
    void         setPoint2Id(const quint32 &value);
    void         setLength(const QString &expression);
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolAlongLine)};
private:
    Q_DISABLE_COPY(VisToolAlongLine)
    quint32              point2Id;
    QGraphicsEllipseItem *point;
    QGraphicsEllipseItem *lineP1;
    QGraphicsEllipseItem *lineP2;
    QGraphicsLineItem    *line;
    qreal                length;
};

#endif // VISTOOLALONGLINE_H
