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

class VPointF;
class VGraphicsSimpleTextItem;

/**
 * @brief The VToolPoint class parent for all tools what create points.
 */
class VToolPoint: public VDrawTool, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VToolPoint(VPattern *doc, VContainer *data, quint32 id, QGraphicsItem * parent = nullptr);
    virtual ~VToolPoint(){}
    static const QString    TagName;
public slots:
    void                    NameChangePosition(const QPointF &pos);
    virtual void            ChangedActivDraw(const QString &newName);
    virtual void            ShowTool(quint32 id, Qt::GlobalColor color, bool enable);
    virtual void            SetFactor(qreal factor);
    virtual void            ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /** @brief radius radius circle. */
    qreal                   radius;

    /** @brief namePoint point label. */
    VGraphicsSimpleTextItem *namePoint;

    /** @brief lineName line what we see if label moved too away from point. */
    QGraphicsLineItem       *lineName;

    virtual void            UpdateNamePosition(qreal mx, qreal my);
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void            hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            RefreshPointGeometry(const VPointF &point);
    void                    RefreshLine();
    virtual QVariant        itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void            keyReleaseEvent(QKeyEvent * event);
private:
    Q_DISABLE_COPY(VToolPoint)
};

#endif // VTOOLPOINT_H
