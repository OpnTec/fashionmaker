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

#ifndef VTOOLHEIGHT_H
#define VTOOLHEIGHT_H

#include "vtoollinepoint.h"
#include "dialogs/dialogheight.h"

class VToolHeight: public VToolLinePoint
{
    Q_OBJECT
public:
                   VToolHeight(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                               const qint64 &basePointId, const qint64 &p1LineId, const qint64 &p2LineId,
                               Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void   setDialog();
    static void    Create(QSharedPointer<DialogHeight> &dialog, VMainGraphicsScene  *scene,
                          VDomDocument *doc, VContainer *data);
    static void    Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                          const qint64 &basePointId, const qint64 &p1LineId, const qint64 &p2LineId,
                          const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc,
                          VContainer *data, const Document::Documents &parse, Tool::Sources typeCreation);
    static QPointF FindPoint(const QLineF &line, const QPointF &point);
    static const QString ToolType;
public slots:
    virtual void   FullUpdateFromFile();
    virtual void   FullUpdateFromGui(int result);
protected:
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void   AddToFile();
private:
    QSharedPointer<DialogHeight> dialogHeight;
    qint64         p1LineId;
    qint64         p2LineId;
};

#endif // VTOOLHEIGHT_H
