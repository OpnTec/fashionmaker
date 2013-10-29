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

#ifndef VTOOLPOINTOFCONTACT_H
#define VTOOLPOINTOFCONTACT_H

#include "vtoolpoint.h"
#include "dialogs/dialogpointofcontact.h"

class VToolPointOfContact : public VToolPoint{
public:
                   VToolPointOfContact(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &radius, const qint64 &center,
                                       const qint64 &firstPointId, const qint64 &secondPointId,
                                       Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void   setDialog();
    static QPointF FindPoint(const qreal &radius, const QPointF &center, const QPointF &firstPoint,
                             const QPointF &secondPoint);
    static void    Create(QSharedPointer<DialogPointOfContact> &dialog, VMainGraphicsScene  *scene,
                          VDomDocument *doc, VContainer *data);
    static void    Create(const qint64 _id, const QString &radius, const qint64 &center,
                          const qint64 &firstPointId, const qint64 &secondPointId, const QString &pointName,
                          const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc,
                          VContainer *data, const Document::Documents &parse, Tool::Sources typeCreation);
    static const QString ToolType;
public slots:
    virtual void   FullUpdateFromFile();
    virtual void   FullUpdateFromGui(int result);
    virtual void   SetFactor(qreal factor);
protected:
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void   AddToFile();
    virtual void   RemoveReferens();
private:
    QString        radius;
    qint64         center;
    qint64         firstPointId;
    qint64         secondPointId;
    QSharedPointer<DialogPointOfContact> dialogPointOfContact;
};

#endif // VTOOLPOINTOFCONTACT_H
