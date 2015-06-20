/************************************************************************
 **
 **  @file   vtoolsinglepoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VTOOLSINGLEPOINT_H
#define VTOOLSINGLEPOINT_H

#include "../vabstractpoint.h"
#include <QGraphicsEllipseItem>

class VPointF;
class VGraphicsSimpleTextItem;

/**
 * @brief The VToolSinglePoint class parent for all tools what create points.
 */
class VToolSinglePoint: public VAbstractPoint, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VToolSinglePoint(VAbstractPattern *doc, VContainer *data, quint32 id, QGraphicsItem * parent = nullptr);
    virtual ~VToolSinglePoint();

    virtual int   type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::SinglePoint)};

    virtual void            paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QString                 name() const;
    void                    setName(const QString &name);

    void                    SetEnabled(bool enabled);

public slots:
    void                    NameChangePosition(const QPointF &pos);
    virtual void            SetFactor(qreal factor);
    virtual void            Disable(bool disable, const QString &namePP);
    virtual void            EnableToolMove(bool move);
    void                    PointChoosed();
    virtual void            FullUpdateFromFile();
protected:
    /** @brief radius radius circle. */
    qreal                   radius;

    /** @brief namePoint point label. */
    VGraphicsSimpleTextItem *namePoint;

    /** @brief lineName line what we see if label moved too away from point. */
    QGraphicsLineItem       *lineName;

    virtual void            UpdateNamePosition();
    virtual void            mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void            hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void            RefreshPointGeometry(const VPointF &point);
    virtual void            RefreshLine();
    virtual QVariant        itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void            keyReleaseEvent(QKeyEvent * event);
    virtual void            contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void            SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
private:
    Q_DISABLE_COPY(VToolSinglePoint)
};

#endif // VTOOLSINGLEPOINT_H
