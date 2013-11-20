/************************************************************************
 **
 **  @file   vgraphicssimpletextitem.h
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

#ifndef VGRAPHICSSIMPLETEXTITEM_H
#define VGRAPHICSSIMPLETEXTITEM_H

#include <QGraphicsSimpleTextItem>

/**
 * @brief The VGraphicsSimpleTextItem class
 */
class VGraphicsSimpleTextItem : public QObject, public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
                 /**
                  * @brief VGraphicsSimpleTextItem
                  * @param parent
                  */
                 VGraphicsSimpleTextItem(QGraphicsItem * parent = 0);
                 /**
                  * @brief VGraphicsSimpleTextItem
                  * @param text
                  * @param parent
                  */
                 VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent = 0 );
    /**
     * @brief FontSize
     * @return
     */
    qint32       FontSize()const {return fontSize;}
signals:
    /**
     * @brief NameChangePosition
     * @param pos
     */
    void         NameChangePosition(const QPointF pos);
protected:
    /**
     * @brief itemChange
     * @param change
     * @param value
     * @return
     */
    QVariant     itemChange ( GraphicsItemChange change, const QVariant &value );
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
private:
    /**
     * @brief fontSize
     */
    qint32       fontSize;
};

#endif // VGRAPHICSSIMPLETEXTITEM_H
