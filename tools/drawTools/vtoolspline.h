/************************************************************************
 **
 **  @file vtoolspline.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
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

#ifndef VTOOLSPLINE_H
#define VTOOLSPLINE_H

#include "vdrawtool.h"
#include <QGraphicsPathItem>
#include "../../dialogs/dialogspline.h"
#include "../../widgets/vcontrolpointspline.h"
#include "../../geometry/vsplinepath.h"

class VToolSpline:public VDrawTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                 VToolSpline (VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                              QGraphicsItem * parent = 0 );
    virtual void setDialog();
    static void  Create(QSharedPointer<DialogSpline> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    static void  Create(const qint64 _id, const qint64 &p1, const qint64 &p4, const qreal &kAsm1,
                        const qreal kAsm2, const qreal &angle1, const qreal &angle2, const qreal &kCurve,
                        VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    static const QString TagName;
    static const QString ToolType;
signals:
    void         RefreshLine ( const qint32 &indexSpline, SplinePoint::Position position, const QPointF &controlPoint,
                               const QPointF &splinePoint );
    void         setEnabledPoint ( bool enable );
public slots:
    virtual void FullUpdateFromFile ();
    virtual void FullUpdateFromGui ( int result );
    void         ControlPointChangePosition (const qint32 &indexSpline, const SplinePoint::Position &position,
                                             const QPointF &pos);
    virtual void ChangedActivDraw ( const QString &newName );
    virtual void ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    virtual void SetFactor(qreal factor);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void AddToFile ();
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void RemoveReferens();
private:
    QSharedPointer<DialogSpline>   dialogSpline;
    QVector<VControlPointSpline *> controlPoints;
    void         RefreshGeometry ();
};

#endif // VTOOLSPLINE_H
