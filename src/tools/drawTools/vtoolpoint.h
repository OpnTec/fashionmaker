/************************************************************************
 **
 **  @file   vtoolpoint.h
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

#ifndef VTOOLPOINT_H
#define VTOOLPOINT_H

#include "vdrawtool.h"
#include "../../widgets/vgraphicssimpletextitem.h"

/**
 * @brief The VToolPoint class
 */
class VToolPoint: public VDrawTool, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
                            /**
                             * @brief VToolPoint
                             * @param doc dom document container
                             * @param data container with variables
                             * @param id object id in container
                             * @param parent parent object
                             */
                            VToolPoint(VDomDocument *doc, VContainer *data, qint64 id, QGraphicsItem * parent = 0);
    virtual                 ~VToolPoint(){}
    /**
     * @brief TagName
     */
    static const QString    TagName;
public slots:
    /**
     * @brief NameChangePosition
     * @param pos
     */
    void                    NameChangePosition(const QPointF &pos);
    /**
     * @brief ChangedActivDraw
     * @param newName
     */
    virtual void            ChangedActivDraw(const QString &newName);
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void            FullUpdateFromGui(int result) = 0;
    /**
     * @brief ShowTool
     * @param id object id in container
     * @param color
     * @param enable
     */
    virtual void            ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief SetFactor
     * @param factor
     */
    virtual void            SetFactor(qreal factor);
    virtual void            ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief radius
     */
    qreal                   radius;
    /**
     * @brief namePoint
     */
    VGraphicsSimpleTextItem *namePoint;
    /**
     * @brief lineName
     */
    QGraphicsLineItem       *lineName;
    /**
     * @brief UpdateNamePosition
     * @param mx
     * @param my
     */
    virtual void            UpdateNamePosition(qreal mx, qreal my);
    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent
     * @param event
     */
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent
     * @param event
     */
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief RefreshPointGeometry
     * @param point
     */
    virtual void            RefreshPointGeometry(const VPointF &point);
    /**
     * @brief RefreshLine
     */
    void                    RefreshLine();
    virtual QVariant        itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void            keyReleaseEvent(QKeyEvent * event);
private:
    Q_DISABLE_COPY(VToolPoint)
};

#endif // VTOOLPOINT_H
