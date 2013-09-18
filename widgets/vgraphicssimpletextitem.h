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

#ifndef VGRAPHICSSIMPLETEXTITEM_H
#define VGRAPHICSSIMPLETEXTITEM_H

#include <QGraphicsSimpleTextItem>

class VGraphicsSimpleTextItem : public QObject, public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
                 VGraphicsSimpleTextItem(QGraphicsItem * parent = 0);
                 VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent = 0 );
signals:
    void         NameChangePosition(const QPointF pos);
protected:
    QVariant     itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
};

#endif // VGRAPHICSSIMPLETEXTITEM_H
