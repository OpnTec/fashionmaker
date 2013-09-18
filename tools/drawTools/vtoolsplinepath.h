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

#ifndef VTOOLSPLINEPATH_H
#define VTOOLSPLINEPATH_H

#include "vdrawtool.h"
#include "xml/vdomdocument.h"
#include <QGraphicsPathItem>
#include "dialogs/dialogsplinepath.h"
#include "widgets/vcontrolpointspline.h"
#include "container/vcontainer.h"

class VToolSplinePath:public VDrawTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                                     VToolSplinePath(VDomDocument *doc, VContainer *data, qint64 id,
                                                     Tool::Enum typeCreation,
                                                     QGraphicsItem * parent = 0);
     virtual void setDialog();
     static void Create(QSharedPointer<DialogSplinePath> &dialog, VMainGraphicsScene  *scene,
                        VDomDocument *doc, VContainer *data);
     static void Create(const qint64 _id, const VSplinePath &path, VMainGraphicsScene  *scene,
                        VDomDocument *doc, VContainer *data, Document::Enum parse, Tool::Enum typeCreation);
signals:
    void                             RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos,
                                                 const QPointF &controlPoint, const QPointF &splinePoint);
    void                             setEnabledPoint(bool enable);
public slots:
    virtual void                     FullUpdateFromFile();
    virtual void                     FullUpdateFromGui(int result);
    void                             ControlPointChangePosition(const qint32 &indexSpline,
                                                                SplinePoint::Position position,
                                                                const QPointF pos);
    virtual void                     ChangedActivDraw(const QString newName);
    virtual void                     ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
protected:
    virtual void                     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                     AddToFile();
    virtual void                     mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                     hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    QSharedPointer<DialogSplinePath> dialogSplinePath;
    QVector<VControlPointSpline *>   controlPoints;
    void                             RefreshGeometry();
    void                             AddPathPoint(QDomElement &domElement, const VSplinePoint &splPoint);
    void                             UpdatePathPoint(QDomNode& node, VSplinePath &path);
    void                             CorectControlPoints(const VSpline &spl, VSplinePath &splPath,
                                                         const qint32 &indexSpline);
};

#endif // VTOOLSPLINEPATH_H
