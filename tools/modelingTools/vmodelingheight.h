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

#ifndef VMODELINGHEIGHT_H
#define VMODELINGHEIGHT_H

#include "vmodelinglinepoint.h"
#include "dialogs/dialogheight.h"

class VModelingHeight : public VModelingLinePoint{
    Q_OBJECT
public:
                            VModelingHeight(VDomDocument *doc, VContainer *data, const qint64 &id,
                                            const QString &typeLine, const qint64 &basePointId, const qint64 &p1LineId,
                                            const qint64 &p2LineId, Tool::Sources typeCreation,
                                            QGraphicsItem * parent = 0);
    virtual void            setDialog();
    static VModelingHeight* Create(QSharedPointer<DialogHeight> &dialog, VDomDocument *doc, VContainer *data);
    static VModelingHeight* Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                                   const qint64 &basePointId, const qint64 &p1LineId, const qint64 &p2LineId,
                                   const qreal &mx, const qreal &my, VDomDocument *doc, VContainer *data,
                                   const Document::Documents &parse, Tool::Sources typeCreation);
    static const QString ToolType;
public slots:
    virtual void            FullUpdateFromFile();
    virtual void            FullUpdateFromGui(int result);
protected:
    virtual void            contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void            AddToFile();
private:
    QSharedPointer<DialogHeight> dialogHeight;
    qint64                  p1LineId;
    qint64                  p2LineId;
};

#endif // VMODELINGHEIGHT_H
