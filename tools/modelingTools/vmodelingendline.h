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

#ifndef VMODELINGENDLINE_H
#define VMODELINGENDLINE_H

#include <QSharedPointer>
#include "vmodelinglinepoint.h"
#include "dialogs/dialogendline.h"

class VModelingEndLine : public VModelingLinePoint
{
    Q_OBJECT
public:
                      VModelingEndLine(VDomDocument *doc, VContainer *data, const qint64 &id,
                                   const QString &typeLine, const QString &formula, const qreal &angle,
                                   const qint64 &basePointId, Tool::Sources typeCreation,
                                   QGraphicsItem * parent = 0);
    virtual void setDialog();
    static VModelingEndLine* Create(QSharedPointer<DialogEndLine> &dialog, VDomDocument *doc,
                                    VContainer *data);
    static VModelingEndLine* Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                                    const QString &formula, const qreal &angle, const qint64 &basePointId,
                                    const qreal &mx, const qreal &my, VDomDocument *doc, VContainer *data,
                                    const Document::Documents &parse, Tool::Sources typeCreation);
public slots:
    virtual void      FullUpdateFromFile();
    virtual void      FullUpdateFromGui(int result);
protected:
    virtual void      contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void      AddToFile();
private:
    QSharedPointer<DialogEndLine> dialogEndLine;
};

#endif // VMODELINGENDLINE_H
