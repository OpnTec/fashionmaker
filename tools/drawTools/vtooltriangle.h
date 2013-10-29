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

#ifndef VTOOLTRIANGLE_H
#define VTOOLTRIANGLE_H

#include "vtoolpoint.h"
#include "dialogs/dialogtriangle.h"

class VToolTriangle : public VToolPoint{
    Q_OBJECT
public:
                   VToolTriangle(VDomDocument *doc, VContainer *data, const qint64 &id, const qint64 &axisP1Id,
                                 const qint64 &axisP2Id, const qint64 &firstPointId, const qint64 &secondPointId,
                                 Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void   setDialog();
    static void    Create(QSharedPointer<DialogTriangle> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                          VContainer *data);
    static void    Create(const qint64 _id, const QString &pointName, const qint64 &axisP1Id, const qint64 &axisP2Id,
                          const qint64 &firstPointId, const qint64 &secondPointId, const qreal &mx, const qreal &my,
                          VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                          const Document::Documents &parse, Tool::Sources typeCreation);
    static QPointF FindPoint(const QPointF axisP1, const QPointF axisP2, const QPointF firstPoint,
                              const QPointF secondPoint);
    static const QString ToolType;
public slots:
    virtual void   FullUpdateFromFile();
    virtual void   FullUpdateFromGui(int result);
protected:
    virtual void   RemoveReferens();
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void   AddToFile();
private:
    Q_DISABLE_COPY(VToolTriangle)
    qint64         axisP1Id;
    qint64         axisP2Id;
    qint64         firstPointId;
    qint64         secondPointId;
    QSharedPointer<DialogTriangle> dialogTriangle;
};

#endif // VTOOLTRIANGLE_H
