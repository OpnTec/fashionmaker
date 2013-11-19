/************************************************************************
 **
 **  @file   vmodelingsplinepath.h
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

#ifndef VMODELINGSPLINEPATH_H
#define VMODELINGSPLINEPATH_H

#include "vmodelingtool.h"
#include <QGraphicsPathItem>
#include "../../dialogs/dialogsplinepath.h"
#include "../../widgets/vcontrolpointspline.h"

/**
 * @brief The VModelingSplinePath class
 */
class VModelingSplinePath:public VModelingTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
                  /**
                   * @brief VModelingSplinePath
                   * @param doc dom document container
                   * @param data
                   * @param id
                   * @param typeCreation
                   * @param parent
                   */
                  VModelingSplinePath(VDomDocument *doc, VContainer *data, qint64 id, const Tool::Sources &typeCreation,
                                      QGraphicsItem * parent = 0);
     /**
      * @brief setDialog
      */
     virtual void setDialog();
     /**
      * @brief Create
      * @param dialog
      * @param doc dom document container
      * @param data
      * @return
      */
     static VModelingSplinePath* Create(QSharedPointer<DialogSplinePath> &dialog, VDomDocument *doc, VContainer *data);
     /**
      * @brief Create
      * @param _id
      * @param path
      * @param doc dom document container
      * @param data
      * @param parse
      * @param typeCreation
      * @return
      */
     static VModelingSplinePath* Create(const qint64 _id, const VSplinePath &path, VDomDocument *doc,
                                        VContainer *data, const Document::Documents &parse,
                                        const Tool::Sources &typeCreation);
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
     * @param pos
     * @param controlPoint
     * @param splinePoint
     */
    void          RefreshLine(const qint32 &indexSpline, SplinePoint::Position pos, const QPointF &controlPoint,
                              const QPointF &splinePoint);
    /**
     * @brief setEnabledPoint
     * @param enable
     */
    void          setEnabledPoint(bool enable);
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void  FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void  FullUpdateFromGui(int result);
    /**
     * @brief ControlPointChangePosition
     * @param indexSpline
     * @param position
     * @param pos
     */
    void          ControlPointChangePosition(const qint32 &indexSpline, SplinePoint::Position position,
                                             const QPointF &pos);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void  contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile
     */
    virtual void  AddToFile();
    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    virtual void  mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent
     * @param event
     */
    virtual void  hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent
     * @param event
     */
    virtual void  hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief RemoveReferens
     */
    virtual void  RemoveReferens();
private:
    /**
     * @brief dialogSplinePath
     */
    QSharedPointer<DialogSplinePath> dialogSplinePath;
    /**
     * @brief controlPoints
     */
    QVector<VControlPointSpline *>   controlPoints;
    /**
     * @brief RefreshGeometry
     */
    void          RefreshGeometry();
    /**
     * @brief AddPathPoint
     * @param domElement
     * @param splPoint
     */
    void          AddPathPoint(QDomElement &domElement, const VSplinePoint &splPoint);
    /**
     * @brief UpdatePathPoint
     * @param node
     * @param path
     */
    void          UpdatePathPoint(QDomNode& node, VSplinePath &path);
    /**
     * @brief CorectControlPoints
     * @param spl
     * @param splPath
     * @param indexSpline
     */
    void          CorectControlPoints(const VSpline &spl, VSplinePath &splPath, const qint32 &indexSpline);
};

#endif // VMODELINGSPLINEPATH_H
