/************************************************************************
 **
 **  @file   vtoolsinglepoint.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
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
 *************************************************************************/

#ifndef VTOOLSINGLEPOINT_H
#define VTOOLSINGLEPOINT_H

#include "vtoolpoint.h"
#include "../../dialogs/dialogsinglepoint.h"

class VToolSinglePoint : public VToolPoint
{
    Q_OBJECT
public:
                 VToolSinglePoint (VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                                   QGraphicsItem * parent = 0 );
    virtual void setDialog();
    static const QString ToolType;
public slots:
    virtual void FullUpdateFromFile();
    virtual void FullUpdateFromGui(int result);
    virtual void ChangedActivDraw(const QString &newName);
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
