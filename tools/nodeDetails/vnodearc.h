/************************************************************************
 **
 **  @file   vnodearc.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

#ifndef VNODEARC_H
#define VNODEARC_H

#include "vabstractnode.h"
#include <QGraphicsPathItem>

class VNodeArc :public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
                 VNodeArc(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc, Draw::Draws typeobject,
                          const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    static void  Create(VDomDocument *doc, VContainer *data, qint64 id, qint64 idArc, const Draw::Draws &typeobject,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    static const QString TagName;
    static const QString ToolType;
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

#endif // VNODEARC_H4
