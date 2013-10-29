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

#ifndef VMODELINGBISECTOR_H
#define VMODELINGBISECTOR_H

#include "vmodelinglinepoint.h"
#include "dialogs/dialogbisector.h"

class VModelingBisector : public VModelingLinePoint{
    Q_OBJECT
public:
                              VModelingBisector(VDomDocument *doc, VContainer *data, const qint64 &id,
                                                const QString &typeLine, const QString &formula,
                                                const qint64 &firstPointId, const qint64 &secondPointId,
                                                const qint64 &thirdPointId, Tool::Sources typeCreation,
                                                QGraphicsItem * parent = 0);
    virtual void              setDialog();
    static VModelingBisector* Create(QSharedPointer<DialogBisector> &dialog, VDomDocument *doc, VContainer *data);
    static VModelingBisector* Create(const qint64 _id, const QString &formula, const qint64 &firstPointId,
                                     const qint64 &secondPointId, const qint64 &thirdPointId, const QString &typeLine,
                                     const QString &pointName, const qreal &mx, const qreal &my, VDomDocument *doc,
                                     VContainer *data, const Document::Documents &parse, Tool::Sources typeCreation);
    static const QString ToolType;
public slots:
    virtual void              FullUpdateFromFile();
    virtual void              FullUpdateFromGui(int result);
protected:
    virtual void              contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void              AddToFile();
    virtual void              RemoveReferens();
private:
    qint64                    firstPointId;
    qint64                    thirdPointId;
    QSharedPointer<DialogBisector> dialogBisector;
};

#endif // VMODELINGBISECTOR_H
