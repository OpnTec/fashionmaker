/************************************************************************
 **
 **  @file   vgraphicssimpletextitem.h
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

#ifndef VGRAPHICSSIMPLETEXTITEM_H
#define VGRAPHICSSIMPLETEXTITEM_H

#include <QGraphicsSimpleTextItem>

/**
 * @brief The VGraphicsSimpleTextItem class pointer label.
 */
class VGraphicsSimpleTextItem : public QObject, public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
                 /**
                  * @brief VGraphicsSimpleTextItem default constructor.
                  * @param parent parent object.
                  */
                 VGraphicsSimpleTextItem(QGraphicsItem * parent = nullptr);
                 /**
                  * @brief VGraphicsSimpleTextItem constructor.
                  * @param text text.
                  * @param parent parent object.
                  */
                 VGraphicsSimpleTextItem( const QString & text, QGraphicsItem * parent = nullptr );
    /**
     * @brief FontSize return label font size.
     * @return font size.
     */
    qint32       FontSize()const {return fontSize;}
signals:
    /**
     * @brief NameChangePosition emit when label change position.
     * @param pos new posotion.
     */
    void         NameChangePosition(const QPointF pos);
    /**
     * @brief ShowContextMenu emit when need show tool context menu.
     * @param event context menu event.
     */
    void         ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief itemChange handle item change.
     * @param change change.
     * @param value value.
     * @return value.
     */
    QVariant     itemChange ( GraphicsItemChange change, const QVariant &value );
    /**
     * @brief hoverMoveEvent handle hover move events.
     * @param event hover move event.
     */
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent handle hover leave events.
     * @param event hover leave event.
     */
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief contextMenuEvent handle context menu events.
     * @param event context menu event.
     */
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
private:
    /**
     * @brief fontSize label font size.
     */
    qint32       fontSize;
};

#endif // VGRAPHICSSIMPLETEXTITEM_H
