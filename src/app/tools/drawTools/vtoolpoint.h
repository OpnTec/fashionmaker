/************************************************************************
 **
 **  @file   vtoolpoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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
 * @brief The VToolPoint class parent for all tools what create points.
 */
class VToolPoint: public VDrawTool, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
                            /**
                             * @brief VToolPoint constructor.
                             * @param doc dom document container.
                             * @param data container with variables.
                             * @param id object id in container.
                             * @param parent parent object.
                             */
                            VToolPoint(VPattern *doc, VContainer *data, quint32 id, QGraphicsItem * parent = nullptr);
    virtual                 ~VToolPoint(){}
    static const QString    TagName;
public slots:
    /**
     * @brief NameChangePosition handle change posion point label.
     * @param pos new position.
     */
    void                    NameChangePosition(const QPointF &pos);
    /**
     * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
     * @param newName new name active pattern peace.
     */
    virtual void            ChangedActivDraw(const QString &newName);
    /**
     * @brief ShowTool  highlight tool.
     * @param id object id in container.
     * @param color highlight color.
     * @param enable enable or disable highlight.
     */
    virtual void            ShowTool(quint32 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief SetFactor set current scale factor of scene.
     * @param factor scene scale factor.
     */
    virtual void            SetFactor(qreal factor);
    /**
     * @brief ShowContextMenu show context menu.
     * @param event context menu event.
     */
    virtual void            ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief radius radius circle.
     */
    qreal                   radius;
    /**
     * @brief namePoint point label.
     */
    VGraphicsSimpleTextItem *namePoint;
    /**
     * @brief lineName line what we see if label moved too away from point.
     */
    QGraphicsLineItem       *lineName;
    /**
     * @brief UpdateNamePosition save new position label to the pattern file.
     * @param mx label bias x axis.
     * @param my label bias y axis.
     */
    virtual void            UpdateNamePosition(qreal mx, qreal my);
    /**
     * @brief mouseReleaseEvent  handle mouse release events.
     * @param event mouse release event.
     */
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /**
     * @brief hoverMoveEvent handle hover move events.
     * @param event hover move event.
     */
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent handle hover leave events.
     * @param event hover leave event.
     */
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief RefreshPointGeometry refresh point on scene.
     * @param point point.
     */
    virtual void            RefreshPointGeometry(const VPointF &point);
    /**
     * @brief RefreshLine refresh line to label on scene.
     */
    void                    RefreshLine();
    /**
     * @brief itemChange hadle item change.
     * @param change change.
     * @param value value.
     * @return value.
     */
    virtual QVariant        itemChange ( GraphicsItemChange change, const QVariant &value );
    /**
     * @brief keyReleaseEvent handle key release events.
     * @param event key release event.
     */
    virtual void            keyReleaseEvent(QKeyEvent * event);
private:
    Q_DISABLE_COPY(VToolPoint)
};

#endif // VTOOLPOINT_H
