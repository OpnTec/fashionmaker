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

#ifndef VTOOLLINE_H
#define VTOOLLINE_H

#include "vdrawtool.h"
#include "QGraphicsLineItem"
#include "dialogs/dialogline.h"

class VToolLine: public VDrawTool, public QGraphicsLineItem
{
    Q_OBJECT
public:
    VToolLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint,
              qint64 secondPoint, Tool::Enum typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void Create(QSharedPointer<DialogLine> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                       VContainer *data);
    static void Create(const qint64 &id, const qint64 &firstPoint, const qint64 &secondPoint,
                       VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
                       const Document::Documents &parse, Tool::Enum typeCreation);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      ChangedActivDraw(const QString newName);
    virtual void      FullUpdateFromGui(int result);
    virtual void      ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
    virtual void      hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void      hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    qint64            firstPoint;
    qint64            secondPoint;
    QSharedPointer<DialogLine> dialogLine;
};

#endif // VTOOLLINE_H
