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

#ifndef VTOOLENDLINE_H
#define VTOOLENDLINE_H

#include "vtoollinepoint.h"
#include "dialogs/dialogendline.h"

class VToolEndLine : public VToolLinePoint{
    Q_OBJECT
public:
                 VToolEndLine(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &typeLine,
                              const QString &formula, const qreal &angle, const qint64 &basePointId,
                              Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void  Create(QSharedPointer<DialogEndLine> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    static void  Create(const qint64 _id, const QString &pointName, const QString &typeLine,
                        const QString &formula, const qreal &angle, const qint64 &basePointId, const qreal &mx,
                        const qreal &my, VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, Tool::Sources typeCreation);
public slots:
    virtual void FullUpdateFromFile();
    virtual void FullUpdateFromGui(int result);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void AddToFile();
private:
    QSharedPointer<DialogEndLine> dialogEndLine;
};

#endif // VTOOLENDLINE_H
