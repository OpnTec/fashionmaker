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

#ifndef VTOOLSINGLEPOINT_H
#define VTOOLSINGLEPOINT_H

#include "dialogs/dialogsinglepoint.h"
#include "vtoolpoint.h"

class VToolSinglePoint : public VToolPoint{
    Q_OBJECT
public:
                 VToolSinglePoint (VDomDocument *doc, VContainer *data, qint64 id, Tool::Sources typeCreation,
                                   QGraphicsItem * parent = 0 );
    virtual void setDialog();
public slots:
    virtual void FullUpdateFromFile();
    virtual void FullUpdateFromGui(int result);
    virtual void ChangedActivDraw(const QString newName);
    virtual void SetFactor(qreal factor);
signals:
    void         FullUpdateTree();
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void AddToFile();
    QVariant     itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void decrementReferens();
private:
    QSharedPointer<DialogSinglePoint> dialogSinglePoint;
};

#endif // VTOOLSINGLEPOINT_H
