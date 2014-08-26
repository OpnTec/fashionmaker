/************************************************************************
 **
 **  @file   vsimplesplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 12, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VSIMPLESPLINEPATH_H
#define VSIMPLESPLINEPATH_H

#include <QGraphicsPathItem>
#include "../tools/vabstracttool.h"

/**
 * @brief The VSimpleSplinePath class for simple spline path. This object used when we cut spline path and want show
 * peaces.
 */
class VSimpleSplinePath : public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VSimpleSplinePath(VPattern *doc, VContainer *data, quint32 id, qreal *factor);
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::SimpleSplinePath)};
protected:
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    Q_DISABLE_COPY(VSimpleSplinePath)
    /** @brief factor scale factor. */
    qreal *factor;
};

#endif // VSIMPLESPLINEPATH_H
