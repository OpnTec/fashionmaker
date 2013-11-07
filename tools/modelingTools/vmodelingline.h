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

#ifndef VMODELINGLINE_H
#define VMODELINGLINE_H

#include "vmodelingtool.h"
#include <QGraphicsLineItem>
#include "../../dialogs/dialogline.h"

class VModelingLine: public VModelingTool, public QGraphicsLineItem
{
    Q_OBJECT
public:
                          VModelingLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint,
                                        qint64 secondPoint, const Tool::Sources &typeCreation,
                                        QGraphicsItem * parent = 0);
    virtual void          setDialog();
    static VModelingLine* Create(QSharedPointer<DialogLine> &dialog, VDomDocument *doc, VContainer *data);
    static VModelingLine* Create(const qint64 &_id, const qint64 &firstPoint, const qint64 &secondPoint,
                                 VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                 const Tool::Sources &typeCreation);
    static const QString  TagName;
    static const QString  ToolType;
public slots:
    virtual void          FullUpdateFromFile();
    virtual void          FullUpdateFromGui(int result);
protected:
    virtual void          contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void          AddToFile();
    virtual void          hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void          hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void          RemoveReferens();
private:
    qint64                firstPoint;
    qint64                secondPoint;
    QSharedPointer<DialogLine> dialogLine;
};

#endif // VMODELINGLINE_H
