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

#ifndef VTOOLALONGLINE_H
#define VTOOLALONGLINE_H

#include "vtoollinepoint.h"
#include "dialogs/dialogalongline.h"

class VToolAlongLine : public VToolLinePoint{
    Q_OBJECT
public:
                 VToolAlongLine(VDomDocument *doc, VContainer *data, qint64 id, const QString &formula,
                                const qint64 &firstPointId, const qint64 &secondPointId, const QString &typeLine,
                                Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void  Create(QSharedPointer<DialogAlongLine> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    static void  Create(const qint64 _id, const QString &pointName, const QString &typeLine, const QString &formula,
                        const qint64 &firstPointId, const qint64 &secondPointId, const qreal &mx, const qreal &my,
                        VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, Tool::Sources typeCreation);
public slots:
    virtual void FullUpdateFromFile();
    virtual void FullUpdateFromGui(int result);
    virtual void SetFactor(qreal factor);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void AddToFile();
    virtual void RemoveReferens();
private:
    qint64       secondPointId;
    QSharedPointer<DialogAlongLine> dialogAlongLine; 
};

#endif // VTOOLALONGLINE_H
