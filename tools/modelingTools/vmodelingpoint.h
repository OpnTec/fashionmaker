/************************************************************************
 **
 **  @file   vmodelingpoint.h
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

#ifndef VMODELINGPOINT_H
#define VMODELINGPOINT_H

#include "vmodelingtool.h"
#include "../../widgets/vgraphicssimpletextitem.h"

/**
 * @brief The VModelingPoint class
 */
class VModelingPoint: public VModelingTool, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
                            /**
                             * @brief VModelingPoint
                             * @param doc dom document container
                             * @param data
                             * @param id
                             * @param parent
                             */
                            VModelingPoint(VDomDocument *doc, VContainer *data, qint64 id, QGraphicsItem * parent = 0);
    virtual                 ~VModelingPoint() {}
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
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void            FullUpdateFromGui(int result) = 0;
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
private:
    Q_DISABLE_COPY(VModelingPoint)
};

#endif // VMODELINGPOINT_H
