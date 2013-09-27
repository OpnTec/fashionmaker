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

#ifndef VMODELINGSPLINE_H
#define VMODELINGSPLINE_H

#include "vmodelingtool.h"
#include "xml/vdomdocument.h"
#include <QGraphicsPathItem>
#include "dialogs/dialogspline.h"
#include "widgets/vcontrolpointspline.h"
#include "container/vcontainer.h"
#include "geometry/vsplinepath.h"

class VModelingSpline:public VModelingTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                                   VModelingSpline (VDomDocument *doc, VContainer *data, qint64 id,
                                                    Tool::Enum typeCreation, QGraphicsItem * parent = 0 );
    virtual void setDialog();
    static VModelingSpline* Create(QSharedPointer<DialogSpline> &dialog, VDomDocument *doc,
                                   VContainer *data);
    static VModelingSpline* Create(const qint64 _id, const qint64 &p1, const qint64 &p4, const qreal &kAsm1,
                                   const qreal kAsm2, const qreal &angle1, const qreal &angle2,
                                   const qreal &kCurve, VDomDocument *doc, VContainer *data,
                                   const Document::Documents &parse, Tool::Enum typeCreation);
signals:
    void                           RefreshLine ( const qint32 &indexSpline, SplinePoint::Position position,
                                               const QPointF &controlPoint, const QPointF &splinePoint );
    void                           setEnabledPoint ( bool enable );
public slots:
    virtual void                   FullUpdateFromFile ();
    virtual void                   FullUpdateFromGui ( int result );
    void                           ControlPointChangePosition ( const qint32 &indexSpline,
                                                                SplinePoint::Position position,
                                                                const QPointF pos);
protected:
    virtual void                   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                   AddToFile ();
    virtual void                   mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void                   hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void                   hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    QSharedPointer<DialogSpline>   dialogSpline;
    QVector<VControlPointSpline *> controlPoints;
    void                           RefreshGeometry ();
};

#endif // VMODELINGSPLINE_H
