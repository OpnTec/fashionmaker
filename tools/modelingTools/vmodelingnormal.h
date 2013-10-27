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

#ifndef VMODELINGNORMAL_H
#define VMODELINGNORMAL_H

#include "vmodelinglinepoint.h"
#include "dialogs/dialognormal.h"

class VModelingNormal : public VModelingLinePoint{
    Q_OBJECT
public:
                            VModelingNormal(VDomDocument *doc, VContainer *data, const qint64 &id,
                                            const QString &typeLine, const QString &formula, const qreal &angle,
                                            const qint64 &firstPointId, const qint64 &secondPointId,
                                            Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void            setDialog();
    static VModelingNormal* Create(QSharedPointer<DialogNormal> &dialog, VDomDocument *doc, VContainer *data);
    static VModelingNormal* Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                                   const qint64 &secondPointId, const QString typeLine, const QString pointName,
                                   const qreal angle, const qreal &mx, const qreal &my, VDomDocument *doc,
                                   VContainer *data, const Document::Documents &parse, Tool::Sources typeCreation);
public slots:
    virtual void            FullUpdateFromFile();
    virtual void            FullUpdateFromGui(int result);
protected:
    virtual void            contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void            AddToFile();
    virtual void            RemoveReferens();
private:
    qint64                  secondPointId;
    QSharedPointer<DialogNormal> dialogNormal;
};

#endif // VMODELINGNORMAL_H
