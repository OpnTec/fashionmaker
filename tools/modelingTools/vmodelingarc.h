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

#ifndef VMODELINGARC_H
#define VMODELINGARC_H

#include "vmodelingtool.h"
#include <QGraphicsPathItem>
#include "dialogs/dialogarc.h"
#include "widgets/vcontrolpointspline.h"

class VModelingArc :public VModelingTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                         VModelingArc(VDomDocument *doc, VContainer *data, qint64 id, Tool::Sources typeCreation,
                                      QGraphicsItem * parent = 0);
    virtual void         setDialog();
    static VModelingArc* Create(QSharedPointer<DialogArc> &dialog, VDomDocument *doc, VContainer *data);
    static VModelingArc* Create(const qint64 _id, const qint64 &center, const QString &radius, const QString &f1,
                                const QString &f2, VDomDocument *doc, VContainer *data,
                                const Document::Documents &parse, Tool::Sources typeCreation);
    static const QString TagName;
    static const QString ToolType;
public slots:
    virtual void         FullUpdateFromFile();
    virtual void         FullUpdateFromGui(int result);
protected:
    virtual void         contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void         AddToFile();
    virtual void         mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void         hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void         hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void         RemoveReferens();
private:
    QSharedPointer<DialogArc> dialogArc;
    void                 RefreshGeometry();
};

#endif // VMODELINGARC_H
