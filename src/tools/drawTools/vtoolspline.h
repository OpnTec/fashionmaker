/************************************************************************
 **
 **  @file   vtoolspline.h
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

#ifndef VTOOLSPLINE_H
#define VTOOLSPLINE_H

#include "vdrawtool.h"
#include <QGraphicsPathItem>
#include "../../dialogs/dialogspline.h"
#include "../../widgets/vcontrolpointspline.h"
#include "../../geometry/vsplinepath.h"

/**
 * @brief The VToolSpline class
 */
class VToolSpline:public VDrawTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                 /**
                  * @brief VToolSpline
                  * @param doc dom document container
                  * @param data
                  * @param id
                  * @param typeCreation
                  * @param parent
                  */
                 VToolSpline (VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                              QGraphicsItem * parent = 0 );
    /**
     * @brief setDialog
     */
    virtual void setDialog();
    /**
     * @brief Create
     * @param dialog
     * @param scene
     * @param doc dom document container
     * @param data
     */
    static void  Create(QSharedPointer<DialogSpline> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param p1
     * @param p4
     * @param kAsm1
     * @param kAsm2
     * @param angle1
     * @param angle2
     * @param kCurve
     * @param scene
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     */
    static void  Create(const qint64 _id, const qint64 &p1, const qint64 &p4, const qreal &kAsm1,
                        const qreal kAsm2, const qreal &angle1, const qreal &angle2, const qreal &kCurve,
                        VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief TagName
     */
    static const QString TagName;
    /**
     * @brief ToolType
     */
    static const QString ToolType;
signals:
    /**
     * @brief RefreshLine
     * @param indexSpline
     * @param position
     * @param controlPoint
     * @param splinePoint
     */
    void         RefreshLine ( const qint32 &indexSpline, SplinePoint::Position position, const QPointF &controlPoint,
                               const QPointF &splinePoint );
    /**
     * @brief setEnabledPoint
     * @param enable
     */
    void         setEnabledPoint ( bool enable );
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void FullUpdateFromFile ();
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void FullUpdateFromGui ( int result );
    /**
     * @brief ControlPointChangePosition
     * @param indexSpline
     * @param position
     * @param pos
     */
    void         ControlPointChangePosition (const qint32 &indexSpline, const SplinePoint::Position &position,
                                             const QPointF &pos);
    /**
     * @brief ChangedActivDraw
     * @param newName
     */
    virtual void ChangedActivDraw ( const QString &newName );
    /**
     * @brief ShowTool
     * @param id
     * @param color
     * @param enable
     */
    virtual void ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief SetFactor
     * @param factor
     */
    virtual void SetFactor(qreal factor);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile
     */
    virtual void AddToFile ();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent
     * @param event
     */
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent
     * @param event
     */
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief RemoveReferens
     */
    virtual void RemoveReferens();
private:
    /**
     * @brief dialogSpline
     */
    QSharedPointer<DialogSpline>   dialogSpline;
    /**
     * @brief controlPoints
     */
    QVector<VControlPointSpline *> controlPoints;
    /**
     * @brief RefreshGeometry
     */
    void         RefreshGeometry ();
};

#endif // VTOOLSPLINE_H
