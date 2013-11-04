/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
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
 ****************************************************************************/

#ifndef VMODELINGPOINT_H
#define VMODELINGPOINT_H

#include "vmodelingtool.h"
#include "widgets/vgraphicssimpletextitem.h"

class VModelingPoint: public VModelingTool, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
                            VModelingPoint(VDomDocument *doc, VContainer *data, qint64 id, QGraphicsItem * parent = 0);
    virtual                 ~VModelingPoint() {}
    static const QString    TagName;
public slots:
    void                    NameChangePosition(const QPointF pos);
    virtual void            FullUpdateFromGui(int result) = 0;
protected:
    qreal                   radius;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *lineName;
    virtual void            UpdateNamePosition(qreal mx, qreal my);
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            RefreshPointGeometry(const VPointF &point);
    void                    RefreshLine();
private:
    Q_DISABLE_COPY(VModelingPoint)
};

#endif // VMODELINGPOINT_H
