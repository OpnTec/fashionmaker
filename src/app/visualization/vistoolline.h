/************************************************************************
 **
 **  @file   vgraphicslineitem.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2014
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

#ifndef VGRAPHICSLINEITEM_H
#define VGRAPHICSLINEITEM_H

#include "visline.h"

class VisToolLine : public VisLine
{
    Q_OBJECT
public:
    VisToolLine(const VContainer *data, QGraphicsItem *parent = 0);
    VisToolLine(const VContainer *data, const quint32 &pointId, const QPointF &scenePos,
                      QGraphicsItem * parent = 0);
    virtual ~VisToolLine();

    void         setPoint1Id(const quint32 &value);
    void         setPoint2Id(const quint32 &value);
    virtual void RefreshGeometry();
private:
    Q_DISABLE_COPY(VisToolLine)
    quint32          point1Id;
    quint32          point2Id;
};

#endif // VGRAPHICSLINEITEM_H
