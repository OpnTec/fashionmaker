/************************************************************************
 **
 **  @file   vtoolline.h
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

#ifndef VTOOLLINE_H
#define VTOOLLINE_H

#include "vdrawtool.h"
#include <QGraphicsLineItem>
#include "../../dialogs/dialogline.h"

class VToolLine: public VDrawTool, public QGraphicsLineItem
{
    Q_OBJECT
public:
                 VToolLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint,
                           qint64 secondPoint, const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    virtual void setDialog();
    static void  Create(QSharedPointer<DialogLine> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    static void  Create(const qint64 &_id, const qint64 &firstPoint, const qint64 &secondPoint,
                        VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    static const QString TagName;
public slots:
    virtual void FullUpdateFromFile();
    virtual void ChangedActivDraw(const QString &newName);
    virtual void FullUpdateFromGui(int result);
    virtual void ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    virtual void SetFactor(qreal factor);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void AddToFile();
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void RemoveReferens();
private:
    qint64       firstPoint;
    qint64       secondPoint;
    QSharedPointer<DialogLine> dialogLine;
    void         RefreshGeometry();
};

#endif // VTOOLLINE_H
