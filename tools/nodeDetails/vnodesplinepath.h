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
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VNODESPLINEPATH_H
#define VNODESPLINEPATH_H

#include "vabstractnode.h"
#include <QGraphicsPathItem>

class VNodeSplinePath : public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VNodeSplinePath(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                        Draw::Draws typeobject, Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    static VNodeSplinePath *Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idSpline,
                                       Draw::Draws typeobject, const Document::Documents &parse, Tool::Sources typeCreation);
public slots:
    virtual void FullUpdateFromFile();
protected:
    virtual void AddToFile();
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    void         RefreshGeometry();
};

#endif // VNODESPLINEPATH_H
