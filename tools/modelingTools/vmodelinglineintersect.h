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

#ifndef VMODELINGLINEINTERSECT_H
#define VMODELINGLINEINTERSECT_H

#include "vmodelingpoint.h"
#include "dialogs/dialoglineintersect.h"

class VModelingLineIntersect:public VModelingPoint{
    Q_OBJECT
public:
                                   VModelingLineIntersect(VDomDocument *doc, VContainer *data, const qint64 &id,
                                                          const qint64 &p1Line1, const qint64 &p2Line1,
                                                          const qint64 &p1Line2, const qint64 &p2Line2,
                                                          Tool::Sources typeCreation, QGraphicsItem * parent = 0);
    virtual void                   setDialog();
    static VModelingLineIntersect* Create(QSharedPointer<DialogLineIntersect> &dialog, VDomDocument *doc,
                                          VContainer *data);
    static VModelingLineIntersect* Create(const qint64 _id, const qint64 &p1Line1Id, const qint64 &p2Line1Id,
                                          const qint64 &p1Line2Id, const qint64 &p2Line2Id, const QString &pointName,
                                          const qreal &mx, const qreal &my, VDomDocument *doc, VContainer *data,
                                          const Document::Documents &parse, Tool::Sources typeCreation);
public slots:
    virtual void                   FullUpdateFromFile();
    virtual void                   FullUpdateFromGui(int result);
protected:
    virtual void                   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                   AddToFile();
    virtual void                   RemoveReferens();
private:
    qint64                         p1Line1;
    qint64                         p2Line1;
    qint64                         p1Line2;
    qint64                         p2Line2;
    QSharedPointer<DialogLineIntersect> dialogLineIntersect;
};

#endif // VMODELINGLINEINTERSECT_H
