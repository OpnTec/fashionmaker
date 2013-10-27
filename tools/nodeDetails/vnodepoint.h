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

#ifndef VNODEPOINT_H
#define VNODEPOINT_H

#include "vabstractnode.h"
#include "widgets/vgraphicssimpletextitem.h"

class VNodePoint: public VAbstractNode, public QGraphicsEllipseItem{
    Q_OBJECT
public:
                 VNodePoint(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint, Draw::Draws typeobject,
                            Tool::Sources typeCreation, QGraphicsItem * parent = 0 );
    static void  Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idPoint, Draw::Draws typeobject,
                        const Document::Documents &parse, Tool::Sources typeCreation);
public slots:
    virtual void FullUpdateFromFile();
    void         NameChangePosition(const QPointF pos);
protected:
    qreal        radius;
    VGraphicsSimpleTextItem *namePoint;
    QGraphicsLineItem       *lineName;
    virtual void AddToFile();
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void UpdateNamePosition(qreal mx, qreal my);
    virtual void RefreshPointGeometry(const VPointF &point);
    void         RefreshLine();
private:
    Q_DISABLE_COPY(VNodePoint)
};

#endif // VNODEPOINT_H
