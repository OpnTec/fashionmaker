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
 * @brief The VToolSpline class tool for creation spline. I mean bezier curve.
 */
class VToolSpline:public VDrawTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                 /**
                  * @brief VToolSpline constructor.
                  * @param doc dom document container.
                  * @param data container with variables.
                  * @param id object id in container.
                  * @param typeCreation way we create this tool.
                  * @param parent parent object.
                  */
                 VToolSpline (VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                              QGraphicsItem * parent = 0 );
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog();
    /**
     * @brief Create help create tool from GUI.
     * @param dialog dialog.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     */
    static void  Create(QSharedPointer<DialogSpline> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                        VContainer *data);
    /**
     * @brief Create help create tool.
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param p1 id first spline point.
     * @param p4 id last spline point.
     * @param kAsm1 coefficient of length first control line.
     * @param kAsm2 coefficient of length second control line.
     * @param angle1 angle from first point to first control point.
     * @param angle2 angle from second point to second control point.
     * @param kCurve coefficient of curvature spline.
     * @param scene pointer to scene.
     * @param doc dom document container.
     * @param data container with variables.
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void  Create(const qint64 _id, const qint64 &p1, const qint64 &p4, const qreal &kAsm1,
                        const qreal kAsm2, const qreal &angle1, const qreal &angle2, const qreal &kCurve,
                        VMainGraphicsScene  *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    static const QString TagName;
    static const QString ToolType;
signals:
    /**
     * @brief RefreshLine refresh control line.
     * @param indexSpline position spline in spline list.
     * @param position position point in spline.
     * @param controlPoint new position control point.
     * @param splinePoint new position spline point.
     */
    void             RefreshLine ( const qint32 &indexSpline, SplinePoint::Position position, const QPointF &controlPoint,
                                   const QPointF &splinePoint );
    /**
     * @brief setEnabledPoint disable control points.
     * @param enable enable or diasable points.
     */
    void             setEnabledPoint ( bool enable );
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void     FullUpdateFromFile ();
    /**
     * @brief FullUpdateFromGui  refresh tool data from change options.
     * @param result result working dialog.
     */
    virtual void     FullUpdateFromGui ( int result );
    /**
     * @brief ControlPointChangePosition handle change position control point.
     * @param indexSpline position spline in spline list.
     * @param position position point in spline.
     * @param pos new position.
     */
    void             ControlPointChangePosition (const qint32 &indexSpline, const SplinePoint::Position &position,
                                                 const QPointF &pos);
    /**
     * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
     * @param newName new name active pattern peace.
     */
    virtual void     ChangedActivDraw ( const QString &newName );
    /**
     * @brief ShowTool highlight tool.
     * @param id object id in container
     * @param color highlight color.
     * @param enable enable or disable highlight.
     */
    virtual void     ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief SetFactor set current scale factor of scene.
     * @param factor scene scale factor.
     */
    virtual void     SetFactor(qreal factor);
protected:
    /**
     * @brief contextMenuEvent handle context menu events.
     * @param event context menu event.
     */
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void     AddToFile ();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void     RefreshDataInFile();
    /**
     * @brief mouseReleaseEvent  handle mouse release events.
     * @param event context menu event.
     */
    virtual void     mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent handle hover move events.
     * @param event hover move event.
     */
    virtual void     hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent handle hover leave events.
     * @param event hover leave event.
     */
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void     RemoveReferens();
    /**
     * @brief itemChange hadle item change.
     * @param change change.
     * @param value value.
     * @return value.
     */
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value );
    /**
     * @brief keyReleaseEvent handle key release events.
     * @param event key release event.
     */
    virtual void     keyReleaseEvent(QKeyEvent * event);
private:
    /**
     * @brief dialogSpline dialog.
     */
    QSharedPointer<DialogSpline>   dialogSpline;
    /**
     * @brief controlPoints list pointers of control points.
     */
    QVector<VControlPointSpline *> controlPoints;
    /**
     * @brief RefreshGeometry  refresh item on scene.
     */
    void              RefreshGeometry ();
};

#endif // VTOOLSPLINE_H
